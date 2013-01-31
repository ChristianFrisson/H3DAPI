from H3DInterface import *

node, = references.getValue()

# print field values on incoming true event
class PrintFields( AutoUpdate(SFBool) ):
  def update( self, event ):
    if event.getValue():
      for n in node.getFieldList():
        f = node.getField( n )
        access_type = f.getAccessType()
        if access_type != INITIALIZE_ONLY and access_type != INPUT_ONLY:
          print n, ": ", f.getValue()
    return True

printFields = PrintFields()