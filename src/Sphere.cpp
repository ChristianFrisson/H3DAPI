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
/// \file Sphere.cpp
/// \brief CPP file for Sphere, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include "Sphere.h"
#ifdef USE_HAPTICS
#include "HapticSphere.h"
#endif

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase Sphere::database( 
                                 "Sphere", 
                                 &(newInstance<Sphere>), 
                                 typeid( Sphere ),
                                 &X3DGeometryNode::database );

namespace SphereInternals {
  FIELDDB_ELEMENT( Sphere, radius, INPUT_OUTPUT );
  FIELDDB_ELEMENT( Sphere, solid, INPUT_OUTPUT );
}


Sphere::Sphere( 
               Inst<    SFNode >  _metadata,
               Inst< SFBound >                 _bound,
               Inst< SFFloat>  _radius,
               Inst< SFBool >  _solid ) :
  X3DGeometryNode( _metadata, _bound ),
  radius  ( _radius   ),
  solid   ( _solid    ),
  gl_quadric( NULL ) {

  type_name = "Sphere";
  database.initFields( this );
  
  radius->setValue( 1 );
  solid->setValue( true );
    
  radius->route( bound );
  radius->route( displayList );
  solid->route( displayList );
}


void Sphere::render() {
  if( !gl_quadric ) {
    gl_quadric = gluNewQuadric();
    gluQuadricTexture( gl_quadric, GL_TRUE );
  }
  glPushMatrix();
  // we have to rotate the sphere in order to get the texturing
  // according to X3D spec.
  glRotatef( -90, 1, 0, 0 );
  gluSphere( gl_quadric, radius->getValue(), 50, 50 );
  glPopMatrix();
} 

#ifdef USE_HAPTICS
void Sphere::traverseSG( TraverseInfo &ti ) {
  // use backface culling if solid is true
  if( solid->getValue() ) useBackFaceCulling( true );
  else useBackFaceCulling( false );

  if( ti.hapticsEnabled() && ti.getCurrentSurface() ) {
    ti.addHapticShapeToAll( new HapticSphere( radius->getValue(),
                                              solid->getValue(),
                                              this,
                                              ti.getCurrentSurface(),
                                              ti.getAccForwardMatrix() ) );
  }
}
#endif
