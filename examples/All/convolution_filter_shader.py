from H3DInterface import *

kernels = [ ("Original", [1], 1),
            ("Blur", [0.0, 0.2, 0.0,
                      0.2, 0.2, 0.2,
                      0.0, 0.2, 0.0], 3 ),
            ("Mean", [1.0/9,1.0/9,1.0/9,
                      1.0/9,1.0/9,1.0/9,
                      1.0/9,1.0/9,1.0/9 ], 3 ),
            ("Sharpen", [-1, -1, -1,
                         -1,  9, -1,
                         -1, -1, -1 ], 3  ) ]
current_kernel = -1

shader_node = getNamedNode( "SHADER" )
text_node = getNamedNode( "TEXT" )

class KeyHandler( AutoUpdate( SFString ) ):
  def update( self, event ):
    global current_kernel
    key = event.getValue()

    current_kernel = (current_kernel  + 1) % len( kernels ) 
    text_node.string.setValue( [kernels[current_kernel][0] ] )
    shader_node.weights.setValue( kernels[current_kernel][1] )
    shader_node.kernelSize.setValue( kernels[current_kernel][2] ) 

    return key

keyhandler = KeyHandler()

keysensor = createNode( "KeySensor" )

keysensor.keyPress.route( keyhandler )
