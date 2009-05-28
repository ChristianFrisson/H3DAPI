from H3DInterface import *

class Attrib( AutoUpdate(TypedField(MFMatrix4f, SFRotation)) ):
  def update( self, event ):
    r = Matrix4f( event.getValue() )
    n = Matrix4f(1.0, 0.0, 0.0, 0.0, 
                 0.0, 1.0, 0.0, 0.0, 
                 0.0, 0.0, 1.0, 0.0, 
                 0.0, 0.0, 0.0, 1.0)
    return [r, r, r, r, r, r, r, r]

set_attrib = Attrib()