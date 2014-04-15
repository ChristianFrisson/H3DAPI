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
/// \file ShadowCaster.h
/// \brief Header file for ShadowCaster.
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __SHADOWCASTER_H__
#define __SHADOWCASTER_H__

#include <H3D/H3DShadowObjectNode.h>

#include <H3D/X3DChildNode.h>
#include <H3D/X3DLightNode.h>
#include <H3D/MFNode.h>
#include <H3D/Shape.h> 
#include <H3D/SFString.h>
#include <H3D/X3DShaderNode.h>
#include <H3D/DEFNodes.h>

namespace H3D {

  class FrameBufferTextureGenerator;
  /// \ingroup H3DNodes
  /// \class ShadowCaster
  /// The ShadowCaster node uses shadow volumes using stencil buffer
  /// to cast shadows from objects in a scene. 
  /// 
  /// NOTE! Do not use this node unless you really need to. If you 
  /// just want to use shadows please use the shadow field in the 
  /// Appearance node for a much easier way of adding shadows.
  ///
  /// The object field specifies objects that should cast shadows.
  ///
  /// The light field specifies the lights from which the objects
  /// should cast shadows.
  ///
  /// The shadowDarkness field specifies how dark the shadow should be
  /// with 1 being totally black.
  /// 
  /// The algorithm field defines what shadow volume algorithm to use.
  /// Possible values are "ZPASS" and "ZFAIL". 
  /// Both have their pros and cons:
  /// - ZPASS is faster, but is not robust in all cases and fails 
  /// if the viewer is in shadow.
  /// - ZFAIL is slower, but more robust. To make it work properly 
  /// the far clip plane has to be set to infinity(done by setting 
  /// visibilityLimit to -1 in NavigationInfo). This will decrease
  /// the precision of the depth buffer, with all the problems
  /// that comes with that.
  ///
  ///  The shadowDepthOffset field controls the z offset of drawn 
  /// shadow volumes. The z offset will be r * defaultShadowDepthOffset
  /// where r is the smallest value that is guaranteed to produce a 
  /// resolvable z offset for a given implementation. If this value is
  /// set to too small there will be z-fighting between shadow and object
  /// that casts the shadow(flickering). If this happens increase this value.
  /// The value needed depends on the precision of the depth buffer.
  ///
  /// The shadowShader field can contain a shader to perform effects
  /// on the shadow volume texture(stencil buffer) before it is applied to the scene.
  /// Can be used e.g. with a GaussianFilterShader to make softer edges.
  /// Any shader put into the shadowShader field needs to have the field
  /// "texture" in it which will be used as input to the shadow volume texture.
  ///
  /// The ShadowCaster node is affected by the transform hierarchy
  /// that it is in and all objects and lighs are specified in 
  /// local coordinates.
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/ShadowCaster.x3d">ShadowCaster.x3d</a>
  ///     ( <a href="examples/ShadowCaster.x3d.html">Source</a> )
  ///
  ///
  /// \par Internal routes:
  /// \dotfile ShadowCaster.dot
  class H3DAPI_API ShadowCaster : public X3DChildNode,
                                  public H3DDisplayListObject {
  public:

    typedef TypedMFNode< X3DLightNode > MFLightNode;
    typedef TypedMFNode< H3DShadowObjectNode > MFShadowObjectNode;
    typedef TypedSFNode< X3DShaderNode > SFShaderNode;

    /// Constructor.
    ShadowCaster( Inst< SFNode             > _metadata = 0,
                  Inst< MFShadowObjectNode > _object = 0,
                  Inst< MFLightNode        > _light = 0,
                  Inst< SFFloat            > _shadowDarkness = 0,
                  Inst< SFFloat            > _shadowDepthOffset = 0,
                  Inst< DisplayList        > _displayList = 0,
                  Inst< SFString           > _algorithm = 0,
                  Inst< SFShaderNode       > _shadowShader = 0 );

    /// OpenGL render function.
    virtual void render();

    void addHeadLight();

    /// The objects that should cast shadows.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// \dotfile ShadowCaster_object.dot
    auto_ptr< MFShadowObjectNode > object;

    /// The lights that shines on the objects. Only 
    /// PointLight and DirectionalLight is supporeted at this time.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// \dotfile ShadowCaster_light.dot
    auto_ptr< MFLightNode > light;

    /// The shadowDarkness field controls the darkness of the shadow.
    /// A value of 1 will draw a completely black shadow, while
    /// a value of 0 will cast no shadow at all.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 0.4 \n
    /// \dotfile ShadowCaster_shadowDarkness.dot
    auto_ptr< SFFloat > shadowDarkness;

    /// The shadowDepthOffset field controls the z offset of drawn 
    /// shadow volumes. The z offset will be r * defaultShadowDepthOffset
    /// where r is the smallest value that is guaranteed to produce a 
    /// resolvable z offset for a given implementation. If this value is
    /// set to too small there will be z-fighting between shadow and object
    /// that casts the shadow(flickering). If this happens increase this value.
    /// The value needed depends on the precision of the depth buffer.
    ///
    /// <b>Default value: </b> 6 \n
    /// <b>Access type: </b> inputOutput \n
    auto_ptr< SFFloat > shadowDepthOffset;

    /// The algorithm field defines what shadow volume algorithm to use.
    /// Possible values are "ZPASS" and "ZFAIL". 
    /// Both have their pros and cons:
    /// - ZPASS is faster, but is not robust in all cases and fails 
    /// if the viewer is in shadow.
    /// - ZFAIL is slower, but more robust. To make it work properly 
    /// the far clip plane has to be set to infinity(done by setting 
    /// visibilityLimit to -1 in NavigationInfo). This will decrease
    /// the precision of the depth buffer, with all the problems
    /// that comes with that.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> "ZPASS" \n
    /// <b>Valid values:</b> "ZPASS" and "ZFAIL" \n
    /// \dotfile ShadowCaster_algorithm.dot
    auto_ptr< SFString > algorithm;

    /// The shadowShader field can contain a shader to perform effects
    /// on the shadow volume texture(stencil buffer) before it is applied to the scene.
    /// Can be used e.g. with a GaussianFilterShader to make softer edges.
    /// Any shader put into the shadowShader field needs to have the field
    /// "texture" in it which will be used as input to the shadow volume texture.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> NULL \n
    /// \dotfile ShadowCaster_shadowShader.dot
    auto_ptr< SFShaderNode > shadowShader;

    /// The H3DNodeDatablase object for this node.
    static H3DNodeDatabase database;

  protected:
    // Internal fbo that renders the main stencil buffer to a RGBA texture.
    AutoRef< Node > generator;

    // DEFNodes of the nodes in shape.
    X3D::DEFNodes dn;

    // holds the Shape used to render the texture from generator onto
    // a FullscreenRectangle with shadowShader applied.
    AutoRef<Shape> shape;

    // the shader that was used last loop.
    X3DShaderNode *last_shader;

    // Callback function to FrameBufferTextureGenerator to render the stencil buffer
    // to an fbo. 
    static void renderShadows( FrameBufferTextureGenerator *fbo, int i, void *args );
   
  };
}

#endif
