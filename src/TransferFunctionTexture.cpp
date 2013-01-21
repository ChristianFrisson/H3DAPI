//////////////////////////////////////////////////////////////////////////////
//    Copyright 2004-2013, SenseGraphics AB
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
/// \file TransferFunctionTexture.cpp
/// \brief CPP file for TransferFunctionTexture
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/TransferFunctionTexture.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase TransferFunctionTexture::database( "TransferFunctionTexture", 
                                            &(newInstance<TransferFunctionTexture>), 
                                            typeid( TransferFunctionTexture ),
                                            &X3DTexture2DNode::database );

namespace TransferFunctionTextureInternals {
  FIELDDB_ELEMENT( TransferFunctionTexture, redFunction, INPUT_OUTPUT );
  FIELDDB_ELEMENT( TransferFunctionTexture, greenFunction, INPUT_OUTPUT );
  FIELDDB_ELEMENT( TransferFunctionTexture, blueFunction, INPUT_OUTPUT );
  FIELDDB_ELEMENT( TransferFunctionTexture, alphaFunction, INPUT_OUTPUT );
}

TransferFunctionTexture::TransferFunctionTexture( 
                           Inst< DisplayList        > _displayList,
                           Inst< SFNode             > _metadata,
                           Inst< SFBool   >  _repeatS,
                           Inst< SFBool   >  _repeatT,
                           Inst< SFBool   >  _scaleToP2,
                           Inst< SFImage            > _image,
                           Inst< SFTextureProperties > _textureProperties,
                           Inst< SFFunctionNode > _redFunction,
                           Inst< SFFunctionNode > _greenFunction,
                           Inst< SFFunctionNode > _blueFunction,
                           Inst< SFFunctionNode > _alphaFunction ) :
  X3DTexture2DNode( _displayList, _metadata, _repeatS, _repeatT,
                    _scaleToP2, _image, _textureProperties ),
  redFunction( _redFunction ),
  greenFunction( _greenFunction ),
  blueFunction( _blueFunction ),
  alphaFunction( _alphaFunction ) {

  type_name = "TransferFunctionTexture";
  database.initFields( this );

  redFunction->route( image );
  greenFunction->route( image );
  blueFunction->route( image );
  alphaFunction->route( image );
}

void TransferFunctionTexture::SFImage::update() {
  H3DFunctionNode *red_function = 
    static_cast< SFFunctionNode * >( routes_in[0] )->getValue();
  H3DFunctionNode *green_function = 
    static_cast< SFFunctionNode * >( routes_in[1] )->getValue();
  H3DFunctionNode *blue_function = 
    static_cast< SFFunctionNode * >( routes_in[2] )->getValue();
  H3DFunctionNode *alpha_function = 
    static_cast< SFFunctionNode * >( routes_in[3] )->getValue();

  unsigned char lut[256*4];
  for( int i = 0 ; i < 256 ; i++ ){
    lut[i*4  ] = (unsigned char) (red_function ?  
                                  red_function->evaluate( i / 255.0 ) * 255 :
                                  i ) ;
    lut[i*4+1] = (unsigned char) (green_function ? 
                                  green_function->evaluate( i / 255.0 ) * 255: 
                                  i );
    lut[i*4+2] = (unsigned char) (blue_function ? 
                                  blue_function->evaluate( i / 255.0 ) * 255 : 
                                  i );
    lut[i*4+3] = (unsigned char) (alpha_function ? 
                                  alpha_function->evaluate( i / 255.0 ) * 255 : 
                                  i );
  }
  
  value = new PixelImage( 256, 1, 1, 32, 
                          PixelImage::RGBA, PixelImage::UNSIGNED,
                          (unsigned char *)lut, true );
}


