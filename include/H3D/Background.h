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
/// \file Background.h
/// \brief Header file for Background, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __BACKGROUND_H__
#define __BACKGROUND_H__

#include <H3D/X3DBackgroundNode.h>
#include <H3D/ImageTexture.h>

namespace H3D {

  /// \ingroup X3DNodes
  /// \class Background
  /// \brief A background node that uses six static images to compose the 
  /// backdrop.
  ///
  /// The backUrl, bottomUrl, frontUrl, leftUrl, rightUrl, and topUrl fields
  /// specify the url to a set of images that define a background panorama
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
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/Background.x3d">Background.x3d</a>
  ///     ( <a href="examples/Background.x3d.html">Source</a> )
  ///   - <a href="../../../H3DAPI/examples/All/Background1.x3d">Background1.x3d</a>
  ///     ( <a href="examples/Background1.x3d.html">Source</a> )
  ///
  /// \par Internal routes:
  /// \dotfile Background.dot
  class H3DAPI_API Background : 
    public X3DBackgroundNode {
  public:
    
    /// Constructor.
    Background( Inst< SFSetBind > _set_bind    = 0,
    Inst< SFNode    > _metadata    = 0,
    Inst< SFTime    > _bindTime    = 0,
    Inst< SFBool    > _isBound     = 0,
    Inst< DisplayList > _displayList = 0,
    Inst< MFFloat   > _groundAngle = 0,
    Inst< MFColor   > _groundColor = 0,
    Inst< MFFloat   > _skyAngle    = 0,
    Inst< MFColor   > _skyColor    = 0,
    Inst< MFString  > _backUrl     = 0,
    Inst< MFString  > _frontUrl    = 0,
    Inst< MFString  > _leftUrl     = 0,
    Inst< MFString  > _rightUrl    = 0,
    Inst< MFString  > _topUrl      = 0,
    Inst< MFString  > _bottomUrl   = 0,
    Inst< SFFloat   > _transparency   = 0 );

    /// Renders the background with OpenGL.
    virtual void render();

    /// The url of the texture to use on the back face of the cube.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// 
    /// \dotfile Background_backUrl.dot
    auto_ptr< MFString >  backUrl;

    /// The url of the texture to use on the front face of the cube.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// 
    /// \dotfile Background_frontUrl.dot
    auto_ptr< MFString >  frontUrl;

    /// The url of the texture to use on the left face of the cube.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// 
    /// \dotfile Background_leftUrl.dot
    auto_ptr< MFString >  leftUrl;

    /// The url of the texture to use on the right face of the cube.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// 
    /// \dotfile Background_rightUrl.dot
    auto_ptr< MFString >  rightUrl;

    /// The url of the texture to use on the top face of the cube.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// 
    /// \dotfile Background_topUrl.dot
    auto_ptr< MFString >  topUrl;

    /// The url of the texture to use on the bottom face of the cube.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// 
    /// \dotfile Background_bottomUrl.dot
    auto_ptr< MFString >  bottomUrl;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  protected:
    /// The texture displayed at the back.
    AutoRef< ImageTexture > backTexture;
    /// The texture displayed at the front.
    AutoRef< ImageTexture > frontTexture;
    /// The texture displayed to the left.
    AutoRef< ImageTexture > leftTexture;
    /// The texture displayed to the right.
    AutoRef< ImageTexture > rightTexture;
    /// The texture displayed at the top.
    AutoRef< ImageTexture > topTexture;
    /// The texture displayed at the bottom.
    AutoRef< ImageTexture > bottomTexture;
  };
}

#endif
