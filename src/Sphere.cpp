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
#include <H3D/OpenHapticsOptions.h>

#include <H3D/X3DPointingDeviceSensorNode.h>
#include <H3D/HapticsRenderers.h>
#include <H3D/H3DHapticsDevice.h>

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
  solid   ( _solid    )
  {

  type_name = "Sphere";
  database.initFields( this );
  
  radius->setValue( 1 );
  solid->setValue( true );
    
  radius->route( bound );
  radius->route( displayList );
  solid->route( displayList );
}


void Sphere::render() {
  H3DFloat theta_parts = 50, phi_parts = 25;

  H3DFloat inc_theta = (H3DFloat) Constants::pi*2 / theta_parts;
  H3DFloat inc_phi =   (H3DFloat) Constants::pi / phi_parts;

  H3DFloat r = radius->getValue();
  
  H3DFloat double_pi = (H3DFloat) Constants::pi * 2;

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

void Sphere::traverseSG( TraverseInfo &ti ) {
  // we want to use a haptic sphere since this will be faster than 
  // using an hapticTriangleSet which is used in X3DGeometryNode::traverseSG.
  // It is at the moment implemented to not care about the openhaptics option
  // force full geometry render.
  if( ti.hapticsEnabled() && ti.getCurrentSurface() ) {
    const vector< H3DHapticsDevice * > &devices = ti.getHapticsDevices();

    for( unsigned int i = 0; i < devices.size(); i++ ) {
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
      }
      
      HAPI::HapticPrimitive * haptic_sphere = 
        new HAPI::HapticPrimitive(
                ti.getAccForwardMatrix(),
                new HAPI::Collision::Sphere( Vec3f( 0, 0, 0 ),
                                             radius->getValue() ),
                ti.getCurrentSurface()->getSurface(),
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
      ti.addHapticShape( i, haptic_sphere );

#ifdef HAVE_CHAI3D
      }
#endif
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
