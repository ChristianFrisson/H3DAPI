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
/// \file ComposedShader.cpp
/// \brief CPP file for ComposedShader.
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include "ShaderFunctions.h"
#include "ComposedShader.h"
#include "X3DTexture2DNode.h"
#include "X3DTexture3DNode.h"

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase ComposedShader::database( 
                                   "ComposedShader", 
                                   &(newInstance<ComposedShader>), 
                                   typeid( ComposedShader ),
                                   &X3DShaderNode::database );

namespace ComposedShaderInternals {
  FIELDDB_ELEMENT( ComposedShader, parts, INPUT_OUTPUT );
}

ComposedShader::ComposedShader( Inst< DisplayList  > _displayList,
                                Inst< SFNode       > _metadata,
                                Inst< SFBool       > _isSelected ,
                                Inst< SFBool       > _isValid,
                                Inst< SFBool       > _activate,
                                Inst< SFString     > _language,
                                Inst< MFShaderPart > _parts ) :
  X3DShaderNode( _displayList, _metadata, _isSelected, 
                 _isValid, _activate),
  X3DProgrammableShaderObject( &database ),
  parts( _parts ),
  program_handle( 0 ) {
  type_name = "ComposedShader";
  database.initFields( this );

  activate->route( displayList, id );
  parts->route( displayList, id );
}

bool ComposedShader::shader_support_checked = false;

bool ComposedShader::addField( const string &name,
                               const Field::AccessType &access,
                               Field *field ) {
  bool success = X3DProgrammableShaderObject::addField( name, 
                                                        access, 
                                                        field  );	
  if( success ) {
    field->route( displayList );
  }
  return success;
}

void ComposedShader::preRender() {
  if( GLEW_ARB_shader_objects ) {
    glUseProgramObjectARB( program_handle );
    Shaders::preRenderTextures( this );
    X3DShaderNode::preRender();
  }
}

void ComposedShader::postRender() {
  if( GLEW_ARB_shader_objects ) {
    glUseProgramObjectARB( 0 );
    Shaders::postRenderTextures( this );
    X3DShaderNode::postRender();
  }
}

void ComposedShader::render() {
  if( !GLEW_ARB_shader_objects ) {
    if( !shader_support_checked ) {
      Console(4) << "Your graphic card driver does not support "
                 << "shader objects( ARB_shader_objects) so you cannot"
                 << " use the ComposedShader node. Shader will be disabled"
                 << endl;
      shader_support_checked = true;
    } 
    if( isValid->getValue() ) isValid->setValue( false, id );
  } else {
    bool all_parts_valid = true;
    // compile all shader parts
    for( MFShaderPart::const_iterator i = parts->begin();
         i != parts->end(); i++ ) {
      if( static_cast< ShaderPart * >(*i)->compileShader() == 0 ) {
        all_parts_valid = false;
      }
    }
    
    if( isValid->getValue() != all_parts_valid )
      isValid->setValue( all_parts_valid, id );
    
    if( all_parts_valid ) {
      // if the first time we run the shader or a TRUE event has been sent
      // to the activate field we must relink the program.
      if( ( displayList->hasCausedEvent( activate ) && activate->getValue( id ) )
          || !program_handle ) {
        if( !program_handle ) program_handle = glCreateProgramObjectARB();
        // detach the old shaders from the program
        for( vector< GLhandleARB >::iterator i = current_shaders.begin();
             i != current_shaders.end(); i++ ) {
          glDetachObjectARB( program_handle, *i );
        }
        current_shaders.clear();
        if( parts->size() > 0 ) {
          // add the new ones.
          for( MFShaderPart::const_iterator i = parts->begin();
               i != parts->end(); i++ ) {
            GLhandleARB handle = 
              static_cast< ShaderPart * >(*i)->getShaderHandle();
            glAttachObjectARB( program_handle, handle );
            current_shaders.push_back( handle );
          }
          // link shader program
          glLinkProgramARB( program_handle );
          GLint link_success;
          glGetObjectParameterivARB( program_handle,
                                     GL_OBJECT_LINK_STATUS_ARB,
                                     &link_success );
          if( link_success == GL_FALSE ) {
            // linking failed, print error message
            GLint nr_characters;
            glGetObjectParameterivARB( program_handle,
                                       GL_OBJECT_INFO_LOG_LENGTH_ARB,
                                       &nr_characters );
            GLcharARB *log = new GLcharARB[nr_characters];
            glGetInfoLogARB( program_handle,
                             nr_characters,
                             NULL,
                             log );
            Console(3) << "Warning: Error while linking shader parts in \""
                       << getName() << "\" node. " << endl
                       << log << endl;
            glDeleteObjectARB( program_handle );
            program_handle = 0;
          }
        } else {
          glDeleteObjectARB( program_handle );
          program_handle = 0;
        }
        glUseProgramObjectARB( program_handle );
      }
    }
    
    if( program_handle ) {
      Shaders::renderTextures( this );

      for( unsigned int i = 0; i < dynamic_fields.size(); i++ ) {
        if( !Shaders::setGLSLUniformVariableValue( program_handle, 
                                                   dynamic_fields[i] ) ) {
          Console(3) << "Warning: Uniform variable \"" 
                     <<  dynamic_fields[i]->getName() 
                     << "\" not defined in shader source of the ShaderPart nodes "
                     << "in the node \"" << getName() << "\"" << endl;
        }
      }
    }
  }
}

