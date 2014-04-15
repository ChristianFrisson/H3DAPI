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
/// \file MetadataFloat.cpp
/// \brief CPP file for MetadataFloat, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/MetadataFloat.h>

using namespace H3D;

H3DNodeDatabase MetadataFloat::database( "MetadataFloat", 
                                          &(newInstance<MetadataFloat>),
                                          typeid( MetadataFloat ),
                                          &X3DNode::database );

namespace MetadataFloatInternals {
  FIELDDB_ELEMENT( MetadataFloat, metadata, INPUT_OUTPUT );
  FieldDBInsert name( INPUT_OUTPUT( &MetadataFloat::database,
                                    "name", 
                                    &MetadataFloat::nameF ) );
  FIELDDB_ELEMENT( MetadataFloat, reference, INPUT_OUTPUT );
  FIELDDB_ELEMENT( MetadataFloat, value, INPUT_OUTPUT );
}

MetadataFloat::MetadataFloat( Inst< SFString       > _name,
                              Inst< SFString       > _reference,
                              Inst< SFMetadataNode > _metadata,
                              Inst< MFFloat       > _value ) :
  X3DMetadataObject( _name, _reference ),
  metadata( _metadata ),
  value( _value ) {

  type_name = "MetadataFloat";
  database.initFields( this );
}

