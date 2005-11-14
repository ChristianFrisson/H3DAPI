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
/// \file Arc2D.cpp
/// \brief CPP file for Arc2D, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include "Arc2D.h"
#include "HLFeedbackShape.h"

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase Arc2D::database( "Arc2D", 
                               &(newInstance<Arc2D>), 
                               typeid( Arc2D ),
                               &X3DGeometryNode::database );

namespace Arc2DInternals {
  FIELDDB_ELEMENT( Arc2D, endAngle, INPUT_OUTPUT );
  FIELDDB_ELEMENT( Arc2D, startAngle, INPUT_OUTPUT );
  FIELDDB_ELEMENT( Arc2D, radius, INPUT_OUTPUT );
}


Arc2D::Arc2D( Inst< SFNode      > _metadata,
              Inst< SFBound     > _bound,
              Inst< DisplayList > _displayList,
              Inst< MFBool      > _isTouched,
              Inst< MFVec3f     > _force,
              Inst< MFVec3f     > _contactPoint,
              Inst< MFVec3f     > _contactNormal,
              Inst< SFFloat     > _endAngle,
              Inst< SFFloat     > _startAngle,
              Inst< SFFloat     > _radius ):
  X3DGeometryNode( _metadata, _bound, _displayList, _isTouched,
                   _force, _contactPoint, _contactNormal ),
  endAngle( _endAngle ),
  startAngle( _startAngle ),
  radius( _radius ) {

  type_name = "Arc2D";
  database.initFields( this );

  endAngle->setValue( (H3DFloat) Constants::pi/2 );
  startAngle->setValue( 0.f );
  radius->setValue( 1.f );

  radius->route( bound );

  endAngle->route( displayList );
  startAngle->route( displayList );
  radius->route( displayList );
}

void Arc2D::render() {

  H3DFloat start_angle = startAngle->getValue();
  H3DFloat end_angle = endAngle->getValue();
  H3DFloat r = radius->getValue();

  if( start_angle == end_angle ) {
    start_angle = 0.f;
    end_angle = (H3DFloat)Constants::pi * 2;
  }

  H3DFloat theta, angle_increment;
  H3DFloat x, y;
  H3DFloat nr_segments = 40;

  angle_increment = (H3DFloat) Constants::pi*2 / nr_segments;

  glBegin( GL_LINE_STRIP );
  
  for ( theta = start_angle; 
        theta < end_angle;
        theta += angle_increment ) {
    x = r * H3DCos(theta);
    y = r * H3DSin(theta);
    
    glVertex2f (x, y);
  }

  theta = end_angle;
  x = r * H3DCos(theta);
  y = r * H3DSin(theta);
  
  glVertex2f(x, y);
 
  glEnd ();
}

void Arc2D::traverseSG( TraverseInfo &ti ) {
  if( ti.hapticsEnabled() && ti.getCurrentSurface() ) {
    ti.addHapticShapeToAll( new HLFeedbackShape( this,
                                                 ti.getCurrentSurface(),
                                                 ti.getAccForwardMatrix(),
                                                 40 ) );
  }
}
