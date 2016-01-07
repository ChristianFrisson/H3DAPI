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
#include <H3D/X3DProgrammableShaderObject.h>

//#define DEBUG_SHARING

using namespace H3D;

ImageTexture::ImageDatabase ImageTexture::image_database;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase ImageTexture::database( "ImageTexture", 
                                        &(newInstance<ImageTexture>), 
                                        typeid( ImageTexture ),
                                        &X3DTexture2DNode::database );

namespace ImageTextureInternals {
  FIELDDB_ELEMENT( ImageTexture, url, INPUT_OUTPUT );
  FIELDDB_ELEMENT( ImageTexture, imageLoader, INPUT_OUTPUT );
  FIELDDB_ELEMENT( ImageTexture, loadInThread, INPUT_OUTPUT );
  FIELDDB_ELEMENT( ImageTexture, canShare, INITIALIZE_ONLY );
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
                           Inst< SFTextureProperties > _textureProperties,
                           Inst< SFString      > _loadInThread,
                           Inst< SFBool        > _canShare ) :
  X3DTexture2DNode( _displayList, _metadata, _repeatS, _repeatT,
                    _scaleToP2, _image, _textureProperties ),
  X3DUrlObject( _url ),
  imageLoader( _imageLoader ),
  loadInThread ( _loadInThread ),
  canShare ( _canShare ),
  load_thread( NULL ),
  wrapped_image ( NULL ),
  inited_sharing ( false ),
  share_textures ( false ) {

  type_name = "ImageTexture";
  database.initFields( this );

  loadInThread->addValidValue ( "DEFAULT" );
  loadInThread->addValidValue ( "MAIN" );
  loadInThread->addValidValue ( "SEPARATE" );
  loadInThread->setValue ( "DEFAULT" );

  canShare->setValue ( true );

  url->route( image );
  imageLoader->route( image );
}

ImageTexture::~ImageTexture () {
  removeSharedImage ();
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
           ++il ) {
        // First try to resolve the url to file contents and load via string buffer
        // Otherwise fallback on using temp files
        string url_contents= texture->resolveURLAsString ( *i );
        if ( url_contents != "" ) {
          istringstream tmp_istream( url_contents );
          Image *_image =
            static_cast< H3DImageLoaderNode * >(*il)->loadImage ( tmp_istream );
          if( _image ) {
            texture->setURLUsed( *i );
            return _image;
          }
        }

        bool is_tmp_file;
        string _url = texture->resolveURLAsFile( *i, &is_tmp_file );
        if( !_url.empty() ) {
          Image *_image = 
            static_cast< H3DImageLoaderNode * >(*il)->loadImage( _url );
          if( is_tmp_file ) ResourceResolver::releaseTmpFileName( _url );
          if( _image ) {
            texture->setURLUsed( *i );
            return _image;
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
    string url_contents= texture->resolveURLAsString ( *i );
    if ( url_contents != "" ) {
      istringstream iss ( url_contents );
      auto_ptr< H3DImageLoaderNode > 
        il( H3DImageLoaderNode::getSupportedFileReader( iss ) );

      if( il.get() ) {
        texture->setURLUsed( *i );
        Image *_image = il->loadImage( iss );
        return _image;
      }
    }

    bool is_tmp_file;
    string _url = texture->resolveURLAsFile( *i, &is_tmp_file );
    if( !_url.empty() ) {
      auto_ptr< H3DImageLoaderNode > 
        il( H3DImageLoaderNode::getSupportedFileReader( _url ) );

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
        Image *_image = il->loadImage( _url );
        if( is_tmp_file ) ResourceResolver::releaseTmpFileName( _url );
        return _image;
      }
      if( is_tmp_file ) ResourceResolver::releaseTmpFileName( _url );
    }
  }

  Console(LogLevel::Error) << "Warning: None of the urls in ImageTexture with url [";
  for( vector<string>::const_iterator i = urls.begin(); 
       i != urls.end(); ++i ) {  
    Console(LogLevel::Error) << " \"" << *i << "\"";
  }
  Console(LogLevel::Error) << "] could be loaded. Either they don't exist or the file format "
             << "is not supported by any H3DImageLoaderNode that is available "
             << "(in " << getOwner()->getName() << ")" << endl;

  texture->setURLUsed( "" );
  return( NULL );
}

Scene::CallbackCode ImageTexture::SFImage::loadImageCB( void *data ) {
  CBData *input = static_cast< CBData * >( data );
  input->texture->image->setValue( input->image );
  input->texture = NULL;
  input->image = NULL;
  return Scene::CALLBACK_DONE;
}

void *ImageTexture::SFImage::loadImageThreadFunc( void * data ) {
  ThreadFuncData *input = static_cast< ThreadFuncData * >( data );
  input->load_thread_mutex.lock();
  SFImage *sfimage = static_cast< SFImage * >( input->texture->image.get() );
  Image *image = sfimage->loadImage( input->texture,
                                     input->urls,
                                     input->image_loaders );
                                  
  sfimage->cb_data.texture = input->texture;
  sfimage->cb_data.image = image;
  Scene::addCallback( loadImageCB, &sfimage->cb_data );
  input->load_thread_mutex.unlock();
  return NULL;
}

void ImageTexture::SFImage::update() {
  ImageTexture *texture = static_cast< ImageTexture * >( getOwner() );
  MFImageLoader *image_loaders = static_cast< MFImageLoader * >( routes_in[1] );
  MFString *urls = static_cast< MFString * >( routes_in[0] );

  if ( texture->useSharing () ) {
    texture->removeSharedImage ();
    texture->addSharedImage ( urls->getValue() );
    return;
  }

  bool load_in_thread = X3DTextureNode::load_images_in_separate_thread;
  
  const string& load_in_thread_local= texture->loadInThread->getValue();
  if ( load_in_thread_local == "DEFAULT" ) {
    GlobalSettings *gs = GlobalSettings::getActive();
    if( gs ) load_in_thread = gs->loadTexturesInThread->getValue();
  } else {
    load_in_thread= load_in_thread_local == "SEPARATE";
  }

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

  if ( X3DProgrammableShaderObject::use_bindless_textures ) {
    // We must make the texture non-resident before the image is swapped
    // so that a new texture handle can be created.
    texture->invalidateTextureHandle ();
  }
}

void ImageTexture::render() {
  if( url->size() > 0 ) {
    try {

      if ( useSharing () ) {
        image->upToDate();
        if ( wrapped_image ) {
          wrapped_image->displayList->callList();
        }
      } else {
        X3DTexture2DNode::render();
      }

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
    //texture_target = getTextureTarget();
    disableTexturing();
  }
}

void ImageTexture::postRender() {
  if ( wrapped_image ) {
    wrapped_image->postRender ();
  } else {
    X3DTexture2DNode::postRender ();
  }
}

void ImageTexture::preRender() {
  image->upToDate();
  if ( wrapped_image ) {
    wrapped_image->preRender ();
  } else {
    X3DTexture2DNode::preRender ();
  }
}

void ImageTexture::enableTexturing() {
  if ( wrapped_image ) {
    wrapped_image->enableTexturing ();
  } else {
    X3DTexture2DNode::enableTexturing ();
  }
}

void ImageTexture::disableTexturing() {
  if ( wrapped_image ) {
    wrapped_image->disableTexturing ();
  } else {
    X3DTexture2DNode::disableTexturing ();
  }
}

GLuint ImageTexture::getTextureId() {
  if ( wrapped_image ) {
    return wrapped_image->getTextureId ();
  } else {
    return X3DTexture2DNode::getTextureId ();
  }
}

GLuint ImageTexture::getTextureUnit() {
  if ( wrapped_image ) {
    return wrapped_image->getTextureUnit ();
  } else {
    return X3DTexture2DNode::getTextureUnit ();
  }
}

GLenum ImageTexture::getTextureTarget() {
  if ( wrapped_image ) {
    return wrapped_image->getTextureTarget ();
  } else {
    return X3DTexture2DNode::getTextureTarget ();
  }
}

void ImageTexture::setTextureId( GLuint id ){
  if (wrapped_image){
    wrapped_image->setTextureId(id);
  }else{
    X3DTexture2DNode::setTextureId(id);
  }
}

void ImageTexture::setTextureUnit( GLint unit ){
  if (wrapped_image){
    wrapped_image->setTextureUnit(unit);
  }else{
    X3DTexture2DNode::setTextureUnit(unit);
  }
}

void ImageTexture::setTextureTarget( GLenum target ){
  if (wrapped_image){
    wrapped_image->setTextureTarget(target);
  }else{
    X3DTexture2DNode::setTextureTarget(target);
  }
}

bool ImageTexture::makeResident() {
  if ( wrapped_image ) {
    return wrapped_image->makeResident ();
  } else {
    return X3DTexture2DNode::makeResident ();
  }
}

void ImageTexture::makeNonResident() {
  if ( wrapped_image ) {
    wrapped_image->makeNonResident ();
  } else {
    X3DTexture2DNode::makeNonResident ();
  }
}

bool ImageTexture::isResident() {
  if ( wrapped_image ) {
    return wrapped_image->isResident ();
  } else {
    return X3DTexture2DNode::isResident ();
  }
}

GLuint64 ImageTexture::getTextureHandle() {
  if ( wrapped_image ) {
    return wrapped_image->getTextureHandle ();
  } else {
    return X3DTexture2DNode::getTextureHandle ();
  }
}

void ImageTexture::invalidateTextureHandle() {
  if ( wrapped_image ) {
    wrapped_image->invalidateTextureHandle ();
  } else {
    X3DTexture2DNode::invalidateTextureHandle ();
  }
}

void ImageTexture::inUse() {
  if ( wrapped_image ) {
    wrapped_image->inUse ();
  } else {
    X3DTexture2DNode::inUse ();
  }
}

bool ImageTexture::useSharing () {
  if ( !inited_sharing ) {
    share_textures= false;
    GraphicsOptions* go = NULL;
    GlobalSettings* gs = GlobalSettings::getActive();
    if( gs ) {
      gs->getOptionNode( go );
    }
    if ( go ) {
      share_textures= go->shareTextures->getValue();
    }
    share_textures= share_textures && canShare->getValue ();
    inited_sharing= true;
  }

  return share_textures;
}

void ImageTexture::removeSharedImage () {
  // Deref shared image
  if ( useSharing () && wrapped_image ) {
    wrapped_image->displayList->unroute ( displayList );
      
    SharedImage& si= image_database [ wrapped_image->url->getValue() ];
    si.use_count--;
    if ( si.use_count <= 0 ) {
#ifdef DEBUG_SHARING
      if ( !wrapped_image->url->empty() ) {
        Console(LogLevel::Error) << "[ImageDatabase]: DELETE image: " << wrapped_image->url->getValueByIndex ( 0 ) << endl;
      }
#endif
      image_database.erase ( wrapped_image->url->getValue() );
    }

    wrapped_image= NULL;

#ifdef DEBUG_SHARING
    Console(LogLevel::Error) << "[ImageDatabase]: DEREF share. There are " << image_database.size() << " image(s) in the database" << endl;
#endif
  }
}

void ImageTexture::addSharedImage ( std::vector < std::string > _urls ) {
  // Ref shared image
  if ( useSharing () ) {
    SharedImage& si= image_database [ _urls ];
    if ( !si.image.get() ) {
      ImageTexture* i= new ImageTexture ();
      i->setName ( getName() + "[WrappedImage]" );
      i->setURLBase ( getURLBase() );
      i->canShare->setValue ( false );
      i->url->setValue ( _urls );

      // Route all texture properties to the wrapped texture
      // Limitation: All textures with the same URL will receive the
      // same properties.
      textureProperties->route ( i->textureProperties );
      loadInThread->route ( i->loadInThread );
      repeatS->route ( i->repeatS );
      repeatT->route ( i->repeatT );
      scaleToPowerOfTwo->route ( i->scaleToPowerOfTwo );
      imageLoader->route ( i->imageLoader );

      si.image.reset ( i );
#ifdef DEBUG_SHARING
      if ( !_urls.empty() ) {
        Console(LogLevel::Error) << "[ImageDatabase]: NEW image: " << _urls[0] << endl;
      }
#endif
    }
    wrapped_image= static_cast < ImageTexture* > ( si.image.get() );
    wrapped_image->textureProperties->setValue( static_cast < ImageTexture* > ( si.image.get() )->textureProperties->getValue() );
    wrapped_image->displayList->route ( displayList );
    si.use_count++;

#ifdef DEBUG_SHARING
    Console(LogLevel::Error) << "[ImageDatabase]: REF share. There are " << image_database.size() << " image(s) in the database" << endl;
#endif
  }
}

ImageTexture::SFImage::~SFImage() {
  thread_data.load_thread_mutex.lock();
  if( cb_data.texture ) {
    Scene::removeCallback( &cb_data );
    if( cb_data.image )
      delete cb_data.image;
    cb_data.texture = NULL;
    cb_data.image = NULL;
  }
  thread_data.load_thread_mutex.unlock();
}

Image* ImageTexture::renderToImage( H3DInt32 _width, H3DInt32 _height, bool output_float_texture /* = false */ ){
  return X3DTextureNode::renderToImage(_width, _height, output_float_texture);
}

std::pair<H3DInt32,H3DInt32> ImageTexture::getDefaultSaveDimensions(){
  if ( wrapped_image ) {
    return wrapped_image->getDefaultSaveDimensions ();
  } else {
    return X3DTexture2DNode::getDefaultSaveDimensions ();
  }
}

GLint ImageTexture::glInternalFormat( Image *i ) {
  TextureProperties *texture_properties = textureProperties->getValue();
  GLint tex_format;
  if( texture_properties ) {
    if( !texture_properties->glInternalFormat( i, tex_format ) ) {
      tex_format = X3DTextureNode::glInternalFormat( i );
    }
  } else {
    tex_format = X3DTextureNode::glInternalFormat( i );
  }

  // Choose compression
  std::string compression = "DEFAULT";

  // Get global setting if present
  GraphicsOptions *graphics_options = NULL;
  GlobalSettings *default_settings = GlobalSettings::getActive();
  if( default_settings ) {
    default_settings->getOptionNode( graphics_options );
  }
  if( graphics_options ) {
    compression = graphics_options->textureCompression->getValue();
  }

  // Combine with local setting
  if( texture_properties ) {
    const std::string& compression_local = texture_properties->textureCompression->getValue();
    if( compression_local != "DEFAULT" ) {
      compression = compression_local;
    }
  }

  return glCompressedInternalFormat( tex_format, compression );
}