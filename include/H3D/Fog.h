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
/// \file Fog.h
/// \brief Header file for Fog, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __FOG_H__
#define __FOG_H__

#include <H3D/X3DBindableNode.h>
#include <H3D/X3DFogObject.h>

namespace H3D {
  /// \ingroup X3DNodes
  /// \class Fog
  /// \brief The Fog node provides a way to simulate atmospheric effects 
  /// by blending objects with the colour specified by the color field based
  /// on the distances of the various objects from the viewer. 
  /// The distances are calculated in the coordinate space of the Fog node.
  /// The visibilityRange specifies the distance in metres (in the local 
  /// coordinate system) at which objects are totally obscured by the fog. 
  /// Objects located outside the visibilityRange from the viewer are drawn 
  /// with a constant colour of color. Objects very close to the viewer are
  /// blended very little with the fog color. A visibilityRange of 0.0 disables
  /// the Fog node. The visibilityRange is affected by the scaling 
  /// transformations of the Fog node's parents; translations and rotations 
  /// have no affect on visibilityRange. 
  ///
  ///
  /// The fogType field controls how much of the fog colour is blended with
  /// the object as a function of distance. If fogType is "LINEAR", the amount
  /// of blending is a linear function of the distance, resulting in a depth
  /// cueing effect. If fogType is "EXPONENTIAL," an exponential increase
  /// in blending is used, resulting in a more natural fog appearance.
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/Fog.x3d">Fog.x3d</a>
  ///     ( <a href="examples/Fog.x3d.html">Source</a> )
  class H3DAPI_API Fog : 
    public X3DBindableNode,
    public X3DFogObject {
  public:
    /// Construtor.
    Fog( Inst< SFSetBind > _set_bind         = 0,
         Inst< SFNode    > _metadata         = 0,
         Inst< SFTime    > _bindTime         = 0,
         Inst< SFBool    > _isBound          = 0,
         Inst< SFColor   > _color            = 0,
         Inst< SFString  > _fogType          = 0,
         Inst< SFFloat   > _visibilityRange  = 0 );

    /// Convenience function to get the top of the Fog stack.
    static inline Fog *getActive() {
      return static_cast< Fog * >( X3DBindableNode::getActive( "Fog" ) );
    }

    /// Render the global fog with OpenGL.
    virtual void renderFog();

    /// Traverse the scenegraph. Set the scale_local_to_global variable.
    /// \param ti The TraverseInfo object containing information about the
    /// traversal.
    virtual void traverseSG( TraverseInfo &ti ) {
      Matrix3f m = ti.getAccForwardMatrix().getScaleRotationPart();
      Vec3f scaled_v = m * Vec3f( 0, 0, 1 );
      scale_local_to_global = scaled_v.length();
    }
   
    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  protected:
    /// The scaling factor from local to global space.
    H3DFloat scale_local_to_global;

  };
}

#endif
