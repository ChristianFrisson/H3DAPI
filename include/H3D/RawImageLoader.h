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
/// \file RawImageLoader.h
/// \brief Header file for RawImageLoader.
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __RAWIMAGELOADER_H__
#define __RAWIMAGELOADER_H__

#include <H3D/H3DImageLoaderNode.h>
#include <H3D/SFInt32.h>
#include <H3D/SFString.h>
#include <H3D/SFVec3f.h>

namespace H3D {

  /// \ingroup H3DNodes
  /// \class RawImageLoader
  /// \brief The RawImageLoader loads a raw data file and lets the user
  /// specify the parameters for the values directly.
  ///
  /// The width, height and depth fields specify the number of pixels in 
  /// each dimension.
  /// The pixelType field specifies the type of each pixel in the image.
  /// Valid values are:
  /// - "LUMINANCE" 
  /// - "LUMINANCE_ALPHA"
  /// - "RGB"
  /// - "RGBA"
  /// - "BGR"
  /// - "BGRA"
  /// - "VEC3"
  /// The pixelComponentType specifies the type of each component of the 
  /// pixel, e.g. if pixelType is RGB, pixelComponentType specifies 
  /// what type of value the components R, G and B are. Valid values are:
  /// - "SIGNED" 
  /// - "UNSIGNED"
  /// - "RATIONAL"
  /// The bitsPerPixel specifies how many bits a pixel takes in memory.
  /// The pixelSize field is an optional field that specifies the size
  /// of each pixel in meters. This can be used for e.g. volume rendering
  /// components to know the image size in space.
  /// 
  /// This loader can also automatically detect and decompress gzip
  /// packaged data
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/ImageLoaders.x3d">ImageLoaders.x3d</a>
  ///     ( <a href="examples/ImageLoaders.x3d.html">Source</a> )
  class H3DAPI_API RawImageLoader : public H3DImageLoaderNode {
  public:
    /// Constructor.
    RawImageLoader( Inst< SFInt32  > _width = 0,
                    Inst< SFInt32  > _height = 0,
                    Inst< SFInt32  > _depth = 0,
                    Inst< SFString > _pixelType = 0,
                    Inst< SFString > _pixelComponentType = 0,
                    Inst< SFInt32  > _bitsPerPixel = 0,
                    Inst< SFVec3f  > _pixelSize = 0 );

    /// Loads the image from the url and returns a PixelImage with the data
    /// loaded from the file/
    virtual Image *loadImage( const string &url );

    /// TODO: Implement 
    ////virtual Image *loadImage( istream &is ) = 0;
   
    /// The width of the image in pixels.
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 1 \n
    auto_ptr< SFInt32 > width;

    /// The height of the image in pixels.
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 1 \n
    auto_ptr< SFInt32 > height;

    /// The height of the image in pixels.
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 1 \n
    auto_ptr< SFInt32 > depth;

    /// The type of each pixel element in the image. Valid values are:
    /// - "LUMINANCE" 
    /// - "LUMINANCE_ALPHA"
    /// - "RGB"
    /// - "RGBA"
    /// - "BGR"
    /// - "BGRA"
    /// - "VEC3"
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> "RGB" \n
    auto_ptr< SFString > pixelType;

    /// The type of each component of the pixel. Valid values are:
    /// - "SIGNED" 
    /// - "UNSIGNED"
    /// - "RATIONAL"
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> "UNSIGNED" \n
    auto_ptr< SFString > pixelComponentType;
    
    /// The number of bits per pixel. 
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> "24" \n
    auto_ptr< SFInt32 > bitsPerPixel;

    /// The size of the pixel in metres. 
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> "Vec3f( 0, 0, 0 )" \n
    auto_ptr< SFVec3f > pixelSize;

    /// The H3DNodeDatabase for this Node.
    static H3DNodeDatabase database;
  };
}

#endif
