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
/// \file PointNormalClipPlane.cpp
/// \brief CPP file for PointNormalClipPlane.
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/PointNormalClipPlane.h>

using namespace H3D;

H3DNodeDatabase PointNormalClipPlane::database( 
        "PointNormalClipPlane", 
        &(newInstance<PointNormalClipPlane>),
        typeid( PointNormalClipPlane ),
        &ClipPlane::database 
        );

namespace PointNormalClipPlaneInternals {
  FIELDDB_ELEMENT( PointNormalClipPlane, plane, OUTPUT_ONLY );
  FIELDDB_ELEMENT( PointNormalClipPlane, point, INPUT_OUTPUT );
  FIELDDB_ELEMENT( PointNormalClipPlane, normal, INPUT_OUTPUT );
}

PointNormalClipPlane::PointNormalClipPlane( Inst< SFNode  > _metadata,
                                            Inst< SFBool  > _enabled,
                                            Inst< PlaneEq > _plane,
                                            Inst< SFBool  > _clipHaptics,
                                            Inst< SFBool  > _clipGraphics,
                                            Inst< SFVec3f > _point,
                                            Inst< SFVec3f > _normal ) :
  ClipPlane( _metadata, _enabled, _plane, _clipHaptics, _clipGraphics ),
  point( _point ),
  normal( _normal ){

  type_name = "PointNormalClipPlane";
  database.initFields( this );

  point->setValue( Vec3f( 0, 0, 0 ) );
  normal->setValue( Vec3f( 0, 1, 0 ) );

  point->route( plane, id );
  normal->route( plane, id );

  plane->setAccessType( Field::OUTPUT_ONLY );
}

void PointNormalClipPlane::PlaneEq::update() {
  const Vec3f &point = static_cast< SFVec3f * >( routes_in[0] )->getValue();
  Vec3f normal = static_cast< SFVec3f * >( routes_in[1] )->getValue();
  normal.normalizeSafe();
  value = Vec4d( normal.x, normal.y, normal.z, 
                 -normal.x*point.x - normal.y*point.y - normal.z*point.z );
}


