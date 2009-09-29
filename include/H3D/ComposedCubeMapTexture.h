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
/// \file ComposedCubeMapTexture.h
/// \brief Header file for ComposedCubeMapTexture, X3D abstract scene-graph 
/// node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __COMPOSEDCUBEMAPTEXTURE_H__
#define __COMPOSEDCUBEMAPTEXTURE_H__

#include <H3D/X3DEnvironmentTextureNode.h>
#include <H3D/X3DTexture2DNode.h>
#include <H3D/DependentNodeFields.h>

namespace H3D {

  /// \ingroup X3DNodes
  /// \class ComposedCubeMapTexture
  /// The ComposedCubeMapTexture node defines a cubic environment map
  /// source as an explicit set of images drawn from individual 2D texture 
  /// descriptions.
  /// 
  /// \par Internal routes:
  /// \dotfile ComposedCubeMapTexture.dot
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/ComposedCubeMapTexture.x3d">ComposedCubeMapTexture.x3d</a>
  ///     ( <a href="examples/ComposedCubeMapTexture.x3d.html">Source</a> )
  class H3DAPI_API ComposedCubeMapTexture : public X3DEnvironmentTextureNode {
  public:
    /// The SFTexture2DNode field is dependent on the displayList field
    /// of the containing X3DTexture2DNode node.
    typedef DependentSFNode< X3DTexture2DNode, 
                             FieldRef< H3DDisplayListObject,
                                       H3DDisplayListObject::DisplayList,
                                       &H3DDisplayListObject::displayList >, 
                             true >
    SFTexture2DNode;

    /// Constructor.
    ComposedCubeMapTexture( Inst< DisplayList      > _displayList = 0,
                            Inst< SFNode           > _metadata = 0,
                            Inst< SFTexture2DNode  > _back   = 0,
                            Inst< SFTexture2DNode  > _front  = 0,
                            Inst< SFTexture2DNode  > _left   = 0,
                            Inst< SFTexture2DNode  > _right  = 0,
                            Inst< SFTexture2DNode  > _top    = 0,
                            Inst< SFTexture2DNode  > _bottom = 0 );

    /// Enables cube map texturing.
    virtual void enableTexturing();

    /// Disables cube map texturing.
    virtual void disableTexturing();

    /// Creates the cube map from the texture fields.
    virtual void render();

    /// The texture to use on the back face of the cube.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// 
    /// \dotfile ComposedCubeMapTexture_back.dot
    auto_ptr< SFTexture2DNode >  back;

    /// The texture to use on the front face of the cube.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// 
    /// \dotfile ComposedCubeMapTexture_front.dot
    auto_ptr< SFTexture2DNode >  front;

    /// The texture to use on the left face of the cube.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// 
    /// \dotfile ComposedCubeMapTexture_left.dot
    auto_ptr< SFTexture2DNode >  left;

    /// The texture to use on the right face of the cube.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// 
    /// \dotfile ComposedCubeMapTexture_right.dot
    auto_ptr< SFTexture2DNode >  right;

    /// The texture to use on the top face of the cube.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// 
    /// \dotfile ComposedCubeMapTexture_top.dot
    auto_ptr< SFTexture2DNode >  top;

    /// The texture to use on the bottom face of the cube.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// 
    /// \dotfile ComposedCubeMapTexture_bottom.dot
    auto_ptr< SFTexture2DNode >  bottom;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;

  };
}

#endif
