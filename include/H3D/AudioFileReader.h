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
/// \file AudioFileReader.h
/// \brief Header file for AudioFileReader
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __AUDIOFILEREADER_H__
#define __AUDIOFILEREADER_H__

#include <H3D/H3DSoundFileNode.h>

#ifdef HAVE_LIBAUDIOFILE
#include <audiofile.h>

namespace H3D {

  /// \ingroup H3DNodes
  /// \class AudioFileReader
  /// \brief AudioFileReader uses the Audio File Library
  /// ( http://www.68k.org/~michael/audiofile/ ) to read sound data.
  ///
  /// It  supports AIFF/AIFF-C, WAVE, NeXT/Sun .snd/.au, Berkeley/IRCAM/CARL
  /// Sound File, Audio Visual Research, Amiga IFF/8SVX, and NIST SPHERE.
  /// Supported compression formats are currently G.711 mu-law and A-law and
  /// IMA and MS ADPCM.
  class H3DAPI_API AudioFileReader : public H3DSoundFileNode {
  public:

    /// Constructor.
    AudioFileReader() {
      file = NULL;
      type_name = "AudioFileReader";
    }

    /// Destructor.
    ~AudioFileReader() {
      if( file )
        afCloseFile( file );
    }

    /// Load a sound file from the given url that will be used to
    /// generate PCM data.
    unsigned int load( const string &_url );
    
    /// Reset the stream to the beginning of the sound stream.
    virtual void reset() {
      afSeekFrame( file, AF_DEFAULT_TRACK, 0 );
    }
      
    /// Returns the total size of the PCM data of the current stream.
    virtual unsigned int totalDataSize() {
      return afGetTrackBytes( file, AF_DEFAULT_TRACK );
    }
    
    /// Returns the number of channels per second for the current PCM 
    /// data stream.
    virtual unsigned int nrChannels() {
      return afGetChannels( file, AF_DEFAULT_TRACK );
    }
    
    /// Returns the number of samples per second for the current PCM 
    /// data stream.
    virtual unsigned int samplesPerSecond() {
      return (unsigned int) afGetRate( file, AF_DEFAULT_TRACK );
    }
    
    /// Returns the number of bits for a sample.
    virtual unsigned int bitsPerSample() {
      int sample_format, sample_width;
      afGetVirtualSampleFormat( file, AF_DEFAULT_TRACK, 
                                &sample_format, &sample_width );
      return sample_width;
    }

    /// The duration in seconds for the the PCM data.
    virtual H3DTime duration() {
      return afGetFrameCount(file, AF_DEFAULT_TRACK) /
        afGetRate(file, AF_DEFAULT_TRACK);
    }

    /// Read PCM data from the stream into the buffer.
    /// \param buffer A buffer to write data into.
    /// \param size The number of bytes of data to read.
    /// \returns The number of bytes written to the buffer.
    virtual unsigned int read( char *buffer, unsigned int size );

    /// Returns true if the node supports the filetype of the file
    /// specified by url.
    static bool supportsFileType( const string &url );
    
    /// The H3DNodeDatabase for this node
    static H3DNodeDatabase database;

    /// Register this node to the H3DSoundFileNodes available.
    static FileReaderRegistration reader_registration;

  protected:
    AFfilehandle file;
    string url;
  };
}

#endif
#endif
