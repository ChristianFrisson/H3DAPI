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
/// \file PointSet.h
/// \brief Header file for PointSet, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __POINTSET_H__
#define __POINTSET_H__

#include <H3D/X3DGeometryNode.h>
#include <H3D/DependentNodeFields.h>
#include <H3D/X3DCoordinateNode.h>
#include <H3D/X3DColorNode.h>
#include <H3D/CoordBoundField.h>
#include <H3D/FogCoordinate.h>

namespace H3D {

  /// \ingroup X3DNodes
  /// \class PointSet
  /// \brief The PointSet node specifies a set of 3D points, in the local
  /// coordinate system, with associated colours at each point.
  ///
  /// The coord field specifies a X3DCoordinateNode. PointSet uses the
  /// coordinates in order. If the coord field is NULL, the point set 
  /// is considered empty.
  ///
  /// PointSet nodes are not lit, not texture-mapped, nor do they
  /// participate in collision detection. The size of each point is
  /// implementation-dependent. 
  ///
  /// If the color field is not NULL, it shall specify a Color node that
  /// contains at least the number of points contained in the coord
  /// node. Colours  are applied to each point in order. The
  /// results are undefined if the number of values in the Color node is
  /// less than the number of values specified in the Coordinate node. 
  ///
  /// If the color field is NULL and there is a Material node defined for the
  /// Appearance node affecting this PointSet node, the emissiveColor of the
  /// Material node shall be used to draw the points. 
  ///
  /// \par Internal routes:
  /// \dotfile PointSet.dot
  class H3DAPI_API PointSet : public X3DGeometryNode {
  public:

    /// Thrown when the number of colors in the color field are less
    /// than the number of points in the coord field.
    H3D_VALUE_EXCEPTION( unsigned int, NotEnoughColors );
    
    /// SFCoordinateNode is dependent on coordChanged field in 
    /// X3DCoordinateNode.
    typedef DependentSFNode< X3DCoordinateNode,
                             FieldRef< X3DGeometricPropertyNode,
                                       Field,
                                       &X3DCoordinateNode::propertyChanged > > 
    SFCoordinateNode;

    /// SFColorNode is dependent on colorChanged field in 
    /// X3DColorNode.
    typedef DependentSFNode< X3DColorNode,
                             FieldRef< X3DGeometricPropertyNode,
                                       Field,
                                       &X3DColorNode::propertyChanged > > 
    SFColorNode;

    /// The SFFogCoordinate is dependent on the propertyChanged
    /// field of the contained FogCoordinate.
    typedef DependentSFNode< 
                FogCoordinate,
                FieldRef< X3DGeometricPropertyNode,
                          Field,
                          &FogCoordinate::propertyChanged > > 
    SFFogCoordinate; 

    /// The bound field for PointSet is a CoordBoundField.
    typedef CoordBoundField SFBound;

    /// Constructor.
    PointSet( Inst< SFNode           > _metadata       = 0,
              Inst< SFBound          > _bound          = 0,
              Inst< DisplayList      > _displayList    = 0,
              Inst< SFColorNode      > _color          = 0,
              Inst< SFCoordinateNode > _coord          = 0,
              Inst< SFFogCoordinate  > _fogCoord        =0);

    /// Render the LineSet with OpenGL
    virtual void render();
    
    /// The number of points rendered by this geometry.
    virtual int nrPoints() {
      X3DCoordinateNode *coord_node = coord->getValue();
      if( coord_node ) {
        unsigned int size = coord_node->nrAvailableCoords();
        if( size > 1 )return size - 1;
      }
      return 0;
    }

    /// Detect collision between a moving sphere and the geometry.
    /// \param radius The radius of the sphere
    /// \param from The start position of the sphere
    /// \param to The end position of the sphere.
    /// \param result A struct containing various results of intersections
    /// such as which geometries intersected the moving sphere.
    /// \returns true if intersected, false otherwise.
    virtual bool movingSphereIntersect( H3DFloat radius,
                                        const Vec3f &from, 
                                        const Vec3f &to,
                                        NodeIntersectResult &result ) {
      return false;
    }

    /// If the color field is not NULL, it shall specify a Color node that
    /// contains at least the number of points contained in the coord
    /// node. Colours  are applied to each point in order.   
    /// If the color field is NULL and there is a Material node defined for the
    /// Appearance node affecting this PointSet node, the emissiveColor of the
    /// Material node shall be used to draw the points. 
    ///
    /// <b>Access type:</b> inputOutput 
    ///
    /// \dotfile PointSet_color.dot 
    auto_ptr< SFColorNode >  color;

    /// The coord field specifies a X3DCoordinateNode. PointSet uses the
    /// coordinates in order. If the coord field is NULL, the point set 
    /// is considered empty.
    ///
    /// <b>Access type:</b> inputOutput 
    ///
    /// \dotfile PointSet_coord.dot 
    auto_ptr< SFCoordinateNode >  coord;

    
    /// If the fogCoord field is not empty, it shall contain a list 
    /// of per-vertex depth values for calculating fog depth.
    /// 
    /// <b>Access type:</b> inputOutput \n
    ///
    /// \dotfile PointSet_fogCoord.dot 
    auto_ptr< SFFogCoordinate > fogCoord;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  };
}

#endif
