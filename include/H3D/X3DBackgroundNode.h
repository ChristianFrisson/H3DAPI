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
/// \file X3DBackgroundNode.h
/// \brief Header file for X3DBackgroundNode, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __X3DBACKGROUNDNODE_H__
#define __X3DBACKGROUNDNODE_H__

#include <H3D/X3DBindableNode.h>
#include <H3D/H3DDisplayListObject.h>
#include <H3D/MFFloat.h>
#include <H3D/MFColor.h>
#include <H3D/SFMatrix4f.h>
#include <H3D/SFMatrix4d.h>
#include <H3D/SFFloat.h>

namespace H3D {

  /// \ingroup AbstractNodes
  /// \class X3DBackgroundNode
  /// \brief X3DBackgroundNode is the abstract type from which all 
  /// backgrounds inherit. X3DBackgroundNode is a bindable node that when
  /// bound to defines the panoramic background for the scene.
  ///
  /// Background nodes are used to specify a colour backdrop that simulates
  /// ground and sky, as well as a background texture, or panorama, that is
  /// placed behind all geometry in the scene and in front of the ground and
  /// sky. Background nodes are specified in the local coordinate system and
  /// are affected by the accumulated rotation of their ancestors as described
  /// below. X3D supports two kinds of background nodes: a simple background 
  /// node that contains a set of url fields for specifying static image files
  /// that compose the backdrop (see Background), and a complex background
  /// node containing arbitrary X3DTextureNode nodes that compose the backdrop
  /// (see TextureBackground). Both types of background node descend from the
  /// base type X3DBackgroundNode. Applications should use the Background
  /// node for simplicity, and the TextureBackground node for more 
  /// flexibility and additional features.
  ///
  /// Background nodes are bindable nodes. There exists a Background stack,
  /// in which the top-most Background on the stack is the currently active
  /// Background. To move a Background to the top of the stack, a TRUE value
  /// is sent to the set_bind field. Once active, the Background is then
  /// bound to the browsers view. A FALSE value sent to set_bind removes
  /// the Background from the stack and unbinds it from the browser's view.
  ///
  /// The backdrop is conceptually a partial sphere (the ground) enclosed 
  /// inside of a full sphere (the sky) in the local coordinate system with 
  /// the viewer placed at the centre of the spheres. Both spheres have
  /// infinite radius and each is painted with concentric circles of 
  /// interpolated colour perpendicular to the local Y-axis of the sphere. 
  /// The Background node is subject to the accumulated rotations of its 
  /// ancestors' transformations. Scaling and translation transformations are
  /// ignored. The sky sphere is always slightly farther away from the viewer
  /// than the ground partial sphere causing the ground to appear in front of
  /// the sky where they overlap.
  ///
  /// The skyColor field specifies the colour of the sky at various angles on
  /// the sky sphere. The first value of the skyColor field specifies the 
  /// colour of the sky at 0.0 radians representing the zenith (i.e., 
  /// straight up from the viewer). The skyAngle field specifies the angles
  /// from the zenith in which concentric circles of colour appear. The zenith
  /// of the sphere is implicitly defined to be 0.0 radians, the natural 
  /// horizon is at pi/2 radians, and the nadir (i.e., straight down from the
  /// viewer) is at pi radians. skyAngle is restricted to non-decreasing
  /// values in the range [0.0, pi]. There shall be one more skyColor value
  /// than there are skyAngle values. The first colour value is the colour at
  /// the zenith, which is not specified in the skyAngle field. If the last
  /// skyAngle is less than pi, then the colour band between the last skyAngle
  /// and the nadir is clamped to the last skyColor. The sky colour is
  /// linearly interpolated between the specified skyColor values.
  ///
  /// The groundColor field specifies the colour of the ground at the
  /// various angles on the ground partial sphere. The first value of the
  /// groundColor field specifies the colour of the ground at 0.0 radians
  /// representing the nadir (i.e., straight down from the user). The 
  /// groundAngle field specifies the angles from the nadir that the 
  /// concentric circles of colour appear. The nadir of the sphere is 
  /// implicitly defined at 0.0 radians. groundAngle is restricted to 
  /// non-decreasing values in the range [0.0, pi/2]. There shall be one more
  /// groundColor value than there are groundAngle values. The first colour
  /// value is for the nadir which is not specified in the groundAngle field.
  /// If the last groundAngle is less than pi/2, the region between the last
  /// groundAngle and the equator is non-existant. The ground colour is 
  /// linearly interpolated between the specified groundColor values.
  ///
  /// Ground colours, sky colours, and panoramic images do not translate
  /// with respect to the viewer, though they do rotate with respect to the
  /// viewer. That is, the viewer can never get any closer to the background,
  /// but can turn to examine all sides of the panorama cube, and can look 
  /// up and down to see the concentric rings of ground and sky (if visible).
  ///
  /// Background nodes are not affected by Fog nodes. Therefore, if a 
  /// Background node is active (i.e., bound) while a Fog node is active,
  /// then the Background node will be displayed with no fogging effects. 
  /// It is the author's responsibility to set the Background values to match
  /// the Fog values (e.g.  ground colours fade to fog colour with 
  /// distance and panorama images tinted with fog colour). Background nodes
  /// are not affected by light sources.
  ///
  /// \par Internal routes:
  /// \dotfile X3DBackgroundNode.dot  
  class H3DAPI_API X3DBackgroundNode : 
    public X3DBindableNode {
  public:
    
    typedef H3DDisplayListObject::DisplayList DisplayList;

    /// Constructor.
    X3DBackgroundNode( Inst< SFSetBind > _set_bind    = 0,
           Inst< SFNode    > _metadata    = 0,
           Inst< SFTime    > _bindTime    = 0,
           Inst< SFBool    > _isBound     = 0,
           Inst< DisplayList > _displayList = 0,
           Inst< MFFloat   > _groundAngle = 0,
           Inst< MFColor   > _groundColor = 0,
           Inst< MFFloat   > _skyAngle    = 0,
           Inst< MFColor   > _skyColor    = 0,
           Inst< SFFloat   > _transparency = 0);

    /// Convenience function to get the top of the X3DBackgroundNode stack.
    static inline X3DBackgroundNode *getActive() {
      return static_cast< X3DBackgroundNode * >
        ( X3DBindableNode::getActive( "X3DBackgroundNode" ) );
    }

    /// Saves the accForwardMatrix for use in the render function.
    virtual void traverseSG( TraverseInfo &ti ) {
      X3DBindableNode::traverseSG( ti );
      if( localToGlobal->getValue() != ti.getAccForwardMatrix() ) {
        localToGlobal->setValue( ti.getAccForwardMatrix() );
      }
    }

    /// Renders the background with OpenGL. Have to have a special call
    /// to be used by GLWindow instead of render() to avoid the background
    /// node to be rendered as part of the scene graph. 
    virtual void renderBackground();

    /// Returns the color that OpenGL should clear the buffer with before
    /// starting to render to it. 
    virtual RGBA glClearColor() {
      if( skyColor->size() == 1 ) {
        RGB c = skyColor->getValueByIndex( 0 );
        H3DFloat a = 1.0f - transparency->getValue();
        return RGBA( c.r*a, c.g*a, c.b*a, a );
      } else  {
        return RGBA( 0, 0, 0, 0 );
      }
    }

    /// Render the background. It will render one sphere with radius 0.05
    /// for the sky and one half sphere with radius 0.95 * 0.05 for the 
    /// ground.
    virtual void render();

    /// The DisplayList instance handling the OpenGL caching of this object. 
    auto_ptr< DisplayList > displayList;

    /// The groundAngle field specifies the angles from the nadir that the
    /// concentric circles of colour appear. The nadir of the sphere is
    /// implicitly defined at 0.0 radians. groundAngle is restricted to
    /// non-decreasing values in the range [0.0, p/2]. 
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// 
    /// \dotfile X3DBackgroundNode_groundAngle.dot
    auto_ptr< MFFloat >  groundAngle;

 
    /// The groundColor field specifies the colour of the ground at the 
    /// various angles on the ground partial sphere. The first value of the
    /// groundColor field specifies the colour of the ground at 0.0 radians
    /// representing the nadir (i.e., straight down from the user). There
    /// shall be one more groundColor value than there are groundAngle values.
    /// The first colour value is for the nadir which is not specified in the
    /// groundAngle field. If the last groundAngle is less than p/2, the
    /// region between the last groundAngle and the equator is non-existant.
    /// The ground colour is linearly interpolated between the specified 
    /// groundColor values.
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// 
    /// \dotfile X3DBackgroundNode_groundAngle.dot
    auto_ptr< MFColor >  groundColor;

    /// The skyAngle field specifies the angles from the zenith in which
    /// concentric circles of colour appear. The zenith of the sphere
    /// is implicitly defined to be 0.0 radians, the natural horizon is
    /// at pi/2 radians, and the nadir (i.e., straight down from the
    /// viewer) is at pi radians.
    /// skyAngle is restricted to non-decreasing values in the range [0.0, pi].
    ///
    /// <b>Access type:</b> inputOutput \n
    /// 
    /// \dotfile X3DBackgroundNode_skyAngle.dot
    auto_ptr< MFFloat >  skyAngle;

    /// The skyColor field specifies the colour of the sky at various 
    /// angles on the sky sphere. The first value of the skyColor field 
    /// specifies the colour of the sky at 0.0 radians representing the
    /// zenith (i.e., straight up from the viewer). 
    /// There shall be one more skyColor value than there are skyAngle 
    /// values. The first colour value is the colour at the zenith, 
    /// which is not specified in the skyAngle field. If the last skyAngle
    /// is less than pi, then the colour band between the last skyAngle
    /// and the nadir is clamped to the last skyColor. The sky colour
    /// is linearly interpolated between the specified skyColor values.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// 
    /// \dotfile X3DBackgroundNode_groundAngle.dot
    auto_ptr< MFColor >  skyColor;

    /// The transparency field simply specifies the transparency for the
    /// sky and ground.
    ///
    /// <b>Access type:</b> inputOutput \n
    ///
    /// \dotfile X3DBackgroundNode_transparency.dot
    auto_ptr< SFFloat > transparency;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  protected:
    auto_ptr< SFMatrix4f > localToGlobal;

    /// projection matrix for background
    auto_ptr< SFMatrix4d > projectionMatrix;


    bool render_enabled;
  };
}

#endif

