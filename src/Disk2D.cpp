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
/// \file Disk2D.cpp
/// \brief CPP file for Disk2D, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include "Disk2D.h"
#include "HLFeedbackShape.h"

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase Disk2D::database( "Disk2D", 
                               &(newInstance<Disk2D>), 
                               typeid( Disk2D ),
                               &X3DGeometryNode::database );

namespace Disk2DInternals {
  FIELDDB_ELEMENT( Disk2D, innerRadius, INPUT_OUTPUT );
  FIELDDB_ELEMENT( Disk2D, outerRadius, INPUT_OUTPUT );
  FIELDDB_ELEMENT( Disk2D, solid, INPUT_OUTPUT );
}


Disk2D::Disk2D( Inst< SFNode      > _metadata,
                Inst< SFBound     > _bound,
                Inst< DisplayList > _displayList,
                Inst< MFBool      > _isTouched,
                Inst< MFVec3f     > _force,
                Inst< MFVec3f     > _contactPoint,
                Inst< MFVec3f     > _contactNormal,
                Inst< SFFloat     > _innerRadius,
                Inst< SFFloat     > _outerRadius,
                Inst< SFBool      > _solid ):
  X3DGeometryNode( _metadata, _bound, _displayList, _isTouched,
                   _force, _contactPoint, _contactNormal ),
  innerRadius( _innerRadius ),
  outerRadius( _outerRadius ),
  solid( _solid ) {

  type_name = "Disk2D";
  database.initFields( this );

  innerRadius->setValue( 0.f );
  outerRadius->setValue( 1.f );
  solid->setValue( false );

  outerRadius->route( bound );

  innerRadius->route( displayList );
  outerRadius->route( displayList );
  solid->route( displayList );
}

void Disk2D::render() {
  H3DFloat inner_radius = innerRadius->getValue();
  H3DFloat outer_radius = outerRadius->getValue();
  
  H3DFloat theta, angle_increment;
  H3DFloat nr_segments = 40;

  angle_increment = (H3DFloat) Constants::pi*2 / nr_segments;

  if( solid->getValue() ) {
    glCullFace( GL_BACK );
    glEnable( GL_CULL_FACE );
  } else {
    glDisable( GL_CULL_FACE );
  }

  if( inner_radius == 0 ) {
    // draw a filled circle
    glBegin( GL_TRIANGLE_FAN );
    glNormal3f( 0.f, 0.f, 1.f );
    glTexCoord2f( 0.5f, 0.5f );
    glVertex2f( 0, 0 );
    H3DFloat x, y;
    int i = 0;
    for ( ; i < nr_segments; i++ ) {
      theta = i * angle_increment;
      x = outer_radius * H3DCos(theta);
      y = outer_radius * H3DSin(theta);
      glTexCoord2f( x /(outer_radius*2) + 0.5f, y / (outer_radius*2) + 0.5f );
      glVertex2f (x, y);
    }
    
    theta = 0;
    x = outer_radius * H3DCos(theta);
    y = outer_radius * H3DSin(theta);
    glTexCoord2f( x /(outer_radius*2) + 0.5f, y / (outer_radius*2) + 0.5f );
    glVertex2f (x, y);
    
    glEnd ();
  } else if( outer_radius == inner_radius ) {
    // draw a circle with lines
    H3DFloat x, y;
    glBegin( GL_LINE_STRIP );
    int i = 0;
    for ( ; i < nr_segments; i++ ) {
      theta = i * angle_increment;
      x = outer_radius * H3DCos(theta);
      y = outer_radius * H3DSin(theta);
      glVertex2f (x, y);
    }
    
    theta = 0;
    x = outer_radius * H3DCos(theta);
    y = outer_radius * H3DSin(theta);
    
    glVertex2f(x, y);
    
    glEnd ();
  } else {
    // draw a disc
    glBegin( GL_QUAD_STRIP );
    
    Vec2f inner_start_point, outer_start_point;
    glNormal3f( 0.f, 0.f, 1.f );
    int i = 0;
    for ( ; i < nr_segments; i++ ) {
      theta = i * angle_increment;
      H3DFloat cos_t = H3DCos( theta );
      H3DFloat sin_t = H3DSin( theta );
      H3DFloat inner_x = inner_radius * cos_t;
      H3DFloat inner_y = inner_radius * sin_t;
      H3DFloat outer_x = outer_radius * cos_t;
      H3DFloat outer_y = outer_radius * sin_t;
      if( i == 0 ) {
        inner_start_point = Vec2f( inner_x, inner_y );
        outer_start_point = Vec2f( outer_x, outer_y );
      }
      
      glTexCoord2f( inner_x / (outer_radius*2) + 0.5f,
                    inner_y / (outer_radius*2) + 0.5f) ;
      glVertex2f (inner_x, inner_y);
      glTexCoord2f( outer_x / (outer_radius*2) + 0.5f,
                    outer_y / (outer_radius*2) + 0.5f );
      glVertex2f (outer_x, outer_y);
    }
    
    if( i != 0 ) {
      glTexCoord2f( inner_start_point.x / (outer_radius*2) + 0.5f,
                    inner_start_point.y / (outer_radius*2) + 0.5f) ;
      glVertex2f (inner_start_point.x, inner_start_point.y);
      glTexCoord2f( outer_start_point.x / (outer_radius*2) + 0.5f,
                    outer_start_point.y / (outer_radius*2) + 0.5f) ;
      glVertex2f (outer_start_point.x, outer_start_point.y);
    }
    
    glEnd ();
  }
}

void Disk2D::traverseSG( TraverseInfo &ti ) {
  if( ti.hapticsEnabled() && ti.getCurrentSurface() ) {
    ti.addHapticShapeToAll( new HLFeedbackShape( this,
                                                 ti.getCurrentSurface(),
                                                 ti.getAccForwardMatrix() ) );
  }
}
