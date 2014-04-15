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
/// \file H3DImageObject.cpp
/// \brief CPP file for H3DImageObject
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/H3DImageObject.h>

using namespace H3D;

void H3DImageObject::SFImage::setPixel( unsigned int x, unsigned int y, 
                                          const RGBA &color ) {
  if( value.get() ) {
    if( x_min == -1 ) {
      x_min = x_max = x;
      y_min = y_max = y;
      z_min = z_max = 0;
    } else {
      if( x < (unsigned int)x_min ) x_min = x;
      else if( x > (unsigned int)x_max ) x_max = x;
    
      if( y < (unsigned int)y_min ) y_min = y;
      else if( y > (unsigned int)y_max ) y_max = y;
    }
    
    value->setPixel( color, x, y );
  }
}

void H3DImageObject::SFImage::setPixel( const Vec2f &tc,
                                          const RGBA &color ) {
  if( value.get() ) {
    setPixel( Vec3f( tc.x, tc.y, 0 ),
              color );
  }
}

void H3DImageObject::SFImage::setPixel( unsigned int x, 
                                        unsigned int y,
                                        unsigned int z,
                                        const RGBA &color ) {
  if( value.get() ) {
    if( x_min == -1 ) {
      x_min = x_max = x;
      y_min = y_max = y;
      z_min = z_max = z;
    } else {
      if( x < (unsigned int)x_min ) x_min = x;
      else if( x > (unsigned int)x_max ) x_max = x;
    
      if( y < (unsigned int)y_min ) y_min = y;
      else if( y > (unsigned int)y_max ) y_max = y;

      if( z < (unsigned int)z_min ) z_min = z;
      else if( z > (unsigned int)z_max ) z_max = z;
    }
    
    value->setPixel( color, x, y, z );
  }
}

void H3DImageObject::SFImage::setPixel( const Vec3f &tc,
                                        const RGBA &color ) {
  if( value.get() ) {
    unsigned int width = value->width();
    unsigned int height = value->height();
    unsigned int depth = value->depth();

    // find the pixel corresponding to the texture coordinate.
    H3DFloat x = tc.x * width;
    H3DFloat y = tc.y * height;
    H3DFloat z = tc.z * depth;
    
    // clamp to pixel values
    if( x < 0 ) x = 0;
    if( y < 0 ) y = 0;
    if( z < 0 ) z = 0;
    if( x >= width ) x = H3DFloat( width - 1 );
    if( y >= height ) x = H3DFloat( height - 1 );
    if( z >= depth ) x = H3DFloat( depth - 1 );
  
    // set the pixel to new value
    setPixel( (unsigned int) x, (unsigned int) y, (unsigned int) z, color );
  }
}
