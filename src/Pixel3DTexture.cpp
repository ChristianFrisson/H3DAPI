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
/// \file Pixel3DTexture.cpp
/// \brief CPP file for Pixel3DTexture, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <Pixel3DTexture.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase Pixel3DTexture::database( 
                                       "Pixel3DTexture", 
                                       &(newInstance<Pixel3DTexture>), 
                                       typeid( Pixel3DTexture ),
                                       &X3DTexture3DNode::database );

namespace Pixel3DTextureInternals {
  FIELDDB_ELEMENT( Pixel3DTexture, image, INPUT_OUTPUT );
}


Pixel3DTexture::Pixel3DTexture( 
                           Inst< DisplayList > _displayList,
                           Inst< SFNode       >  _metadata,
                           Inst< SFBool       >  _repeatS,
                           Inst< SFBool       >  _repeatT,
                           Inst< SFBool       >  _repeatR,
                           Inst< SFBool       >  _scaleToP2,
                           Inst< SFImage      > _image,
                           Inst< SFTextureProperties > _textureProperties ) :
  X3DTexture3DNode( _displayList, _metadata, _repeatS, _repeatT,
                    _repeatR, _scaleToP2, _image, _textureProperties ) {
  type_name = "Pixel3DTexture";
  database.initFields( this );
}

