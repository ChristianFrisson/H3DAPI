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
/// \file NrrdImageLoader.h
/// \brief Header file for NrrdImageLoader, a image loader using the nrrd
/// library to read images.
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __NRRDIMAGELOADER_H__
#define __NRRDIMAGELOADER_H__

#include <H3D/H3DImageLoaderNode.h>

#include <H3DUtil/LoadImageFunctions.h>

#ifdef HAVE_TEEM

namespace H3D {
  /// \ingroup H3DNodes
  /// \class NrrdImageLoader
  /// NrrdImageLoader is a class which takes care of loading the nrrd image
  /// format. See <a href="http://teem.sourceforge.net/nrrd/">Nrrd Homepage</a>
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/ImageLoaders.x3d">ImageLoaders.x3d</a>
  ///     ( <a href="examples/ImageLoaders.x3d.html">Source</a> )
  class H3DAPI_API NrrdImageLoader : public H3DImageLoaderNode {
  public:
    /// Constructor.
    NrrdImageLoader() {
      type_name = "NrrdImageLoader";
    }

    /// Load the image using the NrrdImage library. A new NrrdImageImage
    /// is returned. NULL if not successfully loaded.
    virtual Image *loadImage( const string &url );
    
    /// Returns true if the node supports the filetype of the file
    /// specified by url.
    static bool supportsFileType( const string &url );

    /// The H3DNodeData for this node.
    static H3DNodeDatabase database;    

    /// Register this node to the file readers available.
    static FileReaderRegistration reader_registration;
  };
}

#endif // HAVE_TEEM
#endif
