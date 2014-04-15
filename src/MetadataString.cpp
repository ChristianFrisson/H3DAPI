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
/// \file MetadataString.cpp
/// \brief CPP file for MetadataString, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/MetadataString.h>

using namespace H3D;

H3DNodeDatabase MetadataString::database( "MetadataString", 
                                          &(newInstance<MetadataString>),
                                          typeid( MetadataString ),
                                          &X3DNode::database );

namespace MetadataStringInternals {
  FIELDDB_ELEMENT( MetadataString, metadata, INPUT_OUTPUT );
  FieldDBInsert name( INPUT_OUTPUT( &MetadataString::database,
                                    "name", 
                                    &MetadataString::nameF ) );
  FIELDDB_ELEMENT( MetadataString, reference, INPUT_OUTPUT );
  FIELDDB_ELEMENT( MetadataString, value, INPUT_OUTPUT );
}

MetadataString::MetadataString( Inst< SFString       > _name,
                                Inst< SFString       > _reference,
                                Inst< SFMetadataNode > _metadata,
                                Inst< MFString > _value ) :
  X3DMetadataObject( _name, _reference ),
  metadata( _metadata ),
  value( _value ) {

  type_name = "MetadataString";
  database.initFields( this );
}

