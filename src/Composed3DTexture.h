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
/// \file Composed3DTexture.h
/// \brief Header file for Composed3DTexture, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __PIXEL3DTEXTURE_H__
#define __PIXEL3DTEXTURE_H__

#include "X3DTexture3DNode.h"
#include "DependentNodeFields.h"
#include "X3DTexture2DNode.h"

namespace H3D {
  /// \ingroup Nodes 
  /// \class Composed3DTexture
  /// \brief The Composed3DTexture node defines a 3D image-based texture
  /// map as a  collection of 2D texture sources at various depths
  /// and parameters controlling tiling repetition of the texture
  /// onto geometry.
  ///
  /// The texture values are interpreted with the first image being at
  /// depth 0 and each following image representing an increasing depth 
  /// value in the R direction. A user shall provide 2^n source textures
  /// in this array. The individual source textures will ignore their repeat
  /// field values.
  ///
  /// \par Internal routes:
  /// \dotfile Composed3DTexture.dot  
  class H3DAPI_API Composed3DTexture : public X3DTexture3DNode {
  public:
    /// The MFTexture2DNode field is dependent on the displayList field
    /// of the containing X3DTextureNode node.
    typedef DependentMFNode< X3DTexture2DNode, 
                             FieldRef< H3DDisplayListObject,
                                       H3DDisplayListObject::DisplayList,
                                       &H3DDisplayListObject::displayList >, 
                             true >
    MFTexture2DNode;

    /// Constructor.
    Composed3DTexture( 
                 Inst< DisplayList     > _displayList = 0,
                 Inst< SFNode          > _metadata  = 0,
                 Inst< SFBool          > _repeatS   = 0,
                 Inst< SFBool          > _repeatT   = 0,
                 Inst< SFBool          > _repeatR   = 0,
                 Inst< SFBool          > _scaleToP2 = 0,
                 Inst< SFImage         > _image     = 0,
                 Inst< SFBool          > _interpolate = 0,
                 Inst< MFTexture2DNode > _texture   = 0);

    /// Ignores the image given, and uses the textures
    /// in the texture field to create a 3d texture instead.
    void glTexImage( Image *i, GLenum texture_target, 
                     bool scale_to_power_of_two );
    
    /// Renders the texture with OpenGL. 
    void render() ;

    /// The 2d textures to compose into a 3d texture.
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// 
    /// \dotfile Composed3DTexture_texture.dot
    auto_ptr< MFTexture2DNode >  texture;    

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  };
}

#endif
