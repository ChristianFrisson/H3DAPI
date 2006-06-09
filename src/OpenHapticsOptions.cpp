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
/// \file OpenHapticsOptions.cpp
/// \brief CPP file for OpenHapticsOptions.
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include "OpenHapticsOptions.h"

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase OpenHapticsOptions::database( "OpenHapticsOptions", 
                                      &(newInstance<OpenHapticsOptions>),
                                      typeid( OpenHapticsOptions ),
                                      &X3DRenderOptionsNode::database );

namespace OpenHapticsOptionsInternals {
  FIELDDB_ELEMENT( OpenHapticsOptions, GLShape, INPUT_OUTPUT );
  FIELDDB_ELEMENT( OpenHapticsOptions, touchableFace, INPUT_OUTPUT );
  FIELDDB_ELEMENT( OpenHapticsOptions, useAdaptiveViewport, INPUT_OUTPUT );
  FIELDDB_ELEMENT( OpenHapticsOptions, useHapticCameraView, INPUT_OUTPUT );
}

OpenHapticsOptions::OpenHapticsOptions( 
                           Inst< SFNode>  _metadata,
                           Inst< SFString > _GLShape,
                           Inst< SFString  >  _touchableFace,
                           Inst< SFBool    >  _useAdaptiveViewport,
                           Inst< SFBool    >  _useHapticCameraView ) :
  X3DRenderOptionsNode( _metadata ),
  GLShape( _GLShape ),
  touchableFace( _touchableFace ),
  useAdaptiveViewport( _useAdaptiveViewport ),
  useHapticCameraView( _useHapticCameraView )  {
  type_name = "OpenHapticsOptions";

  database.initFields( this );

  GLShape->setValue( "FEEDBACK_BUFFER" );
  touchableFace->setValue( "FRONT_AND_BACK" );
  useAdaptiveViewport->setValue( true );
  useHapticCameraView->setValue( true );
}


