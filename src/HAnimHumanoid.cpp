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
/// \file HAnimHumanoid.cpp
/// \brief CPP file for HAnimHumanoid, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/HAnimHumanoid.h>
#include <H3D/H3DRenderStateObject.h>
#include <H3D/MatrixTransform.h>
#include <H3D/X3DPointingDeviceSensorNode.h>
#include <H3D/X3DShapeNode.h>
#include <H3D/ClipPlane.h>
#include <H3D/Coordinate.h>
#include <H3D/Normal.h>

using namespace H3D;

H3DNodeDatabase HAnimHumanoid::database( 
        "HAnimHumanoid", 
        newInstance<HAnimHumanoid>,
        typeid( HAnimHumanoid ),
        &X3DChildNode::database 
        );

namespace HAnimHumanoidInternals {
  FIELDDB_ELEMENT( HAnimHumanoid, bboxCenter, INITIALIZE_ONLY );
  FIELDDB_ELEMENT( HAnimHumanoid, bboxSize, INITIALIZE_ONLY );
  FIELDDB_ELEMENT( HAnimHumanoid, info, INPUT_OUTPUT );
  FIELDDB_ELEMENT( HAnimHumanoid, name, INPUT_OUTPUT );
  FIELDDB_ELEMENT( HAnimHumanoid, version, INPUT_OUTPUT );
  FIELDDB_ELEMENT( HAnimHumanoid, skin, INPUT_OUTPUT );
  FIELDDB_ELEMENT( HAnimHumanoid, joints, INPUT_OUTPUT );
  FIELDDB_ELEMENT( HAnimHumanoid, segments, INPUT_OUTPUT );
  FIELDDB_ELEMENT( HAnimHumanoid, sites, INPUT_OUTPUT );
  FIELDDB_ELEMENT( HAnimHumanoid, skeleton, INPUT_OUTPUT );
  FIELDDB_ELEMENT( HAnimHumanoid, viewpoints, INPUT_OUTPUT );
  FIELDDB_ELEMENT( HAnimHumanoid, skinCoord, INPUT_OUTPUT );
  FIELDDB_ELEMENT( HAnimHumanoid, skinNormal, INPUT_OUTPUT );
  FIELDDB_ELEMENT( HAnimHumanoid, center, INPUT_OUTPUT );
  FIELDDB_ELEMENT( HAnimHumanoid, rotation, INPUT_OUTPUT );
  FIELDDB_ELEMENT( HAnimHumanoid, scale, INPUT_OUTPUT );
  FIELDDB_ELEMENT( HAnimHumanoid, scaleOrientation, INPUT_OUTPUT );
  FIELDDB_ELEMENT( HAnimHumanoid, translation, INPUT_OUTPUT );
}

HAnimHumanoid::HAnimHumanoid(  Inst< SFNode         > _metadata    ,
			       Inst< SFBound        > _bound       ,
			       Inst< SFVec3f        > _bboxCenter  ,
			       Inst< SFVec3f        > _bboxSize    ,
			       Inst< MFString       > _info        ,
			       Inst< SFString       > _name        ,
			       Inst< SFString       > _version     ,
			       Inst< MFChild        > _skin        ,
			       Inst< MFJoint        > _joints      ,
			       Inst< MFSegment      > _segments    ,
			       Inst< MFSite         > _sites       ,
			       Inst< MFSkeletonNode > _skeleton    ,
			       Inst< MFSite         > _viewpoints  ,
			       Inst< SFCoordinateNode > _skinCoord ,
			       Inst< SFNormalNode     > _skinNormal,
			       Inst< SFVec3f        > _center      ,
			       Inst< SFRotation     > _rotation    ,
			       Inst< SFVec3f        > _scale       ,
			       Inst< SFRotation     > _scaleOrientation,
			       Inst< SFVec3f        > _translation  ) :
  X3DChildNode( _metadata ),
  X3DBoundedObject( _bound, _bboxCenter, _bboxSize ),
  info( _info ),
  name( _name ),
  version( _version ),
  skin( _skin ),
  joints( _joints ),
  segments( _segments ),
  sites( _sites ),
  skeleton( _skeleton ),
  viewpoints( _viewpoints ),
  skinCoord( _skinCoord ),
  skinNormal( _skinNormal ),
  center( _center ),
  rotation( _rotation ),
  scale( _scale ),
  scaleOrientation( _scaleOrientation ),
  translation( _translation ),
  use_union_bound( false ),
  root_transform( NULL ) {

  type_name = "HAnimHumanoid";
  database.initFields( this );

  displayList->setOwner( this );
  bound->setOwner( this );

  bound->setValue( new EmptyBound );

  center->setValue( Vec3f( 0, 0, 0 ) );
  rotation->setValue( Rotation( 0, 0, 1, 0 ) );
  scale->setValue( Vec3f( 1, 1, 1 ) );
  scaleOrientation->setValue( Rotation( 0, 0, 1, 0 ) );
  translation->setValue( Vec3f( 0, 0, 0 ) );
 }

void HAnimHumanoid::initialize() {
  Transform * t = new Transform;

  // set up all routes
  t->bboxCenter->setValue( bboxCenter->getValue() );
  t->bboxSize->setValue( bboxSize->getValue() );

  skin->route( t->children );
  center->route( t->center );
  rotation->route( t->rotation );
  scale->route( t->scale );
  scaleOrientation->route( t->scaleOrientation );
  translation->route( t->translation );

  t->displayList->route( displayList );

  // now the root_transform will be initialized with the bboxCenter and
  // bboxSize from this node.
  root_transform.reset( t );
}

void HAnimHumanoid::render()     { 

  X3DChildNode::render();
  root_transform->displayList->callList();
};

void HAnimHumanoid::traverseSG( TraverseInfo &ti ) {

  // TODO:
  Coordinate *coord = dynamic_cast< Coordinate * >( skinCoord->getValue() );
  if( coord && points_single.empty() ) {
    points_single = coord->point->getValue();
  }

  Normal *normal = dynamic_cast< Normal * >( skinNormal->getValue() );
  if( normal && normals_single.empty() ) {
    normals_single = normal->vector->getValue();
  }
  
  X3DChildNode::traverseSG( ti );
  root_transform->traverseSG( ti );

  const NodeVector &skel = skeleton->getValue();
  for( unsigned int i = 0; i < skel.size(); i++ ) {
    HAnimJoint *joint = dynamic_cast< HAnimJoint* >( skel[i]);
    if( joint ) joint->traverseSG( ti );
  }

  const NodeVector &jts = joints->getValue();
  vector< Vec3f > modified_points = points_single;
  vector< Vec3f > modified_normals = normals_single;


  unsigned int p_size = points_single.size();
  unsigned int n_size = normals_single.size();

  
  const Matrix4f &global_to_humanoid = ti.getAccInverseMatrix();
  for( unsigned int i = 0; i < jts.size(); i++ ) {
    HAnimJoint *joint = static_cast< HAnimJoint* >( jts[i]);
    if( joint ) {
      const vector<int> &indices = joint->skinCoordIndex->getValue();
      const Matrix4f &joint_to_global = joint->accumulatedForward->getValue();
      Matrix4f joint_to_humanoid = global_to_humanoid * joint_to_global;
      for( unsigned int j = 0; j < indices.size(); j++ ) {
	unsigned int index = indices[j];
	if( index < p_size )
	  modified_points[index] = joint_to_humanoid * points_single[index];
	if( index < n_size ) 
	  modified_normals[index] = joint_to_humanoid.getRotationPart() * normals_single[index];
      }
    }
  }

  if(coord) {
    coord->point->swap( modified_points );
  }
  
  if( normal ) {
    normal->vector->swap( modified_normals );
  }
}

bool HAnimHumanoid::lineIntersect(
                  const Vec3f &from, 
                  const Vec3f &to,    
                  LineIntersectResult &result ) {
  return root_transform->lineIntersect( from, to, result );
}

void HAnimHumanoid::closestPoint( const Vec3f &p,
				  NodeIntersectResult &result ) {
  root_transform->closestPoint( p, result );
}

bool HAnimHumanoid::movingSphereIntersect( H3DFloat radius,
					   const Vec3f &from, 
					   const Vec3f &to,
					   NodeIntersectResult &result ) {
  return root_transform->movingSphereIntersect( radius,
						from,
						to, 
						result );
}


