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
/// \file FullscreenRectangle.cpp
/// \brief CPP file for FullscreenRectangle, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/FullscreenRectangle.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase FullscreenRectangle::database( "FullscreenRectangle", 
                               &(newInstance<FullscreenRectangle>), 
                               typeid( FullscreenRectangle ),
                               &X3DGeometryNode::database );

namespace FullscreenRectangleInternals {
  FIELDDB_ELEMENT( FullscreenRectangle, zValue, INPUT_OUTPUT );
  FIELDDB_ELEMENT( FullscreenRectangle, screenAligned, INPUT_OUTPUT );
}


FullscreenRectangle::FullscreenRectangle( Inst< SFNode      > _metadata,
                                          Inst< SFBound     > _bound,
                                          Inst< DisplayList > _displayList,
                                          Inst< MFBool      > _isTouched,
                                          Inst< MFVec3f     > _force,
                                          Inst< MFVec3f     > _contactPoint,
                                          Inst< MFVec3f     > _contactNormal,
                                          Inst< SFFloat     > _zValue,
                                          Inst< SFBool      > _screenAligned
                                          ):
  X3DGeometryNode( _metadata, _bound, _displayList, _isTouched,
                   _force, _contactPoint, _contactNormal ),
  zValue    ( _zValue     ),
  screenAligned( _screenAligned ) {

  type_name = "FullscreenRectangle";
  database.initFields( this );

  zValue->setValue( (H3DFloat) -1 );
  screenAligned->setValue( false );
  bound->setValue( new EmptyBound );

  zValue->route( displayList );
  screenAligned->route( displayList );

  // turn off display list since we always need to use the current
  // values of the projection and model view matrix. If cached this
  // is not possible.
  displayList->setCacheMode( H3DDisplayListObject::DisplayList::OFF );
}

void FullscreenRectangle::render() {
  H3DFloat z_value = zValue->getValue();
  H3DFloat epsilon = (H3DFloat) 1e-6;

  // assure that we do not draw outside the view frustum because of 
  // roundoff errors
  if( z_value == -1 ) z_value = z_value + epsilon;
  else if( z_value == 1 ) z_value = z_value - epsilon;

  GLdouble mvmatrix[16], projmatrix[16];
  glGetDoublev( GL_MODELVIEW_MATRIX, mvmatrix );
  glGetDoublev( GL_PROJECTION_MATRIX, projmatrix );

  Matrix4d model_view( mvmatrix[0], mvmatrix[4], mvmatrix[8 ], mvmatrix[12],
                       mvmatrix[1], mvmatrix[5], mvmatrix[9 ], mvmatrix[13],
                       mvmatrix[2], mvmatrix[6], mvmatrix[10], mvmatrix[14],
                       mvmatrix[3], mvmatrix[7], mvmatrix[11], mvmatrix[15] );

  Matrix4d projection( projmatrix[0], projmatrix[4], projmatrix[8 ], projmatrix[12],
                       projmatrix[1], projmatrix[5], projmatrix[9 ], projmatrix[13],
                       projmatrix[2], projmatrix[6], projmatrix[10], projmatrix[14],
                       projmatrix[3], projmatrix[7], projmatrix[11], projmatrix[15] );

  Matrix4d normalized_to_local = (projection * model_view).inverse();

  // we calculate the vertex values of the rectangle in local space instead of
  // setting the projection and model view matrix to unit matrices and specifying
  // vertices in normalized device coordinates. This is so shaders using the node
  // have access to the projection matrix and model view matrix used.
  Vec3d v0, v1, v2, v3;
  Vec3f t0, t1, t2, t3;
  GLint front_face;
  glGetIntegerv( GL_FRONT_FACE, &front_face );
  if( front_face == GL_CW && !screenAligned->getValue() ) {
    // we are in mirrored mode and want the screen to flip.
    v0 = normalized_to_local * Vec3d(  1, -1, z_value );
    v1 = normalized_to_local * Vec3d( -1, -1, z_value );
    v2 = normalized_to_local * Vec3d( -1, 1, z_value );
    v3 = normalized_to_local * Vec3d(  1, 1, z_value );
  } else {
    glFrontFace( GL_CCW );
    v0 = normalized_to_local * Vec3d(  1, 1, z_value );
    v1 = normalized_to_local * Vec3d( -1, 1, z_value );
    v2 = normalized_to_local * Vec3d( -1, -1, z_value );
    v3 = normalized_to_local * Vec3d(  1, -1, z_value );
  }

  t0 = Vec3f( 1, 1, 0 );
  t1 = Vec3f( 0, 1, 0 );
  t2 = Vec3f( 0, 0, 0 );
  t3 = Vec3f( 1, 0, 0 );
  
  Vec3d normal = normalized_to_local.getScaleRotationPart() * Vec3d( 0, 0, -1 );
  normal.normalizeSafe();

  glBegin( GL_QUADS );
  glNormal3d  ( normal.x, normal.y, normal.z );
  renderTexCoordForActiveTexture( t0 );
  glVertex3d  ( v0.x, v0.y, v0.z );
  renderTexCoordForActiveTexture( t1 );
  glVertex3d  ( v1.x, v1.y, v1.z );
  renderTexCoordForActiveTexture( t2 );
  glVertex3d  ( v2.x, v2.y, v2.z );
  renderTexCoordForActiveTexture( t3 );
  glVertex3d  ( v3.x, v3.y, v3.z );
  glEnd();

  // restore previous front face.
  glFrontFace( front_face );
}



