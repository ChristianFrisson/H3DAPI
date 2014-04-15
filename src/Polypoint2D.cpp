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
/// \file Polypoint2D.cpp
/// \brief CPP file for Polypoint2D, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/Polypoint2D.h>
#include <H3D/GlobalSettings.h>
#include <H3D/GraphicsOptions.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase Polypoint2D::database( "Polypoint2D", 
                               &(newInstance<Polypoint2D>), 
                               typeid( Polypoint2D ),
                               &X3DGeometryNode::database );

namespace Polypoint2DInternals {
  FIELDDB_ELEMENT( Polypoint2D, point, INPUT_OUTPUT );
}


Polypoint2D::Polypoint2D( Inst< SFNode      > _metadata,
                          Inst< SFBound     > _bound,
                          Inst< DisplayList > _displayList,
                          Inst< MFBool      > _isTouched,
                          Inst< MFVec3f     > _force,
                          Inst< MFVec3f     > _contactPoint,
                          Inst< MFVec3f     > _contactNormal,
                          Inst< MFVec2f     > _point ):
  X3DGeometryNode( _metadata, _bound, _displayList, _isTouched,
                   _force, _contactPoint, _contactNormal ),
  point    ( _point ),
  vboFieldsUpToDate( new Field ),
  vbo_id( NULL ) {

  type_name = "Polypoint2D";
  database.initFields( this );

  point->route( bound );
  point->route( displayList );
  vboFieldsUpToDate->setName( "vboFieldsUpToDate" );
  point->route( vboFieldsUpToDate );
}

Polypoint2D::~Polypoint2D() {
  // Delete buffer if it was allocated.
  if( vbo_id ) {
    glDeleteBuffersARB( 1, vbo_id );
    delete vbo_id;
    vbo_id = NULL;
  }
}

void Polypoint2D::DisplayList::callList( bool build_list ) {
  Polypoint2D *arc = 
   static_cast< Polypoint2D * >( owner );

  glPushAttrib( GL_CURRENT_BIT );
  float v[4];
  glGetMaterialfv( GL_FRONT, GL_EMISSION, v );
  glColor3f( v[0], v[1], v[2] );

  X3DGeometryNode::DisplayList::callList( build_list );
  glPopAttrib();
}

void Polypoint2D::render() {
  // Save the old state of GL_LIGHTING 
  GLboolean lighting_enabled;
  glGetBooleanv( GL_LIGHTING, &lighting_enabled );
  glDisable( GL_LIGHTING );

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
                       point->size() * 2 * sizeof(GLfloat),
                       &(*point->begin()), GL_STATIC_DRAW_ARB );
    } else {
      glBindBufferARB( GL_ARRAY_BUFFER_ARB, *vbo_id);
    }

    // Enable all states for vertex buffer objects.
    // Note that the data is interleaved since this supposedly should be
    // faster on some systems.
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_FLOAT, 0, NULL );
    glDrawArrays( GL_POINTS, 0, point->size() );
    glDisableClientState(GL_VERTEX_ARRAY);
    glBindBufferARB( GL_ARRAY_BUFFER_ARB, 0 );
  } else {
    // render the points
    glBegin( GL_POINTS );
    for( MFVec2f::const_iterator i = point->begin(); i!=point->end(); ++i ) {
      // Render the vertices.
      glVertex3f( (*i).x, (*i).y, 0.f );
    }
    // end GL_POINTS
    glEnd();
  }
  // reenable lighting if it was enabled before
  if( lighting_enabled )
    glEnable( GL_LIGHTING );
}

