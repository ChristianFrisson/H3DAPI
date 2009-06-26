from H3DInterface import *

view_a, view_b = references.getValue()

class SwitchView( PeriodicUpdate(TypedField(MFString, SFString)) ) :
  def update( self, event ):
    key = event.getValue()
    if key == "A" or key == "a":
      view_a.set_bind.setValue( True )
      return ["Viewpoint A with retainedUserOffsets switched ON,", \
              "viewpoint position and orientation after navigation",
              "will be retained when this viewpoint is rebound."]
    if key == "B" or key == "b":
      view_b.set_bind.setValue( True )
      return ["Viewpoint B with retainedUserOffsets switched OFF,", \
              "viewpoint will revert to default position and",
              "orientation when viewpoint is rebound."]
    return [""]

switchView = SwitchView()
