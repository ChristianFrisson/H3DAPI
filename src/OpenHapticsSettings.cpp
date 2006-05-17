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
/// \file OpenHapticsSettings.cpp
/// \brief CPP file for OpenHapticsSettings
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include "OpenHapticsSettings.h"

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase OpenHapticsSettings::database( "OpenHapticsSettings", 
                                      &(newInstance<OpenHapticsSettings>), 
                                      typeid( OpenHapticsSettings ),
                                      &X3DBindableNode::database  );

namespace OpenHapticsSettingsInternals {
  FIELDDB_ELEMENT( OpenHapticsSettings, defaultGLShape, INPUT_OUTPUT );
}


OpenHapticsSettings::OpenHapticsSettings( 
                       Inst<   SFSetBind >  _set_bind,
                       Inst< SFNode    >  _metadata,
                       Inst<  SFTime    >  _bindTime,
                       Inst<  SFBool    >  _isBound,
                       Inst< SFString >  _defaultGLShape ) :
  X3DBindableNode( "OpenHapticsSettings", _set_bind, _metadata, _bindTime, _isBound ),
  defaultGLShape        ( _defaultGLShape ) {

  type_name = "OpenHapticsSettings";
  database.initFields( this );

  defaultGLShape->setValue( "FEEDBACK_BUFFER" );
}
