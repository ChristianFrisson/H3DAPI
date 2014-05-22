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
/// \file X3DTextureCoordinateNode.h
/// \brief Header file for X3DTextureCoordinateNode, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __X3DTEXTURECOORDINATENODE_H__
#define __X3DTEXTURECOORDINATENODE_H__

#include <H3D/X3DGeometricPropertyNode.h>
#include <GL/glew.h>
#include <H3D/GLVertexAttributeObject.h>

namespace H3D {
  class X3DTextureNode;

  /// \ingroup AbstractNodes
  /// \class X3DTextureCoordinateNode
  /// \brief This abstract node type is the base type for all node types which
  /// specify texture coordinates. It adds a new geometric property node
  /// type to those specified in 11 Rendering component.
  ///
  /// Texture coordinate nodes comes in two flavours, types that generate
  /// texture coordinates and nodes that specify the texture coordinates
  /// explicitly. There are two groups of functions, one for each of the types.
  /// When a user wants to use a function he should first check if it is 
  /// supported by calling the supportsExplicitTexCoords() and supportsTexGen() 
  /// functions.
  ///
  /// 
  class H3DAPI_API X3DTextureCoordinateNode : 
    public X3DGeometricPropertyNode,
    public GLVertexAttributeObject{
  public:
    
    /// Constructor.
    X3DTextureCoordinateNode( Inst< SFNode>  _metadata = 0 );

    /// Render the texture coordinate for all texture units used by
    /// the texture in X3DTextureNode::getActiveTexture.
    static void renderTexCoordForActiveTexture( const Vec3f &tc );

    /// Render the texture coordinate for all texture units used by
    /// the given texture .
    static void renderTexCoordForTexture( const Vec3f &tc, X3DTextureNode *t );

    /// Render the vertex buffer object for all texture units used by the
    /// texture in X3DTextureNode::getActiveTexture. The arguments to this
    /// function are the same arguments as to glTexCoordPointer when using
    /// vertex buffer objects since this function is basically a wrapper
    /// around that function to handle the case of the texture being a
    /// MultiTexture.
    static void renderVertexBufferObjectForActiveTexture(
      GLint size, GLenum type, GLsizei stride, const GLvoid *pointer );

    /// Render the vertex buffer object for all texture units used by the
    /// given texture. The other arguments to this
    /// function are the same arguments as to glTexCoordPointer when using
    /// vertex buffer objects since this function is basically a wrapper
    /// around that function to handle the case of the texture being a
    /// MultiTexture.
    static void renderVertexBufferObjectForTexture(
      GLint size, GLenum type, GLsizei stride, const GLvoid *pointer,
      X3DTextureNode *t );

    /// Function that corresponds to the static version of the function
    /// renderVertexBufferObjectForActiveTexture. Disables state for active
    /// texture.
    static void disableVBOForActiveTexture();

    /// Function that corresponds to the static version of the function
    /// renderVertexBufferObjectForTexture. Disables state for given texture.
    static void disableVBOForTexture( X3DTextureNode *t );

    ////////////////////////////////////////////////////////////////
    // Functions for rendering texture coordinates explicitly

    /// Returns true if the node supports rendering of explicit texture 
    /// coordinates.
    virtual bool supportsExplicitTexCoords() {
      return false;
    } 

    /// Perform the OpenGL commands to render a texture coordinate given the 
    /// index of the texture coordinate.
    virtual void render( int index ) {}

    /// Render the texture coordinate for given texture unit
    virtual void renderForTextureUnit( int index,
                                       unsigned int texture_unit ) {}
    
    /// Render the texture coordinate for the texture units between and
    /// including start_unit and end_unit.
    void renderForTextureUnits( int index,
                                unsigned int start_unit,
                                unsigned int end_unit );
    
    /// Render the texture coordinate for all texture units used by
    /// the texture in X3DTextureNode::getActiveTexture.
    void renderForActiveTexture( int index );

    /// Render the texture coordinate for all texture units used by
    /// the texture
    void renderForTexture( int index, X3DTextureNode *t );

    /// Returns the number of texture coordinates this node can render.
    /// -1 means that the node is a texture coordinate generator in
    /// which case nr available coords has no meaning.
    virtual unsigned int nrAvailableTexCoords() { return -1; }

    /// Perform the OpenGL commands to render all texture coordinates as 
    /// an arary.
    virtual void renderArray() {}

    /// Render the texture coordinate for all texture units used by
    /// the texture
    void renderArrayForTexture( X3DTextureNode *t );

    /// Render the texture coordinate for all texture units used by
    /// the texture in X3DTextureNode::getActiveTexture.
    void renderArrayForActiveTexture();

    /// Perform the OpenGL commands to render all texture coordinates as 
    /// an array for the given texture unit.
    virtual void renderArrayForTextureUnit( unsigned int texture_unit );
    
    /// Perform the OpenGL commands to render all texture coordinates as 
    /// an array for the texture units between and
    /// including start_unit and end_unit.
    void renderArrayForTextureUnits( unsigned int start_unit,
                                     unsigned int end_unit );

    /// Disable the array state enabled in renderArray().
    virtual void disableArray() {}

    /// Disable the array state enabled in renderArrayForTexture
    void disableArrayForTexture( X3DTextureNode *t );

    /// Disable the array state enabled in renderArrayForActiveTexture
    void disableArrayForActiveTexture();

    /// Disable the array state enabled in renderArrayForTextureUnit().
    virtual void disableArrayForTextureUnit( unsigned int texture_unit );

    /// Disable the array state enabled in renderArrayForTextureUnits().
    void disableArrayForTextureUnits( unsigned int start_unit,
                                      unsigned int end_unit );


    /// Render the texture coordinate for all texture units used by
    /// the texture
    void renderVertexBufferObjectForTexture( X3DTextureNode *t );

    /// Render the texture coordinate for all texture units used by
    /// the texture in X3DTextureNode::getActiveTexture.
    void renderVertexBufferObjectForActiveTexture();

    /// Perform the OpenGL commands to render all texture coordinates as 
    /// an vertex buffer object for the given texture unit.
    virtual void renderVertexBufferObjectForTextureUnit( unsigned int texture_unit );

    /// Perform the OpenGL commands to render all texture coordinates as 
    /// an vertex buffer object for the texture units between and
    /// including start_unit and end_unit.
    void renderVertexBufferObjectForTextureUnits( unsigned int start_unit,
                                     unsigned int end_unit );

    /// Implement the method to specify data and releated information
    virtual void setAttributeData ( ){};

    /// VBO rendering implementation
    virtual void renderVBO ( ){};

    /// VBO disabling implementation
    virtual void disableVBO ( ){};

    /// Disable the vertex buffer object state state enabled in
    /// renderVertexBufferObjectForTexture
    void disableVertexBufferObjectForTexture( X3DTextureNode *t );

    /// Disable the vertex buffer object state enabled in
    /// renderVertexBufferObjectForActiveTexture
    void disableVertexBufferObjectForActiveTexture();

    /// Disable the vertex buffer object state enabled in
    /// renderVertexBufferObjectForTextureUnit().
    virtual void disableVertexBufferObjectForTextureUnit( unsigned int texture_unit );

    /// Disable the vertex buffer object state enabled in
    /// renderVertexBufferObjectForTextureUnits().
    void disableVertexBufferObjectForTextureUnits( unsigned int start_unit,
                                      unsigned int end_unit );

    ////////////////////////////////////////////////////////////////
    // Functions for nodes supporting texture coordinate generation

    /// Returns true if the node supports rendering of explicit texture 
    /// coordinates.
    virtual bool supportsTexGen() {
      return false;
    } 

    /// Start hardware texture coordinate generation. Algorithm depends
    /// on the texture coordinate node
    virtual void startTexGen() {}

    /// Stop the texture coordinate generation started with startTexGen().
    virtual void stopTexGen() {}

    /// Returns true if the getTexCoord function is available for use.
    /// Some X3DTextureCoordinate nodes do not support getting a coordinate
    /// by an index and will return false(e.g. TextureCoordinateGenerator)
    inline virtual bool supportsGetTexCoord( unsigned int texture_unit ) {
      return false;
    }

    /// Gets texture coordinate of the given index and texture unit.
    /// This function is only valid if supporteGetTexCoord returns true.
    /// TextureCoordinateGenerator e.g. does not support this function.
    inline virtual Vec4f getTexCoord( int index, unsigned int texture_unit ) {
      return Vec4f( 0, 0, 0, 1 );
    }

    /// Start hardware texture coordinate generateion for all texture 
    /// units used by the texture 
    virtual void startTexGenForTexture( X3DTextureNode *t );

    /// Start hardware texture coordinate generateion for all texture 
    /// units used by the texture in X3DTextureNode::getActiveTexture.
    void startTexGenForActiveTexture();
    
    /// Stop texture coordinate generateion for all texture 
    /// units used by the texture 
    virtual void stopTexGenForTexture( X3DTextureNode *t );

    /// Stop hardware texture coordinate generateion for all texture 
    /// units used by the texture in X3DTextureNode::getActiveTexture.
    void stopTexGenForActiveTexture();

    /// Start hardware texture coordinate generation for the texture units 
    /// between and including start_unit and end_unit. Algorithm depends
    /// on the texture coordinate node
    void startTexGenForTextureUnits( unsigned int start_unit,
                                     unsigned int end_unit );

    /// Start hardware texture coordinate generation for the texture unit
    /// specified. Algorithm depends on the texture coordinate node
    virtual void startTexGenForTextureUnit( unsigned int texture_unit );

    /// Stop the texture coordinate generation started with 
    /// startTexGenForTextureUnits().
    void stopTexGenForTextureUnits( unsigned int start_unit,
                                    unsigned int end_unit );

    /// Stop hardware texture coordinate generation for the texture unit
    /// specified. Algorithm depends on the texture coordinate node
    virtual void stopTexGenForTextureUnit( unsigned int texture_unit );

    /// Returns the default xml containerField attribute value.
    /// For this node it is "texCoord".
    virtual string defaultXMLContainerField() {
      return "texCoord";
    }

    protected:
      /// the current texture index
      unsigned int texture_index;
  };
}

#endif
