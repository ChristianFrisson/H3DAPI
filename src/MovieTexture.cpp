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
/// \file MovieTexture.cpp
/// \brief CPP file for MovieTexture.
///
//
//////////////////////////////////////////////////////////////////////////////


#include <H3D/MovieTexture.h>

using namespace H3D;

H3DNodeDatabase MovieTexture::database( 
        "MovieTexture", 
        &(newInstance< MovieTexture > ),
        typeid( MovieTexture ),
        &H3DVideoTextureNode::database 
        );

namespace X3DTexture2DNodeInternals {
  //  FIELDDB_ELEMENT( X3DTexture2DNode, interpolate, INPUT_OUTPUT );
}


MovieTexture::MovieTexture( Inst< DisplayList > _displayList,
                            Inst< SFNode  > _metadata,
                            Inst< SFBool  > _repeatS,
                            Inst< SFBool  > _repeatT,
                            Inst< SFBool  > _scaleToP2,
                            Inst< SFImage > _image,
                            Inst< SFBool  > _interpolate ) :
  H3DVideoTextureNode( _displayList, _metadata, _repeatS,
                       _repeatT, _scaleToP2, _image ) {
  type_name = "MovieTexture";
  database.initFields( this );

//  scaleToPowerOfTwo->setValue( false );
}



