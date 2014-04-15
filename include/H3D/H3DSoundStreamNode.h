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
/// \file H3DSoundStreamNode.h
/// \brief Header file for H3DSoundStreamNode, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __H3DSOUNDSTREAMNODE_H__
#define __H3DSOUNDSTREAMNODE_H__

#include <H3D/Node.h>

namespace H3D {

  /// \ingroup AbstractNodes
  /// \class H3DSoundStreamNode
  /// \brief This abstract node type is used to derive node types that
  /// can stream PCM sound data.
  ///
  /// 
  class H3DAPI_API H3DSoundStreamNode : public Node {
  public:
    
    /// Reset the stream to the beginning of the sound stream.
    virtual void reset() = 0;

    /// Read PCM data from the stream into the buffer.
    /// \param buffer A buffer to write data into.
    /// \param size The number of bytes of data to read.
    /// \returns The number of bytes written to the buffer.
    virtual unsigned int read( char *buffer, unsigned int size ) = 0; 

    /// Returns the number of bits for a sample.
    virtual unsigned int bitsPerSample() = 0;

    /// Returns the number of samples per second for the current PCM 
    /// data stream.
    virtual unsigned int samplesPerSecond() = 0;

    /// Returns the number of channels per second for the current PCM 
    /// data stream.
    virtual unsigned int nrChannels() = 0;

    /// Returns the total size of the PCM data of the current stream.
    virtual unsigned int totalDataSize() = 0;

    /// The duration in seconds for the the PCM data.
    virtual H3DTime duration() = 0;
  };
}

#endif
