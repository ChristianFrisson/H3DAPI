from H3DInterface import *

# Transforms from SFRotation to SFMatrix4f. If self.inverse is true the
# inverse matrix is calculated.
class SFRotation2SFMatrix4f( TypedField( SFMatrix4f, (SFRotation ) ) ):
  def __init__(self, inverse ):
    TypedField( SFMatrix4f, (SFRotation ) ).__init__(self)
    self.inverse = inverse

  def update( self, event ):
    if( self.inverse ):
      return Matrix4f( event.getValue() ).inverse()
    else:
      return Matrix4f( event.getValue() )

vertex_shader, = references.getValue()

rotation2Matrix = SFRotation2SFMatrix4f(0)
rotation2MatrixInverse = SFRotation2SFMatrix4f(1)

flag = 1
def traverseSG():
  vp = getActiveViewpoint()
  global flag
  # routes are defined in traverseSG and not the initialize function in this case
  # because we have not set the viewpoint in X3D. The default viewpoint is set
  # after this PythonScript has been parsed.
  if vp and flag:
    rotation2Matrix.route( vertex_shader.viewpointOrn )
    vp.totalOrientation.route( rotation2Matrix )
    vp.totalPosition.route( vertex_shader.viewpointPosition )
    flag = 0
    try:
      # go to except block in case the viewpointOrnInv field does not exist in the shader.
      rotation2MatrixInverse.route( vertex_shader.viewpointOrnInv )
      vp.totalOrientation.route( rotation2MatrixInverse )
    except:
      # Do nothing, since field is not needed. 
      pass
