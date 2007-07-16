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
/// \file X3DTextureTransform2DNode.h
/// \brief Header file for X3DTextureTransform2DNode, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __X3DTEXTURETRANSFORM2DNODE_H__
#define __X3DTEXTURETRANSFORM2DNODE_H__

#include <X3DTextureTransformNode.h>
#include <SFVec2f.h>
#include <SFFloat.h>

namespace H3D {

  /// \ingroup AbstractNodes  
  /// \class X3DTextureTransform2DNode
  /// \brief This abstract node type is the base type for all node types which
  /// specify a 2D transformation of texture coordinates. 
  ///
  /// \par Internal routes:
  /// \dotfile X3DTextureTransform2DNode.dot
  class H3DAPI_API X3DTextureTransform2DNode : public X3DTextureTransformNode {
  public:
    
    /// Constructor.
    X3DTextureTransform2DNode( Inst< DisplayList > _displayList = 0,
                               Inst< SFNode >  _metadata    = 0,
                               Inst< SFVec2f>  _center      = 0,
                               Inst< SFFloat>  _rotation    = 0,
                               Inst< SFVec2f>  _scale       = 0,
                               Inst< SFVec2f>  _translation = 0 );

    /// The offset in texture coordinate space about which the rotation
    /// and scale fields are applied.
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> Vec2f( 0, 0 ) \n
    /// 
    /// \dotfile X3DTextureTransform2DNode_center.dot
    auto_ptr< SFVec2f >  center;

    /// Specifies a rotation in radians of the texture coordinates about
    /// the center point after the scale has been applied.
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 0 \n
    /// 
    /// \dotfile X3DTextureTransform2DNode_rotation.dot
    auto_ptr< SFFloat >  rotation;

    /// Specifies a scaling factor in S and T of the texture coordinates
    /// about the center point.
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> Vec2f( 1, 1 ) \n
    /// 
    /// \dotfile X3DTextureTransform2DNode_scale.dot
    auto_ptr< SFVec2f >  scale;
    
    /// Specifies a translation of the texture coordinates.
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> Vec2f( 0, 0 ) \n
    /// 
    /// \dotfile X3DTextureTransform2DNode_translation.dot
    auto_ptr< SFVec2f >  translation;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  };
}

#endif
