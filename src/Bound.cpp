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
/// \file Bound.cpp
/// \brief CPP file for Bound and some of its subclasses.
///
//
//////////////////////////////////////////////////////////////////////////////
#include <H3D/Bound.h>

#ifdef MACOSX
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

using namespace H3D;

void BoxBound::render() {
  Vec3f c = center->getValue();
  Vec3f half_size = size->getValue() / 2;
  Vec3f min = c - half_size;
  Vec3f max = c + half_size;

  glDisable( GL_LIGHTING );
  glColor3f( 0, 0, 1 );

  glBegin( GL_LINE_STRIP );
  glVertex3f( min.x, min.y, min.z );
  glVertex3f( min.x, max.y, min.z );
  glVertex3f( max.x, max.y, min.z );
  glVertex3f( max.x, min.y, min.z );
  glVertex3f( min.x, min.y, min.z );
  glEnd();

  glBegin( GL_LINE_STRIP );
  glVertex3f( min.x, min.y, max.z );
  glVertex3f( min.x, max.y, max.z );
  glVertex3f( max.x, max.y, max.z );
  glVertex3f( max.x, min.y, max.z );
  glVertex3f( min.x, min.y, max.z );
  glEnd();

  glBegin( GL_LINES );
  glVertex3f( min.x, min.y, max.z );
  glVertex3f( min.x, min.y, min.z );
  glVertex3f( max.x, min.y, max.z );
  glVertex3f( max.x, min.y, min.z );
  glVertex3f( min.x, max.y, max.z );
  glVertex3f( min.x, max.y, min.z );
  glVertex3f( max.x, max.y, max.z );
  glVertex3f( max.x, max.y, min.z );
  glEnd();
  glEnable( GL_LIGHTING );
}

bool BoxBound::movingSphereIntersect( const Vec3f &from,
                                      const Vec3f &to,
                                      H3DFloat radius ) {
  // duplicate code (see linesegmentIntersect) because we can not set the
  // size otherwise.
   // algorithm from "Realtime Collision Detection" book
  Vec3f e = ( size->getValue() + 2 * Vec3f( radius, radius, radius ) ) / 2.0;
  const Vec3f c = center->getValue();
  // line center
  Vec3f m = (from + to ) * 0.5f;
  // halflength vector
  Vec3f d = to - m;
  // translate to origin
  m = m -c; 

  H3DFloat adx = H3DAbs( d.x );
  if( H3DAbs( m.x ) > e.x + adx ) return false;
  H3DFloat ady = H3DAbs( d.y );
  if( H3DAbs( m.y ) > e.y + ady ) return false;
  H3DFloat adz = H3DAbs( d.z );
  if( H3DAbs( m.z ) > e.z + adz ) return false;

  adx += Constants::f_epsilon;
  ady += Constants::f_epsilon;
  adz += Constants::f_epsilon;

  if( H3DAbs( m.y * d.z - m.z * d.y ) > e.y * adz + e.z * ady ) 
    return false;
  if( H3DAbs( m.z * d.x - m.x * d.z ) > e.x * adz + e.z * adx ) 
    return false;
  if( H3DAbs( m.x * d.y - m.y * d.x ) > e.x * ady + e.y * adx ) 
    return false;

  return true;
}
