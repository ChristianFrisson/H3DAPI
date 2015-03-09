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
/// \file ImageTexture.h
/// \brief Header file for ImageTexture, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __IMAGETEXTURE_H__
#define __IMAGETEXTURE_H__

#include <H3D/X3DTexture2DNode.h>
#include <H3D/H3DImageLoaderNode.h>
#include <H3D/X3DUrlObject.h>
#include <H3D/MFNode.h>
#include <H3D/Scene.h>

namespace H3D {
  /// \ingroup X3DNodes 
  /// \class ImageTexture
  /// The ImageTexture node defines a texture map by specifying an image
  /// file and general parameters for mapping to geometry. The texture
  /// is read from the URL specified by the url field using the
  /// ImageLoader nodes from the imageLoader field. 
  /// The texture used will be the first texture that is successfully
  /// loaded. Each file in the url field be tried with the first 
  /// ImageLoader, if none of those succeed the next ImageLoader
  /// will be tried on all the urls and so on.
  ///
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/ImageTexture.x3d">ImageTexture.x3d</a>
  ///     ( <a href="examples/ImageTexture.x3d.html">Source</a> )
  ///
  /// \par Internal routes:
  /// \dotfile ImageTexture.dot

  class H3DAPI_API ImageTexture :
    public X3DTexture2DNode,
    public X3DUrlObject {
  public:

    /// CouldNotLoadImage is thrown when not any of the urls could be read with
    /// any of the image loaders in a ImageTexture.
    H3D_API_EXCEPTION( CouldNotLoadImage );

    /// A MFNode containing ImageLoader.
    typedef TypedMFNode< H3DImageLoaderNode > MFImageLoader;

    /// SFImage is overridden to update the value from the url 
    /// and imageLoader fields of the ImageTexture. Each url is tried
    /// with all ImageLoader and the first one that is successful is 
    /// the one that is used.
    ///
    /// If X3DTextureNode::load_images_in_separate_thread is true, the image 
    /// field will not be set directly. Instead it will be set to NULL, and
    /// the image downloaded and created in a separate thread. When it has 
    /// been created a callback function is set to execute with 
    /// Scene::setCallback that sets the image field to the new image.
    /// This allows the program to continue to execute and run without textures
    /// while the textures are loaded and the textures will then be applied
    /// as they are available.
    ///
    /// If X3DTextureNode::load_images_in_separate_thread is false the image
    /// will be updated directly and the program has to wait for it to 
    /// download if it is not available.
    /// 
    /// routes_in[0] is the url field.
    /// routes_in[1] is the imageLoader field.
    ///
    class H3DAPI_API SFImage: 
      public TypedField< X3DTexture2DNode::SFImage,
                         Types< MFString, MFImageLoader > > {
    protected:
      /// Updates the value from the url and imageLoader fields
      /// of the ImageTexture
      virtual void update();
      
      /// Tries to create an image. All urls are tested agains all
      /// image loaders until one that works is found. Returns NULL
      /// if it could not load the image, and the new image on success.
      Image * loadImage( ImageTexture *texture,
                         const vector< string > &urls,
                         const NodeVector &image_loader ); 

      /// struct for storing input needed to the thread function
      struct ThreadFuncData {
        ImageTexture *texture;
        vector< string > urls;
        NodeVector image_loaders;
        MutexLock load_thread_mutex;
      };
      
      /// struct for storing input needed for the callback function.
      struct CBData {
        CBData():texture(NULL), image(NULL){}
        ImageTexture *texture;
        Image *image;
      };

      /// data used for the thread function
      ThreadFuncData thread_data;

      /// data used for the callback function.
      CBData cb_data;
      
      /// Callback function for setting the image field when thread 
      /// has finished downloading
      static Scene::CallbackCode loadImageCB( void *data );

      /// Function to use in the thread for downloading and creating an
      /// Image object.
      static void *loadImageThreadFunc( void * );
    public:
      virtual ~SFImage();
    };
      
    /// Constructor.
    ImageTexture( Inst< DisplayList > _displayList = 0,
                  Inst< SFNode   >  _metadata        = 0,
                  Inst< MFString >  _url             = 0,
                  Inst< SFBool   >  _repeatS         = 0,
                  Inst< SFBool   >  _repeatT         = 0,
                  Inst< SFBool   >  _scaleToP2       = 0,
                  Inst< SFImage  > _image            = 0,
                  Inst< MFImageLoader > _imageLoader = 0,
                  Inst< SFTextureProperties > _textureProperties = 0,
                  Inst< SFString > _loadInThread     = 0,
                  Inst< SFBool   > _canShare         = 0 );

    virtual ~ImageTexture ();

    /// render() is overridden to include the url in error messages.
    virtual void render();

    /// Overridden virtual function to perform the same function on the correct 
    /// shared texture from the database.
    virtual void postRender();

    /// Overridden virtual function to perform the same function on the correct 
    /// shared texture from the database.
    virtual void preRender();

    /// Overridden virtual function to perform the same function on the correct 
    /// shared texture from the database.
    virtual void enableTexturing();

    /// Overridden virtual function to perform the same function on the correct 
    /// shared texture from the database.
    virtual void disableTexturing();

    /// Overridden virtual function to perform the same function on the correct 
    /// shared texture from the database.
    virtual GLuint getTextureId();

    /// Overridden virtual function to perform the same function on the correct 
    /// shared texture from the database.
    virtual GLuint getTextureUnit();

    /// Overridden virtual function to perform the same function on the correct 
    /// shared texture from the database.
    virtual GLenum getTextureTarget();

    /// Overridden virtual function to perform the same function on the correct 
    /// shared texture from the database.
    virtual bool makeResident ();

    /// Overridden virtual function to perform the same function on the correct 
    /// shared texture from the database.
    virtual void makeNonResident ();

    /// Overridden virtual function to perform the same function on the correct 
    /// shared texture from the database.
    virtual bool isResident ();

    /// Overridden virtual function to perform the same function on the correct 
    /// shared texture from the database.
    virtual GLuint64 getTextureHandle();

    /// Overridden virtual function to perform the same function on the correct 
    /// shared texture from the database.
    virtual void invalidateTextureHandle ();

    /// Overridden virtual function to perform the same function on the correct 
    /// shared texture from the database.
    virtual void inUse ();


    virtual Image* renderToImage ( H3DInt32 _width, H3DInt32 _height, bool output_float_texture = false  );

    /// Returns the default dimensions to use when this texture is saved to file.
    ///
    /// Returns the dimensions of the Image object
    ///
    virtual std::pair<H3DInt32,H3DInt32> getDefaultSaveDimensions ();

    /// The ImageLoader nodes to use to read the image files.
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> FreeImageLoader \n
    /// 
    /// \dotfile ImageTexture_imageLoader.dot
    auto_ptr< MFImageLoader > imageLoader;

    /// Specifies the thread to use to load the image texture.
    ///
    /// If "DEFAULT" then the setting in GlobalSettings will be used.
    /// If "MAIN" then the main thread is used to load the texture.
    /// If "SEPARATE" then a new thread is created and used to load the texture.
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> "DEFAULT" \n
    /// <b>Valid values:</b> "DEFAULT", "MAIN", "SEPARATE"
    /// 
    /// \dotfile ImageTexture_loadInThread.dot
    auto_ptr< SFString > loadInThread;

    /// Share image data for URLs that are the same. Also requires global
    /// GraphicsOption to enable.
    ///
    /// Note: Images with the same URL and different properties are not handled and
    /// therefore you should use this option to disable sharing of such textures.
    ///
    /// <b>Default value: </b> true \n
    /// <b>Access type: </b> inputOutput \n
    auto_ptr< SFBool > canShare;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  protected:

    /// Returns true if texture sharing should be enabled
    bool useSharing ();

    /// If this image is currently referencing some shared texture, remove this reference
    /// If there are no other references, the texture is deleted
    void removeSharedImage ();

    /// Add a reference to, or create as needed, a shared texture with this url
    void addSharedImage ( std::vector < std::string > _urls );

    /// The thread used for downloading images when 
    /// load_images_in_separate_thread is true.
    auto_ptr< H3DUtil::SimpleThread > load_thread;

    /// A shared texture
    struct SharedImage {
      SharedImage () : use_count ( 0 ) {}

      AutoRef < Node > image;
      int use_count; ///< Number of images referencing this shared texture
    };

    /// Pointer to the shared texture currently in use, or NULL
    ImageTexture* wrapped_image;
    bool inited_sharing;
    bool share_textures;

    typedef std::map < std::vector < std::string >, SharedImage > ImageDatabase;

    /// The shared texture database
    static ImageDatabase image_database;
  };
}

#endif
