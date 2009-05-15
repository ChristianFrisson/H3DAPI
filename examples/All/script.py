from H3DInterface import *
from H3DUtils import *

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

# used in MouseSensor.x3d
increment = BoolToFloatChange( 0.02 )
decrement = BoolToFloatChange( -0.02 )

# used in StringSensor.x3d, KeySensor.x3d
stringSFtoMF0 = FieldValue2StringList( SFString )
stringSFtoMF1 = FieldValue2StringList( SFString )

# used in KeySensor.x3d
intToMFString0 = FieldValue2StringList( SFInt32 )
intToMFString1 = FieldValue2StringList( SFInt32 )

# used in BooleanToggle.x3d
spacePressed = SpacePressed()

# used in TouchSensor.x3d
colorToRed = BoolToColor( RGB(0.7,0.7,0.7), RGB(1,0,0) )

