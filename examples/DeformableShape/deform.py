from H3DInterface import *

# make sure the proxy is drawn at the position of the haptics device
di = getActiveDeviceInfo()
if( di ):
  devices = di.device.getValue()
  for d in devices:
    d.proxyWeighting.setValue( 0 )

# This python file creates an rectangular IndexedTriangleSet with
# columns X rows coordinates.
columns = 31
rows = 31
size = Vec2f( 0.4, 0.4 )

coords = []
tex_coords = []
index = []


step_c = size.x / (columns-1)
step_r = size.y / (rows-1)

tc_step_c = 1.0/ (columns-1)
tc_step_r = 1.0/ (rows-1)

for c in range( columns ):
  for r in range( rows ):
    coords.append( Vec3f( step_c * c - size.x / 2, step_r * r - size.y/2, 0 ) )
    tex_coords.append( Vec2f( tc_step_c * c, tc_step_r * r ) )

for c in range( columns - 1 ):
  for r in range( rows - 1 ):
    v0 = r * columns + c 
    v1 = r * columns + c+1
    v2 = (r+1) * columns + c+1
    v3 = (r+1) * columns + c
    index = index + [v0, v1, v2, v0, v2, v3 ]

deform_node, = references.getValue()

its =  createX3DNodeFromString( "<IndexedTriangleSet  solid=\"FALSE\" />" )[0]

coord = createX3DNodeFromString( "<Coordinate />" )[0]
coord.point.setValue( coords )

tex_coord = createX3DNodeFromString( "<TextureCoordinate />" )[0]
tex_coord.point.setValue( tex_coords )

its.index.setValue( index )
its.coord.setValue( coord ) 
its.texCoord.setValue( tex_coord )

deform_node.geometry.setValue( its )




