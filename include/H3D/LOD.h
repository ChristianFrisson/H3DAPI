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
/// \file LOD.h
/// \brief Header file for LOD, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __LOD_H__
#define __LOD_H__

#include <H3D/MatrixTransform.h>
#include <H3D/SFInt32.h>
#include <H3D/MFFloat.h>


namespace H3D {

  /// \ingroup X3DNodes
  /// \class LOD
  /// \brief The LOD node specifies various levels of detail or 
  ///  complexity for a given object, and provides hints allowing 
  ///  browsers to automatically choose the appropriate version of 
  ///  the object based on the distance from the user. The children 
  ///  field contains a list of nodes that represent the same object
  ///  or objects at varying levels of detail, ordered from highest 
  ///  level of detail to the lowest level of detail. 
  ///
  ///  The range field specifies the ideal distances at which to 
  ///  switch between the levels. The forceTransitions field specifies 
  ///  whether browsers are allowed to disregard level distances in 
  ///  order to provide better performance. A forceTransitions value of
  ///  TRUE specifies that every transition should be performed regardless 
  ///  of any internal optimizations that might be available. 
  ///  A forceTransitions value of FALSE specifies that browsers are 
  ///  allowed to disregard level distances in order to provide better
  ///  performance. 
  ///
  ///  The center field is a translation offset in the local coordinate 
  ///  system that specifies the centre of the LOD node for distance 
  ///  calculations.
  ///
  ///  The number of nodes in the children field shall exceed the number
  ///  of values in the range field by one (i.e., N+1 children nodes for 
  ///  N range values). The range field contains monotonic increasing 
  ///  values that shall be greater than zero. In order to calculate 
  ///  which level to display, first the distance is calculated from the 
  ///  viewer's location, transformed into the local coordinate system of 
  ///  the LOD node (including any scaling transformations), to the center
  ///  point of the LOD node. Then, the LOD node evaluates the step function 
  ///  L(d) to choose a level for a given value of d (where d is the distance
  ///  from the viewer position to the centre of the LOD node).
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/LOD.x3d">LOD.x3d</a>
  ///     ( <a href="examples/LOD.x3d.html">Source</a> )
  class H3DAPI_API LOD : public MatrixTransform {
  public:
    
    /// Constructor.
    LOD(       Inst< AddChildren    > _addChildren      = 0,
               Inst< RemoveChildren > _removeChildren   = 0,
               Inst< MFChild        > _children         = 0,
               Inst< SFNode         > _metadata         = 0,
               Inst< SFBound        > _bound            = 0,
               Inst< SFInt32        > _level_changed    = 0,
               Inst< SFVec3f        > _bboxCenter       = 0,
               Inst< SFVec3f        > _bboxSize         = 0,
               Inst< SFVec3f        > _center           = 0,
               Inst< SFBool         > _forceTransitions = 0,
               Inst< MFFloat        > _range            = 0 );
    
    virtual void render();

    /// Traverse the scenegraph. Calculates distance to viewer and decides
    /// which level in the LOD should be used for rendering.
    /// \param ti The TraverseInfo object containing information about the
    /// traversal.
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

    /// The LOD node generates a level_changed 
    /// event with value i where the value of i
    /// identifies which level was activated 
    /// for display
    /// 
    /// <b>Access type:</b> outputOnly \n
    /// <b>Default value:</b> SFInt32 = 0 \n
    /// 
    /// \dotfile LOD_level_changed.dot
    auto_ptr< SFInt32 > level_changed;

    /// This field is only accessible in C++
    auto_ptr <SFInt32> display_index; 

    ///  The center field is a translation
    ///  offset in the local coordinate system 
    ///  that specifies the centre of the LOD
    ///  node for distance calculations.
    ///
    /// <b>Default value:</b> 0 0 0 \n
    /// <b>Access type:</b> inputOutput \n
    /// 
    /// \dotfile LOD_center.dot
    auto_ptr< SFVec3f > center;

    /// The forceTransitions field specifies
    /// whether browsers are allowed to disregard 
    /// level distances in order to provide better 
    /// performance.
    ///
    /// <b>Default value:</b> false \n
    /// <b>Access type:</b> inputOutput \n
    /// 
    /// \dotfile LOD_forceTransitions.dot
    auto_ptr< SFBool > forceTransitions;

    /// The range field specifies the ideal 
    /// distances at which to switch between 
    /// the levels. 
    ///
    /// Each value in the range field shall 
    /// be greater than the previous value.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// 
    /// \dotfile LOD_range.dot
    auto_ptr< MFFloat > range;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  };
}

#endif
