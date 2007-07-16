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
/// \file X3DTextureCoordinateNode.cpp
/// \brief CPP file for X3DTextureCoordinateNode, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <X3DTextureCoordinateNode.h>
#include "GL/glew.h"
#include <MultiTexture.h>
#include <X3DTextureNode.h>

using namespace H3D;

X3DTextureCoordinateNode::X3DTextureCoordinateNode( 
                                                   Inst< SFNode>  _metadata ) :
  X3DGeometricPropertyNode( _metadata ) {
  type_name = "X3DTextureCoordinateNode";
}

void X3DTextureCoordinateNode::renderForTextureUnits( int index,
                                                      unsigned int start_unit,
                                                      unsigned int end_unit ) {
  for( unsigned int i = start_unit; i <= end_unit; i++ ) {
    renderForTextureUnit( index, i );
  }
}

void X3DTextureCoordinateNode::renderArrayForTextureUnits( unsigned int start_unit,
                                                           unsigned int end_unit ) {
  GLint saved_texture;
  glGetIntegerv( GL_CLIENT_ACTIVE_TEXTURE_ARB, &saved_texture );
  for( unsigned int i = start_unit; i <= end_unit; i++ ) {
    renderArrayForTextureUnit( i );
  }
  glClientActiveTexture( saved_texture );
}

/// Disable the array state enabled in renderArrayForTextureUnits().
void X3DTextureCoordinateNode::disableArrayForTextureUnits( 
                                   unsigned int start_unit,
                                   unsigned int end_unit ) {
  GLint saved_texture;
  glGetIntegerv( GL_CLIENT_ACTIVE_TEXTURE_ARB, &saved_texture );
  for( unsigned int i = start_unit; i <= end_unit; i++ ) {
    disableArrayForTextureUnit( i );
  }
  glClientActiveTexture( saved_texture );
}

void X3DTextureCoordinateNode::renderTexCoordForActiveTexture( const Vec3f &tc ) {
  renderTexCoordForTexture( tc, X3DTextureNode::getActiveTexture() );
}

void X3DTextureCoordinateNode::renderTexCoordForTexture( const Vec3f &tc,
                                            X3DTextureNode *t) {
  MultiTexture *mt = 
    dynamic_cast< MultiTexture * >( t );
  if( mt ) {
    size_t texture_units = mt->texture->size();
    for( unsigned int i = 0; i < texture_units; i++ ) {
      glMultiTexCoord3f( GL_TEXTURE0_ARB + i, tc.x, tc.y, tc.z );
    }
  } else {
    glTexCoord3f( tc.x, tc.y, tc.z );
  }
}

void X3DTextureCoordinateNode::renderArrayForTextureUnit( 
                                  unsigned int texture_unit ) {
  GLint saved_texture;
  glGetIntegerv( GL_CLIENT_ACTIVE_TEXTURE_ARB, &saved_texture );
  glClientActiveTexture( GL_TEXTURE0_ARB + texture_unit );
  renderArray();
  glClientActiveTexture( saved_texture );
}

void X3DTextureCoordinateNode::disableArrayForTextureUnit( 
                                  unsigned int texture_unit ) {
  GLint saved_texture;
  glGetIntegerv( GL_CLIENT_ACTIVE_TEXTURE_ARB, &saved_texture );
  glClientActiveTexture( GL_TEXTURE0_ARB + texture_unit );
  disableArray();
  glClientActiveTexture( saved_texture );
}


void X3DTextureCoordinateNode::renderForTexture( int index, 
                                                 X3DTextureNode *t ) {
  MultiTexture *mt = 
    dynamic_cast< MultiTexture * >( t );
  if( mt ) {
    size_t texture_units = mt->texture->size();
    for( unsigned int i = 0; i < texture_units; i++ ) {
      renderForTextureUnit( index, i );
    }
  } else {
    renderForTextureUnit( index, 0 );
  }
}


void X3DTextureCoordinateNode::renderForActiveTexture( int index ) {
  renderForTexture( index, X3DTextureNode::getActiveTexture() );
}

void X3DTextureCoordinateNode::startTexGenForTextureUnit( unsigned int texture_unit  ) {
  GLint saved_texture;
  glGetIntegerv( GL_ACTIVE_TEXTURE_ARB, &saved_texture );
  glActiveTexture( GL_TEXTURE0_ARB + texture_unit );
  this->startTexGen();
  glActiveTexture( saved_texture );
}

void X3DTextureCoordinateNode::stopTexGenForTextureUnit( unsigned int texture_unit  ) {
  GLint saved_texture;
  glGetIntegerv( GL_ACTIVE_TEXTURE_ARB, &saved_texture );
  glActiveTexture( GL_TEXTURE0_ARB + texture_unit );
  stopTexGen();
  glActiveTexture( saved_texture );
}


void X3DTextureCoordinateNode::startTexGenForTextureUnits( 
                                   unsigned int start_unit,
                                   unsigned int end_unit ) {
  for( unsigned int i = start_unit; i <= end_unit; i++ ) {
    startTexGenForTextureUnit( i );
  }
}

void X3DTextureCoordinateNode::stopTexGenForTextureUnits( 
                                   unsigned int start_unit,
                                   unsigned int end_unit ) {
  for( unsigned int i = start_unit; i <= end_unit; i++ ) {
    stopTexGenForTextureUnit( i );
  }
}


void X3DTextureCoordinateNode::startTexGenForActiveTexture() {
  startTexGenForTexture( X3DTextureNode::getActiveTexture() );
}

void X3DTextureCoordinateNode::stopTexGenForActiveTexture() {
  stopTexGenForTexture( X3DTextureNode::getActiveTexture() );
}

void X3DTextureCoordinateNode::startTexGenForTexture( X3DTextureNode *t ) {
  MultiTexture *mt = 
    dynamic_cast< MultiTexture * >( t );
  if( mt && mt->texture->size() > 0 ) {
    size_t texture_units = mt->texture->size();
    startTexGenForTextureUnits( 0, texture_units - 1 );
  } else {
    startTexGenForTextureUnit( 0 );
  }
}

void X3DTextureCoordinateNode::stopTexGenForTexture( X3DTextureNode *t ) {
  MultiTexture *mt = 
    dynamic_cast< MultiTexture * >( t );
  if( mt && mt->texture->size() > 0 ) {
    size_t texture_units = mt->texture->size();
    stopTexGenForTextureUnits( 0, texture_units - 1 );
  } else {
    stopTexGenForTextureUnit( 0 );
  }
}


void X3DTextureCoordinateNode::renderArrayForTexture( X3DTextureNode *t ) {
  MultiTexture *mt = 
    dynamic_cast< MultiTexture * >( t );

  if( mt && mt->texture->size() > 0 ) {
    size_t texture_units = mt->texture->size();
    renderArrayForTextureUnits( 0, texture_units -1 );
  } else {
    renderArrayForTextureUnit( 0 );
  }
}


void X3DTextureCoordinateNode::renderArrayForActiveTexture() {
  renderArrayForTexture( X3DTextureNode::getActiveTexture() );
}

void X3DTextureCoordinateNode::disableArrayForTexture( X3DTextureNode *t ) {
  MultiTexture *mt = 
    dynamic_cast< MultiTexture * >( t );

  if( mt && mt->texture->size() > 0 ) {
    size_t texture_units = mt->texture->size();
    disableArrayForTextureUnits( 0, texture_units -1 );
  } else {
    disableArrayForTextureUnit( 0 );
  }
}


void X3DTextureCoordinateNode::disableArrayForActiveTexture() {
  disableArrayForTexture( X3DTextureNode::getActiveTexture() );
}
