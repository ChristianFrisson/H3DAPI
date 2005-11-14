//////////////////////////////////////////////////////////////////////////////
//    Copyright 2004, SenseGraphics AB
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
#include "X3DTextureNode.h"
#include "DependentNodeFields.h"

namespace H3D {
  /// \ingroup Nodes 
  /// \class MultiTexture
  /// Partially implemented.
  ///
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
  /// - The mode field controls the type of blending operation. The only
  ///   available mode at this time is "MODULATE".
  /// - The function field defines an optional function to be applied to
  ///   the argument after the mode has been evaluated.
  ///   This is not supported at this time.
  /// - The source field determines the colour source for the second argument.
  ///   This is not supported at this time.
  /// - The texture field contains a list of X3DTextureNode instances 
  ///   (e.g., ImageTexture, PixelTexture and MultiTexture). 
  ///   The texture field may not contain another MultiTexture node.
  class H3DAPI_API MultiTexture : public X3DTextureNode {
  public:
    
    H3D_API_EXCEPTION( MultiTexturesNotSupported );

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
    auto_ptr< SFFloat >  alpha;
    
    /// Base alpha value for SELECT mode operations.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> RGB( 1, 1, 1 ) \n
    /// <b>Valid range:</b> [0-1]
    auto_ptr< SFColor >  color;

    /// The function field defines an optional function to be applied to
    /// the argument after the mode has been evaluated.
    /// This is not supported at this time.
    /// 
    /// <b>Access type:</b> inputOutput \n
    auto_ptr< MFString >  function;

    /// The mode field controls the type of blending operation. The only
    /// available mode at this time is "MODULATE".
    /// 
    /// <b>Access type:</b> inputOutput \n
    auto_ptr< MFString >  mode;

    /// The source field determines the colour source for the second argument.
    /// This is not supported at this time.
    ///
    /// <b>Access type:</b> inputOutput \n
    auto_ptr< MFString >  source;

    /// The texture field contains a list of X3DTextureNode instances 
    /// (e.g., ImageTexture, PixelTexture and MultiTexture). 
    /// The texture field may not contain another MultiTexture node.
    ///
    /// <b>Access type:</b> inputOutput \n
    auto_ptr< MFTexture >  texture;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;

private:
    // variable that is false until a check that the graphics card 
    // driver has support for multi textures is performed. 
    static bool multitexture_support_checked;
  };
}

#endif
