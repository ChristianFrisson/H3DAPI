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
/// \file Circle2D.cpp
/// \brief CPP file for Circle2D, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/Circle2D.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase Circle2D::database( "Circle2D", 
                               &(newInstance<Circle2D>), 
                               typeid( Circle2D ),
                               &X3DGeometryNode::database );

namespace Circle2DInternals {
  FIELDDB_ELEMENT( Circle2D, radius, INPUT_OUTPUT );
}


Circle2D::Circle2D( Inst< SFNode      > _metadata,
                    Inst< SFBound     > _bound,
                    Inst< DisplayList > _displayList,
                    Inst< MFBool      > _isTouched,
                    Inst< MFVec3f     > _force,
                    Inst< MFVec3f     > _contactPoint,
                    Inst< MFVec3f     > _contactNormal,
                    Inst< SFFloat     > _radius ):
  X3DGeometryNode( _metadata, _bound, _displayList, _isTouched,
                   _force, _contactPoint, _contactNormal ),
  radius( _radius ) {

  type_name = "Circle2D";
  database.initFields( this );

  radius->setValue( 1.f );
  radius->route( bound );
  radius->route( displayList );
}

void Circle2D::render() {
  // Save the old state of GL_LIGHTING 
  GLboolean lighting_enabled;
  glGetBooleanv( GL_LIGHTING, &lighting_enabled );
  glDisable( GL_LIGHTING );

  // get the current emissiveColor
  float v[4];
  glGetMaterialfv( GL_FRONT, GL_EMISSION, v );
  glColor3f( v[0], v[1], v[2] );

  H3DFloat theta, angle_increment;
  H3DFloat nr_segments = 40;
  angle_increment = (H3DFloat) Constants::pi*2 / nr_segments;
  H3DFloat r = radius->getValue();

  // draw a circle with lines
  H3DFloat x, y;
  glBegin( GL_LINE_STRIP );
  int i = 0;
  for ( ; i < nr_segments; i++ ) {
    theta = i * angle_increment;
    x = r * H3DCos(theta);
    y = r * H3DSin(theta);
    glVertex2f (x, y);
  }
  
  theta = 0;
  x = r * H3DCos(theta);
  y = r * H3DSin(theta);
  
  glVertex2f(x, y);
  glEnd ();

  // reenable lighting if it was enabled before
  if( lighting_enabled )
    glEnable( GL_LIGHTING );
}

