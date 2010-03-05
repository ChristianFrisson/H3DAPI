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
/// \file RenderProperties.cpp
/// \brief CPP file for RenderProperties
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/RenderProperties.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase RenderProperties::database( 
                                         "RenderProperties", 
                                         &(newInstance<RenderProperties>), 
                                         typeid( RenderProperties ),
                                         &X3DAppearanceChildNode::database );

namespace RenderPropertiesInternals {
  FIELDDB_ELEMENT( RenderProperties, depthTestEnabled, INPUT_OUTPUT );
  FIELDDB_ELEMENT( RenderProperties, depthBufferWriteEnabled, INPUT_OUTPUT );
  FIELDDB_ELEMENT( RenderProperties, colorBufferRedWriteEnabled, INPUT_OUTPUT );
  FIELDDB_ELEMENT( RenderProperties, colorBufferGreenWriteEnabled, INPUT_OUTPUT );
  FIELDDB_ELEMENT( RenderProperties, colorBufferBlueWriteEnabled, INPUT_OUTPUT );
  FIELDDB_ELEMENT( RenderProperties, colorBufferAlphaWriteEnabled, INPUT_OUTPUT );
  FIELDDB_ELEMENT( RenderProperties, smoothShading, INPUT_OUTPUT );
  FIELDDB_ELEMENT( RenderProperties, multiPassTransparency, INPUT_OUTPUT );
}


RenderProperties::RenderProperties( Inst< SFNode      >  _metadata,
                                    Inst< DisplayList > _displayList,
                                    Inst< SFBool      > _depthTestEnabled,
                                    Inst< SFBool      > _smoothShading,
                                    Inst< SFBool      > _multiPassTransparency,
                                    Inst< SFBool      > _depthBufferWriteEnabled,
                                    Inst< SFBool      > _colorBufferRedWriteEnabled,
                                    Inst< SFBool      > _colorBufferGreenWriteEnabled,
                                    Inst< SFBool      > _colorBufferBlueWriteEnabled,
                                    Inst< SFBool      > _colorBufferAlphaWriteEnabled ) :
  X3DAppearanceChildNode( _displayList, _metadata ),
  depthTestEnabled      ( _depthTestEnabled ),
  smoothShading         ( _smoothShading ),
  multiPassTransparency( _multiPassTransparency ),
  depthBufferWriteEnabled( _depthBufferWriteEnabled ),
  colorBufferRedWriteEnabled( _colorBufferRedWriteEnabled ),
  colorBufferGreenWriteEnabled( _colorBufferGreenWriteEnabled ),
  colorBufferBlueWriteEnabled( _colorBufferBlueWriteEnabled ),
  colorBufferAlphaWriteEnabled( _colorBufferAlphaWriteEnabled ) {

  type_name = "RenderProperties";
  database.initFields( this );
  
  depthTestEnabled->setValue( true );
  smoothShading->setValue( true );
  multiPassTransparency->setValue( true );
  depthBufferWriteEnabled->setValue( true );
  colorBufferRedWriteEnabled->setValue( true );
  colorBufferGreenWriteEnabled->setValue( true );
  colorBufferBlueWriteEnabled->setValue( true );
  colorBufferAlphaWriteEnabled->setValue( true );

  depthTestEnabled->route( displayList );
  smoothShading->route( displayList );
  multiPassTransparency->route( displayList );
  depthBufferWriteEnabled->route( displayList );
  colorBufferRedWriteEnabled->route( displayList );
  colorBufferGreenWriteEnabled->route( displayList );
  colorBufferBlueWriteEnabled->route( displayList );
  colorBufferAlphaWriteEnabled->route( displayList );

}


void RenderProperties::render() {

  if( depthTestEnabled->getValue() ) {
    glEnable( GL_DEPTH_TEST );
  } else {
    glDisable( GL_DEPTH_TEST );
  }

  if( smoothShading->getValue() ) {
    glShadeModel( GL_SMOOTH );
  } else {
    glShadeModel( GL_FLAT );
  }

  if( depthBufferWriteEnabled->getValue() ) {
    glDepthMask( GL_TRUE );
  } else {
    glDepthMask( GL_FALSE );
  }

  glColorMask( colorBufferRedWriteEnabled->getValue(),
               colorBufferGreenWriteEnabled->getValue(),
               colorBufferBlueWriteEnabled->getValue(),
               colorBufferAlphaWriteEnabled->getValue() );

}
