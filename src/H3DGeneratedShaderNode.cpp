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
/// \file H3DGeneratedShaderNode.cpp
/// \brief CPP file for H3DGeneratedShaderNode.
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/H3DGeneratedShaderNode.h>
#include <H3D/ShaderPart.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase H3DGeneratedShaderNode::database( 
                                   "H3DGeneratedShaderNode", 
                                   NULL,
                                   typeid( H3DGeneratedShaderNode ),
                                   &ComposedShader::database );

namespace H3DGeneratedShaderNodeInternals {
  FIELDDB_ELEMENT( H3DGeneratedShaderNode, vertexShaderString, OUTPUT_ONLY );
  FIELDDB_ELEMENT( H3DGeneratedShaderNode, fragmentShaderString, OUTPUT_ONLY );
}

H3DGeneratedShaderNode::H3DGeneratedShaderNode( 
                              Inst< DisplayList  > _displayList,
                              Inst< SFNode       > _metadata,
                              Inst< SFBool       > _isSelected ,
                              Inst< SFBool       > _isValid,
                              Inst< SFBool       > _activate,
                              Inst< SFString     > _language,
                              Inst< MFShaderPart > _parts,
                              Inst< SFBool       > _suppressUniformWarnings,
                              Inst< MFString     > _fragmentShaderString,
                              Inst< MFString     > _vertexShaderString ) :
  ComposedShader( _displayList, _metadata, _isSelected, 
                  _isValid, _activate, _language, _parts, 
                  _suppressUniformWarnings),

  fragmentShaderString( _fragmentShaderString ),
  vertexShaderString( _vertexShaderString ),
  rebuildShader( new RebuildShader ) {
  type_name = "H3DGeneratedShaderNode";
  database.initFields( this );

  rebuildShader->setOwner( this );
  rebuildShader->setName( "rebuildShader" );

  // shader_id is used to have unique variable names in the shaders
  // it is simply the memory address in hex
  H3DPtrUint sid = (H3DPtrUint)(this);
  ostringstream ostr;
  ostr << hex << sid;
  shader_id = ostr.str();

  suppressUniformWarnings->setValue( true );
  language->setValue( "GLSL" );

  ShaderPart *vertex_shader = new ShaderPart;
  vertex_shader->type->setValue( "VERTEX" );

  ShaderPart *fragment_shader = new ShaderPart;
  fragment_shader->type->setValue( "FRAGMENT" );

  parts->push_back( vertex_shader );
  parts->push_back( fragment_shader );

  rebuildShader->route( displayList );

  vertexShaderString->route( vertex_shader->url );
  fragmentShaderString->route( fragment_shader->url );
  
}


string H3DGeneratedShaderNode::uniqueShaderName( const string &base_name ) {
  return base_name + shader_id;
}

string H3DGeneratedShaderNode::getFinalFragmentShaderOutputString() {
  return "  gl_FragColor = generated_color; \n";
}

string H3DGeneratedShaderNode::addUniformFields( ComposedShader *shader ) {
  // add dynamic fields for uniform variables and return string for uniform
  // variable specification.
  return "";
  
}

string H3DGeneratedShaderNode::getVertexShaderString(){
  stringstream vss;
  vss << 
    "  gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;\n"
    "  gl_ClipVertex = gl_ModelViewMatrix * gl_Vertex;\n"
    "  gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;\n";
  return vss.str();

}

void H3DGeneratedShaderNode::buildShader() {

  // clear the uniform field map

  uniformFields.clear();


  // clear the previous fields that 
  clearFields();

  // get the different varying and attribute variables.
  vector< H3DGeneratedShaderNode::VaryingVariable > varying;
  getVaryingVariables( varying );

  vector< H3DGeneratedShaderNode::Attribute > attributes;
  getAttributes( attributes );

  // the glsl functions used by the generator.
  string function_string = getFunctionShaderString();

  // this adds fields to the shader and returns a string with
  // the definition of uniform variables.
  string uniform_string = addUniformFields( this );

  // vertex shader
  stringstream sv;

  for( unsigned int i = 0; i < varying.size(); ++i ) {
    sv << "varying " << varying[i].type << " " << varying[i].name << ";" << endl; 
  }

  for( unsigned int i = 0; i < attributes.size(); ++i ) {
    sv << "attribute " << attributes[i].type << " " << attributes[i].name << ";" << endl; 
  }

  sv << uniform_string << endl;

  sv << function_string << endl;

  sv << "void main() {\n" <<endl;
    
  sv << getVertexShaderString() << endl;

  for( unsigned int i = 0; i < varying.size(); ++i ) {
    sv << "  {" << endl;
    sv << "    " << varying[i].calculation << ";" << endl; 
    sv << "  }" << endl;
  }

  sv << "}" << endl;
  //  Console(4) << sv.str() << endl;
  vertexShaderString->clear( id );
  vertexShaderString->push_back( string("glsl:") + sv.str(), id );

  // fragment shader
  stringstream sf;
  sf << getFragmentShaderHeader() << endl;

  for( unsigned int i = 0; i < varying.size(); ++i ) {
    sf << "varying " << varying[i].type << " " << varying[i].name << ";" << endl;
  }

  sf << uniform_string << endl;

  sf << getFunctionShaderString() << endl;

  sf << "void main() {" << endl;
  sf << "  vec4 generated_color = vec4( 0.0, 0.0, 0.0, 1.0 ); " << endl;
  sf << getFragmentShaderString() << endl;
  sf << getFinalFragmentShaderOutputString() << endl;
  sf << "}" << endl;

  //  Console(4) << sf.str() << endl;
  fragmentShaderString->clear( id );
  fragmentShaderString->push_back( string("glsl:") + sf.str(), id );
  
  // force relink of shader
  activate->setValue( true );
}

void H3DGeneratedShaderNode::preRender() {
  rebuildShader->upToDate();

  // make sure that we have a shader. The upToDate call will have created this in
  // most cases, but in the case of no fields are routed to rebuildShader(in e.g.
  // NormalShader) we need to make sure it is built manually,
  if( fragmentShaderString->size() == 0 ) {
    buildShader();
  }
  ComposedShader::preRender();
}

string H3DGeneratedShaderNode::
addUniformToFragmentShader( ComposedShader *shader,
                            const string &name,
                            const string &glsl_type,
                            const Field::AccessType &access,
                            Field *field,
                            int array_size,
                            bool delete_unadded_field ) {
    
  // if field successfully added, we add corresponding code 
  // to the fragment shader
  bool ok = shader->addField( name, access, field ); 
  if( ok ) {
    stringstream s;
    if( array_size == 0 ) s << "[1]";
    else if( array_size != -1 ) s << "[" << array_size << "]";
    string array_string = s.str();
    
    return "uniform " + glsl_type + " " + name + array_string + ";\n";
  } else {
    if( delete_unadded_field )
      delete field;
    return "";
  }
}
