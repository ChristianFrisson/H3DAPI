//////////////////////////////////////////////////////////////////////////////
//    Copyright 2004, SenseGraphics AB
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

#include "X3DGeometricPropertyNode.h"

namespace H3D {

  /// \ingroup AbstractNodes
  /// \class X3DTextureCoordinateNode
  /// \brief This abstract node type is the base type for all node types which
  /// specify texture coordinates. It adds a new geometric property node
  /// type to those specified in 11 Rendering component.
  ///
  /// 
  class H3DAPI_API X3DTextureCoordinateNode : public X3DGeometricPropertyNode {
  public:
    
    /// Constructor.
    X3DTextureCoordinateNode( Inst< SFNode>  _metadata = 0 );

    /// Perform the OpenGL commands to render a texture coordinate given the 
    /// index of the texture coordinate.
    virtual void render( int index ) {}

    /// Render the texture transform for the currently active  texture unit.
    virtual void renderForTextureUnit( int index,
                                       unsigned int texture_unit ) {}
    
    /// Render the texture transform for the texture units between and
    /// inluding start_unit and end_unit.
    virtual void renderForTextureUnits( int index,
                                        unsigned int start_unit,
                                        unsigned int end_unit );
    
    /// Returns the default xml containerField attribute value.
    /// For this node it is "texCoord".
    virtual string defaultXMLContainerField() {
      return "texCoord";
    }

    /// Returns the number of texture coordinates this node can render.
    virtual unsigned int nrAvailableTexCoords() = 0;

    /// Perform the OpenGL commands to render all texture coordinates as 
    /// an arary.
    virtual void renderArray() {}

    /// Perform the OpenGL commands to render all texture coordinates as 
    /// an array for the given texture unit.
    virtual void renderArrayForTextureUnit( unsigned int texture_unit ) {}
    
    /// Perform the OpenGL commands to render all texture coordinates as 
    /// an array for the texture units between and
    /// inluding start_unit and end_unit.
    virtual void renderArrayForTextureUnits( unsigned int start_unit,
                                             unsigned int end_unit );
    
    /// Disable the array state enabled in renderArray().
    virtual void disableArray() {}

    /// Disable the array state enabled in renderArrayForTextureUnit().
    virtual void disableArrayForTextureUnit( unsigned int texture_unit ) {}

    /// Disable the array state enabled in renderArrayForTextureUnits().
    virtual void disableArrayForTextureUnits( unsigned int start_unit,
                                              unsigned int end_unit );

  };
}

#endif
