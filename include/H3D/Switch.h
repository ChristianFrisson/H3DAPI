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
/// \file Switch.h
/// \brief Header file for Switch, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __SWITCH_H__
#define __SWITCH_H__

#include <H3D/X3DGroupingNode.h>
#include <H3D/SFInt32.h>

namespace H3D {

  /// \ingroup X3DNodes
  /// \class Switch
  /// \brief The Switch grouping node traverses zero or one of the nodes
  /// specified in the choice field. 
  ///
  /// The whichChoice field specifies the index of the child to traverse,
  /// with the first child having index 0. If whichChoice is less than zero
  /// or greater than the number of nodes in the choice field, nothing is
  /// chosen.
  ///
  /// All nodes under a Switch continue to receive and send events
  /// regardless of the value of whichChoice. For example, if an active
  /// TimeSensor is contained within an inactive choice of an Switch, the
  /// TimeSensor sends events regardless of the Switch's state.
  /// 
  /// The bboxCenter and bboxSize fields specify a bounding box that
  /// encloses the Switch node's children. This is a hint that may be used
  /// for optimization purposes. The results are undefined if the specified
  /// bounding box is smaller than the actual bounding box of the child with
  /// the largest bounding box at any time. A default bboxSize value, (-1,
  /// -1, -1), implies that the bounding box is not specified and, if
  /// needed, is calculated by the browser. 
  ///
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/Switch.x3d">Switch.x3d</a>
  ///     ( <a href="examples/Switch.x3d.html">Source</a> )
  ///
  /// \par Internal routes:
  /// \dotfile Switch.dot
  class H3DAPI_API Switch : public X3DGroupingNode {
  public:
    
    /// SFBound is specialized to update from the SFBound fields 
    /// routed to it. The resulting Bound object is the bound routed
    /// to to the field with the index given by the first route
    /// If the bboxSize of the 
    /// X3DGroupingNode containing the SFBound field is ( -1, -1, -1 )
    /// bound fields of all Nodes in the children field of the containing
    /// Node that are instances of X3DBoundedObject are routed to it.
    /// Otherwise the bound will be a BoxBound with center and
    /// radius specified with the bboxCenter and bboxSize fields.
    ///
    /// inputs[0] is the whichChoice field.
    class H3DAPI_API SFBound: 
      public TypedField< X3DGroupingNode::SFBound,
                         SFInt32,
                         AnyNumber< X3DBoundedObject::SFBound > > {
      /// The SFBound is updated to a bound that is the union of the 
      /// the Bound objects routed to it.
      virtual void update();
    };

    /// Constructor
    Switch( Inst< AddChildren    > _addChildren    = 0,
            Inst< RemoveChildren > _removeChildren = 0,
            Inst< MFChild        > _children       = 0,
            Inst< SFNode         > _metadata       = 0,
            Inst< SFBound        > _bound          = 0,
            Inst< SFVec3f        > _bboxCenter     = 0,
            Inst< SFVec3f        > _bboxSize       = 0,
            Inst< SFInt32        > _whichChoice    = 0 );

    /// Only render the child defined by whichChoice,
    virtual void render();

    /// Only traverse the childe defined by whichChoice
    virtual void traverseSG( TraverseInfo &ti ); 

    /// Detect intersection between a line segment and a Node.
    /// Calls lineIntersect for the same child as the one that should be
    /// rendered.
    /// \param from The start of the line segment.
    /// \param to The end of the line segment.
    /// \param result Contains info about the closest intersection for every
    /// object that intersects the line.
    /// \returns true if intersected, false otherwise.
    virtual bool lineIntersect(
      const Vec3f &from, 
      const Vec3f &to,    
      LineIntersectResult &result );

    /// Find closest point on Node to p. Calls closestPoint for the same child
    /// as the one that should be rendered.
    /// \param p The point to find the closest point to.
    /// \param result A struct containing various results of closest
    /// points such as which geometries the closest points where
    /// detected on.
    virtual void closestPoint( const Vec3f &p,
                               NodeIntersectResult &result );

    /// Detect collision between a moving sphere and the Node.
    /// Calls movingSphereIntersect for the same child as the one that should
    /// be rendered.
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

    /// The whichChoice field specifies the index of the child to traverse,
    /// with the first child having index 0. If whichChoice is less than zero
    /// or greater than the number of nodes in the choice field, nothing is
    /// chosen.
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> -1 \n
    ///
    /// \dotfile Switch_whichChoice.dot 
    auto_ptr< SFInt32 > whichChoice;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  };
}

#endif
