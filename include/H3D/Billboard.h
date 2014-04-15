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
/// \file Billboard.h
/// \brief Header file for Billboard, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __BILLBOARD_H__
#define __BILLBOARD_H__

#include <H3D/MatrixTransform.h>

namespace H3D {

  /// \ingroup X3DNodes
  /// \class Billboard
  /// \brief The Billboard node is a grouping node
  /// which modifies its coordinate system so that the Billboard
  /// node's local Z-axis turns to point at the viewer. The Billboard
  /// node has children which may be other children nodes.
  ///
  /// The axisOfRotation field specifies which axis to use to perform the
  /// rotation. This axis is defined in the local coordinate system. When
  /// the axisOfRotation field is not (0, 0, 0), the following steps
  /// describe how to rotate the billboard to face the viewer:
  ///
  /// <ol>
  /// <li> Compute the vector from the Billboard node's origin to the viewer's
  ///    position. This vector is called the billboard-to-viewer vector.
  ///
  /// <li> Compute the plane defined by the axisOfRotation and the
  ///    billboard-to-viewer vector.
  ///
  /// <li> Rotate the local Z-axis of the billboard into the plane from b.,
  ///   pivoting around the axisOfRotation.
  /// </ol>
  /// When the axisOfRotation field is set to (0, 0, 0), the special case of
  /// viewer-alignment is indicated. In this case, the object rotates to
  /// keep the billboard's local Y-axis parallel with the Y-axis of the
  /// viewer. This special case is distinguished by setting the
  /// axisOfRotation to (0, 0, 0). The following steps describe how to align
  /// the billboard's Y-axis to the Y-axis of the viewer:
  ///
  /// <ol>
  /// <li> Compute the billboard-to-viewer vector.
  ///
  /// <li> Rotate the Z-axis of the billboard to be collinear with the
  ///    billboard-to-viewer vector and pointing towards the viewer's 
  ///    position. 
  ///
  /// <li> Rotate the Y-axis of the billboard to be parallel and oriented in 
  ///    the same direction as the Y-axis of the viewer.
  /// </ol>
  /// If the axisOfRotation and the billboard-to-viewer line are coincident,
  /// the plane cannot be established and the resulting rotation of the
  /// billboard is undefined. For example, if the axisOfRotation is set to
  /// (0,1,0) (Y-axis) and the viewer flies over the billboard and peers
  /// directly down the Y-axis, the results are undefined.
  ///
  /// Multiple instances of Billboard nodes (DEF/USE) operate as expected:
  /// each instance rotates in its unique coordinate system to face the
  /// viewer.
  ///
  /// The bboxCenter and bboxSize fields specify a bounding box that
  /// encloses the Billboard node's children. This is a hint that may be
  /// used for optimization purposes. The results are undefined if the
  /// specified bounding box is smaller than the actual bounding box of the
  /// children at any time. A default bboxSize value, (-1, -1, -1), implies
  /// that the bounding box is not specified and if needed shall be
  /// calculated by the browser. 
  ///
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/Billboard.x3d">Billboard.x3d</a>
  ///     ( <a href="examples/Billboard.x3d.html">Source</a> )
  ///
  /// \par Internal routes:
  /// \dotfile Billboard.dot
  class H3DAPI_API Billboard : public MatrixTransform {
  public:
    
    /// Specialize the SFMatrix4f to update the matrix from the
    /// fields in the Billboard node.
    /// routes_in[0] axisOfRotation field
    /// routes_in[1] vp_position field
    /// routes_in[2] vp_up field
    class H3DAPI_API SFMatrix4f: 
      public TypedField< H3D::SFMatrix4f,
                         Types< SFVec3f, SFVec3f, SFVec3f > >{
    protected:
 
      /// Update the matrix from the fields in the Billboard node.
      virtual void update();
    };


    /// Constructor.
    Billboard( Inst< AddChildren        > _addChildren      = 0,
               Inst< RemoveChildren     > _removeChildren   = 0,
               Inst< MFChild            > _children         = 0,
               Inst< SFNode             > _metadata         = 0,
               Inst< SFBound            > _bound            = 0,
               Inst< SFVec3f            > _bboxCenter       = 0,
               Inst< SFVec3f            > _bboxSize         = 0,
               Inst< SFTransformedBound > _transformedBound = 0,
               Inst< SFMatrix4f         > _matrix           = 0,
               Inst< SFVec3f            > _axisOfRotation   = 0 );

    /// Set the matrix field in order to set the matrix field to perform
    /// the rotation required by the Billboard.
    /// \param ti The TraverseInfo object containing information about the
    /// traversal.
    virtual void traverseSG( TraverseInfo &ti );

    /// The axisOfRotation field specifies which axis to use to perform
    /// the rotation. This axis is defined in the local coordinate system.
    /// When the axisOfRotation field is set to (0, 0, 0), the special 
    /// case of viewer-alignment is indicated.
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> Vec3f( 0, 1, 0 ) \n
    /// 
    /// \dotfile Billboard_axisOfRotation.dot
    auto_ptr< SFVec3f > axisOfRotation;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;

  protected:
    // The position of the viewpoint in local coordinates of the Billboard
    // node. This field will be set in traverseSG().
    auto_ptr< SFVec3f > vpPosition;

    // The up vector of the viewpoint in local coordinates of the Billboard
    // node. This field will be set in traverseSG().
    auto_ptr< SFVec3f > vpUp;
  };
}

#endif
