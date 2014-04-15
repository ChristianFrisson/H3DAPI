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
/// \file X3DAppearanceNode.h
/// \brief Header file for X3DAppearanceNode, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __X3DAPPEARANCENODE_H__
#define __X3DAPPEARANCENODE_H__

#include <H3D/X3DNode.h>
#include <H3D/H3DDisplayListObject.h>
#include <H3D/H3DSurfaceNode.h>

namespace H3D {

  /// \ingroup AbstractNodes
  /// \class X3DAppearanceNode
  /// \brief This is the base node type for all Appearance nodes.
  ///
  /// 
  class H3DAPI_API X3DAppearanceNode : 
    public X3DNode, public H3DDisplayListObject {
  public:
    typedef TypedSFNode< H3DSurfaceNode > SFSurface;

    /// Constructor.
    X3DAppearanceNode( Inst< DisplayList > _displayList = 0,
                       Inst< SFNode    > _metadata = 0,
                       Inst< SFSurface > _surface  = 0 );


    /// This function will be called by the X3DShapeNode before any rendering 
    /// of geometry and before the call to the render function. 
    virtual void preRender() {}

    /// This function will be called by the X3DShapeNode after the geometry
    /// has been rendered.
    virtual void postRender() {}

    /// Returns a bitmask of the OpenGL attrib bits that will be affected
    /// by this node. The value can be used in glPushAttrib in order
    /// to save the current state.
    virtual GLbitfield getAffectedGLAttribs() { return 0; }
    
    /// This function checks the transparency field to determine if the
    /// material requires that the geometry is rendered with transparency
    virtual bool isTransparent() {
      return false;
    }

    /// This function return true if the appearance has decided that
    /// shadow should be cast. 
    virtual bool hasGeometryShadow() {
      return false;
    }

    /// This function checks if multi-pass transparency should be used or not
    /// (see RenderProperties_multiPassTransparency)
    virtual bool usingMultiPassTransparency() {
      return default_using_multi_pass_transparency;
    }
        
    /// Returns the default xml containerField attribute value.
    /// For this node it is "appearance".
    ///
    virtual string defaultXMLContainerField() {
      return "appearance";
    }

    /// Traversing the scene graph. The current surface will be set to the 
    /// surface of the X3DAppearanceNode.
    /// \param ti The TraverseInfo object containing information about the
    /// traversal.
    virtual void traverseSG( TraverseInfo &ti );

    /// Contains the Surface node that will determine how an object will 
    /// feel haptically.
    ///
    /// <b>Access type:</b> inputOutput \n
    auto_ptr< SFSurface >  surface;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;

    /// Set the default value for usage of multi pass transparancy. This
    /// is the value that will be used if the inherited appearance node
    /// does not contain any information about using multipass transparency
    inline static void setDefaultUsingMultiPassTransparency( bool b ) {
      default_using_multi_pass_transparency = b;
    }

    /// Get the default value for usage of multi pass transparancy.
    inline static bool getDefaultUsingMultiPassTransparency() {
      return default_using_multi_pass_transparency;
    }

  protected:
    /// The default value to use for multipass transparency, if the 
    /// inherited appearance node does not contain any information about
    /// using multipass transparency
    static bool default_using_multi_pass_transparency;
  };
}

#endif
