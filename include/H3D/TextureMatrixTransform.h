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
/// \file TextureMatrixTransform.h
/// \brief Header file for TextureMatrixTransform, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __TEXTUREMATRIXTRANSFORM_H__
#define __TEXTUREMATRIXTRANSFORM_H__

#include <H3D/X3DTextureTransformNode.h>
#include <H3D/SFMatrix4f.h>

namespace H3D {
  /// \ingroup X3DNodes 
  /// \class TextureMatrixTransform
  /// \brief The matrix field specifies a generalized, unfiltered 4×4 
  /// transformation matrix that can be used to modify the texture. 
  /// Any set of values is permitted.
  /// 
  /// \par Internal routes:
  /// \dotfile TextureMatrixTransform.dot
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/TextureMatrixTransform.x3d">TextureMatrixTransform.x3d</a>
  ///     ( <a href="examples/TextureMatrixTransform.x3d.html">Source</a> )
  class H3DAPI_API TextureMatrixTransform : public X3DTextureTransformNode {
  public:
    /// Constructor.
    TextureMatrixTransform( Inst< DisplayList > _displayList = 0,
                            Inst< SFNode      > _metadata    = 0,
                            Inst< SFMatrix4f  > _matrix      = 0 );

    /// Set the GL_TEXTURE matrix to the specified texture
    /// transform for the currently active texture unit.
    virtual void render();

    /// The matrix field specifies a generalized, unfiltered 4×4 
    /// transformation matrix that can be used to modify the texture. 
    /// Any set of values is permitted.Specifies a translation of the texture coordinates.
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> Unit matrix \n
    /// 
    /// \dotfile TextureMatrixTransform_matrix.dot
    auto_ptr< SFMatrix4f > matrix;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  };
}

#endif
