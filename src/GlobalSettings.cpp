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
/// \file GlobalSettings.cpp
/// \brief CPP file for GlobalSettings
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <GlobalSettings.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase GlobalSettings::database( "GlobalSettings", 
                                      &(newInstance<GlobalSettings>), 
                                      typeid( GlobalSettings ),
                                      &X3DBindableNode::database  );

namespace GlobalSettingsInternals {
  FIELDDB_ELEMENT( GlobalSettings, options, INPUT_OUTPUT );
}


GlobalSettings::GlobalSettings( 
                       Inst<   SFSetBind >  _set_bind,
                       Inst< SFNode    >  _metadata,
                       Inst<  SFTime    >  _bindTime,
                       Inst<  SFBool    >  _isBound,
                       Inst< MFOptionNode >  _options ):
  X3DBindableNode( "GlobalSettings", _set_bind, _metadata, 
                   _bindTime, _isBound ),
  options        ( _options ) {

  type_name = "GlobalSettings";
  database.initFields( this );
}
