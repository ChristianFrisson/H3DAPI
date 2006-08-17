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
/// \file SuperShape.cpp
/// \brief CPP file for SuperShape, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include "SuperShape.h"

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase SuperShape::database( 
                                     "SuperShape", 
                                     &(newInstance<SuperShape>), 
                                     typeid( SuperShape ) );

namespace SuperShapeInternals {
  FIELDDB_ELEMENT( SuperShape, metadata, INPUT_OUTPUT );
  FIELDDB_ELEMENT( SuperShape, ss1_m, INPUT_OUTPUT );
  FIELDDB_ELEMENT( SuperShape, ss1_a, INPUT_OUTPUT );
  FIELDDB_ELEMENT( SuperShape, ss1_b, INPUT_OUTPUT );
  FIELDDB_ELEMENT( SuperShape, ss1_n1, INPUT_OUTPUT );
  FIELDDB_ELEMENT( SuperShape, ss1_n2, INPUT_OUTPUT );
  FIELDDB_ELEMENT( SuperShape, ss1_n3, INPUT_OUTPUT );
  FIELDDB_ELEMENT( SuperShape, ss2_m, INPUT_OUTPUT );
  FIELDDB_ELEMENT( SuperShape, ss2_a, INPUT_OUTPUT );
  FIELDDB_ELEMENT( SuperShape, ss2_b, INPUT_OUTPUT );
  FIELDDB_ELEMENT( SuperShape, ss2_n1, INPUT_OUTPUT );
  FIELDDB_ELEMENT( SuperShape, ss2_n2, INPUT_OUTPUT );
  FIELDDB_ELEMENT( SuperShape, ss2_n3, INPUT_OUTPUT );
  FIELDDB_ELEMENT( SuperShape, resolution, INPUT_OUTPUT );
}


SuperShape::SuperShape( 
                       Inst< SFNode >  _metadata,
                       Inst< SFFloat  > _ss1_m,
                       Inst< SFFloat  > _ss1_a,
                       Inst< SFFloat  > _ss1_b,
                       Inst< SFFloat  > _ss1_n1,
                       Inst< SFFloat  > _ss1_n2,
                       Inst< SFFloat  > _ss1_n3,

                       Inst< SFFloat  > _ss2_m,
                       Inst< SFFloat  > _ss2_a,
                       Inst< SFFloat  > _ss2_b,
                       Inst< SFFloat  > _ss2_n1,
                       Inst< SFFloat  > _ss2_n2,
                       Inst< SFFloat  > _ss2_n3,
                       Inst< SFInt32  > _resolution ) : 
  X3DGeometryNode( _metadata ),
  ss1_m ( _ss1_m  ), ss1_a ( _ss1_a  ), ss1_b ( _ss1_b  ),
  ss1_n1( _ss1_n1 ), ss1_n2( _ss1_n2 ), ss1_n3( _ss1_n3 ),
  ss2_m ( _ss2_m  ), ss2_a ( _ss2_a  ), ss2_b ( _ss2_b  ),
  ss2_n1( _ss2_n1 ), ss2_n2( _ss2_n2 ), ss2_n3( _ss2_n3 ),
  resolution( _resolution ) {

  //metadata =  NULL  [X3DMetadataObject];
  //size =      2 2 2 (0,);
  //solid =     TRUE;

  //bound->setValue( new BoxBound );

  ss1_m->setValue ( 4 );
  ss1_a->setValue ( 1 );
  ss1_b->setValue ( 1 );
  ss1_n1->setValue( 100 );
  ss1_n2->setValue( 100 );
  ss1_n3->setValue( 100 );

  ss2_m->setValue ( 4 );
  ss2_a->setValue ( 1 );
  ss2_b->setValue ( 1 );
  ss2_n1->setValue( 100 );
  ss2_n2->setValue( 100 );
  ss2_n3->setValue( 100 );

  resolution->setValue( 128 );

  ss1_m->route( displayList );
  ss1_a->route( displayList );
  ss1_b->route( displayList );
  ss1_n1->route( displayList );
  ss1_n2->route( displayList );
  ss1_n3->route( displayList );
   
  ss2_m->route( displayList );
  ss2_a->route( displayList );
  ss2_b->route( displayList );
  ss2_n1->route( displayList );
  ss2_n2->route( displayList );
  ss2_n3->route( displayList );
   
  resolution->setValue( 128 );
   
  type_name = "SuperShape";
  ss1_m->setName ( "SuperShape.ss1_m" );
  ss1_a->setName ( "SuperShape.ss1_a" );
  ss1_b->setName ( "SuperShape.ss1_b" );
  ss1_n1->setName( "SuperShape.ss1_n1" );
  ss1_n2->setName( "SuperShape.ss1_n2" );
  ss1_n3->setName( "SuperShape.ss1_n3" );

  ss2_m->setName ( "SuperShape.ss2_m" );
  ss2_a->setName ( "SuperShape.ss2_a" );
  ss2_b->setName ( "SuperShape.ss2_b" );
  ss2_n1->setName( "SuperShape.ss2_n1" );
  ss2_n2->setName( "SuperShape.ss2_n2" );
  ss2_n3->setName( "SuperShape.ss2_n3" );

  resolution->setName( "SuperShape.resolution" );

}

void SuperShape::render() {
  X3DGeometryNode::render();

  useBackFaceCulling( true );
  //glEnable( GL_COLOR_MATERIAL );

  float long_step =  2 * 3.14159f / resolution->getValue();
  float lat_step = 3.14159f / resolution->getValue();
  Vec3f vmax = Vec3f( 0, 0, 0 );
  Vec3f vmin = Vec3f( 0, 0, 0 );
  
  for(int lat_count = 0; lat_count < resolution->getValue(); lat_count++ )
    {
      float phi1 = static_cast<float>(lat_count * lat_step - (3.14159/2));
      float phi2 = phi1 + lat_step;
      float phi3 = phi2 + lat_step;
      float phi0 = phi1 - lat_step;

      glBegin( GL_TRIANGLE_STRIP );
      Vec3f fa;
      Vec3f fc;
      for(int long_count = 0;long_count<=resolution->getValue(); long_count++) {
        float theta1 = static_cast<float>(long_count * long_step - 3.14159);
        float theta2 = theta1 + long_step;
        float theta0 = theta1 - long_step;
        Vec3f p10 = SS( theta1,phi0,
                        ss1_m->getValue(),  ss1_a->getValue(),  
                        ss1_b->getValue(),  ss1_n1->getValue(), 
                        ss1_n2->getValue(), ss1_n3->getValue(), 
                        ss2_m->getValue(),  ss2_a->getValue(),  
                        ss2_b->getValue(),  ss2_n1->getValue(), 
                        ss2_n2->getValue(), ss2_n3->getValue() );

        Vec3f p01 = SS( theta0,phi1,
                        ss1_m->getValue(),  ss1_a->getValue(),  
                        ss1_b->getValue(),  ss1_n1->getValue(), 
                        ss1_n2->getValue(), ss1_n3->getValue(), 
                        ss2_m->getValue(),  ss2_a->getValue(),  
                        ss2_b->getValue(),  ss2_n1->getValue(), 
                        ss2_n2->getValue(), ss2_n3->getValue());
        Vec3f p11 = SS(theta1,phi1,
                       ss1_m->getValue(),  ss1_a->getValue(),  
                       ss1_b->getValue(),  ss1_n1->getValue(), 
                       ss1_n2->getValue(), ss1_n3->getValue(), 
                       ss2_m->getValue(),  ss2_a->getValue(),  
                       ss2_b->getValue(),  ss2_n1->getValue(), 
                       ss2_n2->getValue(), ss2_n3->getValue());
        Vec3f p21 = SS(theta2,phi1,
                       ss1_m->getValue(),  ss1_a->getValue(),  ss1_b->getValue(),
                       ss1_n1->getValue(), ss1_n2->getValue(), ss1_n3->getValue(), 
                       ss2_m->getValue(),  ss2_a->getValue(),  ss2_b->getValue(),
                       ss2_n1->getValue(), ss2_n2->getValue(), ss2_n3->getValue());

        Vec3f p02 = SS(theta0,phi2,
                       ss1_m->getValue(),  ss1_a->getValue(),  ss1_b->getValue(),
                       ss1_n1->getValue(), ss1_n2->getValue(), ss1_n3->getValue(), 
                       ss2_m->getValue(),  ss2_a->getValue(),  ss2_b->getValue(),
                       ss2_n1->getValue(), ss2_n2->getValue(), ss2_n3->getValue());
        Vec3f p12 = SS(theta1,phi2,
                       ss1_m->getValue(),  ss1_a->getValue(),  ss1_b->getValue(),
                       ss1_n1->getValue(), ss1_n2->getValue(), ss1_n3->getValue(), 
                       ss2_m->getValue(),  ss2_a->getValue(),  ss2_b->getValue(),
                       ss2_n1->getValue(), ss2_n2->getValue(), ss2_n3->getValue());
        Vec3f p22 = SS(theta2,phi2,
                       ss1_m->getValue(),  ss1_a->getValue(),  ss1_b->getValue(),
                       ss1_n1->getValue(), ss1_n2->getValue(), ss1_n3->getValue(), 
                       ss2_m->getValue(),  ss2_a->getValue(),  ss2_b->getValue(),
                       ss2_n1->getValue(), ss2_n2->getValue(), ss2_n3->getValue());

        Vec3f p13 = SS(theta1,phi3,
                       ss1_m->getValue(),  ss1_a->getValue(),  ss1_b->getValue(),
                       ss1_n1->getValue(), ss1_n2->getValue(), ss1_n3->getValue(), 
                       ss2_m->getValue(),  ss2_a->getValue(),  ss2_b->getValue(),
                       ss2_n1->getValue(), ss2_n2->getValue(), ss2_n3->getValue());
            
        Vec3f n;
        n =  (p12-p02).crossProduct(p12-p11);
        n += (p12-p11).crossProduct(p12-p22);
        n += (p12-p22).crossProduct(p12-p13);
        n += (p12-p13).crossProduct(p12-p02);
        n.normalizeSafe(); glNormal3f( n.x, n.y, n.z );
        //n = Vec3f( theta2, phi1, theta1 ); n.normalize(); glColor3f( n.x, n.y, n.z );
        glVertex3f( p12.x, p12.y, p12.z );
      
        n =  (p11-p01).crossProduct(p11-p10);
        n += (p11-p10).crossProduct(p11-p21);
        n += (p11-p21).crossProduct(p11-p12);
        n += (p11-p12).crossProduct(p11-p01);
      
        n.normalizeSafe(); glNormal3f( n.x, n.y, n.z );
        //n = Vec3f( theta1, phi1, theta2 ); n.normalize(); glColor3f( n.x, n.y, n.z );
        glVertex3f( p11.x, p11.y, p11.z );
      
        if ( p11.x > vmax.x ) vmax.x = p11.x;
        if ( p11.y > vmax.y ) vmax.y = p11.y;
        if ( p11.z > vmax.z ) vmax.z = p11.z;
        if ( p12.x > vmax.x ) vmax.x = p12.x;
        if ( p12.y > vmax.y ) vmax.y = p12.y;
        if ( p12.z > vmax.z ) vmax.z = p12.z;

        if ( p11.x < vmin.x ) vmin.x = p11.x;
        if ( p11.y < vmin.y ) vmin.y = p11.y;
        if ( p11.z < vmin.z ) vmin.z = p11.z;
        if ( p12.x < vmin.x ) vmin.x = p12.x;
        if ( p12.y < vmin.y ) vmin.y = p12.y;
        if ( p12.z < vmin.z ) vmin.z = p12.z;
      }
    
      glEnd();

      // update boxbound
      BoxBound *bb = new BoxBound;
      Vec3f half = Vec3f( (vmax.x+vmin.x)/2,
                          (vmax.y+vmin.y)/2,
                          (vmax.z+vmin.z)/2 );
      bb->center->setValue( half );
      Vec3f size = Vec3f( vmax.x-vmin.x, vmax.y-vmin.y, vmax.z-vmin.z );
      bb->size->setValue( size );
      bound->setValue( bb );
    }
}

#ifdef USE_HAPTICS
void SuperShape::traverseSG( TraverseInfo &ti ) {
  if( ti.hapticsEnabled() && ti.getCurrentSurface() ) {
#ifdef HAVE_OPENHAPTICS
    ti.addHapticShapeToAll( getOpenGLHapticShape( 
                                                 ti.getCurrentSurface(),
                                                 ti.getAccForwardMatrix() ) );
#endif
  }
}
#endif
