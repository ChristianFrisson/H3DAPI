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
//
/// \file Node.h
/// \brief Node base class
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __NODE_H__
#define __NODE_H__

#include <H3D/Instantiate.h>
#include <H3D/H3DNodeDatabase.h>
#include <H3D/TraverseInfo.h>
#include <H3DUtil/RefCountedClass.h>
#include <H3DUtil/Console.h>

//#include "FieldTemplates.h"

using namespace std;

namespace H3D {

  /// \defgroup Nodes Scene-graph nodes.
  ///
  /// \class Node
  /// \brief Node is the base class for all classes that can be part of
  /// the H3D scene-graph.
  class H3DAPI_API Node: public RefCountedClass {
  public:
 
    typedef HAPI::Collision::IntersectionInfo IntersectionInfo;

    /// Constructor.
    Node();
    
    /// Destructor.
    virtual ~Node() {};

    template< class N >
    static Node *newInstance() { return new N; };

    /// the render() function is used for the depth-first rendering traversal 
    /// of the scene-graph. 
    virtual void render()     { };

		/// traverseSG is called onve per scenegraph loop on the scene in order to
    /// traverse the scenegraph. During this traversal things can be updated
    /// in the node and HapticObject instances to be rendered should be added 
    /// to the TraverseInfo object.
    /// \param ti The TraverseInfo object containing information about the
    /// traversal.
    /// 
    virtual void traverseSG( TraverseInfo &ti ) {}


    struct H3DAPI_API LineIntersectResult {
      // Constructor.
      LineIntersectResult( bool _get_transforms = false,
                           bool _override_no_collision = false,
                           bool _use_pt_device_affect = false,
                           void *_user_data = 0 ) :
        get_transforms( _get_transforms ),
        override_no_collision( _override_no_collision ),
        use_pt_device_affect( _use_pt_device_affect ),
        user_data( _user_data ),
        pt_device_affect( false ) {
          current_matrix.push( Matrix4f() );
        };

      /// A vector of HAPI::IntersectionInfo that stores result of intersection
      /// such as point and normal.
      vector< IntersectionInfo > result;
      
      /// A vector of pairs of pointer and index to
      /// differ between different places in the scene graph for the same Node.
      /// This can happen due to the DEF/USE feature of X3D.
      vector< pair< Node *, H3DInt32 > > theNodes;

      /// Flag used to know if lineintersect should be called for the children
      /// in the Collision Node regardless if it is enabled or not.
      bool override_no_collision;

      /// Flag used to know if lineintersect should bother with keeping track
      /// of X3DPointingDeviceSensorNodes.
      bool use_pt_device_affect;

      /// Flag telling a node if it is affected by a
      /// X3DPointingDeviceSensorNode. Needed to allow for correct behaviour
      /// when using the DEF/USE feature of X3D. Should only be set by the
      /// lineIntersect function.
      bool pt_device_affect;

      /// Optional user_data in case someone want to do add some extra feature
      /// to lineIntersect and one or several custom made nodes.
      void * user_data;

      inline void addTransform() {
        if( get_transforms ) {
          geometry_transforms.push_back( getCurrentTransform() );
        }
      }

      inline const vector< Matrix4f > &getGeometryTransforms() {
        return geometry_transforms;
      }

      inline const Matrix4f &getCurrentTransform() {
        return current_matrix.top();
      }

      inline void pushTransform( const Matrix4f &matrix ) {
        current_matrix.push( current_matrix.top() * matrix );
      }

      inline void popTransform() {
        current_matrix.pop();
      }

      inline void transformResult() {
        for( unsigned int i = 0; i < result.size(); i++ ) {
          result[i].point = geometry_transforms[i] * result[i].point;
          result[i].normal = geometry_transforms[i].getRotationPart() *
                             result[i].normal;
        }
      }

    protected:
      bool get_transforms;
      /// A vector of matrices from the local
      /// coordinate space to global space for each node that the
      /// line intersects.
      vector< Matrix4f > geometry_transforms;

      /// The top of the stack is the current matrix that transforms from the
      /// local coordinate space where this Node resides in the scenegraph to
      /// global space.
      stack< Matrix4f > current_matrix;
    };

    /// Detect intersection between a line segment and the Node.
    /// \param from The start of the line segment.
    /// \param to The end of the line segment.
    /// \param result Contains info about the closest intersection for every
    /// object that intersects the line.
    /// \returns true if intersected, false otherwise.
    virtual bool lineIntersect( 
      const Vec3f &from, 
      const Vec3f &to,    
      LineIntersectResult &result ) {
      return false;
    }

    /// Find closest point on Node to p.
    /// \param p The point to find the closest point to.
    /// \param closest_point Return parameter for each closest point
    /// \param normal Return parameter for normal at each closest point.
    /// \param tex_coord Return paramater for each texture coordinate at
    /// closest point.
    virtual void closestPoint( const Vec3f &p,
                               vector< Vec3f > &closest_point,
                               vector< Vec3f > &normal,
                               vector< Vec3f > &tex_coord ){}

    /// Detect collision between a moving sphere and the Node.
    /// Only nodes to which collision is possible will return true
    /// \param The radius of the sphere
    /// \param from The start position of the sphere
    /// \param to The end position of the sphere.
    /// \returns true if intersected, false otherwise.
    virtual bool movingSphereIntersect( H3DFloat radius,
                                        const Vec3f &from, 
                                        const Vec3f &to ){
    return false;
    }

    /// Resets flags used to get correct behaviour for lineIntersect
    /// when using the DEF/USE feature and X3DPointingDeviceSensorNode.
    /// Does nothing for most nodes.
    virtual void resetNodeDefUseId() {};

    /// Increase an integer used to get correct behaviour for lineIntersect
    /// when using the DEF/USE feature and X3DPointingDeviceSensorNode.
    /// Does nothing for most nodes.
    /// \param pt_device_affect A flag which is true if the node is affected
    /// by a X3DPointingDeviceSensorNode.
    virtual void incrNodeDefUseId( bool pt_device_affect ){};
    
    /// Returns the default xml containerField attribute value.
    /// For this node it is "children".
    ///
    virtual string defaultXMLContainerField() {
      return "children";
    }
    
    /// return a pointer to the field specified by name within this instance 
    virtual Field *getField( const string &name );
    
    friend class Field;
    template <class Type>
    friend class SField;
    template <class Type>
    friend class MField;
    template<class RefClass, class BaseField>
    friend class RefCountSField;

  protected:
    static H3DNodeDatabase database;
    int id;
    static int nr_nodes_created;
  };

};

#endif
