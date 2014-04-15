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
/// \file ImageObjectInfo.h
/// \brief Header file for ImageObjectInfo.
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __IMAGEOBJECTINFO_H__
#define __IMAGEOBJECTINFO_H__

#include <H3D/H3DImageObject.h>
#include <H3D/X3DChildNode.h>
#include <H3D/SFInt32.h>
#include <H3D/SFVec3f.h>
#include <H3D/PeriodicUpdate.h>
#include <H3D/X3DTexture3DNode.h>

namespace H3D {
  /// \ingroup H3DNodes 
  /// \class ImageObjectInfo
  /// \brief This node can be used to get information about dimensions, sizes
  /// pixel types etc from H3DImageObject nodes such as Image3DTexture.
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/ImageObjectInfo.x3d">ImageObjectInfo.x3d</a>
  ///     ( <a href="examples/ImageObjectInfo.x3d.html">Source</a> )
  class H3DAPI_API ImageObjectInfo : public X3DChildNode {
  public:
    
    /// An MFNode where we make sure the type of the nodes contained
    /// is a subclass of X3DTextureNode.
    typedef DependentSFNode< X3DTexture3DNode, 
                             FieldRef< H3DImageObject,
                                       H3DImageObject::SFImage,
                                       &H3DImageObject::image >,
                             true >
    SFImageObjectNode;

    /// Field for updating the output fields when the image object
    /// changes value.
    class H3DAPI_API UpdateFields: public PeriodicUpdate< SFImageObjectNode > {
      virtual void update();
    };

    ImageObjectInfo( Inst< SFImageObjectNode > _texture = 0,
                     Inst< SFVec3f           > _pixelSize = 0,
                     Inst< SFInt32           > _width = 0,
                     Inst< SFInt32           > _height = 0,
                     Inst< SFInt32           > _depth = 0,
                     Inst< SFString          > _pixelType = 0,
                     Inst< SFString          > _pixelComponentType = 0,
                     Inst< SFInt32           > _bitsPerPixel = 0,
                     Inst< SFVec3f           > _totalSize = 0 );

    /// The texture field contains the image object we want to get 
    /// information from.
    ///
    /// <b>Access type:</b> inputOutput \n
    auto_ptr< SFImageObjectNode >  texture;

    /// The size of each pixel/voxel in the image object in metres.
    ///
    /// <b>Access type:</b> outputOnly \n
    auto_ptr< SFVec3f > pixelSize;

    /// The width of the image object in pixels.
    ///
    /// <b>Access type:</b> outputOnly \n
    auto_ptr< SFInt32 > width;

    /// The height of the image object in pixels.
    ///
    /// <b>Access type:</b> outputOnly \n
    auto_ptr< SFInt32 > height;

    /// The depth of the image object in pixels.
    ///
    /// <b>Access type:</b> outputOnly \n
    auto_ptr< SFInt32 > depth;

    /// The type of each pixel element in the image object. Valid values are:
    /// - "LUMINANCE" 
    /// - "LUMINANCE_ALPHA"
    /// - "RGB"
    /// - "RGBA"
    /// - "BGR"
    /// - "BGRA"
    /// - "VEC3"
    /// 
    /// <b>Access type:</b> outputOnly \n
    auto_ptr< SFString > pixelType;

    /// The type of each component of the pixel. Valid values are:
    /// - "SIGNED" 
    /// - "UNSIGNED"
    /// - "RATIONAL"
    /// 
    /// <b>Access type:</b> outputOnly \n
    auto_ptr< SFString > pixelComponentType;

    /// The number of bits used for each pixel.
    /// 
    /// <b>Access type:</b> outputOnly \n
    auto_ptr< SFInt32 > bitsPerPixel;

    /// The total size of the volume in metres, i.e. 
    /// Vec3f( width  * pixel_size.x, 
    ///        height * pixel_size.y, 
    ///        depth  * pixel_size.z )
    ///
    /// <b>Access type:</b> outputOnly \n
    auto_ptr< SFVec3f > totalSize;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  protected:
    /// The field used to update the other fields.
    auto_ptr< UpdateFields > update_fields;

  };
}

#endif
