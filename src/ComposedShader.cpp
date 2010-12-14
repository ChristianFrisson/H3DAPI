//////////////////////////////////////////////////////////////////////////////
//    Copyright 2004-2010, SenseGraphics AB
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

#include <H3D/ShaderFunctions.h>
#include <H3D/ComposedShader.h>
#include <H3D/X3DTexture2DNode.h>
#include <H3D/X3DTexture3DNode.h>

#include<sstream>
#include<algorithm>

using namespace H3D;
using namespace std;

map<string, GLhandleARB> ComposedShader::phandles_map;
map<GLhandleARB, int> ComposedShader::phandle_counts;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase ComposedShader::database( 
                                   "ComposedShader", 
                                   &(newInstance<ComposedShader>), 
                                   typeid( ComposedShader ),
                                   &X3DShaderNode::database );

namespace ComposedShaderInternals {
  FIELDDB_ELEMENT( ComposedShader, parts, INPUT_OUTPUT );
  FIELDDB_ELEMENT( ComposedShader, suppressUniformWarnings, INPUT_OUTPUT );
  FIELDDB_ELEMENT( ComposedShader, geometryInputType, INPUT_OUTPUT );
  FIELDDB_ELEMENT( ComposedShader, geometryOutputType, INPUT_OUTPUT );
  FIELDDB_ELEMENT( ComposedShader, geometryVerticesOut, INPUT_OUTPUT );
}

ComposedShader::ComposedShader( Inst< DisplayList  > _displayList,
                                Inst< SFNode       > _metadata,
                                Inst< SFBool       > _isSelected ,
                                Inst< SFBool       > _isValid,
                                Inst< SFBool       > _activate,
                                Inst< SFString     > _language,
                                Inst< MFShaderPart > _parts,
                                Inst< SFBool       > _suppressUniformWarnings,
                                Inst< SFString     > _geometryInputType,
                                Inst< SFString     > _geometryOutputType,
                                Inst< SFInt32      > _geometryVerticesOut) :
  X3DShaderNode( _displayList, _metadata, _isSelected, 
                 _isValid, _activate, _language),
  X3DProgrammableShaderObject( &database ),
  parts( _parts ),
  suppressUniformWarnings( _suppressUniformWarnings ),
  geometryInputType( _geometryInputType ),
  geometryOutputType( _geometryOutputType ),
  geometryVerticesOut( _geometryVerticesOut ),
  program_handle( 0 ),
  setupDynamicRoutes( new SetupDynamicRoutes ) {
  type_name = "ComposedShader";
  database.initFields( this );

  suppressUniformWarnings->setValue( false );

  geometryInputType->addValidValue( "POINTS");
  geometryInputType->addValidValue( "LINES");
  geometryInputType->addValidValue( "TRIANGLES");
  geometryInputType->addValidValue( "TRIANGLES_ADJACENCY");
  geometryInputType->addValidValue( "LINES_ADJACENCY");
  geometryInputType->setValue( "TRIANGLES" );

  geometryOutputType->setValue( "POINTS" );
  geometryOutputType->setValue( "LINE_STRIP" );
  geometryOutputType->setValue( "TRIANGLE_STRIP" );

  geometryOutputType->setValue( "TRIANGLE_STRIP" );

  geometryVerticesOut->setValue( 64 );

  setupDynamicRoutes->setName( "setupDynamicRoutes" );
  setupDynamicRoutes->setOwner( this );

  activate->route( displayList, id );
  parts->route( displayList, id );
  setupDynamicRoutes->route( displayList );
}

bool ComposedShader::shader_support_checked = false;

bool ComposedShader::addField( const string &name, 
                               const Field::AccessType &access, Field *field ){
  bool success = X3DProgrammableShaderObject::addField( name, access, field  );
  if( success ) {
    SFNode * sf_node_field = dynamic_cast< SFNode * >( field );
    MFNode * mf_node_field = dynamic_cast< MFNode * >( field );
    if( sf_node_field || mf_node_field ) {
      field->route( setupDynamicRoutes, id );
    } else {
      field->route( displayList );
    }
  }
  return success;
}

void ComposedShader::preRender() {
  if( GLEW_ARB_shader_objects ) {
    glUseProgramObjectARB( program_handle );
    Shaders::preRenderTextures( this );
    X3DShaderNode::preRender();
  }

  glPushAttrib( GL_COLOR_BUFFER_BIT );
  glEnable( GL_BLEND );
  glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void ComposedShader::postRender() {
  glPopAttrib();

  if( GLEW_ARB_shader_objects ) {
    glUseProgramObjectARB( 0 );
    Shaders::postRenderTextures( this );
    X3DShaderNode::postRender();
  }
}

// The ComposedShader is modified to use 1 instance of different program_handlers
// that share the same set of shader parts. However, a new program_handle object 
// will be created when the Shader is re-activated (regardsless)


void ComposedShader::render() {
  if( !GLEW_ARB_shader_objects ) {
    if( !shader_support_checked ) {
      Console(4) << "Your graphic card driver does not support shader objects( ARB_shader_objects) so you cannot"
                 << " use the ComposedShader node. Shader will be disabled" << endl;
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
    
    if( all_parts_valid )
    {
      // if the first time we run the shader and there is some part attached
      if ( !program_handle && parts->size() ) {
        std::string key = ComposedShader::genKeyFromShader( this );

        if (phandles_map.find(key) != phandles_map.end()) {
          // if a handle found, use that!
          program_handle = phandles_map[key];
          phandle_counts[program_handle]++;
          //std::cout<< getName() << " use program handle " << program_handle
          // << std::endl;
          glUseProgramObjectARB( program_handle );
        } else {
          // if not, create one, link to shaderparts
          GLhandleARB h = createHandle( this );
          if (h != 0) {
            // use that handle
            program_handle = h;
            glUseProgramObjectARB( h );
            phandle_counts[h] = 1;
            std::string key = genKeyFromShader( this );
            phandles_map[key] = h;
            // register shader objects
            for ( MFShaderPart::const_iterator i = parts->begin();
                  i != parts->end(); i++ ) {
              current_shaders.push_back(
                static_cast< ShaderPart * >(*i)->getShaderHandle() );
            }
          }
        }
      }

      // if a TRUE event has been sent to the activate field we 
      // relink the program (without looking up)
      else if( displayList->hasCausedEvent( activate ) &&
                  activate->getValue( id ) )
      {
        // deallocate old instance if not used anywhere
        if (phandle_counts.find(program_handle) != phandle_counts.end()) {
          phandle_counts[program_handle]--;
          if (phandle_counts[program_handle] == 0) {
            // detach the old shaders from the program
            for( vector< GLhandleARB >::iterator i = current_shaders.begin();
              i != current_shaders.end(); i++ ) {
              glDetachObjectARB( program_handle, *i );
            }
            current_shaders.clear();
            // delete object
            //std::cout<< this->getName() << " remove phandle "
            //         << program_handle << std::endl;
            glDeleteObjectARB( program_handle );
            phandle_counts.erase( program_handle );
          }
        } else {
          // if not, this is a floating program handle. delete it anyway
          //std::cout<< this->getName() << " remove phandle " << program_handle
          //         << std::endl;
          glDeleteObjectARB( program_handle );
        }

        // we can't use the old instance (because that forces other
        // shaders to re-link)

        GLhandleARB h = createHandle(this);
        if (h != 0) {
          program_handle = h;
          glUseProgramObjectARB( h );
          // register shader objects
          for ( MFShaderPart::const_iterator i = parts->begin();
                i != parts->end(); i++ ) {
            current_shaders.push_back(
              static_cast< ShaderPart * >(*i)->getShaderHandle() );
          }
        }
      }
    }

    if( program_handle ) {
      Shaders::renderTextures( this );

      for( unsigned int i = 0; i < dynamic_fields.size(); i++ ) {
        if( !Shaders::setGLSLUniformVariableValue( program_handle, 
                                                   dynamic_fields[i] ) &&
            !suppressUniformWarnings->getValue() ) {
          Console(3) << "Warning: Uniform variable \"" << dynamic_fields[i]->getName() 
                     << "\" not defined in shader source or field is of unsupported field type of the ShaderPart nodes "
                     << "in the node \"" << getName() << "\"" << endl;
        }
      }
    }
  }
}

// check if any existing program handle using the same set of ShaderParts
// so that we can reuse the program handle
// Return the handle if found, zero if not found
std::string ComposedShader::genKeyFromShader(ComposedShader* shader)
{
  // build the key as string. we use simple key building method 
  // by making a string of all the shaderParts' handles combined.
  vector<GLhandleARB> keys;
  keys.reserve( shader->parts->size() );
  for( MFShaderPart::const_iterator i = shader->parts->begin();
       i != shader->parts->end(); i++ ) {
    GLhandleARB handle = static_cast< ShaderPart * >(*i)->getShaderHandle();
    keys.push_back( handle );
  }
  sort( keys.begin(), keys.end() );
  stringstream ss;
  for( unsigned int i = 0; i < keys.size(); i++ ) ss << keys[i] << "_";
  return ss.str();
}


// create handle and link to shaderparts. return 0 if failed.
// Preclusion: parts > 0
GLhandleARB ComposedShader::createHandle(ComposedShader* shader) {
  if ( !shader->parts->size() )
    return 0;

  GLhandleARB program_handle = glCreateProgramObjectARB();

  // add the shaders to program
  for ( MFShaderPart::const_iterator i = shader->parts->begin();
        i != shader->parts->end(); i++ ) {
    GLhandleARB handle = static_cast< ShaderPart * >(*i)->getShaderHandle();
    glAttachObjectARB( program_handle, handle );
  }

  // set geometry shader values 
  shader->setGeometryShaderParameters( program_handle );

  // link shader program
  glLinkProgramARB( program_handle );
  GLint link_success;
  glGetObjectParameterivARB( program_handle, GL_OBJECT_LINK_STATUS_ARB,
                             &link_success );
  if( link_success == GL_FALSE ) {
    // linking failed, print error message
    GLint nr_characters;
    glGetObjectParameterivARB( program_handle, GL_OBJECT_INFO_LOG_LENGTH_ARB,
                               &nr_characters );
    GLcharARB *log = new GLcharARB[nr_characters];
    glGetInfoLogARB( program_handle, nr_characters, NULL, log );
    Console(3) << "Warning: Error while linking shader parts in \""
               << const_cast<ComposedShader&>(*shader).getName() << "\" node. "
               << endl << log << endl;
    glDeleteObjectARB( program_handle );
    delete log;

    return 0;
  }

  //std::cout<< const_cast<ComposedShader&>(*shader).getName()
  //  << " created program handle " << program_handle << std::endl;


  return program_handle;
}


void ComposedShader::setGeometryShaderParameters( GLenum program_handle ) {
  if( GLEW_EXT_geometry_shader4 ) {
    // Setting input type.
    const string &input_type = geometryInputType->getValue();
    if( input_type == "POINTS" ) {
      glProgramParameteriEXT(program_handle, 
                             GL_GEOMETRY_INPUT_TYPE_EXT, GL_POINTS);
    } else if( input_type == "LINES" ) {
      glProgramParameteriEXT(program_handle, 
                             GL_GEOMETRY_INPUT_TYPE_EXT,GL_LINES);
    } else if( input_type == "TRIANGLES" ) {
      glProgramParameteriEXT(program_handle, 
                             GL_GEOMETRY_INPUT_TYPE_EXT, GL_TRIANGLES);
    } else if( input_type == "LINES_ADJACENCY" ) {
      glProgramParameteriEXT(program_handle, 
                             GL_GEOMETRY_INPUT_TYPE_EXT,
                             GL_LINES_ADJACENCY_EXT );
    } else if( input_type == "TRIANGLES_ADJACENCY" ) {
      glProgramParameteriEXT(program_handle, 
                             GL_GEOMETRY_INPUT_TYPE_EXT, 
                             GL_TRIANGLES_ADJACENCY_EXT );
    } else {
      Console(4) << "Invalid geometryInputType \"" << input_type
                 << "\" in ComposedShader. Using \"TRIANGLES\" instead." << endl;
      glProgramParameteriEXT(program_handle, 
                             GL_GEOMETRY_INPUT_TYPE_EXT, GL_TRIANGLES);
    }

    // Setting output type.
    const string &output_type = geometryOutputType->getValue();
    if( output_type == "POINTS" ) {
      glProgramParameteriEXT(program_handle, 
                             GL_GEOMETRY_OUTPUT_TYPE_EXT, GL_POINTS);
    } else if( output_type == "LINE_STRIP" ) {
      glProgramParameteriEXT(program_handle, 
                             GL_GEOMETRY_OUTPUT_TYPE_EXT, GL_LINE_STRIP );
    } else if( output_type == "TRIANGLE_STRIP" ) {
      glProgramParameteriEXT(program_handle, 
                             GL_GEOMETRY_OUTPUT_TYPE_EXT, 
                             GL_TRIANGLE_STRIP );
    } else {
      Console(4) << "Invalid geometryOutputType \"" << output_type
                 << "\" in ComposedShader. Using \"TRIANGLE_STRIP\" instead."
                 << endl;
      glProgramParameteriEXT(program_handle, 
                             GL_GEOMETRY_OUTPUT_TYPE_EXT, GL_TRIANGLES);
    } 

    int max_output_vertices;
     glGetIntegerv(GL_MAX_GEOMETRY_OUTPUT_VERTICES_EXT, &max_output_vertices);

    int nr_output_vertices = geometryVerticesOut->getValue();

    if( nr_output_vertices > max_output_vertices ) {
      Console( 4 ) << "Invalid geomtryVerticesOut value " << nr_output_vertices
                   << " in ComposedShader. Hardware supports a maximum of " 
                   << max_output_vertices << "." << endl;
      nr_output_vertices = max_output_vertices;
    }

    // number of output vertices for geometry shader.
    glProgramParameteriEXT(program_handle,
                           GL_GEOMETRY_VERTICES_OUT_EXT,
                           nr_output_vertices );
    
  }
}

void ComposedShader::SetupDynamicRoutes::update() {
  AutoUpdate< Field >::update();
  ComposedShader * cs = static_cast< ComposedShader * >( getOwner() );
  SFNode * sf_node_field = dynamic_cast< SFNode * >( event.ptr );
  MFNode * mf_node_field = dynamic_cast< MFNode * >( event.ptr );

  map< Field *, NodeVector >::iterator in_map =
    fields_to_nodes.find( event.ptr );

  // Start by removing the entry in fields_to_nodes map. Since it might be
  // that the node added is not the same as the previous node, and it might
  // also be so that the new node does not inherit from H3DDisplayListObject.
  if( in_map != fields_to_nodes.end() ) {
    const NodeVector &node_vector = (*in_map).second;
    for( unsigned int i = 0; i < node_vector.size(); i++ ) {
      H3DDisplayListObject *hdln =
        dynamic_cast< H3DDisplayListObject * >( node_vector[i] );
      if( hdln )
        hdln->displayList->unroute( cs->displayList );
    }
    fields_to_nodes.erase( in_map );
  }

  if( sf_node_field ) {
    // Setup route for the node contained in sf_node_field.
    // Add entry to map to remove later.
    Node * n = sf_node_field->getValue();
    H3DDisplayListObject *hdln =
      dynamic_cast< H3DDisplayListObject * >( n );
    if( hdln ) {
      hdln->displayList->route( cs->displayList, cs->id );
      NodeVector tmp_node_vector;
      tmp_node_vector.push_back( n );
      fields_to_nodes[ event.ptr ] = tmp_node_vector;
    }
  } else {
    // Setup routes for all nodes contained in mf_node_field.
    // Add entry to map to remove later.
    const NodeVector &node_vector = mf_node_field->getValue();
    NodeVector tmp_node_vector;
    for( unsigned int i = 0; i < node_vector.size(); i++ ) {
      H3DDisplayListObject *hdln =
        dynamic_cast< H3DDisplayListObject * >( node_vector[i] );
      if( hdln ) {
        hdln->displayList->route( cs->displayList, cs->id );
        tmp_node_vector.push_back( node_vector[i] );
      }
    }
    if( !tmp_node_vector.empty() )
      fields_to_nodes[ event.ptr ] = tmp_node_vector;
  }
}

