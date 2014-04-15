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
/// \file ImageObjectTexture.h
/// \brief Header file for ImageObjectTexture.
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef _IMAGEOBJECTTEXTURE_HH_
#define _IMAGEOBJECTTEXTURE_HH_

#include <H3D/X3DTexture3DNode.h>

namespace H3D {
  /// \ingroup H3DNodes 
  /// \class ImageObjectTexture
  /// \brief The ImageObjectTexture node uses the image data from any
  /// node that implements the H3DImageObject interface as texture data.
  ///
  /// The imageObject field contains the H3DImageObject, which image
  /// data will be used as texture.
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/ImageObjectTexture.x3d">ImageObjectTexture.x3d</a>
  ///     ( <a href="examples/ImageObjectTexture.x3d.html">Source</a> )
  ///   - <a href="../../../H3DAPI/examples/All/ImageObjectTexture1.x3d">ImageObjectTexture1.x3d</a>
  ///     ( <a href="examples/ImageObjectTexture1.x3d.html">Source</a> )
  class H3DAPI_API ImageObjectTexture : public X3DTexture3DNode {
  public:
    
    /// The SFImageObject class contains an H3DImageObject and maintains
    /// a route from the contained object's image field to the image field
    /// of the ImageObjectTexture it resides in.
    class H3DAPI_API SFImageObject: 
      public AutoUpdate< TypedSFNodeObject< H3DImageObject > > {
    protected:
      /// Sets up a route between the added node's image field and the
      /// owners image field.
      virtual void onAdd( Node *n );

      /// Removes the route that was added by onAdd ().
      virtual void onRemove( Node *n );
    };

    /// Constructor.
    ImageObjectTexture( Inst< DisplayList > _displayList = 0,
                        Inst< SFNode  > _metadata  = 0,
                        Inst< SFBool  > _repeatS   = 0,
                        Inst< SFBool  > _repeatT   = 0,
                        Inst< SFBool  > _repeatR   = 0,
                        Inst< SFBool  > _scaleToP2 = 0,
                        Inst< SFImage > _image     = 0,
                        Inst< SFImageObject > _imageObject = 0,
                        Inst< SFTextureProperties > _textureProperties = 0 );
    
    /// The H3DImageObject which image we want to use as data.
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> NULL \n
    auto_ptr< SFImageObject > imageObject;

    /// The H3DNodeDatabase for this node.
    static H3D::H3DNodeDatabase database;
  };
}

#endif
