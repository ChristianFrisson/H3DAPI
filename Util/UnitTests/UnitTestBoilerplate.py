from H3DInterface import *
from H3DUtils import *

from Queue import Queue, Empty


scene = getCurrentScenes()[0]
scene = 0

timer_callback = TimerCallback()

class UnitTestHelper (AutoUpdate( SFFloat) ):
  def __init__( self ):
    AutoUpdate( SFFloat ).__init__(self)
    self.test_funcs = Queue()
    self.early_shutdown_file = '%s'

  def update(self, event):
    pass

  def addTests(self, funclist):
    for func in funclist:
      self.test_funcs.put(func)

  def doTesting(self):
    try:
      func = self.test_funcs.get(False)
      func()
    except:
        shutdown_file = open( self.early_shutdown_file, 'w' )
        shutdown_file.write( "OK" )
        shutdown_file.close()
    timer_callback.addCallback(time.getValue()+1, UnitTestHelper.doTesting, (self,))


testHelper = UnitTestHelper()
timer_callback.addCallback(time.getValue()+1, UnitTestHelper.doTesting, (testHelper,))
