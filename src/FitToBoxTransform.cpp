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
/// \file FitToBoxTransform.cpp
/// \brief CPP file for FitToBoxTransform, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/FitToBoxTransform.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase FitToBoxTransform::database( "FitToBoxTransform", 
                                             &(newInstance<FitToBoxTransform>), 
                                             typeid( FitToBoxTransform ),
                                             &MatrixTransform::database );

namespace FitToBoxTransformInternals {
  FIELDDB_ELEMENT( FitToBoxTransform, boxSize, INPUT_OUTPUT );
  FIELDDB_ELEMENT( FitToBoxTransform, boxCenter, INPUT_OUTPUT );
  FIELDDB_ELEMENT( FitToBoxTransform, uniformScalingOnly, INPUT_OUTPUT );
  FIELDDB_ELEMENT( FitToBoxTransform, active, INPUT_OUTPUT );
}


FitToBoxTransform::FitToBoxTransform( 
                              Inst< AddChildren        > _addChildren,
                              Inst< RemoveChildren     > _removeChildren,
                              Inst< MFChild            > _children,
                              Inst< SFNode             > _metadata,
                              Inst< SFBound            > _bound,
                              Inst< SFVec3f            > _bboxCenter,
                              Inst< SFVec3f            > _bboxSize,
                              Inst< SFTransformedBound > _transformedBound,
                              Inst< SFMatrix4f         > _matrix,
                              Inst< SFVec3f            > _boxSize,
                              Inst< SFVec3f            > _boxCenter,
                              Inst< SFBool             > _uniformScalingOnly,
                              Inst< SFBool             > _active 
                              ) :
  MatrixTransform( _addChildren, _removeChildren, 
                   _children, _metadata, 
                   _bound, _bboxCenter, _bboxSize, 
                   _transformedBound, _matrix ),
  boxSize( _boxSize ),
  boxCenter( _boxCenter ),
  uniformScalingOnly( _uniformScalingOnly ),
  active( _active ) {

  type_name = "FitToBoxTransform";

  database.initFields( this );

  boxCenter->setValue( Vec3f( 0, 0, 0 ) );
  boxSize->setValue( Vec3f( 0.1f, 0.1f, 0.1f ) );
  uniformScalingOnly->setValue( false );
  active->setValue( true );

  boxSize->route( matrix );
  boxCenter->route( matrix );
  bound->route( matrix );
  uniformScalingOnly->route( matrix );
  active->route( matrix );
}

void FitToBoxTransform::SFMatrix4f::update() {
  const Vec3f& size = 
    static_cast< SFVec3f * >( routes_in[0] )->getValue();  
  const Vec3f& center = 
    static_cast< SFVec3f * >( routes_in[1] )->getValue();  
  Bound *b = static_cast< SFBound * >( routes_in[2] )->getValue();
  bool uniform_scaling = 
    static_cast< SFBool * >( routes_in[3] )->getValue();  
  bool active = 
    static_cast< SFBool * >( routes_in[4] )->getValue();  
  
  if( active && size * size > Constants::f_epsilon ) {
    BoxBound *bb = dynamic_cast< BoxBound * >( b );
    if( bb ) {
      const Vec3f &bound_size = bb->size->getValue();

      Vec3f scale( H3DAbs( bound_size.x ) > Constants::f_epsilon ? 
                   size.x / bound_size.x : 1,
                   H3DAbs( bound_size.y ) > Constants::f_epsilon ? 
                   size.y / bound_size.y : 1,
                   H3DAbs( bound_size.z ) > Constants::f_epsilon ? 
                   size.z / bound_size.z : 1
                   );
      if( uniform_scaling ) {
        H3DFloat min_scale = H3DMin( H3DMin( scale.x, scale.y ), scale.z );
        scale = Vec3f( min_scale, min_scale, min_scale );
      }
      const Vec3f &bound_center = bb->center->getValue();
      value = Matrix4f( scale.x, 0, 0, (scale.x * -bound_center.x) + center.x,
                        0, scale.y, 0, (scale.y * -bound_center.y) + center.y,
                        0, 0, scale.z, (scale.z * -bound_center.z) + center.z,
                        0, 0, 0, 1 );
    } else {
      value = Matrix4f();
    }
  }
  //cerr << value << endl;
}

