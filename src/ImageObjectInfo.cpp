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
/// \file ImageObjectInfo.cpp
/// \brief CPP file for ImageObjectInfo.
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/ImageObjectInfo.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase ImageObjectInfo::database( 
                                       "ImageObjectInfo", 
                                       &(newInstance<ImageObjectInfo>), 
                                       typeid( ImageObjectInfo ),
                                       &X3DChildNode::database );

namespace ImageObjectInfoInternals {
  FIELDDB_ELEMENT( ImageObjectInfo, texture, INPUT_OUTPUT );
  FIELDDB_ELEMENT( ImageObjectInfo, pixelSize, OUTPUT_ONLY );
  FIELDDB_ELEMENT( ImageObjectInfo, width, OUTPUT_ONLY );
  FIELDDB_ELEMENT( ImageObjectInfo, height, OUTPUT_ONLY );
  FIELDDB_ELEMENT( ImageObjectInfo, depth, OUTPUT_ONLY );
  FIELDDB_ELEMENT( ImageObjectInfo, pixelType, OUTPUT_ONLY );
  FIELDDB_ELEMENT( ImageObjectInfo, pixelComponentType, OUTPUT_ONLY );
  FIELDDB_ELEMENT( ImageObjectInfo, bitsPerPixel, OUTPUT_ONLY );
  FIELDDB_ELEMENT( ImageObjectInfo, totalSize, OUTPUT_ONLY );
}


ImageObjectInfo::ImageObjectInfo( Inst< SFImageObjectNode > _texture,
                                  Inst< SFVec3f           > _pixelSize,
                                  Inst< SFInt32           > _width,
                                  Inst< SFInt32           > _height,
                                  Inst< SFInt32           > _depth,
                                  Inst< SFString          > _pixelType,
                                  Inst< SFString          > _pixelComponentType,
                                  Inst< SFInt32           > _bitsPerPixel,
                                  Inst< SFVec3f           > _totalSize ):
  texture( _texture ),
  pixelSize( _pixelSize ),
  width( _width ),
  height( _height ),
  depth( _depth ),
  pixelType( _pixelType ),
  pixelComponentType( _pixelComponentType ),
  bitsPerPixel( _bitsPerPixel ),
  totalSize( _totalSize ),
  update_fields( new UpdateFields ) {

  type_name = "ImageObjectInfo";
  database.initFields( this );

  // Name set to not have unknown fields in the doxygen documentation.
  update_fields->setName( "update_fields(internal)" );
  update_fields->setOwner( this );

  texture->route( update_fields );

  width->setValue( 0, id );
  height->setValue( 0, id );
  depth->setValue( 0, id );
  pixelType->setValue( "UNKNOWN", id );
  pixelComponentType->setValue( "UNKNOWN", id );
  bitsPerPixel->setValue( 0, id );
}


void ImageObjectInfo::UpdateFields::update() {
  
  ImageObjectInfo * info = static_cast< ImageObjectInfo * >( getOwner() );
  
  H3DImageObject *image_object = 
    static_cast< SFImageObjectNode *>( routes_in[0] )->getValue();

  Image *image = NULL;
  if( image_object ) image = image_object->image->getValue();

  if( image ) {
    const Vec3f &pixel_size = image->pixelSize();
    if( info->pixelSize->getValue() != pixel_size ) {
      info->pixelSize->setValue( pixel_size, info->id );
    }

    if( info->width->getValue() != image->width() ) {
      info->width->setValue( image->width(), info->id );
    }

    if( info->height->getValue() != image->height() ) {
      info->height->setValue( image->height(), info->id );
    }

    if( info->depth->getValue() != image->depth() ) {
      info->depth->setValue( image->depth(), info->id );
    }

    if( info->bitsPerPixel->getValue() != image->bitsPerPixel() ) {
      info->bitsPerPixel->setValue( image->bitsPerPixel(), info->id );
    }

    Image::PixelType pixel_type = image->pixelType();
    if( pixel_type == Image::LUMINANCE ) {
      info->pixelType->setValue( "LUMINANCE", info->id );
    } else  if( pixel_type == Image::LUMINANCE_ALPHA ) {
      info->pixelType->setValue( "LUMINANCE_ALPHA", info->id );
    } else  if( pixel_type == Image::RGB ) {
      info->pixelType->setValue( "RGB", info->id );
    } else  if( pixel_type == Image::RGBA ) {
      info->pixelType->setValue( "RGBA", info->id );
    } else  if( pixel_type == Image::BGR ) {
      info->pixelType->setValue( "BGR", info->id );
    } else  if( pixel_type == Image::BGRA ) {
      info->pixelType->setValue( "BGRA", info->id );
    } else if( pixel_type == Image::VEC3 ) {
      info->pixelType->setValue( "VEC3", info->id );
    } else {
      info->pixelType->setValue( "UNKNOWN", info->id );
    }

    Image::PixelComponentType component_type = image->pixelComponentType();
    if( component_type == Image::SIGNED ) {
      info->pixelComponentType->setValue( "SIGNED", info->id );
    } else if( component_type == Image::UNSIGNED ) {
      info->pixelComponentType->setValue( "UNSIGNED" , info->id);
    } else if( component_type == Image::RATIONAL ) {
      info->pixelComponentType->setValue( "RATIONAL", info->id );
    } else {
      info->pixelComponentType->setValue( "UNKNOWN", info->id );
    }

    info->totalSize->setValue( Vec3f( image->width() * pixel_size.x, 
                                      image->height() * pixel_size.y, 
                                      image->depth() * pixel_size.z ), info->id ); 
  } else {
    info->pixelSize->setValue( Vec3f(), info->id );
    info->width->setValue( 0, info->id );
    info->height->setValue( 0, info->id );
    info->depth->setValue( 0, info->id );
    info->bitsPerPixel->setValue( 0, info->id );
    info->pixelType->setValue( "UNKNOWN", info->id );
    info->pixelComponentType->setValue( "UNKNOWN", info->id );
    info->totalSize->setValue( Vec3f(), info->id ); 
  }
}
