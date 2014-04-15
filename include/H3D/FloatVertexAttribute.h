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
/// \file FloatVertexAttribute.h
/// \brief Header file for FloatVertexAttribute.
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __FLOATVERTEXATTRIBUTE_H__
#define __FLOATVERTEXATTRIBUTE_H__

#include <H3D/X3DVertexAttributeNode.h>
#include <H3D/MFFloat.h>
#include <H3D/SFInt32.h>
#include <GL/glew.h>
#include <H3D/SFBool.h>
#include <H3D/GLVertexAttributeObject.h>

namespace H3D {
  /// \ingroup X3DNodes
  /// \class FloatVertexAttribute
  /// The FloatVertexAttribute node defines a set of per-vertex single 
  /// precision floating point attributes.
  ///
  /// The numComponents field specifies how many consecutive floating point 
  /// values should be grouped together per vertex. The length of the value
  /// field shall be a multiple of numComponents.
  ///
  /// The value field specifies an arbitrary collection of floating point 
  /// values that will be passed to the shader as per-vertex information.
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/VertexAttributes.x3d">VertexAttributes.x3d</a>
  ///     ( <a href="examples/VertexAttributes.x3d.html">Source</a> )
  class H3DAPI_API FloatVertexAttribute: 
    public X3DVertexAttributeNode{
  public:
    /// Constructor.
    FloatVertexAttribute( Inst< SFNode   > _metadata      = 0,
                          Inst< SFString > _name          = 0,
                          Inst< MFFloat  > _value         = 0,
                          Inst< SFInt32  > _numComponents = 0);

    /// Destructor.
    virtual ~FloatVertexAttribute();

    /// Perform the OpenGL commands to set the vertex attribute
    /// with the given index. value_index is the index of the 
    /// value in the value field that is to be used. 
    virtual void render( int value_index );

    /// Perform the OpenGL commands to set the vertex attributes
    /// as a an vertex attribute array on the attribute with the 
    /// currently set attribute index.
    virtual void renderArray();

    /// Disable the array state enabled in renderArray().
    virtual void disableArray();

    /// Implement the method to specify data and releated information
    virtual void setAttributeData ( );

    /// VBO rendering implementation
    virtual void renderVBO ( );

    /// VBO disabling implementation
    virtual void disableVBO ( );

    /// Check if this vertex attribute needs to be rendered
    virtual bool preRenderCheckFail ( );

     /// The value field specifies an arbitrary collection of floating point 
    /// values that will be passed to the shader as per-vertex information.
    /// The length of the value field shall be a multiple of numComponents.
    /// 
    /// <b>Access type:</b> inputOutput \n
    auto_ptr< MFFloat >  value;

    /// The numComponents field specifies how many consecutive floating 
    /// point values should be grouped together per vertex.
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 4 \n
    /// <b>Valid range:</b> [1-4]
    auto_ptr< SFInt32 > numComponents;


    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;

  };
}

#endif
