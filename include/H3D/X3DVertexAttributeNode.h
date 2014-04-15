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
/// \file X3DVertexAttributeNode.h
/// \brief Header file for X3DVertexAttributeNode.
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __X3DVERTEXATTRIBUTENODE_H__
#define __X3DVERTEXATTRIBUTENODE_H__

#include <H3D/X3DGeometricPropertyNode.h>
#include <H3D/GLVertexAttributeObject.h>
#include <H3D/SFString.h>

namespace H3D {
  /// \ingroup AbstractNodes
  /// \class X3DVertexAttributeNode
  /// This abstract node type is the base type for all node types that specify
  /// per-vertex attribute information to the shader. 
  ///
  /// The name field describes a name that is mapped to the shading 
  /// language-specific name for describing per-vertex data. 
  class H3DAPI_API X3DVertexAttributeNode: 
    public X3DGeometricPropertyNode,
    public GLVertexAttributeObject{
  public:
    /// Constructor.
    X3DVertexAttributeNode( Inst< SFNode   > _metadata = 0,
                            Inst< SFString > _name     = 0);
   
    /// Perform the OpenGL commands to set the vertex attribute
    /// with the given index. value_index is the index of the 
    /// value in the value field that is to be used. The vertex
    /// attribute will be rendered as the attribute index specified
    /// by the setAttribFunction.
    virtual void render( int value_index ){}

    /// Perform the OpenGL commands to set the vertex attributes
    /// as a an vertex attribute array on the attribute with the 
    /// given index.
    virtual void renderArray() {}

    /// Disable the array state enabled in renderArray().
    virtual void disableArray() {}

    /// Set the attribute index this vertex attribute should be rendered
    /// as. A non-positive value will cause the vertex attribute not
    /// be rendered at all.
    inline void setAttribIndex( int _attrib_index ) { 
      attrib_index = _attrib_index;
    }

    /// Implement the method to specify data and releated information
    virtual void setAttributeData ( ){};

    /// VBO rendering implementation
    virtual void renderVBO ( ){};

    /// VBO disabling implementation
    virtual void disableVBO ( ){};

    /// Returns the default xml containerField attribute value.
    /// For this node it is "attrib".
    virtual string defaultXMLContainerField() {
      return "attrib";
    }

    /// The name field describes a name that is mapped to the shading 
    /// language-specific name for describing per-vertex data. 
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> "" \n
    auto_ptr< SFString >  name;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
    
  protected:
    int attrib_index;
  };
}

#endif






