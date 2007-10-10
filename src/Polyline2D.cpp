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
/// \file Polyline2D.cpp
/// \brief CPP file for Polyline2D, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/Polyline2D.h>
#include <HLFeedbackShape.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase Polyline2D::database( "Polyline2D", 
                               &(newInstance<Polyline2D>), 
                               typeid( Polyline2D ),
                               &X3DGeometryNode::database );

namespace Polyline2DInternals {
  FIELDDB_ELEMENT( Polyline2D, lineSegments, INPUT_OUTPUT );
}


Polyline2D::Polyline2D( Inst< SFNode      > _metadata,
                        Inst< SFBound     > _bound,
                        Inst< DisplayList > _displayList,
                        Inst< MFBool      > _isTouched,
                        Inst< MFVec3f     > _force,
                        Inst< MFVec3f     > _contactPoint,
                        Inst< MFVec3f     > _contactNormal,
                        Inst< MFVec2f     > _lineSegments ):
  X3DGeometryNode( _metadata, _bound, _displayList, _isTouched,
                   _force, _contactPoint, _contactNormal ),
  lineSegments( _lineSegments ) {

  type_name = "Polyline2D";
  database.initFields( this );

  lineSegments->route( bound );
  lineSegments->route( displayList );
}

void Polyline2D::render() {
  // render the points
  glBegin( GL_LINE_STRIP );
  for( MFVec2f::const_iterator i = lineSegments->begin(); i!=lineSegments->end(); i++ ) {
    // Render the vertices.
    glVertex3f( (*i).x, (*i).y, 0.f );
  }
  // end GL_POINTS
  glEnd();
}


