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
/// \file ComposedShader.cpp
/// \brief CPP file for ComposedShader.
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/ComposedShader.h>
#include <H3D/X3DTexture2DNode.h>
#include <H3D/X3DTexture3DNode.h>
#include <H3D/ShaderAtomicCounter.h>

#include<fstream>
#include<sstream>
#include<algorithm>

using namespace H3D;
using namespace std;

bool ComposedShader::tessellation_support_checked= false;

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
  FIELDDB_ELEMENT( ComposedShader, transparencyDetectMode, INPUT_OUTPUT );
  FIELDDB_ELEMENT( ComposedShader, transformFeedbackVaryings, INPUT_OUTPUT );
#ifdef EXPORT_SHADER
  FIELDDB_ELEMENT( ComposedShader, saveShadersToUrl, INPUT_OUTPUT );
#endif
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
                                Inst< SFInt32      > _geometryVerticesOut,
                                Inst< SFString     > _transparencyDetectMode,
                                Inst< MFString     > _transformFeedbackVaryings
#ifdef EXPORT_SHADER
                                ,
                                Inst< UpdateSaveShadersToUrl > _saveShadersToUrl
#endif
                                ) :
  X3DShaderNode( _displayList, _metadata, _isSelected, 
                 _isValid, _activate, _language),
  X3DProgrammableShaderObject( &database ),
  parts( _parts ),
  suppressUniformWarnings( _suppressUniformWarnings ),
  geometryInputType( _geometryInputType ),
  geometryOutputType( _geometryOutputType ),
  geometryVerticesOut( _geometryVerticesOut ),
  transparencyDetectMode( _transparencyDetectMode ),
  transformFeedbackVaryings ( _transformFeedbackVaryings ),
#ifdef EXPORT_SHADER
  saveShadersToUrl( _saveShadersToUrl ),
#endif
  program_handle( 0 ),
  setupDynamicRoutes( new SetupDynamicRoutes ),
  updateUniforms ( new UpdateUniforms ) {
  type_name = "ComposedShader";
  database.initFields( this );

  suppressUniformWarnings->setValue( false );

  geometryInputType->addValidValue( "POINTS");
  geometryInputType->addValidValue( "LINES");
  geometryInputType->addValidValue( "TRIANGLES");
  geometryInputType->addValidValue( "TRIANGLES_ADJACENCY");
  geometryInputType->addValidValue( "LINES_ADJACENCY");
  geometryInputType->setValue( "TRIANGLES" );

  geometryOutputType->addValidValue( "POINTS" );
  geometryOutputType->addValidValue( "LINE_STRIP" );
  geometryOutputType->addValidValue( "TRIANGLE_STRIP" );

  geometryOutputType->setValue( "TRIANGLE_STRIP" );

  geometryVerticesOut->setValue( 64 );

  transparencyDetectMode->addValidValue( "AS_MATERIAL" );
  transparencyDetectMode->addValidValue( "TRANSPARENT" );
  transparencyDetectMode->addValidValue( "OPAQUE" );
  transparencyDetectMode->setValue( "AS_MATERIAL" );

  setupDynamicRoutes->setName( "setupDynamicRoutes" );
  setupDynamicRoutes->setOwner( this );

  updateUniforms->setName ( "updateUniforms" );
  updateUniforms->setOwner ( this );

  activate->route( displayList, id );
  parts->route( displayList, id );
  setupDynamicRoutes->route( displayList );
  transformFeedbackVaryings->route ( displayList, id );

  // need to update uniform values if shader is re-linked
  // displayList->route ( updateUniforms );
  activate->route( updateUniforms, id );
}

bool ComposedShader::shader_support_checked = false;

bool ComposedShader::isTransparent( X3DMaterialNode *material ) {
  const string &mode = transparencyDetectMode->getValue();
  if( mode == "TRANSPARENT" ) {
    return true;
  } else if( mode == "OPAQUE" ) {
    return false;
  } else {
    if( mode != "AS_MATERIAL") {
      Console(4) << "Warning: Invalid transparencyDetectMode \"" << mode << "\" in ComposedShader."
     << " Must be one of \"AS_MATERIAL\", \"TRANSPARENT\" or \"OPAQUE\"." << endl;
    }
    if( material ) return material->isTransparent();
  }
  return false;
}

bool ComposedShader::addField( const string &name, 
                               const Field::AccessType &access, Field *field ){
  // composed shader added field can be node field such as texture
  // or just field such as SFFloat, SFBool
  // as field name should be the same as they are in shaders, name will be used 
  // as the id for field.
  // For shader program, the name in dynamic field will match the field in shader
  
  if( !field || uniformFields.find( name )!=uniformFields.end() ) {
    // different shader part may add the same field with same name
    // as uniformFields is a global dataset for the shader program
      return false;
    }
  // the field being added have a unique name never being used before in this shader
  bool success = X3DProgrammableShaderObject::addField( name, access, field  );
  if( success ) {
    // insert a new entry for the newly added field
    
    H3D::Shaders::UniformInfo ui = { field, 0 };
    uniformFields.insert( std::pair< string, H3D::Shaders::UniformInfo >( name , ui ) );
    // if inserted node is a SFNode, or MFNode, need to handle the actul node inside
    // the field specially
    SFNode * sf_node_field = dynamic_cast< SFNode * >( field );
    MFNode * mf_node_field = dynamic_cast< MFNode * >( field );
    if( sf_node_field || mf_node_field ) {
      field->route( setupDynamicRoutes, id );
    } else {
      field->route( displayList );
    }
    field->route( updateUniforms );
  }else{
    // can not add same field twice,even though it has different name.
    // This is the limit of h3d dynamic field object. 
    Console(4)<<"Warning: Failed to add field: "<<name<<" to shader "<< getName()
      <<", either current node is invalid or the added field is already in the node database!"
      <<endl;
  }
  return success;
}

GLbitfield ComposedShader::getAffectedGLAttribs() {
  GLbitfield res = X3DShaderNode::getAffectedGLAttribs();
  if( GLEW_ARB_shader_objects ) {
    res = res | Shaders::getAffectedGLAttribs( this );
  }
  res = res | GL_COLOR_BUFFER_BIT;
  return res;
}

void ComposedShader::preRender() {
  if( GLEW_ARB_shader_objects ) {
    glUseProgramObjectARB( program_handle );
    Shaders::preRenderTextures( this );
    Shaders::preRenderShaderResources( this, program_handle );
    X3DShaderNode::preRender();
  }

  glEnable( GL_BLEND );
  glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void ComposedShader::postRender() {
  if( GLEW_ARB_shader_objects ) {
    glUseProgramObjectARB( 0 );
    Shaders::postRenderTextures( this );
    X3DShaderNode::postRender();
  }
}

void ComposedShader::traverseSG ( TraverseInfo& ti ) {
  X3DShaderNode::traverseSG ( ti );
  Node* n;
  for( H3DDynamicFieldsObject::field_iterator f = this->firstField();f != this->endField(); ++f ) 
  {
    X3DTypes::X3DType x3d_type = (*f)->getX3DType();
    if(x3d_type==X3DTypes::SFNODE){
      n = static_cast<SFNode*>(*f)->getValue();
      if( ShaderAtomicCounter* sac = dynamic_cast<ShaderAtomicCounter*>(n) ) {
        sac->traverseSG(ti);
      }
    }
  }

  // Will be set to true if tessellation shader is present
  static bool require_patches; 

  // Look for tessellation shader
  require_patches = false;
  for( MFShaderPart::const_iterator i = parts->begin();
        i != parts->end(); ++i ) {
    ShaderPart* shader_part= static_cast< ShaderPart * >(*i);
    if ( shader_part->type->getValue() == "TESS_CONTROL" || shader_part->type->getValue() == "TESS_EVALUATION" ) {
      require_patches= true;
      break;
    }
  }

  // Let the geometry know  if it should render patches
  if( require_patches ) {
    if ( GLEW_ARB_tessellation_shader ) {
      ti.setUserData( "shaderRequiresPatches", &require_patches );
    } else if ( !tessellation_support_checked ) {
      Console(4) << "Your graphic card driver does not support tessellation shaders."
                 << " The ComposedShader node " << getName() << " will not be rendered correctly!" << endl;
      tessellation_support_checked = true;
    }
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
    bool re_link= false;
    for( MFShaderPart::const_iterator i = parts->begin();
         i != parts->end(); ++i ) {
      ShaderPart* s= static_cast< ShaderPart * >(*i);
      re_link|= !s->isCompiled();
      if( s->compileShader() == 0 ) {
        all_parts_valid = false;
      }
    }

    if ( re_link ) {
      activate->setValue ( true );
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
          ++(phandle_counts[program_handle]);
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
            phandles_map[key] = h;
            // register shader objects
            for ( MFShaderPart::const_iterator i = parts->begin();
                  i != parts->end(); ++i ) {
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
          --(phandle_counts[program_handle]);
          if (phandle_counts[program_handle] == 0) {
            // detach the old shaders from the program
            for( vector< GLhandleARB >::iterator i = current_shaders.begin();
              i != current_shaders.end(); ++i ) {
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
                i != parts->end(); ++i ) {
            current_shaders.push_back(
              static_cast< ShaderPart * >(*i)->getShaderHandle() );
          }
        }
      }
    }

    if( program_handle ) {
      Shaders::renderTextures( this );
      Shaders::renderShaderResources( this );
//#ifdef HAVE_PROFILER
//      H3DTimer::stepBegin("updateUniform");
//#endif
      // Lazily update uniform values, i.e., only those that have changed
      updateUniforms->upToDate();
//#ifdef HAVE_PROFILER
//      H3DTimer::stepEnd("updateUniform");
//#endif
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
       i != shader->parts->end(); ++i ) {
    GLhandleARB handle = static_cast< ShaderPart * >(*i)->getShaderHandle();
    keys.push_back( handle );
  }
  sort( keys.begin(), keys.end() );
  stringstream ss;
  for( unsigned int i = 0; i < keys.size(); ++i ) ss << keys[i] << "_";
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
        i != shader->parts->end(); ++i ) {
    GLhandleARB handle = static_cast< ShaderPart * >(*i)->getShaderHandle();
    glAttachObjectARB( program_handle, handle );
  }

  // set geometry shader values 
  shader->setGeometryShaderParameters( program_handle );

  // Add transform feedback varyings before program is linked
  if ( !shader->transformFeedbackVaryings->empty() ) {
    std::vector<const GLchar*> varyings ( shader->transformFeedbackVaryings->size() );
    for ( size_t i= 0; i < shader->transformFeedbackVaryings->size(); ++i ) {
      varyings[i]= shader->transformFeedbackVaryings->getValueByIndex ( i ).c_str();
    }
    glTransformFeedbackVaryings(program_handle, varyings.size(), &varyings[0], GL_INTERLEAVED_ATTRIBS);
  }

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
    delete [] log;

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
                             GL_GEOMETRY_INPUT_TYPE_EXT, 4/*GL_TRIANGLES*/);
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
                             GL_GEOMETRY_OUTPUT_TYPE_EXT, 4/*GL_TRIANGLES*/);
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
    for( unsigned int i = 0; i < node_vector.size(); ++i ) {
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
      cs->activate->setValue( true );
      NodeVector tmp_node_vector;
      tmp_node_vector.push_back( n );
      fields_to_nodes[ event.ptr ] = tmp_node_vector;
    }
  } else if( mf_node_field ) {
    // Setup routes for all nodes contained in mf_node_field.
    // Add entry to map to remove later.
    const NodeVector &node_vector = mf_node_field->getValue();
    NodeVector tmp_node_vector;
    for( unsigned int i = 0; i < node_vector.size(); ++i ) {
      H3DDisplayListObject *hdln =
        dynamic_cast< H3DDisplayListObject * >( node_vector[i] );
      if( hdln ) {
        hdln->displayList->route( cs->displayList, cs->id );
        cs->activate->setValue( true );
        tmp_node_vector.push_back( node_vector[i] );
      }
    }
    if( !tmp_node_vector.empty() )
      fields_to_nodes[ event.ptr ] = tmp_node_vector;
  }
}

void ComposedShader::UpdateUniforms::update() {
  ComposedShader* node= static_cast<ComposedShader*>(getOwner());
  bool update_all= hasCausedEvent ( node->activate );
  if( update_all ) { // program re-linked, need to update all uniform
    // update the uniform location information in unifromFields
    //Console(4)<<"program relinked!!!"<<endl;
    std::map< string, H3D::Shaders::UniformInfo >::iterator it;
    for( it = node->uniformFields.begin(); it!= node->uniformFields.end(); ++it  ) {
      const string &name = it->first;
      GLint location = glGetUniformLocationARB( node->program_handle,
        name.c_str() );
      it->second.location = location;
      if( !Shaders::setGLSLUniformVariableValue( node->program_handle, it->second.field, &(it->second), true /* force update */ ) 
        && !node->suppressUniformWarnings->getValue() ) {
        Console(4) << "Warning: Uniform variable \"" << it->first
          << "\" not defined in shader source or field is of unsupported field type of the ShaderPart nodes "
          << "in the node \"" << node->getName() << "\"" << endl;
      }
    }
    // all fields are updated, clear the event to finish the update and return.
    EventCollectingField < Field >::update();
    return;
  }
  
  // no need to update all, check field one by one to update the one needs to be updated
  std::map< string, H3D::Shaders::UniformInfo >::iterator it;
  for( it = node->uniformFields.begin(); it!= node->uniformFields.end(); ++it ) {
    Field* current_field = it->second.field;
    if( hasCausedEvent( current_field ) ) {// current_field update since last time
      //if( current_field->getTypeName()=="SFUniform" ) {
      //  // this is a SFUniform value, check if its value actually changed
      current_field->upToDate();
      // within setGLSLUniformVariableValue, check if the updated value
      // is the same as before to decide whether to reload uniform value to GPU
      if( !Shaders::setGLSLUniformVariableValue( node->program_handle, 
        it->second.field, &it->second ) &&
        !node->suppressUniformWarnings->getValue() ) {
          Console(4) << "Warning: Uniform variable \"" << it->first
            << "\" not defined in shader source or field is of unsupported field type of the ShaderPart nodes "
            << "in the node \"" << node->getName() << "\"" << endl;
      }
    }
    // It is not the current_field caused the update event, skip uniform update
  }
  EventCollectingField < Field >::update();
}

#ifdef EXPORT_SHADER

void ComposedShader::UpdateSaveShadersToUrl::onNewValue( const std::string &v ){

  // when url string changed, re export the shaders
  if ( v.empty ( ) ) return;
  ComposedShader* cs = static_cast< ComposedShader* >( getOwner() );
  GLenum error = glGetError();
  vector< GLhandleARB > handlers = cs->current_shaders;
  GLchar* shader_content;
  for( vector< GLhandleARB >::iterator it = handlers.begin(); it != handlers.end(); it++ ) {
    GLint shader_type;
    GLsizei shader_length;
    glGetShaderiv ( *it, GL_SHADER_SOURCE_LENGTH , &shader_length);

    
    glGetShaderiv( *it, GL_SHADER_TYPE, &shader_type );
    shader_content = new GLchar [shader_length+1];
    glGetShaderSource ( *it, shader_length+1, NULL, shader_content );
    // if
    error = glGetError();
    if( error!=GL_NO_ERROR ) {
      Console(4)<<" Warning: extract shader information error: "<<gluErrorString(error)<<endl;
      continue;
    }
    //glewGetString( shader_type );
    if( shader_type==GL_VERTEX_SHADER_ARB ) {
      ofstream   outFile( v+"_vertex_shader.txt", ofstream::out  );
      outFile<< shader_content <<endl;
      outFile.close();
    }else if( shader_type==GL_GEOMETRY_SHADER_ARB ) {
      ofstream   outFile( v+"_geometry_shader.txt", ofstream::out  );
      outFile<< shader_content <<endl;
      outFile.close();
      break;
    }else if( shader_type==GL_TESS_EVALUATION_SHADER ) {
      ofstream   outFile( v+"_tessEva_shader.txt", ofstream::out  );
      outFile<< shader_content <<endl;
      outFile.close();
    }else if( shader_type==GL_TESS_CONTROL_SHADER ) {
      ofstream   outFile( v+"_tessControl_shader.txt", ofstream::out  );
      outFile<< shader_content <<endl;
      outFile.close();
    }else if( shader_type==GL_FRAGMENT_SHADER_ARB ) {
      ofstream   outFile( v+"_fragment_shader.txt", ofstream::out  );
      outFile<< shader_content <<endl;
      outFile.close();
    }else{
      Console(4)<<"shader type unsupported yet"<<endl;
    }
  }
  //Console(4)<<"will output shader uniform"<<endl;
  ofstream   outFile( v+"_shader_uniform.txt", ofstream::out  );
  //Console(4)<<"shader uniform txt opened."<<endl;
  int total = -1;
  GLhandleARB program_id = cs->getProgramHandle();

  glGetProgramiv( program_id , GL_ACTIVE_UNIFORMS, &total ); 
  outFile<<"current active uniform values:"<<endl;
  for(int i=0; i<total; ++i)  {
    int name_len=-1, num=-1;
    GLenum type = GL_ZERO;
    char name[256];
    glGetActiveUniform( program_id, GLuint(i), sizeof(name)-1,
      &name_len, &num, &type, name );
    name[name_len] = 0;
    GLuint location = glGetUniformLocation( program_id, name );
    float uniform_value[50];
    glGetUniformfvARB( program_id, location, uniform_value  );
    outFile<< name <<" : " ;
    switch ( type )
    {
    case GL_FLOAT:
    case GL_INT:
    case GL_BOOL:
    case GL_DOUBLE:
      outFile<<uniform_value[0] <<endl;
      break;
    case GL_FLOAT_VEC3:
    case GL_BOOL_VEC3:
    case GL_INT_VEC3:
    case GL_DOUBLE_VEC3_EXT:
      outFile<<uniform_value[0] <<" , " << uniform_value[1] <<
      " , " << uniform_value[2]<< endl;
      break;
    default:
      outFile<<"unsupported uniform type!"<<endl;
    }
  }
  outFile.close();
}
#endif