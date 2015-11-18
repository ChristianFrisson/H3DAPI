from H3DInterface import *
from H3DUtils import *

from Queue import Queue, Empty


timer_callback = TimerCallback()

class UnitTestHelper (AutoUpdate( SFFloat) ):
  def __init__( self ):
    AutoUpdate( SFFloat ).__init__(self)
    self.test_funcs = Queue()


  def update(self, event):
    pass

  def addTests(self, funclist):
    for func in funclist:
      self.test_funcs.put(func)

  def doTesting(self, ):
    if not self.test_funcs.empty:
      func = self.test_funcs.get()
      timer_callback.addCallback(time.getValue()+1, doTesting, ())
    #else:
       

    

#class StoreFPS( AutoUpdate( SFFloat ) ):
#  def __init__( self ):
#    AutoUpdate( SFFloat ).__init__(self)
#    self.fps_file = open( '%s', 'w' )
#    self.early_shutdown_file = '%s'
#    # Used to skip the first iterations.
#    self.traverse_sg_counter = 0

#  funcQueue = Queue()

#  def update( self, event ):
#    if self.traverse_sg_counter > 9 and self.traverse_sg_counter < 101:
#      self.fps_file.write( str(event.getValue()) + " " )
#      self.fps_file.flush()
#    if self.traverse_sg_counter == 50:
#      initTest()
#    elif not funcQueue.empty():
#      values = funcQueue.get()
#      func = values[0]
#      args = values[1:]
#      func(*args)
#    self.traverse_sg_counter = self.traverse_sg_counter + 1
#    return 0

#storeFPS = StoreFPS()

testHelper = UnitTestHelper()
timer_callback.addCallback(time.getValue()+1, testHelper.doTesting, ())

#scene = getCurrentScenes()[0]
#scene.frameRate.routeNoEvent( storeFPS )
#scene = None

