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
/// \file FunctionPlotTexture.cpp
/// \brief CPP file for FunctionPlotTexture, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/FunctionPlotTexture.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase FunctionPlotTexture::database( "FunctionPlotTexture", 
                                        &(newInstance<FunctionPlotTexture>), 
                                        typeid( FunctionPlotTexture ),
                                        &X3DTexture2DNode::database );

namespace FunctionPlotTextureInternals {
  FIELDDB_ELEMENT( FunctionPlotTexture, function, INPUT_OUTPUT );
  FIELDDB_ELEMENT( FunctionPlotTexture, backgroundColor, INPUT_OUTPUT );
  FIELDDB_ELEMENT( FunctionPlotTexture, plotColor, INPUT_OUTPUT );
  FIELDDB_ELEMENT( FunctionPlotTexture, xRange, INPUT_OUTPUT );
  FIELDDB_ELEMENT( FunctionPlotTexture, yRange, INPUT_OUTPUT );
  FIELDDB_ELEMENT( FunctionPlotTexture, xResolution, INPUT_OUTPUT );
  FIELDDB_ELEMENT( FunctionPlotTexture, yResolution, INPUT_OUTPUT );
}


FunctionPlotTexture::FunctionPlotTexture( 
                           Inst< DisplayList > _displayList,
                           Inst< SFNode        > _metadata,
                           Inst< SFBool        > _repeatS,
                           Inst< SFBool        > _repeatT,
                           Inst< SFBool        > _scaleToP2,
                           Inst< SFImage       > _image,
                           Inst< SFTextureProperties > _textureProperties,
                           Inst< SFFunctionNode > _function  ,
                            Inst< SFColorRGBA > _backgroundColor,
                            Inst< SFColorRGBA > _plotColor    ,
                            Inst< SFVec2f     > _xRange       ,
                            Inst< SFVec2f     > _yRange       ,
                            Inst< SFInt32     > _xResolution  ,
                            Inst< SFInt32     > _yResolution    ) :
  X3DTexture2DNode( _displayList, _metadata, _repeatS, _repeatT,
                    _scaleToP2, _image, _textureProperties ),
  function( _function ),
  backgroundColor( _backgroundColor ),
  plotColor( _plotColor ),
  xRange( _xRange ),
  yRange( _yRange ),
  xResolution( _xResolution ),
  yResolution( _yResolution ) {

  type_name = "FunctionPlotTexture";
  database.initFields( this );

  backgroundColor->setValue( RGBA( 1, 1, 1, 1 ) );
  plotColor->setValue( RGBA( 0, 0, 0, 1 ) );
  xRange->setValue( Vec2f( 0, 1 ) );
  yRange->setValue( Vec2f( 0, 1 ) );
  xResolution->setValue( 128 );
  yResolution->setValue( 128 );

  function->route( image );
  backgroundColor->route( image );
  plotColor->route( image );
  xRange->route( image );
  yRange->route( image );
  xResolution->route( image );
  yResolution->route( image );
}

void FunctionPlotTexture::SFImage::update() {
  H3DFunctionNode *function = static_cast< SFFunctionNode * >( routes_in[0] )->getValue();
  const RGBA &bg_color = static_cast< SFColorRGBA * >( routes_in[1] )->getValue();
  const RGBA &plot_color = static_cast< SFColorRGBA * >( routes_in[2] )->getValue();
  const Vec2f &x_range = static_cast< SFVec2f * >( routes_in[3] )->getValue();
  const Vec2f &y_range = static_cast< SFVec2f * >( routes_in[4] )->getValue();
  H3DInt32 x_resolution = static_cast< SFInt32 * >( routes_in[5] )->getValue();
  H3DInt32 y_resolution = static_cast< SFInt32 * >( routes_in[6] )->getValue();

  if( !value.get() || 
      (unsigned int)x_resolution != value->width() || 
      (unsigned int)y_resolution != value->height() ) {
    unsigned char *data = new unsigned char[ 4 * x_resolution * y_resolution ];
    unsigned int bytes_used = 4 * x_resolution * y_resolution;
    for( unsigned int i = 0; i < bytes_used; i+=4 ) {
      data[i]   = (unsigned char) ( bg_color.r * 255 );
      data[i+1] = (unsigned char) ( bg_color.g * 255 );
      data[i+2] = (unsigned char) ( bg_color.b * 255 );
      data[i+3] = (unsigned char) ( bg_color.a * 255 );
    }
    value.reset( new PixelImage( x_resolution,
                            y_resolution,
                            1,
                            32,
                            PixelImage::RGBA,
                            PixelImage::UNSIGNED,
                            data ) );
  }

  if( !function ) {
    for( unsigned int x = 0; x < (unsigned int)x_resolution; ++x ) {
      for( unsigned int y = 0; y < (unsigned int)y_resolution; ++y ) {
        setPixel( x, y, bg_color );
      }
    }
  } else {
    
    H3DFloat step_length = (x_range.y - x_range.x) / x_resolution;
    for( unsigned int i = 0; i < (unsigned int)x_resolution; ++i ) {
      H3DDouble x = x_range.x +  i * step_length;
      H3DDouble y = function->evaluate( &x );
      
      H3DInt32 y_pixel = (H3DInt32 ) ((y - y_range.x)/(y_range.y-y_range.x) * (y_resolution - 1 ));

      if( y_pixel < 0 || y_pixel >= y_resolution ) {
        for( unsigned int j = 0; j < (unsigned int)y_resolution; ++j ) {
          setPixel( i, j, bg_color );
        }
      } else {
        for( unsigned int j = 0; j < (unsigned int) y_pixel; ++j )
          setPixel( i, j, bg_color );
        setPixel( i, y_pixel, RGBA( plot_color ) );
        for( unsigned int j = y_pixel+1; j < (unsigned int )y_resolution; ++j )
          setPixel( i, j, bg_color );
      }
    }
  }
}




