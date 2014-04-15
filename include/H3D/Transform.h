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
/// \file Transform.h
/// \brief Header file for Transform, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __TRANSFORM_H__
#define __TRANSFORM_H__

#include <H3D/MatrixTransform.h>
#include <H3D/SFRotation.h>

namespace H3D {

  /// \ingroup X3DNodes
  /// \class Transform
  /// \brief The Transform node is a grouping node that defines a coordinate system
  /// for its children that is relative to the coordinate systems of its
  /// ancestors.
  ///
  /// The bboxCenter and bboxSize fields specify a bounding box that
  /// encloses the children of the Transform node. This is a hint that may
  /// be used for optimization purposes. The results are undefined if the
  /// specified bounding box is smaller than the actual bounding box of the
  /// children at any time. A default bboxSize value, (-1, -1, -1), implies
  /// that the bounding box is not specified and, if needed, shall be
  /// calculated by the browser. The bounding box shall be large enough at
  /// all times to enclose the union of the group's children's bounding
  /// boxes; it shall not include any transformations performed by the group
  /// itself (i.e., the bounding box is defined in the local coordinate
  /// system of the children). The results are undefined if the specified
  /// bounding box is smaller than the true bounding box of the group. 
  /// 
  /// The translation, rotation, scale, scaleOrientation and center fields
  /// define a geometric 3D transformation consisting of (in order):
  ///
  ///   - a (possibly) non-uniform scale about an arbitrary point; 
  ///   - a rotation about an arbitrary point and axis; 
  ///   - a translation.  
  ///
  /// The center field specifies a translation offset from the origin of the
  /// local coordinate system (0,0,0). The rotation field specifies a
  /// rotation of the coordinate system. The scale field specifies a
  /// non-uniform scale of the coordinate system. scale values may have any
  /// value: positive, negative (indicating a reflection), or zero. A value
  /// of zero indicates that any child geometry shall not be displayed. The
  /// scaleOrientation specifies a rotation of the coordinate system before
  /// the scale (to specify scales in arbitrary orientations). The
  /// scaleOrientation applies only to the scale operation. The translation
  /// field specifies a translation to the coordinate system.
  ///
  /// Given a 3-dimensional point P and Transform node, P is transformed
  /// into point P' in its parent's coordinate system by a series of
  /// intermediate transformations. In matrix transformation notation, where
  /// C (center), SR (scaleOrientation), T (translation), R (rotation), and
  /// S (scale) are the equivalent transformation matrices,
  /// 
  /// <center>
  ///    P' = T * C * R * SR * S * -SR * -C * P
  /// </center>
  /// 
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/Transform.x3d">Transform.x3d</a>
  ///     ( <a href="examples/Transform.x3d.html">Source</a> )
  ///
  /// \par Internal routes:
  /// \dotfile Transform.dot
  class H3DAPI_API Transform : public MatrixTransform {
  public:
    
    /// Specialize the SFMatrix4f to update the matrix from the
    /// fields in the Transform node.
    /// routes_in[0] center field
    /// routes_in[1] rotation field
    /// routes_in[2] scale field
    /// routes_in[3] scaleOrientation field
    /// routes_in[4] translation field
    ///
    class H3DAPI_API Matrix: 
      public TypedField< H3D::SFMatrix4f,
                         Types< SFVec3f, SFRotation, SFVec3f, 
                                SFRotation, SFVec3f > >{
    protected:
 
      /// Update the matrix from the fields in the Transform node.
      virtual void update();
    };


    /// Constructor.
    Transform( Inst< AddChildren >        _addChildren        = 0,
               Inst< RemoveChildren >     _removeChildren     = 0,
               Inst< SFVec3f    >         _center             = 0,
               Inst< MFChild    >         _children           = 0,
               Inst< SFNode     >         _metadata           = 0,
               Inst< SFRotation >         _rotation           = 0,
               Inst< SFVec3f    >         _scale              = 0,
               Inst< SFRotation >         _scaleOrientation   = 0,
               Inst< SFVec3f    >         _translation        = 0,
               Inst< SFBound    >         _bound              = 0,
               Inst< SFVec3f    >         _bboxCenter         = 0,
               Inst< SFVec3f    >         _bboxSize           = 0,
               Inst< SFTransformedBound > _transformedBound   = 0,
               Inst< Matrix >             _matrix             = 0,
               Inst< SFMatrix4f >         _accumulatedForward = 0,
               Inst< SFMatrix4f >         _accumulatedInverse = 0  );


    virtual void render();

    /// The center field specifies a translation offset from the origin
    /// of the local coordinate system that the rotation and scaling should
    /// occur around.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> Vec3f( 0, 0, 0 ) \n
    /// 
    /// \dotfile Transform_center.dot
    auto_ptr<    SFVec3f    >  center;

    /// The rotation field specifies an arbitrary rotation.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> Rotation( 0, 0, 1, 0 ) \n
    /// 
    /// \dotfile Transform_rotation.dot
    auto_ptr<    SFRotation >  rotation;

    /// The scale field specifies a non-uniform scale of the coordinate 
    /// system. scale values may have any value: positive, negative 
    /// (indicating a reflection), or zero. A value of zero indicates
    /// that any child geometry shall not be displayed. The rotation field
    /// specifies an arbitrary rotation.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> Vec3f( 1, 1, 1 ) \n
    /// 
    /// \dotfile Transform_scale.dot
    auto_ptr<    SFVec3f    >  scale;

    /// The scaleOrientation specifies a rotation of the coordinate system
    /// before the scale (to specify scales in arbitrary orientations). 
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> Rotation( 0, 0, 1, 0 ) \n
    /// 
    /// \dotfile Transform_scaleOrientation.dot
    auto_ptr<    SFRotation >  scaleOrientation;

    /// Specifies a translation to the coordinate system.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> Vec3f( 0, 0, 0 ) \n
    /// 
    /// \dotfile Transform_translation.dot
    auto_ptr<    SFVec3f    >  translation;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  };
}

#endif
