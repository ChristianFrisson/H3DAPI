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
/// \file ImageTexture.cpp
/// \brief CPP file for ImageTexture, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include "ImageTexture.h"

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase ImageTexture::database( "ImageTexture", 
                                        &(newInstance<ImageTexture>), 
                                        typeid( ImageTexture ),
                                        &X3DTexture2DNode::database );

namespace ImageTextureInternals {
  FIELDDB_ELEMENT( ImageTexture, url, INPUT_OUTPUT );
  FIELDDB_ELEMENT( ImageTexture, imageLoader, INPUT_OUTPUT );
}


ImageTexture::ImageTexture( 
                           Inst< DisplayList > _displayList,
                           Inst< SFNode        > _metadata,
                           Inst< MFString      > _url,
                           Inst< SFBool        > _repeatS,
                           Inst< SFBool        > _repeatT,
                           Inst< SFBool        > _scaleToP2,
                           Inst< SFImage       > _image,
                           Inst< SFBool   > _interpolate,
                           Inst< MFImageLoader > _imageLoader ) :
  X3DTexture2DNode( _displayList, _metadata, _repeatS, _repeatT,
                    _scaleToP2, _image, _interpolate ),
  X3DUrlObject( _url ),
  imageLoader( _imageLoader ) {

  type_name = "ImageTexture";
  database.initFields( this );

  url->route( image );
  imageLoader->route( image );
}

void ImageTexture::SFImage::update() {
  ImageTexture *texture = static_cast< ImageTexture * >( getOwner() );
  MFImageLoader *image_loaders = static_cast< MFImageLoader * >( routes_in[1] );
  MFString *urls = static_cast< MFString * >( routes_in[0] );

  if( image_loaders->size() ) { 
    for( MFString::const_iterator i = urls->begin(); i != urls->end(); ++i ) {
      for( MFImageLoader::const_iterator il = image_loaders->begin();
           il != image_loaders->end();
           il++ ) {
        string url = texture->resolveURLAsFile( *i );
        Image *image = 
          static_cast< H3DImageLoaderNode * >(*il)->loadImage( url );
        if( image ) {
          value = image;
          texture->setURLUsed( *i );
          return;
        }
      }
    }
  }

  for( MFString::const_iterator i = urls->begin(); i != urls->end(); ++i ) {
    string url = texture->resolveURLAsFile( *i );
	  H3DImageLoaderNode *il = H3DImageLoaderNode::getSupportedFileReader( url );
    if( il ) {
      value = il->loadImage( url );
      texture->setURLUsed( *i );
      return;
    }
  }

  cerr << "Warning: None of the urls in ImageTexture with url [";
  for( MFString::const_iterator i = urls->begin(); i != urls->end(); ++i ) {  
    cerr << " \"" << *i << "\"";
  }
  cerr << "] could be loaded. Either they don't exist or the file format "
       << "is not supported by any H3DImageLoaderNode that is available "
       << "(in " << getOwner()->getName() << ")" << endl;

  texture->setURLUsed( "" );
  value = NULL;
}

void ImageTexture::render() {
  if( url->size() > 0 ) {
    try {
      X3DTexture2DNode::render();
    } catch( InvalidTextureDimensions &e ) {
      stringstream s;
      s << e.message << " with url [" ;
      for( MFString::const_iterator i = url->begin(); i != url->end(); ++i ) { 
        s << " \"" << *i << "\"";
      }
      s << "]." << ends;
      e.message = s.str();
      throw e;
    } catch( OpenGLTextureError &e ) {
      stringstream s;
      s << e.message << " with url [" ;
      for( MFString::const_iterator i = url->begin(); i != url->end(); ++i ) { 
        s << " \"" << *i << "\"";
      }
      s << "]." << ends;
      e.message = s.str();
      throw e;
    }
  } else {
    disableTexturing();
  }
}


