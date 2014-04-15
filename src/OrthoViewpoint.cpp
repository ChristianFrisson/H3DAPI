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
/// \file OrthoViewpoint.cpp
/// \brief CPP file for OrthoViewpoint, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/OrthoViewpoint.h>

#ifdef HAVE_OPENGL
#ifdef MACOSX
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif
#endif

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase OrthoViewpoint::database( 
                                    "OrthoViewpoint", 
                                    &(newInstance<OrthoViewpoint>), 
                                    typeid( OrthoViewpoint ),
                                    &X3DViewpointNode::database );

namespace OrthoViewpointInternals {
  FIELDDB_ELEMENT( OrthoViewpoint, fieldOfView, INPUT_OUTPUT );
  FIELDDB_ELEMENT( OrthoViewpoint, retainAspectRatio, INPUT_OUTPUT );
}



OrthoViewpoint::OrthoViewpoint( 
                     Inst< SFSetBind  > _set_bind,
                     Inst< SFVec3f    > _centerOfRotation,
                     Inst< SFString   > _description,
                     Inst< MFFloat    > _fieldOfView,
                     Inst< SFBool     > _jump,
                     Inst< SFNode     > _metadata,
                     Inst< SFOrientation > _orientation,
                     Inst< SFPosition > _position,
                     Inst< SFBool    >  _retainUserOffsets,
                     Inst< SFTime     > _bindTime,
                     Inst< SFBool     > _isBound,
                     Inst< SFMatrix4f > _accForwardMatrix,
                     Inst< SFMatrix4f > _accInverseMatrix,
                     Inst< SFBool    > _retainAspectRatio  ) :
  X3DViewpointNode( _set_bind, _centerOfRotation, _description, _jump,
                    _metadata, _orientation, _position,
                    _retainUserOffsets, _bindTime, _isBound,
                    _accForwardMatrix, _accInverseMatrix ),
  fieldOfView     ( _fieldOfView      ),
  retainAspectRatio( _retainAspectRatio ) {
  
  type_name = "OrthoViewpoint";
  database.initFields( this );

  fieldOfView->push_back( -1 );
  fieldOfView->push_back( -1 );
  fieldOfView->push_back( 1 );
  fieldOfView->push_back( 1 );

  retainAspectRatio->setValue( true );
}

bool OrthoViewpoint::windowFromfieldOfView( H3DFloat width, H3DFloat height,
                                       H3DFloat clip_near,
                                       H3DFloat &top, H3DFloat &bottom,
                                       H3DFloat &right, H3DFloat &left ) {
  vector< H3DFloat > field_of_view = fieldOfView->getValue();
  H3DFloat aspect_ratio = width / height;
  if( field_of_view.size() < 4 ) {
    Console(3) << "Warning: to few values in field fieldOfView in "
               << " OrthoViewpoint node ( " << getName()
               << " ). Default value will be used " << endl;
    field_of_view.clear();
    field_of_view.push_back( -1 );
    field_of_view.push_back( -1 );
    field_of_view.push_back( 1 );
    field_of_view.push_back( 1 );
  }
  H3DFloat dx = field_of_view[2] - field_of_view[0];
  H3DFloat dy = field_of_view[3] - field_of_view[1];
  if( dx > Constants::f_epsilon &&
    dy > Constants::f_epsilon ) {

    H3DFloat view_point_aspect_ratio = dx / dy;
    if( !retainAspectRatio->getValue() || 
        H3DAbs( view_point_aspect_ratio - aspect_ratio ) <=
        Constants::f_epsilon ) {
      top = field_of_view[3];
      bottom = field_of_view[1];
      right = field_of_view[2];
      left = field_of_view[0];
    }
    else {
      // Scaling in case the ratios are not correct
      H3DFloat w_dy = width * dy;
      if( w_dy < 0 ) {
        H3DFloat diff = ( height * dx / width - dy ) / 2;
        top = field_of_view[3] + diff;
        bottom = field_of_view[1] - diff;
        right = field_of_view[2];
        left = field_of_view[0];
      }
      else {
        H3DFloat diff = ( w_dy / height - dx ) / 2;
        top = field_of_view[3];
        bottom = field_of_view[1];
        right = field_of_view[2] + diff;
        left = field_of_view[0] - diff;
      }
    }
  }
  return true;
}

void OrthoViewpoint::setupProjection( EyeMode eye_mode,
                                      H3DFloat width, H3DFloat height,
                                      H3DFloat clip_near, H3DFloat clip_far,
                                      StereoInfo * stereo_info ) {
  H3DFloat top, bottom, right, left;
  getProjectionDimensions( eye_mode, width, height, clip_near, top,
                           bottom, right, left, stereo_info );

  // Code used to mimic glOrtho but handles infinite plane.
  // Se documentation for glOrtho to understand the code.
  GLdouble two_near = 2 * clip_near;
  GLdouble right_sub_left = right - left;
  GLdouble top_sub_bottom = top - bottom;
  GLdouble C, tz;

  if( clip_far != -1 ) {
    C = -2/(clip_far-clip_near);
    tz = -(clip_far + clip_near) / (clip_far-clip_near);
  } else {
    // epsilon to use for infinite far plane to prevent artifacts
    // Not sure if this will prevent artifacts or not in this case
    // since the actual values should be C=0 and tz = -1 for infinite
    // plane but this will create a non-invertable matrix.
    // If one would follow the formula used in Viewpoint for
    // projection matrix then the values should be C = 2*epsilon and
    // D=epsilon-1 but this will not create the same correct effect
    // as for projection matrix. Since far plane happens to end up
    // in the wrong place when using gluunproject.
    GLdouble epsilon = 1e-10;
    C = -2*epsilon;
    tz = -(epsilon + 1);
  }
  
  GLdouble m[16] = { 2/right_sub_left, 0, 0, 0,
                     0, 2/top_sub_bottom, 0, 0,
                     0, 0, C, 0,
    -(right+left)/right_sub_left, -(top + bottom)/top_sub_bottom, tz, 1 };

  glMultMatrixd( m );
}
