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
/// \file Billboard.cpp
/// \brief CPP file for Billboard, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/Billboard.h>
#include <H3D/X3DViewpointNode.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase Billboard::database("Billboard", 
                                    &(newInstance<Billboard>), 
                                    typeid( Billboard ),
                                    &MatrixTransform::database );

namespace BillboardInternals {
  FIELDDB_ELEMENT( Billboard, axisOfRotation, INPUT_OUTPUT );
}

Billboard::Billboard( 
                     Inst< AddChildren        > _addChildren,
                     Inst< RemoveChildren     > _removeChildren,
                     Inst< MFChild            > _children,
                     Inst< SFNode             > _metadata,
                     Inst< SFBound            > _bound,
                     Inst< SFVec3f            > _bboxCenter,
                     Inst< SFVec3f            > _bboxSize,
                     Inst< SFTransformedBound > _transformedBound,
                     Inst< SFMatrix4f         > _matrix, 
                     Inst< SFVec3f            > _axisOfRotation ) :
  MatrixTransform( _addChildren, _removeChildren, _children, 
                   _metadata, _bound, _bboxCenter, _bboxSize,
                   _transformedBound, _matrix ),
  axisOfRotation( _axisOfRotation ),
  vpPosition( new SFVec3f ),
  vpUp( new SFVec3f ) {
  
  type_name = "Billboard";
  database.initFields( this );

  vpPosition->setOwner( this );
  vpUp->setOwner( this );
  vpPosition->setName( "vpPosition" );
  vpUp->setName( "vpUp" );
  
  axisOfRotation->setValue( Vec3f( 0, 1, 0 ) );
  vpPosition->setValue( Vec3f( 0, 0, 0 ) );
  vpUp->setValue( Vec3f( 0, 0, 0 ) );

  axisOfRotation->route( matrix );
  vpPosition->route( matrix );
  vpUp->route( matrix );
}

void Billboard::SFMatrix4f::update() {
  Vec3f axis_of_rotation = 
    static_cast< SFVec3f * >( routes_in[0] )->getValue();
  const Vec3f &vp_position = 
    static_cast< SFVec3f * >( routes_in[1] )->getValue();
  const Vec3f &vp_up = 
    static_cast< SFVec3f * >( routes_in[2] )->getValue();

  Vec3f billboard_to_viewer = vp_position;
  
  if( billboard_to_viewer * billboard_to_viewer > Constants::f_epsilon ) { 
    billboard_to_viewer.normalize();
    if( axis_of_rotation * axis_of_rotation > Constants::f_epsilon ) {
      axis_of_rotation.normalize();
      Vec3f z_axis( 0, 0, 1 );

      if( H3DAbs( axis_of_rotation * billboard_to_viewer -1) > 
          Constants::f_epsilon &&
          H3DAbs( z_axis * axis_of_rotation -1) > 
          Constants::f_epsilon &&
          H3DAbs( z_axis * billboard_to_viewer -1 ) >
          Constants::f_epsilon ) {
              
        // the normal of the plane defined by axis_of_rotation and 
        // billboard_to_viewer.
        Vec3f n1 = axis_of_rotation % billboard_to_viewer;
        n1.normalize();
        
        // the normal of the plane defined by axis_of_rotation and 
        // z_axos.
        Vec3f n2 = axis_of_rotation % z_axis;
        n2.normalize();
        
        H3DFloat angle = acos( n1 * n2 );
        
        if( n2 * billboard_to_viewer < 0 )
          angle = -angle;
        value = Matrix4f( Rotation( axis_of_rotation, angle ) );
      } else {
        value = Matrix4f();
      }
    } else {
      // axis_of_rotation == Vec3f(0,0,0) so this is the special case of 
      // viewer-alignment.
      Vec3f X = vp_up % billboard_to_viewer;
      value = Matrix4f( X.x, vp_up.x, billboard_to_viewer.x, 0,
                        X.y, vp_up.y, billboard_to_viewer.y, 0,
                        X.z, vp_up.z, billboard_to_viewer.z, 0,
                        0, 0, 0, 1);
    }
  }
}

void Billboard::traverseSG( TraverseInfo &ti ) {
  X3DViewpointNode *vp = X3DViewpointNode::getActive();
  Matrix4f vp_to_local;
  Vec3f vp_pos;
  Vec3f vp_y_axis;
  if( vp ) {
    vp_to_local = 
    ti.getAccInverseMatrix() *
    vp->accForwardMatrix->getValue();
  
    vp_pos = vp_to_local * vp->totalPosition->getValue();
    vp_y_axis = 
    vp_to_local.getScaleRotationPart() * 
    (vp->totalOrientation->getValue() *  Vec3f( 0, 1, 0 ) );
  }
  else {
    vp_to_local = 
    ti.getAccInverseMatrix();
  
    vp_pos = vp_to_local * Vec3f( 0, 0, 10 );
    vp_y_axis = 
    vp_to_local.getScaleRotationPart() * 
    Vec3f( 0, 1, 0 );
  }
  vp_y_axis.normalizeSafe();
  // set the vpPosition and vpUp fields if the value has changed.
  if( vp_pos != vpPosition->getValue() )
    vpPosition->setValue( vp_pos );
  if( vp_y_axis != vpUp->getValue() ) 
    vpUp->setValue( vp_y_axis );
  MatrixTransform::traverseSG( ti );
}

