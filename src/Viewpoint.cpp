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
/// \file Viewpoint.cpp
/// \brief CPP file for Viewpoint, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/Viewpoint.h>

#ifdef HAVE_OPENGL
#ifdef MACOSX
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif
#endif

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase Viewpoint::database( 
                                    "Viewpoint", 
                                    &(newInstance<Viewpoint>), 
                                    typeid( Viewpoint ),
                                    &X3DViewpointNode::database );

namespace ViewpointInternals {
  FIELDDB_ELEMENT( Viewpoint, fieldOfView, INPUT_OUTPUT );
}



Viewpoint::Viewpoint( 
                     Inst< SFSetBind  > _set_bind,
                     Inst< SFVec3f    > _centerOfRotation,
                     Inst< SFString   > _description,
                     Inst< SFFloat    > _fieldOfView,
                     Inst< SFBool     > _jump,
                     Inst< SFNode     > _metadata,
                     Inst< SFOrientation > _orientation,
                     Inst< SFPosition > _position,
                     Inst< SFBool    >  _retainUserOffsets,
                     Inst< SFTime     > _bindTime,
                     Inst< SFBool     > _isBound,
                     Inst< SFMatrix4f > _accForwardMatrix,
                     Inst< SFMatrix4f > _accInverseMatrix ) :
  X3DViewpointNode( _set_bind, _centerOfRotation, _description, _jump,
                    _metadata, _orientation, _position,
                    _retainUserOffsets, _bindTime, _isBound,
                    _accForwardMatrix, _accInverseMatrix ),
  fieldOfView     ( _fieldOfView      ) {
  
  type_name = "Viewpoint";
  database.initFields( this );

  fieldOfView->setValue( (H3DFloat)Constants::pi/4 );
  
}

bool Viewpoint::windowFromfieldOfView( H3DFloat width, H3DFloat height,
                                       H3DFloat clip_near,
                                       H3DFloat &top, H3DFloat &bottom,
                                       H3DFloat &right, H3DFloat &left ) {
  H3DFloat fov_h, fov_v;
  H3DFloat field_of_view = fieldOfView->getValue();
  H3DFloat aspect_ratio = width / height;
  
  // calculate the horizontal and vertical field of view components
  // as defined by the X3D spec for Viewpoint.
  if ( field_of_view > Constants::pi )
    field_of_view = (H3DFloat) Constants::pi;
  if ( width < height ) {
    // width is smallest
    fov_h = field_of_view;
    fov_v = 2 * atan( ( height * tan(fov_h/2) ) / width );
    if ( fov_v > Constants::pi ) {
      fov_v = (H3DFloat) Constants::pi;
      fov_h = 2 * atan( ( aspect_ratio ) * tan(fov_v/2) );        
    }
  } else {
    fov_v = field_of_view;
    fov_h = 2 * atan( ( aspect_ratio ) * tan(fov_v/2) );
    if ( fov_h > Constants::pi ) {
      fov_h = field_of_view;
      fov_v = 2 * atan( ( height * tan(fov_h/2) ) / width );
    }
  }

  top    = clip_near * tan(fov_v/2);
  bottom = -top;

  right  = aspect_ratio * top;
  left   = -right;
  return true;
}

void Viewpoint::setupProjection( EyeMode eye_mode,
                                 H3DFloat width, H3DFloat height,
                                 H3DFloat clip_near, H3DFloat clip_far,
                                 StereoInfo * stereo_info ) {
  H3DFloat top, bottom, right, left;
  getProjectionDimensions( eye_mode, width, height, clip_near, top,
                           bottom, right, left, stereo_info );

  GLdouble A = (right+left)/(right - left );
  GLdouble B = (top+bottom)/(top-bottom);
  GLdouble C, D;

  if( clip_far != -1 ) {
    C = -(clip_far + clip_near)/(clip_far-clip_near);
    D = -2 * clip_far * clip_near /(clip_far-clip_near);
  } else {
    // epsilon to use for infinite far plane to prevent artifacts
    GLdouble epsilon = 0.0001;
    C = epsilon - 1; 
    D = -2 * clip_near * (1-epsilon);
  }
  
  GLdouble m[16] = { 2* clip_near/(right-left), 0, 0, 0,
                     0, 2*clip_near/(top - bottom), 0, 0,
                     A, B, C, -1,
                     0, 0, D, 0 };

  glMultMatrixd( m );
}

