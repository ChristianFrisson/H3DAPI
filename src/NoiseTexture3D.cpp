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
/// \file NoiseTexture3D.cpp
/// \brief CPP file for NoiseTexture3D.
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/NoiseTexture3D.h>
#include <H3D/SimplexNoise.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase NoiseTexture3D::database( "NoiseTexture3D", 
                                            &(newInstance<NoiseTexture3D>), 
                                            typeid( NoiseTexture3D ),
                                            &X3DTexture3DNode::database );

namespace NoiseTexture3DInternals {
  FIELDDB_ELEMENT( NoiseTexture3D, frequency, INPUT_OUTPUT );
  FIELDDB_ELEMENT( NoiseTexture3D, lacunarity, INPUT_OUTPUT );
  FIELDDB_ELEMENT( NoiseTexture3D, octaveCount, INPUT_OUTPUT );
  FIELDDB_ELEMENT( NoiseTexture3D, persistence, INPUT_OUTPUT );
  FIELDDB_ELEMENT( NoiseTexture3D, seed, INPUT_OUTPUT );
  FIELDDB_ELEMENT( NoiseTexture3D, width, INPUT_OUTPUT );
  FIELDDB_ELEMENT( NoiseTexture3D, height, INPUT_OUTPUT );
  FIELDDB_ELEMENT( NoiseTexture3D, depth, INPUT_OUTPUT );
  FIELDDB_ELEMENT( NoiseTexture3D, type, INPUT_OUTPUT );
  FIELDDB_ELEMENT( NoiseTexture3D, tileable, INPUT_OUTPUT );
}


NoiseTexture3D::NoiseTexture3D( 
                           Inst< DisplayList > _displayList,
                           Inst< SFNode      > _metadata,
                           Inst< SFImage     > _image,
                           Inst< SFTextureProperties > _textureProperties,
                           Inst< SFInt32     > _width,
                           Inst< SFInt32     > _height,
                           Inst< SFInt32     > _depth,
                           Inst< SFFloat     > _frequency,
                           Inst< SFFloat     > _lacunarity,
                           Inst< SFInt32     > _octaveCount,
                           Inst< SFFloat     > _persistence,
                           Inst< SFInt32     > _seed,
                           Inst< SFString    > _type,
                           Inst< SFBool      > _tileable ) :
  X3DTexture3DNode( _displayList, _metadata, 0, 0, 0,
                    0, _image, _textureProperties ),
  width( _width ),
  height( _height ),
  depth( _depth ),
  frequency( _frequency ),
  lacunarity( _lacunarity ),
  octaveCount( _octaveCount ),
  persistence( _persistence ),
  seed( _seed ),
  type( _type ),
  tileable( _tileable ) {

  type_name = "NoiseTexture3D";
  database.initFields( this );

  frequency->route( image );
  lacunarity->route( image );
  octaveCount->route( image );
  persistence->route( image );
  seed->route( image );
  width->route( image );
  height->route( image );
  depth->route( image );
  type->route( image );
  tileable->route( image );

  frequency->setValue( 1.0 );
  lacunarity->setValue( 2.0 );
  octaveCount->setValue( 6 );
  persistence->setValue( 0.5 );
  seed->setValue( 0 );
  width->setValue( 256 );
  height->setValue( 256 );
  depth->setValue( 256 );
 
  type->addValidValue( "PERLIN" );
  type->addValidValue( "SIMPLEX" );
  type->setValue( "PERLIN" );
  tileable->setValue( false );
}

void NoiseTexture3D::SFImage::update() {
  H3DFloat _frequency = static_cast< SFFloat * >(routes_in[0])->getValue(); 
  H3DFloat _lacunarity = static_cast< SFFloat * >(routes_in[1])->getValue(); 
  H3DInt32 _octaveCount = static_cast< SFInt32 * >(routes_in[2])->getValue(); 
  H3DFloat _persistence = static_cast< SFFloat * >(routes_in[3])->getValue(); 
  H3DInt32 _seed = static_cast< SFInt32 * >(routes_in[4])->getValue();
  H3DInt32 _width = static_cast< SFInt32 * >(routes_in[5])->getValue();
  H3DInt32 _height = static_cast< SFInt32 * >(routes_in[6])->getValue();
  H3DInt32 _depth = static_cast< SFInt32 * >(routes_in[7])->getValue();
  const string & _type = static_cast< SFString * >(routes_in[8])->getValue();
  bool _tileable = static_cast< SFBool * >(routes_in[9])->getValue();

  bool simplexNoise = _type == "SIMPLEX";

  
  unsigned char *data = new unsigned char[ _width * _height * _depth ];

  int x, y, z;
  H3DFloat perlinValue;
  H3DFloat perlinGrayValue;
  unsigned int pixel = 0;

  H3DFloat width_minus_1 = H3DFloat( _width-1 );
  H3DFloat height_minus_1 = H3DFloat( _height-1 );
  H3DFloat depth_minus_1 = H3DFloat( _depth-1 );
  for( z = 0; z < _depth; ++z ) {
    for( y = 0; y < _height; ++y) {
      for( x = 0; x < _width; ++x,pixel++) {
        H3DFloat x_norm = x/width_minus_1;
        H3DFloat y_norm = y/height_minus_1;
        H3DFloat z_norm = z/depth_minus_1;

        if( !simplexNoise ) {
      } else {
          if( _tileable ) {
            perlinValue = NoiseTexture3D::simplexNoiseTileable( x_norm, y_norm, z_norm, _frequency, _lacunarity, H3DFloat( _octaveCount ), _persistence );
          } else {
            perlinValue = NoiseTexture3D::simplexNoise( x_norm, y_norm, z_norm, _frequency, _lacunarity, H3DFloat( _octaveCount ), _persistence );
          }
        }
        
        // just in case clamp to -1.0 or +1.0, because of a warning in the libnoise documentation
        if( perlinValue < -1.0 )
          perlinValue = -1.0;
        if( perlinValue > 1.0 )
          perlinValue = 1.0;

        // shift to interval 0.0 to 1.0
        perlinGrayValue = H3DFloat( 0.5 * perlinValue + 0.5 );

        data[pixel] = (unsigned char)(255 * perlinGrayValue);
      }
    }
  }
  value = new PixelImage( _width, _height, _depth, 8,
                          PixelImage::LUMINANCE,
                          PixelImage::UNSIGNED, data, false, Vec3f(0.01, 0.01, 0.01) );
}


H3DFloat NoiseTexture3D::simplexNoise( H3DFloat x, H3DFloat y, H3DFloat z, H3DFloat frequency, H3DFloat lacunarity, H3DFloat octaves, H3DFloat persistence ) {
  float val = 0;
  float amplitude = 1.0f;
  float x_v = x * frequency;
  float y_v = y * frequency;
  float z_v = z * frequency;

  for (int n = 0; n < octaves; n++) {
    val += SimplexNoise1234::noise( x_v+5*frequency,y_v+5*frequency, z_v+5*frequency ) * amplitude;
    x_v *= lacunarity;
    y_v *= lacunarity;
    z_v *= lacunarity;
    amplitude *= persistence;
  }
  return val;
}


H3DFloat NoiseTexture3D::simplexNoiseTileable( H3DFloat x, H3DFloat y, H3DFloat z, H3DFloat frequency, H3DFloat lacunarity, H3DFloat octaves, H3DFloat persistence ) {

   H3DFloat Fhhh = simplexNoise(x,   y,   z, frequency, lacunarity, octaves, persistence );
   H3DFloat Flhh = simplexNoise(x-1, y,   z, frequency, lacunarity, octaves, persistence );
   H3DFloat Fhlh = simplexNoise(x,   y-1, z, frequency, lacunarity, octaves, persistence );
   H3DFloat Fllh = simplexNoise(x-1, y-1, z, frequency, lacunarity, octaves, persistence );
   
   H3DFloat Fhhl = simplexNoise(x, y, z-1, frequency, lacunarity, octaves, persistence );
   H3DFloat Flhl = simplexNoise(x-1, y, z-1, frequency, lacunarity, octaves, persistence );
   H3DFloat Fhll = simplexNoise(x, y-1, z-1, frequency, lacunarity, octaves, persistence );
   H3DFloat Flll = simplexNoise(x-1, y-1, z-1, frequency, lacunarity, octaves, persistence );

   H3DFloat v  = 
     Fhhh * (1-x) * (1-y ) * (1-z) +
     Flhh * x       * (1-y)  * (1-z) +
     Fhlh * (1-x) * y        * (1-z) +
     Fllh * x       * y        * (1-z) +
     Fhhl * (1-x) * (1-y ) * (z) +
     Flhl * x       * (1-y)  * (z) +
     Fhll * (1-x) * y        * (z) +
     Flll * x       * y        * (z);

  return v;
}


