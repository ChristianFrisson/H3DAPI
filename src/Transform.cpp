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
/// \file Transform.cpp
/// \brief CPP file for Transform, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/Transform.h>

using namespace H3D;

// Internal namespace for automatically generated code from Maple.
namespace TransformInternal {
 
  // The following functions have been automatically generated from Maple.
  // See maple/transform.mws. These are several functions for calculating
  // the transform matrix depending on what arguments have been changed.

  // Given all the values from the Transform node it returns the transformation
  // matrix in m.
  void fullTransform( const Vec3f &t, 
                      const Rotation &r,
                      const Vec3f &s,
                      const Rotation &so,
                      const Vec3f &c,
                      Matrix4f &m     ) {
    H3DFloat t1 = r.axis.x*r.axis.x;
    H3DFloat t2 = H3DCos(r.angle);
    H3DFloat t5 = t1+t2*(1.0f-t1);
    H3DFloat t6 = so.axis.x*so.axis.x;
    H3DFloat t7 = H3DCos(so.angle);
    H3DFloat t10 = t6+t7*(1.0f-t6);
    H3DFloat t13 = 1.0f-t2;
    H3DFloat t14 = r.axis.x*r.axis.y*t13;
    H3DFloat t15 = H3DSin(r.angle);
    H3DFloat t16 = r.axis.z*t15;
    H3DFloat t17 = t14-t16;
    H3DFloat t19 = 1.0f-t7;
    H3DFloat t20 = so.axis.x*so.axis.y*t19;
    H3DFloat t21 = H3DSin(so.angle);
    H3DFloat t22 = so.axis.z*t21;
    H3DFloat t23 = t20+t22;
    H3DFloat t26 = r.axis.x*r.axis.z*t13;
    H3DFloat t27 = r.axis.y*t15;
    H3DFloat t28 = t26+t27;
    H3DFloat t30 = so.axis.x*so.axis.z*t19;
    H3DFloat t31 = so.axis.y*t21;
    H3DFloat t32 = t30-t31;
    H3DFloat t35 = (t5*t10+t17*t23+t28*t32)*s.x;
    H3DFloat t37 = t20-t22;
    H3DFloat t39 = so.axis.y*so.axis.y;
    H3DFloat t42 = t39+t7*(1.0f-t39);
    H3DFloat t45 = so.axis.y*so.axis.z*t19;
    H3DFloat t46 = t21*so.axis.x;
    H3DFloat t47 = t45+t46;
    H3DFloat t50 = (t5*t37+t17*t42+t28*t47)*s.y;
    H3DFloat t52 = t30+t31;
    H3DFloat t54 = t45-t46;
    H3DFloat t56 = so.axis.z*so.axis.z;
    H3DFloat t59 = t56+t7*(1.0f-t56);
    H3DFloat t62 = (t5*t52+t17*t54+t28*t59)*s.z;
    H3DFloat t64 = t35*t10+t50*t37+t62*t52;
    H3DFloat t68 = t35*t23+t50*t42+t62*t54;
    H3DFloat t72 = t35*t32+t50*t47+t62*t59;
    H3DFloat t77 = t14+t16;
    H3DFloat t79 = r.axis.y*r.axis.y;
    H3DFloat t82 = t79+t2*(1.0f-t79);
    H3DFloat t85 = r.axis.y*r.axis.z*t13;
    H3DFloat t86 = t15*r.axis.x;
    H3DFloat t87 = t85-t86;
    H3DFloat t90 = (t77*t10+t82*t23+t87*t32)*s.x;
    H3DFloat t96 = (t77*t37+t82*t42+t87*t47)*s.y;
    H3DFloat t102 = (t77*t52+t82*t54+t87*t59)*s.z;
    H3DFloat t104 = t90*t10+t96*t37+t102*t52;
    H3DFloat t108 = t90*t23+t96*t42+t102*t54;
    H3DFloat t112 = t90*t32+t96*t47+t102*t59;
    H3DFloat t117 = t26-t27;
    H3DFloat t119 = t85+t86;
    H3DFloat t121 = r.axis.z*r.axis.z;
    H3DFloat t124 = t121+t2*(1.0f-t121);
    H3DFloat t127 = (t117*t10+t119*t23+t124*t32)*s.x;
    H3DFloat t133 = (t117*t37+t119*t42+t124*t47)*s.y;
    H3DFloat t139 = (t117*t52+t119*t54+t124*t59)*s.z;
    H3DFloat t141 = t127*t10+t133*t37+t139*t52;
    H3DFloat t145 = t127*t23+t133*t42+t139*t54;
    H3DFloat t149 = t127*t32+t133*t47+t139*t59;
    m[0][0] = t64;
    m[0][1] = t68;
    m[0][2] = t72;
    m[0][3] = -t64*c.x-t68*c.y-t72*c.z+c.x+t.x;
    m[1][0] = t104;
    m[1][1] = t108;
    m[1][2] = t112;
    m[1][3] = -t104*c.x-t108*c.y-t112*c.z+c.y+t.y;
    m[2][0] = t141;
    m[2][1] = t145;
    m[2][2] = t149;
    m[2][3] = -t141*c.x-t145*c.y-t149*c.z+c.z+t.z;
    m[3][0] = 0.0f;
    m[3][1] = 0.0f;
    m[3][2] = 0.0f;
    m[3][3] = 1.0f;
  }

  // Given all the values from the Transform node except the scaling paramaters 
  // it returns the transformation matrix in m.
  void transformNoScale( const Vec3f &t, 
                         const Rotation &r,
                         const Vec3f &c,
                         Matrix4f &m) {
    H3DFloat t1 = r.axis.x*r.axis.x;
    H3DFloat t2 = H3DCos(r.angle);
    H3DFloat t5 = t1+t2*(1.0f-t1);
    H3DFloat t7 = 1.0f-t2;
    H3DFloat t8 = r.axis.x*r.axis.y*t7;
    H3DFloat t9 = H3DSin(r.angle);
    H3DFloat t10 = r.axis.z*t9;
    H3DFloat t11 = t8-t10;
    H3DFloat t13 = r.axis.x*r.axis.z*t7;
    H3DFloat t14 = r.axis.y*t9;
    H3DFloat t15 = t13+t14;
    H3DFloat t20 = t8+t10;
    H3DFloat t21 = r.axis.y*r.axis.y;
    H3DFloat t24 = t21+t2*(1.0f-t21);
    H3DFloat t26 = r.axis.y*r.axis.z*t7;
    H3DFloat t27 = t9*r.axis.x;
    H3DFloat t28 = t26-t27;
    H3DFloat t33 = t13-t14;
    H3DFloat t34 = t26+t27;
    H3DFloat t35 = r.axis.z*r.axis.z;
    H3DFloat t38 = t35+t2*(1.0f-t35);
    m[0][0] = t5;
    m[0][1] = t11;
    m[0][2] = t15;
    m[0][3] = -t5*c.x-t11*c.y-t15*c.z+c.x+t.x;
    m[1][0] = t20;
    m[1][1] = t24;
    m[1][2] = t28;
    m[1][3] = -t20*c.x-t24*c.y-t28*c.z+c.y+t.y;
    m[2][0] = t33;
    m[2][1] = t34;
    m[2][2] = t38;
    m[2][3] = -t33*c.x-t34*c.y-t38*c.z+c.z+t.z;
    m[3][0] = 0.0f;
    m[3][1] = 0.0f;
    m[3][2] = 0.0f;
    m[3][3] = 1.0f;
  }

  // Given all the values from the Transform node, except the rotation
  // paramaters, it returns the transformation matrix in m.
  void transformNoRotation( const Vec3f &t, 
                            const Vec3f &s,
                            const Rotation &so,
                            const Vec3f &c,
                            Matrix4f &m) {
    H3DFloat t1 = so.axis.x*so.axis.x;
    H3DFloat t2 = H3DCos(so.angle);
    H3DFloat t5 = t1+t2*(1.0f-t1);
    H3DFloat t6 = t5*t5;
    H3DFloat t9 = 1.0f-t2;
    H3DFloat t10 = so.axis.x*so.axis.y*t9;
    H3DFloat t11 = H3DSin(so.angle);
    H3DFloat t12 = so.axis.z*t11;
    H3DFloat t13 = t10-t12;
    H3DFloat t14 = t13*t13;
    H3DFloat t17 = so.axis.x*so.axis.z*t9;
    H3DFloat t18 = so.axis.y*t11;
    H3DFloat t19 = t17+t18;
    H3DFloat t20 = t19*t19;
    H3DFloat t22 = t6*s.x+t14*s.y+t20*s.z;
    H3DFloat t23 = t5*s.x;
    H3DFloat t24 = t10+t12;
    H3DFloat t26 = t13*s.y;
    H3DFloat t27 = so.axis.y*so.axis.y;
    H3DFloat t30 = t27+t2*(1.0f-t27);
    H3DFloat t32 = t19*s.z;
    H3DFloat t34 = so.axis.y*so.axis.z*t9;
    H3DFloat t35 = t11*so.axis.x;
    H3DFloat t36 = t34-t35;
    H3DFloat t38 = t23*t24+t26*t30+t32*t36;
    H3DFloat t39 = t17-t18;
    H3DFloat t41 = t34+t35;
    H3DFloat t43 = so.axis.z*so.axis.z;
    H3DFloat t46 = t43+t2*(1.0f-t43);
    H3DFloat t48 = t23*t39+t26*t41+t32*t46;
    H3DFloat t53 = t24*t24;
    H3DFloat t55 = t30*t30;
    H3DFloat t57 = t36*t36;
    H3DFloat t59 = t53*s.x+t55*s.y+t57*s.z;
    H3DFloat t66 = t24*s.x*t39+t30*s.y*t41+t36*s.z*t46;
    H3DFloat t71 = t39*t39;
    H3DFloat t73 = t41*t41;
    H3DFloat t75 = t46*t46;
    H3DFloat t77 = t71*s.x+t73*s.y+t75*s.z;
    m[0][0] = t22;
    m[0][1] = t38;
    m[0][2] = t48;
    m[0][3] = -t22*c.x-t38*c.y-t48*c.z+c.x+t.x;
    m[1][0] = t38;
    m[1][1] = t59;
    m[1][2] = t66;
    m[1][3] = -t38*c.x-t59*c.y-t66*c.z+c.y+t.y;
    m[2][0] = t48;
    m[2][1] = t66;
    m[2][2] = t77;
    m[2][3] = -t48*c.x-t66*c.y-t77*c.z+c.z+t.z;
    m[3][0] = 0.0;
    m[3][1] = 0.0;
    m[3][2] = 0.0;
    m[3][3] = 1.0;
  }
  // Given a translation, the transformation matrix for it is returned
  // in m.
  void transformNoScaleOrRotation( const Vec3f &t, 
                                   Matrix4f &m) {
    m[0][0] = 1.0;
    m[0][1] = 0.0;
    m[0][2] = 0.0;
    m[0][3] = t.x;
    m[1][0] = 0.0;
    m[1][1] = 1.0;
    m[1][2] = 0.0;
    m[1][3] = t.y;
    m[2][0] = 0.0;
    m[2][1] = 0.0;
    m[2][2] = 1.0;
    m[2][3] = t.z;
    m[3][0] = 0.0;
    m[3][1] = 0.0;
    m[3][2] = 0.0;
    m[3][3] = 1.0;
  }
};

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase Transform::database( 
                                    "Transform", 
                                    &(newInstance<Transform>), 
                                    typeid( Transform ),
                                    &MatrixTransform::database );

namespace TransformInternals {
  FIELDDB_ELEMENT( Transform, center, INPUT_OUTPUT );
  FIELDDB_ELEMENT( Transform, rotation, INPUT_OUTPUT );
  FIELDDB_ELEMENT( Transform, scale, INPUT_OUTPUT );
  FIELDDB_ELEMENT( Transform, scaleOrientation, INPUT_OUTPUT );
  FIELDDB_ELEMENT( Transform, translation, INPUT_OUTPUT );
}


Transform::Transform( 
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
                     Inst< SFMatrix4f > _accumulatedInverse ) :
  MatrixTransform( _addChildren, _removeChildren, 
                   _children, _metadata, 
                   _bound, _bboxCenter, _bboxSize, 
                   _transformedBound, _matrix,
                   _accumulatedForward, _accumulatedInverse ),
  center          ( _center           ),
  rotation        ( _rotation         ),
  scale           ( _scale            ),
  scaleOrientation( _scaleOrientation ),
  translation     ( _translation      )  {

  type_name = "Transform";
  
  database.initFields( this );

  center->setValue( Vec3f( 0, 0, 0 ) ); 
  rotation->setValue( Rotation( 0, 0, 1, 0 ) );
  scale->setValue( Vec3f( 1, 1, 1 ) );
  scaleOrientation->setValue( Rotation( 0, 0, 1, 0 ) );
  translation->setValue( Vec3f( 0, 0, 0 ) );

  center->route( matrix );
  rotation->route( matrix );
  scale->route( matrix );
  scaleOrientation->route( matrix );
  translation->route( matrix );

  matrix->setAccessType( Field::OUTPUT_ONLY );

}

void Transform::Matrix::update() {
  Vec3f    c  = static_cast< SFVec3f *>(routes_in[0])->getValue();
  Rotation r  = static_cast< SFRotation *>(routes_in[1])->getValue();
  Vec3f    s  = static_cast< SFVec3f * >(routes_in[2])->getValue();
  Rotation so = static_cast< SFRotation * >(routes_in[3])->getValue();
  Vec3f    t  = static_cast< SFVec3f * >(routes_in[4])->getValue();

  // if any of the scaling components is 0 then the children should not
  // be rendered.
  if( H3DAbs( s.x ) < Constants::f_epsilon ||
      H3DAbs( s.y ) < Constants::f_epsilon ||
      H3DAbs( s.z ) < Constants::f_epsilon )
    return;

  // in order to save some computation we first check if we have a 
  // scaling or rotation component. If not we can skip the 
  // computations for that component.
  bool has_scaling = s != Vec3f( 1, 1, 1 );
  bool has_rotation = r.angle != 0;

  if( has_scaling ) {
    if( has_rotation ) {
      r.axis.normalizeSafe();
      TransformInternal::fullTransform( t, r, s, so, c, value );
    } else
      TransformInternal::transformNoRotation( t, s, so, c, value );
  } else {
    if( has_rotation ) {
      r.axis.normalizeSafe();
      TransformInternal::transformNoScale( t, r, c, value );
    } else
      TransformInternal::transformNoScaleOrRotation( t, value );
  }
}

void Transform::render() {
  Vec3f s = scale->getValue();
  if( H3DAbs( s.x ) > Constants::f_epsilon &&
      H3DAbs( s.y ) > Constants::f_epsilon &&
      H3DAbs( s.z ) > Constants::f_epsilon ){
    MatrixTransform::render(); 
  }
}



