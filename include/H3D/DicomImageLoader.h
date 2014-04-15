//////////////////////////////////////////////////////////////////////////////
//    Copyright 2004-2014, SenseGraphics AB
//
//    This file is part of MedX3D.
//
//    MedX3D is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    MedX3D is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with MedX3D; if not, write to the Free Software
//    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//    A commercial license is also available. Please contact us at 
//    www.sensegraphics.com for more information.
//
//
/// \file DicomImageLoader.h
/// \brief Header file for DicomImageLoader, a image loader using the Dicom
/// library to read images.
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __DICOMIMAGELOADER_H__
#define __DICOMIMAGELOADER_H__

#include <H3D/H3DImageLoaderNode.h>
#include <H3D/SFBool.h>
#include <H3DUtil/LoadImageFunctions.h>

#ifdef HAVE_DCMTK

namespace H3D {
  /// \ingroup H3DNodes
  /// \class DicomImageLoader
  /// DicomImageLoader uses the DCMTK library to load a DICOM image file.
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/ImageLoaders.x3d">ImageLoaders.x3d</a>
  ///     ( <a href="examples/ImageLoaders.x3d.html">Source</a> )
  class H3DAPI_API DicomImageLoader : public H3DImageLoaderNode {
  public:
    /// Constructor.
    DicomImageLoader();

    /// Load the image using the DCMTK library. A new DicomImage
    /// is returned. NULL if not successfully loaded.
    virtual Image *loadImage( const string &url );

    /// Returns true if the node supports the filetype of the file
    /// specified by url.
    static bool supportsFileType( const string &url );

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;  

    /// Register this node to the H3DImageLoaderNodes available.
    static FileReaderRegistration reader_registration;

    /// If true then DicomImageLoader only load the file with the
    /// specified url instead of trying to find all files that belongs to the
    /// dataset. Default value is false.
    auto_ptr< SFBool > loadSingleFile;
  };
}

#endif // HAVE_DCMTK

#endif
