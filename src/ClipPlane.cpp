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
  FIELDDB_ELEMENT( ClipPlane, clipHapticsDevice, INPUT_OUTPUT );
}

int ClipPlane::nr_active_clip_planes = 0;
GLint ClipPlane::max_nr_clip_planes = -1;

ClipPlane::ClipPlane( Inst< SFNode  >  _metadata,
                      Inst< SFBool  >  _enabled,
                      Inst< SFVec4d >  _plane,
                      Inst< SFBool  >  _clipHaptics,
                      Inst< SFBool  >  _clipGraphics,
                      Inst< MFBool  >  _clipHapticsDevice) :
  X3DChildNode( _metadata ),
  enabled( _enabled ),
  plane( _plane ),
  clipHaptics( _clipHaptics ),
  clipGraphics( _clipGraphics ),
  clipHapticsDevice( _clipHapticsDevice ) {

  type_name = "ClipPlane";
  database.initFields( this );

  displayList->setOwner( this );

  enabled->setValue( true );
  plane->setValue( Vec4d( 0,1,0,0 ) );
  clipHaptics->setValue( true );
  clipGraphics->setValue( true );

  plane_index = -1;

  clipGraphics->route( displayList );
  plane->route( displayList );
  enabled->route( displayList );
}

void ClipPlane::enableHapticsState( TraverseInfo &ti ) {
  haptics_enabled = ti.getHapticsEnabled();
  const vector< bool > &clip_per_device = clipHapticsDevice->getValue();
  const Vec4d &v = plane->getValue();
  
  if( clip_per_device.empty() ) {
    // no per device values, so use the clipHaptics field to decide
    // if clipping should be done for all devices or not.
    if( clipHaptics->getValue()  ) {
      const vector< H3DHapticsDevice * > &devices = ti.getHapticsDevices();
      for( unsigned int i = 0; i < devices.size(); ++i ) {
        H3DHapticsDevice *hd = ti.getHapticsDevice( i );
        const Vec3f &pos = ti.getAccInverseMatrix() 
                           * hd->trackerPosition->getValue();
        if( pos.x * v.x + pos.y * v.y + pos.z * v.z + v.w < 0 )
          ti.disableHaptics(i);
      }
    }
  } else {
    // we have per device values, so use them to determine if clipping
    // should be done.
    const vector< H3DHapticsDevice * > &devices = ti.getHapticsDevices();
    for( unsigned int i = 0; i < devices.size(); ++i ) {
      size_t index =  
        i < clip_per_device.size() ? i : clip_per_device.size() - 1;
      if( clip_per_device[ index ] ) {
        H3DHapticsDevice *hd = ti.getHapticsDevice( i );
        const Vec3f &pos = ti.getAccInverseMatrix() 
                           * hd->trackerPosition->getValue();
        if( pos.x * v.x + pos.y * v.y + pos.z * v.z + v.w < 0 )
          ti.disableHaptics(i);
      }
    }
  }
}

void ClipPlane::disableHapticsState( TraverseInfo &ti ) {
  ti.setHapticsEnabled( haptics_enabled );
}

void ClipPlane::enableGraphicsState() { 
  bool clip_nr_is_free = checkIfFreeClipPlaneIndex();
  if ( enabled->getValue() && 
       clipGraphics->getValue() && 
       clip_nr_is_free ) {
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
    --nr_active_clip_planes;
    plane_index = -1;
  }
};

bool ClipPlane::truncateLine( const Vec3f &from, const Vec3f &to,
                              Vec3f &result_from, Vec3f &result_to ) {
  if( !enabled->getValue() ) {
    result_from = from;
    result_to = to;
    return true;
  }
  Vec4d plane_eq = plane->getValue();
  Vec3f plane_normal = Vec3f( (H3DFloat)plane_eq.x,
                              (H3DFloat)plane_eq.y,
                              (H3DFloat)plane_eq.z );
  // Check if both points below plane.
  // Calculate first intersection and replace result_from or result_to.
  H3DFloat plane_d = -(H3DFloat)plane_eq.w;
  // Compute t-value for plane.
  Vec3f ab = to - from;
  H3DFloat numerator = plane_d - plane_normal * from;
  H3DFloat denominator = plane_normal * ab;
  // No check if the denomintator is 0 since we assume IEEE 754 floating-point
  // arithmetic. That means that t will be above 1 or below 0 which means that
  // it never goes into the first case in the if check below.
  H3DFloat t = numerator / denominator;

  // If t in [0,1] compute and return intersection point.
  if( t >= 0.0f && t <= 1.0f ) {
    if( denominator > 0 ) {
      // Normal is in the direction of the line segment. Which means that
      // from is below the plane because otherwise there should not be an
      // intersection. Therefore the result_from is changed to the new point.
      result_from = from + t * ab;
      result_to = to;
    } else {
      // Normal is in the opposite direction of the line segment. Which means
      // that from is above the plane because otherwise there should not be an
      // intersection. Therefore the result_to is changed to the new point.
      result_from = from;
      result_to = from + t * ab;
    }
    return true;
  } else if( -numerator >= 0 ) {
    // from is above or in the plane which means that the line segment is above
    // the plane since it does not intersect it. Return true.
    result_from = from;
    result_to = to;
    return true;
  }

  return false;
}

