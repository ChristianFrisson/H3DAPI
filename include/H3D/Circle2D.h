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
/// \file Circle2D.h
/// \brief Header file for Circle2D, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __CIRCLE2D_H__
#define __CIRCLE2D_H__

#include <H3D/X3DGeometryNode.h>
#include <H3D/SFFloat.h>

namespace H3D {

  /// \ingroup X3DNodes
  /// \class Circle2D
  /// \brief The Circle2D node specifies a circle centred at (0,0) in the
  /// local 2D coordinate system.  The radius field specifies the radius
  /// of the Circle2D. The value of radius shall be greater than zero.
  /// 
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/Circle2D.x3d">Circle2D.x3d</a>
  ///     ( <a href="examples/Circle2D.x3d.html">Source</a> )
  ///
  /// \par Internal routes:
  /// \dotfile Circle2D.dot
  
  class H3DAPI_API Circle2D : 
    public X3DGeometryNode {
  public:

    /// SFBound is specialized update itself from the radius field 
    /// of the Circle2D node.
    ///
    /// routes_in[0] is the radius field of the Circle2D node.  
    ///
    class H3DAPI_API SFBound: public TypedField< X3DGeometryNode::SFBound,
    SFFloat > {
      /// Update the bound from the radius field. 
      virtual void update() {
        H3DFloat d = 2 * static_cast< SFFloat * >( routes_in[0] )->getValue();
        BoxBound *bb = new BoxBound;
        bb->size->setValue( Vec3f( d, d, 0.f ) );
        value = bb;
      }
    };

    /// Display list is extended in order to set color to emissive
    /// color from material outside of display list, since we have to
    /// do a glGet to get the value. If we have it inside, the display
    /// list will not be rebuilt and the color not change.
    class H3DAPI_API DisplayList: public X3DGeometryNode::DisplayList {
    public:
      /// Set the color of the lineset outside display list.
      virtual void callList( bool build_list = true );
    };

    /// Constructor.
    Circle2D( Inst< SFNode      > _metadata = 0,
           Inst< SFBound     > _bound = 0,
           Inst< DisplayList > _displayList = 0,
           Inst< MFBool      > _isTouched = 0,
           Inst< MFVec3f     > _force = 0,
           Inst< MFVec3f     > _contactPoint = 0,
           Inst< MFVec3f     > _contactNormal = 0,
           Inst< SFFloat     > _radius = 0 );
   
    /// Renders the Circle2D using OpenGL.
    virtual void render();
    
    /// The number of lines rendered by this geometry.
    virtual int nrLines() {
      return 40;
    }

    /// The radius field specifies the radius of the circle.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 1 \n
    /// 
    /// \dotfile Circle2D_radius.dot
    auto_ptr< SFFloat > radius;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  };
}

#endif
