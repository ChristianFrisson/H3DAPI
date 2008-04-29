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

setup_viewpoint_route = 1
vertex_shader, = references.getValue()

def traverseSG():
  global setup_viewpoint_route
  if setup_viewpoint_route:
    setup_viewpoint_route = 0
    vp = getActiveViewpoint()
    if vp:
      rotation2Matrix = SFRotation2SFMatrix4f(0)
      rotation2Matrix.route( vertex_shader.viewpointOrn )
      vp.totalOrientation.route( rotation2Matrix )
      vp.totalPosition.route( vertex_shader.viewpointPosition )
      try:
        # go to except block in case this field does not exist.
        vertex_shader.viewpointOrnInv
        rotation2MatrixInverse = SFRotation2SFMatrix4f(1)
        rotation2MatrixInverse.route( vertex_shader.viewpointOrnInv )
        vp.totalOrientation.route( rotation2MatrixInverse )
      except:
        # Do nothing, since field is not needed. Need to have some code here
        # since python complains about no indentation otherwise.
        a = 1
