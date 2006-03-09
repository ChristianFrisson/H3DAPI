from H3DInterface import *

class SFVec3fToColor( TypedField( MFColor, ( SFVec3f, SFMatrix4f, MFVec3f ) ) ):
  def update( self, event ):
    inputs = self.getRoutesIn()
    light_pos_global = inputs[0].getValue()
    acc_inverse_matrix = inputs[1].getValue()
    points = inputs[2].getValue()
    light_pos = acc_inverse_matrix * light_pos_global
    res = []
    for p in points:
      v = light_pos - p
      v.normalize()
      v.y = -v.y
      v = v * 0.5 + Vec3f( 0.5, 0.5, 0.5 ) 
      res.append( RGB( v.x, v.y, v.z ) )
    return res

toColor = SFVec3fToColor()
