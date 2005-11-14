//////////////////////////////////////////////////////////////////////////////
//    Copyright 2004, SenseGraphics AB
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
/// \file SuperShape.h
/// \brief UNIMPLEMENTED: Header file for SuperShape, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __SUPERSHAPE_H__
#define __SUPERSHAPE_H__

#include <iostream>
#include "X3DGeometryNode.h"
#include "FieldTemplates.h"
#include "Exception.h"

/// Standard Template Library namespace
using namespace std;

/// H3D namespace
using namespace H3D;

/// Exception handling namespace
using namespace Exception;

/// H3D API namespace
namespace H3D {

  /// \ingroup Geometries
  /// \class SuperShape
  /// \brief The SuperShape node implements a SuperShape!
  class H3DAPI_API SuperShape : public X3DGeometryNode {
  public:
    
    SuperShape( Inst< SFNode   >  _metadata = 0,
                Inst< SFFloat  > _ss1_m  = 0,
                Inst< SFFloat  > _ss1_a  = 0,
                Inst< SFFloat  > _ss1_b  = 0,
                Inst< SFFloat  > _ss1_n1 = 0,
                Inst< SFFloat  > _ss1_n2 = 0,
                Inst< SFFloat  > _ss1_n3 = 0,
                Inst< SFFloat  > _ss2_m  = 0,
                Inst< SFFloat  > _ss2_a  = 0,
                Inst< SFFloat  > _ss2_b  = 0,
                Inst< SFFloat  > _ss2_n1 = 0,
                Inst< SFFloat  > _ss2_n2 = 0,
                Inst< SFFloat  > _ss2_n3 = 0,
                Inst< SFInt32  > _resolution = 0 );

    virtual void render();

    float R( float phi, float m, float a, float b, 
             float n1, float n2, float n3 ) {
      float t2 = m*phi/4.0f;
      float t3 = cosf(t2);
      float t6 = fabsf(t3/a);
      float t7 = powf(t6,n2);
      float t8 = sinf(t2);
      float t11 = fabsf(t8/b);
      float t12 = powf(t11,n3);
      float t14 = powf(t7+t12,-1/n1);
      return t14;
      //return powf(powf(fabsf(cosf((alpha*m)/4.)/a), n2) 
      //            + powf(fabsf(sinf((alpha*m)/4.)/b),n3), -1/n1);
    }
  
    Vec3f SS( float theta, float phi, 
              float sm, float sa, float sb, float sn1, float sn2, float sn3,
              float tm, float ta, float tb, float tn1, float tn2, float tn3 ) { 
      float r1theta = R( theta, sm, sa, sb, sn1, sn2, sn3 );
      float r2phi = R( phi, tm, ta, tb, tn1, tn2, tn3 );
    
      float x = r1theta * cosf(theta) * r2phi * cosf(phi);
      float y = r1theta * sin(theta) * r2phi * cosf(phi);
      float z = r2phi * sin(phi);

      return Vec3f( x, y, z);
    }

    Vec3f SSd( float theta, float phi, 
              float sm, float sa, float sb, float sn1, float sn2, float sn3,
              float tm, float ta, float tb, float tn1, float tn2, float tn3 ) { 
      float r1theta = R( theta, sm, sa, sb, sn1, sn2, sn3 );
      float r2phi = R( phi, tm, ta, tb, tn1, tn2, tn3 );
    
      float dx1 = cosf(theta)-r1theta*sinf(theta);
      float dy1 = sinf(theta)+r1theta*cosf(theta);

      float dy2 = cosf(phi)-r2phi*sinf(phi);
      float dz2 = sinf(phi)+r2phi*cosf(phi);
      //float z = r2phi * sin(phi);

      return Vec3f( dx1, dy1*dy2, dz2);
    }

    float SSdz( float theta, float phi, float m, float a, float b, 
                float n1, float n2, float n3 ) {
      return (4*n1*cosf(phi)*(powf(cosf((m*phi)/4.f)/a, n2) 
                              + powf(sinf((m*phi)/4.f)/b, n3)) 
              + m*sinf(phi)*(-(n3*(1/tanf((m*phi)/4.f))
                               *powf(sinf((m*phi)/4.f)/b, n3)) 
                             + n2*powf(cosf((m*phi)/4.f)/a, n2)
                             *tanf((m*phi)/4.f)))
        /(4.f*n1*powf(powf(cosf((m*phi)/4.f)/a, n2) 
                     + powf(sinf((m*phi)/4.f)/b, n3), (1 + n1)/n1));
    }
  
    /// Traverse the scenegraph. A HLFeedbackShape is added for haptic
    /// rendering if haptics is enabled.
    virtual void traverseSG( TraverseInfo &ti );  
    
    // Fields
    auto_ptr< SFFloat  > ss1_m;
    auto_ptr< SFFloat  > ss1_a;
    auto_ptr< SFFloat  > ss1_b;
    auto_ptr< SFFloat  > ss1_n1;
    auto_ptr< SFFloat  > ss1_n2;
    auto_ptr< SFFloat  > ss1_n3;

    auto_ptr< SFFloat  > ss2_m;
    auto_ptr< SFFloat  > ss2_a;
    auto_ptr< SFFloat  > ss2_b;
    auto_ptr< SFFloat  > ss2_n1;
    auto_ptr< SFFloat  > ss2_n2;
    auto_ptr< SFFloat  > ss2_n3;

    auto_ptr< SFInt32  > resolution;

    static H3DNodeDatabase database;
  };
}

#endif
