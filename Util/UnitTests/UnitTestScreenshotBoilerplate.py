from H3DInterface import *
from H3DUtils import *

from Queue import Queue, Empty

timer_callback = TimerCallback()

class UnitTestHelper :
  def __init__( self ):
    self.test_funcs = Queue()
    self.screenshot_queue = Queue()
    self.early_shutdown_file = '%s'
    self.output_file_prefix = '%s'
    self.screenshot_counter = 0    
    self.screenshot_queue.put("Startup")

  def addTests(self, funclist):
    for func in funclist:
      self.test_funcs.put(func)

  def doTesting(self):
    try: # If a screenshot has been queued up then it will take that and delay the next test function call by another second
      screenshot_name = self.screenshot_queue.get(False)
      self.outputScreenshot(screenshot_name)
    except:
      pass
    try: # If there're more test functions queued up they'll be called, otherwise we shut down.
      func = self.test_funcs.get(False)
      screenshot_name = func()
      self.screenshot_queue.put(screenshot_name)
    except:
        shutdown_file = open( self.early_shutdown_file, 'w' )
        shutdown_file.write( "OK" )
        shutdown_file.close()
    timer_callback.addCallback(time.getValue()+0.2, UnitTestHelper.doTesting, (self,))

  def outputScreenshot(self, test_step_name = None):
    if test_step_name == None:
      test_step_name = str(self.screenshot_counter)
      self.screenshot_counter += 1
    takeScreenshot(self.output_file_prefix + test_step_name + '.png')


testHelper = UnitTestHelper()
timer_callback.addCallback(time.getValue()+1, UnitTestHelper.doTesting, (testHelper,))


def testRotation():
  trans = getNamedNode('T')
  rot = trans.getField('rotation')
  rot.setValue(Rotation(50, 0, 20, 1))
  return "testRotation"

testHelper.addTests([testRotation])
