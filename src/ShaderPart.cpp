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
/// \file ShaderPart.cpp
/// \brief CPP file for ShaderPart.
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/ShaderPart.h>
#include <H3D/ResourceResolver.h>
#include <fstream>
#include <H3D/GlobalSettings.h>


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
  FIELDDB_ELEMENT( ShaderPart, forceReload, INPUT_OUTPUT );
}

namespace {
  const int pre_processor_max_recurse_depth= 32;
  const std::string include_marker= "#pragma h3dapi include";
}

ShaderPart::ShaderPart( Inst< SFNode         > _metadata,
                        Inst< MFString       > _url ,
                        Inst< SFString       > _type,
                        Inst< SFShaderString > _shader_string,
                        Inst< SFBool         > _forceReload):
  X3DNode( _metadata ),
  X3DUrlObject( _url ),
  type( _type ),
  shaderString( _shader_string ),
  forceReload( _forceReload ),
  shader_handle( 0 ),
  debug_options_previous( NULL ) {
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
  forceReload->route( shaderString );
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
    int print_error = 0;
    if( compile_success == GL_FALSE ) print_error = 1;
    else if( printShaderLog() ) print_error = 2;
    if( print_error != 0 ) {
      GLint nr_characters;
      glGetObjectParameterivARB( shader_handle,
        GL_OBJECT_INFO_LOG_LENGTH_ARB,
        &nr_characters );
      if( nr_characters > 1 ) {
        GLcharARB *log = new GLcharARB[nr_characters];
        glGetInfoLogARB( shader_handle,
          nr_characters,
          NULL,
          log );
        if( print_error == 1 ) Console(3) << "Warning: Error w";
        else Console(3) << "Warning: W";
        Console(3) << "hen compiling shader source of \"" 
          << getName() << "\" node (" << url_used 
          << ")." << endl << log << endl;

        if( print_error == 1 ) {
          glDeleteObjectARB( shader_handle );
          shader_handle = 0;
        }
        delete [] log;
      }
    }
    //PROFILE_END();
    return shader_handle;
}

std::string ShaderPart::shaderStringFromURL ( const std::string& shader_url ) {
  // First try to resolve the url to file contents and load via string buffer
  // Otherwise fallback on using temp files
  string url_contents= resolveURLAsString ( shader_url );
  if ( url_contents != "" ) {
    return url_contents;
  }

  bool is_tmp_file;
  string url = resolveURLAsFile( shader_url, &is_tmp_file );
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
      buffer = new char [(unsigned int)length + 1];
      // read data as a block:
      is.read (buffer,length);
      length = is.gcount();
      is.close();
      if( is_tmp_file ) ResourceResolver::releaseTmpFileName( url );
      buffer[length] = '\0';
      string value= string( buffer );
      delete [] buffer;
      //PROFILE_END();
      return value;
    }
    is.close();
    if( is_tmp_file ) ResourceResolver::releaseTmpFileName( url );
  }

  return "";
}

std::string ShaderPart::preProcess ( const std::string& input, const std::string& url, int depth ) {
  // Catch infinite recursion
  if ( depth > pre_processor_max_recurse_depth ) {
    Console(4) << "Warning: ShaderPart: " << getName() << ": Maximum recursion depth reached in pre-processing. Could be recursive include." << endl;
    return input;
  }

  string output;
  stringstream ss ( input );
  string line;
  while ( getline ( ss, line ) ) {

    // #include statements
    size_t i= line.find ( include_marker );
    if ( i != string::npos ) {
      string path= line.substr ( i+include_marker.length() );

      // Strip white space and quotes
      size_t a, b;
      for ( a= 0; a < path.size() && (isspace ( path[a] ) || path[a] == '\"'); ++a );
      for ( b= path.size()-1; b >= 0 && (isspace ( path[b] ) || path[b] == '\"'); --b );
      path= path.substr ( a, b-a+1 );

      // change url resolver base
      string old_base= getURLBase ();
      string base= url.substr( 0, url.find_last_of( "/\\" ) + 1 );

      // if the url is relative this should succeed
      setURLBase ( old_base + base );
      string include_source= shaderStringFromURL ( path );
      if ( include_source == "" ) {
        // if the url is absolute this should succeed
        setURLBase ( base );
        include_source= shaderStringFromURL ( path );
      }

      if ( include_source != "" ) {
        include_source= preProcess ( include_source, path, depth+1 );
        output+= include_source + "\n";
      } else {
        Console(4) << "Warning: ShaderPart: " << getName() << ": Could not include shader file: " << path << " in " << getURLBase() << "." << endl;
      }

      setURLBase ( old_base );
    } else {
      output+= line + "\n";
    }
  }
  return output;
}

bool ShaderPart::isCompiled () {
  return shaderString->isUpToDate();
}

void ShaderPart::SFShaderString::update() {
  //PROFILE_START("shaderpart: update");
  ShaderPart *shader_part = static_cast< ShaderPart * >( getOwner() ); 
  MFString *urls = static_cast< MFString * >( routes_in[0] );
  for( MFString::const_iterator i = urls->begin(); i != urls->end(); ++i ) {
    string include_source= shader_part->shaderStringFromURL ( *i );
    if ( include_source != "" ) {
      include_source= shader_part->preProcess ( include_source, *i );
      value= include_source;
      shader_part->setURLUsed ( *i );
      return;
    }
  }
  Console(4) << "None of the urls in ShaderPart:"<<this->getFullName()<<" with url [";
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

bool ShaderPart::printShaderLog() {
  DebugOptions *debug_options = NULL;
  GlobalSettings *default_settings = GlobalSettings::getActive();
  if( default_settings&&default_settings->optionNodesUpdated() ) {
    default_settings->getOptionNode( debug_options );
    if( debug_options ) {// update debug options
      debug_options_previous = debug_options;
      return debug_options->printShaderWarnings->getValue();
    }else{
      // global setting change in last frame, but no debug options in it now
      debug_options_previous = NULL;
      return false;
    }
  }
  else if( default_settings ) { 
    // global setting option node exist but not updated
    if( debug_options_previous!=NULL ) {
      return debug_options_previous->printShaderWarnings->getValue();
    }else{
      return false;
    }
  }else{
    // no global settings at all now
    debug_options_previous = NULL;
    return false;
  }
}

void ShaderPart::initialize() {
  X3DNode::initialize();
  GlobalSettings *default_settings = GlobalSettings::getActive();
  if( default_settings ) default_settings->getOptionNode( debug_options_previous );
}
