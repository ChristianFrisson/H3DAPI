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
/// \file PackagedShader.cpp
/// \brief CPP file for PackagedShader.
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/PackagedShader.h>
#include <H3D/X3DTexture2DNode.h>
#include <H3D/X3DTexture3DNode.h>
#include <H3D/ShaderFunctions.h>
#include "H3D/ResourceResolver.h"

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase PackagedShader::database( 
                                   "PackagedShader", 
                                   &(newInstance<PackagedShader>), 
                                   typeid( PackagedShader ),
                                   &X3DShaderNode::database );

namespace PackagedShaderInternals {
  FIELDDB_ELEMENT( PackagedShader, url, INPUT_OUTPUT );
}

PackagedShader::PackagedShader( Inst< DisplayList  > _displayList,
                                Inst< SFNode       > _metadata,
                                Inst< SFBool       > _isSelected ,
                                Inst< SFBool       > _isValid,
                                Inst< SFBool       > _activate,
                                Inst< SFString     > _language,
                                Inst< MFString     > _url ) :
  X3DShaderNode( _displayList, _metadata, _isSelected, 
                 _isValid, _activate, _language ),
  X3DUrlObject( _url ),
  X3DProgrammableShaderObject( &database )
#ifdef HAVE_CG
  ,cg_context( 0 ),
  cg_vertex_program( 0 ),
  cg_fragment_program( 0 )
#endif
{
  type_name = "PackagedShader";
  database.initFields( this );

  addInlinePrefix("cg");

  activate->route( displayList, id );
}


#ifdef HAVE_CG

bool PackagedShader::addField( const string &name,
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

void PackagedShader::preRender() {
  if( !cg_vertex_program && !cg_fragment_program ) {
    initCGShaderProgram();
  }

  if( cg_vertex_program && cg_fragment_program ) {
    Shaders::preRenderTextures( this );
    Shaders::renderTextures( this );
    cgGLBindProgram( cg_vertex_program );
    cgGLBindProgram( cg_fragment_program );
    cgGLEnableProfile( cg_vertex_profile );
    cgGLEnableProfile( cg_fragment_profile );
  }
  X3DShaderNode::preRender();
}


void PackagedShader::postRender() {
  if( cg_vertex_program && cg_fragment_program ) {
    cgGLDisableProfile( cg_vertex_profile );
    cgGLDisableProfile( cg_fragment_profile );
    Shaders::postRenderTextures( this );
  }
  X3DShaderNode::postRender();
}

void PackagedShader::render() {
  if( !cg_vertex_program || !cg_fragment_program ) {
    initCGShaderProgram();
  }

  if( cg_vertex_program && cg_fragment_program ) {
    for( unsigned int i = 0; i < dynamic_fields.size(); ++i ) {
      if( !Shaders::setCGUniformVariableValue( cg_vertex_program, 
                                               dynamic_fields[i] ) ) {
        Console(3) << "Warning: Uniform variable \"" 
                   <<  dynamic_fields[i]->getName() 
                   << "\" not defined in shader source (" << source_url << ")" 
                   << " of the node \"" << getName() << "\"" << endl;
      }
      if( !Shaders::setCGUniformVariableValue( cg_fragment_program, 
                                               dynamic_fields[i] ) ) {
        Console(3) << "Warning: Uniform variable \"" 
                   <<  dynamic_fields[i]->getName() 
                   << "\" not defined in shader source (" << source_url << ")" 
                   << " of the node \"" << getName() << "\"" << endl;
      }
    }
  }
}


void PackagedShader::initCGShaderProgram() {
  setURLUsed( "" );
  if( !cg_context ) 
    cg_context = cgCreateContext();
  
  CGerror err = cgGetError();
  if (err != CG_NO_ERROR) {
    Console(3) << cgGetErrorString( err ) << endl;
  }

  if( cg_vertex_program ) {
    cgDestroyProgram( cg_vertex_program );
    cg_vertex_program = NULL;
  }

  if( cg_fragment_program ) {
    cgDestroyProgram( cg_fragment_program );
    cg_fragment_program = NULL;
  }

  cg_vertex_profile = Shaders::cgProfileFromString( language->getValue(),
                                                   "VERTEX" );
  cg_fragment_profile = Shaders::cgProfileFromString( language->getValue(),
                                                     "FRAGMENT" );

  if( cgGLIsProfileSupported( cg_vertex_profile ) &&
      cgGLIsProfileSupported( cg_fragment_profile )) {
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
          cg_vertex_program = cgCreateProgramFromFile( cg_context,
                                                       CG_SOURCE, 
                                                       resolved_url.c_str(),
                                                       cg_vertex_profile,
                                                       "vert_main", NULL);
        } else {
          cg_vertex_program = cgCreateProgram( cg_context,
                                               CG_SOURCE, 
                                               url_contents.c_str(),
                                               cg_vertex_profile,
                                               "vert_main", NULL);
        }
        err = cgGetError();
        if( err != CG_NO_ERROR ) {
          Console(3) << "Warning: Shader program error when compiling vertex "
                     << "shader source (" << *i << ") in \"" 
                     << getName() << "\" node." << endl 
                     << cgGetErrorString( err ) 
                     << endl;
          const char *last_listing = cgGetLastListing( cg_context );
          if( last_listing ) Console(3) << last_listing  << endl;
        }

        if ( url_contents == "" ) {
          cg_fragment_program = cgCreateProgramFromFile( cg_context,
                                                         CG_SOURCE, 
                                                         resolved_url.c_str(),
                                                         cg_fragment_profile,
                                                         "frag_main", NULL);
        } else {
          cg_fragment_program = cgCreateProgram( cg_context,
                                                 CG_SOURCE, 
                                                 url_contents.c_str(),
                                                 cg_fragment_profile,
                                                 "frag_main", NULL);
        }
        if( is_tmp_file ) 
          ResourceResolver::releaseTmpFileName( resolved_url );
        err = cgGetError();
        if( err != CG_NO_ERROR ) {
          Console(3) << "Warning: Shader program error when compiling fragment "
                     << "shader source (" << *i << ") in \"" 
                     << getName() << "\" node." << endl 
                     << cgGetErrorString( err ) 
                     << endl;
          const char *last_listing = cgGetLastListing( cg_context );
          if( last_listing ) Console(3) << last_listing  << endl;
          if( cg_vertex_program ) {
            cgDestroyProgram( cg_vertex_program );
            cg_vertex_program = NULL;
          }
        } else {
          source_url = *i;
        }
        break;
      }
    }
    if( getURLUsed() == "" ) {
      Console(4) << "Warning: None of the urls in PackagedShader node with url [";
      for( MFString::const_iterator i = url->begin(); 
           i != url->end(); ++i ) {  
        Console(4) << " \"" << *i << "\"";
      }
      Console(4) << "] could be loaded (in " << getName() << ")" << endl;
    }
  } else {
    Console(4) << "Warning: Your graphics card does not support the "
               << "CG shader profile specified (" << language->getValue()
               << ") in \"" << getName() << "\". Shader will be disabled. " << endl;
  }
  
  cgGLSetManageTextureParameters( cg_context, true );
  
  if( cg_vertex_program && cg_fragment_program ) {
    cgGLLoadProgram( cg_vertex_program );
    cgGLLoadProgram( cg_fragment_program );
  }
}

#endif // HAVE_CG
