//////////////////////////////////////////////////////////////////////////////
//    Copyright 2011-2014, SenseGraphics AB
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
/// \file H3DForceEffect.cpp
/// \brief Cpp file for H3DForceEffect.
///
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/H3DForceEffect.h>

using namespace H3D;

H3DNodeDatabase H3DForceEffect::database( "H3DForceEffect", 
                                           NULL,
                                           typeid( H3DForceEffect ),
                                           &X3DChildNode::database );

namespace H3DForceEffectInternals {
  FIELDDB_ELEMENT( H3DForceEffect, deviceIndex, INPUT_OUTPUT );
}

H3DForceEffect::H3DForceEffect( Inst< SFNode> _metadata,
                                Inst< MFInt32 > _deviceIndex ) :
  X3DChildNode( _metadata ),
  deviceIndex( _deviceIndex ) {
  type_name = "H3DForceEffect";
  database.initFields( this );
}
