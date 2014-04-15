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
/// \file MultiTextureCoordinate.h
/// \brief Header file for MultiTextureCoordinate, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __MULTITEXTURECOORDINATE_H__
#define __MULTITEXTURECOORDINATE_H__

#include <H3D/X3DTextureCoordinateNode.h>
#include <H3D/DependentNodeFields.h>
#include <H3D/MFNode.h>

namespace H3D {

  /// \class MultiTextureCoordinate
  /// \brief MultiTextureCoordinate supplies multiple texture coordinates
  /// per vertex.
  /// This node can be used to set the texture coordinates for the different
  /// texture channels.
  ///
  /// By default, if using MultiTexture with an IndexedFaceSet without a 
  /// MultiTextureCoordinate texCoord node, texture coordinates for channel
  /// 0 are replicated along the other channels. Likewise, if there are too
  /// few entries in the texCoord field, the last entry is replicated. 
  ///
  /// Example:
  /// \code
  /// Shape { 
  ///   appearance Appearance { 
  ///     texture MultiTexture {  
  ///       mode [ "MODULATE" "MODULATE" ] 
  ///       texture [ 
  ///         ImageTexture { url "brick.jpg")  
  ///         ImageTexture { repeatS FALSE repeatT FALSE url "light_gray.png"} 
  ///       ]
  ///     }
  ///   }
  ///   geometry IndexedFaceSet {  
  ///     ... 
  ///     texCoord MultiTextureCoord {
  ///       texCoord [ 
  ///         TextureCoordinate { ... } 
  ///         TextureCoordinate { ... } 
  ///       ] 
  ///     }
  ///   }
  /// }
  /// \endcode
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/MultiTextureCoordinate.x3d">MultiTextureCoordinate.x3d</a>
  ///     ( <a href="examples/MultiTextureCoordinate.x3d.html">Source</a> )
  class H3DAPI_API MultiTextureCoordinate : public X3DTextureCoordinateNode {
  public:

    /// The MFTextureCoordinateNode is dependent on the propertyChanged 
    /// field of the contained X3DTextureCoordinateNodes.
    typedef DependentMFNode< 
                X3DTextureCoordinateNode,
                FieldRef< X3DGeometricPropertyNode,
                          Field,
                          &X3DTextureCoordinateNode::propertyChanged >, true >
    MFTextureCoordinateNode;

    /// Constructor.
    MultiTextureCoordinate( Inst< SFNode                  > _metadata = 0,
                            Inst< MFTextureCoordinateNode > _texCoord = 0 );

    /// Returns true if any of the nodes in the texCoord field supports
    /// explicit coordinates
    virtual bool supportsExplicitTexCoords();

    /// Returns true if any of the nodes in the texCoord field supports
    /// texture coordinate generation
    virtual bool supportsTexGen();

    /// This function call is not allowed. Use renderForTextureUnit() or 
    /// renderForTextureUnits() instead. Call will raise an exception.
    virtual void render( int index );

    /// Render the texture coordinate for the currently active texture unit.
    virtual void renderForTextureUnit( int index,
                                       unsigned int texture_unit );
    
    /// Returns the number of texture coordinates this node can render.
    virtual unsigned int nrAvailableTexCoords();

    /// This function all is not allowed. Use renderForTextureUnit() or 
    /// renderForTextureUnits() instead. Call will raise an exception.
    virtual void renderArray();

    /// Perform the OpenGL commands to render all texture coordinates as 
    /// an array for the given texture unit.
    virtual void renderArrayForTextureUnit( unsigned int texture_unit );
    
    /// Disable the array state enabled in renderArrayForTextureUnit().
    virtual void disableArrayForTextureUnit( unsigned int texture_unit );

    /// Perform the OpenGL commands to render all vertices as a vertex
    /// buffer object.
    virtual void renderVertexBufferObject();

    /// Perform the OpenGL commands to render all texture coordinates as 
    /// an vertex buffer object for the given texture unit.
    virtual void renderVertexBufferObjectForTextureUnit(
      unsigned int texture_unit );

    /// Disable the array state enabled in renderArrayForTextureUnit().
    virtual void disableVertexBufferObjectForTextureUnit(
      unsigned int texture_unit );

    /// Start hardware texture coordinate generation. 
    virtual void startTexGen();

    /// Start hardware texture coordinate generation. 
    virtual void stopTexGen();

    /// Start hardware texture coordinate generation for the given texture
    /// unit.
    virtual void startTexGenForTextureUnit( unsigned int texture_unit);

    /// Stop the texture coordinate generation started with startTexGen().
    virtual void stopTexGenForTextureUnit( unsigned int texture_unit );

    /// Returns true if the getTexCoord function is available for use.
    virtual bool supportsGetTexCoord( unsigned int texture_unit );

    /// Gets texture coordinate of the given index and texture unit.
    virtual Vec4f getTexCoord( int index, unsigned int texture_unit );

    /// The X3DTextureCoordinate nodes to use for each texture unit.
    /// 
    /// <b>Access type:</b> inputOutput \n
    auto_ptr< MFTextureCoordinateNode > texCoord;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  };
}

#endif
