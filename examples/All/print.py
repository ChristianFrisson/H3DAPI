from H3DInterface import *

node, = references.getValue()

# print field values on incoming true event
class PrintFields( AutoUpdate(SFBool) ):
  def update( self, event ):
    if event.getValue():
      for n in node.getFieldList():
        f = node.getField( n )
        print n, ": ", f.getValue()

printFields = PrintFields()