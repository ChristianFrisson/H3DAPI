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
/// \file Disk2D.h
/// \brief Header file for Disk2D, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __DISK2D_H__
#define __DISK2D_H__

#include <H3D/X3DGeometryNode.h>
#include <H3D/SFFloat.h>

namespace H3D {

  /// \ingroup X3DNodes
  /// \class Disk2D
  /// \brief The Disk2D node specifies a circular disk which is centred 
  /// at (0, 0) in the local coordinate system. The outerRadius field 
  /// specifies the radius of the outer dimension of the Disk2D. 
  /// 
  /// The innerRadius field specifies the inner dimension of the Disk2D. 
  /// The value of outerRadius shall be greater than zero. The value of
  /// innerRadius shall be greater than or equal to zero and less than
  /// or equal to outerRadius. If innerRadius is zero, the Disk2D is
  /// completely filled. Otherwise, the area within the innerRadius forms
  /// a hole in the Disk2D. If innerRadius is equal to outerRadius, a 
  /// solid circle shall be drawn using the current line properties. 
  ///
  /// Textures are applied individually to each face of the Disk2D. On 
  /// the front (+Z) and back (-Z) faces of the Disk2D, when viewed from
  /// the +Z-axis, the texture is mapped onto each face with the same
  /// orientation as if the image were displayed normally in 2D. 
  /// 
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/Disk2D.x3d">Disk2D.x3d</a>
  ///     ( <a href="examples/Disk2D.x3d.html">Source</a> )
  ///
  /// \par Internal routes:
  /// \dotfile Disk2D.dot
  
  class H3DAPI_API Disk2D : 
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
    Disk2D( Inst< SFNode      > _metadata = 0,
            Inst< SFBound     > _bound = 0,
            Inst< DisplayList > _displayList = 0,
            Inst< MFBool      > _isTouched = 0,
            Inst< MFVec3f     > _force = 0,
            Inst< MFVec3f     > _contactPoint = 0,
            Inst< MFVec3f     > _contactNormal = 0,
            Inst< SFFloat     > _innerRadius = 0,
            Inst< SFFloat     > _outerRadius = 0,
            Inst< SFBool      > _solid = 0 );
   
    /// Renders the Disk2D using OpenGL.
    virtual void render();

    // Traverse the scenegraph. See X3DGeometryNode::traverseSG
    // for more info.
    virtual void traverseSG( TraverseInfo &ti );  

    /// The number of triangles rendered by this geometry.
    virtual int nrTriangles() {
      if( innerRadius->getValue() == 0 ) {
        return 40;
      } else if( innerRadius->getValue() != outerRadius->getValue() ) {
        return 80;
      } 
      else return 0;
    }

    virtual int nrLines() {
      if( innerRadius->getValue() == outerRadius->getValue() ) return 40;
      else return 0;
    }

    ///  The innerRadius field specifies the inner dimension of the Disk2D. 
    /// The value of innerRadius shall be greater than or equal to zero
    /// and less than or equal to outerRadius. If innerRadius is zero,
    /// the Disk2D is completely filled. Otherwise, the area within the 
    /// innerRadius forms a hole in the Disk2D.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 0 \n
    /// 
    /// \dotfile Disk2D_innerRadius.dot
    auto_ptr< SFFloat > innerRadius;

    /// The outerRadius field specifies the radius of the outer 
    /// dimension of the Disk2D. The value of outerRadius shall be 
    /// greater than zero.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 1 \n
    /// 
    /// \dotfile Disk2D_outerRadius.dot
    auto_ptr< SFFloat > outerRadius;

    /// The solid field specifies if both sides of the closed arc should be 
    /// rendered. If solid is FALSE then both sides will be rendered.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> FALSE
    /// 
    /// \dotfile Disk2D_solid.dot
    auto_ptr< SFBool > solid;
    
    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  };
}

#endif
