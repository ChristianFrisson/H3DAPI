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
/// \file ToggleGroup.h
/// \brief Header file for ToggleGroup.
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __TOGGLEGROUP_H__
#define __TOGGLEGROUP_H__

#include <H3D/X3DGroupingNode.h>

namespace H3D {

  /// \ingroup H3DNodes
  /// \class ToggleGroup
  /// \brief The ToggleGroup node enables you to enable/disable haptics and 
  /// graphics rendering of its children nodes.
  ///
  /// The hapticsOn field specifies if nodes in the children field of 
  /// this group should be rendered haptically or not. The field is ignored
  /// if the hapticsOnDevice is non-empty.
  ///
  /// The hapticsOnDevice field specifies, per device, if nodes in the 
  /// children field of this group should be rendered haptically for
  /// that device or not. 
  /// If more devices are used than the number of values in 
  /// hapticsEnabled the last value of hapticsEnabled will be used 
  /// for all devices with an index bigger than available values.
  /// If hapticsOnDevice is empty the hapticsOn field is used to control
  /// haptics for all devices.
  ///
  /// The graphicsOn field specifies if nodes in the children field of 
  /// this group should be rendered graphically or not.
  ///
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/ToggleGroup.x3d">ToggleGroup.x3d</a>
  ///     ( <a href="examples/ToggleGroup.x3d.html">Source</a> )
  ///
  /// \par Internal routes:
  /// \dotfile ToggleGroup.dot
  class H3DAPI_API ToggleGroup : public X3DGroupingNode {
  public:
    /// Constructor.
    ToggleGroup( Inst< AddChildren    > _addChildren     = 0,
                 Inst< RemoveChildren > _removeChildren  = 0,
                 Inst< MFChild        > _children        = 0,
                 Inst< SFNode         > _metadata        = 0,
                 Inst< SFBound        > _bound           = 0,
                 Inst< SFVec3f        > _bboxCenter      = 0,
                 Inst< SFVec3f        > _bboxSize        = 0,
                 Inst< SFBool         > _hapticsOn       = 0,
                 Inst< SFBool         > _graphicsOn      = 0,
                 Inst< MFBool         > _hapticsOnDevice = 0,
                 Inst< SFBool         > _traverseOn  = 0);

    /// Haptics is disabled if hapticsOn is false.
    virtual void traverseSG( TraverseInfo &ti );

    /// Only renders children if graphicsOn is true. 
    virtual void render();

    /// Detect intersection between a line segment and the Node.
    /// Calls lineIntersect for its children only if graphicsOn is true.
    /// A setting in the parameter result can override this default behaviour.
    /// \param from The start of the line segment.
    /// \param to The end of the line segment.
    /// \param result Contains info about the closest intersection for every
    /// object that intersects the line.
    /// \returns true if intersected, false otherwise.
    virtual bool lineIntersect( const Vec3f &from,
                                const Vec3f &to,
                                LineIntersectResult &result );

    /// Find closest point on Node to p.
    /// Calls lineIntersect for its children only if graphicsOn is true.
    /// A setting in the parameter result can override this default behaviour.
    /// \param p The point to find the closest point to.
    /// \param result A struct containing various results of closest
    /// points such as which geometries the closest points where
    /// detected on.
    virtual void closestPoint( const Vec3f &p,
                               NodeIntersectResult &result );

    /// Detect collision between a moving sphere and the Node.
    /// Only nodes to which collision is possible will return true
    /// Calls movingSphereIntersect for its children only if graphicsOn is
    /// true. A setting in the parameter result can override this default
    /// behaviour.
    /// \param radius The radius of the sphere
    /// \param from The start position of the sphere
    /// \param to The end position of the sphere.
    /// \param result A struct containing various results of intersections
    /// such as which geometries intersected the moving sphere.
    /// \returns true if intersected, false otherwise.
    virtual bool movingSphereIntersect( H3DFloat radius,
                                        const Vec3f &from,
                                        const Vec3f &to,
                                        NodeIntersectResult &result );

    /// The hapticsOn field specifies if nodes in the children field of 
    /// this group should be rendered haptically or not. A value of TRUE
    /// indicates that children will be rendered for all haptics devices
    /// for which the hapticsEnabled field specifies TRUE. By default
    /// all devices. If FALSE haptics will be disabled for all devices.
    /// The hapticsOn field is ignored if the hapticsOnDevice field is
    /// non-empty.
    ///
    /// <b>Default value:</b> TRUE /n
    /// <b>Access type: </b> inputOutput
    /// 
    /// \dotfile ToggleGroup_hapticsOn.dot
    auto_ptr< SFBool >  hapticsOn;

    /// The graphicsOn field specifies if nodes in the children field of 
    /// this group should be rendered graphically or not.
    ///
    /// <b>Default value:</b> TRUE /n
    /// <b>Access type: </b> inputOutput
    /// 
    /// \dotfile ToggleGroup_graphicsOn.dot
    auto_ptr< SFBool >  graphicsOn;

    /// The hapticsOnDevice field specifies, per device, if nodes in the 
    /// children field of this group should be rendered haptically for
    /// that device or not. 
    /// If more devices are used than the number of values in 
    /// hapticsEnabled the last value of hapticsEnabled will be used 
    /// for all devices with an index bigger than available values.
    /// 
    /// If hapticsOnDevice is empty the hapticsOn field is used to control
    /// haptics for all devices.
    ///
    /// <b>Access type: </b> inputOutput
    /// 
    /// \dotfile ToggleGroup_hapticsOnDevice.dot
    auto_ptr< MFBool > hapticsOnDevice;

    /// If set to false the children nodes of this node will not be traversed
    /// at all
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> true \n
    auto_ptr< SFBool > traverseOn;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  };
}

#endif
