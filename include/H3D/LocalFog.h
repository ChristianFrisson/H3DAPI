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
/// \file LocalFog.h
/// \brief Header file for LocalFog, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __LOCALFOG_H__
#define __LOCALFOG_H__

#include <H3D/X3DChildNode.h>
#include <H3D/X3DFogObject.h>
#include <H3D/H3DRenderStateObject.h>
#include <H3D/SFBool.h>

namespace H3D {
  /// \ingroup X3DNodes
  /// \class LocalFog
  /// \brief The LocalFog node provides a way to simulate atmospheric effects
  /// by blending objects with the colour specified by the color field based
  /// on the distances of the various objects from the viewer. 
  /// The distances are calculated in the coordinate space of the LocalFog
  /// node. The visibilityRange field specifies the distance in metres (in
  /// the local coordinate system) at which objects are totally obscured by 
  /// the fog. Objects located outside the visibilityRange from the viewer are
  /// drawn with a constant colour of color. Objects very close to the viewer
  /// are blended very little with the fog color. A visibilityRange of 0.0 
  /// disables the LocalFog node. The visibilityRange is affected by the
  /// scaling transformations of the LocalFog node’s parents; translations
  /// and rotations have no affect on visibilityRange.
  ///
  /// The fogType field controls how much of the fog colour is blended with
  /// the object as a function of distance. If fogType is "LINEAR", the amount
  /// of blending is a linear function of the distance, resulting in a depth
  /// cueing effect. If fogType is "EXPONENTIAL", an exponential increase in
  /// blending is used, resulting in a more natural fog appearance.
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/LocalFog.x3d">LocalFog.x3d</a>
  ///     ( <a href="examples/LocalFog.x3d.html">Source</a> )
  class H3DAPI_API LocalFog : 
    public X3DChildNode,
    public X3DFogObject,
    public H3DRenderStateObject {
  public:
    /// Construtor.
    LocalFog( Inst< SFNode    > _metadata         = 0,
              Inst< SFColor   > _color            = 0,
              Inst< SFString  > _localFogType     = 0,
              Inst< SFFloat   > _visibilityRange  = 0,
              Inst< SFBool   > _enabled  = 0 );

    /// Set up and enable the fog with OpenGL.
    virtual void enableGraphicsState();

    /// Restore the previous attribs.
    virtual void disableGraphicsState();

    /// If enabled is true the LocalFog node is enabled otherwise it is
    /// disabled and will not affect the scene.
    ///
    /// <b>Access type:</b> inputOutput
    /// <b>Default value:</b> TRUE \n
    /// 
    /// \dotfile LocalFog_enabled.dot
    auto_ptr< SFBool >  enabled;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  };
}

#endif
