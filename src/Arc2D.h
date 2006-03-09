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
/// \file Arc2D.h
/// \brief Header file for Arc2D, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __ARC2D_H__
#define __ARC2D_H__

#include "X3DGeometryNode.h"
#include "SFFloat.h"

namespace H3D {

  /// \ingroup Nodes
  /// \class Arc2D
  /// \brief The Arc node specifies a linear circular arc whose center is 
  /// at (0,0) and whose angles are measured starting at the positive x-axis
  /// and sweeping towards the positive y-axis. 
  ///
  /// The radius field specifies the radius of the circle of which the arc
  /// is a portion. The arc extends from the startAngle counterclockwise
  /// to the endAngle. The value of radius shall be greater than zero. 
  /// The values of startAngle and endAngle shall be in the range (0, 2pi).
  /// If startAngle and endAngle have the same value, a circle is specified.
  /// 
  /// \par Internal routes:
  /// \dotfile Arc2D.dot
  
  class H3DAPI_API Arc2D : 
    public X3DGeometryNode {
  public:

    /// SFBound is specialized update itself from the radius field 
    /// of the Box node.
    ///
    /// routes_in[0] is the size field of the Box node.  
    ///
    class SFBound: public TypedField< X3DGeometryNode::SFBound,
    SFFloat > {
      /// Update the bound from the radius field. 
      virtual void update() {
        H3DFloat d = 2 * static_cast< SFFloat * >( routes_in[0] )->getValue();
        BoxBound *bb = new BoxBound;
        bb->size->setValue( Vec3f( d, d, 0.f ) );
        value = bb;
      }
    };

    /// Constructor.
    Arc2D( Inst< SFNode      > _metadata = 0,
           Inst< SFBound     > _bound = 0,
           Inst< DisplayList > _displayList = 0,
           Inst< MFBool      > _isTouched = 0,
           Inst< MFVec3f     > _force = 0,
           Inst< MFVec3f     > _contactPoint = 0,
           Inst< MFVec3f     > _contactNormal = 0,
           Inst< SFFloat     > _endAngle = 0,
           Inst< SFFloat     > _startAngle = 0,
           Inst< SFFloat     > _radius = 0 );
   
    /// Renders the Arc2D using OpenGL.
    virtual void render();

    /// Traverse the scenegraph. A HLFeedbackShape is added for haptic
    /// rendering if haptics is enabled.
    virtual void traverseSG( TraverseInfo &ti );  

    /// The end angle for the arc.  The arc extends from the startAngle
    /// counterclockwise to the endAngle. 
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> pi/2 \n
    /// 
    /// \dotfile Arc2D_endAngle.dot
    auto_ptr< SFFloat > endAngle;

    /// The start angle for the arc.  The arc extends from the startAngle
    /// counterclockwise to the endAngle. 
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 0 \n
    /// 
    /// \dotfile Arc2D_startAngle.dot
    auto_ptr< SFFloat > startAngle;

    /// The radius field specifies the radius of the circle of which 
    /// the arc is a portion.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 1 \n
    /// 
    /// \dotfile Arc2D_radius.dot
    auto_ptr< SFFloat > radius;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  };
}

#endif
