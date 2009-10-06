from H3DInterface import *

rbc, rgdPlane, gfxPlane, rgdSlat = references.getValue()

# Toggle RigidBodyPhysics by pressing 'p'
class ToggleRBP ( AutoUpdate(SFString) ):
	def update( self, event ):
		global rbc
		if event.getValue() == 'p':
			rbc.enabled.setValue(1 - rbc.enabled.getValue())
		return ''
toggleRBP = ToggleRBP()
