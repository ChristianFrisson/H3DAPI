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
/// \file Sphere.cpp
/// \brief CPP file for Sphere, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/Sphere.h>
#include <H3D/H3DSurfaceNode.h>

#include <H3D/HapticsOptions.h>
#include <H3D/GlobalSettings.h>

#include <H3D/HapticsRenderers.h>
#include <H3D/H3DHapticsDevice.h>
#include <H3D/ShadowSphere.h>
#include <H3D/GraphicsCachingOptions.h>

// HAPI includes
#include <HAPI/HapticPrimitive.h>

#ifdef HAVE_OPENHAPTICS
#include <HAPI/OpenHapticsRenderer.h>
#endif

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase Sphere::database( 
                                 "Sphere", 
                                 &(newInstance<Sphere>), 
                                 typeid( Sphere ),
                                 &X3DGeometryNode::database );

namespace SphereInternals {
  FIELDDB_ELEMENT( Sphere, radius, INPUT_OUTPUT );
  FIELDDB_ELEMENT( Sphere, solid, INPUT_OUTPUT );
}


Sphere::Sphere( 
               Inst<    SFNode >  _metadata,
               Inst< SFBound >                 _bound,
               Inst< SFFloat>  _radius,
               Inst< SFBool >  _solid ) :
  X3DGeometryNode( _metadata, _bound ),
  radius  ( _radius   ),
  solid   ( _solid    ),
  vboFieldsUpToDate( new Field ),
  vbo_id( NULL ) {

  type_name = "Sphere";
  database.initFields( this );

  radius->setValue( 1 );
  solid->setValue( true );

  radius->route( bound );
  radius->route( displayList );
  solid->route( displayList );

  vboFieldsUpToDate->setName( "vboFieldsUpToDate" );
  radius->route( vboFieldsUpToDate );
}

Sphere::~Sphere() {
  // Delete buffer if it was allocated.
  if( vbo_id ) {
    glDeleteBuffersARB( 2, vbo_id );
    delete [] vbo_id;
    vbo_id = NULL;
  }
}

H3DShadowObjectNode *Sphere::getShadowObject() {
  ShadowSphere *shadow = new ShadowSphere();
  shadow->radius->setValue( radius->getValue() );
  return shadow;
}

void Sphere::render() {
  bool prefer_vertex_buffer_object = false;
  if( GLEW_ARB_vertex_buffer_object ) {
    GraphicsCachingOptions * gco = NULL;
    getOptionNode( gco );
    if( !gco ) {
      GlobalSettings * gs = GlobalSettings::getActive();
      if( gs ) {
        gs->getOptionNode( gco );
      }
    }
    if( gco ) {
      prefer_vertex_buffer_object = gco->preferVertexBufferObject->getValue();
    }
  }

  H3DFloat theta_parts = 50, phi_parts = 25;

  H3DFloat inc_theta = (H3DFloat) Constants::pi*2 / theta_parts;
  H3DFloat inc_phi = (H3DFloat) Constants::pi / phi_parts;

  H3DFloat r = radius->getValue();

  H3DFloat double_pi = (H3DFloat) Constants::pi * 2;

  if( prefer_vertex_buffer_object ) {
    // Use vertex buffer objects to create sphere.
    if( !vboFieldsUpToDate->isUpToDate() ) {
      vboFieldsUpToDate->upToDate();
      // Only create and transfer data when it has been modified.
      unsigned int nr_data_vertices = 9; // 9  floats/vertex.
      GLsizei data_size = 
        (GLsizei)( ( theta_parts + 1 ) * (phi_parts+1) * nr_data_vertices );
      GLfloat * sphere_data = new GLfloat[data_size];
      unsigned int nr_index_data = 6; // 2 triangles, 3 vertices/triangle.
      GLsizei index_data_size =
        (GLsizei)(theta_parts * phi_parts * nr_index_data);
      GLuint * sphere_index_data = new GLuint[index_data_size];
      // Iterate through the parts to create vertices.
      for (unsigned int p = 0; p <= phi_parts; p++ ) {
        for (unsigned int t = 0; t <= theta_parts; t++ ) {
          H3DFloat phi = p * inc_phi;
          bool at_seam = t == theta_parts;
          H3DFloat theta = ( at_seam ? 0 :t * inc_theta );

          H3DFloat x, y, z;

          x = - H3DSin( phi ) * H3DSin( theta );
          y = H3DCos( phi );
          z = - H3DSin( phi ) * H3DCos( theta );

          unsigned int vert_index =
            (unsigned int)( p * ( theta_parts + 1 ) + t );
          unsigned int base_data_index =
          (unsigned int)( vert_index * nr_data_vertices );
          // Vertex
          sphere_data[ base_data_index ] = x * r;
          sphere_data[ base_data_index + 1 ] = y * r;
          sphere_data[ base_data_index + 2 ] = z * r;
          // Normal
          sphere_data[ base_data_index + 3 ] = x;
          sphere_data[ base_data_index + 4 ] = y;
          sphere_data[ base_data_index + 5 ] = z;
          // Texture coordinate
          sphere_data[ base_data_index + 6 ] =
            at_seam ? 1 : (GLfloat) (theta / double_pi);
          sphere_data[ base_data_index + 7 ] =
            (GLfloat) (1 - phi/ Constants::pi);
          sphere_data[ base_data_index + 8 ] = 0;

          if( !at_seam && p != phi_parts ) {
            // Create indices to define triangles.
            // If the entire point grid is unfolded onto a flat map then
            // the triangles created here can be seen as the ones connecting
            // this point to the three point to the east, south and south-east.
            // This is the reason why no indices are added for the triangles
            // at the end of the loop
            // (there are no more points to the east and south).
            // First triangle.
            unsigned int base_index =
              (unsigned int)( ( p * theta_parts + t ) * nr_index_data );
            sphere_index_data[ base_index ] = vert_index;
            sphere_index_data[ base_index + 1 ] =
              vert_index + (GLuint)theta_parts + 1;
            sphere_index_data[ base_index + 2 ] = vert_index + 1;

            // Second triangle.
            sphere_index_data[ base_index + 3 ] = vert_index + 1;
            sphere_index_data[ base_index + 4 ] =
              vert_index + (GLuint)theta_parts + 1;
            sphere_index_data[ base_index + 5 ] =
              vert_index + (GLuint)theta_parts + 2;
          }
        }
      }

      if( !vbo_id ) {
        vbo_id = new GLuint[2];
        glGenBuffersARB( 2, vbo_id );
      }
      glBindBufferARB( GL_ARRAY_BUFFER_ARB, vbo_id[0] );
      glBufferDataARB( GL_ARRAY_BUFFER_ARB,
                       data_size * sizeof(GLfloat),
                       sphere_data, GL_STATIC_DRAW_ARB );
      glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, vbo_id[1] );
      glBufferDataARB( GL_ELEMENT_ARRAY_BUFFER_ARB,
                           index_data_size * sizeof(GLuint),
                           sphere_index_data, GL_STATIC_DRAW_ARB );
      delete [] sphere_data;
      delete [] sphere_index_data;
    } else {
      glBindBufferARB( GL_ARRAY_BUFFER_ARB, vbo_id[0] );
      glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, vbo_id[1] );
    }

    // Enable all states for vertex buffer objects.
    // Note that the data is interleaved since this supposedly should be
    // faster on some systems.
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 9 * sizeof(GLfloat), NULL );
    glEnableClientState(GL_NORMAL_ARRAY);
    glNormalPointer( GL_FLOAT, 9 * sizeof(GLfloat),
                     (GLvoid*)(3*sizeof(GLfloat)) );
    X3DTextureCoordinateNode::renderVertexBufferObjectForActiveTexture(
      3, GL_FLOAT, 9 * sizeof(GLfloat), (GLvoid*)(6*sizeof(GLfloat)) );

    // Draw the triangles
    glDrawRangeElements( GL_TRIANGLES,
                         0,
                       (GLsizei)(( theta_parts + 1 ) * ( phi_parts + 1 ) - 1),
                       (GLsizei)(theta_parts * phi_parts * 6),
                         GL_UNSIGNED_INT,
                         NULL );

    // Disable state.
    X3DTextureCoordinateNode::disableVBOForActiveTexture();
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    glBindBufferARB( GL_ARRAY_BUFFER_ARB, 0 );
    glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, 0 );
  } else {

    // Draw sphere using glBegin and quads. Faster if stored
    // in displaylist.
    glBegin( GL_QUADS );

    for (unsigned int p = 0; p < phi_parts; p++ ) {
      for (unsigned int t = 0; t < theta_parts; t++ ) {
        H3DFloat phi = p * inc_phi;
        H3DFloat theta = t * inc_theta;
        H3DFloat next_phi = phi + inc_phi;
        bool at_seam = t == theta_parts - 1;
        H3DFloat next_theta = ( at_seam ? 0 :theta + inc_theta );

        H3DFloat x, y, z;

        x = - H3DSin( phi ) * H3DSin( theta );
        y = H3DCos( phi );
        z = - H3DSin( phi ) * H3DCos( theta );

        glNormal3f( x, y, z );
        renderTexCoordForActiveTexture( 
                            Vec3f( (H3DFloat) (theta / double_pi), 
                                   (H3DFloat) (1 - phi/ Constants::pi),
                                   0 ) );
        glVertex3f( x * r, y * r, z * r );

        x = - H3DSin( next_phi ) * H3DSin( theta );
        y = H3DCos( next_phi );
        z = - H3DSin( next_phi ) * H3DCos( theta );

        glNormal3f( x, y, z );
        renderTexCoordForActiveTexture( 
                            Vec3f( (H3DFloat) (theta / double_pi), 
                                   (H3DFloat) (1 - next_phi/ Constants::pi ),
                                   0 ) );
        glVertex3f( x * r, y * r, z * r );

        x = - H3DSin( next_phi ) * H3DSin( next_theta );
        y = H3DCos( next_phi );
        z = - H3DSin( next_phi ) * H3DCos( next_theta );

        glNormal3f( x, y, z );
        renderTexCoordForActiveTexture( 
                       Vec3f( at_seam ? 1 : (H3DFloat) (next_theta / double_pi ), 
                              (H3DFloat)(1 - next_phi/ Constants::pi),
                              0 ) );
        glVertex3f( x * r, y * r, z * r );

        x = - H3DSin( phi ) * H3DSin( next_theta );
        y = H3DCos( phi );
        z = - H3DSin( phi ) * H3DCos( next_theta );

        glNormal3f( x, y, z );
        renderTexCoordForActiveTexture( 
                     Vec3f( at_seam ? 1 : (H3DFloat)(next_theta / double_pi), 
                            (H3DFloat)(1 - phi/ Constants::pi),
                            0 ) );
        glVertex3f( x * r, y * r, z * r );
      }
    }

    glEnd();
  }
} 

void Sphere::traverseSG( TraverseInfo &ti ) {
  // we want to use a haptic sphere since this will be faster than 
  // using an hapticTriangleSet which is used in X3DGeometryNode::traverseSG.
  // It is at the moment implemented to not care about the openhaptics option
  // force full geometry render.
  if( ti.getCurrentSurface() ) {
    const vector< H3DHapticsDevice * > &devices = ti.getHapticsDevices();

    for( unsigned int i = 0; i < devices.size(); i++ ) {
      if( ti.hapticsEnabled(i) ) { 
      H3DHapticsDevice *hd = devices[i];

      H3DHapticsRendererNode* hd_renderer = hd->hapticsRenderer->getValue();

#ifdef HAVE_CHAI3D
      if( dynamic_cast< Chai3DRenderer * >( hd_renderer ) ) {
        X3DGeometryNode::createAndAddHapticShapes( ti, hd, i, 0 );
      } else {
#endif

      HapticsOptions *haptics_options = NULL;
      getOptionNode( haptics_options );

      HAPI::Collision::FaceType touchable_face;

      if( usingCulling() ) {
        if( getCullFace() == GL_FRONT ) touchable_face = HAPI::Collision::BACK;
        else touchable_face = HAPI::Collision::FRONT;
      } else {
        touchable_face = HAPI::Collision::FRONT_AND_BACK;
      }

      if( !haptics_options ) {
        GlobalSettings *default_settings = GlobalSettings::getActive();
        if( default_settings ) {
          default_settings->getOptionNode( haptics_options );
        }
      }
      
      string dynamic_mode = "TRANSFORM_CHANGED";

      if( haptics_options ) {
        const string &face = haptics_options->touchableFace->getValue();
        if( face == "FRONT" ) touchable_face = HAPI::Collision::FRONT;
        else if( face == "BACK" ) touchable_face = HAPI::Collision::BACK;
        else if( face == "FRONT_AND_BACK" ) 
          touchable_face = HAPI::Collision::FRONT_AND_BACK;
        else if( face == "AS_GRAPHICS" ) {
          // default values are the same as graphics
        } else {
          Console(4) << "Warning: Invalid default touchable face: "
            << face 
            << ". Must be \"FRONT\", \"BACK\" or \"FRONT_AND_BACK\" "
            << "(in active HapticsOptions node\" )" << endl;
        }

        dynamic_mode = haptics_options->dynamicMode->getValue();
      }
      
      HAPI::HapticPrimitive * haptic_sphere = 
        new HAPI::HapticPrimitive(
                ti.getAccForwardMatrix(),
                new HAPI::Collision::Sphere( Vec3f( 0, 0, 0 ),
                                             radius->getValue() ),
                ti.getCurrentSurface()->getSurface(i),
                touchable_face,
                this,
                -1,
                &X3DGeometryNode::cleanUpFunction );
      ref();

#ifdef HAVE_OPENHAPTICS
      if( dynamic_cast< OpenHapticsRenderer * >( hd_renderer ) ) {
      // Even for openhaptics we want to use the HAPI::HapticPrimitive
      // class with a HAPI::Collision::Sphere in it for haptic
      // rendering. In this case without calling some glRender function of
      // HAPI::HapticPrimitive. Cleaning up of the options node is taken care
      // of by an AutoPtrVector.
      // forcing full geometry rendering for openhaptics will not be used
      // for Sphere.
      haptic_sphere->addRenderOption(
        new HAPI::OpenHapticsRenderer::OpenHapticsOptions( 
          HAPI::OpenHapticsRenderer::OpenHapticsOptions::CUSTOM,
          false,
          false ) );
      }
#endif

      addDynamicInfoToShape( ti, dynamic_mode, haptic_sphere );
      ti.addHapticShape( i, haptic_sphere );

#ifdef HAVE_CHAI3D
      }
#endif
      }
    }
  }

  // use backface culling if solid is true
  if( solid->getValue() ) useBackFaceCulling( true );
  else useBackFaceCulling( false );
}

bool Sphere::lineIntersect(
                  const Vec3f &from, 
                  const Vec3f &to,    
                  LineIntersectResult &result ) {

  IntersectionInfo temp_result;
  HAPI::Collision::Sphere temp_sphere( Vec3f( 0.0f, 0.0f, 0.0f ),
                                       radius->getValue() );
  bool returnValue =
    temp_sphere.lineIntersect( from, to, temp_result );
  if( returnValue ) {
    temp_result.primitive = 0;
    result.addResults( temp_result, this );
    result.addPtDevMap();
  }
  return returnValue;
}

void Sphere::closestPoint( const Vec3f &p,
                           NodeIntersectResult &result ) {
  Vec3d temp_closest_point, temp_normal, temp_tex_coord;
  HAPI::Collision::Sphere temp_sphere( Vec3f( 0.0f, 0.0f, 0.0f ),
                                    radius->getValue() );
  temp_sphere.closestPoint( p, temp_closest_point, 
                            temp_normal, temp_tex_coord );
  IntersectionInfo temp_result;
  temp_result.point = temp_closest_point;
  temp_result.normal = temp_normal;
  temp_result.tex_coord = temp_tex_coord;
  result.addResults( temp_result, this );
}

bool Sphere::movingSphereIntersect( H3DFloat _radius,
                                    const Vec3f &from, 
                                    const Vec3f &to,
                                    NodeIntersectResult &result ) {
  HAPI::Collision::Sphere temp_sphere( Vec3f( 0.0f, 0.0f, 0.0f ),
                                    radius->getValue() );
  IntersectionInfo temp_result;
  bool return_value = temp_sphere.movingSphereIntersect( _radius,
                                                         from,
                                                         to,
                                                         temp_result );
  if( return_value ) {
    temp_result.primitive = 0;
    result.addResults( temp_result, this );
  }
  return return_value;
}
