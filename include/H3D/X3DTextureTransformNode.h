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
/// \file X3DTextureTransformNode.h
/// \brief Header file for X3DTextureTransformNode, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __X3DTEXTURETRANSFORMNODE_H__
#define __X3DTEXTURETRANSFORMNODE_H__

#include <H3D/X3DAppearanceChildNode.h>
namespace H3D {
  /// \ingroup AbstractNodes
  /// \class X3DTextureTransformNode
  /// \brief This abstract node type is the base type for all node types which
  /// specify a transformation of texture coordinates. 
  ///
  /// 
  class H3DAPI_API X3DTextureTransformNode : public X3DAppearanceChildNode {
  public:
    
    /// The rendering of texture transformation requires the specifiation
    /// of which texture units to render the transformation for. Therefore
    /// callList() cannot be called directly and no display list will be
    /// created. Use the render() and renderForMultipleTextures() funcions
    /// instead.
    class H3DAPI_API DisplayList: public H3DDisplayListObject::DisplayList {
      H3D_API_EXCEPTION( InvalidCallToCallList );
      
      /// Perform front face code outside the display list.
      virtual void callList( bool build_list ) {
        stringstream s;
        s << "Cannot call callList() on the DisplayList of an "
          << "X3DTextureTransformNode since the rendering requires "
          << "specification of which texture units to render for. Use "
          << "render() or renderForMultipleTexture() instead.";
        throw InvalidCallToCallList( s.str(), H3D_FULL_LOCATION );
      }
    };

    /// Constructor.
    X3DTextureTransformNode( Inst< DisplayList > _displayList = 0,
                             Inst< SFNode>  _metadata = 0 );

    /// Render the texture transform for the texture units between and
    /// including start_unit and end_unit.
    virtual void renderForTextureUnits( unsigned int start_unit,
                                        unsigned int end_unit );

    /// Render the texture transform for the specified texture unit.
    virtual void renderForTextureUnit( unsigned int texture_unit );

    /// This function will be called by the X3DShapeNode before any rendering 
    /// of geometry and before the call to the render function. So this is the
    /// place to save the states that are going to be changed in render() in
    /// order to restore it in postRender().
    virtual void preRender();

    /// This function will be called by the X3DShapeNode after the geometry
    /// has been rendered to restore the states to what it was before 
    /// the call to preRender().
    virtual void postRender();

    /// Returns the default xml containerField attribute value.
    /// For this node it is "textureTransform".
    ///
    virtual string defaultXMLContainerField() {
      return "textureTransform";
    }
  };
}

#endif
