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
/// \file Image.cpp
/// \brief CPP file for Image.
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include "Image.h"

using namespace H3D;


void Image::getSample( void *value, 
                       H3DFloat x, 
                       H3DFloat y, 
                       H3DFloat z ) {

  H3DFloat px = x * width() - 0.5f;
  H3DFloat py = y * height()- 0.5f;
  H3DFloat pz = z * depth() - 0.5f;

  if( px < 0 ) px = 0;
  if( py < 0 ) py = 0;
  if( pz < 0 ) pz = 0;

  H3DFloat fx = H3DFloor( px );
  H3DFloat fy = H3DFloor( py );
  H3DFloat fz = H3DFloor( pz );
  
  H3DFloat cx = H3DCeil( px );
  H3DFloat cy = H3DCeil( py );
  H3DFloat cz = H3DCeil( pz );
    
  if( cx >= width() ) cx--;
  if( cy >= height() ) cy--;
  if( cz >= depth() ) cz--;

  H3DFloat xd = px - fx;
  H3DFloat yd = py - fy;
  H3DFloat zd = pz - fz;

  // interpolate in z
  H3D::RGBA fff = getPixel( fx, fy, fz );
  H3D::RGBA ffc = getPixel( fx, fy, cz );
  H3D::RGBA fcf = getPixel( fx, cy, fz );
  H3D::RGBA fcc = getPixel( fx, cy, cz );
  H3D::RGBA cff = getPixel( cx, fy, fz );
  H3D::RGBA cfc = getPixel( cx, fy, cz );
  H3D::RGBA ccf = getPixel( cx, cy, fz );
  H3D::RGBA ccc = getPixel( cx, cy, cz );
  
  H3D::RGBA i1 = fff * (1-zd) + ffc * zd;
  H3D::RGBA i2 = fcf * (1-zd) + fcc * zd;
  H3D::RGBA j1 = cff * (1-zd) + cfc * zd;
  H3D::RGBA j2 = ccf * (1-zd) + ccc * zd;
  
  H3D::RGBA w1 = i1 * (1-yd) + i2 * yd;
  H3D::RGBA w2 = j1 * (1-yd) + j2 * yd;
  
  H3D::RGBA v = w1 * (1-xd) + w2 * xd;

  RGBAToImageValue( v, value );

}


namespace ImageInternals {
	inline H3DFloat getSignedValueAsFloat( void *i, 
                                         unsigned int bytes_to_read ) {
	  long v;
    memcpy( &v,
            i,
            bytes_to_read );
    return v / (H3DFloat) LONG_MAX;
	}

	inline H3DFloat getUnsignedValueAsFloat( void *i, 
                                           unsigned int bytes_to_read ) {
	  unsigned long v = 0;
    memcpy( &v,
            i,
            bytes_to_read );
    return v / (H3DFloat) (H3DPow( 2.0, (int)bytes_to_read * 8 ) - 1);
	}

	inline H3DFloat getRationalValueAsFloat( void *i, 
                                           unsigned int bytes_to_read ) {
    assert( bytes_to_read == 4 || bytes_to_read == 8 );
    double v = 0;
    if( bytes_to_read == 4 ) {
      v = *((float *)i);
    } else if( bytes_to_read == 8 ) {
      v = *((double *)i);
    }
    return (H3DFloat) v;
	}

	inline void writeFloatAsSignedValue( H3DFloat r,
                                       void *i, 
                                       unsigned int bytes_to_write ) {
	  long v = (long)(r * (H3DPow( 2.0, (int)bytes_to_write * 8 - 1 ) - 1));
    memcpy( i,
            (&v) + sizeof( long ) - bytes_to_write,
            bytes_to_write );
	}

	inline void writeFloatAsUnsignedValue( H3DFloat r,
                                       void *i, 
                                       unsigned int bytes_to_write ) {
	  unsigned long v = (unsigned long) (r * (H3DPow( 2.0, (int)bytes_to_write * 8 ) - 1) );
    memcpy( i,
            &v,
            bytes_to_write );
    H3DFloat f = getUnsignedValueAsFloat( i, bytes_to_write );
      
	}


	inline void writeFloatAsRationalValue( H3DFloat r,
                                         void *i, 
                                         unsigned int bytes_to_write ) {
    assert( bytes_to_write == 4 || bytes_to_write == 8 );

    if( bytes_to_write == 4 ) {
      *((float *)i) = r;
    } else if( bytes_to_write == 8 ) {
      *((double *)i) = r;
    }
	}

}

H3D::RGBA Image::getPixel( int x, int y, int z ) {
  unsigned int byte_rem = bitsPerPixel() % 8;
  unsigned int bytes_per_pixel = bitsPerPixel() / 8;

  assert( byte_rem == 0 );
  if( bytes_per_pixel <= 8 ) {
    char pixel_data[8]; 
    getElement( pixel_data, x, y, z );
    H3DUtil::RGBA rgba = imageValueToRGBA( pixel_data );
    return rgba;
  } else {
    char *pixel_data = new char[bytes_per_pixel];
    getElement( pixel_data, x, y, z );
    H3DUtil::RGBA rgba = imageValueToRGBA( pixel_data );
    delete [] pixel_data;
    return rgba;
  }
}

void Image::setPixel( const H3D::RGBA &value, int x, int y, int z ) {
  unsigned int byte_rem = bitsPerPixel() % 8;
  unsigned int bytes_per_pixel = bitsPerPixel() / 8;

  assert( byte_rem == 0 );
  if( bytes_per_pixel <= 8 ) {
    char pixel_data[8];
    RGBAToImageValue( value, pixel_data );
    setElement( pixel_data, x, y, z );    
  } else {
    char *pixel_data = new char[bytes_per_pixel];
    RGBAToImageValue( value, pixel_data );
    setElement( pixel_data, x, y, z );
    delete [] pixel_data;
  }
}

H3D::RGBA Image::imageValueToRGBA( void *_pixel_data ) {
  using namespace ImageInternals;

  char *pixel_data = (char *) _pixel_data;

  unsigned int byte_rem = bitsPerPixel() % 8;
  unsigned int bytes_per_pixel = bitsPerPixel() / 8;

  assert( byte_rem == 0 );

  switch( pixelType() ) {
  case Image::LUMINANCE: 
    switch( pixelComponentType() ) { 
    case Image::UNSIGNED: {
      H3DFloat fv = getUnsignedValueAsFloat( pixel_data, 
                                             bytes_per_pixel );
      return H3D::RGBA( fv, fv, fv, 1 );
    }
    case Image::SIGNED: {
      H3DFloat fv = getSignedValueAsFloat( pixel_data, 
                                           bytes_per_pixel );
      return H3D::RGBA( fv, fv, fv, 1 );
    }
    case Image::RATIONAL: {
      H3DFloat fv = getRationalValueAsFloat( pixel_data, 
                                             bytes_per_pixel );
      return H3D::RGBA( fv, fv, fv, 1 );
    }
    };
  case Image::LUMINANCE_ALPHA: {
    unsigned int bytes_per_component = bytes_per_pixel / 2;
    switch( pixelComponentType() ) { 
    case Image::UNSIGNED: {
      H3DFloat fv = getUnsignedValueAsFloat( pixel_data, 
                                             bytes_per_component );
      H3DFloat a = getUnsignedValueAsFloat( pixel_data + bytes_per_component, 
                                            bytes_per_component );
      return H3D::RGBA( fv, fv, fv, a );
    }
    case Image::SIGNED: {
      H3DFloat fv = getSignedValueAsFloat( pixel_data, 
                                           bytes_per_component );
      H3DFloat a = getSignedValueAsFloat( pixel_data + bytes_per_component, 
                                          bytes_per_component );

      return H3D::RGBA( fv, fv, fv, a );
    }
    case Image::RATIONAL: {
      H3DFloat fv = getRationalValueAsFloat( pixel_data, 
                                             bytes_per_component );
      H3DFloat a = getRationalValueAsFloat( pixel_data + bytes_per_component, 
                                             bytes_per_component );
      return H3D::RGBA( fv, fv, fv, a );
    }
    };
  }
  case Image::RGB: {
    unsigned int bytes_per_component = bytes_per_pixel / 3;
    switch( pixelComponentType() ) { 
    case Image::UNSIGNED: {
      H3DFloat r = getUnsignedValueAsFloat( pixel_data, 
                                            bytes_per_component );
      H3DFloat g = getUnsignedValueAsFloat( pixel_data + bytes_per_component, 
                                            bytes_per_component );
      H3DFloat b = getUnsignedValueAsFloat( pixel_data + 
                                            2 * bytes_per_component, 
                                            bytes_per_component );
      return H3D::RGBA( r, g, b, 1 );
    }
    case Image::SIGNED: {
      H3DFloat r = getSignedValueAsFloat( pixel_data, 
                                          bytes_per_component );
      H3DFloat g = getSignedValueAsFloat( pixel_data + bytes_per_component, 
                                          bytes_per_component );
      H3DFloat b = getSignedValueAsFloat( pixel_data + 
                                          2 * bytes_per_component, 
                                          bytes_per_component );
      return H3D::RGBA( r, g, b, 1 );
    }
    case Image::RATIONAL: {
      H3DFloat r = getSignedValueAsFloat( pixel_data, 
                                          bytes_per_component );
      H3DFloat g = getSignedValueAsFloat( pixel_data + bytes_per_component, 
                                          bytes_per_component );
      H3DFloat b = getSignedValueAsFloat( pixel_data + 
                                          2 * bytes_per_component, 
                                          bytes_per_component );
      return H3D::RGBA( r, g, b, 1 );
    }
    };
  }
  case Image::BGR:  {
    unsigned int bytes_per_component = bytes_per_pixel / 3;
    switch( pixelComponentType() ) { 
    case Image::UNSIGNED: {
      H3DFloat b = getUnsignedValueAsFloat( pixel_data, 
                                            bytes_per_component );
      H3DFloat g = getUnsignedValueAsFloat( pixel_data + bytes_per_component, 
                                            bytes_per_component );
      H3DFloat r = getUnsignedValueAsFloat( pixel_data + 
                                            2 * bytes_per_component, 
                                            bytes_per_component );
      return H3D::RGBA( r, g, b, 1 );
    }
    case Image::SIGNED: {
      H3DFloat b = getSignedValueAsFloat( pixel_data, 
                                          bytes_per_component );
      H3DFloat g = getSignedValueAsFloat( pixel_data + bytes_per_component, 
                                          bytes_per_component );
      H3DFloat r = getSignedValueAsFloat( pixel_data + 
                                          2 * bytes_per_component, 
                                          bytes_per_component );
      return H3D::RGBA( r, g, b, 1 );
    }
    case Image::RATIONAL: {
      H3DFloat b = getSignedValueAsFloat( pixel_data, 
                                          bytes_per_component );
      H3DFloat g = getSignedValueAsFloat( pixel_data + bytes_per_component, 
                                          bytes_per_component );
      H3DFloat r = getSignedValueAsFloat( pixel_data + 
                                          2 * bytes_per_component, 
                                          bytes_per_component );
      return H3D::RGBA( r, g, b, 1 );
    }
    };
  }
  case Image::RGBA:  {
    unsigned int bytes_per_component = bytes_per_pixel / 4;
    switch( pixelComponentType() ) { 
    case Image::UNSIGNED: {
      H3DFloat r = getUnsignedValueAsFloat( pixel_data, 
                                            bytes_per_component );
      H3DFloat g = getUnsignedValueAsFloat( pixel_data + bytes_per_component, 
                                            bytes_per_component );
      H3DFloat b = getUnsignedValueAsFloat( pixel_data + 
                                            2 * bytes_per_component, 
                                            bytes_per_component );
      H3DFloat a = getUnsignedValueAsFloat( pixel_data + 
                                            3 * bytes_per_component, 
                                            bytes_per_component );
      return H3D::RGBA( r, g, b, a );
    }
    case Image::SIGNED: {
      H3DFloat r = getSignedValueAsFloat( pixel_data, 
                                          bytes_per_component );
      H3DFloat g = getSignedValueAsFloat( pixel_data + bytes_per_component, 
                                          bytes_per_component );
      H3DFloat b = getSignedValueAsFloat( pixel_data + 
                                          2 * bytes_per_component, 
                                          bytes_per_component );
      H3DFloat a = getSignedValueAsFloat( pixel_data + 
                                          3 * bytes_per_component, 
                                          bytes_per_component );
      return H3D::RGBA( r, g, b, a );
    }
    case Image::RATIONAL: {
      H3DFloat r = getSignedValueAsFloat( pixel_data, 
                                          bytes_per_component );
      H3DFloat g = getSignedValueAsFloat( pixel_data + bytes_per_component, 
                                          bytes_per_component );
      H3DFloat b = getSignedValueAsFloat( pixel_data + 
                                          2 * bytes_per_component, 
                                          bytes_per_component );
      H3DFloat a = getSignedValueAsFloat( pixel_data + 
                                          3 * bytes_per_component, 
                                          bytes_per_component );
      return H3D::RGBA( r, g, b, a );
    }
    };
  }
  case Image::BGRA: {
    unsigned int bytes_per_component = bytes_per_pixel / 4;
    switch( pixelComponentType() ) { 
    case Image::UNSIGNED: {
      H3DFloat b = getUnsignedValueAsFloat( pixel_data, 
                                            bytes_per_component );
      H3DFloat g = getUnsignedValueAsFloat( pixel_data + bytes_per_component, 
                                            bytes_per_component );
      H3DFloat r = getUnsignedValueAsFloat( pixel_data + 
                                            2 * bytes_per_component, 
                                            bytes_per_component );
      H3DFloat a = getUnsignedValueAsFloat( pixel_data + 
                                            3 * bytes_per_component, 
                                            bytes_per_component );
      return H3D::RGBA( r, g, b, a );
    }
    case Image::SIGNED: {
      H3DFloat b = getSignedValueAsFloat( pixel_data, 
                                          bytes_per_component );
      H3DFloat g = getSignedValueAsFloat( pixel_data + bytes_per_component, 
                                          bytes_per_component );
      H3DFloat r = getSignedValueAsFloat( pixel_data + 
                                          2 * bytes_per_component, 
                                          bytes_per_component );
      H3DFloat a = getSignedValueAsFloat( pixel_data + 
                                          3 * bytes_per_component, 
                                          bytes_per_component );
      return H3D::RGBA( r, g, b, a );
    }
    case Image::RATIONAL: {
      H3DFloat b = getSignedValueAsFloat( pixel_data, 
                                          bytes_per_component );
      H3DFloat g = getSignedValueAsFloat( pixel_data + bytes_per_component, 
                                          bytes_per_component );
      H3DFloat r = getSignedValueAsFloat( pixel_data + 
                                          2 * bytes_per_component, 
                                          bytes_per_component );
      H3DFloat a = getSignedValueAsFloat( pixel_data + 
                                          3 * bytes_per_component, 
                                          bytes_per_component );
      return H3D::RGBA( r, g, b, a );
    }
    };
  }
  };
  return H3D::RGBA();
}

void Image::RGBAToImageValue( const H3D::RGBA &rgba, void *_pixel_data ) {
  using namespace ImageInternals;

  char *pixel_data = (char *) _pixel_data;

  unsigned int byte_rem = bitsPerPixel() % 8;
  unsigned int bytes_per_pixel = bitsPerPixel() / 8;

  assert( byte_rem == 0 );

  switch( pixelType() ) {
  case Image::LUMINANCE: 
    switch( pixelComponentType() ) { 
    case Image::UNSIGNED: {
      writeFloatAsUnsignedValue( rgba.r,
                                 pixel_data, 
                                 bytes_per_pixel );
      return;
    }
    case Image::SIGNED: {
      writeFloatAsSignedValue( rgba.r,
                               pixel_data, 
                               bytes_per_pixel );
      return;
    }
    case Image::RATIONAL: {
      writeFloatAsRationalValue( rgba.r,
                                 pixel_data, 
                                 bytes_per_pixel );
      return;
    }
    };
  case Image::LUMINANCE_ALPHA: {
    unsigned int bytes_per_component = bytes_per_pixel / 2;
    switch( pixelComponentType() ) { 
    case Image::UNSIGNED: {
      writeFloatAsUnsignedValue( rgba.r,
                                 pixel_data, 
                                 bytes_per_component );
      writeFloatAsUnsignedValue( rgba.a,
                                 pixel_data + bytes_per_component, 
                                 bytes_per_component );
      return;
    }
    case Image::SIGNED: {
      writeFloatAsUnsignedValue( rgba.r,
                                 pixel_data, 
                                 bytes_per_component );
      writeFloatAsUnsignedValue( rgba.a,
                                 pixel_data + bytes_per_component, 
                                 bytes_per_component );
      return;
    }
    case Image::RATIONAL: {
      writeFloatAsRationalValue( rgba.r,
                                 pixel_data, 
                                 bytes_per_component );
      writeFloatAsRationalValue( rgba.a,
                                 pixel_data + bytes_per_component, 
                                 bytes_per_component );
      return;
    }
    };
  }
  case Image::RGB: {
    unsigned int bytes_per_component = bytes_per_pixel / 3;
    switch( pixelComponentType() ) { 
    case Image::UNSIGNED: {
      writeFloatAsUnsignedValue( rgba.r,
                                 pixel_data, 
                                 bytes_per_component );
      writeFloatAsUnsignedValue( rgba.g,
                                 pixel_data + bytes_per_component, 
                                 bytes_per_component );
      writeFloatAsUnsignedValue( rgba.b,
                                 pixel_data + 2 * bytes_per_component, 
                                 bytes_per_component );
      return;
    }
    case Image::SIGNED: {
      writeFloatAsSignedValue( rgba.r,
                               pixel_data, 
                               bytes_per_component );
      writeFloatAsSignedValue( rgba.g,
                               pixel_data + bytes_per_component, 
                               bytes_per_component );
      writeFloatAsSignedValue( rgba.b,
                               pixel_data + 2 * bytes_per_component, 
                               bytes_per_component );
      return;
    }
    case Image::RATIONAL: {
      writeFloatAsRationalValue( rgba.r,
                               pixel_data, 
                               bytes_per_component );
      writeFloatAsRationalValue( rgba.g,
                               pixel_data + bytes_per_component, 
                               bytes_per_component );
      writeFloatAsRationalValue( rgba.b,
                               pixel_data + 2 * bytes_per_component, 
                               bytes_per_component );
      return;
    }
    };
  }
  case Image::BGR:  {
    unsigned int bytes_per_component = bytes_per_pixel / 3;
    switch( pixelComponentType() ) { 
    case Image::UNSIGNED: {
      writeFloatAsUnsignedValue( rgba.b,
                                 pixel_data, 
                                 bytes_per_component );
      writeFloatAsUnsignedValue( rgba.g,
                                 pixel_data + bytes_per_component, 
                                 bytes_per_component );
      writeFloatAsUnsignedValue( rgba.r,
                                 pixel_data + 2 * bytes_per_component, 
                                 bytes_per_component );
      return;
    }
    case Image::SIGNED: {
      writeFloatAsSignedValue( rgba.b,
                               pixel_data, 
                               bytes_per_component );
      writeFloatAsSignedValue( rgba.g,
                               pixel_data + bytes_per_component, 
                               bytes_per_component );
      writeFloatAsSignedValue( rgba.r,
                               pixel_data + 2 * bytes_per_component, 
                               bytes_per_component );
      return;
    }
    case Image::RATIONAL: {
      writeFloatAsRationalValue( rgba.b,
                                 pixel_data, 
                                 bytes_per_component );
      writeFloatAsRationalValue( rgba.g,
                                 pixel_data + bytes_per_component, 
                                 bytes_per_component );
      writeFloatAsRationalValue( rgba.r,
                                 pixel_data + 2 * bytes_per_component, 
                                 bytes_per_component );
      return;
    }
    };
  }
  case Image::RGBA:  {
    unsigned int bytes_per_component = bytes_per_pixel / 4;
    switch( pixelComponentType() ) { 
    case Image::UNSIGNED: {
      writeFloatAsUnsignedValue( rgba.r,
                                 pixel_data, 
                                 bytes_per_component );
      writeFloatAsUnsignedValue( rgba.g,
                                 pixel_data + bytes_per_component, 
                                 bytes_per_component );
      writeFloatAsUnsignedValue( rgba.b,
                                 pixel_data + 2 * bytes_per_component, 
                                 bytes_per_component );
      writeFloatAsUnsignedValue( rgba.a,
                                 pixel_data + 3 * bytes_per_component, 
                                 bytes_per_component );
      return;
    }
    case Image::SIGNED: {
      writeFloatAsSignedValue( rgba.r,
                               pixel_data, 
                               bytes_per_component );
      writeFloatAsSignedValue( rgba.g,
                               pixel_data + bytes_per_component, 
                               bytes_per_component );
      writeFloatAsSignedValue( rgba.b,
                               pixel_data + 2 * bytes_per_component, 
                               bytes_per_component );
      writeFloatAsSignedValue( rgba.a,
                                 pixel_data + 3 * bytes_per_component, 
                                 bytes_per_component );
      return;
    }
    case Image::RATIONAL: {
      writeFloatAsRationalValue( rgba.r,
                                 pixel_data, 
                                 bytes_per_component );
      writeFloatAsRationalValue( rgba.g,
                                 pixel_data + bytes_per_component, 
                                 bytes_per_component );
      writeFloatAsRationalValue( rgba.b,
                                 pixel_data + 2 * bytes_per_component, 
                                 bytes_per_component );
      writeFloatAsRationalValue( rgba.a,
                                 pixel_data + 3 * bytes_per_component, 
                                 bytes_per_component );
      return;
    }

    };
  }
  case Image::BGRA: {
   unsigned int bytes_per_component = bytes_per_pixel / 4;
    switch( pixelComponentType() ) { 
    case Image::UNSIGNED: {
      writeFloatAsUnsignedValue( rgba.b,
                                 pixel_data, 
                                 bytes_per_component );
      writeFloatAsUnsignedValue( rgba.g,
                                 pixel_data + bytes_per_component, 
                                 bytes_per_component );
      writeFloatAsUnsignedValue( rgba.r,
                                 pixel_data + 2 * bytes_per_component, 
                                 bytes_per_component );
      writeFloatAsUnsignedValue( rgba.a,
                                 pixel_data + 3 * bytes_per_component, 
                                 bytes_per_component );
      return;
    }
    case Image::SIGNED: {
      writeFloatAsSignedValue( rgba.b,
                               pixel_data, 
                               bytes_per_component );
      writeFloatAsSignedValue( rgba.g,
                               pixel_data + bytes_per_component, 
                               bytes_per_component );
      writeFloatAsSignedValue( rgba.r,
                               pixel_data + 2 * bytes_per_component, 
                               bytes_per_component );
      writeFloatAsSignedValue( rgba.a,
                                 pixel_data + 3 * bytes_per_component, 
                                 bytes_per_component );
      return;
    }
    case Image::RATIONAL: {
      writeFloatAsRationalValue( rgba.b,
                                 pixel_data, 
                                 bytes_per_component );
      writeFloatAsRationalValue( rgba.g,
                                 pixel_data + bytes_per_component, 
                                 bytes_per_component );
      writeFloatAsRationalValue( rgba.r,
                                 pixel_data + 2 * bytes_per_component, 
                                 bytes_per_component );
      writeFloatAsRationalValue( rgba.a,
                                 pixel_data + 3 * bytes_per_component, 
                                 bytes_per_component );
      return;
    }

    };
  }
  };
}
