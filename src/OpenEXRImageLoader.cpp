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
/// \file OpenEXRImageLoader.cpp
/// \brief CPP file for OpenEXRImageLoader, a image loader using the FreeImage
/// library to read images.
///
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/OpenEXRImageLoader.h>

#ifdef HAVE_OPENEXR
#include <OpenEXR/ImfTestFile.h>
#include <OpenEXR/ImfNamespace.h>
namespace IMF = OPENEXR_IMF_NAMESPACE;
using namespace IMF;

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase OpenEXRImageLoader::database( "OpenEXRImageLoader", 
                                           &(newInstance<OpenEXRImageLoader>), 
                                           typeid( OpenEXRImageLoader ) );

H3DImageLoaderNode::FileReaderRegistration 
OpenEXRImageLoader::reader_registration(
                            "OpenEXRImageLoader",
                            &(newImageLoaderNode< OpenEXRImageLoader >),
                            &OpenEXRImageLoader::supportsFileType
                            );

bool OpenEXRImageLoader::supportsFileType( const string &url ) {
  return isOpenExrFile ( url.c_str() );
}

#endif // HAVE_FREEIMAGE
