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
/// \file DDSImageLoader.cpp
/// \brief CPP file for DDSImageLoader, an image loader for DDS format
///
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/DDSImageLoader.h>
#include <H3D/OpenEXRImageLoader.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase DDSImageLoader::database( "DDSImageLoader", 
                                           &(newInstance<DDSImageLoader>), 
                                           typeid( DDSImageLoader ) );

H3DImageLoaderNode::FileReaderRegistration 
DDSImageLoader::reader_registration(
                            "DDSImageLoader",
                            &(newImageLoaderNode< DDSImageLoader >),
                            &DDSImageLoader::supportsFileType
                            );

bool DDSImageLoader::supportsFileType( const string &url ) {
  // We could check the magic number, but keep it simple
  return url.substr( url.length() - 4 ) == ".dds";
}

bool DDSImageLoader::supportsStreamType( istream &is ) {
  // Magic number
  const unsigned int dds_magic_no = 0x20534444;

  // Check magic number
  int magic_no;
  is.read( (char*)&magic_no, sizeof( magic_no ) );
  bool is_dds = is && magic_no == dds_magic_no;

  // Rewind stream
  is.clear();
  is.seekg( 0 );

  return is_dds;
}