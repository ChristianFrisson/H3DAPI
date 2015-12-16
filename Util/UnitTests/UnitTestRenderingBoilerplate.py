
from H3DInterface import *
from H3DUtils import *

from Queue import Queue, Empty

import sys
from importlib import import_module


timer_callback = TimerCallback()

class UnitTestHelper :
  def __init__( self, early_shutdown_file, output_file_prefix, fps_file):
    self.test_funcs = Queue()
    self.screenshot_queue = Queue()
    self.early_shutdown_file = early_shutdown_file
    self.output_file_prefix = output_file_prefix
    self.screenshot_counter = 0    
  #  self.screenshot_queue.put("Startup")
    self.fps_counter = StoreFPS()
    self.measure_fps = False
    self.fps_file_path = fps_file
    self.fps_file_name = 'default'
    self.last_step_name = "Startup"
    self.store_console = False
    self.save_custom_output = False
    self.custom_output = ''
    self.validation_file = output_file_prefix + "validation.txt"
    temp = open(self.validation_file, 'w')
    temp.write('')
    temp.flush()
    temp.close()

  def addTests(self, funclist):
    for func in funclist:
      self.test_funcs.put(func)

  def doTesting(self):
    if self.store_console:
      print "[" + self.last_step_name + "]"
      self.store_console = False
    if self.measure_fps:
      try:
        fps_data = self.fps_counter.stop()
        self.measure_fps = False
        #fps_file = open(os.path.join(self.fps_file_path, self.fps_file_name) + '.perf.txt', 'w')
        #fps_file.write(fps_data)
        #fps_file.flush()
        #fps_file.close()
        print "saving to " + self.validation_file
        f = open(self.validation_file, 'a')
        f.write('performance\n')
        f.write(self.last_step_name + '\n')
        f.write(fps_data + '\n')
        f.write('\n\n')
        f.flush()
        f.close()
      except:
        pass
    if self.save_custom_output:
      try:
        self.save_custom_output = False
        f = open(self.validation_file, 'a')
        f.write('custom\n')
        f.write(self.last_step_name + '\n')
        f.write(self.custom_output) 
        f.write('\n\n')
        f.flush()
        f.close()
      except:
        pass
    try: # If a screenshot has been queued up then it will take that and delay the next test function call by another second
      screenshot_name = self.screenshot_queue.get(False)
      print "screenshot: " + screenshot_name
      self.outputScreenshot(screenshot_name)
    except:
      pass
    try: # If there're more test functions queued up they'll be called, otherwise we shut down.
      print "length of test_funcs is " + str(len(self.test_funcs))
      func_data = self.test_funcs.get(False)
      self.last_step_name = func[0]
      func = func_data[1]
      print "ffffff"
      for validation in func.validation:
        if validation['type'] == 'screenshot':
          screenshot_name = validation['name']
          self.screenshot_queue.put(screenshot_name)
        elif validation['type'] == 'performance':
          self.measure_fps = True
          self.fps_file_name = validation['name']
        elif validation['type'] == 'console':
          print "[" + validation['name'] + "]"
          self.last_step_name = validation['name']
          self.store_console = True
        elif validation['type'] == 'custom':
          self.custom_output = ''
          self.store_custom_output = True
      func()
      if hasattr(func, 'start_time'):\
        timer_callback.addCallback(time.getValue()+func.start_time, UnitTestHelper.startFuncDelayed, (self,func))
      else:
        if self.measure_fps:
          self.fps_counter.start()
        if(hasattr(func, 'run_time')):
          timer.addCallback(time.getValue()+func.run_time, UnitTestHelper.doTesting, (self,))
        else:
          timer.addCallback(time.getValue()+1, UnitTestHelper.doTesting, (self,))          
      return
    except:
      shutdown_file = open( self.early_shutdown_file, 'w' )
      shutdown_file.write( "OK" )
      shutdown_file.flush()
      shutdown_file.close()


  def printCustom(value):
    if self.store_custom_output:
      self.custom_output.append(value + "\n")
    else:
      print "Warning: Test script for step " + self.current_step_name + " called printCustom without specifying @custom. The output will not be saved or evaluated."

  def startFuncDelayed(self, func):
      if self.measure_fps:
        self.fps_counter.start()
      if(hasattr(func, 'run_time')):
        timer.addCallback(time.getValue()+func.run_time, UnitTestHelper.doTesting, (self,))
      else:
        timer.addCallback(time.getValue()+1, UnitTestHelper.doTesting, (self,))          


  def outputScreenshot(self, test_step_name = None):
    if test_step_name == None:
      test_step_name = str(self.screenshot_counter)
      self.screenshot_counter += 1
    filename = self.output_file_prefix + test_step_name + '.png'
    takeScreenshot(filename)
    print "saving to " + self.validation_file
    f = open(self.validation_file, 'a')
    f.write('screenshot\n')
    f.write(self.last_step_name + '\n')
    f.write(filename + '\n')
    f.write('\n\n')
    f.flush()
    f.close()



class StoreFPS( AutoUpdate( SFFloat ) ):
  def __init__( self):
#    self.fps_file = open( filepath, 'w' )
#    self.start_time = start_time
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
    scene.frameRate.routeNoEvent( storeFPS )
    scene = None

  def stop( self ):
    self.running = False
    return self.fps_string