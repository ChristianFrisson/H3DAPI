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
  
  H3DImageObject *image_object = dynamic_cast< H3DImageObject * >(
    static_cast< SFImageObjectNode *>( routes_in[0] )->getValue() );

  Image *image = NULL;
  if( image_object ) image = image_object->image->getValue();
  info->updateFieldsFromImage( image );
}

void ImageObjectInfo::SFImageObjectNode::onAdd( Node * n ) {
  SFImageObjectNodeBase::onAdd(n);
  ImageObjectInfo * info = static_cast< ImageObjectInfo * >( getOwner() );
  H3DImageObject *image_object = dynamic_cast< H3DImageObject * >(n);

  Image *image = NULL;
  if( image_object ) image = image_object->image->getValue();
  info->updateFieldsFromImage( image );
}

void ImageObjectInfo::updateFieldsFromImage( Image * image ) {
  if( image ) {
    const Vec3f &pixel_size = image->pixelSize();
    if( pixelSize->getValue() != pixel_size ) {
      pixelSize->setValue( pixel_size, id );
    }

    if( width->getValue() != image->width() ) {
      width->setValue( image->width(), id );
    }

    if( height->getValue() != image->height() ) {
      height->setValue( image->height(), id );
    }

    if( depth->getValue() != image->depth() ) {
      depth->setValue( image->depth(), id );
    }

    if( bitsPerPixel->getValue() != image->bitsPerPixel() ) {
      bitsPerPixel->setValue( image->bitsPerPixel(), id );
    }

    Image::PixelType pixel_type = image->pixelType();
    string new_pixel_type_value;
    if( pixel_type == Image::LUMINANCE ) {
      new_pixel_type_value = "LUMINANCE";
    } else  if( pixel_type == Image::LUMINANCE_ALPHA ) {
      new_pixel_type_value = "LUMINANCE_ALPHA";
    } else  if( pixel_type == Image::RGB ) {
      new_pixel_type_value = "RGB";
    } else  if( pixel_type == Image::RGBA ) {
      new_pixel_type_value = "RGBA";
    } else  if( pixel_type == Image::BGR ) {
      new_pixel_type_value = "BGR";
    } else  if( pixel_type == Image::BGRA ) {
      new_pixel_type_value = "BGRA";
    } else if( pixel_type == Image::VEC3 ) {
      new_pixel_type_value = "VEC3";
    } else {
      new_pixel_type_value = "UNKNOWN";
    }

    if( pixelType->getValue() != new_pixel_type_value )
      pixelType->setValue( new_pixel_type_value, id );

    Image::PixelComponentType component_type = image->pixelComponentType();
    string new_component_type;
    if( component_type == Image::SIGNED ) {
      new_component_type = "SIGNED";
    } else if( component_type == Image::UNSIGNED ) {
      new_component_type = "UNSIGNED";
    } else if( component_type == Image::RATIONAL ) {
      new_component_type = "RATIONAL";
    } else {
      new_component_type = "UNKNOWN";
    }

    if( pixelComponentType->getValue() != new_component_type )
      pixelComponentType->setValue( new_component_type, id );

    totalSize->setValue( Vec3f( image->width() * pixel_size.x, 
                                      image->height() * pixel_size.y, 
                                      image->depth() * pixel_size.z ), id ); 
  } else {
    if( pixelSize->getValue() != Vec3f() )
      pixelSize->setValue( Vec3f(), id );
    if( width->getValue() != 0 )
      width->setValue( 0, id );
    if( height->getValue() != 0 )
      height->setValue( 0, id );
    if( depth->getValue() != 0 )
      depth->setValue( 0, id );
    if( bitsPerPixel->getValue() != 0 )
      bitsPerPixel->setValue( 0, id );
    string default_string = "UNKNOWN";
    if( pixelType->getValue() != default_string )
      pixelType->setValue( "UNKNOWN", id );
    if( pixelComponentType->getValue() != default_string )
      pixelComponentType->setValue( "UNKNOWN", id );
    if( totalSize->getValue() != Vec3f() )
      totalSize->setValue( Vec3f(), id ); 
  }
}
