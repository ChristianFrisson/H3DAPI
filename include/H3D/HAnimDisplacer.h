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
/// \file HAnimDisplacer.h
/// \brief Header file for HAnimDisplacer, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __HANIMDISPLACER_H__
#define __HANIMDISPLACER_H__

#include <H3D/X3DGeometricPropertyNode.h>
#include <H3D/MFVec3f.h>
#include <H3D/MFInt32.h>
#include <H3D/SFString.h>
#include <H3D/SFFloat.h>


namespace H3D {

  /// \ingroup X3DNodes
  /// \class HAnimDisplacer
  /// \brief Applications may need to alter the shape of individual segments.
  ///
  /// At the most basic level, this is done by writing to the point field of
  /// the node derived from X3DCoordinateNode that is found in the coord field
  /// of the HAnimSegment node.
  ///
  /// In some cases, the application may need to be able to identify specific
  /// groups of vertices within an HAnimSegment.
  /// 
  /// EXAMPLE?The application may need to know which vertices within the skull
  /// HAnimSegment comprise the left eyebrow.
  ///
  /// It may also require "hints" as to the direction in which each vertex
  /// should move. That information is stored in a node called an HAnimDisplacer.
  /// The HAnimDisplacers for a particular HAnimSegment are stored in the
  /// displacers field of that HAnimSegment.
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/BoxMan.x3d">BoxMan.x3d</a>
  ///     ( <a href="examples/BoxMan.x3d.html">Source</a> )
  ///
  /// \par Internal routes:
  /// \dotfile HAnimDisplacer.dot
  class H3DAPI_API HAnimDisplacer : public X3DGeometricPropertyNode {
  public:
    /// Constructor.
    HAnimDisplacer( Inst< SFNode  >  _metadata = 0,
                    Inst< MFInt32 > _coordIndex = 0,
                    Inst< MFVec3f > _displacements = 0,
                    Inst< SFString > _name = 0,
                    Inst< SFFloat > _weight = 0 );

    /// Displaces the coordinates in points according to the HAnimDisplacer
    /// fields.
    /// \param points The points to displace.
    /// \param displace_transform Transformation matrix to apply to the
    /// displacements before adding them to points.
    template< class VectorType >
    inline void displaceCoordinates( VectorType &points,
                                     const Matrix4f &displace_transform  );

    /// The coordIndex field contains the indices into the coordinate array
    /// for the mesh of the vertices that are affected by the Displacer
    ///
    /// <b>Access type:</b> inputOutput \n
    ///
    /// \dotfile HAnimDisplacer_coordIndex.dot
    auto_ptr< MFInt32 > coordIndex;

    /// The displacements field, if present, provides a set of 3D values that
    /// are added to the neutral or resting position of each of the vertices
    /// referenced in the coordIndex field of the associated mesh. These values
    /// correspond one-to-one with the values in the coordIndex array of the
    /// mesh. The values shall be maximum displacements, and the application
    /// shall scale them using the value of the weight field before adding
    /// them to the neutral vertex positions.
    ///
    /// <b>Access type:</b> inputOutput \n
    ///
    /// \dotfile HAnimDisplacer_displacements.dot
    auto_ptr< MFVec3f > displacements;

    /// The name field contains a name  that is used for 
    /// identifying the object. 
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> "" \n
    /// 
    /// \dotfile HAnimDisplacer_name.dot 
    auto_ptr< SFString    >  name;

    /// The displacements shall be uniformly scaled by the
    /// value of the weight field.
    ///
    /// <b>Access type:</b> inputOutput \n
    ///
    /// \dotfile HAnimDisplacer_weight.dot
    auto_ptr< SFFloat > weight;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  };

  template< class VectorClass >
  void HAnimDisplacer::displaceCoordinates( VectorClass &points,
                                            const Matrix4f &displace_transform) {
    const vector< Vec3f > &displace = displacements->getValue();
    const vector< int > &indices = coordIndex->getValue();
    H3DFloat w = weight->getValue();

    // if weight is 0 this displacer adds nothing.
    if( w == 0 ) return;

    size_t p_size = points.size();
    size_t d_size = displace.size();

    for( size_t i = 0; i < indices.size(); ++i ) {
      int index = indices[i];
      if( (unsigned int) index < p_size && i < d_size) {
        Vec4f point_displacement = displace_transform * Vec4f( displace[i].x, displace[i].y, displace[i].z, 0 ) ;
        points[index] = w * (Vec3f(point_displacement.x,
                                   point_displacement.y,
                                   point_displacement.z ) ) + points[index];
      }
    }
  }

}

#endif
