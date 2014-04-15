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
/// \file DirectionalLight.h
/// \brief Header file for DirectionalLight, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __DIRECTIONALLIGHT_H__
#define __DIRECTIONALLIGHT_H__

#include <H3D/X3DLightNode.h>
#include <H3D/SFVec3f.h>

namespace H3D {

  /// \ingroup X3DNodes
  /// \class DirectionalLight
  /// \brief The DirectionalLight node defines a directional light source that
  /// illuminates along rays parallel to a given 3-dimensional vector. 
  ///
  /// The direction field specifies the direction vector of the illumination
  /// emanating from the light source in the local coordinate system. Light
  /// is emitted along parallel rays from an infinite distance away. A
  /// directional light source illuminates only the objects in its enclosing
  /// parent group. The light may illuminate everything within this
  /// coordinate system, including all children and descendants of its
  /// parent group. The accumulated transformations of the parent nodes
  /// affect the light. 
  ///
  /// DirectionalLight nodes do not attenuate with distance. 
  ///
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/DirectionalLight.x3d">DirectionalLight.x3d</a>
  ///     ( <a href="examples/DirectionalLight.x3d.html">Source</a> )
  ///
  /// \par Internal routes:
  /// \dotfile DirectionalLight.dot
  class H3DAPI_API DirectionalLight : public X3DLightNode {
  public:
    
    /// Constructor.
    DirectionalLight( Inst< SFNode >  _metadata         = 0,
                      Inst< SFFloat>  _ambientIntensity = 0,
                      Inst< SFColor>  _color            = 0,
                      Inst< SFFloat>  _intensity        = 0,
                      Inst< SFBool >  _on               = 0,
                      Inst< SFBool >  _global           = 0,
                      Inst< SFVec3f>  _direction        = 0 );

    /// Return the OpenGL parameters for this light.
    virtual GLLightInfo getGLLightInfo();

    /// Traversal function. Adding light to shadow caster.
    virtual void traverseSG( TraverseInfo &ti );

    /// The direction field specifies the direction vector of the
    /// illumination emanating from the light source in the local
    /// coordinate system. 
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> Vec3f( 0, 0, -1 ) \n
    /// 
    /// \dotfile DirectionalLight_direction.dot
    auto_ptr< SFVec3f >  direction;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  };
}

#endif
