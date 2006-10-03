//////////////////////////////////////////////////////////////////////////////
//    Copyright 2004, SenseGraphics AB
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
/// \file Cylinder.cpp
/// \brief CPP file for Cylinder, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include "Cylinder.h"
 
using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase Cylinder::database( "Cylinder", 
                                    &(newInstance<Cylinder>), 
                                    typeid( Cylinder ),
                                    &X3DGeometryNode::database );

namespace CylinderInternals {
  FIELDDB_ELEMENT( Cylinder, bound, INPUT_OUTPUT );
  FIELDDB_ELEMENT( Cylinder, bottom, INPUT_OUTPUT );
  FIELDDB_ELEMENT( Cylinder, height, INPUT_OUTPUT );
  FIELDDB_ELEMENT( Cylinder, radius, INPUT_OUTPUT );
  FIELDDB_ELEMENT( Cylinder, side, INPUT_OUTPUT );
  FIELDDB_ELEMENT( Cylinder, solid, INPUT_OUTPUT );
  FIELDDB_ELEMENT( Cylinder, top, INPUT_OUTPUT );
}

Cylinder::Cylinder( 
                   Inst< SFNode  >  _metadata,
                   Inst< SFBound > _bound,
                   Inst< SFBool  >  _bottom,
                   Inst< SFFloat >  _height,
                   Inst< SFFloat >  _radius,
                   Inst< SFBool  >  _side,
                   Inst< SFBool  >  _solid,
                   Inst< SFBool  >  _top ) :
  X3DGeometryNode( _metadata, _bound ),
  bottom  ( _bottom   ),
  height  ( _height   ),
  radius  ( _radius   ),
  side    ( _side     ),
  solid   ( _solid    ),
  top     ( _top      ) {

  type_name = "Cylinder";
  database.initFields( this );

  bottom->setValue( true );
  height->setValue(2);
  radius->setValue(1);
  side->setValue ( true );
  solid->setValue( true );
  top->setValue  ( true );

  bottom->route( displayList );
  height->route( displayList );
  radius->route( displayList );
  side->route( displayList );
  solid->route( displayList );
  top->route( displayList );

  height->route( bound );
  radius->route( bound );
}

void Cylinder::render() { 
  X3DGeometryNode::render();     
  
  const float l_radius = radius->getValue();
  const float l_height = height->getValue();
  bool is_solid = solid->getValue();
  int nr_faces = 120;

  // render side
  if ( side->getValue() ) {
    glBegin( GL_QUAD_STRIP );
    for( int i=0; i<=nr_faces; ++i ) {
      float ratio = (float) i / nr_faces;
      float angle =  (float)(ratio * (Constants::pi*2));
      
      float sina = sin( angle );
      float cosa = cos( angle );

      glNormal3f( -sina, 0, -cosa );
      glTexCoord2f( ratio, 1 );
      glVertex3f( -l_radius * sina, l_height / 2, -l_radius * cosa );
      glTexCoord2f( ratio, 0 );
      glVertex3f( -l_radius * sina, -l_height / 2, -l_radius * cosa );
    }
    glEnd();
  }

  // render top
  if ( top->getValue() ) {
    glBegin( GL_POLYGON );
    glNormal3f( 0, 1, 0 );
    for( int i = 0; i < nr_faces; i++ ) {
      float angle = (float)( i * (Constants::pi*2) / (float) nr_faces);
      float sina = sin( angle );
      float cosa = cos( angle );
      glTexCoord2f( 0.5f - 0.5f * sina, 0.5f + 0.5f * cosa );
      glVertex3f( -l_radius * sina, l_height / 2, -l_radius * cosa );
    }
    glEnd();
  }
    
  // render bottom
  if ( bottom->getValue() ) {
    glBegin( GL_POLYGON );
    glNormal3f( 0, -1, 0 );
    for( int i = nr_faces; i >= 0; i-- ) {
      float angle = (float)( i * (Constants::pi*2) / (float) nr_faces);
      float sina = sin( angle );
      float cosa = cos( angle );
      glTexCoord2f( 0.5f - 0.5f * sina, 0.5f + 0.5f * cosa );
      glVertex3f( -l_radius * sina, -l_height / 2, -l_radius * cosa );
    }
    glEnd();
  }
    
};



#ifdef USE_HAPTICS
void Cylinder::traverseSG( TraverseInfo &ti ) {
  if( solid->getValue() ) {
    useBackFaceCulling( true );
  } else {
    useBackFaceCulling( false );
  }
  if( ti.hapticsEnabled() && ti.getCurrentSurface() ) {
#ifdef HAVE_OPENHAPTICS
    ti.addHapticShapeToAll(  getOpenGLHapticShape( ti.getCurrentSurface(),
                                                   ti.getAccForwardMatrix()));
#endif
  }
}
#endif
