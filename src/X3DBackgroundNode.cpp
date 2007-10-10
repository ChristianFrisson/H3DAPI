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
/// \file X3DBackgroundNode.cpp
/// \brief CPP file for X3DBackgroundNode, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/X3DBackgroundNode.h>
#include <assert.h>

using namespace H3D;


// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase X3DBackgroundNode::database( 
                           "X3DBackgroundNode", 
                           NULL, 
                           typeid( X3DBackgroundNode ),
                           &X3DBindableNode::database );

namespace X3DBackgroundNodeInternals {
  FIELDDB_ELEMENT( X3DBackgroundNode, groundAngle, INPUT_OUTPUT );
  FIELDDB_ELEMENT( X3DBackgroundNode, groundColor, INPUT_OUTPUT );
  FIELDDB_ELEMENT( X3DBackgroundNode, skyAngle, INPUT_OUTPUT );
  FIELDDB_ELEMENT( X3DBackgroundNode, skyColor, INPUT_OUTPUT );
}

X3DBackgroundNode::X3DBackgroundNode( Inst< SFSetBind > _set_bind,
                                      Inst< SFNode    > _metadata,
                                      Inst< SFTime    > _bindTime,
                                      Inst< SFBool    > _isBound,
                                      Inst< DisplayList > _displayList,
                                      Inst< MFFloat   > _groundAngle,
                                      Inst< MFColor   > _groundColor,
                                      Inst< MFFloat   > _skyAngle,
                                      Inst< MFColor   > _skyColor ) :
  X3DBindableNode( "X3DBackgroundNode", _set_bind, _metadata, _bindTime, _isBound ),
  displayList( _displayList ),
  groundAngle( _groundAngle ),
  groundColor( _groundColor ),
  skyAngle( _skyAngle ),
  skyColor( _skyColor ),
  localToGlobal( new SFMatrix4f ) {
  
  displayList->setOwner( this );
  displayList->setName( "displayList" );
  localToGlobal->setOwner( this );
  localToGlobal->setName( "localToGlobal" );

  type_name = "X3DBackgroundNode";
  database.initFields( this );

  skyColor->push_back( RGB( 0, 0, 0 ) );

  groundAngle->route( displayList );
  groundColor->route( displayList );
  skyAngle->route( displayList );
  skyColor->route( displayList );
  localToGlobal->route( displayList );
}

RGB getColor( H3DFloat theta,
              const vector< RGB > & color,
              const vector< H3DFloat > &angle ) {
  unsigned int i;
  for( i = 0; i < angle.size(); i++ ) {
    if( theta <= angle[i] ) {
      break;
    }  
  }
  if( i == angle.size() ) return color[i];

  H3DFloat weighting;
  if( i == 0 ) weighting = theta / angle[0];
  else weighting = (theta - angle[i-1]) / ( angle[i] - angle[i-1] );
  
  RGB c1 = color[i];
  RGB c2 = color[i+1];

  return RGB( c1.r + (c2.r - c1.r) * weighting,
              c1.g + (c2.g - c1.g) * weighting,
              c1.b + (c2.b - c1.b) * weighting );
}


void X3DBackgroundNode::render() {
  if( render_enabled ) {
    glPushAttrib( GL_ALL_ATTRIB_BITS );
    assert( skyColor->size() > skyAngle->size() );
    Vec3f c = Vec3f( 0, 0, 0 );
    int n = 40;
    H3DFloat r = (H3DFloat) 0.05;
  
    const vector< RGB > &sky_color = skyColor->getValue();
    const vector< H3DFloat > &sky_angle = skyAngle->getValue();
    const vector< RGB > &ground_color = groundColor->getValue();
    const vector< H3DFloat > &ground_angle = groundAngle->getValue();
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
  
    int i,j;
    H3DFloat theta1,theta2,theta3;
    Vec3f e,p;
    H3DFloat TWOPI = (H3DFloat) Constants::pi * 2;
    H3DFloat PID2 = (H3DFloat) Constants::pi / 2;

    if( skyColor->size() > 1 ) {
      // draw outer sphere( sky sphere ). Don't need to draw sphere unless
      // more than one skyColor. Buffer will be cleared with skyColor[0]
      // in GLWindow.cpp
      for (j=0;j<n/2;j++) {
        theta1 = j * TWOPI / n - PID2;
        theta2 = (j + 1) * TWOPI / n - PID2;
      
        glBegin(GL_QUAD_STRIP);
        for (i=0;i<=n;i++) {
          theta3 = i * TWOPI / n;
          e.x = H3DCos(theta2) * H3DCos(theta3);
          e.y = H3DSin(theta2);
          e.z = H3DCos(theta2) * H3DSin(theta3);
          p.x = c.x + r * e.x;
          p.y = c.y + r * e.y;
          p.z = c.z + r * e.z;
          glNormal3f(e.x,e.y,e.z);
          RGB color = getColor( (H3DFloat)(-(theta2 + PID2)+Constants::pi), 
                                sky_color, sky_angle );
          glColor3f( color.r, color.g, color.b );
          glVertex3f(p.x,p.y,p.z);
          e.x = cos(theta1) * cos(theta3);
          e.y = sin(theta1);
          e.z = cos(theta1) * sin(theta3);
          p.x = c.x + r * e.x;
          p.y = c.y + r * e.y;
          p.z = c.z + r * e.z;
          glNormal3f(e.x,e.y,e.z);
          color = getColor( (H3DFloat)( -(theta1 + PID2)+Constants::pi ),
                            sky_color, sky_angle );
          glColor3f( color.r, color.g, color.b );
          glVertex3f(p.x,p.y,p.z);
        }
        glEnd();
      }
    }

    if( ground_color.size() > 0 ) {
      // draw inner half sphere( ground sphere )
      r = r * (H3DFloat) 0.95;
      for (j=0;j<n/4;j++) {
        theta1 = j * TWOPI / n - PID2;
        theta2 = (j + 1) * TWOPI / n - PID2;
      
        glBegin(GL_QUAD_STRIP);
        for (i=0;i<=n;i++) {
          theta3 = i * TWOPI / n;
          e.x = cos(theta2) * cos(theta3);
          e.y = sin(theta2);
          e.z = cos(theta2) * sin(theta3);
          p.x = c.x + r * e.x;
          p.y = c.y + r * e.y;
          p.z = c.z + r * e.z;
          glNormal3f(e.x,e.y,e.z);
          RGB color =  getColor( theta2 + PID2 , ground_color, ground_angle );
          glColor3f( color.r, color.g, color.b );
          glVertex3f(p.x,p.y,p.z);
          e.x = cos(theta1) * cos(theta3);
          e.y = sin(theta1);
          e.z = cos(theta1) * sin(theta3);
          p.x = c.x + r * e.x;
          p.y = c.y + r * e.y;
          p.z = c.z + r * e.z;
          glNormal3f(e.x,e.y,e.z);
          color = getColor( theta1 + PID2, ground_color, ground_angle );
          glColor3f( color.r, color.g, color.b );
          glVertex3f(p.x,p.y,p.z);
        }
        glEnd();
      }
    }
    glMatrixMode( GL_MODELVIEW );
    glPopMatrix();
    glPopAttrib();
  }
}

