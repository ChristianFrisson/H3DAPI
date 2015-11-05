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
/// \file H3DSingleTextureNode.cpp
/// \brief cpp file for H3DSingleTextureNode
///
//
//////////////////////////////////////////////////////////////////////////////
#include <H3D/H3DSingleTextureNode.h> 
#include <H3D/X3DShaderNode.h>
#include <H3D/X3DProgrammableShaderObject.h>
#include <H3D/Scene.h>

//#define DEBUG_BINDLESS

using namespace H3D;

H3DSingleTextureNode::TextureVector H3DSingleTextureNode::resident_textures;

/// Constructor
H3DSingleTextureNode::H3DSingleTextureNode( 
  Inst< DisplayList > _displayList,
  Inst< SFNode  > _metadata ): 
  X3DTextureNode( _displayList, _metadata ),
  texture_handle ( 0 ),
  last_used ( 0 ),
  is_resident ( false ),
  texture_id(0),
  texture_unit(0),
  texture_target(0){
}

H3DSingleTextureNode::~H3DSingleTextureNode () {
  if ( X3DProgrammableShaderObject::use_bindless_textures ) {
    makeNonResident ();
  }
}

bool H3DSingleTextureNode::makeResident () {
#ifdef GL_ARB_bindless_texture
  if ( !is_resident ) {
    if ( !texture_handle ) {
      texture_handle= glGetTextureHandleARB ( getTextureId() );
    }
    glMakeTextureHandleResidentARB ( texture_handle );
    if ( glGetError() == GL_NO_ERROR ) {
      Field::FieldVector fields= shader_fields;
      for ( Field::FieldVector::iterator i= fields.begin(); i != fields.end(); ++i ) {
        (*i)->touch ();
      }
      resident_textures.push_back ( this );
      is_resident= true;

#ifdef DEBUG_BINDLESS
      Console(LogLevel::Error) << "Resident texture count: " << resident_textures.size() << endl;
#endif

      return true;
    }
  }

#endif

  return false;
}

void H3DSingleTextureNode::makeNonResident () {
#ifdef GL_ARB_bindless_texture
  if ( is_resident && texture_handle ) {
    if ( glIsTextureHandleResidentARB ( texture_handle ) ) {
      glMakeTextureHandleNonResidentARB ( texture_handle );
    }

    // Ignore errors making the texture non-resident
    if ( glGetError () != GL_NO_ERROR ) {
      Console(LogLevel::Error) << "ERROR: Cannot make texture non-resident: " << getName() << endl;
    }

    is_resident= false;
    TextureVector::iterator i= find ( resident_textures.begin(), resident_textures.end(), this );
    if ( i != resident_textures.end() ) {
      resident_textures.erase ( i );
    }

#ifdef DEBUG_BINDLESS
        Console(LogLevel::Error) << "Resident texture count: " << resident_textures.size() << endl;
#endif
  }
#endif
}

void H3DSingleTextureNode::invalidateTextureHandle () {
  makeNonResident ();
  texture_handle= 0;
}

void H3DSingleTextureNode::addShaderField ( Field& _field ) { 
  shader_fields.push_back ( &_field );
}

void H3DSingleTextureNode::removeShaderField ( Field& _field ) { 
  Field::FieldVector::iterator i= find ( shader_fields.begin(), shader_fields.end(), &_field );
  if ( i != shader_fields.end() ) {
    shader_fields.erase ( i );
  }
}

void H3DSingleTextureNode::inUse () {
  last_used= Scene::time->getValue();
}

void H3DSingleTextureNode::makeUnusedTexturesNonResident ( H3DTime _unusedTime ) {
  TextureVector unused;
  for ( TextureVector::iterator i= resident_textures.begin(); i != resident_textures.end(); ++i ) {
    H3DSingleTextureNode* t= *i;
    if ( TimeStamp() - t->last_used > _unusedTime ) {
      unused.push_back ( t );
    }
  }

  for ( TextureVector::iterator i= unused.begin(); i != unused.end(); ++i ) {
    H3DSingleTextureNode* t= *i;
    t->makeNonResident ();
  }
}