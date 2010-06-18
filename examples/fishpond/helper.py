from H3DInterface import *

# field to call a call-back function when touched
class CallbackEventField( TypedField( AutoUpdate( SFBool ), Field ) ):
  def __init__( self, callback_function, args ):
    TypedField( AutoUpdate( SFBool ), Field ).__init__( self )
    self.cb = (callback_function, args )

  def update( self, event ):
    apply( self.cb[0], self.cb[1] )
    return 1

