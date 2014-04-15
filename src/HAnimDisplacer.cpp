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
/// \file HAnimDisplacer.cpp
/// \brief CPP file for HAnimDisplacer, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/HAnimDisplacer.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase HAnimDisplacer::database( "HAnimDisplacer", 
                                          &(newInstance<HAnimDisplacer>),
                                          typeid( HAnimDisplacer ),
                                          &X3DGeometricPropertyNode::database );

namespace HAnimDisplacerInternals {
  FIELDDB_ELEMENT( HAnimDisplacer, coordIndex, INPUT_OUTPUT );
  FIELDDB_ELEMENT( HAnimDisplacer, displacements, INPUT_OUTPUT );
  FIELDDB_ELEMENT( HAnimDisplacer, name, INPUT_OUTPUT );
  FIELDDB_ELEMENT( HAnimDisplacer, weight, INPUT_OUTPUT );
}

HAnimDisplacer::HAnimDisplacer( 
                       Inst< SFNode >  _metadata,
                       Inst< MFInt32 > _coordIndex,
                       Inst< MFVec3f > _displacements,
                       Inst< SFString > _name, 
                       Inst< SFFloat > _weight  ) :
  X3DGeometricPropertyNode( _metadata ),
  coordIndex   ( _coordIndex    ),
  displacements( _displacements ),
  name( _name ),
  weight( _weight ) {

  type_name = "HAnimDisplacer";
  database.initFields( this );

  weight->setValue(0);

  coordIndex->route( propertyChanged );
  displacements->route( propertyChanged );
  weight->route( propertyChanged );
}




