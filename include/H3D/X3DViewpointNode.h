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
/// \file X3DViewpointNode.h
/// \brief Header file for X3DViewpointNode, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __X3DX3DViewpointNodeNODE_H__
#define __X3DX3DViewpointNodeNODE_H__

#include <H3D/X3DBindableNode.h>
#include <H3D/FieldTemplates.h>
#include <H3DUtil/Exception.h>
#include <H3D/SFVec3f.h>
#include <H3D/SFString.h>
#include <H3D/SFFloat.h>
#include <H3D/SFRotation.h>
#include <H3D/SFMatrix4f.h>
#include <H3D/StereoInfo.h>

namespace H3D {
  /// \ingroup X3DNodes
  /// \class X3DViewpointNode
  /// \brief The X3DViewpointNode node defines a specific location in the local 
  /// coordinate system from which the user may view the scene. 
  ///
  /// X3DViewpointNode nodes are bindable children nodes (see X3DBindableNode)
  /// and thus there exists a X3DViewpointNode node stack in which the top-most 
  /// X3DViewpointNode node on the stack is the currently active
  /// X3DViewpointNode node. If a TRUE value is sent to the set_bind field of a
  /// X3DViewpointNode node, it is moved to the top of the X3DViewpointNode
  /// node stack and activated. When a X3DViewpointNode node is at the top of
  /// the stack, the user's view is conceptually re-parented as a child of the
  /// X3DViewpointNode node. All subsequent changes to the X3DViewpointNode
  /// node's coordinate system change the user's view (e.g., changes to any
  /// ancestor transformation nodes or to the X3DViewpointNode node's position
  /// or orientation fields). Sending a set_bind FALSE event removes the
  /// X3DViewpointNode node from the stack and produces isBound FALSE and
  /// bindTime events. If the popped X3DViewpointNode node is at the top of the
  /// X3DViewpointNode stack, the user's view is re-parented to the next entry
  /// in the stack. When a X3DViewpointNode node is moved to the top of the
  /// stack, the existing top of stack X3DViewpointNode node sends an isBound
  /// FALSE event and is pushed down the stack.
  ///
  /// An author can automatically move the user's view through the world by
  /// binding the user to a X3DViewpointNode node and then animating either the
  /// X3DViewpointNode node or the transformations above it. 
  /// The bindTime field sends the time at which the X3DViewpointNode node is
  /// bound or unbound. This can happen:
  ///
  /// - during loading.
  /// - when a set_bind event is sent to the X3DViewpointNode node;
  /// - when the browser binds to the X3DViewpointNode node through its user
  ///   interface described below.
  ///
  /// The position and orientation fields of the X3DViewpointNode node specify
  /// relative locations in the local coordinate system. Position is
  /// relative to the coordinate system's origin (0,0,0), while orientation
  /// specifies a rotation relative to the default orientation. In the
  /// default position and orientation, the viewer is on the Z-axis looking
  /// down the Z-axis toward the origin with +X to the right and +Y straight
  /// up. X3DViewpointNode nodes are affected by the transformation hierarchy.
  ///
  /// Navigation types (see NavigationInfo) that require a definition
  /// of a down vector (e.g., terrain following) shall use the negative
  /// Y-axis of the coordinate system of the currently bound X3DViewpointNode
  /// node. Likewise, navigation types that require a definition of an up
  /// vector shall use the positive Y-axis of the coordinate system of the
  /// currently bound X3DViewpointNode node. The orientation field of the
  /// X3DViewpointNode node does not affect the definition of the down or up
  /// vectors. This allows the author to separate the viewing direction from
  /// the gravity direction.
  ///
  /// The jump field specifies whether the user's view "jumps" to the
  /// position and orientation of a bound X3DViewpointNode node or remains
  /// unchanged. This jump is instantaneous and discontinuous in that no
  /// collisions are performed and no ProximitySensor nodes are checked in
  /// between the starting and ending jump points. If the user's position
  /// before the jump is inside a ProximitySensor the exitTime of that
  /// sensor shall send the same timestamp as the bind field. Similarly, if
  /// the user's position after the jump is inside a ProximitySensor the
  /// enterTime of that sensor shall send the same timestamp as the bind
  /// field. Regardless of the value of jump at bind time, the relative
  /// viewing transformation between the user's view and the current
  /// X3DViewpointNode node shall be stored with the current X3DViewpointNode
  /// node for later use when un-jumping (i.e., popping the X3DViewpointNode
  /// node binding stack from a X3DViewpointNode node with jump TRUE). The
  /// following summarizes the bind stack rules (X3DBindableNode) with
  /// additional rules regarding X3DViewpointNode nodes:
  /// <ul>
  /// <li>
  /// During read, the first encountered X3DViewpointNode node is bound by
  /// pushing it to the top of the X3DViewpointNode node stack. If a
  /// X3DViewpointNode node name is specified in the URL that is being read,
  /// this named X3DViewpointNode node is considered to be the first
  /// encountered X3DViewpointNode node. Nodes contained within Inline nodes
  /// (see 9.4.2 Inline), within the strings passed to the
  /// Browser.createX3DFromString() method, or within files passed to
  /// the Browser.createX3DFromURL() method (see 2.[I19775-2]) are not
  /// candidates for the first encountered X3DViewpointNode node. The first
  /// node within a prototype instance is a valid candidate for the first
  /// encountered X3DViewpointNode node. The first encountered X3DViewpointNode
  /// node sends an isBound TRUE event.
  /// <li>
  /// When a set_bind TRUE event is received by a X3DViewpointNode node,
  /// <ol><li>
  /// If it is not on the top of the stack: The relative transformation from
  /// the current top of stack X3DViewpointNode node to the user's view is
  /// stored with the current top of stack X3DViewpointNode node. The current
  /// top of stack node sends an isBound FALSE event. The new node is moved to
  /// the top of the stack and becomes the currently bound X3DViewpointNode
  /// node. The new X3DViewpointNode node (top of stack) sends an isBound TRUE
  /// event. If jump is TRUE for the new X3DViewpointNode node, the user's view
  /// is instantaneously "jumped" to match the values in the position and
  /// orientation fields of the new X3DViewpointNode node.
  /// <li> If the node is already at the top of the stack, this event has
  /// no effect.
  /// </ol>
  /// <li>
  /// When a set_bind FALSE event is received by a X3DViewpointNode node in the
  /// stack, it is removed from the stack. If it was on the top of the
  /// stack,
  /// - it sends an isBound FALSE event.
  /// - The next node in the stack becomes the currently bound X3DViewpointNode
  ///   node and issues an isBound TRUE event.
  /// - if its jump field value is TRUE, the user's view is instantaneously
  ///   "jumped" to the position and orientation of the next X3DViewpointNode
  ///   node in the stack with the stored relative transformation of this next
  ///   X3DViewpointNode node applied.
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
  /// The jump field may change after a X3DViewpointNode node is bound. The
  /// rules described above still apply. If jump was TRUE when the
  /// X3DViewpointNode node is bound, but changed to FALSE before the set_bind
  /// FALSE is sent, the X3DViewpointNode node does not un-jump during unbind.
  /// If jump was FALSE when the X3DViewpointNode node is bound, but changed to
  /// TRUE before the set_bind FALSE is sent, the X3DViewpointNode node does
  /// perform the un-jump during unbind.
  ///
  /// Note that there are two other mechanisms that result in the binding of
  /// a new X3DViewpointNode:
  ///
  /// - An Anchor node's url field specifies a "#X3DViewpointNodeName".
  /// - A script invokes the loadURL() method and the URL argument specifies a
  ///   "#X3DViewpointNodeName".
  ///
  /// Both of these mechanisms override the jump field value of the
  /// specified X3DViewpointNode node (#X3DViewpointNodeName) and assume that
  /// jump is TRUE when binding to the new X3DViewpointNode. The behaviour of
  /// the viewer transition to the newly bound X3DViewpointNode depends on the
  /// currently bound NavigationInfo node's type field value.
  ///
  /// \anchor X3DViewpointNode_fieldOfView
  /// The fieldOfView field specifies a preferred minimum viewing angle from
  /// this X3DViewpointNode in radians. A small field of view roughly
  /// corresponds to a telephoto lens; a large field of view roughly
  /// corresponds to a wide-angle lens. The field of view shall be greater than
  /// zero and smaller than ?. The value of fieldOfView represents the minimum
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
  /// The description field specifies a textual description of the
  /// X3DViewpointNode node. This may be used by browser-specific user
  /// interfaces. If a X3DViewpointNode's description field is empty it is
  /// recommended that the browser not present this X3DViewpointNode in its
  /// browser-specific user interface.
  ///
  /// The centerOfRotation field specifies a center about which to rotate
  /// the user's eyepoint when in EXAMINE mode. If the browser does not
  /// provide the ability to spin around the object in EXAMINE mode, or
  /// LOOKAT is not in the list of allowed navigation modes, this field
  /// shall be ignored.
  ///
  /// The URL syntax ".../scene.wrl#X3DViewpointNodeName" specifies the user's 
  /// initial view when loading "scene.wrl" to be the first X3DViewpointNode
  /// node in the X3D file that appears as DEF X3DViewpointNodeName
  /// X3DViewpointNode {...}. This overrides the first X3DViewpointNode node
  /// in the X3D file as the initial user view, and a set_bind TRUE message is
  /// sent to the X3DViewpointNode node named "X3DViewpointNodeName". If the
  /// X3DViewpointNode node named "X3DViewpointNodeName" is not found, the
  /// browser shall use the first X3DViewpointNode node in the X3D file (i.e.
  /// the normal default behaviour). The URL syntax "#X3DViewpointNodeName"
  /// (i.e. no file name) specifies a X3DViewpointNode within the existing X3D
  /// file. If this URL is loaded (e.g. Anchor node's url field or loadURL()
  /// method is invoked by a Script node), the X3DViewpointNode node named
  /// "X3DViewpointNodeName" is bound (a set_bind TRUE event is sent to this
  /// X3DViewpointNode node).
  ///
  /// The retainUserOffsets field indicates whether a viewpoint needs to retain
  /// (TRUE) or reset to zero (FALSE) any prior user navigation offsets from
  /// defined viewpoint position, orientation. When an node of type
  /// X3DViewpointNode is bound, user navigation offsets are reinitialized if
  /// the associated retainUserOffsets is TRUE.
  ///
  /// \par Internal routes:
  /// \dotfile X3DViewpointNode.dot
  class H3DAPI_API X3DViewpointNode : public X3DBindableNode {
  public:

    /// Used with setupProjection function to indicate for which eye the
    /// frustum should be calculated.
    enum EyeMode {
      /// Not a stereo mode, only one frustum is needed.
      MONO = 0,
      /// For the left eye.
      LEFT_EYE = 1,
      /// For the right eye.
      RIGHT_EYE = 2
    };

    /// This field is used for the position field in order to reset the
    /// internal variables rel_pos and rel_orn used for H3DAPIs internal
    /// navigation.
    class H3DAPI_API SFPosition:  public SFVec3f {
    public:
      virtual void setValue( const Vec3f &v, int id = 0 ) {
        SFVec3f::setValue( v, id );
        X3DViewpointNode * temp_owner =
          static_cast< X3DViewpointNode * >( getOwner() );
        temp_owner->relPos->setValue( Vec3f() );
        temp_owner->relOrn->setValue( Rotation() );
      }

    protected:
      virtual void update() {
        SFVec3f::update();
        X3DViewpointNode * temp_owner =
          static_cast< X3DViewpointNode * >( getOwner() );
        temp_owner->relPos->setValue( Vec3f() );
        temp_owner->relOrn->setValue( Rotation() );
      }
    };
#ifdef __BORLANDC__
    friend class SFPosition;
#endif

    /// This field is used for the orientation field in order to reset the
    /// internal variables rel_pos and rel_orn used for H3DAPIs internal
    /// navigation.
    class H3DAPI_API SFOrientation:  public SFRotation {
    public:
      virtual void setValue( const Rotation &v, int id = 0 ) {
        SFRotation::setValue( v, id );
        X3DViewpointNode * temp_owner =
          static_cast< X3DViewpointNode * >( getOwner() );
        temp_owner->relPos->setValue( Vec3f() );
        temp_owner->relOrn->setValue( Rotation() );
      }

    protected:
      virtual void update() {
        SFRotation::update();
        X3DViewpointNode * temp_owner =
          static_cast< X3DViewpointNode * >( getOwner() );
        temp_owner->relPos->setValue( Vec3f() );
        temp_owner->relOrn->setValue( Rotation() );
      }
    };
#ifdef __BORLANDC__
    friend class SFOrientation;
#endif

    /// This field sums the value of the two SFVec3f routed to it.
    class H3DAPI_API SFSumVec3f: 
      public TypedField< SFVec3f, Types< SFPosition, SFVec3f > > {
    protected:
      virtual void update() {
        SFVec3f::update();
        value = static_cast< SFPosition * >( routes_in[0] )->getValue() +
                static_cast< SFVec3f * >( routes_in[1] )->getValue();
      }
    };
#ifdef __BORLANDC__
    friend class SFSumVec3f;
#endif

    /// This field sums the value of the two SFRotation routed to it.
    class H3DAPI_API SFSumRotation: public TypedField< SFRotation,
      Types< SFOrientation, SFRotation > > {
    protected:
      virtual void update() {
        SFRotation::update();
        Rotation temp_rot =
          static_cast< SFOrientation * >( routes_in[0] )->getValue();
        temp_rot.axis.normalizeSafe();
        value = temp_rot *
                static_cast< SFRotation * >( routes_in[1] )->getValue();
      }
    };
#ifdef __BORLANDC__
    friend class SFSumRotation;
#endif

    /// Constructor.
    X3DViewpointNode( Inst< SFSetBind     > _set_bind          = 0,
                      Inst< SFVec3f       > _centerOfRotation  = 0,
                      Inst< SFString      > _description       = 0,
                      Inst< SFBool        > _jump              = 0,
                      Inst< SFNode        > _metadata          = 0,
                      Inst< SFOrientation > _orientation       = 0,
                      Inst< SFPosition    > _position          = 0,
                      Inst< SFBool        > _retainUserOffsets = 0,
                      Inst< SFTime        > _bindTime          = 0,
                      Inst< SFBool        > _isBound           = 0,
                      Inst< SFMatrix4f    > _accForwardMatrix  = 0,
                      Inst< SFMatrix4f    > _accInverseMatrix  = 0,
                      Inst< SFSumVec3f    > _totalPosition     = 0,
                      Inst< SFSumRotation > _totalOrientation  = 0 );

    /// Destructor
    virtual ~X3DViewpointNode() {
      removeFromStack();
      viewpoints.remove( this );
      viewpoints_changed = true;
    }

    /// Traverse the scenegraph. Saves the accumulated inverse
    /// matrix for later use when transforming the X3DViewpointNode in
    /// GLWindow.
    /// \param ti The TraverseInfo object containing information about the
    /// traversal.
    virtual void traverseSG( TraverseInfo &ti );

    /// Convenience function to get the top of the X3DViewpointNode stack.
    static inline X3DViewpointNode *getActive() {
      return static_cast< X3DViewpointNode * >( 
              X3DBindableNode::getActive( "X3DViewpointNode" ) );
    }

    /// Calculates the top, bottom, right and left clipping values for the view
    /// frustum at the near plane. Must be implemented by all subclasses.
    ///
    /// \param[in] width Width of window in number of pixels.
    /// \param[in] height Height of window in number of pixels.
    /// \param[in] clip_near Distance to near plane from viewpoint in meters.
    /// \param[out] top The top clipping value.
    /// \param[out] bottom The bottom clipping value.
    /// \param[out] right The right clipping value.
    /// \param[out] left The left clipping value.
    virtual bool windowFromfieldOfView( H3DFloat width, H3DFloat height,
                                        H3DFloat clip_near,
                                        H3DFloat &top, H3DFloat &bottom,
                                        H3DFloat &right, H3DFloat &left )
#ifndef H3D_GENERATE_DOTROUTE_FILES
                                        = 0;
#else
    { return false; }
#endif

    /// Move this instance to the stack top. 
    virtual void toStackTop();

    /// Remove the bindable node from the stack.
    virtual void removeFromStack();

    /// Function for rotating the viewpoint around rotation_center
    /// without changing the position or orientation fields.
    /// \param rotation The rotation to be applied.
    /// \param collision If true then collision between avatar and 
    /// nodes will be considered. NOTE: Not used in current implementation.
    /// \param rotation_center The center of rotation.
    virtual void rotateAround( Rotation rotation, bool collision,
                               Vec3f rotation_center );

    // Function for rotating the viewpoint around rotation_center = position
    /// without changing the position or orientation fields.
    virtual void rotateAroundSelf( Rotation rotation );

    /// Function for translating the viewpoint with direction
    /// without changing the position or orientation fields.
    /// \param direction The translation vector.
    /// \param collision If true then collision between avatar and 
    /// nodes will be considered.
    /// \param avatar_size Information about how the avatar is specified.
    /// \param topNode The topmost node in the scenegraph containing objects
    /// with which collision should be detected. Only used if the collision
    /// argument is true.
    // TODO: remove the collision parameter? topNode can be used.
    virtual void translate( Vec3f direction, bool collision,
                            const vector< H3DFloat > &avatar_size,
                            X3DChildNode * topNode );

    /// Function for moving the viewpoint to the new_pos
    /// without changing the position or orientation fields.
    virtual void moveTo( Vec3f new_pos );

    /// Function for detecting collision between avatar and objects
    virtual bool detectCollision( const vector< H3DFloat > &avatar_size,
                                  X3DChildNode * topNode );

    typedef list< X3DViewpointNode * > ViewpointList;
    
    /// Returns a list of all current X3DViewpointNode instances.
    static const ViewpointList &getAllViewpoints() {
      return viewpoints;
    }
    
    /// True on any addition/deletion/in_scene_graph status change
    /// of viewpoints, reset once inquired
    static bool viewpointsChanged() {
      bool status = viewpoints_changed;
      viewpoints_changed = false;
      return status;
    }

    /// Adds a projection matrix to the current OpenGL matrix based on the
    /// viewpoint values.
    /// \param[in] eye_mode The eye to render for (LEFT_EYE, RIGHT_EYE or MONO).
    /// \param[in] width Width of window in number of pixels.
    /// \param[in] height Height of window in number of pixels.
    /// \param[in] clip_near Distance to near plane from viewpoint in meters.
    /// \param[in] clip_far Distance to near plane from viewpoint in meters. -1
    /// means infinity.
    /// \param[in] stereo_info Stereo rendering parameters, used to specify 
    /// e.g. interocular distance.
    virtual void setupProjection( EyeMode eye_mode,
                                  H3DFloat width, H3DFloat height,
                                  H3DFloat clip_near, H3DFloat clip_far,
                                  StereoInfo * stereo_info = 0 )
#ifndef H3D_GENERATE_DOTROUTE_FILES
                                        = 0;
#else
    {}
#endif

    /// Adds a view matrix to the current OpenGL matrix to transform from
    /// view coordinates (camera/viewpoint space) to world space. This means
    /// e.g. setting up position and orientation of the viewpoint.
    /// \param[in] eye_mode The eye to render for (LEFT_EYE, RIGHT_EYE or MONO).
    /// \param[in] stereo_info Stereo rendering parameters, used to specify 
    /// interocular distance
    virtual void setupViewMatrix( EyeMode eye_mode,
                                  StereoInfo * stereo_info = 0 ); 


    /// Gets the viewMatrix for this viewpoint, i.e. the transformation
    /// matrix from world coordinates to view coordinates.
    virtual Matrix4f getViewMatrix( EyeMode eye_mode,
                                    StereoInfo * stereo_info = 0 );

    /// True if this viewpoint node exists outside a ViewpointGroup
    inline bool isTopLevel() {
      return is_top_level;
    }

    /// True if this viewpoint node exists in scene graph
    inline bool inSceneGraph() {
      return in_scene_graph;
    }

    /// The centerOfRotation field specifies a center about which to
    /// rotate the user's eyepoint when in EXAMINE mode.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> Vec3f( 0, 0, 0 ) \n
    /// 
    /// \dotfile X3DViewpointNode_centerOfRotation.dot
    auto_ptr< SFVec3f    > centerOfRotation;

    /// The description field specifies a textual description of the
    /// X3DViewpointNode node.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> "" \n
    /// 
    /// \dotfile X3DViewpointNode_description.dot
    auto_ptr< SFString   > description;

    /// The jump field specifies whether the user's view "jumps" to the 
    /// position and orientation of a bound X3DViewpointNode node or remains 
    /// unchanged.
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> TRUE \n
    /// 
    /// \dotfile X3DViewpointNode_jump.dot
    auto_ptr< SFBool     > jump;

    /// Specifies the orientation of the X3DViewpointNode relative to the
    /// default orientation. Per default the viewer is on the Z-axis looking
    /// down the -Z-axis toward the origin with +X to the right and +Y straight
    /// up.
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> Rotation( 0, 0, 1, 0 ) \n
    /// 
    /// \dotfile X3DViewpointNode_orientation.dot
    auto_ptr< SFOrientation > orientation;
    
    /// Specifies the position of the X3DViewpointNode the local coordinate
    /// system.
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> Vec3f( 0, 0, 10 ) \n
    /// 
    /// \dotfile X3DViewpointNode_position.dot
    auto_ptr< SFPosition > position;

    /// The retainUserOffsets field indicates whether a viewpoint needs to
    /// retain (TRUE) or reset to zero (FALSE) any prior user navigation
    /// offsets from defined viewpoint position, orientation.
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> false \n
    /// 
    /// \dotfile X3DViewpointNode_retainUserOffsets.dot
    auto_ptr< SFBool    > retainUserOffsets;

    /// The matrix from local space to global space.
    ///
    /// <b>Access type:</b> outputOnly \n
    /// 
    /// \dotfile X3DViewpointNode_accForwardMatrix.dot
    auto_ptr< SFMatrix4f > accForwardMatrix;

    /// The matrix from global space to the local space the X3DViewpointNode
    /// resides in.
    /// 
    /// <b>Access type:</b> outputOnly \n
    /// 
    /// \dotfile X3DViewpointNode_accInverseMatrix.dot
    auto_ptr< SFMatrix4f > accInverseMatrix;

    /// Returns the sum of the field position and the internal field
    /// relPos used for navigation.
    /// 
    /// <b>Access type:</b> outputOnly \n
    /// 
    /// \dotfile X3DViewpointNode_totalPosition.dot
    auto_ptr< SFSumVec3f > totalPosition;

    /// Returns the sum of the field orientation and the internal field
    /// relOrn used for navigation.
    /// 
    /// <b>Access type:</b> outputOnly \n
    /// 
    /// \dotfile X3DViewpointNode_totalOrientation.dot
    auto_ptr< SFSumRotation > totalOrientation;
    
    static H3DNodeDatabase database;

    /// Internal field that stores the relative offset from the position
    /// set by the user. Set while navigating. The actual position of the
    /// viewpoint can be obtained through the totalPosition field.
    auto_ptr< SFVec3f > relPos;

    /// Internal field that stores the relative offset from the orientation
    /// set by the user. Set while navigating. The actual orientation of the
    /// viewpoint can be obtained through the totalOrientation field.
    auto_ptr< SFRotation > relOrn;

  protected:
    static ViewpointList viewpoints;
    static bool viewpoints_changed;

    /// True if the viewpooint exists outside a ViewpointGroup
    bool is_top_level;

    /// True is the viewpoint exists in scene graph
    bool in_scene_graph;

    // Internal function for easier calculations of parameters sent to
    // glFrustum or glOrtho. For usage see function setupProjection in
    // Viewpoint or OrthoViewpoint.
    void getProjectionDimensions( EyeMode eye_mode,
                                  H3DFloat width,
                                  H3DFloat height,
                                  H3DFloat clip_near,
                                  H3DFloat &top,
                                  H3DFloat &bottom,
                                  H3DFloat &right,
                                  H3DFloat &left,
                                  StereoInfo * stereo_info = 0 );

  };
}

#endif
