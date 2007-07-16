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
/// \file ForceField.cpp
/// \brief cpp file for ForceField
///
//
//////////////////////////////////////////////////////////////////////////////
#include <ForceField.h> 

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase ForceField::database( "ForceField", 
                                      &(newInstance<ForceField>), 
                                      typeid( ForceField ),
                                      &H3DForceEffect::database );

namespace ForceFieldInternals {
  FIELDDB_ELEMENT( ForceField, force, INPUT_OUTPUT );
}

/// Constructor
ForceField::ForceField( Inst< SFVec3f > _force,
                        Inst< SFNode>  _metadata ) :
  H3DForceEffect( _metadata ),
  force( _force ) {
  
  type_name = "ForceField";
  database.initFields( this );
  force->setValue( Vec3f( 0,0,0 ) );
}
