from H3DInterface import *
from H3DUtils import *

timer_callback = TimerCallback()


class StoreFPS( AutoUpdate( SFFloat ) ):
  def __init__( self ):
    AutoUpdate( SFFloat ).__init__(self)
    self.fps_file = open( '%s', 'w' )
    self.early_shutdown_file = '%s'
    # Used to skip the first iterations.
    self.started = False

  def update( self, event ):
    if self.started:
      self.fps_file.write( str(event.getValue()) + " " )
      self.fps_file.flush()
    return 0

  def start( self ):
    self.started = True
    scene = getCurrentScenes()[0]
    scene.frameRate.routeNoEvent( storeFPS )
    scene = None
    timer_callback.addCallback(time.getValue()+%d, StoreFPS.stop, (self,))

  def stop( self ):
    self.started = False
    self.fps_file.close()
    takeScreenshot( '%s' )
    shutdown_file = open( self.early_shutdown_file, 'w' )
    shutdown_file.write( "OK" )
    shutdown_file.close()
    
storeFPS = StoreFPS()

timer_callback.addCallback(time.getValue()+%d, StoreFPS.start, (storeFPS,))