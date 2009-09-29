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
/// \file TextureBackground.h
/// \brief Header file for TextureBackground, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __TEXTUREBACKGROUND_H__
#define __TEXTUREBACKGROUND_H__

#include <H3D/X3DBackgroundNode.h>
#include <H3D/X3DTextureNode.h>
#include <H3D/X3DTextureCoordinateNode.h>
#include <H3D/DependentNodeFields.h>
#include <H3D/SFFloat.h>

namespace H3D {

  /// \ingroup X3DNodes
  /// \class TextureBackground
  /// \brief A background node that uses six individual texture nodes to
  /// compose the backdrop. Unlike the Background node, which only supports
  /// static image formats referenced by URL fields, the contents of the
  /// TextureBackground node can be arbitrary X3DTextureNode nodes.
  ///
  /// The backTexture, bottomTexture, frontTexture, leftTexture, 
  /// rightTexture, and topTexture fields specify a set of textures that
  /// define a background panorama
  /// between the ground/sky backdrop and the scene's geometry. The panorama
  /// consists of six images, each of which is mapped onto a face of an 
  /// infinitely large cube contained within the backdrop spheres and centred
  /// in the local coordinate system. The images are applied individually
  /// to each face of the cube. On the front, back, right, and left faces 
  /// of the cube, when viewed from the origin looking down the negative
  /// Z-axis with the Y-axis as the view up direction, each image is mapped
  /// onto the corresponding face with the same orientation as if the image
  /// were displayed normally in 2D (back to back face, front to front face,
  /// left to left face, and right to right face). On the top face of the
  /// cube, when viewed from the origin looking along the +Y-axis with the 
  /// +Z-axis as the view up direction, the top image is mapped onto the face
  /// with the same orientation as if the image were displayed normally in 
  /// 2D. On the bottom face of the box, when viewed from the origin along
  /// the negative Y-axis with the negative Z-axis as the view up direction,
  /// the bottom image is mapped onto the face with the same orientation as
  ///if the image were displayed normally in 2D.
  ///
  /// Alpha values in the panorama images (i.e., two or four component images)
  /// specify that the panorama is semi-transparent or transparent in regions,
  /// allowing the groundColor and skyColor to be visible.
  ///
  /// Often, the bottom and top images will not be specified, to allow sky
  /// and ground to show. The other four images may depict surrounding
  /// mountains or other distant scenery. 
  /// 
  /// The other fields work just as in X3DBackgroundNode.
  /// 
  /// \par Internal routes:
  /// \dotfile TextureBackground.dot
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/TextureBackground.x3d">TextureBackground.x3d</a>
  ///     ( <a href="examples/TextureBackground.x3d.html">Source</a> )
  class H3DAPI_API TextureBackground : 
    public X3DBackgroundNode {
  public:
    
    /// The SFTextureNode field is dependent on the displayList field
    /// of the containing X3DTextureNode node.
    typedef DependentSFNode< X3DTextureNode, 
                             FieldRef< H3DDisplayListObject,
                                       H3DDisplayListObject::DisplayList,
                                       &H3DDisplayListObject::displayList >, 
                             true >
    SFTextureNode;

    /// Constructor.
    TextureBackground( Inst< SFSetBind > _set_bind    = 0,
		       Inst< SFNode    > _metadata    = 0,
		       Inst< SFTime    > _bindTime    = 0,
		       Inst< SFBool    > _isBound     = 0,
		       Inst< DisplayList > _displayList = 0,
		       Inst< MFFloat   > _groundAngle = 0,
		       Inst< MFColor   > _groundColor = 0,
		       Inst< MFFloat   > _skyAngle    = 0,
		       Inst< MFColor   > _skyColor    = 0,
		       Inst< SFTextureNode  > _backTexture     = 0,
		       Inst< SFTextureNode  > _frontTexture    = 0,
		       Inst< SFTextureNode  > _leftTexture     = 0,
		       Inst< SFTextureNode  > _rightTexture    = 0,
		       Inst< SFTextureNode  > _topTexture      = 0,
		       Inst< SFTextureNode  > _bottomTexture   = 0,
		       Inst< SFFloat        > _transparency    = 0 );

    
    /// Renders the background with OpenGL.
    virtual void render();

    /// The texture to use on the back face of the cube.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// 
    /// \dotfile TextureBackground_backTexture.dot
    auto_ptr< SFTextureNode >  backTexture;

    /// The texture to use on the front face of the cube.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// 
    /// \dotfile TextureBackground_frontTexture.dot
    auto_ptr< SFTextureNode >  frontTexture;

    /// The texture to use on the left face of the cube.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// 
    /// \dotfile TextureBackground_leftTexture.dot
    auto_ptr< SFTextureNode >  leftTexture;

    /// The texture to use on the right face of the cube.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// 
    /// \dotfile TextureBackground_rightTexture.dot
    auto_ptr< SFTextureNode >  rightTexture;

    /// The texture to use on the top face of the cube.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// 
    /// \dotfile TextureBackground_topTexture.dot
    auto_ptr< SFTextureNode >  topTexture;

    /// The texture to use on the bottom face of the cube.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// 
    /// \dotfile TextureBackground_bottomTexture.dot
    auto_ptr< SFTextureNode >  bottomTexture;

    /// The transparency field is not supported in H3D API.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// 
    /// \dotfile TextureBackground_transparency.dot
    auto_ptr< SFFloat >  transparency;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  protected:
    inline void renderTexCoordForTexture( const Vec3f &tc, 
                                          X3DTextureNode *t ) {
      X3DTextureCoordinateNode::renderTexCoordForTexture( tc, t );
    }
  };
}

#endif
