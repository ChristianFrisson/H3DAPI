# This is the example from the H3D Manual(Examples section)

#import the H3D fields and types
from H3DInterface import *

# The Color class is of type SFColor and its value is determined by
# the SFBool field that is routed to it. If its value is 1 the color
# is red, otherwise it is blue.
class Color( TypedField( SFColor, SFBool ) ):
  def update( self, event ):
    if( event.getValue() ):
      return RGB( 1, 0, 0 )
    else:
      return RGB( 0, 0, 1 )

# create an instance of the Color class.
color = Color()
