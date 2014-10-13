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
/// \file X3DProgrammableShaderObject.cpp
/// \brief CPP file for X3DProgrammableShaderObject  Abstract X3D-node.
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/X3DProgrammableShaderObject.h>
#include <H3D/GlobalSettings.h>

using namespace H3D;

bool X3DProgrammableShaderObject::use_bindless_textures = false;
bool X3DProgrammableShaderObject::use_bindless_textures_set = false;

X3DProgrammableShaderObject::X3DProgrammableShaderObject( H3DNodeDatabase* _database ) {

  bool wants_bindless_textures= false;
  if( GlobalSettings* default_settings= GlobalSettings::getActive() ) {
    GraphicsOptions* options;
    default_settings->getOptionNode( options );
    wants_bindless_textures= options->bindlessTextures->getValue();
  }

#ifdef GL_ARB_bindless_texture
  bool bindless_textures= GLEW_ARB_bindless_texture && wants_bindless_textures;
  if ( !use_bindless_textures_set ) {
    use_bindless_textures= bindless_textures;
    use_bindless_textures_set= true;
	if ( wants_bindless_textures && !GLEW_ARB_bindless_texture ) {
	  Console(4) << "WARNING: Bindless textures are not supported by your system! Using regular textures instead." << endl;
	}
  } else if ( bindless_textures != use_bindless_textures ) {
    Console(4) << "WARNING: Cannot change bindlessTextures option after creating a shader node!" << endl;
  }
#endif

}

X3DProgrammableShaderObject::~X3DProgrammableShaderObject() {
}

bool X3DProgrammableShaderObject::addField ( 
  const string &name,
  const Field::AccessType &access,
  Field *field ) {
  bool success= H3DDynamicFieldsObject::addField ( name, access, field );
  
  if ( X3DProgrammableShaderObject::use_bindless_textures && success ) {
    if ( field->getX3DType() == X3DTypes::SFNODE || 
         field->getX3DType() == X3DTypes::MFNODE ) {

      if ( !update_textures.get() ) {
        update_textures.reset ( new UpdateTextures );
      }

      field->route ( update_textures );
    }
  }

  return success;
}
    
bool X3DProgrammableShaderObject::removeField ( const string& _name ) {

  if ( X3DProgrammableShaderObject::use_bindless_textures ) {
    Field* field= NULL;
    for ( field_iterator i= firstField(); i != endField(); ++i ) {
      if ( (*i)->getName () == _name ) {
        field= *i;
        break;
      }
    }
    if ( !field ) return false;

    field->unroute ( update_textures );
    update_textures->clearField ( *field );
  }

  return H3DDynamicFieldsObject::removeField ( _name );
}

void X3DProgrammableShaderObject::clearFields() {
  if ( X3DProgrammableShaderObject::use_bindless_textures ) {
    for ( field_iterator i= firstField(); i != endField(); ++i ) {
      Field* field= *i;
      field->unroute ( update_textures );
      update_textures->clearField ( *field );
    }
  }

  H3DDynamicFieldsObject::clearFields();
}

void X3DProgrammableShaderObject::UpdateTextures::clearField ( Field& _field ) {
  FieldToNodes::iterator i= fields_to_textures.find ( &_field );
  if ( i != fields_to_textures.end() ) {
    NodeVector& n= (*i).second;
    for ( NodeVector::const_iterator j= n.begin(); j != n.end(); ++j ) {
      static_cast < H3DSingleTextureNode* > ( *j )->removeShaderField ( _field );
    }
    fields_to_textures.erase ( &_field );
  }
}

void X3DProgrammableShaderObject::UpdateTextures::update () {
  AutoUpdate< Field >::update();

  clearField ( *event.ptr );

  switch ( event.ptr->getX3DType() ) {
  case X3DTypes::SFNODE: {
    SFNode* f= static_cast< SFNode * >( event.ptr );
    if ( H3DSingleTextureNode* t= dynamic_cast < H3DSingleTextureNode* > ( f->getValue() ) ) {
      t->addShaderField ( *f );
      fields_to_textures[f].push_back ( t );
    }

    break;
  }
  case X3DTypes::MFNODE: {
    MFNode* f= static_cast< MFNode * >( event.ptr );
    const NodeVector& n= f->getValue();
    for ( NodeVector::const_iterator i= n.begin(); i != n.end(); ++i ) {
      if ( H3DSingleTextureNode* t= dynamic_cast < H3DSingleTextureNode* > ( *i ) ) {
        t->addShaderField ( *f );
        fields_to_textures[f].push_back ( t );
      }
    }
    break;
  }
  default:
    Console(4) << "ERROR: Unknown field type!" << endl;
    break;
  }

}

X3DProgrammableShaderObject::UpdateTextures::~UpdateTextures () {
  for ( FieldToNodes::iterator i= fields_to_textures.begin(); i != fields_to_textures.end(); ++i ) {
    clearField ( *(*i).first );
  }
}
