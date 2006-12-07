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
  H3DFloat theta_parts = 50, phi_parts = 25;

  H3DFloat inc_theta = (H3DFloat) Constants::pi*2 / theta_parts;
  H3DFloat inc_phi =   (H3DFloat) Constants::pi / phi_parts;

  H3DFloat r = radius->getValue();
  
  H3DFloat double_pi = (H3DFloat) Constants::pi * 2;

  glBegin( GL_QUADS );

  for (unsigned int p = 0; p < phi_parts; p++ ) {
    for (unsigned int t = 0; t < theta_parts; t++ ) {
      H3DFloat phi = p * inc_phi;
      H3DFloat theta = t * inc_theta;
      H3DFloat next_phi = phi + inc_phi;
      bool at_seam = t == theta_parts - 1;
      H3DFloat next_theta = ( at_seam ? 0 :theta + inc_theta );
      
      H3DFloat x, y, z;
      
      x = - H3DSin( phi ) * H3DSin( theta );
      y = H3DCos( phi );
      z = - H3DSin( phi ) * H3DCos( theta );
      
      glNormal3f( x, y, z );
      renderTexCoordForActiveTexture( 
                          Vec3f( (H3DFloat) (theta / double_pi), 
                                 (H3DFloat) (1 - phi/ Constants::pi),
                                 0 ) );
      glVertex3f( x * r, y * r, z * r );

      x = - H3DSin( next_phi ) * H3DSin( theta );
      y = H3DCos( next_phi );
      z = - H3DSin( next_phi ) * H3DCos( theta );

      glNormal3f( x, y, z );
      renderTexCoordForActiveTexture( 
                          Vec3f( (H3DFloat) (theta / double_pi), 
                                 (H3DFloat) (1 - next_phi/ Constants::pi ),
                                 0 ) );
      glVertex3f( x * r, y * r, z * r );

      x = - H3DSin( next_phi ) * H3DSin( next_theta );
      y = H3DCos( next_phi );
      z = - H3DSin( next_phi ) * H3DCos( next_theta );

      glNormal3f( x, y, z );
      renderTexCoordForActiveTexture( 
                     Vec3f( at_seam ? 1 : (H3DFloat) (next_theta / double_pi ), 
                            (H3DFloat)(1 - next_phi/ Constants::pi),
                            0 ) );
      glVertex3f( x * r, y * r, z * r );

      x = - H3DSin( phi ) * H3DSin( next_theta );
      y = H3DCos( phi );
      z = - H3DSin( phi ) * H3DCos( next_theta );

      glNormal3f( x, y, z );
      renderTexCoordForActiveTexture( 
                   Vec3f( at_seam ? 1 : (H3DFloat)(next_theta / double_pi), 
                          (H3DFloat)(1 - phi/ Constants::pi),
                          0 ) );
      glVertex3f( x * r, y * r, z * r );
    }
  }

  glEnd();
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
