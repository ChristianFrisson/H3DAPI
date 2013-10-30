from H3DInterface import *

tex3d = createX3DNodeFromString( 
  "<ImageTexture3D url='images/stone_wall.bmp' />" )[0]
tex2d = createX3DNodeFromString( 
  "<ImageTexture url='images/nautilus.jpg' />" )[0]
flag = 0

class SwitchTexture( AutoUpdate(TypedField(SFNode, SFTime)) ):
  def update( self, event ):
    global flag
    if flag:
      flag = 0
      return tex2d
    flag = 1
    return tex3d

switchTexture = SwitchTexture()