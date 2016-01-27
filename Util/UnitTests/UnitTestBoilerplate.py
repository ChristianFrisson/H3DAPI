from H3DInterface import *
from H3DUtils import *

import sys
import os
from Queue import Queue, Empty
from inspect import getmembers, isfunction
import operator
from importlib import import_module



timer_callback = TimerCallback()

class UnitTestHelper :
  def __init__( self, early_shutdown_file, output_file_prefix, screenshot_filename_prefix):
    self.test_funcs = Queue()
    self.screenshot_queue = Queue()
    self.early_shutdown_file = early_shutdown_file
    self.output_file_prefix = output_file_prefix
    self.screenshot_counter = 0    
#    self.screenshot_queue.put("Startup")
    self.fps_counter = StoreFPS()
    self.screenshot_filename_prefix = screenshot_filename_prefix
    self.validation_file = output_file_prefix + "/validation.txt"
    self.last_func = None
    self.customPrintHelper = None
    temp = open(self.validation_file, 'w')
    temp.flush()
    temp.close()


  def addTests(self, funclist):
    for func in funclist:
      self.test_funcs.put(func)

  def doTesting(self):
    if not self.last_func is None:
      try:
        for validation in self.last_func.validation:
          validation['post'](self, validation, self.validation_file)
      except Exception as e:
        print str(e)
   
    try: # If there're more test functions queued up they'll be called, otherwise we shut down.
      func_data = self.test_funcs.get(False)
      self.last_step_name = func_data[0]
      try:
        f = open(self.validation_file, 'a')
        f.write(self.last_step_name + '\n')
        f.flush()
        f.close()
      except Exception as E:
        print str(e)
      func = func_data[1]
      self.last_func = func_data[1]
      start_time = None
      run_time = None
      for validation in func.validation:
        if (not (start_time is None) and (hasattr(validation, 'start_time') or (start_time < validation['start_time']))):
          start_time = validation['start_time']
    
      if not(start_time is None):
        timer_callback.addCallback(time.getValue()+start_time, UnitTestHelper.startFuncDelayed, (self,func, run_time))
      else:
        try:
          for validation in func.validation:
            validation['init'](self, validation, self.validation_file)
        except Exception as e:
          print(str(e))
        try:
          func()
        except Exception as e:
          print str(e)
        if(not (run_time is None)):
          timer_callback.addCallback(time.getValue()+run_time, UnitTestHelper.doTesting, (self,))
        else:
          timer_callback.addCallback(time.getValue()+1, UnitTestHelper.doTesting, (self,))          
      return
    except Exception as e:
      shutdown_file = open( self.early_shutdown_file, 'w' )
      shutdown_file.write( "OK" )
      shutdown_file.flush()
      shutdown_file.close()
      throwQuitAPIException()

  def startFuncDelayed(self, func, run_time):
      try:
        for validation in func.validation:
          validation['init'](self, validation, self.validation_file)
      except Exception as e:
        print(str(e))
      try:
        func()
      except Exception as e:
        print str(e)

      if(not (run_time is None)):
        timer_callback.addCallback(time.getValue()+run_time, UnitTestHelper.doTesting, (self,))
      else:
        timer_callback.addCallback(time.getValue()+0.5, UnitTestHelper.doTesting, (self,))          

  def printCustom(self, value):
    if self.customPrintHelper is None:
      print "Error: Test script called printCustom from a function that does not have the @custom decorator"
    else:
      self.customPrintHelper(self, value)




class StoreFPS( AutoUpdate( SFFloat ) ):
  def __init__( self):
    AutoUpdate( SFFloat ).__init__(self)
    self.running = False

  def update( self, event ):
    if self.running:
      self.fps_string = self.fps_string + str(event.getValue()) + " "
    return 0

  def start( self ):
    self.fps_string = ""
    self.running = True
    scene = getCurrentScenes()[0]
    scene.frameRate.routeNoEvent( self )
    scene = None

  def stop( self ):
    self.running = False
    return self.fps_string


TestCaseScriptFolder = getNamedNode('TestCaseScriptFolder').getField('value').getValueAsString().replace('"', '')
TestCaseDefFolder = getNamedNode('TestCaseDefFolder').getField('value').getValueAsString().replace('"', '')
TestBaseFolder = getNamedNode('TestBaseFolder').getField('value').getValueAsString().replace('"', '')
sys.path.append(TestBaseFolder) # This is so we can properly import from UnitTestUtil.py
TestCaseScriptFilename = getNamedNode('TestCaseScriptFilename').getField('value').getValueAsString().replace('"', '')
sys.path.append(TestCaseScriptFolder)
TestCaseName = getNamedNode('TestCaseName').getField('value').getValueAsString().replace('"', '')
StartTime = getNamedNode('StartTime').getField('value').getValue()[0]
res = import_module(TestCaseScriptFilename)
res.__scriptnode__ = globals()['__scriptnode__']   

# import all the functions that have our validator decorators attached. We identify them by the presence of a validation array.
# the result is a list of tuples containing (function name, function address), so we sort by the latter to ensure the test functions will be executed in the same order as they appear in the file
testfunctions_list = [o for o in getmembers(res) if isfunction(o[1]) and hasattr(o[1], "validation")]
testfunctions_list.sort(key=operator.itemgetter(1))

testHelper = UnitTestHelper(TestBaseFolder+"/test_complete", os.path.abspath(os.path.join(TestCaseDefFolder, "output").replace("\\", '/')), TestCaseName + '_')
testHelper.addTests(testfunctions_list)
res.printCustom = testHelper.printCustom

timer_callback.addCallback(time.getValue()+StartTime, UnitTestHelper.doTesting, (testHelper,))       