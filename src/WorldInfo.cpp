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
/// \file WorldInfo.cpp
/// \brief CPP file for WorldInfo, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include "WorldInfo.h"

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase WorldInfo::database( 
                                   "WorldInfo", 
                                   &(newInstance<WorldInfo>), 
                                   typeid( WorldInfo ),
                                   &X3DInfoNode::database );

namespace WorldInfoInternals {
  FIELDDB_ELEMENT( WorldInfo, info, INITIALIZE_ONLY );
  FIELDDB_ELEMENT( WorldInfo, title, INITIALIZE_ONLY );
}


WorldInfo::WorldInfo( Inst< SFNode   > _metadata,
                      Inst< MFString > _info,
                      Inst< SFString > _title ) :
  X3DInfoNode( _metadata ),
  info( _info ),
  title( _title ) {
  type_name = "WorldInfo";
  database.initFields( this );
  
  title->setValue( "" );
}
