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
/// \file ShaderPart.cpp
/// \brief CPP file for ShaderPart.
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/ShaderPart.h>
#include "H3D/ResourceResolver.h"
#include <fstream>


using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase ShaderPart::database( 
                                   "ShaderPart", 
                                   &(newInstance<ShaderPart>), 
                                   typeid( ShaderPart ),
                                   &X3DNode::database );

namespace ShaderPartInternals {
  FIELDDB_ELEMENT( ShaderPart, url, INPUT_OUTPUT );
  FIELDDB_ELEMENT( ShaderPart, type, INITIALIZE_ONLY );
}

ShaderPart::ShaderPart( Inst< SFNode         > _metadata,
                        Inst< MFString       > _url ,
                        Inst< SFString       > _type,
                        Inst< SFShaderString > _shader_string ):
  X3DNode( _metadata ),
  X3DUrlObject( _url ),
  type( _type ),
  shaderString( _shader_string ),
  shader_handle( 0 ) {
  type_name = "ShaderPart";
  database.initFields( this );

  shaderString->setOwner( this );
  shaderString->setName( "shaderString" );

  addInlinePrefix( "glsl" );

  type->addValidValue( "VERTEX" );
  type->addValidValue( "FRAGMENT" );
  type->addValidValue( "GEOMETRY" );
  type->addValidValue( "TESS_CONTROL" );
  type->addValidValue( "TESS_EVALUATION" );
  type->setValue( "VERTEX", id );
  url->route( shaderString );
}


GLhandleARB ShaderPart::compileShader() {
  if( shaderString->isUpToDate() ) {
    return shader_handle;
  } else {
    return compileShaderPart();
  }
}

GLhandleARB ShaderPart::compileShaderPart(){

    //PROFILE_START("shaderpart: compile");
    if( shader_handle ) {
      glDeleteObjectARB( shader_handle );
      shader_handle = 0;
    }

    const string &s = shaderString->getValue();
    if( s == "" ) return 0;

    const string &shader_type = type->getValue();
    if( shader_type == "FRAGMENT" ) {
      shader_handle = glCreateShaderObjectARB( GL_FRAGMENT_SHADER_ARB );
    } else if( shader_type == "VERTEX" ) {
      shader_handle = glCreateShaderObjectARB( GL_VERTEX_SHADER_ARB );
    } else if( shader_type == "GEOMETRY" ) {
      if( !GLEW_EXT_geometry_shader4 ) {
        Console(4) << "Warning: Geometry shaders not supported by your graphics card. ShaderPart with type \"GEOMETRY\" will be ignored." << endl;
        return 0;
      }
      shader_handle = glCreateShaderObjectARB( GL_GEOMETRY_SHADER_EXT );
    }  else if( shader_type == "TESS_CONTROL" ) {
      if( !GLEW_ARB_tessellation_shader ) {
        Console(4) << "Warning: Tesselation shaders not supported by your graphics card. ShaderPart with type \"TESS_CONTROL\" will be ignored." << endl;
        return 0;
      }
      shader_handle = glCreateShaderObjectARB( GL_TESS_CONTROL_SHADER );
    }  else if( shader_type == "TESS_EVALUATION" ) {
      if( !GLEW_ARB_tessellation_shader ) {
        Console(4) << "Warning: Tesselation shaders not supported by your graphics card. ShaderPart with type \"TESS_EVALUATION\" will be ignored." << endl;
        return 0;
      }
      shader_handle = glCreateShaderObjectARB( GL_TESS_EVALUATION_SHADER );
    } else {
      shader_handle = 0;
      Console(3) << "Warning: Unsupported shader type \"" << shader_type
        << "\" in ShaderPart node. Must be either \"FRAGMENT\"," 
        << "\"VERTEX\", \"GEOMETRY\", \"TESS_CONTROL\" "
        << " or \"TESS_EVALUATION." << endl;
      return shader_handle;
    }

    const char * shader_string = s.c_str();
    glShaderSourceARB( shader_handle, 1, &shader_string, NULL );
    glCompileShaderARB( shader_handle );

    GLint compile_success;
    glGetObjectParameterivARB( shader_handle,
      GL_OBJECT_COMPILE_STATUS_ARB,
      &compile_success );
    if( compile_success == GL_FALSE ) {
      GLint nr_characters;
      glGetObjectParameterivARB( shader_handle,
        GL_OBJECT_INFO_LOG_LENGTH_ARB,
        &nr_characters );
      GLcharARB *log = new GLcharARB[nr_characters];
      glGetInfoLogARB( shader_handle,
        nr_characters,
        NULL,
        log );
      Console(3) << "Warning: Error when compiling shader source of \"" 
        << getName() << "\" node (" << url_used 
        << ")." << endl << log << endl;

      glDeleteObjectARB( shader_handle );
      delete [] log;
      shader_handle = 0;
    }
    //PROFILE_END();
    return shader_handle;
}

bool ShaderPart::isCompiled () {
  return shaderString->isUpToDate();
}

void ShaderPart::SFShaderString::update() {
  //PROFILE_START("shaderpart: update");
  ShaderPart *shader_part = static_cast< ShaderPart * >( getOwner() ); 
  MFString *urls = static_cast< MFString * >( routes_in[0] );
  for( MFString::const_iterator i = urls->begin(); i != urls->end(); ++i ) {
    // First try to resolve the url to file contents and load via string buffer
    // Otherwise fallback on using temp files
    string url_contents= ResourceResolver::resolveURLAsString ( *i );
    if ( url_contents != "" ) {
      shader_part->setURLUsed( *i );
      value= url_contents;
      return;
    }

    bool is_tmp_file;
    string url = shader_part->resolveURLAsFile( *i, &is_tmp_file );
    if( url != "" ) {
      ifstream is( url.c_str() );
      if( is.good() ) {
        std::streamsize length;
        char * buffer;
        
        // get length of file:
        is.seekg (0, ios::end);
        length = is.tellg();
        is.seekg (0, ios::beg);
        
      // allocate memory:
        buffer = new char [length + 1];
        // read data as a block:
        is.read (buffer,length);
        length = is.gcount();
        is.close();
        if( is_tmp_file ) ResourceResolver::releaseTmpFileName( url );
        buffer[length] = '\0';
        shader_part->setURLUsed( *i );
        value = string( buffer );
        delete [] buffer;
        //PROFILE_END();
        return;
      }
      is.close();
      if( is_tmp_file ) ResourceResolver::releaseTmpFileName( url );
     }
  }
  Console(4) << "None of the urls in ShaderPart with url [";
  for( MFString::const_iterator i = urls->begin(); i != urls->end(); ++i ) {  
    Console(4) << " \"" << *i << "\"";
  }
  Console(4) << "] could be loaded." << endl;
  shader_part->setURLUsed( "" );
  value = "";
  //PROFILE_END();
}

X3DUrlObject::LoadStatus ShaderPart::loadStatus() {
  if( url_used != "" ) return X3DUrlObject::LOADED;
  else return X3DUrlObject::FAILED;
}
