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
/// \file Viewpoint.cpp
/// \brief CPP file for Viewpoint, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include "Viewpoint.h"

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase Viewpoint::database( 
                                    "Viewpoint", 
                                    &(newInstance<Viewpoint>), 
                                    typeid( Viewpoint ),
                                    &X3DBindableNode::database );

namespace ViewpointInternals {
  FIELDDB_ELEMENT( Viewpoint, centerOfRotation, INPUT_OUTPUT );
  FIELDDB_ELEMENT( Viewpoint, description, INPUT_OUTPUT );
  FIELDDB_ELEMENT( Viewpoint, fieldOfView, INPUT_OUTPUT );
  FIELDDB_ELEMENT( Viewpoint, jump, INPUT_OUTPUT );
  FIELDDB_ELEMENT( Viewpoint, orientation, INPUT_OUTPUT );
  FIELDDB_ELEMENT( Viewpoint, position, INPUT_OUTPUT );
  FIELDDB_ELEMENT( Viewpoint, accForwardMatrix, OUTPUT_ONLY );
  FIELDDB_ELEMENT( Viewpoint, accInverseMatrix, OUTPUT_ONLY );
}



Viewpoint::Viewpoint( 
                     Inst< SFSetBind  > _set_bind,
                     Inst< SFVec3f    > _centerOfRotation,
                     Inst< SFString   > _description,
                     Inst< SFFloat    > _fieldOfView,
                     Inst< SFBool     > _jump,
                     Inst< SFNode     > _metadata,
                     Inst< SFRotation > _orientation,
                     Inst< SFVec3f    > _position,
                     Inst< SFTime     > _bindTime,
                     Inst< SFBool     > _isBound,
                     Inst< SFMatrix4f > _accForwardMatrix,
                     Inst< SFMatrix4f > _accInverseMatrix ) :
  X3DBindableNode( "Viewpoint", _set_bind, _metadata, _bindTime, _isBound ),
  centerOfRotation( _centerOfRotation ),
  description     ( _description      ),
  fieldOfView     ( _fieldOfView      ),
  jump            ( _jump             ),
  orientation     ( _orientation      ),
  position        ( _position         ),
  accForwardMatrix( _accForwardMatrix ),
  accInverseMatrix( _accInverseMatrix ) {
  
  type_name = "Viewpoint";
  database.initFields( this );

  centerOfRotation->setValue( Vec3f( 0, 0, 0 ) );
  orientation->setValue( Rotation( 0, 0, 1, 0 ) );
  position->setValue( Vec3f( 0, 0, 10 ) );
  fieldOfView->setValue( (H3DFloat)Constants::pi/4 );
  // need to give the id since the fields are output only.
  accForwardMatrix->setValue( Matrix4f(), id );
  accInverseMatrix->setValue( Matrix4f(), id );
}


