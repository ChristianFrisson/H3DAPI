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
/// \file VirtualHandGloveSensor.cpp
/// \brief CPP file for VirtualHandGloveSensor.
///
//
//
//////////////////////////////////////////////////////////////////////////////

// H3D includes
#include <H3D/VirtualHandGloveSensor.h>
#include <H3D/Scene.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase VirtualHandGloveSensor::database( 
                                          "VirtualHandGloveSensor", 
                                          NULL,
                                          typeid( VirtualHandGloveSensor ),
                                          &X3DSensorNode::database
                                          );

namespace VirtualHandGloveSensorInternal {
  FIELDDB_ELEMENT( VirtualHandGloveSensor, host, INITIALIZE_ONLY );
  FIELDDB_ELEMENT( VirtualHandGloveSensor, port, INITIALIZE_ONLY );
}

VirtualHandGloveSensor::VirtualHandGloveSensor( Inst< OnEnable   > _enabled,
                                    Inst< SFNode   > _metadata,
                                    Inst< SFBool   > _isActive,
                                    Inst< SFString > _host,
                                    Inst< SFString > _port  ) :
  X3DSensorNode( _enabled, _metadata, _isActive ),
  host( _host ),
  port( _port ),
  update( new Update )
#ifdef HAVE_VIRTUAL_HAND_SDK
  , glove( 0 )
#endif
            {

  type_name = "VirtualHandGloveSensor";

  database.initFields( this );
  update->setOwner(this);
  Scene::time->routeNoEvent(update);

  host->setValue( "localhost" );
  port->setValue( "12345" );
}

void VirtualHandGloveSensor::initialize() {
  X3DSensorNode::initialize();
  // The enabled field is now set by the user.
  if( enabled->getValue() ) {
    if( connect() )
      isActive->setValue( true, id );
  }
}

