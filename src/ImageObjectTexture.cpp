//////////////////////////////////////////////////////////////////////////////
//    Copyright 2004, SenseGraphics AB
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
/// \file ImageObjectTexture.cpp
/// \brief .cpp for ImageObjectTexture.
///
//
//////////////////////////////////////////////////////////////////////////////

#include "ImageObjectTexture.h"

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase ImageObjectTexture::database
    ( "ImageObjectTexture", 
      &(newInstance<ImageObjectTexture>), 
      typeid( ImageObjectTexture ),
      &X3DTexture3DNode::database );

namespace ImageObjectTextureInternals {
  FIELDDB_ELEMENT( ImageObjectTexture, imageObject, INPUT_OUTPUT );
}

/// Constructor.
ImageObjectTexture::ImageObjectTexture( Inst< DisplayList > _displayList,
                                        Inst< SFNode  > _metadata,
                                        Inst< SFBool  > _repeatS,
                                        Inst< SFBool  > _repeatT,
                                        Inst< SFBool  > _repeatR,
                                        Inst< SFBool  > _scaleToP2,
                                        Inst< SFImage > _image,
                                        Inst< SFBool  > _interpolate, 
                                        Inst< SFImageObject > _imageObject ): 
  X3DTexture3DNode( _displayList, _metadata, _repeatS, _repeatT, _repeatR,
                    _scaleToP2, _image, _interpolate ),
  imageObject( _imageObject ) {

  type_name = "ImageObjectTexture";
  database.initFields( this );
}
    
void ImageObjectTexture::SFImageObject::onAdd( H3D::Node *n ) {
  TypedSFNodeObject< H3DImageObject>::onAdd( n );
  H3DImageObject *image_object = dynamic_cast< H3DImageObject * >( n );
  if( image_object ) {
    ImageObjectTexture *iot = 
      static_cast< ImageObjectTexture * >( getOwner() );
    image_object->image->route( iot->image );
  }
}

void ImageObjectTexture::SFImageObject::onRemove( H3D::Node *n ) {
  H3DImageObject *image_object = dynamic_cast< H3DImageObject * >( n );
  if( image_object ) {
    ImageObjectTexture *iot = 
      static_cast< ImageObjectTexture * >( getOwner() );
    image_object->image->unroute( iot->image );
  }
  TypedSFNodeObject< H3DImageObject>::onRemove( n );
}
