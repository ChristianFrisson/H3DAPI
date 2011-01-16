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
/// \file HAnimJoint.cpp
/// \brief CPP file for HAnimJoint, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/HAnimJoint.h>

#ifdef MACOSX
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#ifdef FREEGLUT
#include <GL/freeglut.h>
#endif

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase HAnimJoint::database( 
                                    "HAnimJoint", 
                                    &(newInstance<HAnimJoint>), 
                                    typeid( HAnimJoint ),
                                    &Transform::database );

namespace HAnimJointInternals {
  FIELDDB_ELEMENT( HAnimJoint, displacers, INPUT_OUTPUT );
  FIELDDB_ELEMENT( HAnimJoint, limitOrientation, INPUT_OUTPUT );
  FIELDDB_ELEMENT( HAnimJoint, llimit, INPUT_OUTPUT );
  FIELDDB_ELEMENT( HAnimJoint, ulimit, INPUT_OUTPUT );
  FIELDDB_ELEMENT( HAnimJoint, name, INPUT_OUTPUT );
  FIELDDB_ELEMENT( HAnimJoint, skinCoordIndex, INPUT_OUTPUT );
  FIELDDB_ELEMENT( HAnimJoint, skinCoordWeight, INPUT_OUTPUT );
  FIELDDB_ELEMENT( HAnimJoint, stiffness, INPUT_OUTPUT );
}


HAnimJoint::HAnimJoint( 
                     Inst< AddChildren > _addChildren,
                     Inst< RemoveChildren > _removeChildren,
                     Inst< SFVec3f    > _center,
                     Inst< MFChild    > _children,
                     Inst< SFNode     > _metadata,
                     Inst< SFRotation > _rotation,
                     Inst< SFVec3f    > _scale,
                     Inst< SFRotation > _scaleOrientation,
                     Inst< SFVec3f    > _translation,
                     Inst< SFBound    > _bound,
                     Inst< SFVec3f    > _bboxCenter,
                     Inst< SFVec3f    > _bboxSize,
                     Inst< SFTransformedBound > _transformedBound,
                     Inst< Matrix > _matrix,
                     Inst< SFMatrix4f > _accumulatedForward,
                     Inst< SFMatrix4f > _accumulatedInverse,
		     Inst< MFDisplacer > _displacers,
		     Inst< SFRotation  > _limitOrientation,
		     Inst< MFFloat     > _llimit,
		     Inst< MFFloat     > _ulimit,
		     Inst< SFString    > _name,
		     Inst< MFInt32     > _skinCoordIndex,
		     Inst< MFFloat     > _skinCoordWeight,
		     Inst< MFFloat     > _stiffness ) :
  Transform( _addChildren, _removeChildren, 
	     _center, _children, _metadata, 
	     _rotation, _scale, _scaleOrientation, _translation,
	     _bound, _bboxCenter, _bboxSize, 
	     _transformedBound, _matrix,
	     _accumulatedForward, _accumulatedInverse ),
  displacers       ( _displacers ),
  limitOrientation ( _limitOrientation ),
  llimit           ( _llimit            ),
  ulimit           ( _ulimit ),
  name             ( _name      ),
  skinCoordIndex   ( _skinCoordIndex ),
  skinCoordWeight  ( _skinCoordWeight ),
  stiffness        ( _stiffness ) {

  type_name = "HAnimJoint";
  database.initFields( this );

  limitOrientation->setValue( Rotation( 0, 0, 1, 0 ) );
  stiffness->push_back( 0 );
  stiffness->push_back( 0 );
  stiffness->push_back( 0 );
}

//void HAnimJoint::render() {
//}

void HAnimJoint::traverseSG( TraverseInfo &ti ) {
  ti.pushMatrices( matrix->getValue(),
                   matrix->getValue().inverse() );

  // set accumulated forward and inverse matrices
  accumulatedForward->setValue( ti.getAccForwardMatrix(), id );
  accumulatedInverse->setValue( ti.getAccInverseMatrix(), id );
  const NodeVector &c = children->getValue();
  for( unsigned int i = 0; i < c.size(); i++ ) {
    HAnimJoint *joint = dynamic_cast< HAnimJoint* >( c[i]);
    if( joint ) joint->traverseSG( ti );
  }
  ti.popMatrices();
  

}


void HAnimJoint::renderSkeleton( RenderType type ) {
  // render sphere at joint center
  glPushMatrix();
  const Vec3f &ctr = center->getValue();
  glTranslatef( ctr.x, ctr.y,ctr.z );
  glutSolidSphere( 0.01, 20, 20 );    
  glPopMatrix();

  // render children
  glMatrixMode( GL_MODELVIEW );
  glPushMatrix();
  multiplyGLMatrix();
 
  const NodeVector &c = children->getValue();
  for( unsigned int i = 0; i < c.size(); i++ ) {
    HAnimJoint *joint = dynamic_cast< HAnimJoint* >( c[i]);
    if( joint ) joint->renderSkeleton( type );
  }    
  glPopMatrix();

  // render lines if in skeleton mode
  if( type == HAnimJoint::SKELETON ) {
    glPushAttrib( GL_LIGHTING_BIT );
    glDisable( GL_LIGHTING );
    glColor3f( 1, 1, 1 );
    
    glBegin( GL_LINES );
    for( unsigned int i = 0; i < c.size(); i++ ) {
      HAnimJoint *joint = dynamic_cast< HAnimJoint* >( c[i]);
      if( joint ) {
	Vec3f joint_center = matrix->getValue() * joint->center->getValue();
	glVertex3f( ctr.x, ctr.y, ctr.z );
	glVertex3f( joint_center.x, joint_center.y, joint_center.z );
      } 
    }    
    glEnd();
    glPopAttrib();
  } 
}

