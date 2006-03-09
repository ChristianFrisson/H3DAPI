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
/// \file X3DLightNode.h
/// \brief Header file for X3DLightNode, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __X3DLIGHTNODE_H__
#define __X3DLIGHTNODE_H__

#include "X3DChildNode.h"
#include "H3DDisplayListObject.h"
#include "H3DRenderStateObject.h"
#include "SFFloat.h"
#include "SFColor.h"

namespace H3D {

  /// \ingroup AbstractNodes
  /// \class X3DLightNode
  /// \brief The X3DLightNode abstract node type is the base type from which
  /// all node types that serve as light sources are derived. 
  ///
  /// Any node used as a source of illumination is derived from X3DLightNode.
  /// All light sources contain an intensity, a color, and an ambientIntensity
  /// field. The intensity field specifies the brightness of the direct
  /// emission from the light, and the ambientIntensity specifies the 
  /// intensity of the ambient emission from the light. Light intensity may
  /// range from 0.0 (no light emission) to 1.0 (full intensity). The color
  /// field specifies the spectral colour properties of both the direct and
  /// ambient light emission as an RGB value. The on field specifies whether
  /// the light is enabled or disabled. If the value is FALSE, then the light
  /// is disabled and will not affect any nodes in the scene. If the value
  /// is TRUE, then the light will affect other nodes.
  ///
  /// \par Internal routes:
  /// \dotfile X3DLightNode.dot
  class H3DAPI_API X3DLightNode : 
    public X3DChildNode,
    public H3DDisplayListObject,
    public H3DRenderStateObject {
  public:
     
    /// Constructor.
    X3DLightNode( Inst< SFNode >  _metadata         = 0,
                  Inst< SFFloat>  _ambientIntensity = 0,
                  Inst< SFColor>  _color            = 0,
                  Inst< SFFloat>  _intensity        = 0,
                  Inst< SFBool >  _on               = 0 );

    /// Turn the light on.
    virtual void enableGraphicsState();

    /// Turn the light off.
    virtual void disableGraphicsState();
  
    /// Specifies the intensity of the ambient emission from the light.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 0 \n
    /// <b>Valid range:</b> [0-1]
    /// 
    /// \dotfile X3DLightNode_ambientIntensity.dot
    auto_ptr< SFFloat >  ambientIntensity;

    /// The color field specifies the spectral colour properties of both 
    /// the direct and ambient light emission as an RGB value.
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> RGB( 1, 1, 1 ) \n
    /// <b>Valid range:</b> [0-1]
    /// 
    /// \dotfile X3DLightNode_color.dot
    auto_ptr< SFColor >  color;

    /// The intensity field specifies the brightness of the direct emission
    /// from the light.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 1 \n
    /// <b>Valid range:</b> [0-1]
    /// 
    /// \dotfile X3DLightNode_intensity.dot
    auto_ptr< SFFloat >  intensity;

    /// The on field specifies whether the light is enabled or disabled. If 
    /// the value is FALSE, then the light is disabled and will not affect
    /// any nodes in the scene.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> TRUE \n
    /// 
    /// \dotfile X3DLightNode_on.dot
    auto_ptr< SFBool  >  on;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;

  protected:
    GLuint light_index;
    GLint max_lights;
    static GLuint global_light_index;
  };
}

#endif
