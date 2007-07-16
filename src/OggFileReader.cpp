//////////////////////////////////////////////////////////////////////////////
//    Copyright 2004-2007, SenseGraphics AB
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

#include <OggFileReader.h>
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

namespace OggFileReaderInternals {
  // funcions for use with the ov_callback structure.
  size_t read(void *ptr, size_t size, size_t nmemb, void *datasource) {
    ifstream *is = static_cast< ifstream * >( datasource );
    is->read( (char *)ptr, size * nmemb );
    return is->gcount();
  }

  int seek(void *datasource, ogg_int64_t offset, int whence) {
    return -1;
  }

  int close(void *datasource) {
    ifstream *is = static_cast< ifstream * >( datasource );
    is->close();
    delete is;
    return 0;
  }
  
  long tell(void *datasource) {
    ifstream *is = static_cast< ifstream * >( datasource );
    return is->tellg();
  }
}

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
  ov_callbacks cb;
  cb.read_func = &OggFileReaderInternals::read;
  cb.seek_func = &OggFileReaderInternals::seek;
  cb.close_func = &OggFileReaderInternals::close;
  cb.tell_func = &OggFileReaderInternals::tell;

  ifstream *is= new ifstream( url.c_str(), ios::binary );
  if( is->good() ) {
    if( ov_open_callbacks( is, &ogg_file, NULL, 0, cb ) < 0 ) {
      if( !should_clear )
        should_clear = true;
      is->close();
      delete is;
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
  ov_callbacks cb;
  cb.read_func = &OggFileReaderInternals::read;
  cb.seek_func = &OggFileReaderInternals::seek;
  cb.close_func = &OggFileReaderInternals::close;
  cb.tell_func = &OggFileReaderInternals::tell;

  ifstream *is= new ifstream( url.c_str(), ios::binary );
  if( is->good() ) {
    OggVorbis_File tmp_ogg_file;
    if( ov_open_callbacks( is, &tmp_ogg_file, NULL, 0, cb ) < 0 ) {
      is->close();
      delete is;
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
