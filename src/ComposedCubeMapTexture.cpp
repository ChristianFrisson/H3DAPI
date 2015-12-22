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
/// \file ComposedCubeMapTexture.cpp
/// \brief CPP file for ComposedCubeMapTexture, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/ComposedCubeMapTexture.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase ComposedCubeMapTexture::database( 
                           "ComposedCubeMapTexture", 
                           &(newInstance< ComposedCubeMapTexture > ), 
                           typeid( ComposedCubeMapTexture ),
                           &X3DEnvironmentTextureNode::database );

namespace ComposedCubeMapTextureInternals {
  FIELDDB_ELEMENT( ComposedCubeMapTexture, back, INPUT_OUTPUT );
  FIELDDB_ELEMENT( ComposedCubeMapTexture, front, INPUT_OUTPUT );
  FIELDDB_ELEMENT( ComposedCubeMapTexture, left, INPUT_OUTPUT );
  FIELDDB_ELEMENT( ComposedCubeMapTexture, right, INPUT_OUTPUT );
  FIELDDB_ELEMENT( ComposedCubeMapTexture, top, INPUT_OUTPUT );
  FIELDDB_ELEMENT( ComposedCubeMapTexture, bottom, INPUT_OUTPUT );
}

ComposedCubeMapTexture::ComposedCubeMapTexture( 
                                          Inst< DisplayList > _displayList,
                                          Inst< SFNode    > _metadata,
                                          Inst< SFTexture2DNode  > _back,
                                          Inst< SFTexture2DNode  > _front,
                                          Inst< SFTexture2DNode  > _left,
                                          Inst< SFTexture2DNode  > _right,
                                          Inst< SFTexture2DNode  > _top,
                                          Inst< SFTexture2DNode  > _bottom ) :
  X3DEnvironmentTextureNode( _displayList, _metadata ),
  back( _back ),
  front( _front ),
  left( _left ),
  right( _right ),
  top( _top ),
  bottom( _bottom ),
  textureUpdated( new EventCollectingField<Field> ){
    
  type_name = "ComposedCubeMapTexture";
  database.initFields( this );

  back->route( displayList );
  front->route( displayList );
  left->route( displayList );
  right->route( displayList );
  top->route( displayList );
  bottom->route( displayList );

  textureUpdated->setName( "textureUpdated" );
  back->route( textureUpdated );
  front->route( textureUpdated );
  left->route( textureUpdated );
  right->route( textureUpdated );
  top->route( textureUpdated );
  bottom->route( textureUpdated );
}

void ComposedCubeMapTexture::render() {
  if( !GLEW_ARB_texture_cube_map ) {
    Console(LogLevel::Error) << "Warning: ARB_texture_cube_map extension not supported "
               << "by your graphics card. ComposedCubeMapTexture node cannot "
               << "be used." << endl; 
  } else {
    glGetIntegerv( GL_ACTIVE_TEXTURE_ARB, &texture_unit );
    if( !cube_map_id ) {
      glGenTextures( 1, &cube_map_id );
    }
    X3DTexture2DNode *back_tex = back->getValue();
    X3DTexture2DNode *front_tex = front->getValue();
    X3DTexture2DNode *left_tex = left->getValue();
    X3DTexture2DNode *right_tex = right->getValue();
    X3DTexture2DNode *top_tex = top->getValue();
    X3DTexture2DNode *bottom_tex = bottom->getValue();
    
    if( back_tex && front_tex && left_tex && 
        right_tex && top_tex && bottom_tex ) {
      unsigned int dim;
      bool have_all_textures = true;
      bool invalid_dims = false;
      bool first_image = true;

      Image *back_image =  back_tex->image->getValue();
      Image *front_image =  front_tex->image->getValue();
      Image *left_image =  left_tex->image->getValue();
      Image *right_image =  right_tex->image->getValue();
      Image *top_image =  top_tex->image->getValue();
      Image *bottom_image =  bottom_tex->image->getValue();

      have_all_textures = 
        back_image && front_image &&
        left_image && right_image && 
        top_image && bottom_image;

      if( back_image ) { 
        if( first_image ) {
          first_image = false;
          dim = back_image->width();
        }
        if( back_image->height() != dim ) invalid_dims = true;
      } 

      if( front_image ) { 
        if( first_image ) {
          first_image = false;
          dim = front_image->width();
        }
        if( front_image->height() != dim || front_image->width() != dim ) 
          invalid_dims = true;
      } 

      if( left_image ) { 
        if( first_image ) {
          first_image = false;
          dim = left_image->width();
        }
        if( left_image->height() != dim || left_image->width() != dim ) 
          invalid_dims = true;
      } 

      if( right_image ) { 
        if( first_image ) {
          first_image = false;
          dim = right_image->width();
        }
        if( right_image->height() != dim || right_image->width() != dim ) 
          invalid_dims = true;
      } 

      if( top_image ) { 
        if( first_image ) {
          first_image = false;
          dim = top_image->width();
        }
        if( top_image->height() != dim || top_image->width() != dim ) 
          invalid_dims = true;
      } 

      if( bottom_image ) { 
        if( first_image ) {
          first_image = false;
          dim = bottom_image->width();
        }
        if( bottom_image->height() != dim || bottom_image->width() != dim ) 
          invalid_dims = true;
      } 

      if( !invalid_dims ) {
        glBindTexture( GL_TEXTURE_CUBE_MAP_ARB, cube_map_id );
        if( textureUpdated->hasCausedEvent( back ) && 
            back_tex->image->getValue() ){
          back_tex->glTexImage( back_tex->image->getValue(), 
                                GL_TEXTURE_CUBE_MAP_POSITIVE_Z, false );
        }
        if( textureUpdated->hasCausedEvent( front ) &&
            front_tex->image->getValue() ) {
          front_tex->glTexImage( front_tex->image->getValue(), 
                                 GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, false );
        }
        if( textureUpdated->hasCausedEvent( left ) &&
            left_tex->image->getValue() ) {
          left_tex->glTexImage( left_tex->image->getValue(), 
                                GL_TEXTURE_CUBE_MAP_NEGATIVE_X, false );
        }
        if( textureUpdated->hasCausedEvent( right ) &&
            right_tex->image->getValue()) {
          right_tex->glTexImage( right_tex->image->getValue(), 
                                 GL_TEXTURE_CUBE_MAP_POSITIVE_X, false );
        }
        if( textureUpdated->hasCausedEvent( top ) &&
            top_tex->image->getValue()) {
          top_tex->glTexImage( top_tex->image->getValue(), 
                               GL_TEXTURE_CUBE_MAP_POSITIVE_Y, false );
        }
        if( textureUpdated->hasCausedEvent( bottom ) &&
            bottom_tex->image->getValue()) {
          bottom_tex->glTexImage( bottom_tex->image->getValue(), 
                                  GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, false );
        }
        
        if( have_all_textures ) {
          enableTexturing();
          
          glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
          glTexParameteri( GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_WRAP_S, 
                           GL_CLAMP_TO_EDGE);
          glTexParameteri( GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_WRAP_T,
                           GL_CLAMP_TO_EDGE);
          glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_MAG_FILTER, 
                          GL_LINEAR);
          glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_MIN_FILTER, 
                          GL_LINEAR);
        } 
      } else {
        glBindTexture( GL_TEXTURE_CUBE_MAP_ARB, 0 );
        Console(LogLevel::Warning) << "Warning: Invalid cube map textures in \"" << getName()
                   << "\" node. All images must have the same square dimensions."
                   << endl;
      }
    } else {
      Console(LogLevel::Warning) << "Warning: All cube sides not specified in \"" 
                 << getName() << "\" node. Node will be disabled. " << endl;
    }
    textureUpdated->upToDate();
  }
}

void ComposedCubeMapTexture::enableTexturing() {
  glEnable( GL_TEXTURE_CUBE_MAP_ARB );
}

void ComposedCubeMapTexture::disableTexturing() {
  glDisable( GL_TEXTURE_CUBE_MAP_ARB );
}

