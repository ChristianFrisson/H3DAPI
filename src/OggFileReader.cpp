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
/// \file OggFileReader.cpp
/// \brief CPP file for OggFileReader, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/OggFileReader.h>
#include <fstream>

#ifdef HAVE_LIBVORBIS

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase OggFileReader::database( 
                                        "OggFileReader", 
                                        &(newInstance< OggFileReader >),
                                          typeid( OggFileReader ),
                                          &H3DSoundFileNode::database );

H3DSoundFileNode::FileReaderRegistration OggFileReader::reader_registration(
                            "OggFileReader",
                            &(newSoundFileNode< OggFileReader >),
                            &OggFileReader::supportsFileType 
                            );

unsigned int OggFileReader::read( char *buffer, unsigned int size ) {
      
  int section;
  unsigned int bytes_read = 0;
  
  while( bytes_read < size ) {
    int res = ov_read(&ogg_file, buffer + bytes_read, 
                      size - bytes_read, !isLittleEndian(), 2, 1, & section);
    if( !should_clear )
      should_clear = true;
    if( res <= 0 ) break;
    else  bytes_read += res;
  }
  return bytes_read;
}

unsigned int OggFileReader::load( const string &_url ) {
  url = _url;
 
  FILE *f= fopen( url.c_str(), "rb" );
  if( f != NULL ) {
    if( ov_open_callbacks( f, &ogg_file, NULL, 0, OV_CALLBACKS_DEFAULT ) < 0 ) {
      if( !should_clear )
        should_clear = true;
      fclose( f );
      return 0;
    } else {
      info = ov_info(&ogg_file, -1);
      comment = ov_comment(&ogg_file, -1);
    }
  } else {
    return 0;
  }
  return totalDataSize();
}

bool OggFileReader::supportsFileType( const string &url ) {
 

  FILE *f= fopen( url.c_str(), "rb" );
  if( f != NULL ) {
    OggVorbis_File tmp_ogg_file;
    if( ov_open_callbacks( f, &tmp_ogg_file, NULL, 0, OV_CALLBACKS_DEFAULT ) < 0 ) {
      fclose( f );
      return false;
    } else {
      ov_clear( &tmp_ogg_file );
      return true;
    }
  } else {
    return false;
  }
}

#endif
