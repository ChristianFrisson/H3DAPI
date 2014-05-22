from H3DInterface import *

src= getNamedNode ( 'SRC' )
dst= getNamedNode ( 'DST' )

# Clone the SRC node
deepCopy= SFBool()
deepCopy.setValue( 1 )
changeClone = SFBool()
changeClone.setValue( False )

class UpdateClone( PeriodicUpdate( SFBool ) ):
  def update( self, event ):
    routes_in = self.getRoutesIn()
    cloned= src.clone(routes_in[0].getValue())
    dst.children.clear()
    dst.children.push_back( cloned )
    if routes_in[1].getValue():
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
    return True

updateClone = UpdateClone()
deepCopy.routeNoEvent( updateClone )
changeClone.route( updateClone )

addProgramSetting( deepCopy, "Deep copy" )
addProgramSetting( changeClone, "Change clone" )