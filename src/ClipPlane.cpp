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
/// \file ClipPlane.cpp
/// \brief CPP file for ClipPlane, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/ClipPlane.h>
#include <H3D/H3DHapticsDevice.h>

using namespace H3D;

H3DNodeDatabase ClipPlane::database( 
        "ClipPlane", 
        &(newInstance<ClipPlane>),
        typeid( ClipPlane ),
        &X3DChildNode::database 
        );

namespace ClipPlaneInternals {
  FIELDDB_ELEMENT( ClipPlane, enabled, INPUT_OUTPUT );
  FIELDDB_ELEMENT( ClipPlane, plane, INPUT_OUTPUT );
  FIELDDB_ELEMENT( ClipPlane, clipGraphics, INPUT_OUTPUT );
  FIELDDB_ELEMENT( ClipPlane, clipHaptics, INPUT_OUTPUT );
}

int ClipPlane::nr_active_clip_planes = 0;
GLint ClipPlane::max_nr_clip_planes = -1;

ClipPlane::ClipPlane( Inst< SFNode  >  _metadata,
                      Inst< SFBool  >  _enabled,
                      Inst< SFVec4d >  _plane,
                      Inst< SFBool  >  _clipHaptics,
                      Inst< SFBool  >  _clipGraphics ) :
  X3DChildNode( _metadata ),
  enabled( _enabled ),
  plane( _plane ),
  clipHaptics( _clipHaptics ),
  clipGraphics( _clipGraphics ) {

  type_name = "ClipPlane";
  database.initFields( this );

  displayList->setOwner( this );

  enabled->setValue( true );
  plane->setValue( Vec4d( 0,1,0,0 ) );
  clipHaptics->setValue( true );
  clipGraphics->setValue( true );

  plane_index = -1;
  haptics_enabled = true;

  clipGraphics->route( displayList );
  plane->route( displayList );
}

void ClipPlane::enableHapticsState( TraverseInfo &ti ) {
  haptics_enabled = ti.hapticsEnabled();
  if( clipHaptics->getValue() && ti.getHapticsDevices().size() > 0 ) {
    const Vec4d &v = plane->getValue();
    H3DHapticsDevice *hd = ti.getHapticsDevice( 0 );
    const Vec3f &pos = ti.getAccInverseMatrix() 
      * hd->trackerPosition->getValue();
    if( pos.x * v.x + pos.y * v.y + pos.z * v.z + v.w < 0 )
      ti.disableHaptics();
  }
}

void ClipPlane::disableHapticsState( TraverseInfo &ti ) {
  if( haptics_enabled )
    ti.enableHaptics();
  else
    ti.disableHaptics();
}

void ClipPlane::enableGraphicsState() { 
  if( max_nr_clip_planes == -1 )
    glGetIntegerv( GL_MAX_CLIP_PLANES, &max_nr_clip_planes );
  if ( enabled->getValue() && 
       clipGraphics->getValue() && 
       nr_active_clip_planes < max_nr_clip_planes ) {
    plane_index = nr_active_clip_planes++;
    const Vec4d &v = plane->getValue();
    GLdouble e[] = { v.x, v.y, v.z, v.w };
    glClipPlane( GL_CLIP_PLANE0 + plane_index, e );
    glEnable( GL_CLIP_PLANE0 + plane_index );
  } 
};

void ClipPlane::disableGraphicsState() { 
  if( enabled->getValue() && 
      clipGraphics->getValue() && 
	  plane_index < max_nr_clip_planes ) {
    glDisable( GL_CLIP_PLANE0 + plane_index );
    nr_active_clip_planes--;
    plane_index = -1;
  }
};

