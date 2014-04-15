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
/// \file MultiDeviceSurface.cpp
/// \brief cpp file for MultiDeviceSurface
///
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/MultiDeviceSurface.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase MultiDeviceSurface::database( 
                                         "MultiDeviceSurface",
                                         &(newInstance<MultiDeviceSurface>),
                                         typeid( MultiDeviceSurface ),
                                         &H3DSurfaceNode::database );

namespace MultiDeviceSurfaceInternals {
  FIELDDB_ELEMENT( MultiDeviceSurface, surface, INPUT_OUTPUT );
}

MultiDeviceSurface::MultiDeviceSurface( Inst< MFSurfaceNode > _surface ) :
  H3DSurfaceNode(),
  surface( _surface ) {
  type_name = "MultiDeviceSurface";
  database.initFields( this );
}

HAPI::HAPISurfaceObject *MultiDeviceSurface::getSurface( unsigned int device_index ) {
  unsigned int size = (unsigned int) surface->size();

  // no surfaces
  if( size == 0 ) return NULL;
  
  unsigned int surface_index = device_index;
  if( device_index >= size ) surface_index = size - 1;

  H3DSurfaceNode *s = surface->getValueByIndex( surface_index );
  if( s ) return s->getSurface( device_index );
  else return NULL;
}
