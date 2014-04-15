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
                                 Inst< AddChildren        > _addChildren,
                                 Inst< RemoveChildren     > _removeChildren,
                                 Inst< MFChild            > _children,
                                 Inst< SFNode             > _metadata,
                                 Inst< SFBound            > _bound,
                                 Inst< SFVec3f            > _bboxCenter,
                                 Inst< SFVec3f            > _bboxSize,
                                 Inst< SFTransformedBound > _transformedBound,
                                 Inst< SFMatrix4f         > _matrix,
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

void MatrixTransform::multiplyGLMatrix() {
  const Matrix4f &m = matrix->getValue();
  GLfloat mv[] = { 
    m[0][0], m[1][0], m[2][0], 0,
    m[0][1], m[1][1], m[2][1], 0,
    m[0][2], m[1][2], m[2][2], 0,
    m[0][3], m[1][3], m[2][3], 1 };
  
  glMultMatrixf( mv );
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
  if( accumulatedForward->getValue() != ti.getAccForwardMatrix() )
    accumulatedForward->setValue( ti.getAccForwardMatrix(), id );

  if( accumulatedInverse->getValue() != ti.getAccInverseMatrix() )
    accumulatedInverse->setValue( ti.getAccInverseMatrix(), id );
  X3DGroupingNode::traverseSG( ti );
  ti.popMatrices();
}

bool MatrixTransform::lineIntersect(
                             const Vec3f &from, 
                             const Vec3f &to,    
                             LineIntersectResult &result ) {
  const Matrix4f &the_matrix = matrix->getValue();
  result.pushTransform( the_matrix );
  Matrix4f the_matrix_inverse = the_matrix.inverse();

  Vec3f local_from = the_matrix_inverse * from;
  Vec3f local_to = the_matrix_inverse * to;
  bool intersection = X3DGroupingNode::lineIntersect( local_from,
                                                      local_to,
                                                      result );
  result.popTransform();
  return intersection;
}

void MatrixTransform::closestPoint( const Vec3f &p,
                                    NodeIntersectResult &result ) {
  const Matrix4f &the_matrix = matrix->getValue();
  result.pushTransform( the_matrix );
  Matrix4f the_matrix_inverse = the_matrix.inverse();
  Vec3f local_p = the_matrix_inverse * p;
  X3DGroupingNode::closestPoint( local_p, result );
  result.popTransform();
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
  } else if( bound ) {
    stringstream s;
    s << "Unsupported Bound type " << typeid( *bound ).name();
    throw Exception::H3DAPIException( s.str(), H3D_FULL_LOCATION );
  } else {
    stringstream s;
    s << "Bound is NULL. Congratulations, you found a bug. Please file a bug report.";
    throw Exception::H3DAPIException( s.str(), H3D_FULL_LOCATION );
  }
}

bool MatrixTransform::movingSphereIntersect( H3DFloat radius,
                                             const Vec3f &from, 
                                             const Vec3f &to,
                                             NodeIntersectResult &result ) {
  const Matrix4f &the_matrix = matrix->getValue();
  result.pushTransform( the_matrix );
  Matrix4f matrix_inverse = the_matrix.inverse();
  Vec3f local_from = matrix_inverse * from;
  Vec3f local_to = matrix_inverse * to;
  Vec3f scaling_values = matrix_inverse.getScalePart();
  H3DFloat local_radius = radius *
    max( scaling_values.x, max( scaling_values.y, scaling_values.z ) );
  bool intersection = X3DGroupingNode::movingSphereIntersect(
                        local_radius, local_from, local_to, result );
  result.popTransform();
  return intersection;
}
