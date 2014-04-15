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
/// \file GeometryGroup.h
/// \brief Header file for GeometryGroup, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __GEOMETRYGROUP_H__
#define __GEOMETRYGROUP_H__

#include <H3D/X3DGeometryNode.h>
#include <H3D/X3DBoundedObject.h>
#include <H3D/DependentNodeFields.h>

namespace H3D {

  /// \ingroup X3DNodes
  /// \class GeometryGroup
  /// \brief The GeometryGroup node is treated by the scenegraph as one geometry
  /// but it can contain several geometries.
  ///
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/GeometryGroup.x3d">GeometryGroup.x3d</a>
  ///     ( <a href="examples/GeometryGroup.x3d.html">Source</a> )
  ///
  /// \par Internal routes:
  /// \dotfile GeometryGroup.dot
  class H3DAPI_API GeometryGroup : public X3DGeometryNode {
  public:

    class H3DAPI_API MFGeometry :
      public DependentMFNode< X3DGeometryNode,
        FieldRef< H3DDisplayListObject,
                  H3DDisplayListObject::DisplayList,
                  &H3DDisplayListObject::displayList >,
                              true >  {
      public:
      /// The base class of MFChild.
      typedef DependentMFNode< 
        X3DGeometryNode,
        FieldRef< H3DDisplayListObject,
                  H3DDisplayListObject::DisplayList,
                  &H3DDisplayListObject::displayList >,
        true >  MFGeometryBase;

      /// Destructor. Clears the MFNode in order to get the correct onRemove 
      /// function to be called upon destruction. Must be done in all subclasses
      /// that overrides onRemove ().
      ~MFGeometry() {
        clear();
      }

      /// Set up the routes we want to maintain.
      virtual void onAdd( Node *n );

      /// Remove the routes we want to maintain.
      virtual void onRemove( Node *n );

    private:
      friend class X3DGeometryNode;
    };
    
    /// SFBound is specialized to update from the SFBound fields 
    /// routed to it. The resulting Bound object is the union of 
    /// the Bound objects routed to it. If the bboxSize of the 
    /// X3DGrouping node containing the SFBound field is ( -1, -1, -1 )
    /// bound fields of all Nodes in the children field of the containing
    /// Node that are instances of X3DBoundedObject are routed to it.
    /// Otherwise the bound will be a BoxBound with center and
    /// radius specified with the bboxCenter and bboxSize fields.
    ///
    class H3DAPI_API SFBound: 
      public TypedField< X3DBoundedObject::SFBound,
                         void,
                         AnyNumber< X3DBoundedObject::SFBound > > {
      /// The SFBound is updated to a bound that is the union of the 
      /// the Bound objects routed to it.
      virtual void update();
    };

    /// Specialized field class which grabs the first true value
    /// for each index for any of the contained geometries. The geometry which
    /// is responsible for the true value is then used for the contactPoint,
    /// contactNormal and force fields. This is to make sure that the contact
    /// values for each haptics device is consistent and belongs to one contact
    /// point.
    class H3DAPI_API IsTouched : public MFBool {
    protected:
      virtual void update();
    };

    /// Specialized field class which gets one value from each routed field
    /// and stores that at the corresponding index in its own value. The index to use
    /// is the index order of the routes in.
    class H3DAPI_API MFVec3fPerIndex :
      public TypedField< MFVec3f, MFVec3f, AnyNumber< MFVec3f > > {
    public:
      void unrouteAllFrom();
    protected:
      virtual void update();
    };

    /// Constructor.
    GeometryGroup( Inst< SFNode > _metadata = 0,
                   Inst< SFBound > _bound = 0,
                   Inst< DisplayList > _displayList = 0,
                   Inst< IsTouched   > _isTouched = 0,
                   Inst< MFVec3fPerIndex > _force = 0,
                   Inst< MFVec3fPerIndex > _contactPoint = 0,
                   Inst< MFVec3fPerIndex > _contactNormal = 0,
                   Inst< MFVec3fPerIndex > _contactTexCoord = 0,
                   Inst< SFBoundTree > _boundTree = 0,
                   Inst< MFGeometry > _geometry = 0 );

    /// The number of triangles renderered in this geometry.
    virtual int nrTriangles();

    /// Renders the GeometryGroup with OpenGL.
    virtual void render();

    /// Traverse the scenegraph. Or rather, all its child geometries.
    virtual void traverseSG( TraverseInfo &ti );

    /// Detect intersection between a line segment and the sphere.
    /// \param from The start of the line segment.
    /// \param to The end of the line segment.
    /// \param result Contains info about the closest intersection for every
    /// object that intersects the line.
    /// \returns true if intersected, false otherwise.
    virtual bool lineIntersect( 
      const Vec3f &from,
      const Vec3f &to,    
      LineIntersectResult &result );

    /// Find closest point on the sphere to point p.
    /// \param p The point to find the closest point to.
    /// \param result A struct containing various results of closest
    /// points such as which geometries the closest points where
    /// detected on.
    virtual void closestPoint( const Vec3f &p,
                               NodeIntersectResult &result );

    /// Detect collision between a moving sphere and this sphere.
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


    /// Returns true if this geometry supports the automatic generation
    /// of tangents and binormals as FloatVertexAttribues(needed by
    /// e.g. PhongShader.
    virtual bool supportsTangentAttributes();

    /// Function overridden from HAPIGLShape. Just call the 
    /// displayList->callList of contained geometries.
    virtual void glRender();

    /// The geometry nodes.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// 
    /// \dotfile GeometryGroup_geometry.dot
    auto_ptr< MFGeometry > geometry;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  };
}

#endif
