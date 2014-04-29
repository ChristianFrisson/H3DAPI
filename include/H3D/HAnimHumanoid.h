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
/// \file HAnimHumanoid.h
/// \brief Header file for HAnimHumanoid, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __HANIMHUMANOID_H__
#define __HANIMHUMANOOID_H__

#include <H3D/X3DChildNode.h>
#include <H3D/X3DBoundedObject.h>
#include <H3D/X3DCoordinateNode.h>
#include <H3D/X3DNormalNode.h>
#include <H3D/DependentNodeFields.h>
#include <H3D/H3DDisplayListObject.h>
#include <H3D/Transform.h>
#include <H3D/SFRotation.h>
#include <H3D/MFString.h>
#include <H3D/HAnimJoint.h>
#include <H3D/HAnimSite.h>

namespace H3D {

  /// \ingroup X3DNodes
  /// \class HAnimHumanoid
  /// \brief The HAnimHumanoid node is a container for the entire humanoid.
  ///
  /// In addition it is used to store human-readable data such
  /// as author and copyright information, as well as to store references to
  /// the HAnimJoint, HAnimSegment, and HAnimSite nodes in addition to serving
  /// as a container for the entire humanoid. Thus, it serves as a central node
  /// for moving the humanoid through its environment.
  /// 
  /// \note This nodes functionality is not fully implemented, only joints are used.
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/BoxMan.x3d">BoxMan.x3d</a>
  ///     ( <a href="examples/BoxMan.x3d.html">Source</a> )
  ///
  /// \par Internal routes:
  /// \dotfile HAnimHumanoid.dot
  class H3DAPI_API HAnimHumanoid : 
    public X3DChildNode,
    public X3DBoundedObject,
    public H3DDisplayListObject {
  public:

    //TODO

    class MFJoint : public MFNode {
      virtual void onAdd( Node * );
      virtual void onRemove( Node * );
    };

    typedef MFNode MFSegment;
    typedef MFNode MFSite;

    class MFSkeletonNode: public MFNode {
      virtual void onAdd( Node * );
    };

    typedef MFNode MFChild;
      
    /// The SFCoordinateNode is dependent on the propertyChanged field of the 
    /// contained X3DCoordinateNode.
    typedef DependentSFNode< X3DCoordinateNode,
                             FieldRef< X3DGeometricPropertyNode,
                                       Field,
                                       &X3DCoordinateNode::propertyChanged > > 
    SFCoordinateNode;
    
   /// The SFNormalNode is dependent on the propertyChanged field of the 
    /// contained X3DNormalNode.
    typedef DependentSFNode< X3DNormalNode,
                             FieldRef< X3DGeometricPropertyNode,
                                       Field,
                                       &X3DNormalNode::propertyChanged > > 
    SFNormalNode;
    
    /// Constructor.
    HAnimHumanoid( Inst< SFNode         > _metadata       = 0,
                   Inst< SFBound        > _bound          = 0,
                   Inst< SFVec3f        > _bboxCenter     = 0,
                   Inst< SFVec3f        > _bboxSize       = 0,
                   Inst< MFString       > _info           = 0,
                   Inst< SFString       > _name           = 0,
                   Inst< SFString       > _version        = 0,
                   Inst< MFChild        > _skin           = 0,
                   Inst< MFJoint        > _joints         = 0,
                   Inst< MFSegment      > _segments       = 0,
                   Inst< MFSite         > _sites          = 0,
                   Inst< MFSkeletonNode > _skeleton       = 0,
                   Inst< MFSite         > _viewpoints     = 0,
                   Inst< SFCoordinateNode > _skinCoord    = 0,
                   Inst< SFNormalNode     > _skinNormal   = 0,
                   Inst< SFVec3f        > _center         = 0,
                   Inst< SFRotation     > _rotation       = 0,
                   Inst< SFVec3f        > _scale          = 0,
                   Inst< SFRotation     > _scaleOrientation = 0,
                   Inst< SFVec3f        > _translation    = 0,
                   Inst< SFString       > _renderMode     = 0
                   );

    /// Initializes the root_transform. 
    virtual void initialize();

    /// Render the skin.
    virtual void render();

    /// Traverse the scenegraph. traverseSG() is called in all skin nodes.
    virtual void traverseSG( TraverseInfo &ti );

    /// Detect intersection between a line segment and a Node.
    /// Calls lineIntersect for all skin
    /// \param from The start of the line segment.
    /// \param to The end of the line segment.
    /// \param result Contains info about the closest intersection for every
    /// object that intersects the line.
    /// \returns true if intersected, false otherwise.
    virtual bool lineIntersect(
      const Vec3f &from, 
      const Vec3f &to,    
      LineIntersectResult &result );

    /// Find closest point on Node to p. Calls closestPoint for
    /// all skin
    /// \param p The point to find the closest point to.
    /// \param result A struct containing various results of closest
    /// points such as which geometries the closest points where
    /// detected on.
    virtual void closestPoint( const Vec3f &p,
                               NodeIntersectResult &result );

    /// Detect collision between a moving sphere and the Node.
    /// Calls movingSphereIntersect for all skin
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
  
    /// if true a route will be set up between the bound field of the
    /// nodes in skin and the bound field of the grouping node. 
    bool use_union_bound;

    /// The info field consists of a sequence of strings, each of which is
    /// of the form "tag=value". The following tags are currently defined 
    /// by the H-Anim Specification: 
    /// - authorName 
    /// - authorEmail 
    /// - copyright 
    /// - creationDate 
    /// - usageRestrictions 
    /// - humanoidVersion 
    /// - age 
    /// - gender 
    /// - height 
    /// - weight
    ///
    /// The gender tag typically has a value of "female", "male" or "neuter".
    /// The humanoidVersion tag refers to the version of the humanoid being
    /// used, in order to track revisions to the data. It is not the same as
    /// the version field of the Humanoid object, which refers to the version
    /// of the H-Anim specification that was used when building the humanoid.
    /// Additional tag=value pairs can be included as needed for specific
    /// applications and can be proposed as additions to future versions of
    /// this International Standard.  
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> [] \n
    /// 
    /// \dotfile HAnimHumanoid_info.dot  
    auto_ptr< MFString > info;

    /// The name field stores the name of the humanoid defined by the 
    /// Humanoid object.
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> "" \n
    /// 
    /// \dotfile HAnimHumanoid_name.dot
    auto_ptr< SFString > name;

    /// The version field stores the version of the H-Anim International
    /// Standard to which the Humanoid object conforms. E.g. "200x".
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> "" \n
    /// 
    /// \dotfile HAnimHumanoid_version.dot
    auto_ptr< SFString > version;

    /// The skin field contains the nodes that define the graphical 
    /// appearance of the HAnimHumanoid. It should contain one or
    /// more triangle set definitions that utilize the point and normal
    /// data that is defined within the skinCoord and skinNormal 
    /// fields.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// 
    /// \dotfile HAnimHumanoid_skin.dot
    auto_ptr< MFChild > skin;
    
    /// The joints field contains a list of references, one for each 
    /// HAnimJoint object defined within the skeleton field hierarchy of
    /// the HAnimHumanoid object. The order in which the joints are listed is
    /// irrelevant since the names of the joints are stored in the HAnimJoint
    /// objects themselves.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// 
    /// \dotfile HAnimHumanoid_joints.dot
    auto_ptr< MFJoint > joints;

    /// The segments field contains a list of references, one for each
    /// HAnimSegment object defined within the skeleton field hierarchy of the
    /// Humanoid object.  The order in which the segments are listed is
    /// irrelevant since the names of the segments are stored in the 
    /// HAnimSegment objects themselves.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// 
    /// \dotfile HAnimHumanoid_segments.dot
    auto_ptr< MFSegment > segments;

    /// The sites field contains a list of references, one for each 
    /// HAnimSite object defined within the skeleton field hierarchy of the
    /// Humanoid object. The order in which the sites are listed is 
    /// irrelevant since the names of the sites are stored in the HAnimSite
    /// objects themselves.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// 
    /// \dotfile HAnimHumanoid_sites.dot
    auto_ptr< MFSite > sites;

    /// The skeleton field contains the HAnimHumanoid root HAnimJoint object.  
    /// The HAnimHumanoid object is considered the parent object of this joint
    /// object and defines a coordinate space for it. Thus, the HAnimHumanoid
    /// object's transformation affects the HAnimJoint object hierarchy
    /// in the skeleton field.  A hierarchy of HAnimJoint objects is 
    /// defined for each H-Anim humanoid figure within the skeleton field
    /// of the HAnimHumanoid object and a hierarchical definition of joints
    /// is present even when the geometry of the humanoid figure is not
    /// defined within the skeleton field. The skeleton field contain can
    /// also contain zero or more HAnimSite objects which allow landmarks to 
    /// be established with respect to the overall humanoid figure, but which
    /// will not be affected by the movement of any of the HAnimJoint objects.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// 
    /// \dotfile HAnimHumanoid_skeleton.dot
    auto_ptr< MFSkeletonNode > skeleton; 

    /// The viewpoints field has a different functionality and behaviour than
    /// the joints, segments and sites fields. The viewpoints field can contain
    /// zero or more HAnimSite object definitions. The HAnimSite objects 
    /// defined within this field are affected by the transformations applied
    /// to the HAnimHumanoid object, but are not affected by any of the 
    /// transformations performed
    /// on the skeletal hierarchy defined within the skeleton field. The 
    /// purpose of these HAnimSite objects is to define locations for virtual 
    /// cameras in the reference frame of the HAnimHumanoid object. HAnimSite
    /// objects
    /// that are defined in this manner are intended to be used as attachment
    /// points from which a certain viewing perspective can be seen (such as 
    /// viewing the face or profile of the human figure).
    ///
    /// <b>Access type:</b> inputOutput \n
    /// 
    /// \dotfile HAnimHumanoid_viewpoints.dot
    auto_ptr< MFSite > viewpoints;

    /// The skinCoord field contains a single sequence of points which are
    /// used by the internal mechanisms of the HAnimHumanoid object to create the
    /// appropriate surface deformations as well as by the indexed face
    /// set definitions within the skin field which do the actual rendering
    /// of the surface geometry.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// 
    /// \dotfile HAnimHumanoid_skinCoord.dot
    auto_ptr< SFCoordinateNode > skinCoord;

    /// The skinNormal field contains the normal data definition which is 
    /// used by the internal mechanisms of the HAnimHumanoid object to create
    /// the appropriate surface deformations as well as the indexed face
    /// set definitions within the skin field, which contains the actual
    /// surface geometry that is rendered.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// 
    /// \dotfile HAnimHumanoid_skinNormal.dot
    auto_ptr< SFNormalNode > skinNormal;

    /// The center field specifies a translation offset from the origin of the
    /// local coordinate system (0,0,0). 
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> Vec3f( 0, 0, 0 ) \n
    /// 
    /// \dotfile HAnimHumanoid_center.dot
    auto_ptr< SFVec3f > center;

    /// The rotation field specifies a rotation of the coordinate system of
    /// the humanoid figure. 
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> Rotation( 0, 0, 1, 0 ) \n
    /// 
    /// \dotfile HAnimHumanoid_rotation.dot
    auto_ptr< SFRotation > rotation;

    /// The scale field specifies a non-uniform scale of the humanoid figure
    /// coordinate system and the scale values shall be greater than zero.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> Vec3f( 1, 1, 1 ) \n
    /// 
    /// \dotfile HAnimHumanoid_scale.dot
    auto_ptr< SFVec3f > scale;

    /// The scaleOrientation specifies a rotation of the coordinate system of
    /// the humanoid figure before the scale (to specify scales in arbitrary
    /// orientations). The scaleOrientation applies only to the scale operation.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> Rotation( 0, 0, 1, 0 ) \n
    /// 
    /// \dotfile HAnimHumanoid_scaleOrientation.dot
    auto_ptr< SFRotation > scaleOrientation;

    /// The translation field specifies a translation to the coordinate system
    /// of the entire humanoid figure.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> Vec3f( 0, 0, 0 ) \n
    /// 
    /// \dotfile HAnimHumanoid_translation.dot
    auto_ptr< SFVec3f > translation;

    /// The renderMode field defines how the humanoid should be rendered.
    ///
    /// - "SKIN" - the skin in the skin field is renderes
    /// - "JOINTS" - a sphere is rendered for each joint
    /// - "SKELETON" - a sphere is rendered for each joint and a line is
    /// drawn between the connected joints.
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> "SKIN", "JOINTS", "SKELETON" \n
    /// <b>Default value:</b> "SKIN" \n
    /// 
    /// \dotfile HAnimHumanoid_renderMode.dot
    auto_ptr< SFString > renderMode;
    
    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;

    protected:
    
      /// Template function to apply the joint movements and displacements
      /// to original skin coordinates and normals. Since the points can
      /// be both floats or doubles a template is used.
      /// \param orig_points The original points before any deformation.
      /// \param orig_normals The original normals before any deformation.
      /// \param modified_points At input a copy of orig_points, at function
      /// return it will contain the points with the deformations applied.
      /// \param modified_normals  At input a copy of orig_normals, at function
      /// return it will contain the normals with the deformations applied.
      template< class VectorType >
      void updateCoordinates( const VectorType &orig_points,
                              const vector< Vec3f > &orig_normals,
                              VectorType &modified_points,
                              vector< Vec3f > &modified_normals );

      /// Transform node that will contain the skin nodes and which values
      /// will be controlled by the transformation fields in the HAnimHumanoid. 
      AutoRef< Transform > root_transform;

      /// vector for storing the original points before joint and displacer
      /// modification if the original coord data is double precision.
      vector< Vec3d > points_double;

      /// vector for storing the original points before joint and displacer
      /// modification if the original coord data is single precision.
      vector< Vec3f > points_single;

      /// vector for storing the original normals before joint and displacer
      /// modification if the original normal data is single precision.
      vector< Vec3f > normals_single;

      /// The coordinate node that was used as base coordinate in last
      /// traverseSG
      AutoRef< X3DCoordinateNode > current_coordinate;

      /// The normal node that was used as base normals in last
      /// traverseSG
      AutoRef< X3DNormalNode > current_normal;

      /// Field used to know if any values have changed so that the
      /// coordinates have to be updated. All joints accumulatedJointMatrx
      /// and displacers fields are routed here.
      auto_ptr< Field > joint_matrix_changed;
  };
}

#endif
