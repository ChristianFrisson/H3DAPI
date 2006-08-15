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
/// \file RenderProperties.h
/// \brief Header file for RenderProperties, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __RENDERPROPERTIES_H__
#define __RENDERPROPERTIES_H__

#include "X3DAppearanceChildNode.h"
#include "SFColor.h"
#include "SFInt32.h"

namespace H3D {

  /// \ingroup H3DNodes
  /// \class RenderProperties
  /// \brief The RenderProperties node specifies additional rendering options
  /// that cannot be specified with the X3D Appearance node. 
  ///
  /// The lightingEnabled field specifies if the geometry should take 
  /// lighting into account when calculating colors. If false the color
  /// for each vertex will be used directly.
  ///
  /// The depthTestEnabled field specifies if depth test should be enabled
  /// or not. If depth test is disabled geometries will be drawn over the 
  /// current values in the frame buffer regardless of if it is in front of 
  /// or behind the previous objects drawn.
  ///
  /// The smoothShading field specifies if smooth shading should be used.
  /// If false, flat shading is used.
  /// 
  /// \par Internal routes:
  /// \dotfile RenderProperties.dot
  class H3DAPI_API RenderProperties : public X3DAppearanceChildNode {
  public:
    
    /// Constructor.
    RenderProperties( Inst< SFNode      >  _metadata   = 0 ,
                      Inst< DisplayList > _displayList      = 0,
                      Inst< SFBool      > _lightingEnabled  = 0,
                      Inst< SFBool      > _depthTestEnabled = 0,
                      Inst< SFBool      > _smoothShading    = 0 );

    /// This function will be called by the X3DShapeNode before any rendering 
    /// of geometry and before the call to the render function. So this is the
    /// place to save the states that are going to be changed in render() in
    /// order to restore it in postRender().
    virtual void preRender() {
      glPushAttrib( GL_LIGHTING_BIT | GL_ENABLE_BIT );
    }

    /// This function will be called by the X3DShapeNode after the geometry
    /// has been rendered to restore the states to what it was before 
    /// the call to preRender().
    virtual void postRender() {
      glPopAttrib();
    }

    /// Set up the line properties with OpenGL.
    virtual void render();

    /// Returns the default xml containerField attribute value.
    /// For this node it is "fillProperties".
    ///
    virtual string defaultXMLContainerField() {
      return "renderProperties";
    }
    
    /// The lightingEnabled field specifies if the geometry should take 
    /// lighting into account when calculating colors. If false the color
    /// for each vertex will be used directly.
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> true \n
    /// 
    /// \dotfile RenderProperties_lightingEnabled.dot 
    auto_ptr< SFBool  >  lightingEnabled;

    /// The depthTestEnabled field specifies if depth test should be enabled
    /// or not. If depth test is disabled geometries will be drawn over the 
    /// current values in the frame buffer regardless of if it is in front of 
    /// or behind the previous objects drawn.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> true \n
    /// 
    /// \dotfile RenderProperties_depthTestEnabled.dot 
    auto_ptr< SFBool >  depthTestEnabled;

    /// The smoothShading field specifies if smooth shading should be used.
    /// If false, flat shading is used.
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> true \n
    /// 
    /// \dotfile RenderProperties_smoothShading.dot 
    auto_ptr< SFBool > smoothShading;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  };
}

#endif
