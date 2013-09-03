//////////////////////////////////////////////////////////////////////////////
//    Copyright 2004-2013, SenseGraphics AB
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
/// \file ShaderProgram.cpp
/// \brief CPP file for ShaderProgram.
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/ShaderProgram.h>
#include <H3D/X3DTexture2DNode.h>
#include <H3D/X3DTexture3DNode.h>
#include <H3D/ShaderFunctions.h>
#include "H3D/ResourceResolver.h"

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase ShaderProgram::database( 
                                   "ShaderProgram", 
                                   &(newInstance<ShaderProgram>), 
                                   typeid( ShaderProgram ),
                                   &X3DNode::database );

namespace ShaderProgramInternals {
  FIELDDB_ELEMENT( ShaderProgram, url, INPUT_OUTPUT );
  FIELDDB_ELEMENT( ShaderProgram, type, INITIALIZE_ONLY );
}

ShaderProgram::ShaderProgram( Inst< SFNode      > _metadata,
                              Inst< DisplayList > _displayList,
                              Inst< MFString    > _url ,
                              Inst< SFString    > _type ):
  X3DNode( _metadata ),
  X3DUrlObject( _url ),
  X3DProgrammableShaderObject( &database ),
  H3DDisplayListObject( _displayList ),
  type( _type )
#ifdef HAVE_CG
  ,cg_context( NULL ),
  cg_program( NULL ) 
#endif
{

  type_name = "ShaderProgram";
  database.initFields( this );
  displayList->setOwner( this );
  type->setValue( "VERTEX", id );

  url->route( displayList );
#ifdef HAVE_CG
  setCGProfile( "CG" );
#endif
}

#ifdef HAVE_CG
ShaderProgram::~ShaderProgram() {
  cgDestroyContext( cg_context );
}

void ShaderProgram::render() {
  if( !cg_program ) {
    initCGShaderProgram();
  }

  if( cg_program ) {
    for( unsigned int i = 0; i < dynamic_fields.size(); ++i ) {
      if( !Shaders::setCGUniformVariableValue( cg_program, 
                                               dynamic_fields[i] ) ) {
        Console(3) << "Warning: Uniform variable \"" 
                   <<  dynamic_fields[i]->getName() 
                   << "\" not defined in shader source (" << source_url << ")" 
                   << " of the node \"" << getName() << "\"" << endl;
      }
    }
  }
}

bool ShaderProgram::addField( const string &name,
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

void ShaderProgram::enableCGShaderProgram() {
  if( !cg_program ) {
    initCGShaderProgram();
  }

  if( cg_program ) {
    Shaders::preRenderTextures( this );
    Shaders::renderTextures( this );
    cgGLBindProgram( cg_program );
    cgGLEnableProfile( cg_profile );
  }
}

void ShaderProgram::disableCGShaderProgram() {
  if( cg_program ) {
    cgGLDisableProfile( cg_profile );
    Shaders::postRenderTextures( this );
  }
}

void ShaderProgram::initCGShaderProgram() {
  setURLUsed( "" );
  const string &shader_type = type->getValue();
  if( !(shader_type == "FRAGMENT" || shader_type == "VERTEX") ) {
    Console(3) << "Warning: Unsupported shader type \"" << shader_type
               << "\" in \"" << getName() << "\". Must be either \"FRAGMENT\"" 
               << " or \"VERTEX\"." << endl;
    return;
  }

  if( !cg_context ) 
    cg_context = cgCreateContext();
  
  CGerror err = cgGetError();
  if (err != CG_NO_ERROR) {
    Console(3) << cgGetErrorString( err ) << endl;
  }

  if( cg_program ) {
    cgDestroyProgram( cg_program );
    cg_program = NULL;
  }

  cg_profile = Shaders::cgProfileFromString( cg_profile_string,
                                             shader_type );

  if( cgGLIsProfileSupported( cg_profile ) ) {
    for( MFString::const_iterator i = url->begin(); 
         i != url->end(); ++i ) {
      // First try to resolve URL to file contents, if that is not supported
      // by the resolvers then fallback to resolve as local filename
      string url_contents= resolveURLAsString( *i );
      bool is_tmp_file= false;
      string resolved_url;
      if ( url_contents == "" ) {
        resolved_url= resolveURLAsFile( *i, &is_tmp_file );
      }

      if( resolved_url != "" || url_contents != "" ) {
        setURLUsed( *i );
        if ( url_contents == "" ) {
          cg_program = cgCreateProgramFromFile( cg_context,
                                                CG_SOURCE, 
                                                resolved_url.c_str(),
                                                cg_profile,
                                                NULL, NULL);
        } else {
          cg_program = cgCreateProgram( cg_context,
                                        CG_SOURCE, 
                                        url_contents.c_str(),
                                        cg_profile,
                                        NULL, NULL);
        }
        err = cgGetError();
        if( err == CG_NO_ERROR ) {
          source_url = *i;
        } else {
          Console(3) << "Warning: Shader program error when compiling source (" 
                     << *i << ") of \"" 
                     << getName() << "\" node." << endl << cgGetErrorString( err ) 
                     << endl;
          const char *last_listing = cgGetLastListing( cg_context );
          if( last_listing ) Console(3) << last_listing  << endl;
        }
        break;
      }
      if( is_tmp_file ) ResourceResolver::releaseTmpFileName( resolved_url ); 
    }

    if( getURLUsed() == "" ) {
      Console(4) << "Warning: None of the urls in ShaderProgram node with url [";
      for( MFString::const_iterator i = url->begin(); 
           i != url->end(); ++i ) {  
        Console(4) << " \"" << *i << "\"";
      }
      Console(4) << "] could be loaded (in " << getName() << ")" << endl;
    }
  } else {
    Console(4) << "Warning: Your graphics card does not support the "
               << "CG shader profile specified (" << cg_profile_string
               << ") in \"" << getName() 
               << "\". Shader will be disabled. " << endl;
  }
  
  cgGLSetManageTextureParameters( cg_context, true );

  if( cg_program ) {
    cgGLLoadProgram( cg_program );
  }
}

/// Destroy the current cg program object. 
void ShaderProgram::destroyCGShaderProgram() {
  if( cg_program ) {
    cgDestroyProgram( cg_program );
    cg_program = 0;
  }
}


void ShaderProgram::setCGProfile( const string &profile ) {
  cg_profile_string = profile;
}

#endif // HAVE_CG









