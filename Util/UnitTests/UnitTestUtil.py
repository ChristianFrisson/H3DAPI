# All decorators should add an array called validation to the function if it does not already have one. This array should contain one entry for every decorator that then contains any validation-specific data.
# The parameters start_time and run_time will, if set on the function, determine how long it waits before executing the function and how long it waits after executing the function respectively. It is optional for a decorator to add these.
# It should add an entry into that array containing at least the following:
# init(testHelper, validator, validation_output_file) a function that initiates any data the validator needs. This will be run before the test step function is called. validation_output_file is a file that can and should have any validation output appended to it.
# post(testHelper, validator, validation_output_file) a function that stores validation data. This will be run the next scene graph iteration after run_time has passed. If run_time is undefined then or 0 then it will be run the next iteration after the test function is called.

import os
from H3DInterface import takeScreenshot

def screenshot(start_time = None, run_time = None):
  def _screenshot(func):
    def init(testHelper, validator, validation_output_file):
      pass

    def post(testHelper, validator, validation_output_file):
      try:
        screenshot_name = validator['step_name']
        filename = os.path.abspath(os.path.join(testHelper.output_file_prefix, "renderings/", testHelper.screenshot_filename_prefix + validator['step_name'] + '.png').replace('\\', '/'))
        takeScreenshot(filename)
    #    pp.pprint("saving to " + self.validation_file)
        f = open(validation_output_file, 'a')
        f.write('screenshot\n')
        f.write(filename + '\n')
        f.flush()
        f.close()
      except Exception as e:
        if str(e) != '':
          print(str(e))   
    
    if not hasattr(func, 'validation'):
      func.validation = []

    func.validation.append({'type': 'screenshot', 'step_name' : func.__name__, 'init' :  init, 'post' : post, 'start_time' : start_time, "run_time" : run_time})
    return func
  return _screenshot


def performance(start_time = None, run_time = None):
  def _performance(func):
    def init(testHelper, validator, validation_output_file):
      testHelper.fps_counter.start()

    def post(testHelper, validator, validation_output_file):
      try:
        fps_data = testHelper.fps_counter.stop()
        f = open(validation_output_file, 'a')
        f.write('performance\n')
        f.write(fps_data + '\n')
        f.flush()
        f.close()
      except Exception as e:
        print(str(e))   

    if not hasattr(func, 'validation'):
      func.validation = []
      
    if (not (start_time is None) and (not hasattr(func, 'start_time') or (func.start_time < start_time))):
      func.start_time = start_time

    if (not (run_time is None) and (not hasattr(func, 'run_time') or (func.run_time < run_time))):
      func.run_time = run_time
      
    func.validation.append({'type': 'performance', 'step_name' : func.__name__, 'init' :  init, 'post' : post, 'start_time' : start_time, "run_time" : run_time})
    return func
  return _performance


def console(start_time = None):
  def _console(func):
    def init(testHelper, validator, validation_output_file):
      print 'console_start_' + validator['step_name']

    def post(testHelper, validator, validation_output_file):
      f = open(validation_output_file, 'a')
      f.write('console\n')
      f.flush()
      f.close()
      print 'console_end_' + validator['step_name']

    if not hasattr(func, 'validation'):
      func.validation = []
      
    if (not (start_time is None) and (not hasattr(func, 'start_time') or (func.start_time < start_time))):
      func.start_time = start_time


      
    func.validation.append({'type': 'console', 'step_name' : func.__name__, 'init' :  init, 'post' : post, 'start_time' : start_time, "run_time" : None})
    return func
  return _console


def custom(start_time = None):
  def _custom(func):
    def customPrintHelper(testHelper, value):
      testHelper.customValidator['custom_output'] += value + "\n"

    def init(testHelper, validator, validation_output_file):
      testHelper.customValidator = validator
      testHelper.customPrintHelper = customPrintHelper
      validator['custom_output'] = ''

    def post(testHelper, validator, validation_output_file):
      testHelper.customPrintHelper = None
      try:
        f = open(validation_output_file, 'a')
        f.write('custom_start\n')
        f.write(validator['custom_output']) 
        f.write('custom_end\n')
        f.flush()
        f.close()
        f = open(os.path.abspath(os.path.join(testHelper.output_file_prefix, "text/" + validator['step_name'] + "_custom.txt")), 'w')
        f.write(validator['custom_output'])
        f.flush()
        f.close()
      except Exception as e:
        print(str(e))


    if not hasattr(func, 'validation'):
      func.validation = []
      
    if (not (start_time is None) and (not hasattr(func, 'start_time') or (func.start_time < start_time))):
      func.start_time = start_time

      
    func.validation.append({'type': 'custom', 'step_name' : func.__name__, 'init' :  init, 'post' : post, 'start_time' : start_time, "run_time" : None, 'custom_output' : ""})
    return func
  return _custom

