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
/// \file MatrixTransform.cpp
/// \brief CPP file for MatrixTransform, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/MatrixTransform.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase MatrixTransform::database( 
                                          "MatrixTransform", 
                                          &(newInstance<MatrixTransform>), 
                                          typeid( MatrixTransform ),
                                          &X3DGroupingNode::database );

namespace MatrixTransformInternals {
  FIELDDB_ELEMENT( MatrixTransform, matrix, INPUT_OUTPUT );
  FIELDDB_ELEMENT( MatrixTransform, accumulatedForward, OUTPUT_ONLY );
  FIELDDB_ELEMENT( MatrixTransform, accumulatedInverse, OUTPUT_ONLY );
}


MatrixTransform::MatrixTransform( 
                                 Inst< MFChild >  _addChildren,
                                 Inst< MFChild >  _removeChildren,
                                 Inst< MFChild >  _children,
                                 Inst< SFNode  >  _metadata,
                                 Inst< SFBound > _bound,
                                 Inst< SFVec3f >  _bboxCenter,
                                 Inst< SFVec3f >  _bboxSize,
                                 Inst< SFTransformedBound > _transformedBound,
                                 Inst< SFMatrix4f > _matrix,
                                 Inst< SFMatrix4f > _accumulatedForward,
                                 Inst< SFMatrix4f > _accumulatedInverse
                                 ) :
  X3DGroupingNode( _addChildren, _removeChildren, 
                   _children, _metadata, 
                   _bound, _bboxCenter, _bboxSize ),
  matrix( _matrix ),
  accumulatedForward( _accumulatedForward ),
  accumulatedInverse( _accumulatedInverse ),
  transformedBound( _transformedBound ) {
    
  transformedBound->owner = this;
  matrix->setValue( Matrix4f() ); 
  type_name = "MatrixTransform";
  database.initFields( this );

  transformedBound->setName( "transformedBound" );
  transformedBound->setOwner( this );
  transformedBound->setAccessType( Field::OUTPUT_ONLY );

  transformedBound->setValue( new InfiniteBound, id );

  matrix->route( displayList );
  matrix->route( transformedBound, id );
  bound->route( transformedBound, id );
}

void MatrixTransform::render() { 
  GLboolean norm= glIsEnabled( GL_NORMALIZE );
  if ( !norm ) 
    glEnable( GL_NORMALIZE );
  
  GLint front_face;

  const Matrix4f &m = matrix->getValue();
  GLfloat mv[] = { 
    m[0][0], m[1][0], m[2][0], 0,
    m[0][1], m[1][1], m[2][1], 0,
    m[0][2], m[1][2], m[2][2], 0,
    m[0][3], m[1][3], m[2][3], 1 };

  glMatrixMode( GL_MODELVIEW );
  glPushMatrix();
  glMultMatrixf( mv );

  Matrix3f m3 = m.getScaleRotationPart();
  bool negative_scaling = 
    ( ( m3.getRow( 0 ) % m3.getRow( 1 ) ) * m3.getRow(2) ) < 0;
  if( negative_scaling ) {
    // scaling is negative so we have to change which side is front face
    // in order for the normals to be correct.
    glGetIntegerv( GL_FRONT_FACE, &front_face );
    if( front_face == GL_CW ) {
      glFrontFace( GL_CCW );
    } else {
      glFrontFace( GL_CW );
    }
  }

  X3DGroupingNode::render();     
  
  // restore previous values
  if( negative_scaling )
    glFrontFace( front_face );
  if ( !norm ) 
    glDisable( GL_NORMALIZE );

  glMatrixMode( GL_MODELVIEW );
  glPopMatrix();
};

/// Traverse the scenegraph. 
void MatrixTransform::traverseSG( TraverseInfo &ti ) {
  ti.pushMatrices( matrix->getValue(),
                   matrix->getValue().inverse() );

  // set accumulated forward and inverse matrices
  accumulatedForward->setValue( ti.getAccForwardMatrix(), id );
  accumulatedInverse->setValue( ti.getAccInverseMatrix(), id );
  X3DGroupingNode::traverseSG( ti );
  ti.popMatrices();
}

bool MatrixTransform::lineIntersect(
                             const Vec3f &from, 
                             const Vec3f &to,    
                             LineIntersectResult &result ) {
  result.pushTransform( matrix->getValue() );
  Matrix4f theMatrixInverse = result.getCurrentTransform().inverse();

  Vec3f local_from = theMatrixInverse * from;
  Vec3f local_to = theMatrixInverse * to;
  bool intersection = X3DGroupingNode::lineIntersect( local_from,
                                                      local_to,
                                                      result );
  result.popTransform();
  return intersection;
}

void MatrixTransform::closestPoint(
                  const Vec3f &p,
                  vector< Vec3f > &closest_point,
                  vector< Vec3f > &normal,
                  vector< Vec3f > &tex_coord ) {
  Matrix4f theMatrix = matrix->getValue();
  Matrix4f theMatrixInverse = theMatrix.inverse();
  Vec3f local_p = theMatrixInverse * p;
  H3DInt32 sizeBefore = closest_point.size();
  X3DGroupingNode::closestPoint( local_p, closest_point, normal, tex_coord );
  for( unsigned int i = sizeBefore; i < closest_point.size(); i++ ) {
    closest_point[i] = theMatrix * closest_point[i];
    normal[i] = theMatrix.getRotationPart() * normal[i];
  }
}


void MatrixTransform::SFTransformedBound::update() {
  Bound *bound = static_cast< SFBound * >( routes_in[1] )->getValue();
  BoxBound *box_bound = dynamic_cast< BoxBound * >( bound );
  if( box_bound ) {
    value = new TransformedBoxBound;
  }else if( dynamic_cast< InfiniteBound * >( bound ) ) {
    value = new InfiniteBound;
  } else if( dynamic_cast< EmptyBound * >( bound ) ) {
    value = new EmptyBound;
  } else {
    stringstream s;
    s << "Unsupported Bound type " << typeid( *bound ).name();
    throw Exception::H3DAPIException( s.str(), H3D_FULL_LOCATION );
  }
}

bool MatrixTransform::movingSphereIntersect( H3DFloat radius,
                                             const Vec3f &from, 
                                             const Vec3f &to ) {
  Matrix4f theMatrix = matrix->getValue();
  Matrix4f theMatrixInverse = theMatrix.inverse();
  Vec3f local_from = theMatrixInverse * from;
  Vec3f local_to = theMatrixInverse * to;
  Vec3f scaling_values = theMatrixInverse.getScalePart();
  H3DFloat scale_value = scaling_values.x;
  if( scale_value < scaling_values.y )
    scale_value = scaling_values.y;
  if( scale_value < scaling_values.z )
    scale_value = scaling_values.z;
  H3DFloat local_radius = radius * scale_value;
  return X3DGroupingNode::movingSphereIntersect( 
    local_radius, local_from, local_to );
}
