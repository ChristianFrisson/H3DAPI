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
/// \file Collision.h
/// \brief Header file for Collision, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __COLLISION_H__
#define __COLLISION_H__

#include <H3D/X3DGroupingNode.h>
#include <H3D/X3DSensorNode.h>

namespace H3D {
  /// \ingroup X3DNodes
  /// \class Collision
  /// \brief The Collision node is a grouping node that specifies the collision
  /// detection properties for its children (and their descendants), specifies
  /// surrogate objects that replace its children during collision detection,
  /// and sends events signalling that a collision has occurred between the
  /// avatar and the Collision node's geometry or surrogate. By default, all
  /// geometric nodes in the scene are collidable with the viewer except
  /// IndexedLineSet and PointSet. Browsers shall detect geometric collisions
  /// between the avatar (see NavigationInfo) and the scene's geometry
  /// and prevent the avatar from 'entering' the geometry. See 23.2.4 Collision
  /// detection and terrain following for general information on collision
  /// detection.
  ///
  /// If there are no Collision nodes specified in a X3D file, browsers shall
  /// detect collisions between the avatar and all objects during navigation.
  /// 
  /// The Collision node's enabled field enables and disables collision
  /// detection as well as terrain following when the navigation type requires
  /// it. If enabled is set to FALSE, the children and all descendants of the
  /// Collision node shall not be checked for collision or terrain, even though
  /// they are drawn. This includes any descendent Collision nodes that have
  /// enabled set to TRUE (i.e., setting enabled to FALSE turns collision off
  /// for every child node below it).
  ///
  /// The value of the isActive field indicates the current state of the
  /// Collision node. An isActive TRUE event is generated when a collision
  /// occurs. An isActive FALSE event is generated when a collision no longer
  /// occurs.
  ///
  /// Collision nodes with the enabled field set to TRUE detect the nearest
  /// collision with their descendent geometry (or proxies). When the nearest
  /// collision is detected, the collided Collision node sends the time of the
  /// collision through its collideTime field. If a Collision node contains a
  /// child, descendant, or proxy (see below) that is a Collision node, and
  /// both Collision nodes detect that a collision has occurred, both send a
  /// collideTime event at the same time. A collideTime event shall be
  /// generated if the avatar is colliding with collidable geometry when the
  /// Collision node is read from a X3D file or inserted into the
  /// transformation hierarchy.
  ///
  /// The bboxCenter and bboxSize fields specify a bounding box that encloses
  /// the Collision node's children. This is a hint that may be used for
  /// optimization purposes. The results are undefined if the specified
  /// bounding box is smaller than the actual bounding box of the children at
  /// any time. A default bboxSize value, (-1, -1, -1), implies that the
  /// bounding box is not specified and if needed shall be calculated by the
  /// browser. More details on the bboxCenter and bboxSize fields can be found
  /// in 10.2.2 Bounding boxes..
  ///
  /// The collision proxy, defined in the proxy field, is any legal children
  /// node as described in 10.2.1 Grouping and children node types that is used
  /// as a substitute for the Collision node's children during collision
  /// detection. The proxy is used strictly for collision detection; it is not
  /// drawn.
  ///
  /// If the value of the enabled field is TRUE and the proxy field is
  /// non-NULL, the proxy field defines the scene on which collision detection
  /// is performed. If the proxy value is NULL, collision detection is
  /// performed against the children of the Collision node.
  /// If proxy is specified, any descendent children of the Collision node are
  /// ignored during collision detection. If children is empty, enabled is
  /// TRUE, and proxy is specified, collision detection is performed against
  /// the proxy but nothing is displayed. In this manner, invisible collision
  /// objects may be supported.
  ///
  /// The collideTime field generates an event specifying the time when the
  /// avatar (see 23.3.4 NavigationInfo) makes contact with the collidable
  /// children or proxy of the Collision node. An ideal implementation computes
  /// the exact time of collision. Implementations may approximate the ideal by
  /// sampling the positions of collidable objects and the user.
  /// The NavigationInfo node contains additional information for parameters
  /// that control the avatar size.
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/Collision.x3d">Collision.x3d</a>
  ///     ( <a href="examples/Collision.x3d.html">Source</a> )
  // According to X3D-spec Collision should inherit from X3DSensorNode
  // but since this is not allowed (for the database and such) the fields
  // in common with X3DSensorNode (enabled and isActive)
  // will just be defined here instead.
  class H3DAPI_API Collision : public X3DGroupingNode {
  public:
    
    typedef TypedSFNode< X3DChildNode > SFChild;

    /// Constructor.
    Collision( Inst< AddChildren    > _addChildren    = 0,
               Inst< RemoveChildren > _removeChildren = 0,
               Inst< SFBool         > _enabled        = 0,
               Inst< MFChild        > _children       = 0,
               Inst< SFNode         > _metadata       = 0,
               Inst< SFTime         > _collideTime    = 0,
               Inst< SFBool         > _isActive       = 0,
               Inst< SFBound        > _bound          = 0,
               Inst< SFVec3f        > _bboxCenter     = 0,
               Inst< SFVec3f        > _bboxSize       = 0,
               Inst< SFChild        > _proxy          = 0 );

    /// Detect intersection between a line segment and a Node.
    /// Calls lineIntersect for all children if the Node is enabled.
    /// \param from The start of the line segment.
    /// \param to The end of the line segment.
    /// \param result Contains info about the closest intersection for every
    /// object that intersects the line.
    /// \returns true if intersected, false otherwise.
    virtual bool lineIntersect(
      const Vec3f &from, 
      const Vec3f &to,    
      LineIntersectResult &result );

    /// Find closest point on Node to p. Calls closestPoint for
    /// all children if the node is enabled.
    /// \param p The point to find the closest point to.
    /// \param result A struct containing various results of closest
    /// points such as which geometries the closest points where
    /// detected on.
    virtual void closestPoint( const Vec3f &p,
                               NodeIntersectResult &result );

    /// Detect collision between a moving sphere and the Node.
    /// Calls movingSphereIntersect for all children if the node is
    /// enabled.
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

    /// TODO: implement this field.
    /// The collideTime field generates an event specifying the time when the
    /// avatar (see NavigationInfo) makes contact with the collidable
    /// children or proxy of the Collision node.
    ///
    /// <b>Access type:</b> outputOnly \n
    auto_ptr< SFTime > collideTime;

    /// The collision proxy, defined in the proxy field, is any legal children
    /// node as described in 10.2.1 Grouping and children node types that is
    /// used as a substitute for the Collision node's children during collision
    /// detection. The proxy is used strictly for collision detection; it is
    /// not drawn.
    ///
    /// <b>Access type:</b> inpuOutput \n
    /// <b>Default value:</b> NULL \n
    auto_ptr< SFChild > proxy;

    // Fields defined to fake this node as a sensor
    /// The enabled field enables and disables collision detection as well as
    /// terrain following (not implemented in H3DAPI) when the navigation type
    /// requires it. If enabled is set to FALSE, the children and all
    /// descendants of the Collision node shall not be checked for collision
    /// or terrain, even though they are drawn. This includes any descendent
    /// Collision nodes that have enabled set to TRUE (i.e., setting enabled to
    /// FALSE turns collision off for every child node below it).
    ///
    /// <b>Access type:</b> inpuOutput \n
    /// <b>Default value:</b> true \n
    auto_ptr< SFBool >  enabled;

    /// The value of the isActive field indicates the current state of the
    /// Collision node. An isActive TRUE event is generated when a collision
    /// occurs. An isActive FALSE event is generated when a collision no longer
    /// occurs.
    ///
    /// <b>Access type:</b> inpuOutput \n
    /// <b>Default value:</b> false\n
    auto_ptr<  SFBool > isActive;
    
    static H3DNodeDatabase database;
  };
}

#endif
