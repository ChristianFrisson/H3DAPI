from H3DInterface import *

COORD, = references.getValue()

class DevicePositionChangeField( AutoUpdate(TypedField(SFBool, (SFBool, SFVec3f))) ):
  def update(self, event):
    click, pos = getRoutesIn(self)
    if click.getValue():
      COORD.point.push_back( pos.getValue() )
    return True

position_change = DevicePositionChangeField()

class KeyPress( AutoUpdate(SFString) ):
  def update(self, event):
    k = event.getValue()
    if k == ' ':
      print 'Clear board'
      COORD.point.clear()
    return ''
    
keypress = KeyPress()
