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
/// \file NoiseTexture.cpp
/// \brief CPP file for NoiseTexture, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/NoiseTexture.h>
#include <H3D/SimplexNoise.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase NoiseTexture::database( "NoiseTexture", 
                                            &(newInstance<NoiseTexture>), 
                                            typeid( NoiseTexture ),
                                            &X3DTexture2DNode::database );

namespace NoiseTextureInternals {
  FIELDDB_ELEMENT( NoiseTexture, frequency, INPUT_OUTPUT );
  FIELDDB_ELEMENT( NoiseTexture, lacunarity, INPUT_OUTPUT );
  FIELDDB_ELEMENT( NoiseTexture, octaveCount, INPUT_OUTPUT );
  FIELDDB_ELEMENT( NoiseTexture, persistence, INPUT_OUTPUT );
  FIELDDB_ELEMENT( NoiseTexture, seed, INPUT_OUTPUT );
  FIELDDB_ELEMENT( NoiseTexture, width, INPUT_OUTPUT );
  FIELDDB_ELEMENT( NoiseTexture, height, INPUT_OUTPUT );
  FIELDDB_ELEMENT( NoiseTexture, type, INPUT_OUTPUT );
  FIELDDB_ELEMENT( NoiseTexture, tileable, INPUT_OUTPUT );
}


NoiseTexture::NoiseTexture( 
                           Inst< DisplayList > _displayList,
                           Inst< SFNode      > _metadata,
                           Inst< SFImage     > _image,
                           Inst< SFTextureProperties > _textureProperties,
                           Inst< SFInt32     > _width,
                           Inst< SFInt32     > _height,
                           Inst< SFFloat     > _frequency,
                           Inst< SFFloat     > _lacunarity,
                           Inst< SFInt32     > _octaveCount,
                           Inst< SFFloat     > _persistence,
                           Inst< SFInt32     > _seed,
                           Inst< SFString    > _type,
                           Inst< SFBool      > _tileable ) :
  X3DTexture2DNode( _displayList, _metadata, 0, 0,
                    0, _image, _textureProperties ),
  width( _width ),
  height( _height ),
  frequency( _frequency ),
  lacunarity( _lacunarity ),
  octaveCount( _octaveCount ),
  persistence( _persistence ),
  seed( _seed ),
  type( _type ),
  tileable( _tileable ) {

  type_name = "NoiseTexture";
  database.initFields( this );

  frequency->route( image );
  lacunarity->route( image );
  octaveCount->route( image );
  persistence->route( image );
  seed->route( image );
  width->route( image );
  height->route( image );
  type->route( image );
  tileable->route( image );

  frequency->setValue( 1.0 );
  lacunarity->setValue( 2.0 );
  octaveCount->setValue( 6 );
  persistence->setValue( 0.5 );
  seed->setValue( 0 );
  width->setValue( 512 );
  height->setValue( 512 );
  type->addValidValue( "PERLIN" );
  type->addValidValue( "SIMPLEX" );
  type->setValue( "PERLIN" );
  tileable->setValue( false );
}

void NoiseTexture::SFImage::update() {
  float frequency = static_cast< SFFloat * >(routes_in[0])->getValue(); 
  float lacunarity = static_cast< SFFloat * >(routes_in[1])->getValue(); 
  int octaveCount = static_cast< SFInt32 * >(routes_in[2])->getValue(); 
  float persistence = static_cast< SFFloat * >(routes_in[3])->getValue(); 
  int seed = static_cast< SFInt32 * >(routes_in[4])->getValue();
  int width = static_cast< SFInt32 * >(routes_in[5])->getValue();
  int height = static_cast< SFInt32 * >(routes_in[6])->getValue();
  const string & type = static_cast< SFString * >(routes_in[7])->getValue();
  bool tileable = static_cast< SFBool * >(routes_in[8])->getValue();

  bool simplexNoise = type == "SIMPLEX";

  unsigned char *data = new unsigned char[ width * height ];

  int x, y;
  double perlinValue;
  H3DFloat perlinGrayValue;

  for( y = 0; y < height; ++y ) {
    for( x = 0; x < width; ++x) {
      double x_norm = x/(width-1.0);
      double y_norm = y/(height-1.0);
      if( !simplexNoise ) {
      } else {
        if( tileable ) {
          perlinValue = NoiseTexture::simplexNoiseTileable( x_norm, y_norm, frequency, lacunarity, octaveCount, persistence );
        } else {
          perlinValue = NoiseTexture::simplexNoise( x_norm, y_norm, frequency, lacunarity, octaveCount, persistence );
        }
      }
      // just in case clamp to -1.0 or +1.0, because of a warning in the libnoise documentation
      if( perlinValue < -1.0 )
        perlinValue = -1.0;
      if( perlinValue > 1.0 )
        perlinValue = 1.0;
      // shift to interval 0.0 to 1.0
      perlinGrayValue = 0.5 * perlinValue + 0.5;

      data[y*width + x ] = 255 * perlinGrayValue;
    }
  }
  value = new PixelImage( width, height, 1, 8, 
                          PixelImage::LUMINANCE,
                          PixelImage::UNSIGNED, data, false, Vec3f(0.01, 0.01, 0.01) );
}


H3DFloat NoiseTexture::simplexNoise( H3DFloat x, H3DFloat y, H3DFloat frequency, H3DFloat lacunarity, H3DFloat octaves, H3DFloat persistence ) {
  float val = 0;
  float amplitude = 1.0f;
  float x_v = x * frequency;
  float y_v = y * frequency;
 
  for (int n = 0; n < octaves; n++) {
     val += SimplexNoise1234::noise( x_v+5*frequency,y_v+5*frequency ) * amplitude;
     x_v *= lacunarity;
     y_v *= lacunarity;
     amplitude *= persistence;
  }
  return val;
}


H3DFloat NoiseTexture::simplexNoiseTileable( H3DFloat x, H3DFloat y, H3DFloat frequency, H3DFloat lacunarity, H3DFloat octaves, H3DFloat persistence ) {

   double Fxy = simplexNoise(x, y, frequency, lacunarity, octaves, persistence );
   double Flxy = simplexNoise(x-1,y, frequency, lacunarity, octaves, persistence );
   double Flxly = simplexNoise(x-1,y - 1,  frequency, lacunarity, octaves, persistence );
   double Fxly = simplexNoise(x, y - 1, frequency, lacunarity, octaves, persistence );

   H3DFloat v = 
            Fxy  * (1-x) * (1-y ) +
            Flxy * x * (1-y) +
            Flxly * x * y +
            Fxly * (1 - x) * y; 
   return v;
}

