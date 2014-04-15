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
/// \file ViewpointGroup.h
/// \brief Header file for ViewpointGroup, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __VIEWPOINTGROUP_H__
#define __VIEWPOINTGROUP_H__

#include <H3D/X3DViewpointNode.h>
#include <H3D/MFNode.h>

namespace H3D {
  /// \ingroup X3DNodes
  /// \class ViewpointGroup
  /// \brief ViewpointGroup is used to control display of viewpoints on the
  /// viewpoint list. Use of ViewpointGroup allows a viewpoint list to have
  /// a tree structure, similar to a bookmark list. 
  ///
  /// The children field is a sequence of nodes of type X3DViewpointNode.
  ///
  /// The description field provides a simple description or navigation hint
  /// to be displayed for this ViewpointGroup.
  ///
  /// The displayed field determines whether this ViewpointGroup is displayed
  /// in the current viewpoint list. 
  ///
  /// The center and size fields are defined identically as the corresponding
  /// ProximitySensor definitions. The center field provides a position offset
  /// from origin of local coordinate system. The size field provides the size
  /// of a proximity box within which the ViewpointGroup is usable and
  /// displayed on the viewpoint list. A size field of 0 0 0 specifies that the
  /// ViewpointGroup is always usable and displayed.
  ///
  /// The retainUserOffsets field specifies whether the user is returned to the
  /// originally defined viewpoint position/orientation after local navigation.
  ///
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/ViewpointGroup.x3d">ViewpointGroup.x3d</a>
  ///     ( <a href="examples/ViewpointGroup.x3d.html">Source</a> )
  class H3DAPI_API ViewpointGroup : public X3DChildNode {
  public:

    /// Type class of children field
    class H3DAPI_API MFChild: public MFNode {
      virtual void onAdd( Node * n );
    };

    /// Constructor.
    ViewpointGroup( Inst< SFVec3f  >  _center           = 0,
                    Inst< MFChild  >  _children         = 0,
                    Inst< SFString >  _description      = 0,
                    Inst< SFBool   >  _displayed        = 0,
                    Inst< SFNode   >  _metadata         = 0,
                    Inst< SFBool   >  _retainUserOffsets = 0,
                    Inst< SFVec3f  >  _size             = 0 );

    virtual ~ViewpointGroup() {
      viewpoint_groups.remove( this );
      viewpoint_groups_changed = true;
    }

    virtual bool windowFromfieldOfView( H3DFloat width, H3DFloat height,
                                        H3DFloat clip_near,
                                        H3DFloat &top, H3DFloat &bottom,
                                        H3DFloat &right, H3DFloat &left ) {
      return false;
    }

    /// Traverse the scenegraph. Check changes of viewpoint position
    /// since last scene graph traversal.
    /// \param ti The TraverseInfo object containing information about the
    /// traversal.
    virtual void traverseSG( TraverseInfo &ti );

    /// Returns true if the given viewpoint is in the children field of this
    /// ViewpointGroup.
    bool containsViewpoint( X3DViewpointNode *vp ) const;
    
    list< Node * > getChildrenAsList();

    /// Returns true if this node exists outside of another ViewpointGroup
    inline bool isTopLevel() {
      return is_top_level;
    }

    /// Returns true if this node exists in the scene graph
    inline bool inSceneGraph() {
      return in_scene_graph;
    }

    typedef list< ViewpointGroup * > ViewpointGroupList;

    /// Returns a list of all current ViewpointGroup instances.
    static const ViewpointGroupList &getAllViewpointGroups() {
      return viewpoint_groups;
    }

    /// True on any addition/deletion/in_scene_graph status change
    /// of viewpoint group, reset once inquired
    static bool viewpointGroupsChanged() {
      bool status = viewpoint_groups_changed;
      viewpoint_groups_changed = false;
      return status;
    }

    /// The center field provides a position offset from origin of local
    /// coordinate system.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 0 0 0 \n
    auto_ptr< SFVec3f > center;

    /// The children field is a sequence of nodes of type X3DViewpointNode.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> NULL \n
    auto_ptr< MFChild > children;

    /// The description field provides a simple description or navigation 
    /// hint to be displayed for this ViewpointGroup. 
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> "" \n
    auto_ptr< SFString > description;

    /// The displayed field determines whether this ViewpointGroup is displayed
    /// in the current viewpoint list. 
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> TRUE \n
    auto_ptr< SFBool > displayed;

    /// The retainUserOffsets field specifies whether the user is returned to the
    /// originally defined viewpoint position/orientation after local navigation.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> FALSE \n
    auto_ptr< SFBool > retainUserOffsets;

    /// The size field provides the size of a proximity box within which the
    /// ViewpointGroup is usable and displayed on the viewpoint list. A size
    /// field of 0 0 0 specifies that the ViewpointGroup is always usable and
    /// displayed.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> TRUE \n
    auto_ptr< SFVec3f > size;

    /// Flag to show whether this ViewpointGroup should be displayed in 
    /// browser list of viewpoints.
    bool display_in_list;
    
    static H3DNodeDatabase database;

  protected:
    //static NodeVector groups;
    static ViewpointGroupList viewpoint_groups;

    //on any change of is_top_level or in_scene_graph of any node in viewpoint_groups
    static bool viewpoint_groups_changed;

    Vec3f last_position;

    /// True if this ViewpointGroup exists outside of a another ViewpointGroup
    bool is_top_level;

    /// True if this ViewpointGroup exists in scene graph
    bool in_scene_graph;
  };
}

#endif
