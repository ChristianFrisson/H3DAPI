//////////////////////////////////////////////////////////////////////////////
//    Copyright 2004-2013, SenseGraphics AB
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
/// \file TransformInfo.cpp
/// \brief CPP file for TransformInfo, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/TransformInfo.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase TransformInfo::database( 
                        "TransformInfo", 
                        &(newInstance<TransformInfo>), 
                        typeid( TransformInfo ),
                        &X3DChildNode::database );

namespace TransformInfoInternals {
  FIELDDB_ELEMENT( TransformInfo, accForwardMatrix, OUTPUT_ONLY );
  FIELDDB_ELEMENT( TransformInfo, accInverseMatrix, OUTPUT_ONLY );
  FIELDDB_ELEMENT( TransformInfo, outputGLMatrices, INPUT_OUTPUT );
  FIELDDB_ELEMENT( TransformInfo, glModelViewMatrix, OUTPUT_ONLY );
  FIELDDB_ELEMENT( TransformInfo, glModelViewMatrixInverse, OUTPUT_ONLY );
  FIELDDB_ELEMENT( TransformInfo, glProjectionMatrix, OUTPUT_ONLY );
  FIELDDB_ELEMENT( TransformInfo, glProjectionMatrixInverse, OUTPUT_ONLY );
}

TransformInfo::TransformInfo( Inst< SFNode     > _metadata,
                              Inst< SFMatrix4f > _accForwardMatrix,
                              Inst< SFMatrix4f > _accInverseMatrix,
                              Inst< DisplayList > _displayList,
                              Inst< SFBool      > _outputGLMatrices,
                              Inst< SFMatrix4f  > _glModelViewMatrix,
                              Inst< Matrix4fInverse > _glModelViewMatrixInverse,
                              Inst< SFMatrix4f  > _glProjectionMatrix,
                              Inst< Matrix4fInverse > _glProjectionMatrixInverse ) :
  X3DChildNode( _metadata ),
  H3DDisplayListObject( _displayList ),
  accForwardMatrix( _accForwardMatrix ),
  accInverseMatrix( _accInverseMatrix ),
  outputGLMatrices( _outputGLMatrices ),
  glModelViewMatrix( _glModelViewMatrix ),
  glModelViewMatrixInverse( _glModelViewMatrixInverse ),
  glProjectionMatrix( _glProjectionMatrix ),
  glProjectionMatrixInverse( _glProjectionMatrixInverse ) {

  type_name = "TransformInfo";
  database.initFields( this );

  displayList->setOwner( this );

  outputGLMatrices->setValue( false );

  glModelViewMatrix->route( glModelViewMatrixInverse, id );
  glProjectionMatrix->route( glProjectionMatrixInverse, id );

  outputGLMatrices->route( displayList );
  glModelViewMatrix->route( displayList );
  glProjectionMatrix->route( displayList );
}

void TransformInfo::traverseSG( TraverseInfo &ti ) {
  X3DChildNode::traverseSG( ti );

  const Matrix4f &acc_frw = ti.getAccForwardMatrix();
  const Matrix4f &acc_inv = ti.getAccInverseMatrix();

  if( acc_frw != accForwardMatrix->getValue() )
    accForwardMatrix->setValue( acc_frw, id );

  if( acc_inv != accInverseMatrix->getValue() )
    accInverseMatrix->setValue( acc_inv, id );
}

void TransformInfo::render() {
  if( outputGLMatrices->getValue() ) {
    displayList->setCacheMode( DisplayList::OFF );
    GLfloat mv[16], p[16];
    glGetFloatv( GL_MODELVIEW_MATRIX, mv );
    glGetFloatv( GL_PROJECTION_MATRIX, p );

    Matrix4f model_view( mv[0], mv[4], mv[8], mv[12],
                         mv[1], mv[5], mv[9], mv[13],
                         mv[2], mv[6], mv[10], mv[14],
                         mv[3], mv[7], mv[11], mv[15] );

    Matrix4f projection( p[0], p[4], p[8], p[12],
                         p[1], p[5], p[9], p[13],
                         p[2], p[6], p[10], p[14],
                         p[3], p[7], p[11], p[15] );

    if( glModelViewMatrix->getValue() != model_view ) {
      glModelViewMatrix->setValue( model_view, id );
      glModelViewMatrixInverse->setValue( model_view.inverse(), id );
    }

    if( glProjectionMatrix->getValue() != projection ) {
      glProjectionMatrix->setValue( projection, id );
      glProjectionMatrixInverse->setValue( projection.inverse(), id );
    }
  } else {
    displayList->setCacheMode( DisplayList::ON );
  }
}
