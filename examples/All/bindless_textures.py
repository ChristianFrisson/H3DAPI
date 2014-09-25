from H3DInterface import *


group = getNamedNode( "GROUP" )
transform = getNamedNode( "TRANSFORM" )

grid_size= 10
spacing= 0.05
org= -(spacing*grid_size)/2
group.children.erase ( transform )

for x in range (grid_size):
	for y in range (grid_size):
		t= transform.clone()
		t.translation.setValue ( Vec3f ( org+spacing*x, org+spacing*y, 0 ) )
		group.children.push_back ( t )

