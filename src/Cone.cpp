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
/// \file Cone.cpp
/// \brief CPP file for Cone, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include "Cone.h"

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase Cone::database( "Cone", 
                                &(newInstance<Cone>), 
                                typeid( Cone ),
                                &X3DGeometryNode::database );

namespace ConeInternals {
  FIELDDB_ELEMENT( Cone, bottom, INPUT_OUTPUT );
  FIELDDB_ELEMENT( Cone, bottomRadius, INPUT_OUTPUT );
  FIELDDB_ELEMENT( Cone, height, INPUT_OUTPUT );
  FIELDDB_ELEMENT( Cone, side, INPUT_OUTPUT );
  FIELDDB_ELEMENT( Cone, solid, INPUT_OUTPUT );
}

Cone::Cone( 
            Inst< SFNode  >  _metadata,
            Inst< SFBound >  _bound,
            Inst< SFBool  >  _bottom,
            Inst< SFFloat >  _bottomRadius,
            Inst< SFFloat >  _height,
            Inst< SFBool  >  _side,
            Inst< SFBool  >  _solid ) :
  X3DGeometryNode( _metadata, _bound ),
  bottom      ( _bottom       ),
  bottomRadius( _bottomRadius ),
  height      ( _height       ),
  side        ( _side         ),
  solid       ( _solid        ) {


  type_name = "Cone";
  database.initFields( this );

  bottom->setValue( true );
  bottomRadius->setValue( 1 );
  height->setValue( 2 );
  side->setValue( true );
  solid->setValue( true );

  height->route( bound );
  bottomRadius->route( bound );
  
  bottom->route( displayList );
  bottomRadius->route( displayList );
  height->route( displayList );
  side->route( displayList );
  solid->route( displayList );
}


void Cone::render() {
  X3DGeometryNode::render();     
  
  const H3DFloat l_radius = bottomRadius->getValue();
  const H3DFloat l_height = height->getValue();
  bool is_solid = solid->getValue();
  int nr_faces = 120;
  
  // render side
  if ( side->getValue() ) {
    glBegin( GL_QUAD_STRIP );
    for( int i=0; i<=nr_faces; ++i ) {
      H3DFloat ratio = (H3DFloat) i / nr_faces;
      H3DFloat angle =  ratio * (H3DFloat)Constants::pi*2;
      
      H3DFloat sina = sin( angle );
      H3DFloat cosa = cos( angle );
      
      glNormal3f( -sina, atan( l_radius / l_height ) , -cosa );
      glTexCoord2f( ratio, 1 );
      glVertex3f( 0, l_height / 2, 0 );
      glTexCoord2f( ratio, 0 );
      glVertex3f( -l_radius * sina, -l_height / 2, -l_radius * cosa );
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
}


#ifdef USE_HAPTICS
void Cone::traverseSG( TraverseInfo &ti ) {
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

