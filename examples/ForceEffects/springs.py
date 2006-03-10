from H3DInterface import *

# route 0 is the active field of the spring effect
# route 1 is the startDistance field
# route 2 is the escapeDistance

class SphereRadius( TypedField( SFFloat, ( SFBool, SFFloat, SFFloat )  ) ):
  def update( self, event ):
    routes_in = self.getRoutesIn()
    active = routes_in[0].getValue()
    start_dist = routes_in[1].getValue()
    escape_dist = routes_in[2].getValue()
    if( active ):
      return escape_dist
    else:
      return start_dist

sphereRadius = SphereRadius()
