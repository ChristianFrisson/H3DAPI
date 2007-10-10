//////////////////////////////////////////////////////////////////////////////
//    Copyright 2004-2007, SenseGraphics AB
//
//    This file is part of H3D API.
//
//    H3D API is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    H3D API is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with H3D API; if not, write to the Free Software
//    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//    A commercial license is also available. Please contact us at 
//    www.sensegraphics.com for more information.
//
//
/// \file PixelTexture.h
/// \brief Header file for PixelTexture, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __PIXELTEXTURE_H__
#define __PIXELTEXTURE_H__

#include <iostream>
#include <H3D/X3DTexture2DNode.h>

namespace H3D {
  /// \ingroup X3DNodes 
  /// \class PixelTexture
  /// The PixelTexture node defines a 2D image-based texture map
  /// as an explicit array of pixel values (image field) and parameters 
  /// controlling tiling repetition of the texture onto geometry.
  /// The repeatS and repeatT fields specify how the texture wraps 
  /// in the S and T directions. 
  /// If repeatS is TRUE (the default), 
  /// the texture map is repeated outside the 0-to-1 texture coordinate
  /// range in the S direction so that it fills the shape. If repeatS 
  /// is FALSE, the texture coordinates are clamped in the S direction 
  /// to lie within the 0.0 to 1.0 range. The repeatT field is analogous 
  /// to the repeatS field.
  /// The image field are specified as three integers representing the 
  /// width, height and number of components in the image, followed by 
  /// width*height hexadecimal or integer values representing the 
  /// pixels in the image, separated by whitespace:
  ///
  /// \code
  /// image <width> <height> <num components> <pixel values>
  /// \endcode
  ///
  /// Pixel values are limited to 256 levels of intensity 
  /// (i.e., 0-255 decimal or 0x00-0xFF hexadecimal). A one-component 
  /// image specifies one-byte hexadecimal or integer values representing 
  /// the intensity of the image. For example, 0xFF is full intensity in
  /// hexadecimal (255 in decimal), 0x00 is no intensity (0 in decimal). 
  /// A two-component image specifies the intensity in the first (high) 
  /// byte and the alpha opacity in the second (low) byte. Pixels in a 
  /// three-component image specify the red component in the first 
  /// (high) byte, followed by the green and blue components 
  /// (e.g., 0xFF0000 is red, 0x00FF00 is green, 0x0000FF is blue). 
  /// Four-component images specify the alpha opacity byte after 
  /// red/green/blue (e.g., 0x0000FF80 is semi-transparent blue). A value 
  /// of 0x00 is completely transparent, 0xFF is completely opaque. Note 
  /// that alpha equals (1.0 - transparency), if alpha and transparency 
  /// range from 0.0 to 1.0.
  /// Each pixel is read as a single unsigned number. For example, 
  /// a 3-component pixel with value 0x0000FF may also be written as 
  /// 0xFF (hexadecimal) or 255 (decimal). Pixels are specified from 
  /// left to right, bottom to top. The first hexadecimal value is the
  /// lower left pixel and the last value is the upper right pixel.
  /// 
  /// \par Example:
  /// 
  /// \code
  /// <PixelTexture DEF="greyscaleImage" image="1 2 1 0xFF 0x00"/>
  /// \endcode
  /// is a 1 pixel wide by 2 pixel high one-component (i.e., greyscale) 
  /// image, with the bottom pixel white and the top pixel black.
  ///
  /// \par Internal routes:
  /// \dotfile PixelTexture.dot  
  class H3DAPI_API PixelTexture : public X3DTexture2DNode {
  public:

    /// Constructor.
    PixelTexture( 
                 Inst< DisplayList > _displayList = 0,
                 Inst< SFNode  > _metadata  = 0,
                 Inst< SFBool  > _repeatS   = 0,
                 Inst< SFBool  > _repeatT   = 0,
                 Inst< SFBool  > _scaleToP2 = 0,
                 Inst< SFImage > _image     = 0,
                 Inst< SFTextureProperties > _textureProperties = 0 );

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  };
}

#endif
