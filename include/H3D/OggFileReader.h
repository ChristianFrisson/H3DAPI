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
/// \file OggFileReader.h
/// \brief Header file for OggFileReader, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////

#ifndef __OGGFILEREADER_H__
#define __OGGFILEREADER_H__

#include <H3D/H3DSoundFileNode.h>

#ifdef HAVE_LIBVORBIS
#include <vorbis/vorbisfile.h>

namespace H3D {

  /// \ingroup H3DNodes
  /// \class OggFileReader
  /// \brief OggFileReader uses the libVorbis (http://www.vorbis.com)
  /// decoder library to support
  /// Ogg Vorbis files.
  class H3DAPI_API OggFileReader : public H3DSoundFileNode {
  public:

    /// Constructor.
    OggFileReader() {
      should_clear = false;
      type_name = "OggFileReader";
    }

    /// Destructor.
    ~OggFileReader() {
      if( should_clear )
        ov_clear( &ogg_file );
    }

    /// Load a sound file from the given url that will be used to
    /// generate PCM data.
    unsigned int load( const string &_url );
          
    /// The duration in seconds for the the PCM data.
    virtual H3DTime duration() {
      return ov_time_total( &ogg_file, -1 );
    }
    
    /// Reset the stream to the beginning of the sound stream.
    virtual void reset() {
      ov_raw_seek( &ogg_file, 0 );
    }
    
    /// Returns the total size of the PCM data of the current stream.
    virtual unsigned int totalDataSize() {
      return (unsigned int)( ov_pcm_total( &ogg_file, -1 ) * bitsPerSample() / 8 );
    }
      
    /// Returns the number of channels per second for the current PCM 
    /// data stream.
    virtual unsigned int nrChannels() {
      return info->channels;
    }
    
    /// Returns the number of bits for a sample.
    virtual unsigned int bitsPerSample() {
      return 16;
    }

    /// Read PCM data from the stream into the buffer.
    /// \param buffer A buffer to write data into.
    /// \param size The number of bytes of data to read.
    /// \returns The number of bytes written to the buffer.
    virtual unsigned int read( char *buffer, unsigned int size );
    
    /// Returns the number of samples per second for the current PCM 
    /// data stream.
    virtual unsigned int samplesPerSecond() {
      return info->rate;
    }
    
    /// Returns true if the node supports the filetype of the file
    /// specified by url.
    static bool supportsFileType( const string &url );

    /// The H3DNodeDatabase for this node
    static H3DNodeDatabase database;

    /// Register this node to the H3DSoundFileNodes available.
    static FileReaderRegistration reader_registration;

  protected:
    OggVorbis_File ogg_file;
    // used to know if ov_clear should be called
    // there was a problem when creating and destroying OggFileReader
    // without using it.
    bool should_clear;
    vorbis_info* info;
    vorbis_comment *comment;
    string url;
  };
}

#endif
#endif
