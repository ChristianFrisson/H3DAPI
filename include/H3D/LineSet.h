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
/// \file LineSet.h
/// \brief Header file for LineSet, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __LINESET_H__
#define __LINESET_H__

#include <H3D/X3DGeometryNode.h>
#include <H3D/DependentNodeFields.h>
#include <H3D/X3DCoordinateNode.h>
#include <H3D/X3DColorNode.h>
#include <H3D/CoordBoundField.h>
#include <H3D/MFInt32.h>
#include <H3D/FogCoordinate.h>

namespace H3D {

  /// \ingroup X3DNodes
  /// \class LineSet
  /// \brief The LineSet node represents a 3D geometry formed by constructing
  /// polylines from 3D vertices specified in the coord field. 
  ///
  /// The color field specifies the colour of the line set at each vertex
  /// and contains a X3DColorNode. If the color field is NULL and there is a
  /// Material defined for the Appearance affecting this LineSet, the
  /// emissiveColor of the Material shall be used to draw the lines. 
  ///
  /// The coord field specifies the 3D vertices of the line set and contains
  /// a Coordinate node. 
  ///
  /// The vertexCount field describes how many vertices are to be used in
  /// each polyline from the coordinate field. Coordinates are assigned to
  /// each line by taking vertexCount[n] vertices from the coordinate
  /// field. Each value of the vertexCount array shall be greater than or
  /// equal to two. It shall be an error to have a value less than two. 
  ///
  /// Lines are not lit, are not texture-mapped, and do not participate in
  /// collision detection. The width of lines is implementation dependent
  /// and each line segment is solid (i.e., not dashed). 
  ///
  /// \par Internal routes:
  /// \dotfile LineSet.dot
  class H3DAPI_API LineSet : public X3DGeometryNode {
  public:
    /// Thrown then vertexCount value is < 2.
    H3D_VALUE_EXCEPTION( int, InvalidVertexCountValue );

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

    /// The bound field for LineSet is a CoordBoundField.
    typedef CoordBoundField SFBound;

    /// Constructor
    LineSet( Inst< SFNode           > _metadata       = 0,
             Inst< SFBound          > _bound          = 0,
             Inst< DisplayList      > _displayList    = 0,
             Inst< SFColorNode      > _color          = 0,
             Inst< SFCoordinateNode > _coord          = 0,
             Inst< MFInt32          > _vertexCount    = 0,
             Inst< SFFogCoordinate  > _fogCoord       = 0 );

    /// The number of lines rendered by this geometry.
    virtual int nrLines() {
      X3DCoordinateNode *coord_node = coord->getValue();
      if( coord_node ) {
        unsigned int size = coord_node->nrAvailableCoords();
        if( size > 1 )return size - 1;
      }
      return 0;
    }

    /// Render the LineSet with OpenGL
    virtual void render();

    /// The color field specifies the colour of the line set at each vertex
    /// and contains a X3DColorNode. If the color field is NULL and there is a
    /// Material defined for the Appearance affecting this LineSet, the
    /// emissiveColor of the Material shall be used to draw the lines. 
    ///
    /// <b>Access type:</b> inputOutput 
    ///
    /// \dotfile LineSet_color.dot 
    auto_ptr< SFColorNode  >  color;
    /// The coord field specifies the 3D vertices of the line set and contains
    /// a X3DCoordinateNode.
    ///
    /// <b>Access type:</b> inputOutput 
    ///
    /// \dotfile LineSet_coord.dot 
    auto_ptr< SFCoordinateNode  >  coord;

    /// The vertexCount field describes how many vertices are to be used in
    /// each polyline from the coordinate field. Coordinates are assigned to
    /// each line by taking vertexCount[n] vertices from the coordinate
    /// field.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Value range:</b> >=2
    ///
    /// \dotfile LineSet_coord.dot 
    auto_ptr< MFInt32 >  vertexCount;
    
    /// If the fogCoord field is not empty, it shall contain a list 
    /// of per-vertex depth values for calculating fog depth.
    /// 
    /// <b>Access type:</b> inputOutput \n
    ///
    /// \dotfile FogCoordinate_fogCoord.dot 
    auto_ptr< SFFogCoordinate > fogCoord;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  };
}

#endif
