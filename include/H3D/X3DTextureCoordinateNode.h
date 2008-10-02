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
/// \file X3DTextureCoordinateNode.h
/// \brief Header file for X3DTextureCoordinateNode, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __X3DTEXTURECOORDINATENODE_H__
#define __X3DTEXTURECOORDINATENODE_H__

#include <H3D/X3DGeometricPropertyNode.h>

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
  class H3DAPI_API X3DTextureCoordinateNode : public X3DGeometricPropertyNode {
  public:
    
    /// Constructor.
    X3DTextureCoordinateNode( Inst< SFNode>  _metadata = 0 );

    /// Render the texture coordinate for all texture units used by
    /// the texture in X3DTextureNode::getActiveTexture.
    static void renderTexCoordForActiveTexture( const Vec3f &tc );

    /// Render the texture coordinate for all texture units used by
    /// the given texture .
    static void renderTexCoordForTexture( const Vec3f &tc, X3DTextureNode *t );

    
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
  };
}

#endif
