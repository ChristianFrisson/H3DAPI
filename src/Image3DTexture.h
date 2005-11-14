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
/// \file Image3DTexture.h
/// \brief Header file for Image3DTexture, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __IMAGE3DTEXTURE_H__
#define __IMAGE3DTEXTURE_H__

#include "X3DTexture3DNode.h"
#include "H3DImageLoaderNode.h"
#include "X3DUrlObject.h"

namespace H3D {
  /// \ingroup Nodes 
  /// \class Image3DTexture
  /// \brief The Image3DTexture node defines a texture map by
  /// specifying a single file that contains complete 3D data and
  /// general parameters for mapping to geometry. The texture
  /// is read from the URL specified by the url field using the
  /// H3DImageLoaderNode nodes from the imageLoader field. 
  /// The texture used will be the first texture that is successfully
  /// loaded. Each file in the url field be tried with the first 
  /// ImageLoader, if none of those succeed the next ImageLoader
  /// will be tried on all the urls and so on.
  ///
  /// \par Internal routes:
  /// \dotfile Image3DTexture.dot

  class H3DAPI_API Image3DTexture : 
    public X3DTexture3DNode,
    public X3DUrlObject {
  public:

    /// CouldNotLoadImage is thrown when not any of the urls could be read with
    /// any of the image loaders in a Image3DTexture.
    H3D_API_EXCEPTION( CouldNotLoadImage );

    /// A MFNode containing ImageLoader.
    typedef TypedMFNode< H3DImageLoaderNode > MFImageLoader;

    /// SFImage is overridden to update the value from the url 
    /// and imageLoader fields of the Image3DTexture. Each url is tried
    /// with all ImageLoader and the first one that is successful is 
    /// the one that is used.
    /// routes_in[0] is the url field.
    /// routes_in[1] is the imageLoader field.
    ///
    class H3DAPI_API SFImage: 
      public TypedField< X3DTexture3DNode::SFImage,
                         Types< MFString, MFImageLoader > > {
    protected:
      /// Updates the value from the url and imageLoader fields
      /// of the Image3DTexture
      virtual void update();
    };
      
    /// Constructor.
    Image3DTexture( Inst< DisplayList   > _displayList = 0,
                    Inst< SFNode        > _metadata    = 0,
                    Inst< MFString      > _url         = 0,
                    Inst< SFBool        > _repeatS     = 0,
                    Inst< SFBool        > _repeatT     = 0,
                    Inst< SFBool        > _repeatR     = 0,   
                    Inst< SFBool        > _scaleToP2   = 0,
                    Inst< SFImage       > _image       = 0,
                    Inst< SFBool        > _interpolate = 0,
                    Inst< MFImageLoader > _imageLoader = 0 );

    /// render() is overridden to include the url in error messages.
    virtual void render();

    /// The ImageLoader nodes to use to read the image files.
    /// 
    /// Access type:</b> inputOutput \n
    /// Default value:</b> FreeImageLoader \n
    /// 
    /// \dotfile Image3DTexture_imageLoader.dot
    auto_ptr< MFImageLoader > imageLoader;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  };
}

#endif
