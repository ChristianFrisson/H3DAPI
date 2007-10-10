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
/// \file OrthoViewpoint.cpp
/// \brief CPP file for OrthoViewpoint, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/OrthoViewpoint.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase OrthoViewpoint::database( 
                                    "OrthoViewpoint", 
                                    &(newInstance<OrthoViewpoint>), 
                                    typeid( OrthoViewpoint ),
                                    &X3DViewpointNode::database );

namespace OrthoViewpointInternals {
  FIELDDB_ELEMENT( OrthoViewpoint, fieldOfView, INPUT_OUTPUT );
}



OrthoViewpoint::OrthoViewpoint( 
                     Inst< SFSetBind  > _set_bind,
                     Inst< SFVec3f    > _centerOfRotation,
                     Inst< SFString   > _description,
                     Inst< MFFloat    > _fieldOfView,
                     Inst< SFBool     > _jump,
                     Inst< SFNode     > _metadata,
                     Inst< SFRotation > _orientation,
                     Inst< SFVec3f    > _position,
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
  
  type_name = "OrthoViewpoint";
  database.initFields( this );

  fieldOfView->push_back( -1 );
  fieldOfView->push_back( -1 );
  fieldOfView->push_back( 1 );
  fieldOfView->push_back( 1 );
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
    if( H3DAbs( view_point_aspect_ratio - aspect_ratio ) <=
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
