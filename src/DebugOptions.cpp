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
/// \file DebugOptions.cpp
/// \brief CPP file for DebugOptions.
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/DebugOptions.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase DebugOptions::database( "DebugOptions", 
                                      &(newInstance<DebugOptions>),
                                      typeid( DebugOptions ),
                                      &H3DOptionNode::database );

namespace DebugOptionsInternals {
  FIELDDB_ELEMENT( DebugOptions, drawBound, INPUT_OUTPUT );
  FIELDDB_ELEMENT( DebugOptions, drawBoundTree, INPUT_OUTPUT );
  FIELDDB_ELEMENT( DebugOptions, drawHapticTriangles, INPUT_OUTPUT );
  FIELDDB_ELEMENT( DebugOptions, printShaderWarnings, INPUT_OUTPUT );
}

DebugOptions::DebugOptions( 
                           Inst< SFNode  > _metadata,
                           Inst< SFBool  > _drawBound,
                           Inst< SFInt32 > _drawBoundTree,
                           Inst< SFBool  > _drawHapticTriangles,
                           Inst< SFBool  > _printShaderWarnings ) :
  H3DOptionNode( _metadata ),
  drawBound( _drawBound ),
  drawBoundTree( _drawBoundTree ),
  drawHapticTriangles( _drawHapticTriangles ),
  printShaderWarnings( _printShaderWarnings ) {
  type_name = "DebugOptions";

  database.initFields( this );

  drawBound->route( updateOption );
  drawBoundTree->route( updateOption );
  drawHapticTriangles->route( updateOption );
  printShaderWarnings->route( updateOption );

  drawBound->setValue( false );
  drawBoundTree->setValue( -1 );
  drawHapticTriangles->setValue( false );
  printShaderWarnings->setValue( false );
}


