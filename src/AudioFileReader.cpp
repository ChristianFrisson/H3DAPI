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
/// \file AudioFileReader.cpp
/// \brief CPP file for AudioFileReader, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/AudioFileReader.h>

#ifdef HAVE_LIBAUDIOFILE

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase AudioFileReader::database( 
                                        "AudioFileReader", 
                                        &(newInstance< AudioFileReader >),
                                          typeid( AudioFileReader ),
                                          &H3DSoundFileNode::database );

H3DSoundFileNode::FileReaderRegistration AudioFileReader::reader_registration(
                            "AudioFileReader",
                            &(newSoundFileNode< AudioFileReader >),
                            &AudioFileReader::supportsFileType 
                            );


unsigned int AudioFileReader::read( char *buffer, unsigned int size ) {
  int sample_format, sample_width;
  afGetVirtualSampleFormat( file, AF_DEFAULT_TRACK, 
                            &sample_format, &sample_width );
  unsigned int sample_size = nrChannels() * (sample_width /8 ); 
  unsigned int nr_samples = size / sample_size;
  int samples_read = afReadFrames( file, AF_DEFAULT_TRACK, buffer, 
                                   nr_samples );
  return samples_read * sample_size;
}

unsigned int AudioFileReader::load( const string &_url ) {
   url = _url;
   file = afOpenFile( url.c_str(), "r", NULL );
   if( file ) {
     int fileformat = afGetFileFormat( file, NULL );
     if( fileformat == AF_FILE_UNKNOWN  ) {
       afCloseFile( file );
       return 0;
     } else {
       if( isLittleEndian() )
         afSetVirtualByteOrder( file, AF_DEFAULT_TRACK, 
                                AF_BYTEORDER_LITTLEENDIAN );
       int sample_format, sample_width;
       afGetVirtualSampleFormat( file, AF_DEFAULT_TRACK, 
                                 &sample_format, &sample_width );
       // OpenAL only supports 8 bit unsigned and 16 bit signed data
       // so force it to be compatible.
       if( sample_width == 8 ) {
         afSetVirtualSampleFormat( file, AF_DEFAULT_TRACK, 
                                   AF_SAMPFMT_UNSIGNED, 8 );
       } else {
         afSetVirtualSampleFormat( file, AF_DEFAULT_TRACK, 
                                   AF_SAMPFMT_TWOSCOMP, 16 );
       }
     }
   } else {
     return 0;
   }
   return totalDataSize();
}

bool AudioFileReader::supportsFileType( const string &url ) {
  // Remove error handler to avoid annoying console messages
  afSetErrorHandler(NULL);
  
  AFfilehandle file;
  file = afOpenFile( url.c_str(), "r", NULL );
  if( file ) {
    int fileformat = afGetFileFormat( file, NULL );
    afCloseFile( file );
    return fileformat != AF_FILE_UNKNOWN;
  } else {
    return false;
  }
}

#endif
