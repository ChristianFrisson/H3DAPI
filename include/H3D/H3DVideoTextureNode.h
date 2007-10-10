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
/// \file H3DVideoTextureNode.h
/// \brief Header file for H3DVideoTextureNode.
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __H3DVIDEOTEXTURENODE_H__
#define __H3DVIDEOTEXTURENODE_H__

#include <H3D/X3DTexture2DNode.h>
#include <H3D/H3DVideoClipDecoderNode.h>

namespace H3D {
  /// \ingroup AbstractNodes 
  /// \class H3DVideoTextureNode
  /// \brief H3DVideoTextureNode is a virtual base class for classes
  /// using video as a texture.
  /// Subclasses must set the decoder
  class H3DAPI_API H3DVideoTextureNode : public X3DTexture2DNode {
  public:
    
    /// Constructor.
    H3DVideoTextureNode( 
                 Inst< DisplayList > _displayList = 0,
                 Inst< SFNode      > _metadata    = 0,
                 Inst< SFBool      > _repeatS     = 0,
                 Inst< SFBool      > _repeatT     = 0,
                 Inst< SFBool      > _scaleToP2   = 0,
                 Inst< SFImage     > _image       = 0,
                 Inst< SFTextureProperties > _textureProperties = 0 ):
      X3DTexture2DNode( _displayList, _metadata, _repeatS, _repeatT,
                        _scaleToP2, _image, _textureProperties ),
      frame_bytes_allocated( 0 ) {}

    /// Traverse the senegraph. 
    virtual void traverseSG( TraverseInfo &ti ) {
      // break the display list cache if we have a new frame
      if( decoder.get() && decoder->haveNewFrame() )
        repeatS->touch();
      X3DTexture2DNode::traverseSG( ti );
    }

    /// Render the texture.
    virtual void render();

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  protected:
    /// The decoder to use for the texture. Must be set by subclasses
    /// in order for the video to be rendered.
    AutoRef< H3DVideoClipDecoderNode > decoder;

    /// The number of bytes currently allocated for frame data.
    unsigned int frame_bytes_allocated;
  };
}

#endif
