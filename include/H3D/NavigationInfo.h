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
/// \file NavigationInfo.h
/// \brief Header file for NavigationInfo, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __NAVIGATIONINFO_H__
#define __NAVIGATIONINFO_H__

#include <H3D/X3DViewpointNode.h>
#include <H3D/MFFloat.h>
#include <H3D/MFString.h>

namespace H3D {
  /// \ingroup X3DNodes
  /// \class NavigationInfo
  /// \brief The NavigationInfo node contains information describing 
  /// the physical characteristics of the viewer's avatar and viewing model.
  /// 
  /// NavigationInfo node is a bindable node. The range field specifies the
  /// ideal distances (termed level distances) at which to switch between the
  /// levels. If forceTransitions is TRUE, browsers shall switch levels at the
  /// level distances specified. If forceTransistions is FALSE, browsers may
  /// disregard level distances in order to provide better performance.
  /// Whenever the current Viewpoint nodes changes, the current NavigationInfo
  /// node shall be re-parented to it by the browser. Whenever the current
  /// NavigationInfo node changes, the new NavigationInfo node shall be
  /// re-parented to the current Viewpoint node by the browser.
  ///
  /// If a TRUE value is sent to the set_bind field of a NavigationInfo node,
  /// the node is pushed onto the top of the NavigationInfo node stack. When a
  /// NavigationInfo node is bound, the browser uses the fields of the
  /// NavigationInfo node to set the navigation controls of its user interface
  /// and the NavigationInfo node is conceptually re-parented under the
  /// currently bound Viewpoint node. All subsequent scaling changes to the
  /// current Viewpoint node's coordinate system automatically change aspects
  /// (see below) of the NavigationInfo node values used in the browser
  /// (e.g., scale changes to any ancestors' transformations). A FALSE value
  /// sent to set_bind pops the NavigationInfo node from the stack, results in
  /// an isBound FALSE event, and pops to the next entry in the stack which
  /// shall be re-parented to the current Viewpoint node. 7.2.2 Bindable
  /// children nodes has more details on binding stacks.
  ///
  /// The type field specifies an ordered list of navigation paradigms that
  /// specify a combination of navigation types and the initial navigation
  /// type. The navigation type of the currently bound NavigationInfo node
  /// determines the user interface capabilities of the browser. For example,
  /// if the currently bound NavigationInfo node's type is "WALK", the browser
  /// shall present a WALK navigation user interface paradigm (see below for
  /// description of WALK). Browsers shall recognize at least the following
  /// navigation types: "ANY", "WALK", "EXAMINE", "FLY", "LOOKAT", and "NONE"
  /// and with support as specified in Table 23.2.
  ///
  /// If "ANY" does not appear in the type field list of the currently bound
  /// NavigationInfo, the browser's navigation user interface shall be
  /// restricted to the recognized navigation types specified in the list.
  /// In this case, browsers shall not present a user interface that allows
  /// the navigation type to be changed to a type not specified in the list.
  /// However, if any one of the values in the type field are "ANY", the
  /// browser may provide any type of navigation interface, and allow the
  /// user to change the navigation type dynamically. Furthermore, the first
  /// recognized type in the list shall be the initial navigation type
  /// presented by the browser's user interface.
  ///
  /// ANY navigation specifies that the browser may choose the navigation
  /// paradigm that best suits the content and provide a user interface to
  /// allow the user to change the navigation paradigm dynamically. The results
  /// are undefined if the currently bound NavigationInfo's type value is "ANY"
  /// and Viewpoint transitions are triggered by the Anchor node or the
  /// loadURL() scripting method (neither is implemented yet in H3DAPI).
  ///
  /// WALK navigation is used for exploring a virtual world on foot or in a
  /// vehicle that rests on or hovers above the ground. It is strongly
  /// recommended that WALK navigation define the up vector in the +Y direction
  /// and provide some form of terrain following and gravity in order to
  /// produce a walking or driving experience. If the bound NavigationInfo's
  /// type is "WALK", the browser shall strictly support collision detection.
  ///
  /// FLY navigation is similar to WALK except that terrain following and
  /// gravity may be disabled or ignored. There shall still be some notion of
  /// "up" however. If the bound NavigationInfo's type is "FLY", the browser
  /// shall strictly support collision detection.
  ///
  /// LOOKAT navigation is used to explore a scene by navigating to a
  /// particular object. Selecting an object with LOOKAT:
  /// <ul>
  /// <li>Moves the viewpoint directly to some convenient viewing distance
  /// from the bounding box center of the selected object, with the viewpoint
  /// orientation set to aim the view at the approximate "center" of the
  /// object.</a></li>
  /// <li>Sets the center of rotation in the currently bound Viewpoint node to
  /// the approximate "center" of the selected object.</a></li>
  /// </ul>
  ///
  /// EXAMINE navigation is used for viewing individual objects. EXAMINE shall
  /// provide the ability to orbit or spin the user's eyepoint about the center
  /// of rotation in response to user actions. The center of rotation for
  /// moving the viewpoint around the object and determining the viewpoint
  /// orientation is specified in the currently bound Viewpoint node. The
  /// browser shall strictly support collision detection (see Collision) and
  /// shall trigger exit and enter events throughout EXAMINE operations.
  ///
  /// LOOKAT navigation in combination with EXAMINE is used to explore a scene
  /// by navigating to a particular object, then being able to conveniently
  /// navigate in order to examine the object from different orientations.
  /// If content specifies both LOOKAT and EXAMINE types, any LOOKAT operations
  /// shall change the center of rotation for subsequent EXAMINE operations.
  ///
  /// NONE navigation disables and removes all browser-specific navigation user
  /// interface forcing the user to navigate using only mechanisms provided in
  /// the scene, such as Anchor nodes or scripts that include loadURL().
  /// NONE has an effect only when it is the first supported navigation type.
  /// If NONE is not the first supported navigation type, it has no effect.
  ///
  /// If the NavigationInfo type is "WALK", "FLY", "EXAMINE", or "NONE" or a
  /// combination of these types (i.e., "ANY" is not in the list), Viewpoint
  /// transitions triggered by the Anchor node or the loadURL()scripting method
  /// shall be implemented as a jump from the old Viewpoint to the new
  /// Viewpoint with transition effects that shall not trigger events besides
  /// the exit and enter events caused by the jump.
  ///
  /// Browsers may create browser-specific navigation type extensions. It is
  /// recommended that extended type names include a unique suffix
  /// (e.g., HELICOPTER_mydomain.com) to prevent conflicts. Viewpoint
  /// transitions triggered by the Anchor node or the loadURL()scripting method
  /// are undefined for extended navigation types. If none of the types are
  /// recognized by the browser, the default "ANY" is used. These strings
  /// values are case sensitive ("any" is not equal to "ANY").
  ///
  /// The transitionType field specifies an ordered list of paradigms that
  /// determine the manner in which the browser moves the viewer when a new
  /// Viewpoint node is bound. Browsers shall recognize and support at least
  /// the following transition types: "TELEPORT", "LINEAR", and "ANIMATE".
  /// For value "TELEPORT", the transition shall be immediate without any
  /// intervening positions. For value "LINEAR", the browser shall perform
  /// a linear interpolation of the position and orientation values.
  /// For value "ANIMATE", the browser shall perform a browser-specific
  /// animation effect. If all values are unrecognized or the field is empty,
  /// the default value of "LINEAR" shall be used. This field applies to any
  /// transitions between positions and orientations including Viewpoint
  /// bindings and LOOKAT navigation type.
  ///
  /// The transitionTime field specifies the duration of any viewpoint
  /// transition. The transition starts when the next Viewpoint node is bound.
  /// The duration of the transition depends on the value of the transitionType
  /// field. If transitionType is "TELEPORT", the transition is instantaneous
  /// and completes at the same time it starts. A transition type of "LINEAR"
  /// indicates that the transition lasts the number of seconds specified by
  /// the first value in the transitionTime field. If transitionType is
  /// "ANIMATE", transitionTime provides browser-dependent parameters to the
  /// browsers viewpoint animation engine. When a transition completes,
  /// a transitionComplete event is signaled.
  ///
  /// The speed field specifies the rate at which the viewer travels through
  /// a scene in metres per second. Since browsers may provide mechanisms to
  /// travel faster or slower, this field specifies the default, average speed
  /// of the viewer when the NavigationInfo node is bound. If the
  /// NavigationInfo type is EXAMINE, speed shall not affect the viewer's
  /// rotational speed. Scaling in the transformation hierarchy of the
  /// currently bound Viewpoint node (see above) scales the speed; parent
  /// translation and rotation transformations have no effect on speed.
  /// Speed shall be non-negative. Zero speed indicates that the avatar's
  /// position is stationary, but its orientation and field of view may still
  /// change. If the navigation type is "NONE", the speed field has no effect.
  ///
  /// The avatarSize field specifies the user's physical dimensions in the
  /// world for the purpose of collision detection and terrain following.
  /// It is a multi-value field allowing several dimensions to be specified.
  /// The first value shall be the allowable distance between the user's
  /// position and any collision geometry (as specified by a Collision node )
  /// before a collision is detected. The second shall be the height above the
  /// terrain at which the browser shall maintain the viewer. The third shall
  /// be the height of the tallest object over which the viewer can move. This
  /// allows staircases to be built with dimensions that can be ascended by
  /// viewers in all browsers. The transformation hierarchy of the currently
  /// bound Viewpoint node scales the avatarSize. Translations and rotations
  /// have no effect on avatarSize.
  ///
  /// In H3DAPI only the first entry of avatarSize is considered.
  ///
  /// For purposes of terrain following, the browser maintains a notion of the
  /// down direction (down vector), since gravity is applied in the direction
  /// of the down vector. This down vector shall be along the negative Y-axis
  /// in the local coordinate system of the currently bound Viewpoint node
  /// (i.e., the accumulation of the Viewpoint node's ancestors'
  /// transformations, not including the Viewpoint node's orientation field).
  ///
  /// Geometry beyond the visibilityLimit may not be rendered. A value of 0.0
  /// indicates an infinite visibility limit. The visibilityLimit field is
  /// restricted to be greater than or equal to zero.
  ///
  /// The speed, avatarSize and visibilityLimit values are all scaled by the
  /// transformation being applied to the currently bound Viewpoint node.
  /// If there is no currently bound Viewpoint node, the values are interpreted
  /// in the world coordinate system. This allows these values to be
  /// automatically adjusted when binding to a Viewpoint node that has a
  /// scaling transformation applied to it without requiring a new
  /// NavigationInfo node to be bound as well. The results are undefined if
  /// the scale applied to the Viewpoint node is non-uniform.
  ///
  /// The headlight field specifies whether a browser shall turn on a
  /// headlight. A headlight is a directional light that always points in the
  /// direction the user is looking. Setting this field to TRUE allows the
  /// browser to provide a headlight, possibly with user interface controls
  /// to turn it on and off. Scenes that enlist precomputed lighting
  /// (EXAMPLE  radiosity solutions) can turn the headlight off. The headlight
  /// shall have intensity = 1, color = (1 1 1), ambientIntensity = 0.0, and
  /// direction = (0 0 ?1).
  ///
  /// It is recommended that the near clipping plane be set to one-half of the
  /// collision radius as specified in the avatarSize field (setting the near
  /// plane to this value prevents excessive clipping of objects just above the
  /// collision volume, and also provides a region inside the collision volume
  /// for content authors to include geometry intended to remain fixed relative
  /// to the viewer). Such geometry shall not be occluded by geometry outside
  /// of the collision volume.
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/NavigationInfo.x3d">NavigationInfo.x3d</a>
  ///     ( <a href="examples/NavigationInfo.x3d.html">Source</a> )
  class H3DAPI_API NavigationInfo : public X3DBindableNode {
  public:

    /// Constructor.
    NavigationInfo( Inst< SFSetBind > _set_bind         = 0,
                    Inst< SFNode    > _metadata         = 0,
                    Inst< SFTime    > _bindTime         = 0,
                    Inst< SFBool    > _isBound          = 0,
                    Inst< MFFloat   > _avatarSize       = 0,
                    Inst< SFBool    > _headlight        = 0,
                    Inst< SFFloat   > _speed            = 0,
                    Inst< SFTime    > _transitionTime   = 0,
                    Inst< MFString  > _transitionType   = 0,
                    Inst< MFString  > _type             = 0,
                    Inst< SFFloat   > _visibilityLimit  = 0,
                    Inst< SFBool    > _transitionComplete = 0,
                    Inst< SFBool    > _headlightShadows = 0,
                    Inst< SFFloat   > _nearVisibilityLimit  = 0 );

    /// Convenience function to get the top of the NavigationInfo stack.
    static inline NavigationInfo *getActive() {
      return static_cast< NavigationInfo * >( X3DBindableNode::getActive( "NavigationInfo" ) );
    }

    inline void setNavType( string type ) { nav_type = type; }

    string getUsedNavType();

    inline void setTransitionComplete( bool value ) {
      transitionComplete->setValue( value, id );
    }

    /// The avatarSize field specifies the user's physical dimensions 
    /// in the world for the purpose of collision detection and terrain
    /// following. Only the first number in avatarSize is supported by H3DAPI.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> [0.25 1.6 0.75] \n
    auto_ptr< MFFloat > avatarSize;

    /// The headlight field specifies whether a headlight should be turned
    /// on. A headlight is a directional light that always points in the
    /// direction the user is looking.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> TRUE \n
    auto_ptr< SFBool > headlight;

    /// The speed field specifies the rate at which the viewer travels
    /// through a scene in metres per second. 
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 1.0 \n
    auto_ptr< SFFloat > speed;

    /// The transitionTime field specifies the duration of any viewpoint
    /// transition.
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 1.0 \n
    /// <b>Valid Range:</b> [0, Inf)
    auto_ptr< SFTime > transitionTime;

    /// The transitionType field specifies an ordered list of paradigms
    /// that determine the manner in which the browser moves the viewer
    /// when a new Viewpoint node is bound. Not supported by H3D API.
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> ["LINEAR"]: \n
    /// <b>Valid values:</b> "TELEPORT", "LINEAR", "ANIMATE"
    auto_ptr< MFString > transitionType;

    /// The type field specifies an ordered list of navigation paradigms 
    /// that specify a combination of navigation types and the initial
    /// navigation type.
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> ["EXAMINE", "ANY"]\n
    auto_ptr< MFString > type;
    
    /// Geometry beyond the visibilityLimit may not be rendered. A value
    /// of 0.0 indicates an infinite visibility limit. The visibilityLimit
    /// field is restricted to be greater than or equal to zero. This
    /// basically specifies the far clipping plane.
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 0.0 \n
    /// <b>Valid values:</b> [0.0,inf) \n
    auto_ptr< SFFloat > visibilityLimit;

    /// Geometry closer than the nearVisibilityLimit may not be rendered. 
    /// This basically specifies the near clipping plane.
    /// A value of 0.0 indicates that the near clipping plane will be 
    /// automatically calculated from the geometry in the scene.
    /// nearVisibilityLimit must always be less than visibilityLimit
    /// (with exception from the special case of 0.0).
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 0 \n
    /// <b>Default value:</b> 0, [0.0,inf) \n
    auto_ptr< SFFloat > nearVisibilityLimit;

    /// When a transition completes, a transitionComplete event is
    /// signaled.
    /// 
    /// <b>Access type:</b> outputOnly \n
    auto_ptr< SFBool > transitionComplete;

    /// Determines if the headlight should be used in shadow calculations
    /// or not. If FALSE no shadows will be cast using the head light.
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> FALSE \n
    auto_ptr< SFBool > headlightShadows;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;

  protected:
    string nav_type;
  };
}

#endif
