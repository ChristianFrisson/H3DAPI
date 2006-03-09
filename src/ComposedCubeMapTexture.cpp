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
/// \file ComposedCubeMapTexture.cpp
/// \brief CPP file for ComposedCubeMapTexture, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include "ComposedCubeMapTexture.h"

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
  bottom( _bottom ) {
    
  type_name = "ComposedCubeMapTexture";
  database.initFields( this );

  back->route( displayList );
  front->route( displayList );
  left->route( displayList );
  right->route( displayList );
  top->route( displayList );
  bottom->route( displayList );
}

void ComposedCubeMapTexture::render() {
  if( !GLEW_ARB_texture_cube_map ) {
    Console(4) << "Warning: ARB_texture_cube_map extension not supported "
               << "by your graphics card. ComposedCubeMapTexture node cannot "
               << "be used." << endl; 
  } else {
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
      bool textures_ok = true;
      Image *i = back_tex->image->getValue();
      if( i ) { 
        dim = i->width();
        textures_ok = textures_ok && ( i->height() == dim );
        i = front_tex->image->getValue();
        textures_ok = i && textures_ok && i->width() == dim && 
          i->height() == dim;
        i = left_tex->image->getValue();
        textures_ok = i && textures_ok && i->width() == dim && 
          i->height() == dim;
        i = right_tex->image->getValue();
        textures_ok = i && textures_ok && i->width() == dim && 
          i->height() == dim;
        i = top_tex->image->getValue();
        textures_ok = i && textures_ok && i->width() == dim && 
          i->height() == dim;
        i = bottom_tex->image->getValue();
        textures_ok = i && textures_ok && i->width() == dim && 
          i->height() == dim;
	  } else {
        textures_ok = false;
	  }

      if( textures_ok ) {
        
        glBindTexture( GL_TEXTURE_CUBE_MAP_ARB, cube_map_id );
        if( displayList->hasCausedEvent( back ) ) {
          back_tex->glTexImage( back_tex->image->getValue(), 
                                GL_TEXTURE_CUBE_MAP_POSITIVE_Z, false );
        }
        if( displayList->hasCausedEvent( front ) ) {
          front_tex->glTexImage( front_tex->image->getValue(), 
                                 GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, false );
        }
        if( displayList->hasCausedEvent( left ) ) {
          left_tex->glTexImage( left_tex->image->getValue(), 
                                GL_TEXTURE_CUBE_MAP_NEGATIVE_X, false );
        }
        if( displayList->hasCausedEvent( right ) ) {
          right_tex->glTexImage( right_tex->image->getValue(), 
                                 GL_TEXTURE_CUBE_MAP_POSITIVE_X, false );
        }
        if( displayList->hasCausedEvent( top ) ) {
          top_tex->glTexImage( top_tex->image->getValue(), 
                               GL_TEXTURE_CUBE_MAP_POSITIVE_Y, false );
        }
        if( displayList->hasCausedEvent( bottom ) ) {
          bottom_tex->glTexImage( bottom_tex->image->getValue(), 
                                  GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, false );
        }
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
      } else {
        glBindTexture( GL_TEXTURE_CUBE_MAP_ARB, 0 );
        Console(3) << "Warning: Invalid cube map textures in \"" << getName()
                   << "\" node. All images must have the same square dimensions."
                   << endl;
      }
    } else {
      Console(3) << "Warning: All cube sides not specified in \"" 
                 << getName() << "\" node. Node will be disabled. " << endl;
    }
  }
}

void ComposedCubeMapTexture::enableTexturing() {
  glEnable( GL_TEXTURE_CUBE_MAP_ARB );
}

void ComposedCubeMapTexture::disableTexturing() {
  glDisable( GL_TEXTURE_CUBE_MAP_ARB );
}

