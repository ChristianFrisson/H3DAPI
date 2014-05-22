## \file H3DUtils.py Documentation file for H3DUtils python module.
## This module contains various functions which returns useful
## field template classes.
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

## The TimerCallback field is a field in which you can set callback functions
## to be called at a later time that you specify.
## <b>Example usage:</b>
##
## <pre>
## def test( v ):
##   print v
##
## tc = TimerCallback()
## tc.addCallback( time.getValue()+3, test, ("Hello world!",) )
## </pre>
## This will call the function test 3 seconds from it is run. 
class TimerCallback( AutoUpdate( SFTime ) ):
  ## Constructor.
  def __init__( self ):
    AutoUpdate( SFTime ).__init__( self )
    ## The list of callbacks currently in use.
    self.callbacks = []

    # Set up a route from H3DInterface.time in order for the update
    # function to run once per scene graph loop.
    time.route( self )

  ## Specialized update function to call callback functions when the time
  ## is right.
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

  ## Add a callback function. The function will be called at the specified
  ## time with the given arguments and then removed.
  ## \param time The time to run the function.
  ## \param func The function to call.
  ## \param args Tuple with the arguments to call.
  ## \return Handle to callback function.
  def addCallback( self, time, func, args ):
    cb = (time, func, args )
    self.callbacks.append( cb )
    return cb

  ## Remove a callback function before it has executed.
  ## \param cb The handle of the callback to remove.
  def removeCallback( self, cb ):
    try:
      self.callbacks.remove( cb )
    except:
      pass
