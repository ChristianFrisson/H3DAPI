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
/// \file FreeImageLoader.h
/// \brief Header file for FreeImageLoader, a image loader using the FreeImage
/// library to read images.
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __FREEIMAGELOADER_H__
#define __FREEIMAGELOADER_H__

#include <H3D/H3DImageLoaderNode.h>

#ifdef HAVE_FREEIMAGE

#include <H3DUtil/LoadImageFunctions.h>

namespace H3D {
  /// \ingroup H3DNodes
  /// \class FreeImageLoader
  /// FreeImageLoader uses the FreeImage library to load the images.
  /// Most image file types are supported including PNG, JPEG, GIF, BMP,
  /// TIFF and PSD
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/ImageLoaders.x3d">ImageLoaders.x3d</a>
  ///     ( <a href="examples/ImageLoaders.x3d.html">Source</a> )
  class H3DAPI_API FreeImageLoader : public H3DImageLoaderNode {
  public:
    /// Constructor.
    FreeImageLoader() {
      type_name = "FreeImageLoader";
    }

    /// Load the image using the FreeImage library. A new FreeImageImage
    /// is returned. NULL if not successfully loaded.
    /// \param url URL to the file to load.
    virtual Image *loadImage( const string &url ) {
      return H3DUtil::loadFreeImage( url );
    }
    
    /// TODO: Implement 
    ////virtual Image *loadImage( const istream &is ) = 0;
    
    /// Returns true if the node supports the filetype of the file
    /// specified by url.
    static bool supportsFileType( const string &url );

    static H3DNodeDatabase database;    

    /// Register this node to the H3DSoundFileNodes available.
    static FileReaderRegistration reader_registration;
  };
}

#endif // HAVE_FREEIMAGE
#endif
