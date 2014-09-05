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
/// \file OpenEXRImageLoader.h
/// \brief Header file for OpenEXRImageLoader, a image loader using the FreeImage
/// library to read images.
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __OPENEXRIMAGELOADER_H__
#define __OPENEXRIMAGELOADER_H__

#include <H3D/H3DImageLoaderNode.h>

#ifdef HAVE_OPENEXR

#include <H3DUtil/LoadImageFunctions.h>

namespace H3D {

  // Note for developers.
  // This class will cause a minor "memory leak" because it never deinitializes
  // some statically initialized classes. See
  // See http://lists.nongnu.org/archive/html/openexr-devel/2013-11/msg00003.html
  // for more information.

  /// \ingroup H3DNodes
  /// \class OpenEXRImageLoader
  /// OpenEXRImageLoader uses the OpenEXR library to load the images.
  /// HDR floating point imagees 
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/ImageLoaders.x3d">ImageLoaders.x3d</a>
  ///     ( <a href="examples/ImageLoaders.x3d.html">Source</a> )
  class H3DAPI_API OpenEXRImageLoader : public H3DImageLoaderNode {
  public:
    /// Constructor.
    OpenEXRImageLoader() {
      type_name = "OpenEXRImageLoader";
    }

    /// Load the image using the OpenEXR library. A new PixelImage
    /// is returned. NULL if not successfully loaded.
    /// \param url URL to the file to load.
    virtual Image *loadImage( const string &url ) {
      return H3DUtil::loadOpenEXRImage ( url );
    }
    
    /// Returns true if the node supports the filetype of the file
    /// specified by url.
    static bool supportsFileType( const string &url );
    
    static H3DNodeDatabase database;    

    /// Register this node to the available readers.
    static FileReaderRegistration reader_registration;
  };
}

#endif // HAVE_OPENEXR
#endif
