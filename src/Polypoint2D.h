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
/// \file Polypoint2D.h
/// \brief Header file for Polypoint2D, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __POLYPOINT2D_H__
#define __POLYPOINT2D_H__

#include "X3DGeometryNode.h"

namespace H3D {

  /// \ingroup Nodes
  /// \class Polypoint2D
  /// \brief The Polyline2D node specifies a set of vertices in the local 2D
  /// coordinate system at each of which is displayed a point.  
  /// 
  /// The points field specifies the vertices to be displayed.
  /// 
  /// \par Internal routes:
  /// \dotfile Polypoint2D.dot
  
  class H3DAPI_API Polypoint2D : 
    public X3DGeometryNode {
  public:

    /// SFBound is specialized update itself from the radius field 
    /// of the Box node.
    ///
    /// routes_in[0] is the size field of the Box node.  
    ///
    class SFBound: public TypedField< X3DGeometryNode::SFBound,
    MFVec2f > {
      /// Update the bound from the radius field. 
      virtual void update() {
        const vector< Vec2f > &points = 
          static_cast< MFVec2f * >( routes_in[0] )->getValue();
        BoxBound *bound = new BoxBound();
        bound->fitAround2DPoints( points.begin(), points.end() );
        value = bound;
      }
    };

    /// Constructor.
    Polypoint2D( Inst< SFNode      > _metadata = 0,
                 Inst< SFBound     > _bound = 0,
                 Inst< DisplayList > _displayList = 0,
                 Inst< MFBool      > _isTouched = 0,
                 Inst< MFVec3f     > _force = 0,
                 Inst< MFVec3f     > _contactPoint = 0,
                 Inst< MFVec3f     > _contactNormal = 0,
                 Inst< MFVec2f     > _point = 0 );
   
    /// Renders the Polypoint2D using OpenGL.
    virtual void render();

    /// Traverse the scenegraph. A HLFeedbackShape is added for haptic
    /// rendering if haptics is enabled.
    virtual void traverseSG( TraverseInfo &ti );  

    /// The point field specifies the vertices to be displayed.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// 
    /// \dotfile Polypoint2D_size.dot
    auto_ptr< MFVec2f > point;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  };
}

#endif
