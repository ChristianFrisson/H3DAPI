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
/// \file TextureTransform.h
/// \brief Header file for TextureTransform, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __TEXTURETRANSFORM_H__
#define __TEXTURETRANSFORM_H__

#include <H3D/X3DTextureTransform2DNode.h>

namespace H3D {
  /// \ingroup X3DNodes 
  /// \class TextureTransform
  /// \brief The TextureTransform node defines a 2D transformation that is 
  /// applied to texture coordinates (see TextureCoordinate). 
  ///
  /// This node affects the way textures coordinates are applied to the
  /// geometric surface. The transformation consists of (in order):
  ///
  ///  - a translation.
  ///  - a rotation about the centre point.
  ///  - a non-uniform scale about the centre point.
  ///
  /// These parameters support changes to the size, orientation, and
  /// position of textures on shapes. Note that these operations appear
  /// reversed when viewed on the surface of geometry. For example, a scale
  /// value of (2 2) will scale the texture coordinates and have the net
  /// effect of shrinking the texture size by a factor of 2 (texture
  /// coordinates are twice as large and thus cause the texture to
  /// repeat). A translation of (0.5 0.0) translates the texture coordinates
  /// +.5 units along the S-axis and has the net effect of translating the
  /// texture 0.5 along the S-axis on the geometry's surface. A rotation of
  /// pi/2 of the texture coordinates results in a pi/2 rotation of the
  /// texture on the geometry.
  ///
  /// The center field specifies a translation offset in texture coordinate
  /// space about which the rotation and scale fields are applied. The scale
  /// field specifies a scaling factor in S and T of the texture coordinates
  /// about the center point. scale values shall be in the range (,). The
  /// rotation field specifies a rotation in radians of the texture
  /// coordinates about the center point after the scale has been applied. A
  /// positive rotation value makes the texture coordinates rotate
  /// counterclockwise about the centre, thereby rotating the appearance of
  /// the texture itself clockwise. The translation field specifies a
  /// translation of the texture coordinates.
  ///
  /// In matrix transformation notation, where Tc is the untransformed
  /// texture coordinate, Tc' is the transformed texture coordinate, C
  /// (center), T (translation), R (rotation), and S (scale) are the
  /// intermediate transformation matrices,
  ///
  /// <center>
  ///    Tc' = C * S * R * C * T * Tc
  /// </center>
  ///
  /// Note that this transformation order is the reverse of the Transform
  /// node transformation order since the texture coordinates, not the
  /// texture, are being transformed (i.e., the texture coordinate system). 
  /// 
  /// \par Internal routes:
  /// \dotfile TextureTransform.dot
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/TextureTransform.x3d">TextureTransform.x3d</a>
  ///     ( <a href="examples/TextureTransform.x3d.html">Source</a> )
  class H3DAPI_API TextureTransform : public X3DTextureTransform2DNode {
  public:
    /// Constructor.
    TextureTransform( Inst< DisplayList > _displayList = 0,
                      Inst< SFNode >  _metadata    = 0,
                      Inst< SFVec2f>  _center      = 0,
                      Inst< SFFloat>  _rotation    = 0,
                      Inst< SFVec2f>  _scale       = 0,
                      Inst< SFVec2f>  _translation = 0 );

    /// Set the GL_TEXTURE matrix to the specified texture
    /// transform for the currently active texture unit.
    virtual void render();

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  };
}

#endif
