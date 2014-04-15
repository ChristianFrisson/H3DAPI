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
/// \file TriangleSet2D.cpp
/// \brief CPP file for TriangleSet2D, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/TriangleSet2D.h>
#include <H3D/MultiTexture.h>
#include <H3D/GlobalSettings.h>
#include <H3D/GraphicsOptions.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase TriangleSet2D::database( "TriangleSet2D", 
                               &(newInstance<TriangleSet2D>), 
                               typeid( TriangleSet2D ),
                               &X3DGeometryNode::database );

namespace TriangleSet2DInternals {
  FIELDDB_ELEMENT( TriangleSet2D, vertices, INPUT_OUTPUT );
  FIELDDB_ELEMENT( TriangleSet2D, solid, INPUT_OUTPUT );
}


TriangleSet2D::TriangleSet2D( Inst< SFNode      > _metadata,
                              Inst< SFBound     > _bound,
                              Inst< DisplayList > _displayList,
                              Inst< MFBool      > _isTouched,
                              Inst< MFVec3f     > _force,
                              Inst< MFVec3f     > _contactPoint,
                              Inst< MFVec3f     > _contactNormal,
                              Inst< MFVec2f     > _vertices,
                              Inst< SFBool      > _solid ):
  X3DGeometryNode( _metadata, _bound, _displayList, _isTouched,
                   _force, _contactPoint, _contactNormal ),
  vertices( _vertices ),
  solid( _solid ),
  vboFieldsUpToDate( new Field ),
  vbo_id( NULL ) {

  type_name = "TriangleSet2D";
  database.initFields( this );

  solid->setValue( false );

  vertices->route( bound );
  vertices->route( displayList );
  solid->route( displayList );

  vboFieldsUpToDate->setName( "vboFieldsUpToDate" );
  vertices->route( vboFieldsUpToDate );
}

TriangleSet2D::~TriangleSet2D() {
  // Delete buffer if it was allocated.
  if( vbo_id ) {
    glDeleteBuffersARB( 1, vbo_id );
    delete vbo_id;
    vbo_id = NULL;
  }
}

void TriangleSet2D::render() {
  // render the points
  const vector< Vec2f > &v = vertices->getValue();

  BoxBound *bb = dynamic_cast< BoxBound * >( bound->getValue() );
  MultiTexture *mt = 
    dynamic_cast< MultiTexture * >( X3DTextureNode::getActiveTexture() );
  if( bb ) {
    const Vec3f &center = bb->center->getValue();
    const Vec3f &size = bb->size->getValue();
    H3DFloat sparams[4] = {1/size.x,0,0,0.5f - center.x / size.x};
    H3DFloat tparams[4] = {0,1/size.y,0,0.5f - center.y / size.y};
    
    if( mt ) {
      size_t texture_units = mt->texture->size();
      for( size_t i = 0; i < texture_units; ++i ) {
        glActiveTexture( GL_TEXTURE0_ARB + (unsigned int) i );
        glTexGend( GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR );
        glTexGend( GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR );
        glTexGenfv( GL_S, GL_OBJECT_PLANE, sparams );
        glTexGenfv( GL_T, GL_OBJECT_PLANE, tparams );
        glEnable( GL_TEXTURE_GEN_S );
        glEnable( GL_TEXTURE_GEN_T );
      }
    } else {
      glTexGend( GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR );
      glTexGend( GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR );
      glTexGenfv( GL_S, GL_OBJECT_PLANE, sparams );
      glTexGenfv( GL_T, GL_OBJECT_PLANE, tparams );
      glEnable( GL_TEXTURE_GEN_S );
      glEnable( GL_TEXTURE_GEN_T );
    }
  }

  bool prefer_vertex_buffer_object = false;
  if( GLEW_ARB_vertex_buffer_object ) {
    GraphicsOptions * go = NULL;
    getOptionNode( go );
    if( !go ) {
      GlobalSettings * gs = GlobalSettings::getActive();
      if( gs ) {
        gs->getOptionNode( go );
      }
    }
    if( go ) {
      prefer_vertex_buffer_object = go->preferVertexBufferObject->getValue();
    }
  }

  if( prefer_vertex_buffer_object ) {
    // Use vertex buffer objects to create sphere.
    if( !vboFieldsUpToDate->isUpToDate() ) {
      vboFieldsUpToDate->upToDate();
      if( !vbo_id ) {
        vbo_id = new GLuint;
        glGenBuffersARB( 1, vbo_id );
      }
      glBindBufferARB( GL_ARRAY_BUFFER_ARB, *vbo_id );
      glBufferDataARB( GL_ARRAY_BUFFER_ARB,
                       v.size() * 2 * sizeof(GLfloat),
                       &(*v.begin()), GL_STATIC_DRAW_ARB );
    } else {
      glBindBufferARB( GL_ARRAY_BUFFER_ARB, *vbo_id);
    }

    glNormal3f( 0.f, 0.f, 1.f );
    // Enable all states for vertex buffer objects.
    // Note that the data is interleaved since this supposedly should be
    // faster on some systems.
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_FLOAT, 0, NULL );
    glDrawArrays( GL_TRIANGLES, 0, (GLsizei) v.size() );
    glDisableClientState(GL_VERTEX_ARRAY);
    glBindBufferARB( GL_ARRAY_BUFFER_ARB, 0 );
  } else {
    glBegin( GL_TRIANGLES );
    glNormal3f( 0.f, 0.f, 1.f );
    for( unsigned int i = 0; i < v.size(); i+=3 ) {
      glVertex2f( v[i].x, v[i].y );
      glVertex2f( v[i+1].x, v[i+1].y );
      glVertex2f( v[i+2].x, v[i+2].y );
    }
    glEnd();
  }

  if( bb && mt ) {
    size_t texture_units = mt->texture->size();
    for( size_t i = 0; i < texture_units; ++i ) {
      glActiveTexture( GL_TEXTURE0_ARB + (unsigned int) i );
      glDisable( GL_TEXTURE_GEN_S );
      glDisable( GL_TEXTURE_GEN_T );
    }
  } else {
      glDisable( GL_TEXTURE_GEN_S );
      glDisable( GL_TEXTURE_GEN_T );
  }
}

void TriangleSet2D::traverseSG( TraverseInfo &ti ) {
  X3DGeometryNode::traverseSG( ti );
  // use backface culling if solid is true
  if( solid->getValue() ) useBackFaceCulling( true );
  else useBackFaceCulling( false );
}

