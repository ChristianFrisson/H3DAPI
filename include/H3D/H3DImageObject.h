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
/// \file H3DImageObject.h
/// \brief Header file for H3DImageObject, X3D abstract texture scene-graph 
/// node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __H3DIMAGEOBJECT_H__
#define __H3DIMAGEOBJECT_H__

#include <H3D/X3DAppearanceChildNode.h>
#include <H3DUtil/Image.h>
#include "GL/glew.h"

namespace H3D {

  /// \ingroup AbstractInterface
  /// \class H3DImageObject
  /// \brief This abstract interface is inherited by all nodes that contains an
  /// SFImage object with an Image containing data. The data can be used
  /// for e.g. texturing or volume haptics rendering. 
  /// 
  class H3DAPI_API H3DImageObject  {
  public:
    /// The SFImage field is a field containing an Image pointer. The 
    /// pointer will be reference counted by the SFImage field.
    class H3DAPI_API SFImage: 
      public RefCountSField< Image, ParsableField > {
  
    public:
      /// Constructor.
      SFImage() : x_min( -1 ), y_min( -1 ), 
                  x_max(-1), y_max( -1 ) {
      }

      /// Call this function before doing any calls to any setPixel
      /// functions in order to record all the changes to the image.
      inline virtual void beginEditing() { resetChanges(); }

      /// Call this function after doing editing to the image in order
      /// to send an event that editing has been done. 
      inline virtual void endEditing() { touch(); }

      /// Set the volume that has been edited manually.
      inline void setEditedArea( int _min_x, int _min_y, int _min_z,
                                 int _max_x, int _max_y, int _max_z ) {
        x_min = _min_x;
        y_min = _min_y;
        z_min = _min_z;
        x_max = _max_x;
        y_max = _max_y;
        z_max = _max_z;
      }
      
      /// Set a pixel in the image. The components from the color argument
      /// will be used depending on the image type as follows:
      /// LUMINANCE:       color.r
      /// LUMINANCE_ALPHA: color.r, color.a 
      /// RGB, BGR, VEC3:  color.rgb
      /// RGBA, BGRA       color.rgba
      virtual void setPixel( unsigned int x, unsigned int y, 
                             const RGBA &color );

      /// Set a pixel in the image specified by a texture coordinate. 
      /// The components from the color argument
      /// will be used depending on the image type as follows:
      /// LUMINANCE:       color.r
      /// LUMINANCE_ALPHA: color.r, color.a 
      /// RGB, BGR, VEC3:  color.rgb
      /// RGBA, BGRA       color.rgba
      /// Texture coordinates will be clamped to the range [0,1]
      virtual void setPixel( const Vec2f &tex_coord, 
                             const RGBA &color );

      /// Set a pixel in the image. The components from the color argument
      /// will be used depending on the image type as follows:
      /// LUMINANCE:       color.r
      /// LUMINANCE_ALPHA: color.r, color.a 
      /// RGB, BGR, VEC3:  color.rgb
      /// RGBA, BGRA       color.rgba
      virtual void setPixel( unsigned int x, unsigned int y, unsigned int z, 
                             const RGBA &color );

      /// Set a pixel in the image specified by a texture coordinate. 
      /// The components from the color argument
      /// will be used depending on the image type as follows:
      /// LUMINANCE:       color.r
      /// LUMINANCE_ALPHA: color.r, color.a 
      /// RGB, BGR, VEC3:  color.rgb
      /// RGBA, BGRA       color.rgba
      /// Texture coordinates will be clamped to the range [0,1]
      virtual void setPixel( const Vec3f &tex_coord, 
                             const RGBA &color );

      /// Returns the minimum value in x that has been changed by the 
      /// setPixel functions.
      /// Value is only valid if imageChanged() is true.
      inline int xMin() { return x_min; }

      /// Returns the minimum value in y that has been changed by the 
      /// setPixel functions.
      /// Value is only valid if imageChanged() is true.
      inline int yMin() { return y_min; }

      /// Returns the minimum value in z that has been changed by the 
      /// setPixel functions.
      /// Value is only valid if imageChanged() is true.
      inline int zMin() { return z_min; }

      /// Returns the maximum value in x that has been changed by the 
      /// setPixel functions.
      /// Value is only valid if imageChanged() is true.
      inline int xMax() { return x_max; }

      /// Returns the maximum value in y that has been changed by the 
      /// setPixel functions.
      /// Value is only valid if imageChanged() is true.
      inline int yMax() { return y_max; }

      /// Returns the maximum value in z that has been changed by the 
      /// setPixel functions.
      /// Value is only valid if imageChanged() is true.
      inline int zMax() { return z_max; }

      /// Returns the offset in x of the changed part of the image.
      /// Value is only valid if imageChanged() is true.
      inline int xOffset() { return x_min; }

      /// Returns the offset in y of the changed part of the image.
      /// Value is only valid if imageChanged() is true.
      inline int yOffset() { return y_min; }

      /// Returns the offset in z of the changed part of the image.
      /// Value is only valid if imageChanged() is true.
      inline int zOffset() { return z_min; }
      
      /// Returns the width of the changed part of the image.
      /// Value is only valid if imageChanged() is true.
      inline int changedWidth() { return x_max - x_min + 1; }

      /// Returns the height of the changed part of the image.
      /// Value is only valid if imageChanged() is true.
      inline int changedHeight() { return y_max - y_min + 1; }

      /// Returns the depth of the changed part of the image.
      /// Value is only valid if imageChanged() is true.
      inline int changedDepth() { return z_max - z_min + 1; }

      /// Returns true if the image has been updated with the 
      /// setPixel functions.
      inline bool imageChanged() { return x_min != -1; }

      /// Resets all variables that keep track of what part of the
      /// image has been updated.
      inline void resetChanges() { x_min = -1; }
    
    protected:
      virtual void setValueFromString( const string &s ) {
        setValue( X3D::X3DStringTo3DImage( s ) );
      }
      int x_min, y_min, x_max, y_max, z_min, z_max;
    };
        
    /// Constructor.
    H3DImageObject( Inst< SFImage > _image = 0 ):
      image( _image ) {}

    /// Field containing the Image to use as a texture. 
    auto_ptr< SFImage > image;
        
  };
}

#endif
