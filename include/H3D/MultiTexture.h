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
/// \file MultiTexture.h
/// \brief Header file for MultiTexture, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __MULTITEXTURE_H__
#define __MULTITEXTURE_H__

#include <iostream>
#include <H3D/X3DTextureNode.h>
#include <H3D/DependentNodeFields.h>
#include <H3D/SFFloat.h>
#include <H3D/SFColor.h>
#include <H3D/MFString.h>

namespace H3D {
  /// \ingroup X3DNodes 
  /// \class MultiTexture
  /// MultiTexture enables the application of several individual textures
  /// to a 3D object to achieve a more complex visual effect. 
  /// MultiTexture can be used as a value for the texture field in an 
  /// Appearance node.
  /// The texture field contains a list of texture nodes 
  /// (e.g., ImageTexture, PixelTexture, MovieTexture, and MultiTexture). 
  /// The texture field may not contain another MultiTexture node.
  ///
  /// - The color and alpha fields define base RGB and alpha values for SELECT
  ///   mode operations.
  /// - The mode field controls the type of blending operation. 
  ///   "MODULATE" Multiply texture color with current color. Arg1 * Arg2 
  ///   "REPLACE" Replace current color. Arg2.
  ///   "MODULATE2X" Multiply the components of the arguments, and shift 
  ///   the products to the left 1 bit (effectively multiplying them by 2) 
  ///   for brightening.
  ///   "MODULATE4X" Multiply the components of the arguments, and shift 
  ///   the products to the left 2 bits (effectively multiplying them by 4)
  ///   for brightening.
  ///   "ADD" Add the components of the arguments. Arg1 + Arg2
  ///   "ADDSIGNED" Add the components of the arguments with a -0.5 bias,
  ///   making the effective range of values from -0.5 through 0.5.
  ///   "ADDSIGNED2X" Add the components of the arguments with a -0.5 bias,
  ///   and shift the products to the left 1 bit. 
  ///   "SUBTRACT" Subtract the components of the second argument from those
  ///   of the first argument. Arg1 - Arg2.
  ///   "BLENDDIFFUSEALPHA" Linearly blend this texture stage, using the 
  ///   interpolated alpha from each vertex. Arg1 * (Alpha) + Arg2 * (1-Alpha).
  ///   "BLENDTEXTUREALPHA" Linearly blend this texture stage, using the alpha
  ///   from this stage's texture. Arg1 * (Alpha) + Arg2 * (1 - Alpha).
  ///   "BLENDFACTORALPHA" Linearly blend this texture stage, using the alpha
  ///   factor from the MultiTexture node. Arg1 × (Alpha) + Arg2 × (1 = Alpha).
  ///   "BLENDCURRENTALPHA" Linearly blend this texture stage, using the alpha
  ///   taken from the previous texture stage. 
  ///   Arg1 * (Alpha)+Arg2*(1 - Alpha)
  ///   "OFF" Turn off the texture unit
  ///   "SELECTARG1" Use color argument 1. Arg1.
  ///   "SELECTARG2" Use color argument 2. Arg2.
  ///   "DOTPRODUCT3" Modulate the components of each argument (as signed 
  ///   components), add their products, then replicate the sum to all 
  ///   color channels, including alpha.
  ///   This can do either diffuse or specular bump mapping with correct
  ///   input. Performs the function 
  ///   (Arg1.R × Arg2.R + Arg1.G × Arg2.G + Arg1.B × Arg2.B) where each
  ///   component has been scaled and offset to make it signed. The result 
  ///   is replicated into all four (including alpha) channels.
  /// - The function field defines an optional function to be applied to
  ///   the argument after the mode has been evaluated.
  ///   "" (default) No function is applied.
  ///   "COMPLEMENT" Invert the argument so that, if the result of the 
  ///   argument were referred to by the variable x, the value would be
  ///   1.0 minus x.
  ///   "ALPHAREPLICATE" Replicate the alpha information to all color 
  ///   channels before the operation completes.
  /// - The source field determines the colour source for the second argument.
  ///   "" (default) The second argument color (ARG2) is the color from the
  ///   previous rendering stage (DIFFUSE for first stage).
  ///   "DIFFUSE" The texture argument is the diffuse color interpolated
  ///   from vertex components during Gouraud shading.
  ///   "FACTOR" The texture argument is the factor (color, alpha) from
  ///   the MultiTexture node.
  /// - The texture field contains a list of X3DTextureNode instances 
  ///   (e.g., ImageTexture, PixelTexture and MultiTexture). 
  ///   The texture field may not contain another MultiTexture node.
  ///
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/MultiTexture.x3d">MultiTexture.x3d</a>
  ///     ( <a href="examples/MultiTexture.x3d.html">Source</a> )
  ///
  /// \par Internal routes:
  /// \dotfile MultiTexture.dot
  class H3DAPI_API MultiTexture : public X3DTextureNode {
  public:
    
    /// An MFNode where we make sure the type of the nodes contained
    /// is a subclass of X3DTextureNode.
    typedef DependentMFNode< X3DTextureNode, 
                             FieldRef< H3DDisplayListObject,
                                       H3DDisplayListObject::DisplayList,
                                       &H3DDisplayListObject::displayList >,
                             true >
    MFTexture;

    /// Constructor.
    MultiTexture( 
                 Inst< DisplayList > _displayList = 0,
                 Inst< SFFloat  >  _alpha    = 0,
                 Inst< SFColor  >  _color    = 0,
                 Inst< MFString >  _function = 0,
                 Inst< SFNode   >  _metadata = 0,
                 Inst< MFString >  _mode     = 0,
                 Inst< MFString >  _source   = 0,
                 Inst< MFTexture >  _texture = 0 );

    /// Install the textures in OpenGL.
    virtual void render();

    /// Enable texturing for all textures in the MultiTexture.
    virtual void enableTexturing();

    /// Disable texturing for all textures in the MultiTexture.
    virtual void disableTexturing();

    /// Base alpha value for SELECT mode operations.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 1 \n
    /// <b>Valid range:</b> [0-1]
    ///
    /// \dotfile MultiTexture_alpha.dot
    auto_ptr< SFFloat >  alpha;
    
    /// Base alpha value for SELECT mode operations.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> RGB( 1, 1, 1 ) \n
    /// <b>Valid range:</b> [0-1]
    ///
    /// \dotfile MultiTexture_color.dot
    auto_ptr< SFColor >  color;

    /// The function field defines an optional function to be applied to
    /// the argument after the mode has been evaluated.
    /// This is not supported at this time.
    /// 
    /// <b>Access type:</b> inputOutput \n
    ///
    /// \dotfile MultiTexture_function.dot
    auto_ptr< MFString >  function;

    /// The mode field controls the type of blending operation. The only
    /// available mode at this time is "MODULATE".
    /// 
    /// <b>Access type:</b> inputOutput \n
    ///
    /// \dotfile MultiTexture_mode.dot
    auto_ptr< MFString >  mode;

    /// The source field determines the colour source for the second argument.
    /// This is not supported at this time.
    ///
    /// <b>Access type:</b> inputOutput \n
    ///
    /// \dotfile MultiTexture_source.dot
    auto_ptr< MFString >  source;

    /// The texture field contains a list of X3DTextureNode instances 
    /// (e.g., ImageTexture, PixelTexture and MultiTexture). 
    /// The texture field may not contain another MultiTexture node.
    ///
    /// <b>Access type:</b> inputOutput \n
    ///
    /// \dotfile MultiTexture_texture.dot
    auto_ptr< MFTexture >  texture;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;

  };
}

#endif
