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
/// \file Background.cpp
/// \brief CPP file for Background, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/Background.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase Background::database( 
                           "Background", 
                           &(newInstance< Background >), 
                           typeid( Background ),
                           &X3DBackgroundNode::database );

namespace BackgroundInternals {
  FIELDDB_ELEMENT( Background, backUrl, INPUT_OUTPUT );
  FIELDDB_ELEMENT( Background, frontUrl, INPUT_OUTPUT );
  FIELDDB_ELEMENT( Background, leftUrl, INPUT_OUTPUT );
  FIELDDB_ELEMENT( Background, rightUrl, INPUT_OUTPUT );
  FIELDDB_ELEMENT( Background, topUrl, INPUT_OUTPUT );
  FIELDDB_ELEMENT( Background, bottomUrl, INPUT_OUTPUT );
}

Background::Background( Inst< SFSetBind > _set_bind,
                        Inst< SFNode    > _metadata,
                        Inst< SFTime    > _bindTime,
                        Inst< SFBool    > _isBound,
                        Inst< DisplayList > _displayList,
                        Inst< MFFloat   > _groundAngle,
                        Inst< MFColor   > _groundColor,
                        Inst< MFFloat   > _skyAngle,
                        Inst< MFColor   > _skyColor,
                        Inst< MFString  > _backUrl,
                        Inst< MFString  > _frontUrl,
                        Inst< MFString  > _leftUrl,
                        Inst< MFString  > _rightUrl,
                        Inst< MFString  > _topUrl,
                        Inst< MFString  > _bottomUrl ) :
  X3DBackgroundNode( _set_bind, _metadata, _bindTime, _isBound, _displayList,
                   _groundAngle, _groundColor, _skyAngle, _skyColor ),
  backUrl( _backUrl ),
  frontUrl( _frontUrl ),
  leftUrl( _leftUrl ),
  rightUrl( _rightUrl ),
  topUrl( _topUrl ),
  bottomUrl( _bottomUrl ),
  backTexture( new ImageTexture ),
  frontTexture( new ImageTexture ),
  leftTexture( new ImageTexture ),
  rightTexture( new ImageTexture ),
  topTexture( new ImageTexture ),
  bottomTexture( new ImageTexture ) {

  type_name = "Background";
  database.initFields( this );

  backUrl->route( backTexture->url );
  frontUrl->route( frontTexture->url );
  leftUrl->route( leftTexture->url );
  rightUrl->route( rightTexture->url );
  topUrl->route( topTexture->url );
  bottomUrl->route( bottomTexture->url );

  backTexture->repeatS->setValue( false );
  frontTexture->repeatS->setValue( false );
  leftTexture->repeatS->setValue( false );
  rightTexture->repeatS->setValue( false );
  topTexture->repeatS->setValue( false );
  bottomTexture->repeatS->setValue( false );

  backTexture->repeatT->setValue( false );
  frontTexture->repeatT->setValue( false );
  leftTexture->repeatT->setValue( false );
  rightTexture->repeatT->setValue( false );
  topTexture->repeatT->setValue( false );
  bottomTexture->repeatT->setValue( false );

  backTexture->displayList->route( displayList );
  frontTexture->displayList->route( displayList );
  leftTexture->displayList->route( displayList );
  rightTexture->displayList->route( displayList );
  topTexture->displayList->route( displayList );
  bottomTexture->displayList->route( displayList );
}

void Background::render() {
  if( render_enabled ) {
    X3DBackgroundNode::render();
    glPushAttrib( GL_ALL_ATTRIB_BITS );
    H3DFloat s = (H3DFloat)0.05;
    glCullFace( GL_BACK );
    glEnable( GL_CULL_FACE );
    glDisable( GL_LIGHTING );

    Matrix3f m = localToGlobal->getValue().getRotationPart();

    GLfloat mv[] = { 
      m[0][0], m[1][0], m[2][0], 0,
      m[0][1], m[1][1], m[2][1], 0,
      m[0][2], m[1][2], m[2][2], 0,
      0, 0, 0, 1 };
  
    glMatrixMode( GL_MODELVIEW );
    glPushMatrix();
    glMultMatrixf( mv );
    glColor4f( 1, 1, 1, 1 );
    if( frontUrl->size() > 0 ) {
      frontTexture->preRender();
      frontTexture->displayList->callList();
      glBegin( GL_QUADS );
      glNormal3f( 0, 0, 1 );
      glTexCoord2f( 1, 1 );
      glVertex3f( s, s, -s );
      glTexCoord2f( 0, 1 );
      glVertex3f( -s, s, -s );
      glTexCoord2f( 0, 0 );
      glVertex3f( -s, -s, -s );
      glTexCoord2f( 1, 0 );
      glVertex3f( s, -s, -s );      
      glEnd();
      frontTexture->postRender();
    }
  
    if( backUrl->size() > 0 ) {
      backTexture->preRender();
      backTexture->displayList->callList();
      glBegin( GL_QUADS );
      glNormal3f( 0, 0, -1 );
      glTexCoord2f( 0, 0 );
      glVertex3f( s, -s, s );
      glTexCoord2f( 1, 0 );
      glVertex3f( -s, -s, s );
      glTexCoord2f( 1, 1 );
      glVertex3f( -s, s, s );
      glTexCoord2f( 0, 1 );
      glVertex3f( s, s, s );
      glEnd();
      backTexture->postRender();
    }

    if( leftUrl->size() > 0 ) {
      leftTexture->preRender();
      leftTexture->displayList->callList();
      glBegin( GL_QUADS );
      glNormal3f( 1, 0, 0 );
      glTexCoord2f( 0, 1 );
      glVertex3f( -s, s, s );
      glTexCoord2f( 0, 0 );
      glVertex3f( -s, -s, s );
      glTexCoord2f( 1, 0 );
      glVertex3f( -s, -s, -s );
      glTexCoord2f( 1, 1 );
      glVertex3f( -s, s, -s );
      glEnd();
      leftTexture->postRender();
    }

    if( rightUrl->size() > 0 ) {
      rightTexture->preRender();
      rightTexture->displayList->callList();
      glBegin( GL_QUADS );
      glNormal3f( -1, 0, 0 );
      glTexCoord2f( 0, 1 );
      glVertex3f( s, s, -s );
      glTexCoord2f( 0, 0 );
      glVertex3f( s, -s, -s );
      glTexCoord2f( 1, 0 );
      glVertex3f( s, -s, s );
      glTexCoord2f( 1, 1 );
      glVertex3f( s, s, s );
      glEnd();
      rightTexture->postRender();
    }

    if( topUrl->size() > 0 ) {
      topTexture->preRender();
      topTexture->displayList->callList();
      glBegin( GL_QUADS );
      glNormal3f( 0, -1, 0 );
      glTexCoord2f( 0, 1 );
      glVertex3f( -s, s, s );
      glTexCoord2f( 0, 0 );
      glVertex3f( -s, s, -s );
      glTexCoord2f( 1, 0 );
      glVertex3f( s, s, -s );
      glTexCoord2f( 1, 1 );
      glVertex3f( s, s, s );
      glEnd();
      topTexture->postRender();
    }

    if( bottomUrl->size() > 0 ) {
      bottomTexture->preRender();
      bottomTexture->displayList->callList();
      glBegin( GL_QUADS );
      glNormal3f( 0, 1, 0 );
      glTexCoord2f( 1, 0 );
      glVertex3f( s, -s, s );
      glTexCoord2f( 1, 1 );
      glVertex3f( s, -s, -s );
      glTexCoord2f( 0, 1 );
      glVertex3f( -s, -s, -s );
      glTexCoord2f( 0, 0 );
      glVertex3f( -s, -s, s );
      glEnd();
      bottomTexture->postRender();
    }


    glMatrixMode( GL_MODELVIEW );
    glPopMatrix(); 
    glPopAttrib();
  }
}












