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
/// \file Extrusion.h
/// \brief Header file for Extrusion, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __EXTRUSION_H__
#define __EXTRUSION_H__

#include "X3DGeometryNode.h"
#include "SFInt32.h"
#include "SFFloat.h"
#include "MFRotation.h"
#include "MFVec2f.h"

namespace H3D {

  /// \ingroup X3DNodes
  /// \class Extrusion
  /// \brief
  /// \par Internal routes:
  /// \dotfile Extrusion.dot 
  class Extrusion : public X3DGeometryNode {
  public:

    /// Render the Extrusion with OpenGL.
    virtual void render();
		/// returns true if the two points are coincident
		inline bool coinc(Vec3f a, Vec3f b) { return H3DAbs( ( a - b ).lengthSqr()) < Constants::f_epsilon;}

    /// Constructor.
    Extrusion( Inst< SFNode           > _metadata        = 0,
               Inst< SFBound          > _bound           = 0,
               Inst< DisplayList      > _displayList     = 0,
               Inst< SFBool           > _beginCap				 = 0,
							 Inst< SFBool           > _ccw             = 0,
               Inst< SFBool           > _convex					 = 0,
               Inst< SFFloat          > _creaseAngle     = 0,
               Inst< MFVec2f          > _crossSection    = 0,
               Inst< SFBool						> _endCap	         = 0,
               Inst< MFRotation       > _orientation     = 0,
               Inst< MFVec2f          > _scale	         = 0,
               Inst< SFBool           > _solid           = 0,
							 Inst< MFVec3f          > _spine           = 0 );

    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> TRUE \n
    /// 
    /// \dotfile Extrusion_beginCap.dot 
    auto_ptr< SFBool >  beginCap;

	
	/// The ccw field defines the ordering of the vertex coordinates of 
    /// the geometry with respect to user-given or automatically generated
    /// normal vectors used in the lighting model equations. If ccw is TRUE,
    /// the normals shall follow the right hand rule; the orientation of
    /// each normal with respect to the vertices (taken in order) shall be
    /// such that the vertices appear to be oriented in a counterclockwise
    /// order when the vertices are viewed (in the local coordinate system
    /// of the Shape) from the opposite direction as the normal. If ccw is
    /// FALSE, the normals shall be oriented in the opposite direction. If
    /// normals are not generated but are supplied using a Normal node, and
    /// the orientation of the normals does not match the setting of the 
    /// ccw field, results are undefined.
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> TRUE \n
    /// 
    /// \dotfile Extrusion_ccw.dot 
    auto_ptr< SFBool >  ccw;
    
	/// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> TRUE \n
    /// 
    /// \dotfile Extrusion_convex.dot 
    auto_ptr< SFBool >  convex;

	/// The creaseAngle field affects how default normals are generated. 
    /// If the angle between the geometric normals of two adjacent faces is
    /// less than the crease angle, normals shall be calculated so that the
    /// faces are shaded smoothly across the edge; otherwise, normals shall
    /// be calculated so that a lighting discontinuity across the edge is 
    /// produced.
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 0 \n
    /// <b>Valid range:</b> [0-inf]
    ///
    /// \dotfile Extrusion_creaseAngle.dot 
    auto_ptr< SFFloat > creaseAngle;

	/// 
    /// <b>Access type:</b> inputOutput \n
	/// <b>Default value:</b> [1 1 1 -1 -1 -1 -1 1 1 1] \n
    /// 
    /// \dotfile Extrusion_crossSection.dot 
    auto_ptr< MFVec2f >  crossSection;

	/// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> TRUE \n
    /// 
    /// \dotfile Extrusion_endCap.dot 
    auto_ptr< SFBool >  endCap;

	/// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 0 0 1 0 \n
    /// 
    /// \dotfile Extrusion_orientation.dot 
    auto_ptr< MFRotation >  orientation;

	/// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 1 1 \n
    /// 
    /// \dotfile Extrusion_scale.dot 
    auto_ptr< MFVec2f >  scale;

    /// The solid field determines whether one or both sides of each polygon
    /// shall be displayed. If solid is FALSE, each polygon shall be visible
    /// regardless of the viewing direction (i.e., no backface culling shall
    /// be done, and two sided lighting shall be performed to illuminate both
    /// sides of lit surfaces). If solid is TRUE, back face culling will be 
    /// performed.
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> TRUE \n
    ///
    /// \dotfile Extrusion_solid.dot 
    auto_ptr< SFBool >  solid;

	/// 
    /// <b>Access type:</b> inputOutput \n
	/// <b>Default value:</b> [0 0 0 0 1 0] \n
    /// 
    /// \dotfile Extrusion_spine.dot 
    auto_ptr< MFVec3f >  spine;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  };
}

#endif
