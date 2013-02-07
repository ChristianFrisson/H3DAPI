from H3DInterface import *

src= getNamedNode ( 'SRC' )
dst= getNamedNode ( 'DST' )

# Clone the SRC node
deepCopy= True
cloned= src.clone (deepCopy)
dst.children.push_back ( cloned )

# Change the cloned node to verify that it is indeed a different instance
# If using shallow copy, then the original will also be changed
cloned_coord= cloned.children.getValue()[0].geometry.getValue().coord.getValue()
cloned_points= cloned_coord.point.getValue()
cloned_points[0]= Vec3f ( 0, 0.15, 0.1 )
cloned_coord.point.setValue ( cloned_points )

# The Appearance node is shared between sphere and triangle set, so in both deep
# and shallow copies, this should change the color of the sphere and triangle set
cloned_mat= cloned.children.getValue()[1].children.getValue()[0].appearance.getValue().material.getValue()
cloned_mat.diffuseColor.setValue ( RGB ( 1, 0, 0 ) )