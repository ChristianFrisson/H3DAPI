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
/// \file MultiTextureTransform.h
/// \brief Header file for MultiTextureTransform, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __MULTITEXTURETRANSFORM_H__
#define __MULTITEXTURETRANSFORM_H__

#include <H3D/X3DTextureTransformNode.h>
#include <H3D/DependentNodeFields.h>

namespace H3D {

  /// \class MultiTextureTransform
  /// \brief MultiTextureTransform supplies multiple texture transforms per 
  /// appearance. 
  /// This node can be used to set the texture transform for each of the 
  /// different texture channels. MultiTextureTransform may appear in the
  /// scene graph wherever any X3DTextureTransformNode appears.
  ///
  /// Each entry in the textureTransform field shall contain an 
  /// X3DTextureTransformNode or NULL.
  ///
  /// If using MultiTexture with an IndexedFaceSet without a 
  /// MultiTextureTransform node, texture coordinates for channel 0 are
  /// replicated along the other channels. Similarly, if there are too few
  /// entries in the textureTransform field, the last entry is replicated.
  ///
  /// Example:
  /// \code
  /// Shape { 
  ///   appearance Appearance { 
  ///     texture MultiTexture {  
  ///       mode [ "MODULATE" "MODULATE" ] 
  ///       texture [ 
  ///         ImageTexture { url "brick.jpg")  
  ///         ImageTexture { repeatS FALSE repeatT FALSE url "light_gray.png"} 
  ///       ]
  ///     }
  ///
  ///     textureTransform MultiTextureTransform {
  ///       textureTransform [  
  ///         TextureTransform {}  
  ///         TextureTransform { scale 0.5 0.5 } 
  ///       ]
  ///     }  
  ///   }
  /// }
  /// \endcode
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/MultiTextureTransform.x3d">MultiTextureTransform.x3d</a>
  ///     ( <a href="examples/MultiTextureTransform.x3d.html">Source</a> )
  class H3DAPI_API MultiTextureTransform : public X3DTextureTransformNode {
  public:
    
    /// The MFTextureTransformNode field is dependent on the displayList field
    /// of the containing X3DTextureTransformNode node.
    typedef DependentMFNode< X3DTextureTransformNode, 
                             FieldRef< H3DDisplayListObject,
                                       H3DDisplayListObject::DisplayList,
                                       &H3DDisplayListObject::displayList >,
                             true >
    MFTextureTransformNode;

    /// Constructor.
    MultiTextureTransform( Inst< DisplayList            > _displayList = 0,
                           Inst< SFNode                 > _metadata    = 0,
                           Inst< MFTextureTransformNode > _textureTransform=0 );

    /// This function all is not allowed. Use renderForTextureuUnit() or 
    /// renderForTextureUnits() instead. Call will raise an exception.
    virtual void render();

    /// Render the texture transform for the currently active  texture unit.
    virtual void renderForTextureUnit( unsigned int texture_unit );
    
    /// Render the texture transform for the texture units between and
    /// inluding start_unit and end_unit.
    virtual void renderForTextureUnits( unsigned int start_unit,
                                        unsigned int end_unit );

    /// The X3DTextureTransform nodes to use for each texture unit.
    /// 
    /// <b>Access type:</b> inputOutput \n
    auto_ptr< MFTextureTransformNode > textureTransform;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  };
}

#endif
