from H3DInterface import *

def PrintFieldValue( base_class ):
  class PrintValueClass( AutoUpdate( base_class ) ):
    def update( self, event ):
      v = event.getValue()
      print v
      return v
  return PrintValueClass()

def FieldValue2String( base_class ):
  class Value2StringClass( TypedField( SFString, base_class ) ):
    def update( self, event ):
      v = event.getValue()
      return str( v )
  return Value2StringClass()

def FieldValue2StringList( base_class ):
  class Value2StringListClass( TypedField( MFString, base_class ) ):
    def update( self, event ):
      v = event.getValue()
      return [str( v )]
  return Value2StringListClass()

def FieldValue2Int( base_class ):
  class Value2IntClass( TypedField( SFInt32, base_class ) ):
    def update( self, event ):
      v = event.getValue()
      return int( v )
  return Value2IntClass()

def SField2MField( sfield, mfield ):
  class SField2MFieldClass( TypedField( mfield, sfield ) ):
    def update( self, event ):
      v = event.getValue()
      return [v]
  return SField2MFieldClass()

# The TimerCallback field is a field in which you can set callback functions
# to be called at a later time that you specify.
class TimerCallback( AutoUpdate( SFTime ) ):
  def __init__( self ):
    AutoUpdate( SFTime ).__init__( self )
    self.callbacks = []
    time.route( self )

  def update( self, event ):
    t = event.getValue()
    cbs_to_remove = []
    for cb in self.callbacks:
      if t > cb[0]:
        apply( cb[1], cb[2] )
        cbs_to_remove.append( cb )
    for cb in cbs_to_remove:
      self.callbacks.remove( cb )

    return event.getValue()

  # add a callback function. The function will be called at the specified
  # time with the given arguments.
  def addCallback( self, time, func, args ):
    self.callbacks.append( (time, func, args ) )
