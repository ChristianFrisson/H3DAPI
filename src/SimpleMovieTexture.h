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
/// \file SimpleMovieTexture.h
/// \brief Header file for SimpleMovieTexture.
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __SIMPLEMOVIETEXTURE_H__
#define __SIMPLEMOVIETEXTURE_H__

#include "H3DVideoTextureNode.h"
#include "X3DUrlObject.h"
#include "SFFloat.h"
#include "SFTime.h"
#include "SFInt32.h"

namespace H3D {
  /// \ingroup Nodes 
  /// \class SimpleMovieTexture
  class H3DAPI_API SimpleMovieTexture : public H3DVideoTextureNode,
                                        public X3DUrlObject{
  public:

    class DecoderManager: public TypedField< AutoUpdate< SFBool >,
                          Types< SFBool, SFBool, SFBool, SFBool, MFString > > {
      virtual void update();
    };

    

    /// Constructor.
    SimpleMovieTexture ( 
                 Inst< DisplayList  > _displayList  = 0,
                 Inst< SFNode       > _metadata     = 0,
                 Inst< SFBool       > _repeatS      = 0,
                 Inst< SFBool       > _repeatT      = 0,
                 Inst< SFBool       > _scaleToP2    = 0,
                 Inst< SFImage      > _image        = 0,
                 Inst< SFBool       > _interpolate  = 0,
                 Inst< SFBool       > _play         = 0,
                 Inst< SFBool       > _stop         = 0,
                 Inst< SFBool       > _pause        = 0,
                 Inst< SFTime      > _length       = 0,
                 Inst< SFFloat     > _frameRate    = 0,
                 Inst< SFBool      > _playAudio    = 0,
                 Inst< SFBool      > _loop         = 0,
                 Inst< SFInt32     > _width        = 0,
                 Inst< SFInt32     > _height       = 0,
                 Inst< MFString >  _url             = 0 );

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;

    auto_ptr< SFBool > play;
    auto_ptr< SFBool > stop;
    auto_ptr< SFBool > pause;
    auto_ptr< SFTime > length;
    auto_ptr< SFFloat > frameRate;
    auto_ptr< SFBool > playAudio;
    auto_ptr< SFBool > loop;
    auto_ptr< SFInt32 > width;
    auto_ptr< SFInt32 > height;
    auto_ptr< DecoderManager > decoderManager;
    
  };
}

#endif
