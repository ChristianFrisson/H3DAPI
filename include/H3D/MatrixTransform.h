/////////////////////////////////////////////////////////////////////////////
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
/// \file MatrixTransform.h
/// \brief Header file for MatrixTransform, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __MATRIXTRANSFORM_H__
#define __MATRIXTRANSFORM_H__

#include <H3D/X3DGroupingNode.h>

namespace H3D {

  /// \ingroup X3DNodes
  /// \class MatrixTransform
  /// \brief The MatrixTransform node is a grouping node that defines a 
  /// coordinate system for its children that is relative to the coordinate
  /// systems of its ancestors. The matrix field is a 4x4 matrix defining
  /// the transform matrix from the children's coordinate system to its
  /// ancestors coordinate system.
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
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/MatrixTransform.x3d">MatrixTransform.x3d</a>
  ///     ( <a href="examples/MatrixTransform.x3d.html">Source</a> )
  ///
  /// \par Internal routes:
  /// \dotfile MatrixTransform.dot
  class H3DAPI_API MatrixTransform : public X3DGroupingNode {
  public:
    typedef H3D::SFMatrix4f SFMatrix4f;

    /// SFTransformedBound maintains the routes from the owner Node's bound
    /// and matrix field to the contained TransformedBoxBound instance's 
    /// boxBound and matrix field. The contained Bound object is updated
    /// from the given SFMatrix4f and SFBound fields. If the SFBound contains
    /// a InfiniteBound this field also will contain a InfiniteBound. Otherwise
    /// it will contain a TransformedBoxBound .
    ///
    /// routes_in[0] is the matrix field of the MatrixTransform.
    /// routes_in[1] is the bound field of the MatrixTransform.
    ///
    class H3DAPI_API SFTransformedBound: 
      public TypedField< SFBound, Types< SFMatrix4f, SFBound > > {
    public:
    
      /// Destructor. Sets the value to NULL in order to get the correct
      /// onRemove () function to be called upon destruction. Must be done
      /// in all fields that override onRemove ().
      ~SFTransformedBound() {
        value = NULL;
      }

    protected:
      /// 
      virtual void update();

      /// Set up the routes from the owner Node's bound
      /// and matrix field to the contained TransformedBoxBound instance's 
      /// boxBound and matrix field.
      virtual void onAdd( Bound *b ) {
        SFBound::onAdd( b );
        TransformedBoxBound *tbb = dynamic_cast< TransformedBoxBound * >( b );
        if( b && tbb ) {
          MatrixTransform *mt = static_cast< MatrixTransform * >(owner);
          mt->bound->route( tbb->boxBound );
          mt->matrix->route( tbb->matrix );
        }
      }
      
      /// Remove the routes that were set up in onAdd().
      virtual void onRemove( Bound *b ) {
        TransformedBoxBound *tbb = dynamic_cast< TransformedBoxBound * >( b );
        if( b && tbb ) {
          MatrixTransform *mt = static_cast< MatrixTransform * >(owner);
          mt->bound->unroute( tbb->boxBound );
          mt->matrix->unroute( tbb->matrix );
        }
        SFBound::onRemove( b );
      }

      friend class MatrixTransform;
    };
    
    /// Constructor.
    MatrixTransform( Inst< AddChildren > _addChildren             = 0,
                     Inst< RemoveChildren > _removeChildren       = 0,
                     Inst< MFChild    > _children                 = 0,
                     Inst< SFNode     > _metadata                 = 0,
                     Inst< SFBound    > _bound                    = 0,
                     Inst< SFVec3f    > _bboxCenter               = 0,
                     Inst< SFVec3f    > _bboxSize                 = 0,
                     Inst< SFTransformedBound > _transformedBound = 0,
                     Inst< SFMatrix4f > _matrix                   = 0,
                     Inst< SFMatrix4f > _accumulatedForward       = 0,
                     Inst< SFMatrix4f > _accumulatedInverse       = 0 );

    virtual void render();

    /// Traverse the scenegraph. 
    virtual void traverseSG( TraverseInfo &ti );
    
    /// Multiply the currently active OpenGL matrix with the matrix
    /// of this node.
    void multiplyGLMatrix();

    /// Detect intersection between a line segment and this Node.
    /// Transforms the line into correct coordinate system and also transforms
    /// the result if there is a result. Calls lineIntersect for its children.
    /// \param from The start of the line segment.
    /// \param to The end of the line segment.
    /// \param result Contains info about the closest intersection for every
    /// object that intersects the line.
    /// \returns true if intersected, false otherwise.
    virtual bool lineIntersect( 
      const Vec3f &from, 
      const Vec3f &to,    
      LineIntersectResult &result );

    /// Find closest point on Node to p. Transforms point
    /// p and calls closestPoint for all children.
    /// \param p The point to find the closest point to.
    /// \param result A struct containing various results of closest
    /// points such as which geometries the closest points where
    /// detected on.
    virtual void closestPoint( const Vec3f &p,
                               NodeIntersectResult &result );

    /// Detect collision between a moving sphere and the Node.
    /// Transforms points and radius (if non-uniform scaling the biggest
    /// of the scale parameters is chosen)
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

    /// The transformation matrix.
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> Unit matrix \n
    /// 
    /// \dotfile MatrixTransform_matrix.dot
    auto_ptr< SFMatrix4f > matrix;
    
    /// The accumulated forward transform matrix, i.e. the
    /// transformation matrix from the local space at the current 
    /// traversal position to global space. Note that this field will not stay
    /// constant for a constant scene graph if the DEF/USE feature is used for
    /// instances of this node.
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> Unit matrix \n
    auto_ptr< SFMatrix4f > accumulatedForward;
    
    /// The accumulated inverse transform matrix, i.e. the
    /// transformation matrix from global space to the local space 
    /// at the current traversal position. Note that this field will not stay
    /// constant for a constant scene graph if the DEF/USE feature is used for
    /// instances of this node.
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> Unit matrix \n
    auto_ptr< SFMatrix4f > accumulatedInverse;
    
    /// This is the same as the bound field, but with the transformation 
    /// applied to the bound.
    /// 
    /// <b>Access type:</b> outputOnly \n
    /// 
    /// \dotfile MatrixTransform_transformedBound.dot
    auto_ptr< SFTransformedBound > transformedBound;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  };
}

#endif
