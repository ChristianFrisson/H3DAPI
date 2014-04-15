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
/// \file FreeImageLoader.cpp
/// \brief CPP file for FreeImageLoader, a image loader using the FreeImage
/// library to read images.
///
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/FreeImageLoader.h>

#ifdef HAVE_FREEIMAGE
#include <H3DUtil/FreeImageImage.h>
#include <FreeImage.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase FreeImageLoader::database( "FreeImageLoader", 
                                           &(newInstance<FreeImageLoader>), 
                                           typeid( FreeImageLoader ) );

H3DImageLoaderNode::FileReaderRegistration 
FreeImageLoader::reader_registration(
                            "FreeImageLoader",
                            &(newImageLoaderNode< FreeImageLoader >),
                            &FreeImageLoader::supportsFileType,
                            &FreeImageLoader::supportsStreamType
                            );

bool FreeImageLoader::supportsFileType( const string &url ) {
  FREE_IMAGE_FORMAT format = FreeImage_GetFileType( url.c_str() );
  return format != FIF_UNKNOWN;
}

bool FreeImageLoader::supportsStreamType( istream &is ) {
  FREE_IMAGE_FORMAT format = FreeImage_GetFileTypeFromHandle ( FreeImageImage::getIStreamIO(), static_cast<fi_handle>(&is) );
  return format != FIF_UNKNOWN;
}

#endif // HAVE_FREEIMAGE
