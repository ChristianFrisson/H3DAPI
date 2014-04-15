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
/// \file H3DShadowObjectNode.h
/// \brief Header file for H3DShadowObjectNode.
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __H3DSHADOWOBJECTNODE_H__
#define __H3DSHADOWOBJECTNODE_H__

#include <H3D/X3DNode.h>
#include <H3D/X3DLightNode.h>
#include <H3D/MatrixTransform.h>

namespace H3D {

  /// \ingroup AbstractNodes
  /// \class H3DShadowObjectNode
  /// The H3DShadowObjectNode is the base class for all shadow objects 
  /// for use in the ShadowCaster node.
  ///
  /// The purpose of a shadow object is to draw a shadow volume, i.e.
  /// a triangle mesh towards infinity that encompasses the volume that
  /// is in shadow. E.g. a shadow object that is a sphere and a directinal light
  /// source would create a shadow volume that is a cylinder starting at the 
  /// position of the sphere and going off into infinity in the same direction
  /// as the light direction.
  class H3DAPI_API H3DShadowObjectNode : public X3DNode {
  public:
    typedef TypedSFNode< MatrixTransform > SFTransformNode;

    /// Constructor.
    H3DShadowObjectNode( Inst< SFNode > _metadata = 0,
                         Inst< SFTransformNode > _transform = 0 );

    /// Abstract function for drawing the shadow volume for a given light.
    /// render_caps determines if one should draw end caps to the volume
    /// or if it is ok to leave it open.
    virtual void renderShadow( X3DLightNode *l, 
                               bool render_caps,
                               const Matrix4f &local_to_global = Matrix4f() ) = 0;

    /// Returns the default xml containerField attribute value.
    /// For this node it is "children".
    ///
    virtual string defaultXMLContainerField() {
      return "object";
    }
 
    /// The transform field specifies a transformation of the shadow volume
    /// object.
    ///
    /// <b>Access type:</b> inputOutput \n
    auto_ptr< SFTransformNode > transform;

    /// The H3DNodeDatabase for the node
    static H3DNodeDatabase database;
 };
}

#endif
