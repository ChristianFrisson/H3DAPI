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
/// \file Viewpoint.h
/// \brief Header file for Viewpoint, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __VIEWPOINT_H__
#define __VIEWPOINT_H__

#include "X3DBindableNode.h"
#include "FieldTemplates.h"
#include "Exception.h"

namespace H3D {
  /// \ingroup Nodes
  /// \class Viewpoint
  /// \brief The Viewpoint node defines a specific location in the local 
  /// coordinate system from which the user may view the scene. 
  ///
  /// Viewpoint nodes are bindable children nodes (see X3DBindableNode)
  /// and thus there exists a Viewpoint node stack in which the top-most 
  /// Viewpoint node on the stack is the currently active Viewpoint node.
  /// If a TRUE value is sent to the set_bind field of a Viewpoint node,
  /// it is moved to the top of the Viewpoint node stack and activated. 
  /// When a Viewpoint node is at the top of the stack, the user's view
  /// is conceptually re-parented as a child of the Viewpoint node. All 
  /// subsequent changes to the Viewpoint node's coordinate system change
  /// the user's view (e.g., changes to any ancestor transformation nodes
  /// or to the Viewpoint node's position or orientation fields). Sending
  /// a set_bind FALSE event removes the Viewpoint node from the stack and
  /// produces isBound FALSE and bindTime events. If the popped Viewpoint
  /// node is at the top of the viewpoint stack, the user's view is 
  /// re-parented to the next entry in the stack. 
  /// When a Viewpoint node is moved to the top of the stack, the existing
  /// top of stack Viewpoint node sends an isBound FALSE event and is pushed
  /// down the stack.
  ///
  /// An author can automatically move the user's view through the world by
  /// binding the user to a Viewpoint node and then animating either the
  /// Viewpoint node or the transformations above it. 
  /// The bindTime field sends the time at which the Viewpoint node is bound
  /// or unbound. This can happen:
  ///
  /// - during loading.
  /// - when a set_bind event is sent to the Viewpoint node;
  /// - when the browser binds to the Viewpoint node through its user
  ///   interface described below.
  ///
  /// The position and orientation fields of the Viewpoint node specify
  /// relative locations in the local coordinate system. Position is
  /// relative to the coordinate system's origin (0,0,0), while orientation
  /// specifies a rotation relative to the default orientation. In the
  /// default position and orientation, the viewer is on the Z-axis looking
  /// down the Z-axis toward the origin with +X to the right and +Y straight
  /// up. Viewpoint nodes are affected by the transformation hierarchy.
  ///
  /// Navigation types (see NavigationInfo) that require a definition
  /// of a down vector (e.g., terrain following) shall use the negative
  /// Y-axis of the coordinate system of the currently bound Viewpoint
  /// node. Likewise, navigation types that require a definition of an up
  /// vector shall use the positive Y-axis of the coordinate system of the
  /// currently bound Viewpoint node. The orientation field of the Viewpoint
  /// node does not affect the definition of the down or up vectors. This
  /// allows the author to separate the viewing direction from the gravity
  /// direction.
  ///
  /// The jump field specifies whether the user's view "jumps" to the
  /// position and orientation of a bound Viewpoint node or remains
  /// unchanged. This jump is instantaneous and discontinuous in that no
  /// collisions are performed and no ProximitySensor nodes are checked in
  /// between the starting and ending jump points. If the user's position
  /// before the jump is inside a ProximitySensor the exitTime of that
  /// sensor shall send the same timestamp as the bind field. Similarly, if
  /// the user's position after the jump is inside a ProximitySensor the
  /// enterTime of that sensor shall send the same timestamp as the bind
  /// field. Regardless of the value of jump at bind time, the relative
  /// viewing transformation between the user's view and the current
  /// Viewpoint node shall be stored with the current Viewpoint node for
  /// later use when un-jumping (i.e., popping the Viewpoint node binding
  /// stack from a Viewpoint node with jump TRUE). The following summarizes
  /// the bind stack rules (X3DBindableNode) with
  /// additional rules regarding Viewpoint nodes:
  /// <ul>
  /// <li>
  /// During read, the first encountered Viewpoint node is bound by pushing
  /// it to the top of the Viewpoint node stack. If a Viewpoint node name is
  /// specified in the URL that is being read, this named Viewpoint node is
  /// considered to be the first encountered Viewpoint node. Nodes contained
  /// within Inline nodes (see 9.4.2 Inline), within the strings passed to
  /// the Browser.createX3DFromString() method, or within files passed to
  /// the Browser.createX3DFromURL() method (see 2.[I19775-2]) are not
  /// candidates for the first encountered Viewpoint node. The first node
  /// within a prototype instance is a valid candidate for the first
  /// encountered Viewpoint node. The first encountered Viewpoint node sends
  /// an isBound TRUE event.
  /// <li>
  /// When a set_bind TRUE event is received by a Viewpoint node,
  /// <ol><li>
  /// If it is not on the top of the stack: The relative transformation from
  /// the current top of stack Viewpoint node to the user's view is stored
  /// with the current top of stack Viewpoint node. The current top of stack
  /// node sends an isBound FALSE event. The new node is moved to the top of
  /// the stack and becomes the currently bound Viewpoint node. The new
  /// Viewpoint node (top of stack) sends an isBound TRUE event. If jump is
  /// TRUE for the new Viewpoint node, the user's view is instantaneously
  /// "jumped" to match the values in the position and orientation fields of
  /// the new Viewpoint node.  
  /// <li> If the node is already at the top of the stack, this event has
  /// no affect.
  /// </ol>
  /// <li>
  /// When a set_bind FALSE event is received by a Viewpoint node in the
  /// stack, it is removed from the stack. If it was on the top of the
  /// stack,
  /// - it sends an isBound FALSE event.
  /// - The next node in the stack becomes the currently bound Viewpoint
  ///   node and issues an isBound TRUE event.
  /// - if its jump field value is TRUE, the user's view is instantaneously
  ///   "jumped" to the position and orientation of the next Viewpoint node in
  ///   the stack with the stored relative transformation of this next
  ///   Viewpoint node applied.
  /// <li>
  /// If a set_bind FALSE event is received by a node not in the stack, the
  /// event is ignored and isBound events are not sent.
  /// <li>
  /// When a node replaces another node at the top of the stack, the isBound
  /// TRUE and FALSE events from the two nodes are sent simultaneously
  /// (i.e., with identical timestamps).
  /// <li>
  /// If a bound node is deleted, it behaves as if it received a set_bind
  /// FALSE event (see c. above).
  /// </ul>
  /// 
  /// The jump field may change after a Viewpoint node is bound. The rules
  /// described above still apply. If jump was TRUE when the Viewpoint node
  /// is bound, but changed to FALSE before the set_bind FALSE is sent, the
  /// Viewpoint node does not un-jump during unbind. If jump was FALSE when
  /// the Viewpoint node is bound, but changed to TRUE before the set_bind
  /// FALSE is sent, the Viewpoint node does perform the un-jump during
  /// unbind.
  ///
  /// Note that there are two other mechanisms that result in the binding of
  /// a new Viewpoint:
  ///
  /// - An Anchor node's url field specifies a "#ViewpointName".
  /// - A script invokes the loadURL() method and the URL argument specifies a
  ///   "#ViewpointName".
  ///
  /// Both of these mechanisms override the jump field value of the
  /// specified Viewpoint node (#ViewpointName) and assume that jump is TRUE
  /// when binding to the new Viewpoint. The behaviour of the viewer
  /// transition to the newly bound Viewpoint depends on the currently bound
  /// NavigationInfo node's type field value.
  ///
  /// \anchor Viewpoint_fieldOfView
  /// The fieldOfView field specifies a preferred minimum viewing angle from
  /// this viewpoint in radians. A small field of view roughly corresponds
  /// to a telephoto lens; a large field of view roughly corresponds to a
  /// wide-angle lens. The field of view shall be greater than zero and
  /// smaller than ?. The value of fieldOfView represents the minimum
  /// viewing angle in any direction axis perpendicular to the view. For
  /// example, a browser with a rectangular viewing projection shall have
  /// the following relationship:
  ///
  ///   display width    tan(FOVhorizontal/2) \n
  ///   -------------- = -----------------    \n
  ///   display height   tan(FOVvertical/2)   \n
  ///
  /// where the smaller of display width or display height determines which
  /// angle equals the fieldOfView (the larger angle is computed using the
  /// relationship described above). The larger angle shall not exceed ? and
  /// may force the smaller angle to be less than fieldOfView in order to
  /// sustain the aspect ratio.
  ///
  /// The description field specifies a textual description of the Viewpoint
  /// node. This may be used by browser-specific user interfaces. If a
  /// Viewpoint's description field is empty it is recommended that the
  /// browser not present this Viewpoint in its browser-specific user
  /// interface.
  ///
  /// The centerOfRotation field specifies a center about which to rotate
  /// the user's eyepoint when in EXAMINE mode. If the browser does not
  /// provide the ability to spin around the object in EXAMINE mode, or
  /// LOOKAT is not in the list of allowed navigation modes, this field
  /// shall be ignored. 
  /// The URL syntax ".../scene.wrl#ViewpointName" specifies the user's 
  /// initial view when loading "scene.wrl" to be the first Viewpoint node
  /// in the X3D file that appears as DEF ViewpointName Viewpoint {...}. 
  /// This overrides the first Viewpoint node in the X3D file as the initial
  /// user view, and a set_bind TRUE message is sent to the Viewpoint node
  /// named "ViewpointName". If the Viewpoint node named "ViewpointName" is 
  /// not found, the browser shall use the first Viewpoint node in the X3D
  /// file (i.e. the normal default behaviour). The URL syntax 
  /// "#ViewpointName" (i.e. no file name) specifies a viewpoint within the
  /// existing X3D file. If this URL is loaded (e.g. Anchor node's url field
  /// or loadURL() method is invoked by a Script node), the Viewpoint node 
  /// named "ViewpointName" is bound (a set_bind TRUE event is sent to this
  /// Viewpoint node).
  class H3DAPI_API Viewpoint : public X3DBindableNode {
  public:
    /// Constructor.
    Viewpoint( 
              Inst< SFSetBind >  _set_bind         = 0,
              Inst< SFVec3f   >  _centerOfRotation = 0,
              Inst< SFString  >  _description      = 0,
              Inst< SFFloat   >  _fieldOfView      = 0,
              Inst< SFBool    >  _jump             = 0,
              Inst< SFNode    >  _metadata         = 0,
              Inst< SFRotation>  _orientation      = 0,
              Inst< SFVec3f   >  _position         = 0,
              Inst< SFTime    >  _bindTime         = 0,
              Inst< SFBool    >  _isBound          = 0,
              Inst< SFMatrix4f > _accForwardMatrix = 0,
              Inst< SFMatrix4f > _accInverseMatrix = 0 );

    /// Traverse the scenegraph. Saves the accumulated inverse
    /// matrix for later use when transforming the Viewpoint in
    /// GLWindow.
    virtual void traverseSG( TraverseInfo &ti ) {
      accInverseMatrix->setValue( ti.getAccInverseMatrix(), id );
      accForwardMatrix->setValue( ti.getAccForwardMatrix(), id );
    }

    /// Convenience function to get the top of the Viewpoint stack.
    static inline Viewpoint *getActive() {
      return static_cast< Viewpoint * >( X3DBindableNode::getActive( "Viewpoint" ) );
    }

    /// The centerOfRotation field specifies a center about which to
    /// rotate the user's eyepoint when in EXAMINE mode.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> Vec3f( 0, 0, 0 ) \n
    auto_ptr< SFVec3f    > centerOfRotation;

    /// The description field specifies a textual description of the
    /// Viewpoint node.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> "" \n
    auto_ptr< SFString   > description;

    /// The fieldOfView field specifies a preferred minimum viewing angle
    /// from this viewpoint in radians. 
    /// See /ref Viewpoint_fieldOfView "Detailed description" for more info.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> pi/4 \n
    /// <b>Value range:</b> (0,pi) \n
    auto_ptr< SFFloat    > fieldOfView;

    /// The jump field specifies whether the user's view "jumps" to the 
    /// position and orientation of a bound Viewpoint node or remains 
    /// unchanged.
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> TRUE \n
    auto_ptr< SFBool     > jump;

    /// Specifies the orientation of the Viewpoint relative to the default 
    /// orientation. Per default the viewer is on the Z-axis looking down
    /// the -Z-axis toward the origin with +X to the right and +Y straight
    /// up.
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> Rotation( 0, 0, 1, 0 ) \n
    auto_ptr< SFRotation > orientation;
    
    /// Specifies the position of the Viewpoint the local coordinate system.
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> Vec3f( 0, 0, 10 ) \n
    auto_ptr< SFVec3f    > position;

    /// The matrix from local space to global space.
    ///
    /// <b>Access type:</b> outputOnly \n
    auto_ptr< SFMatrix4f > accForwardMatrix;

    /// The matrix from global space to the local space the Viewpoint
    /// resides in.
    /// 
    /// <b>Access type:</b> outputOnly \n
    auto_ptr< SFMatrix4f > accInverseMatrix;
    
    static H3DNodeDatabase database;
  };
}

#endif
