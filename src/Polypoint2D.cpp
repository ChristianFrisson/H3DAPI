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
/// \file Polypoint2D.cpp
/// \brief CPP file for Polypoint2D, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <Polypoint2D.h>
#include <HLFeedbackShape.h>

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
  point    ( _point     ) {

  type_name = "Polypoint2D";
  database.initFields( this );

  point->route( bound );
  point->route( displayList );
}

void Polypoint2D::render() {
    // render the points
  glBegin( GL_POINTS );
  for( MFVec2f::const_iterator i = point->begin(); i!=point->end(); i++ ) {
    // Render the vertices.
    glVertex3f( (*i).x, (*i).y, 0.f );
  }
  // end GL_POINTS
  glEnd();
}

