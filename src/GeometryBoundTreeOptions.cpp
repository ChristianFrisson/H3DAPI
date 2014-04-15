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
/// \file GeometryBoundTreeOptions.cpp
/// \brief CPP file for GeometryBoundTreeOptions.
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/GeometryBoundTreeOptions.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase GeometryBoundTreeOptions::database(
                                      "GeometryBoundTreeOptions", 
                                      &(newInstance<GeometryBoundTreeOptions>),
                                      typeid( GeometryBoundTreeOptions ),
                                      &H3DOptionNode::database );

namespace GeometryBoundTreeOptionsInternals {
  FIELDDB_ELEMENT( GeometryBoundTreeOptions, boundType, INPUT_OUTPUT );
  FIELDDB_ELEMENT( GeometryBoundTreeOptions, 
                   maxTrianglesInLeaf, INPUT_OUTPUT );
}

GeometryBoundTreeOptions::GeometryBoundTreeOptions( 
                           Inst< SFNode>  _metadata,
                           Inst< SFString > _boundType,
                           Inst< SFInt32  > _maxTrianglesInLeaf ):
  H3DOptionNode( _metadata ),
  boundType( _boundType ),
  maxTrianglesInLeaf( _maxTrianglesInLeaf ) {
  
  type_name = "GeometryBoundTreeOptions";
  database.initFields( this );

  boundType->route( updateOption );
  maxTrianglesInLeaf->route( updateOption );

  boundType->addValidValue( "AABB" );
  boundType->addValidValue( "OBB" );
  boundType->addValidValue( "SPHERE" );
  boundType->setValue( "AABB" );
  maxTrianglesInLeaf->setValue( 1 );
}


