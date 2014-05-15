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
/// \file VisibilitySensor.cpp
/// \brief CPP file for VisibilitySensor, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/VisibilitySensor.h>

#include <GL/glew.h>

#ifdef MACOSX
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

using namespace H3D;

H3DNodeDatabase VisibilitySensor::database( 
        "VisibilitySensor", 
        &(newInstance<VisibilitySensor>), 
        typeid( VisibilitySensor ),
        &X3DEnvironmentalSensorNode::database 
        );

namespace VisibilitySensorInternals {
  FIELDDB_ELEMENT( VisibilitySensor, viewFrustumMode, INPUT_OUTPUT );
}
VisibilitySensor::VisibilitySensor( Inst< SFNode > _metadata ,
                                    Inst< SFVec3f > _center ,
                                    Inst< SFVec3f > _size ,
                                    Inst< SFTime > _enterTime ,
                                    Inst< SFTime > _exitTime ,
                                    Inst< SFBool > _enabled ,
                                    Inst< SFBool > _isActive,
                                    Inst< SFBool > _viewFrustumMode ) :
                                    X3DEnvironmentalSensorNode( _metadata, 
                                                                _center, 
                                                                _enabled, 
                                                                _size,
                                                                _enterTime,
                                                                _exitTime,
                                                                _isActive ),
                  viewFrustumMode(_viewFrustumMode),
                                    set_time( new SetTime ),
                                    queryId(0){

  type_name = "VisibilitySensor";
  database.initFields( this );

  set_time->setOwner( this );
  set_time->setName( "set_time" );
  isActive->route( set_time );

  viewFrustumMode->setValue( false );
  visib_pix_no_threshold = 10;
  prev_travinfoadr = 0;
  
}
void VisibilitySensor::traverseSG( TraverseInfo &ti ) {
 
  if( enabled->getValue() &&
    ( size->getValue().x >= 0.0 &&
      size->getValue().y >= 0.0 &&
      size->getValue().z >= 0.0 ))
  {
    
    if( prev_travinfoadr != &ti)
    {
      // First Instance DEF/USE of traveseSG
      prev_maxnoinstances = no_instance;
      vector<int>::iterator p;
      for( p = list.begin(); p != list.end(); ++p ) {
        if( *p > no_instance)
          p=list.erase( p );
      }
      no_instance = 0;
    }
    ++no_instance;
      
    const Matrix4f &vs_frw_m = ti.getAccForwardMatrix();

    H3DFloat xmin = center->getValue().x - size->getValue().x / 2.0f;
    H3DFloat xmax = center->getValue().x + size->getValue().x / 2.0f;
    H3DFloat ymin = center->getValue().y - size->getValue().y / 2.0f;
    H3DFloat ymax = center->getValue().y + size->getValue().y / 2.0f;
    H3DFloat zmin = center->getValue().z - size->getValue().z / 2.0f;
    H3DFloat zmax = center->getValue().z + size->getValue().z / 2.0f;
    
    Vec3f loc0(xmin, ymin, zmax);
    Vec3f loc1(xmax, ymin, zmax);
    Vec3f loc2(xmax, ymax, zmax);
    Vec3f loc3(xmin, ymax, zmax);
    Vec3f loc4(xmin, ymin, zmin);
    Vec3f loc5(xmax, ymin, zmin);
    Vec3f loc6(xmax, ymax, zmin);
    Vec3f loc7(xmin, ymax, zmin);

    // Global coordinates of the corners of visibilitySensor

    Vec3f g0 = vs_frw_m * loc0;
    Vec3f g1 = vs_frw_m * loc1;
    Vec3f g2 = vs_frw_m * loc2;
    Vec3f g3 = vs_frw_m * loc3;
    Vec3f g4 = vs_frw_m * loc4;
    Vec3f g5 = vs_frw_m * loc5;
    Vec3f g6 = vs_frw_m * loc6;
    Vec3f g7 = vs_frw_m * loc7;

    GLuint sampleCount;
    GLint available;
    GLint bitsSupported;

    // check to make sure functionality is supported
    glGetQueryiv(GL_SAMPLES_PASSED, GL_QUERY_COUNTER_BITS_ARB, 
      &bitsSupported);
    if (bitsSupported == 0) {
         cout<<"query check is not supported"<<endl;
      
    }

    if ( !glIsQuery( queryId ) )
      glGenQueriesARB(1, &queryId );
      
    // before this point, render major occluders
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    glDepthMask(GL_FALSE);

  bool depthTestEnabled = glIsEnabled(GL_DEPTH_TEST);
  if( viewFrustumMode->getValue() )
    glDisable(GL_DEPTH_TEST);
  
  // also disable texturing and any fancy shaders
    glBeginQueryARB(GL_SAMPLES_PASSED_ARB, queryId );
    // render bounding box for object i
    glBegin( GL_QUADS );
  
    // +z
    //glNormal3f  ( 0, 0, 1 );

    glVertex3f  (g2.x, g2.y,g2.z);
    glVertex3f  (g3.x, g3.y,g3.z);
    glVertex3f  (g0.x, g0.y,g0.z);
    glVertex3f  (g1.x, g1.y,g1.z);


    // -z
    //glNormal3f  ( 0, 0, -1 );

    glVertex3f  (g5.x, g5.y,g5.z);
    glVertex3f  (g4.x, g4.y,g4.z);
    glVertex3f  (g7.x, g7.y,g7.z);
    glVertex3f  (g6.x, g6.y,g6.z);

    // +y
    //glNormal3f  ( 0, 1, 0 );

    glVertex3f  (g2.x, g2.y,g2.z);
    glVertex3f  (g6.x, g6.y,g6.z);
    glVertex3f  (g7.x, g7.y,g7.z);
    glVertex3f  (g3.x, g3.y,g3.z);

    // -y
    //glNormal3f  ( 0, -1, 0 );

    glVertex3f  (g0.x, g0.y,g0.z);
    glVertex3f  (g4.x, g4.y,g4.z);
    glVertex3f  (g5.x, g5.y,g5.z);
    glVertex3f  (g1.x, g1.y,g1.z);

    // +x
    //glNormal3f  ( 1, 0, 0 );

    glVertex3f  (g2.x, g2.y,g2.z);
    glVertex3f  (g1.x, g1.y,g1.z);
    glVertex3f  (g5.x, g5.y,g5.z);
    glVertex3f  (g6.x, g6.y,g6.z);

    // -x
    //glNormal3f  ( -1, 0, 0 );

    glVertex3f  (g7.x, g7.y,g7.z);
    glVertex3f  (g4.x, g4.y,g4.z);
    glVertex3f  (g0.x, g0.y,g0.z);
    glVertex3f  (g3.x, g3.y,g4.z);


    glEnd();
    
    glEndQueryARB(GL_SAMPLES_PASSED_ARB);
      
    glFlush();

    do {
      glGetQueryObjectivARB(queryId,
                GL_QUERY_RESULT_AVAILABLE_ARB,
                &available);
    } while (!available);

    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glDepthMask(GL_TRUE);

  if( viewFrustumMode->getValue() && depthTestEnabled )
    glEnable(GL_DEPTH_TEST);
      
    glGetQueryObjectuivARB(queryId, GL_QUERY_RESULT_ARB,
                &sampleCount);

  
    if( (int)sampleCount > visib_pix_no_threshold )
    {
      vector<int>::iterator p = find(list.begin(), list.end(), 
        no_instance );
      if ( p == list.end() ) {
        if( list.size() == 0 ) isActive->setValue( true, id );
            list.push_back( no_instance );
      }
    }
    else
    {
      vector<int>::iterator p = find(list.begin(), list.end(), 
        no_instance );
      if ( p != list.end() ) {
            list.erase( p );
          if( list.size() == 0 ) isActive->setValue( false, id );
      }
    }

    prev_travinfoadr = &ti;
  
  }
}
