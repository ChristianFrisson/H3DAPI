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
/// \file ShaderImage3D.cpp
/// \brief CPP file for ShaderImage3D.
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/ShaderImage3D.h>
using namespace H3D;

H3DNodeDatabase ShaderImage3D::database ( "ShaderImage3D",
  &(newInstance<ShaderImage3D>),
  typeid(ShaderImage3D),
  &ShaderImageNode::database );

map<string, GLenum> ShaderImage3D::stringImageFormat_map = ShaderImage3D::initStringImageFormat_map ( );
const vector<string> ShaderImage3D::image_formats = ShaderImage3D::initImage_formats ( );

namespace ShaderImage3DInternals{
  FIELDDB_ELEMENT ( ShaderImage3D, width, INPUT_OUTPUT );
  FIELDDB_ELEMENT ( ShaderImage3D, height, INPUT_OUTPUT );
  FIELDDB_ELEMENT ( ShaderImage3D, depth, INPUT_OUTPUT );
  FIELDDB_ELEMENT ( ShaderImage3D, format, INPUT_OUTPUT );
  };

ShaderImage3D::ShaderImage3D( Inst< DisplayList  > _displayList ,
                              Inst< SFNode       > _metadata ,
                              Inst< SFInt32      > _width ,
                              Inst< SFInt32      > _height ,
                              Inst< SFInt32      > _depth ,
                              Inst< SFString     > _format ) :
  ShaderImageNode(_displayList,_metadata),
  width(_width),
  height(_height),
  depth(_depth),
  format(_format){
  type_name = "ShaderImage3D";
  database.initFields ( this );
  displayList->setName( "displayList" );
  displayList->setOwner( this );

  width->setValue ( 512 );
  height->setValue ( 512 );
  depth->setValue ( 8 );

  format->addValidValues ( image_formats.begin ( ), image_formats.end ( ) );
  
  format->setValue ( "GL_RGBA16F" );

  width->route ( displayList );
  height->route ( displayList );
  depth->route ( displayList );
  format->route ( displayList );

  width->route( reinitShaderImage );
  height->route( reinitShaderImage );
  depth->route( reinitShaderImage );
  format->route( reinitShaderImage );
}


void ShaderImage3D::render ( ){
#ifdef GLEW_ARB_shader_image_load_store
  if ( texture_id == 0|| (!reinitShaderImage->isUpToDate()) )
  {// either the first render invocation or parameter for the image needs update
    reinitShaderImage->upToDate();
    prepareShaderImage ( );
  }
  // bind all the layers to the image unit so it can be accessed in the shader
  // keep the format the same as the texture internal format, or there will be internal converting 
  // the format defined when binding the texture layer with image unit is what defined the manager it will accessed in glsl shader
  // This type requires the image uniform variable in shader to have a coresponding format qualifer
  // image_unit will be the reference that shader used to access this image
  string format_s = format->getValue ( );
  GLenum format_t = stringImageFormat_map[format_s];
  glActiveTexture ( texture_unit );
  // set up barrier before actually access the image from shaders
  glMemoryBarrier ( GL_SHADER_IMAGE_ACCESS_BARRIER_BIT | GL_ATOMIC_COUNTER_BARRIER_BIT
#ifdef GL_ARB_shader_storage_buffer_object
                   | GL_SHADER_STORAGE_BARRIER_BIT
#endif
      );
  glBindImageTextureEXT ( image_unit, texture_id, 0, true, 0, GL_READ_WRITE, format_t );

#endif
}

void ShaderImage3D::prepareShaderImage ( ){
#ifdef GLEW_ARB_shader_image_load_store
  if( !texture_id ) {
    // generate texture if do not have valid one
    glGenTextures(1,&texture_id);
  }
  glBindTexture ( GL_TEXTURE_2D_ARRAY, texture_id );
  // set filter, for texture image, GL_NEAREST is required
  glTexParameteri ( GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
  glTexParameteri ( GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

  // configure the texture image, for shader image load and or store,
  // the last three parameter is used to define how data is organized,
  // as we do not actually specify any data for texture so the data format 
  // and data type are not important as long as it does not cause gl error.
  glTexImage3D ( GL_TEXTURE_2D_ARRAY, 0, stringImageFormat_map[format->getValue()], 
                  width->getValue ( ), height->getValue ( ), depth->getValue ( ), 0, GL_RGBA, GL_FLOAT, 0 );
#endif
}

std::map<string, GLenum> ShaderImage3D::initStringImageFormat_map ( ){
  map<string, GLenum> m;
  m["GL_RGBA32F"] = GL_RGBA32F; m["GL_RGBA16F"] = GL_RGBA16F;
  m["GL_R32F"] = GL_R32F; m["GL_R16F"] = GL_R16F;
  m["GL_RGBA32UI"] = GL_RGBA32UI; m["GL_RGBA16UI"] = GL_RGBA16UI; m["GL_RGBA8UI"] = GL_RGBA8UI;
  m["GL_R32UI"] = GL_R32UI; m["GL_R16UI"] = GL_R16UI; m["GL_R8UI"] = GL_R8UI;
  m["GL_RGBA32I"] = GL_RGBA32I; m["GL_RGBA16I"] = GL_RGBA16I; m["GL_RGBA8I"] = GL_RGBA8I;
  m["GL_R32I"] = GL_R32I; m["GL_R16I"] = GL_R16I; m["GL_R8I"] = GL_R8I;
  return m;
}

std::vector<string> ShaderImage3D::initImage_formats ( ){
  string vs[16] = { "GL_RGBA32F", "GL_RGBA16F", "GL_R32F", "GL_R16F",
    "GL_RGBA32UI", "GL_RGBA16UI", "GL_RGBA8UI", "GL_R32UI", "GL_R16UI", "GL_R8UI"
    "GL_RGBA32I", "GL_RGBA16I", "GL_RGBA8I", "GL_R32I", "GL_R16I", "GL_R8I" };
  vector<string> v ( &vs[0], &vs[0] + 16 );
  return v;
}


