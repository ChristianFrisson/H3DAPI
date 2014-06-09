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
/// \file FullscreenRectangle.cpp
/// \brief CPP file for FullscreenRectangle, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/FullscreenRectangle.h>
#include <H3D/GlobalSettings.h>

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

GLuint  FullscreenRectangle::vbo_id[2] = { 0, 0 };
bool FullscreenRectangle::vbo_initialized = false;
vector< GLfloat > FullscreenRectangle::fullscreenRectangle_data = vector< GLfloat >();
vector< GLuint > FullscreenRectangle::fullscreenRectangle_index_data = vector< GLuint >();

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

  bool prefer_vertex_buffer_object = false;
  if( GLEW_ARB_vertex_array_object ) {
    GraphicsOptions *go = NULL;
    getOptionNode( go );
    if( !go ) {
      GlobalSettings *gs = GlobalSettings::getActive();
      if( gs ) {
        gs->getOptionNode( go );
      }
    }
    if( go ) {
      prefer_vertex_buffer_object = go->preferVertexBufferObject->getValue();
    }
  }

  // build up the vertex data
  unsigned int nr_data_vertices = 9; // 9 floats (3 each for VTN) per vertex
  fullscreenRectangle_data.clear();
  //fullscreenRectangle_data.resize( (unsigned int) 4*nr_data_vertices, 0 ); // 4 vertices in total
  unsigned int nr_index = 6; // 2 triangles, 3 vertices/triangle.
  //fullscreenRectangle_index_data.resize( (unsigned int) 4, 0 );
  fullscreenRectangle_index_data.clear();

  H3DFloat z_value = zValue->getValue();
  H3DFloat epsilon = (H3DFloat) 1e-6;

  // assure that we do not draw outside the view frustum because of 
  // roundoff errors
  if( z_value == -1 ) z_value = z_value + epsilon;
  else if( z_value == 1 ) z_value = z_value - epsilon;

  // Set polygon mode to always be filled. FullscreenRectangle is almost exclusively used for 
  // post processing effects and if wireframe mode is used we do not want the default behavior
  // being to render it in wireframe as nothing at all will be shown then. Hence we force it 
  // filled.
  glPushAttrib( GL_POLYGON_MODE );
  glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

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
  t0 = Vec3f( 1, 1, 0 );
  t1 = Vec3f( 0, 1, 0 );
  t2 = Vec3f( 0, 0, 0 );
  t3 = Vec3f( 1, 0, 0 );
  Vec3d normal = normalized_to_local.getScaleRotationPart() * Vec3d( 0, 0, -1 );
  normal.normalizeSafe();

  glGetIntegerv( GL_FRONT_FACE, &front_face );
  if( front_face == GL_CW && !screenAligned->getValue() ) {
    // we are in mirrored mode and want the screen to flip.
    v0 = normalized_to_local * Vec3d(  1, -1, z_value );
    v1 = normalized_to_local * Vec3d( -1, -1, z_value );
    v2 = normalized_to_local * Vec3d( -1, 1, z_value );
    v3 = normalized_to_local * Vec3d(  1, 1, z_value );
    fillVec3ToArray(v0, fullscreenRectangle_data);
    fillVec3ToArray(normal, fullscreenRectangle_data);
    fillVec3ToArray(t0, fullscreenRectangle_data);
    fillVec3ToArray(v1, fullscreenRectangle_data);
    fillVec3ToArray(normal, fullscreenRectangle_data);
    fillVec3ToArray(t1, fullscreenRectangle_data);
    fillVec3ToArray(v2, fullscreenRectangle_data);
    fillVec3ToArray(normal, fullscreenRectangle_data);
    fillVec3ToArray(t2, fullscreenRectangle_data);
    fillVec3ToArray(v3, fullscreenRectangle_data);
    fillVec3ToArray(normal,fullscreenRectangle_data);
    fillVec3ToArray(t3,fullscreenRectangle_data);
    
  } else {
    glFrontFace( GL_CCW );
    v0 = normalized_to_local * Vec3d(  1, 1, z_value );
    v1 = normalized_to_local * Vec3d( -1, 1, z_value );
    v2 = normalized_to_local * Vec3d( -1, -1, z_value );
    v3 = normalized_to_local * Vec3d(  1, -1, z_value );
    fillVec3ToArray(v0, fullscreenRectangle_data);
    fillVec3ToArray(normal, fullscreenRectangle_data);
    fillVec3ToArray(t0, fullscreenRectangle_data);
    fillVec3ToArray(v1, fullscreenRectangle_data);
    fillVec3ToArray(normal, fullscreenRectangle_data);
    fillVec3ToArray(t1, fullscreenRectangle_data);
    fillVec3ToArray(v2, fullscreenRectangle_data);
    fillVec3ToArray(normal, fullscreenRectangle_data);
    fillVec3ToArray(t2, fullscreenRectangle_data);
    fillVec3ToArray(v3, fullscreenRectangle_data);
    fillVec3ToArray(normal,fullscreenRectangle_data);
    fillVec3ToArray(t3,fullscreenRectangle_data);
  }
  fillVec3ToArray(Vec3f(0,1,2),fullscreenRectangle_index_data);
  fillVec3ToArray(Vec3f(0,2,3),fullscreenRectangle_index_data);

  // setup data pointer and send to graphic card 
  GLvoid *vertex_pointer = NULL, *normal_pointer = NULL, *texture_pointer = NULL, *index_pointer = NULL;

  if( prefer_vertex_buffer_object ) {
    if( !vbo_initialized ) {
      vbo_initialized = true;
      
      glGenBuffersARB( 2, vbo_id );

      glBindBufferARB( GL_ARRAY_BUFFER_ARB, vbo_id[0] );
      glBufferDataARB( GL_ARRAY_BUFFER_ARB, fullscreenRectangle_data.size()*sizeof(GLfloat), &(*fullscreenRectangle_data.begin()), GL_DYNAMIC_DRAW_ARB );
      glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, vbo_id[1] );
      glBufferDataARB( GL_ELEMENT_ARRAY_BUFFER_ARB, fullscreenRectangle_index_data.size()*sizeof(GLuint), &(*fullscreenRectangle_index_data.begin()), GL_DYNAMIC_DRAW_ARB );
    }else{
      glBindBufferARB( GL_ARRAY_BUFFER_ARB, vbo_id[0] );
      glBufferDataARB( GL_ARRAY_BUFFER_ARB, fullscreenRectangle_data.size()*sizeof(GLfloat), &(*fullscreenRectangle_data.begin()), GL_DYNAMIC_DRAW_ARB );
      glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, vbo_id[1] );
      glBufferDataARB( GL_ELEMENT_ARRAY_BUFFER_ARB, fullscreenRectangle_index_data.size()*sizeof(GLuint), &(*fullscreenRectangle_index_data.begin()), GL_DYNAMIC_DRAW_ARB );
    }
    vertex_pointer = (GLvoid*)(0*sizeof(GLfloat));
    normal_pointer = (GLvoid*)(3*sizeof(GLfloat));
    texture_pointer = (GLvoid*)(6*sizeof(GLfloat)); 
  }else{
    vector< GLfloat >::iterator start_pointer = fullscreenRectangle_data.begin();
    vertex_pointer = &(*start_pointer);
    start_pointer += 3;
    normal_pointer = &(*start_pointer);
    start_pointer += 3;
    texture_pointer = &(*start_pointer);
    index_pointer = &(*fullscreenRectangle_index_data.begin());
  }

  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, 9 * sizeof(GLfloat), vertex_pointer);
  glEnableClientState(GL_NORMAL_ARRAY);
  glNormalPointer(GL_FLOAT, 9*sizeof(GLfloat),normal_pointer);
  X3DTextureCoordinateNode::renderVertexBufferObjectForActiveTexture(3,GL_FLOAT,9*sizeof(GLfloat),texture_pointer);

  glDrawRangeElements(  GL_TRIANGLES,
                        0,
                        GLsizei(3),   //index start from 0 to 3
                        GLsizei(6),   //will be 6 index in total, 3 for each triangle
                        GL_UNSIGNED_INT,
                        index_pointer);

  X3DTextureCoordinateNode::disableVBOForActiveTexture();
  glDisableClientState(GL_NORMAL_ARRAY);
  glDisableClientState(GL_VERTEX_ARRAY);
  if( prefer_vertex_buffer_object ) {
    glBindBufferARB( GL_ARRAY_BUFFER_ARB, 0 );
    glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, 0 );
  }

  /*glBegin( GL_QUADS );
  glNormal3d  ( normal.x, normal.y, normal.z );
  renderTexCoordForActiveTexture( t0 );
  glVertex3d  ( v0.x, v0.y, v0.z );
  renderTexCoordForActiveTexture( t1 );
  glVertex3d  ( v1.x, v1.y, v1.z );
  renderTexCoordForActiveTexture( t2 );
  glVertex3d  ( v2.x, v2.y, v2.z );
  renderTexCoordForActiveTexture( t3 );
  glVertex3d  ( v3.x, v3.y, v3.z );
  glEnd();*/

  // restore previous front face.
  glFrontFace( front_face );

  glPopAttrib();
}

void FullscreenRectangle::fillVec3ToArray( const Vec3d V, vector<float>& array ){
  array.push_back(V.x);
  array.push_back(V.y);
  array.push_back(V.z);
}

void FullscreenRectangle::fillVec3ToArray( const Vec3f V, vector<GLuint>& array ){
  array.push_back(V.x);
  array.push_back(V.y);
  array.push_back(V.z);
}