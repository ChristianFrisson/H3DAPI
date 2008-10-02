//////////////////////////////////////////////////////////////////////////////
//    Copyright 2004-2007, SenseGraphics AB
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

  class H3DAPI_API ViewpointGroup : public X3DViewpointNode {
  public:
    typedef TypedMFNode< X3DViewpointNode > MFChild;

    /// Constructor.
    ViewpointGroup( Inst< SFSetBind >  _set_bind         = 0,
                    Inst< SFVec3f   >  _centerOfRotation = 0,
                    Inst< SFString  >  _description      = 0,
                    Inst< SFBool    >  _jump             = 0,
                    Inst< SFNode    >  _metadata         = 0,
                    Inst< SFOrientation>  _orientation      = 0,
                    Inst< SFPosition >  _position         = 0,
                    Inst< SFBool    >  _retainUserOffsets = 0,
                    Inst< SFTime    >  _bindTime         = 0,
                    Inst< SFBool    >  _isBound          = 0,
                    Inst< SFMatrix4f > _accForwardMatrix = 0,
                    Inst< SFMatrix4f > _accInverseMatrix = 0,
                    Inst< SFVec3f >  _center            = 0,
                    Inst< MFChild >   _children          = 0,
                    Inst< SFBool >   _displayed         = 0,
                    Inst< SFVec3f >  _size              = 0 );

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

     /// Move this instance to the stack top. 
    virtual void toStackTop() {}

    /// Remove the bindable node from the stack.
    virtual void removeFromStack() {}

    /// Returns true if the given viewpoint is in the children field
    /// any ViewpointGroups in the the children field.
    bool containsViewpoint( X3DViewpointNode *vp ) const;

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

    /// The displayed field determines whether this ViewpointGroup is displayed
    /// in the current viewpoint list. 
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> TRUE \n
    auto_ptr< SFBool > displayed;

    /// The size field provides the size of a proximity box within which the
    /// ViewpointGroup is usable and displayed on the viewpoint list. A size
    /// field of 0 0 0 specifies that the ViewpointGroup is always usable and
    /// displayed.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> TRUE \n
    auto_ptr< SFVec3f > size;

    bool display_in_list;
    
    static H3DNodeDatabase database;
  protected:
    Vec3f last_position;
  };
}

#endif
