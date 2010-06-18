from H3DInterface import *
from helper import *
from decimal import Decimal
import cPickle
import os
import math

# Navigation manager for mouse movements
class MouseManager:
  def __init__( self, transform, rotation_transform ):
    #print "Creating mousemanager"
    self.enabled = 1
    self.mouse_sensor = createX3DNodeFromString( "<MouseSensor />" )[0]
    # Can't register the callback function directly, must create a field first
    self.motion_cb = CallbackEventField( self.onMotionEvent, () )
    self.mouse_sensor.motion.routeNoEvent( self.motion_cb )
    self.transform = transform
    self.rotation_transform = rotation_transform
    self.rotation_sensitivity = 0.01
    self.translation_sensitivity = 0.0001
    self.scale_sensitivity = 0.001
    self.rotation_offset = Rotation( 1, 0, 0, 0 )

  def setEnabled( self, enabled ):
    self.enabled = enabled

  def setRotationOffset( self, offset ):
    self.rotation_offset = offset

  def onMotionEvent( self ):
    global viewstate
    if( self.enabled ):
      motion = self.mouse_sensor.motion.getValue()    
      position = self.mouse_sensor.position.getValue()
      if( self.mouse_sensor.leftButton.getValue() ):
        epsilon = 1e-8
        if( motion * motion > epsilon ):
          perp = Vec3f( motion.y, motion.x, 0 )
          perp = self.rotation_offset * perp
          perp.normalize();
          r = Rotation( perp, 
                        motion.length() * self.rotation_sensitivity ) * self.rotation_transform.rotation.getValue()
          self.rotation_transform.rotation.setValue( r )

      if( self.mouse_sensor.rightButton.getValue() ):
        t = Vec3f( motion.x, -motion.y, 0 ) * self.translation_sensitivity
        t = t + self.transform.translation.getValue()
        self.transform.translation.setValue( t )
