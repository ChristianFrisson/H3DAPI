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
/// \file DefaultAppearance.cpp
/// \brief CPP file for DefaultAppearance.
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/DefaultAppearance.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase DefaultAppearance::database( "DefaultAppearance", 
                                      &(newInstance<DefaultAppearance>),
                                      typeid( DefaultAppearance ),
                                      &H3DOptionNode::database );

namespace DefaultAppearanceInternals {
  FIELDDB_ELEMENT( DefaultAppearance, defaultAppearance, INPUT_OUTPUT );
}

DefaultAppearance::DefaultAppearance( 
                           Inst< SFNode>  _metadata,
                           Inst< SFAppearance > _defaultAppearance ) :
  H3DOptionNode( _metadata ),
  defaultAppearance( _defaultAppearance ) {
  defaultAppearance->route( updateOption );
  type_name = "DefaultAppearance";
  database.initFields( this );
}


