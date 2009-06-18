from H3DInterface import *
from H3DUtils import *
import math

# returns true on " " string event value
class SpacePressed( AutoUpdate(TypedField(SFBool, SFString)) ):
  def update( self, event ):
    if event.getValue() == " ":
      return True
    return False

# updates color on boolean true event value
class BoolToColor( TypedField(SFColor, SFBool) ):
  def __init__( self, ic, nc ):
    TypedField(SFColor, SFBool).__init__( self )
    self.ini_color = ic
    self.new_color = nc
    
  def update( self, event ):
    if event.getValue():
      return self.new_color
    return self.ini_color

# in/decrement input value on boolean receipt  
class BoolToFloatChange( TypedField(SFFloat, (SFBool, SFFloat)) ):
  def __init__( self, o ):
    TypedField(SFFloat, (SFBool, SFFloat)).__init__( self )
    self.offset = o
  
  def update( self, event ):
    r = self.getRoutesIn()
    if len( r ) < 2: return 0
    if event == self.getRoutesIn()[0]:
      return r[1].getValue() + self.offset
    return r[1].getValue()

class MatrixToMFString( AutoUpdate(TypedField(MFString, SFMatrix4f)) ):
  def update( self, event ):
    m = event.getValue()
    l = []
    for i in range(4):
      s = ''
      for j in range(4):
        s += "%0.2f" % (m.getElement(i, j)) + "   "
      l.append(s)
    return l
    
rotation_angle = 0
class Rotate( TypedField(SFRotation, SFBool) ):    
  def update( self, event ):
    global rotation_angle
    rotation_angle += math.pi/30
    if rotation_angle > 2*math.pi:
      rotation_angle -= 2*math.pi
    return Rotation( 0, 0, -1, rotation_angle )
    
rotate = Rotate()

# used in TransformInfo.x3d
matrixToMFString = MatrixToMFString()

# used in MouseSensor.x3d
increment = BoolToFloatChange( 0.02 )
decrement = BoolToFloatChange( -0.02 )

# used in StringSensor.x3d, KeySensor.x3d
stringSFtoMF0 = FieldValue2StringList( SFString )
stringSFtoMF1 = FieldValue2StringList( SFString )

# used in KeySensor.x3d
intToMFString0 = FieldValue2StringList( SFInt32 )
intToMFString1 = FieldValue2StringList( SFInt32 )

# used in TimeTrigger.x3d
timeToMFString = FieldValue2StringList( SFTime )

# used in BooleanToggle.x3d
spacePressed = SpacePressed()

# used in TouchSensor.x3d
colorToRed = BoolToColor( RGB(0.7,0.7,0.7), RGB(1,0,0) )

# used in KeySensor.x3d
colorToYellow = BoolToColor( RGB(0.7,0.7,0.7), RGB(1,1,0) )
colorToGreen = BoolToColor( RGB(0.7,0.7,0.7), RGB(0,1,0) )