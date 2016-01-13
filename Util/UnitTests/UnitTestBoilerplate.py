import H3DInterface
import H3DUtils
from H3DInterface import *
from H3DUtils import *
from Queue import Queue, Empty
import sys
import os
from imp import *
import inspect
from inspect import getmembers, isfunction
from importlib import import_module
import pprint




pp = pprint.PrettyPrinter(indent=4)

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
    self.measure_fps = False
    self.last_step_name = "Startup"
    self.store_console_output = False
    self.store_custom_output = False
    self.custom_output = ''
    self.screenshot_filename_prefix = screenshot_filename_prefix
    self.validation_file = output_file_prefix + "/validation.txt"
    temp = open(self.validation_file, 'w')
#    temp.write('Startup\n')
    temp.flush()
    temp.close()

  def addTests(self, funclist):
    for func in funclist:
      self.test_funcs.put(func)

  def doTesting(self):

    if self.store_console_output:
#     pp.pprint(self.last_step_name)
      f = open(self.validation_file, 'a')
      f.write('console_end\n')
      f.flush()
      f.close()
      self.store_console_output = False
    if self.measure_fps:
      try:
#       pp.pprint("measured fps")
        fps_data = self.fps_counter.stop()
        self.measure_fps = False
#       pp.pprint("saving to " + self.validation_file)
        f = open(self.validation_file, 'a')
        f.write('performance\n')
        f.write(fps_data + '\n')
        f.flush()
        f.close()
      except Exception as e:
        print(str(e))   
    if self.store_custom_output:
      try:
#        pp.pprint("custom output")
        self.store_custom_output = False
        f = open(self.validation_file, 'a')
        f.write('custom_start\n')
        f.write(self.custom_output) 
        f.write('custom_end\n')
        f.flush()
        f.close()
        self.custom_output = ''
      except Exception as e:
        print(str(e))   
    try: # If a screenshot has been queued up then it will take that
      screenshot_name = self.screenshot_queue.get(False)
#      pp.pprint("screenshot: " + screenshot_name)
      self.outputScreenshot(screenshot_name)
    except Exception as e:
      if str(e) != '':
        print(str(e))   
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
      for validation in func.validation:
        if validation['type'] == 'screenshot':
          screenshot_name = validation['name']
          self.screenshot_queue.put(screenshot_name)
        elif validation['type'] == 'performance':
          self.measure_fps = True
        elif validation['type'] == 'console':
          #pp.pprint("[" + validation['name'] + "]")
          self.last_step_name = validation['name']
          self.store_console_output = True
        elif validation['type'] == 'custom':
          self.custom_output = ''
          self.store_custom_output = True
      if hasattr(func, 'start_time'):
        timer_callback.addCallback(time.getValue()+func.start_time, UnitTestHelper.startFuncDelayed, (self,func))
      else:
        try:
          if self.store_console_output:
            print "console_start"
            f = open(self.validation_file, 'a')
            f.write("console_start\n")
            f.flush()
            f.close()
        except Exception as e:
          print(str(e))
        try:
          func()
        except Exception as e:
          print str(e)
        if self.measure_fps:
          self.fps_counter.start()
        if(hasattr(func, 'run_time')):
          timer_callback.addCallback(time.getValue()+func.run_time, UnitTestHelper.doTesting, (self,))
        else:
          timer_callback.addCallback(time.getValue()+1, UnitTestHelper.doTesting, (self,))          
      return
    except Exception as e:
      shutdown_file = open( self.early_shutdown_file, 'w' )
      shutdown_file.write( "OK" )
      shutdown_file.flush()
      shutdown_file.close()
      throwQuitAPIException()


  def printCustom(self, value):
    if self.store_custom_output:
      self.custom_output += value + "\n"
    else:
      pp.pprint("Warning: Test script for step " + self.last_step_name + " called printCustom without specifying @custom. The output will not be saved or evaluated.")

  def startFuncDelayed(self, func):
      try:
        if self.store_console_output:
          f = open(self.validation_file, 'a')
          f.write("console_start\n")
          f.flush()
          f.close()
      except Exception as e:
        print(str(e))
      try:
        func()
      except Exception as e:
        print str(e)
      if self.measure_fps:
        self.fps_counter.start()
      if(hasattr(func, 'run_time')):
        timer_callback.addCallback(time.getValue()+func.run_time, UnitTestHelper.doTesting, (self,))
      else:
        timer_callback.addCallback(time.getValue()+1, UnitTestHelper.doTesting, (self,))          


  def outputScreenshot(self, test_step_name = None):
    if test_step_name == None:
      test_step_name = str(self.screenshot_counter)
      self.screenshot_counter += 1
    filename = os.path.abspath(os.path.join(self.output_file_prefix, "renderings/", self.screenshot_filename_prefix + test_step_name + '.png').replace('\\', '/'))
    takeScreenshot(filename)
#    pp.pprint("saving to " + self.validation_file)
    f = open(self.validation_file, 'a')
    f.write('screenshot\n')
    f.write(filename + '\n')
    f.flush()
    f.close()



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


TestcaseScriptFolder = getNamedNode('TestCaseScript').getField('value').getValueAsString().replace('"', '')
TestBaseFolder = getNamedNode('TestBaseFolder').getField('value').getValueAsString().replace('"', '')
sys.path.append(TestBaseFolder) # This is so we can properly import from UnitTestUtil.py
TestcaseScriptFilename = getNamedNode('TestCaseScriptFilename').getField('value').getValueAsString().replace('"', '')
sys.path.append(TestcaseScriptFolder)
TestcaseName = getNamedNode('TestCaseName').getField('value').getValueAsString().replace('"', '')
StartTime = getNamedNode('StartTime').getField('value').getValue()[0]
res = import_module(TestcaseScriptFilename)
res.__scriptnode__ = globals()['__scriptnode__']    
testfunctions_list = [o for o in getmembers(res) if isfunction(o[1])]
testfunctions_list = [item for item in testfunctions_list if ((item not in getmembers(H3DInterface)) and (item not in getmembers(H3DUtils)) and (item not in getmembers(__import__("UnitTestUtil"))))]

testHelper = UnitTestHelper(TestBaseFolder+"/test_complete", os.path.abspath(os.path.join(TestcaseScriptFolder, "output").replace("\\", '/')), TestcaseName + '_')
testHelper.addTests(testfunctions_list)
res.printCustom = testHelper.printCustom

timer_callback.addCallback(time.getValue()+StartTime, UnitTestHelper.doTesting, (testHelper,))       