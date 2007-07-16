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
/// \file Matrix4VertexAttribute.h
/// \brief Header file for Matrix4VertexAttribute.
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __MATRIX4VERTEXATTRIBUTE_H__
#define __MATRIX4VERTEXATTRIBUTE_H__

#include <X3DVertexAttributeNode.h>
#include <MFMatrix4f.h>

namespace H3D {

  /// \ingroup X3DNodes
  /// \class Matrix4VertexAttribute
  /// Defines a set of per-vertex 4×4 matrix attributes.
  ///
  /// The value field specifies an arbitrary collection of matrix values
  /// that will be passed to the shader as per-vertex information.
  class H3DAPI_API Matrix4VertexAttribute: public X3DVertexAttributeNode {
  public:
    /// Constructor.
    Matrix4VertexAttribute( Inst< SFNode     > _metadata = 0,
                            Inst< SFString   > _name     = 0,
                            Inst< MFMatrix4f > _value  = 0 );
    
    /// Perform the OpenGL commands to set the vertex attribute
    /// with the given index. value_index is the index of the 
    /// value in the value field that is to be used and attrib_index
    /// is the index of the attribute to set.
    virtual void render( int value_index );

    /// Perform the OpenGL commands to set the vertex attributes
    /// as a an vertex attribute array on the attribute with the 
    /// given index.
    virtual void renderArray();

    /// Disable the array state enabled in renderArray().
    virtual void disableArray();

    /// The value field specifies an arbitrary collection of Matrix4f values
    /// that will be passed to the shader as per-vertex information.
    /// 
    /// <b>Access type:</b> inputOutput \n
    auto_ptr< MFMatrix4f >  value;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  };
}

#endif
