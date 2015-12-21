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
/// \file ShaderImage2D.cpp
/// \brief CPP file for ShaderImage2D.
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/ShaderImage2D.h>

using namespace H3D;

H3DNodeDatabase ShaderImage2D::database ( "ShaderImage2D",
  &(newInstance<ShaderImage2D>),
  typeid(ShaderImage2D),
  &(ShaderImageNode::database) );

map<string, GLenum> ShaderImage2D::stringImageFormat_map = ShaderImage2D::initStringImageFormat_map ( );
const vector<string> ShaderImage2D::image_formats = ShaderImage2D::initImage_formats ( );
map<string, string> ShaderImage2D::imageFormatQualifier_map = ShaderImage2D::initImageFormatQualifier_map();

namespace ShaderImage2DInternals{
  FIELDDB_ELEMENT ( ShaderImage2D, width,  INPUT_OUTPUT );
  FIELDDB_ELEMENT ( ShaderImage2D, height, INPUT_OUTPUT );
  FIELDDB_ELEMENT ( ShaderImage2D, format, INPUT_OUTPUT );
}

ShaderImage2D::ShaderImage2D( 
                                Inst< DisplayList  > _displayList,
                                Inst< SFNode       > _metadata,
                                Inst< SFInt32      > _width ,
                                Inst< SFInt32      > _height,
                                Inst< SFString     > _format) :
  ShaderImageNode(_displayList,_metadata),
  width(_width),
  height(_height),
  format(_format){
  type_name = "ShaderImage2D";
  database.initFields ( this );
  displayList->setName( "displayList" );
  displayList->setOwner( this );

  

  width->setValue ( 512 );
  height->setValue ( 512 );
  format->addValidValues ( image_formats.begin ( ), image_formats.end ( ) );
  format->setValue ( "RGBA16F" );

  width->route ( displayList );
  height->route ( displayList );
  format->route ( displayList );

  width->route( reinitShaderImage );
  height->route( reinitShaderImage );
  format->route( reinitShaderImage );
}



void ShaderImage2D::render ( ){
  // before calling render, make sure that glActiveTexture is called 
  // or setTextureUnit is called , so texture_unit is the correct one to be used
  // also make sure setImageUnit is called for the similar reason
  glGetIntegerv(GL_ACTIVE_TEXTURE_ARB, &texture_unit);
#ifdef GLEW_ARB_shader_image_load_store
  if( texture_id == 0 || !reinitShaderImage->isUpToDate() )
  {// either the first render invocation or parameter for the image needs update
    reinitShaderImage->upToDate();
    prepareShaderImage ( );
  }
  // For now, always set memory barrier here so image read/write is coherent
  // to make sure the write to shader image is finish and can be accessed later through read
  // this is not always necessary of course and can take time for opengl to sync.
  // TODO: add dedicated node to set up memory barrier by the user, so can be selectively
  // set only when necessary, or add parameter for this node to decide whether
  // set barrier or not
  glMemoryBarrier ( GL_SHADER_IMAGE_ACCESS_BARRIER_BIT|GL_ATOMIC_COUNTER_BARRIER_BIT
#ifdef GL_ARB_shader_storage_buffer_object
      |GL_SHADER_STORAGE_BARRIER_BIT
#endif
      );
  // by default, bind the level zero of the texture to the image unit we generated
  // currently , this is hard coded, maybe can be exposed as parameter later 
  //Console(LogLevel::Info) << "texture id i  dfs: " << texture_id << std::endl;
  //Console(LogLevel::Info) << "image unit is: " << image_unit << std::endl;
  //Console(LogLevel::Info) << "texture unit is: " << texture_unit << std::endl;
  glBindImageTextureEXT ( image_unit, texture_id, 0, false, 0, 
    GL_READ_WRITE, stringImageFormat_map[format->getValue ( )] );
#endif
}

void ShaderImage2D::prepareShaderImage ( ){
#ifdef GLEW_ARB_shader_image_load_store
  if ( !texture_id ) {
    // texture for shader image is zero, need to create a new one
    glGenTextures ( 1, &texture_id );
  }
  // bind to specified texture unit 
  glActiveTexture ( texture_unit );
  glBindTexture ( GL_TEXTURE_2D, texture_id );

  // texture creation , as no pixel data will be assigned, the pixel type and format 
  // does not need to be correct
  glTexImage2D ( GL_TEXTURE_2D, 0, stringImageFormat_map[format->getValue ( )], 
    width->getValue ( ), height->getValue ( ), 0, GL_RED_INTEGER, GL_UNSIGNED_INT, 0 );
  // set filter
  glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
  glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
  glBindTexture( GL_TEXTURE_2D, 0 );
#endif
}

std::map<string, GLenum> ShaderImage2D::initStringImageFormat_map ( ){
  map<string, GLenum> m;
  // there is a bug currently, GL_R32UI seems not correctly working
  // so use GL_R32F  as temp workaround, need to try test and fix later 
  m["GL_RGBA32F"] = GL_RGBA32F; m["GL_RGBA16F"] = GL_RGBA16F; 
  m["GL_R32F"] = GL_R32F; m["GL_R16F"] = GL_R16F; m["GL_RG16F"] = GL_RG16F;
  m["GL_RGBA32UI"] = GL_RGBA32UI; m["GL_RGBA16UI"] = GL_RGBA16UI; m["GL_RGBA8UI"] = GL_RGBA8UI;
  m["GL_R32UI"] = GL_R32UI; m["GL_R16UI"] = GL_R16UI; m["GL_R8UI"] = GL_R8UI;
  m["GL_RGBA32I"] = GL_RGBA32I; m["GL_RGBA16I"] = GL_RGBA16I; m["GL_RGBA8I"] = GL_RGBA8I;
  m["GL_R32I"] = GL_R32I; m["GL_R16I"] = GL_R16I; m["GL_R8I"] = GL_R8I;
  return m;
}


std::vector<string> ShaderImage2D::initImage_formats ( ){
  string vs[16] = { "GL_RGBA32F", "GL_RGBA16F", "GL_R32F", "GL_R16F", "GL_RG16F",
    "GL_RGBA32UI", "GL_RGBA16UI", "GL_RGBA8UI", "GL_R32UI", "GL_R16UI", "GL_R8UI"
    "GL_RGBA32I", "GL_RGBA16I", "GL_RGBA8I", "GL_R32I", "GL_R16I", "GL_R8I" };
  vector<string> v ( &vs[0], &vs[0] + 16 );
  return v;
}

std::map<string, string> ShaderImage2D::initImageFormatQualifier_map(){
  map<string, string> m;
  m["GL_RGBA32F"] = "rgba32f"; m["GL_RGBA16F"] = "rgba16f"; 
  m["GL_R32F"] = "r32f"; m["GL_R16F"] = "r16f"; m["GL_RG16F"] = "rg16f";
  m["GL_RGBA32UI"] = "rgba32ui"; m["GL_RGBA16UI"] = "rgba16ui"; m["GL_RGBA8UI"] = "rgba8ui";
  m["GL_R32UI"] = "r32ui"; m["GL_R16UI"] = "r16ui"; m["GL_R8UI"] = "r8ui";
  m["GL_RGBA32I"] = "rgba32i"; m["GL_RGBA16I"] = "rgba16i"; m["GL_RGBA8I"] = "rgba8i";
  m["GL_R32I"] = "r32i"; m["GL_R16I"] = "r16i"; m["GL_R8I"] = "r8i";
  return m;
}

