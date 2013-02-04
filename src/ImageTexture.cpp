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
/// \file ImageTexture.cpp
/// \brief CPP file for ImageTexture, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/ImageTexture.h>
#include <H3D/ResourceResolver.h>
#include <H3D/DicomImageLoader.h>
#include <H3D/GlobalSettings.h>

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
                           Inst< MFImageLoader > _imageLoader,
                           Inst< SFTextureProperties > _textureProperties ) :
  X3DTexture2DNode( _displayList, _metadata, _repeatS, _repeatT,
                    _scaleToP2, _image, _textureProperties ),
  X3DUrlObject( _url ),
  imageLoader( _imageLoader ),
  load_thread( NULL ) {

  type_name = "ImageTexture";
  database.initFields( this );

  url->route( image );
  imageLoader->route( image );
}

Image* ImageTexture::SFImage::loadImage( ImageTexture *texture,
                                         const vector< string > &urls,
                                         const NodeVector &image_loaders ) {
  // First try the image loader nodes specified
  if( image_loaders.size() ) { 
    for( vector<string>::const_iterator i = urls.begin(); 
         i != urls.end(); ++i ) {
      for( NodeVector::const_iterator il = image_loaders.begin();
           il != image_loaders.end();
           il++ ) {
        // First try to resolve the url to file contents and load via string buffer
        // Otherwise fallback on using temp files
        string url_contents= ResourceResolver::resolveURLAsString ( *i );
        if ( url_contents != "" ) {
          Image *image = 
            static_cast< H3DImageLoaderNode * >(*il)->loadImage ( istringstream ( url_contents ) );
          if( image ) {
            texture->setURLUsed( *i );
            return image;
          }
        }

        bool is_tmp_file;
        string url = texture->resolveURLAsFile( *i, &is_tmp_file );
        if( !url.empty() ) {
          Image *image = 
            static_cast< H3DImageLoaderNode * >(*il)->loadImage( url );
          if( is_tmp_file ) ResourceResolver::releaseTmpFileName( url );
          if( image ) {
            texture->setURLUsed( *i );
            return image;
          }
        }
      }
    }
  }
  
  // Now try to find any image loader that can handle the format
  for( vector<string>::const_iterator i = urls.begin(); 
       i != urls.end(); ++i ) {
    // First try to resolve the url to file contents and load via string buffer
    // Otherwise fallback on using temp files
    string url_contents= ResourceResolver::resolveURLAsString ( *i );
    if ( url_contents != "" ) {
      istringstream iss ( url_contents, ios_base::binary );
      auto_ptr< H3DImageLoaderNode > 
        il( H3DImageLoaderNode::getSupportedFileReader( iss ) );

      if( il.get() ) {
        texture->setURLUsed( *i );
        Image *image = il->loadImage( iss );
        return image;
      }
    }

    bool is_tmp_file;
    string url = texture->resolveURLAsFile( *i, &is_tmp_file );
    if( !url.empty() ) {
      auto_ptr< H3DImageLoaderNode > 
        il( H3DImageLoaderNode::getSupportedFileReader( url ) );

      // special case for if we have a DicomImageLoader. Since DICOM 3d image
      // files often are distributed over several files the default behavior
      // is to combine all these files into one image. In this case we want
      // to use only the file the url points to so we modify the reader
      // to do that.
#ifdef HAVE_DCMTK
      DicomImageLoader *dicom_loader = 
        dynamic_cast< DicomImageLoader * >( il.get() );
      if( dicom_loader ) dicom_loader->loadSingleFile->setValue( true );
#endif

      if( il.get() ) {
        texture->setURLUsed( *i );
        Image *image = il->loadImage( url );
        if( is_tmp_file ) ResourceResolver::releaseTmpFileName( url );
        return image;
      }
      if( is_tmp_file ) ResourceResolver::releaseTmpFileName( url );
    }
  }

  Console(4) << "Warning: None of the urls in ImageTexture with url [";
  for( vector<string>::const_iterator i = urls.begin(); 
       i != urls.end(); ++i ) {  
    Console(4) << " \"" << *i << "\"";
  }
  Console(4) << "] could be loaded. Either they don't exist or the file format "
             << "is not supported by any H3DImageLoaderNode that is available "
             << "(in " << getOwner()->getName() << ")" << endl;

  texture->setURLUsed( "" );
  return( NULL );
}

Scene::CallbackCode ImageTexture::SFImage::loadImageCB( void *data ) {
  CBData *input = static_cast< CBData * >( data );
  input->texture->image->setValue( input->image );
  return Scene::CALLBACK_DONE;
}

void *ImageTexture::SFImage::loadImageThreadFunc( void * data ) {
  ThreadFuncData *input = static_cast< ThreadFuncData * >( data );
  SFImage *sfimage = static_cast< SFImage * >( input->texture->image.get() );
  Image *image = sfimage->loadImage( input->texture,
                                     input->urls,
                                     input->image_loaders );
                                  
  sfimage->cb_data.texture = input->texture;
  sfimage->cb_data.image = image;
  Scene::addCallback( loadImageCB, &sfimage->cb_data );
  return NULL;
}

void ImageTexture::SFImage::update() {
  ImageTexture *texture = static_cast< ImageTexture * >( getOwner() );
  MFImageLoader *image_loaders = static_cast< MFImageLoader * >( routes_in[1] );
  MFString *urls = static_cast< MFString * >( routes_in[0] );

  bool load_in_thread = X3DTextureNode::load_images_in_separate_thread;
  GlobalSettings *gs = GlobalSettings::getActive();
  if( gs ) load_in_thread = gs->loadTexturesInThread->getValue();

  if( load_in_thread ) {
    value = NULL;
    // delete the old thread
    texture->load_thread.reset( NULL );
    
    thread_data.texture = texture;
    thread_data.urls = urls->getValue();
    thread_data.image_loaders = image_loaders->getValue();

    texture->load_thread.reset( new H3DUtil::SimpleThread( &loadImageThreadFunc, 
                                                   (void *)&thread_data ) );
    texture->load_thread->setThreadName( "ImageTexture load thread" );
  } else {
    value = loadImage( texture, urls->getValue(), image_loaders->getValue() );
  }

  // reset the editing variables since we are doing a full load of a new
  // texture
  resetChanges();
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
      s << "].";
      e.message = s.str();
      throw e;
    } catch( OpenGLTextureError &e ) {
      stringstream s;
      s << e.message << " with url [" ;
      for( MFString::const_iterator i = url->begin(); i != url->end(); ++i ) { 
        s << " \"" << *i << "\"";
      }
      s << "].";
      e.message = s.str();
      throw e;
    }
  } else {
    texture_target = getTextureTarget();
    disableTexturing();
  }
}


