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
#include <H3D/CoordinateDouble.h>
#include <H3D/Normal.h>
#include <H3D/HAnimDisplacer.h>

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
  FIELDDB_ELEMENT( HAnimHumanoid, renderMode, INPUT_OUTPUT );
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
                               Inst< SFVec3f        > _translation,
                               Inst< SFString       > _renderMode ) :
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
  renderMode( _renderMode ),
  use_union_bound( false ),
  root_transform( NULL ),
  joint_matrix_changed( new Field ) {

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
  renderMode->addValidValue( "SKIN" );
  renderMode->addValidValue( "JOINTS" );
  renderMode->addValidValue( "SKELETON" );
  renderMode->setValue( "SKIN" );

  renderMode->route( displayList );
 }


template< class VectorType >
void HAnimHumanoid::updateCoordinates( const VectorType &orig_points,
                                       const vector< Vec3f > &orig_normals,
                                       VectorType &modified_points,
                                       vector< Vec3f > &modified_normals ) {

  unsigned int p_size = (unsigned int) orig_points.size();
  unsigned int n_size = (unsigned int) orig_normals.size();
  unsigned int max_size = H3DMax( p_size, n_size );
  vector< bool  > point_written( max_size, false ); 

  const NodeVector &jts = joints->getValue();
 
  // do joint movements
  for( unsigned int i = 0; i < jts.size(); ++i ) {
    HAnimJoint *joint = static_cast< HAnimJoint* >( jts[i]);
    if( joint ) {
      const vector<int> &indices = joint->skinCoordIndex->getValue();
      const vector<H3DFloat> &weights = joint->skinCoordWeight->getValue();
      const Matrix4f &joint_to_global = joint->accumulatedForward->getValue();
      Matrix4f joint_to_humanoid = joint->accumulatedJointMatrix->getValue();
      Matrix3f joint_to_humanoid_rot = joint_to_humanoid.getRotationPart();

      for( unsigned int j = 0; j < indices.size(); ++j ) {
        unsigned int index = indices[j];

        // point calculation
        if( index < p_size ) {
          
          typename VectorType::value_type weighted_point = 
            joint_to_humanoid * orig_points[index];
          if( j < weights.size() )
            weighted_point *= weights[j];

          if( point_written[index] ) {
            modified_points[index] += weighted_point;
          } else {
            modified_points[index] = weighted_point;
          }
        }

        // normal calculation
        if( index < n_size ) {
          Vec3f weighted_normal = joint_to_humanoid_rot * orig_normals[index];
          if( j < weights.size() )
            weighted_normal *= weights[j];

          if( point_written[index] ) {
            modified_normals[index] += weighted_normal;
          } else {
            modified_normals[index] = weighted_normal;
          } 
        }

        
        if( index < max_size ) {
          point_written[index] = true;
        }

        
      }
    }
  }

  // do displacer movements
  for( unsigned int i = 0; i < jts.size(); ++i ) {
    HAnimJoint *joint = static_cast< HAnimJoint* >( jts[i]);
    if( joint ) {
      const NodeVector &disp = joint->displacers->getValue();  
      if( disp.size() > 0 ) {
        for( unsigned int i = 0; i < disp.size(); ++i ) {
          HAnimDisplacer *displacer = 
            static_cast< HAnimDisplacer* >( disp[i]);
          if( displacer ) {
            displacer->displaceCoordinates(modified_points, 
                                           joint->accumulatedJointMatrix->getValue() );
          }
        }
      }
    }
  }
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

  root_transform->bound->route( bound );
}

void HAnimHumanoid::render()     { 
  X3DChildNode::render();

  const string &render_type = renderMode->getValue();

  

  if( render_type == "JOINTS" || render_type == "SKELETON" ) {
    HAnimJoint::RenderType type = 
      render_type == "JOINTS" ? HAnimJoint::JOINTS : HAnimJoint::SKELETON;
    glMatrixMode( GL_MODELVIEW );
    glPushMatrix();
    root_transform->multiplyGLMatrix();
    const NodeVector &skel = skeleton->getValue();
    for( unsigned int i = 0; i < skel.size(); ++i ) {
      HAnimJoint *joint = dynamic_cast< HAnimJoint* >( skel[i]);
      if( joint ) joint->renderSkeleton( type );
    }
    glPopMatrix();
  } else {
    // render skinned version
    root_transform->displayList->callList();
    const NodeVector &skel = skeleton->getValue();
    for( unsigned int i = 0; i < skel.size(); ++i ) {
      HAnimJoint *joint = dynamic_cast< HAnimJoint* >( skel[i]);
      if( joint ) joint->displayList->callList();
    }
  }

};

void HAnimHumanoid::traverseSG( TraverseInfo &ti ) {

  X3DCoordinateNode *coord = skinCoord->getValue();

  // if skinCoord contains a coordinate node that has not been used before
  // save its points as base coordinates.
  if( coord != current_coordinate.get() ) {
    current_coordinate.reset( coord );
    if( coord ) {
      if( Coordinate *c = dynamic_cast< Coordinate * >( coord ) ) {
        points_double.clear();
        points_single = c->point->getValue();
      } else if( CoordinateDouble *c = dynamic_cast< CoordinateDouble * >( coord ) ) {
        points_single.clear();
        points_double = c->point->getValue();
      } else {
        Console(4) << "Unsupported X3DCoordinateNode: \"" 
                   << coord->getTypeName() << "\" in HAnimHumanoid." << endl;
        points_single.clear();
        points_double.clear();
      }
    }
  }

  X3DNormalNode *base_normal = skinNormal->getValue();
  Normal *normal = dynamic_cast< Normal * >( base_normal  );

  if( base_normal != current_normal.get() ) {
    current_normal.reset( base_normal );
    if( normal ) {
      normals_single = normal->vector->getValue();
    } else if( base_normal ) {
      Console(4) << "Unsupported X3DNormalNode: \"" 
                   << base_normal->getTypeName() << "\" in HAnimHumanoid." << endl;
      normals_single.clear();
    }
  }
  
  X3DChildNode::traverseSG( ti );
  root_transform->traverseSG( ti );

  const NodeVector &skel = skeleton->getValue();
  const NodeVector &jts = joints->getValue();

  // traverse skeleton 
  for( unsigned int i = 0; i < skel.size(); ++i ) {
    Node *n = skel[i];
    if( n ) n->traverseSG( ti );
  }

  if( !joint_matrix_changed->isUpToDate() ) {
    joint_matrix_changed->upToDate();

    if( CoordinateDouble *c = dynamic_cast< CoordinateDouble * >( coord ) ) {
      vector< Vec3d > modified_points = points_double;
      vector< Vec3f > modified_normals = normals_single;
      updateCoordinates( points_double, normals_single, 
                         modified_points, modified_normals );
      c->point->swap( modified_points );
      if( normal ) normal->vector->swap( modified_normals );
    } else {
      vector< Vec3f > modified_points = points_single;
      vector< Vec3f > modified_normals = normals_single;
      updateCoordinates( points_single, normals_single,
                         modified_points, modified_normals );
      if( Coordinate *c = dynamic_cast< Coordinate * >( coord )) { 
        c->point->swap( modified_points );
      }
      if( normal ) normal->vector->swap( modified_normals );
    }
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


void HAnimHumanoid::MFSkeletonNode::onAdd( Node *n ) {
  HAnimHumanoid *humanoid = static_cast< HAnimHumanoid * >( getOwner() );
  if( n ) {
    if( HAnimJoint *joint = dynamic_cast< HAnimJoint *>( n ) ) {
      
    } else if( dynamic_cast< HAnimSite * >( n ) ) {
      
    } else {
      Console(4) << "Invalid Node type: \"" << n->getTypeName() 
                 << "\" in HAnimHumanoid.skeleton field. Must be HAnimJoint or HAnimSite" << endl;
      return;
    }
  }
  MFNode::onAdd( n );
}


void HAnimHumanoid::MFJoint::onAdd( Node *n ) {
  HAnimHumanoid *humanoid = static_cast< HAnimHumanoid * >( getOwner() );
  if( n ) {
    if( HAnimJoint *joint = dynamic_cast< HAnimJoint *>( n ) ) {
      joint->accumulatedJointMatrix->route( humanoid->joint_matrix_changed );
      joint->displacers->route( humanoid->joint_matrix_changed );
      joint->displayList->route( humanoid->displayList );
    } else if( HAnimSite *site = dynamic_cast< HAnimSite * >( n ) ) {
      site->displayList->route( humanoid->displayList );
    } else {
      Console(4) << "Invalid Node type: \"" << n->getTypeName() 
                 << "\" in HAnimHumanoid.skeleton field. Must be HAnimJoint or HAnimSite" << endl;
      return;
    }
  }
  MFNode::onAdd( n );
}

void HAnimHumanoid::MFJoint::onRemove( Node *n ) {
  MFNode::onRemove( n );
  HAnimHumanoid *humanoid = static_cast< HAnimHumanoid * >( getOwner() );

  if( HAnimJoint *joint = dynamic_cast< HAnimJoint *>( n ) ) {
    joint->accumulatedJointMatrix->unroute( humanoid->joint_matrix_changed );
    joint->displacers->unroute( humanoid->joint_matrix_changed );
    joint->displayList->unroute( humanoid->displayList );
  } else if( HAnimSite *site = dynamic_cast< HAnimSite *>( n ) ) {
    site->displayList->unroute( humanoid->displayList );
  }
}
