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
/// \file MultiTexture.cpp
/// \brief CPP file for MultiTexture, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include "MultiTexture.h"

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase MultiTexture::database( 
                                       "MultiTexture", 
                                       &(newInstance<MultiTexture>), 
                                       typeid( MultiTexture ),
                                       &X3DTextureNode::database );

namespace MultiTextureInternals {
  FIELDDB_ELEMENT( MultiTexture, alpha, INPUT_OUTPUT );
  FIELDDB_ELEMENT( MultiTexture, color, INPUT_OUTPUT );
  FIELDDB_ELEMENT( MultiTexture, function, INPUT_OUTPUT );
  FIELDDB_ELEMENT( MultiTexture, mode, INPUT_OUTPUT );
  FIELDDB_ELEMENT( MultiTexture, source, INPUT_OUTPUT );
  FIELDDB_ELEMENT( MultiTexture, texture, INPUT_OUTPUT );
}


bool MultiTexture::multitexture_support_checked = false;


MultiTexture::MultiTexture( 
                           Inst< DisplayList > _displayList,
                           Inst< SFFloat  >  _alpha,
                           Inst< SFColor  >  _color,
                           Inst< MFString >  _function,
                           Inst< SFNode   >  _metadata,
                           Inst< MFString >  _mode,
                           Inst< MFString >  _source,
                           Inst< MFTexture >  _texture ):
  X3DTextureNode( _displayList, _metadata ),
  alpha( _alpha ),
  color( _color ),
  function( _function ),
  mode( _mode ),
  source( _source ),
  texture( _texture ) {

  alpha->setValue( 1 );
  color->setValue( RGB( 1, 1, 1 ) );
  type_name = "MultiTexture";

  database.initFields( this );

  alpha->route( displayList );
  color->route( displayList );
  function->route( displayList );
  mode->route( displayList );
  source->route( displayList );
  texture->route( displayList );
}

void MultiTexture::render() {
  if( !multitexture_support_checked ) {
    if( !GLEW_ARB_multitexture ) {
      stringstream s;
      s << "Your graphic card driver does not support "
        << "multi texturing so you cannot use the MultiTexture "
        << "node. " << ends;
      throw MultiTexturesNotSupported( s.str() );
    }
  }
  int used_texture_units = 0;
  GLint saved_texture;
  glGetIntegerv( GL_ACTIVE_TEXTURE_ARB, &saved_texture );
    
  int nr_textures_supported;
  glGetIntegerv( GL_MAX_TEXTURE_UNITS_ARB, &nr_textures_supported );
    
  
  for( MFTexture::const_iterator i = texture->begin();
       i != texture->end();
       i++ ) {
        
    if( used_texture_units >= nr_textures_supported ) {
      cerr << "Warning! MultiTexture: Unable to display all"
           << "textures. Your device only has support for " 
           << nr_textures_supported << " texture units.\n";
      break;
    }
    glActiveTexture( GL_TEXTURE0_ARB + used_texture_units );
    used_texture_units++;
    static_cast< X3DTextureNode * >(*i)->displayList->callList();
  }
  glActiveTexture( saved_texture );
  setActiveTexture( this );
}

void MultiTexture::enableTexturing() {
  GLint saved_texture;
  glGetIntegerv( GL_ACTIVE_TEXTURE_ARB, &saved_texture );
  int used_texture_units = 0;
  for( MFTexture::const_iterator i = texture->begin();
       i != texture->end();
       i++ ) {
    if( dynamic_cast< MultiTexture * >( *i ) ){
      //TODO::::
    } else {
      glActiveTexture( GL_TEXTURE0_ARB + used_texture_units );
      used_texture_units++;
      static_cast< X3DTextureNode * >(*i)->enableTexturing();
    }
  }
  glActiveTexture( saved_texture );
}


void MultiTexture::disableTexturing() {
  GLint saved_texture;
  glGetIntegerv( GL_ACTIVE_TEXTURE_ARB, &saved_texture );
  int used_texture_units = 0;
  for( MFTexture::const_iterator i = texture->begin();
       i != texture->end();
       i++ ) {
    if( dynamic_cast< MultiTexture * >( *i ) ) {
      //TODO:::
    } else {
      glActiveTexture( GL_TEXTURE0_ARB + used_texture_units );
      used_texture_units++;
      static_cast< X3DTextureNode * >(*i)->disableTexturing();
    }
  }
  glActiveTexture( saved_texture );
}
