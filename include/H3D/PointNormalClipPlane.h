//////////////////////////////////////////////////////////////////////////////
//    Copyright 2004-2013, SenseGraphics AB
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
/// \file PointNormalClipPlane.h
/// \brief Header file for PointNormalClipPlane.
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __POINTNORMALCLIPPLANE_H__
#define __POINTNORMALCLIPPLANE_H__

#include <H3D/ClipPlane.h>
#include <H3D/SFVec3f.h>

namespace H3D {

  /// \ingroup X3DNodes
  /// \class PointNormalClipPlane
  /// \brief This is a ClipPlane where the plane is specified with a 
  /// point and a normal. Everything above the plane(in the direction
  /// of the normal) will be rendered.
  ///
  /// \par Internal routes:
  /// \dotfile PointNormalClipPlane.dot
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/PointNormalClipPlane.x3d">PointNormalClipPlane.x3d</a>
  ///     ( <a href="examples/PointNormalClipPlane.x3d.html">Source</a> )
  class H3DAPI_API PointNormalClipPlane : 
    public ClipPlane {
  public:

    /// The PlaneEq updates the plane equation from the values of 
    /// the point and normal fields.
    /// routes_in[0] is the point field.
    /// routes_in[1] is the normal field.
    class H3DAPI_API PlaneEq: public TypedField< SFVec4d,
                                                 Types< SFVec3f, SFVec3f > > {
      /// Update the plane equation from the values of 
      /// the point and normal fields.
      virtual void update();
    };
     
    /// Constructor.
    PointNormalClipPlane( Inst< SFNode  > _metadata      = 0,
                          Inst< SFBool  > _enabled       = 0,
                          Inst< PlaneEq > _plane         = 0,
                          Inst< SFBool  > _clipHaptics   = 0,
                          Inst< SFBool  > _clipGraphics  = 0,
                          Inst< SFVec3f > _point         = 0,
                          Inst< SFVec3f > _normal        = 0);

    /// A point the lies on the plane to be used as clip plane.
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> Vec3f( 0, 0, 0 ) \n
    /// 
    /// \dotfile PointNormalClipPlane_point.dot
    auto_ptr< SFVec3f > point;

    /// The normal of the plane to be used as clip plane.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> Vec3f( 0, 1, 0 ) \n
    /// 
    /// \dotfile PointNormalClipPlane_normal.dot
    auto_ptr< SFVec3f > normal;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  };
}

#endif
