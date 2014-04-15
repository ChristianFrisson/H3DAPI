//////////////////////////////////////////////////////////////////////////////
//    Copyright 2004-2014, SenseGraphics AB
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
/// \file TextureProperties.h
/// \brief Header file for TextureProperties, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __TEXTUREPROPERTIES_H__
#define __TEXTUREPROPERTIES_H__

#include <H3D/X3DNode.h>
#include <H3D/H3DDisplayListObject.h>
#include <H3D/SFInt32.h>
#include <H3D/SFBool.h>
#include <H3D/SFString.h>
#include <H3D/SFFloat.h>
#include <H3D/SFColorRGBA.h>
#include <H3D/SFVec4f.h>

namespace H3D {
  /// \ingroup X3DNodes
  /// \class TextureProperties
  /// \brief TextureProperties allows fine control over a texture's application.
  /// This node can be used to set the texture properties for a node with a
  /// textureProperties field. A texture with a TextureProperties node will
  /// ignore the repeatS and repeatT fields on the texture.
  ///
  /// The anisotropicDegree field describes the minimum degree of anisotropy to
  /// account for in texture filtering. A value of 1 implies no anisotropic
  /// filtering. Values above the system's maximum supported value will be
  /// clamped to the maximum allowed. 
  ///
  /// The borderColor field describes the color to use for border pixels.
  ///
  /// The borderWidth field describes the number of pixels to use for a 
  /// texture border.
  ///
  /// The boundaryModeS, boundaryModeT nad boundaryModeR fields describe the
  /// way texture coordinate boundaries are handled. Valid values are:
  ///
  /// - "CLAMP"  Clamp texture coordinates to the range [0,1]  
  /// - "CLAMP_TO_EDGE"  Clamp texture coordinates such that a border
  ///    texel is never sampled. Coordinates are clamped to the range 
  ///    [1/(2N), 1 - 1/(2N)], where N is the size of the texture in the
  ///    direction of clamping.  
  /// - "CLAMP_TO_BOUNDARY"  Clamp texture coordinates such that texture
  ///   samples are border texels for fragments whose corresponding texture
  ///   coordinate is sufficiently outside the range [0,1]. Texture 
  ///  coordinates are clamped to the range [-1/(2N), 1 + 1/(2N)].  
  /// - "MIRRORED_REPEAT"  Texture coordinates are mirrored and then clamped
  ///   as in CLAMP_TO_EDGE  
  /// - "REPEAT"  Repeat a texture across the fragment. Ignore the integer 
  ///   part of the texture coordinates, using only the fractional part.  
  ///
  ///
  /// The magnificationFilter field describes the way textures are filtered
  /// when the image is smaller then the screen space representation. 
  /// Valid values are:
  ///
  /// - "AVG_PIXEL"  Select the weighted average of the four texture elements
  ///   that are closest to the center of the pixel being textured.  
  /// - "DEFAULT"  Select the browser-specified default magnification mode. In 
  ///   H3D API the same as AVG_PIXEL.
  /// - "FASTEST"  Select the fastest method available.  
  /// - "NEAREST_PIXEL"  Select the pixel that is nearest to the center
  ///   of the pixel being textured.  
  /// - "NICEST"  Select the highest quality method available.  
  ///
  /// The minificationFilter field describes the way textures are filtered
  /// when the image is larger then the screen space representation. Valid
  /// values are: 
  ///
  /// -"AVG_PIXEL"  Select the weighted average of the four texture elements
  ///  that are closest to the center of the pixel being textured.  
  /// -"AVG_PIXEL_AVG_MIPMAP"  Performs tri-linear filtering. Choose the two
  ///   mipmaps that most closely match the size of the pixel being textured
  ///   and use the weighted average of the four texture elements that are
  ///   closest to the center of the pixel to produce a texture value from
  ///   each mipmap. The final texture value is a weighted average of those
  ///   two values.   
  /// -"AVG_PIXEL_NEAREST_MIPMAP"  Choose the mipmap that most closely matches
  ///   the size of the pixel being textured and use the weighted average of
  ///   the four texture elements that are closest to the center of the pixel
  ///   to produce a texture value.  
  /// -"DEFAULT"  Select the browser-specified default minification mode.  
  /// -"FASTEST"  Select the fastest method available. Mipmaps shall be used, 
  ///   if available.  
  /// -"NEAREST_PIXEL"  Select the pixel that is nearest to the center of 
  ///   the pixel being textured.  
  /// -"NEAREST_PIXEL_AVG_MIPMAP"  Choose the two mipmaps that most closely 
  ///   match the size of the pixel being textured and use the texture element
  /// nearest to the center of the pixel to produce a texture value from
  /// each mipmap. The final texture value is a weighted average of those
  /// two values.  
  /// -"NEAREST_PIXEL_NEAREST_MIPMAP"  Choose the mipmap that most closely 
  ///  matches the size of the pixel being textured and use the texture
  ///  element nearest to the center of the pixel) to produce a texture value.
  /// 
  /// -"NICEST"  Select the highest quality method available. Mipmaps shall be 
  ///  used, if available.  
  ///
  /// Modes with MIPMAP in the name require mipmaps. If mipmaps are not 
  /// provided, the mode shall pick the corresponding non-mipmapped mode
  /// (e.g., AVG_PIXEL_NEAREST_MIPMAP becomes AVG_PIXEL).
  ///
  /// The texturePriority field describes the texture residence priority for
  /// allocating texture memory. Zero indicates the lowest priority and 1
  /// indicates the highest priority. Values are clamped to the range [0,1].
  ///
  /// The generateMipMaps field describes whether mipmaps should be generated
  /// for the texture. Mipmaps are required for filtering modes with MIPMAP
  /// in their value.
  /// 
  /// The textureCompression fields describes if/how a texture should be 
  /// compressed. Valid values are:
  ///
  /// - "DEFAULT"  No compression
  /// - "FASTEST" Select the fastest compression mode available.  
  /// - "HIGH"  Select the compression mode with the greatest amount of 
  ///   compression.  
  /// - "LOW"  Select the compression mode with the least amount of
  ///   compression.  
  /// - "MEDIUM"  Select a compression mode with a moderate amount of 
  ///   compression.  
  /// - "NICEST"  Select the compression mode that produces the nicest effect.
  ///
  /// The textureCompareMode field specifies what texture compare
  /// function to use. Texture compare is used to compare the 
  /// R texture coordinates to the texture value and return 
  /// 1 if the test succeeds and textureCompareFailValue on fail.
  /// The output of a texture lookup with this enabled will 
  /// there for only return 1 or textureCompareFailValue depending
  /// on the test. The original texture value is only used for the
  /// test. The normal usage of this is when using shadow mapping
  /// to determine if a fragment is in shadow or not.
  /// 
  /// The valid values are:
  /// <table>
  /// <tr><td>"NONE"</td><td>No texture compare is done. Texture works as 
  /// normal.</td></tr>
  /// <tr><td>"GEQUAL"</td><td>Output texture value is 1 if R texture 
  /// coordinate >= original texture value, textureCompareFailValue otherwise.</td> 
  /// <tr><td>"LEQUAL"</td><td>Output texture value is 1 if R texture 
  /// coordinate <= original texture value, textureCompareFailValue otherwise.</td> 
  /// </table>
  ///
  /// The textureCompareFailValue contains the value to use as texture
  /// output when the test in textureCompareMode fails.
  ///
  /// The textureType specifies what texture target type
  /// should be used for the texture. This can affect how texture
  /// values are interpolated and how texture coordinates are 
  /// used to look up into the texture.
  /// 
  /// The valid values are:
  /// <table>
  /// <tr><td>"NORMAL"</td><td>GL_TEXTURE_2D for 2d textures and GL_TEXTURE_3D for 3d 
  /// textures.</td></tr>
  /// <tr><td>"2D_RECTANGLE"</td><td>GL_TEXTURE_RECTANGLE. Only valid for 2D textures. 
  /// Make the texture lookup in pixel space instead of as normal with normalized
  /// coordinates. E.g. tex coord (4,5) will look up the pixel with that index directly</td> </tr>
  /// <tr><td>"2D_ARRAY"</td><td>GL_TEXTURE_2D_ARRAY. Only valid for 3D textures and together with shaders. 
  /// Texture is a stack of 2D images where the R component specifies the
  /// index of the 2D image to use. </td> </tr>
  /// </table>
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/TextureProperties.x3d">TextureProperties.x3d</a>
  ///     ( <a href="examples/TextureProperties.x3d.html">Source</a> )
  ///
  /// \par Internal routes:
  /// \dotfile TextureProperties.dot  
  class H3DAPI_API TextureProperties : 
    public X3DNode {
  public:
    
    /// Constructor.
    TextureProperties( Inst< SFNode  > _metadata              = 0,
                       Inst< SFFloat  > _anisotropicDegree    = 0,
                       Inst< SFColorRGBA > _borderColor       = 0,
                       Inst< SFInt32  > _borderWidth          = 0,
                       Inst< SFString > _boundaryModeS        = 0,
                       Inst< SFString > _boundaryModeT        = 0,
                       Inst< SFString > _boundaryModeR        = 0,
                       Inst< SFString  > _magnificationFilter = 0,
                       Inst< SFString  > _minificationFilter  = 0,
                       Inst< SFString > _textureCompression   = 0,
                       Inst< SFFloat > _texturePriority        = 0,
                       Inst< SFBool  > _generateMipMaps      = 0,
                       Inst< SFVec4f > _textureTransferScale = 0,
                       Inst< SFVec4f > _textureTransferBias  = 0,
                       Inst< SFString > _textureCompareMode = 0,
                       Inst< SFFloat  > _textureCompareFailValue = 0,
                       Inst< SFString > _textureType        = 0 );

    /// Returns the default xml containerField attribute value.
    /// For this node it is "textureProperties".
    virtual string defaultXMLContainerField() {
      return "textureProperties";
    }

    /// Render all OpenGL texture properties that are set by glTexParamter
    /// calls for the given texture_target.
    virtual void renderTextureProperties( GLenum texture_target );


    /// The anisotropicDegree field describes the minimum degree of anisotropy
    /// to account for in texture filtering. A value of 1 implies no
    /// anisotropic filtering. Values above the system's maximum supported
    /// value will be clamped to the maximum allowed. 
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 1.0 \n
    ///
    /// \dotfile TextureProperties_anisotropicDegree.dot 
    auto_ptr< SFFloat >  anisotropicDegree;

    /// The borderColor field describes the color to use for border pixels.
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 0 0 0 0  \n
    ///
    /// \dotfile TextureProperties_borderColor.dot 
    auto_ptr< SFColorRGBA >  borderColor;

    /// The borderWidth field describes the color to use for border pixels.
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 0 \n
    ///
    /// \dotfile TextureProperties_borderWidth.dot 
    auto_ptr< SFInt32 >  borderWidth;

    /// The boundaryModeS field describes the way S texture coordinate 
    /// boundaries are handled
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> "REPEAT" \n
    ///
    /// \dotfile TextureProperties_boundaryModeS.dot 
    auto_ptr< SFString > boundaryModeS;

    /// The boundaryModeT field describes the way T texture coordinate 
    /// boundaries are handled
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> "REPEAT" \n
    ///
    /// \dotfile TextureProperties_boundaryModeT.dot 
    auto_ptr< SFString > boundaryModeT;

    /// The boundaryModeR field describes the way R texture coordinate 
    /// boundaries are handled
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> "REPEAT" \n
    ///
    /// \dotfile TextureProperties_boundaryModeR.dot 
    auto_ptr< SFString > boundaryModeR;
    
    /// The magnificationFilter field describes the way textures are filtered
    /// when the image is smaller then the screen space representation.   
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> "FASTEST" \n
    ///
    /// \dotfile TextureProperties_magnificationFilter.dot 
    auto_ptr< SFString >  magnificationFilter;

    /// The minificationFilter field describes the way textures are filtered
    /// when the image is larger then the screen space representation.   
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> "FASTEST" \n
    ///
    /// \dotfile TextureProperties_minificationFilter.dot 
    auto_ptr< SFString >  minificationFilter;

    /// The textureCompression fields describes if/how a texture should be 
    /// compressed.
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> "FASTEST" \n
    ///
    /// \dotfile TextureProperties_textureCompression.dot 
    auto_ptr< SFString >  textureCompression;

    /// The texturePriority field describes the texture residence priority for
    /// allocating texture memory. Zero indicates the lowest priority and 1
    /// indicates the highest priority. Values are clamped to the range [0,1].
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 0 \n
    ///
    /// \dotfile TextureProperties_texturePriority.dot 
    auto_ptr< SFFloat >  texturePriority;

    /// The generateMipMaps field describes whether mipmaps should be generated
    /// for the texture. Mipmaps are required for filtering modes with MIPMAP
    /// in their value.
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> false \n
    ///
    /// \dotfile TextureProperties_generateMipMaps.dot 
    auto_ptr< SFBool >  generateMipMaps;


    /// The textureTransferScale field defines the scale component of the 
    /// transfer function from image value to texture memory value.
    ///
    /// When the image data is transferred to texture memory a scaling factor
    /// and bias value can be added to each pixel value before putting it into 
    /// texture space. The four values are mapped to r, g, b and alpha 
    /// components respectively. This does not affect the values in the 
    /// image field of the texture, but only the values in texture memory.
    /// E.g.
    /// texture_memory_value.r = 
    ///       (image_value.r * textureTransferScale.x) + textureTransferBias.x 
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 1 1 1 1 \n
    ///
    /// \dotfile TextureProperties_textureTransferScale.dot  
    auto_ptr< SFVec4f > textureTransferScale;

    /// The textureTransferBias field defines the bias component of the 
    /// transfer function from image value to texture memory value.
    ///
    /// When the image data is transferred to texture memory a scaling factor
    /// and bias value can be added to each pixel value before putting it into 
    /// texture space. The four values are mapped to r, g, b and alpha 
    /// components respectively. This does not affect the values in the 
    /// image field of the texture, but only the values in texture memory.
    /// E.g.
    /// texture_memory_value.r = 
    ///       (image_value.r * textureTransferScale.x) + textureTransferBias.x 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 0 0 0 0 \n
    ///
    /// \dotfile TextureProperties_textureTransferBias.dot  
    auto_ptr< SFVec4f > textureTransferBias;


    /// Field that gets an event when any of the X3D fields in the
    /// TextureProperties generates an event
    auto_ptr< Field > propertyChanged;

    /// The textureCompareMode field specifies what texture compare
    /// function to use. Texture compare is used to compare the 
    /// R texture coordinates to the texture value and return 
    /// 1 if the test succeeds and textureCompareFailValue on fail.
    /// The output of a texture lookup with this enabled will 
    /// there for only return 1 or textureCompareFailValue depending
    /// on the test. The original texture value is only used for the
    /// test. The normal usage of this is when using shadow mapping
    /// to determine if a fragment is in shadow or not.
    /// 
    /// The valid values are:
    /// <table>
    /// <tr><td>"NONE"</td><td>No texture compare is done. Texture works as 
    /// normal.</td></tr>
    /// <tr><td>"GEQUAL"</td><td>Output texture value is 1 if R texture 
    /// coordinate >= original texture value, textureCompareFailValue otherwise.</td> 
    /// <tr><td>"LEQUAL"</td><td>Output texture value is 1 if R texture 
    /// coordinate <= original texture value, textureCompareFailValue otherwise.</td> 
    /// </table>
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Valid values:</b> "NONE", "GEQUAL", "LEQUAL" \n
    /// <b>Default value:</b> "NONE" \n
    ///
    /// \dotfile TextureProperties_textureCompareMode.dot   
    auto_ptr< SFString > textureCompareMode;

    /// The textureCompareFailValue contains the value to use as texture
    /// output when the test in textureCompareMode fails.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 0 \n
    ///
    /// \dotfile TextureProperties_textureCompareMode.dot  
    auto_ptr< SFFloat > textureCompareFailValue;


    /// The textureType specifies what texture target type
    /// should be used for the texture. This can affect how texture
    /// values are interpolated and how texture coordinates are 
    /// used to look up into the texture.
    /// 
    /// The valid values are:
    /// <table>
    /// <tr><td>"NORMAL"</td><td>GL_TEXTURE_2D for 2d textures and GL_TEXTURE_3D for 3d 
    /// textures.</td></tr>
    /// <tr><td>"2D_RECTANGLE"</td><td>GL_TEXTURE_RECTANGLE. Only valid for 2D textures. 
    /// Make the texture lookup in pixel space instead of as normal with normalized
    /// coordinates. E.g. tex coord (4,5) will look up the pixel with that index directly</td> </tr>
    /// <tr><td>"2D_ARRAY"</td><td>GL_TEXTURE_2D_ARRAY. Only valid for 3D textures and together with shaders. 
    /// Texture is a stack of 2D images where the R component specifies the
    /// index of the 2D image to use. </td> </tr>
    /// </table>
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Valid values:</b> "NORMAL", "2D_RECTANGLE"(if 2d texture), "2D_ARRAY"(if 3d texture) \n
    /// <b>Default value:</b> "NORMAL" \n
    ///
    /// \dotfile TextureProperties_textureType.dot     
    auto_ptr< SFString > textureType;
    
    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  };
}

#endif
