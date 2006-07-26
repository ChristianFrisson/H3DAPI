//////////////////////////////////////////////////////////////////////////////
//    Copyright 2004, SenseGraphics AB
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

#include "X3DTexture2DNode.h"
#include "H3DVideoClipDecoderNode.h"

namespace H3D {
  /// \ingroup Nodes 
  /// \class H3DVideoTextureNode
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
                 Inst< SFBool      > _interpolate = 0,
                 Inst< SFBool      > _scaleFrameToFillTexture = 0 ):
      scaleFrameToFillTexture( _scaleFrameToFillTexture ),
      frame_bytes_allocated( 0 ) {
      scaleFrameToFillTexture->setValue( false );
    }

    virtual void traverseSG( TraverseInfo &ti ) {
      if( decoder.get() && decoder->haveNewFrame() )
        repeatS->touch();
      X3DTexture2DNode::traverseSG( ti );
    }

    virtual void render();

    auto_ptr< SFBool > scaleFrameToFillTexture;

    static H3DNodeDatabase database;
  protected:
    AutoRef< H3DVideoClipDecoderNode > decoder;
    unsigned int frame_bytes_allocated;
  };
}

#endif
