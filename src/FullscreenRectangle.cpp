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
/// \file FullscreenRectangle.cpp
/// \brief CPP file for FullscreenRectangle, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/FullscreenRectangle.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase FullscreenRectangle::database( "FullscreenRectangle", 
                               &(newInstance<FullscreenRectangle>), 
                               typeid( FullscreenRectangle ),
                               &X3DGeometryNode::database );

namespace FullscreenRectangleInternals {
  FIELDDB_ELEMENT( FullscreenRectangle, zValue, INPUT_OUTPUT );
}


FullscreenRectangle::FullscreenRectangle( Inst< SFNode      > _metadata,
                          Inst< SFBound     > _bound,
                          Inst< DisplayList > _displayList,
                          Inst< MFBool      > _isTouched,
                          Inst< MFVec3f     > _force,
                          Inst< MFVec3f     > _contactPoint,
                          Inst< MFVec3f     > _contactNormal,
                          Inst< SFFloat     > _zValue ):
  X3DGeometryNode( _metadata, _bound, _displayList, _isTouched,
                   _force, _contactPoint, _contactNormal ),
  zValue    ( _zValue     ) {

  type_name = "FullscreenRectangle";
  database.initFields( this );

  zValue->setValue( -1 );
  bound->setValue( new EmptyBound );

  zValue->route( displayList );
}

void FullscreenRectangle::render() {
  H3DFloat z_value = zValue->getValue();

  glMatrixMode (GL_MODELVIEW);
  glPushMatrix ();
  glLoadIdentity ();
  glMatrixMode (GL_PROJECTION);
  glPushMatrix ();
  glLoadIdentity ();
    
  glBegin( GL_QUADS );
  
  glNormal3f  ( 0, 0, 1 );
  renderTexCoordForActiveTexture( Vec3f( 1, 1, 0 ) );
  glVertex3f  ( 1, 1, z_value );
  renderTexCoordForActiveTexture( Vec3f( 0, 1, 0 ) );
  glVertex3f  ( -1, 1, z_value );
  renderTexCoordForActiveTexture( Vec3f( 0, 0, 0 ) );
  glVertex3f  ( -1, -1, z_value );
  renderTexCoordForActiveTexture( Vec3f( 1, 0, 0 ) );
  glVertex3f  ( 1, -1, z_value );
  
  glEnd();

  glPopMatrix ();
  glMatrixMode (GL_MODELVIEW);
  glPopMatrix ();
}



