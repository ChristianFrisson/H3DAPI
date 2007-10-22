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
/// \file H3DHapticsDevice.cpp
/// \brief cpp file for H3DHapticsDevice
///
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/H3DHapticsDevice.h>
#include <H3D/X3DGeometryNode.h>
#include <H3D/DeviceInfo.h>
#include <H3D/X3DViewpointNode.h>

#include <OpenHapticsRenderer.h>
#include <HAPI/GodObjectRenderer.h>
#include <HAPI/PhantomHapticsDevice.h>
#include <HAPI/HAPIHapticsRenderer.h>

using namespace H3D;

H3DNodeDatabase H3DHapticsDevice::database( "H3DHapticsDevice", 
                                         NULL,
                                         typeid( H3DHapticsDevice ) ); 

namespace H3DHapticsDeviceInternals {
  FIELDDB_ELEMENT( H3DHapticsDevice, initialized, OUTPUT_ONLY );
  FIELDDB_ELEMENT( H3DHapticsDevice, enabled, OUTPUT_ONLY );
  FIELDDB_ELEMENT( H3DHapticsDevice, set_enabled, INPUT_ONLY );
  FIELDDB_ELEMENT( H3DHapticsDevice, deviceOrientation, OUTPUT_ONLY );
  FIELDDB_ELEMENT( H3DHapticsDevice, devicePosition, OUTPUT_ONLY );
  FIELDDB_ELEMENT( H3DHapticsDevice, trackerPosition, OUTPUT_ONLY );
  FIELDDB_ELEMENT( H3DHapticsDevice, trackerOrientation, OUTPUT_ONLY );
  FIELDDB_ELEMENT( H3DHapticsDevice, positionCalibration, INPUT_OUTPUT );
  FIELDDB_ELEMENT( H3DHapticsDevice, orientationCalibration, INPUT_OUTPUT );
  FIELDDB_ELEMENT( H3DHapticsDevice, proxyPosition, OUTPUT_ONLY );
  FIELDDB_ELEMENT( H3DHapticsDevice, weightedProxyPosition, OUTPUT_ONLY );
  FIELDDB_ELEMENT( H3DHapticsDevice, proxyWeighting, INPUT_OUTPUT );
  FIELDDB_ELEMENT( H3DHapticsDevice, mainButton, OUTPUT_ONLY );
  FIELDDB_ELEMENT( H3DHapticsDevice, secondaryButton, OUTPUT_ONLY );
  FIELDDB_ELEMENT( H3DHapticsDevice, buttons, OUTPUT_ONLY );
  FIELDDB_ELEMENT( H3DHapticsDevice, force, OUTPUT_ONLY );
  FIELDDB_ELEMENT( H3DHapticsDevice, torque, OUTPUT_ONLY );
  FIELDDB_ELEMENT( H3DHapticsDevice, inputDOF, OUTPUT_ONLY );
  FIELDDB_ELEMENT( H3DHapticsDevice, outputDOF, OUTPUT_ONLY );
  FIELDDB_ELEMENT( H3DHapticsDevice, hapticsRate, OUTPUT_ONLY );
  FIELDDB_ELEMENT( H3DHapticsDevice, stylus, INPUT_OUTPUT );
  FIELDDB_ELEMENT( H3DHapticsDevice, hapticsRenderer, INPUT_OUTPUT );
  FIELDDB_ELEMENT( H3DHapticsDevice, proxyPositions, INPUT_OUTPUT );
  FIELDDB_ELEMENT( H3DHapticsDevice, followViewpoint, INPUT_OUTPUT );
  FIELDDB_ELEMENT( H3DHapticsDevice, deviceVelocity, OUTPUT_ONLY );
}


/// Constructor.
H3DHapticsDevice::H3DHapticsDevice( 
               Inst< SFVec3f         > _devicePosition,
               Inst< SFRotation      > _deviceOrientation      ,
               Inst< TrackerPosition > _trackerPosition        ,
               Inst< TrackerOrientation > _trackerOrientation  ,
               Inst< PosCalibration  > _positionCalibration    ,
               Inst< OrnCalibration  > _orientationCalibration ,
               Inst< SFVec3f         > _proxyPosition          ,
               Inst< WeightedProxy   > _weightedProxyPosition  ,     
               Inst< SFFloat         > _proxyWeighting         ,
               Inst< MainButton      > _mainButton             ,
               Inst< SecondaryButton > _secondaryButton       ,
               Inst< SFInt32         > _buttons                ,
               Inst< SFVec3f         > _force                  ,
               Inst< SFVec3f         > _torque                 ,
               Inst< SFInt32         > _inputDOF               ,
               Inst< SFInt32         > _outputDOF              ,
               Inst< SFInt32         > _hapticsRate            ,
               Inst< SFNode          > _stylus                 ,
               Inst< SFHapticsRendererNode > _hapticsRenderer,
               Inst< MFVec3f         > _proxyPositions,
               Inst< SFBool          > _followViewpoint,
               Inst< SFVec3f         > _deviceVelocity ):
  devicePosition( _devicePosition ),
  deviceOrientation( _deviceOrientation ),
  trackerPosition( _trackerPosition ),
  trackerOrientation( _trackerOrientation ),
  positionCalibration( _positionCalibration ),
  orientationCalibration( _orientationCalibration ),
  proxyPosition( _proxyPosition ),
  weightedProxyPosition( _weightedProxyPosition ),
  proxyWeighting( _proxyWeighting ),
  mainButton( _mainButton ),
  secondaryButton( _secondaryButton ),
  buttons( _buttons ),
  force( _force ),
  torque( _torque ),
  inputDOF( _inputDOF ),
  outputDOF( _outputDOF ),
  followViewpoint( _followViewpoint ),
  deviceVelocity( _deviceVelocity ),
  hapticsRate( _hapticsRate ),
  hapticsLoopTime( new SFTime ),
  stylus( _stylus ),
  initialized( new SFBool ),
  hapticsRenderer( _hapticsRenderer ),
  proxyPositions( _proxyPositions ),
  set_enabled( new SetEnabled ),
  enabled( new SFBool ),
  adjustedPositionCalibration( new PosCalibration ),
  adjustedOrnCalibration( new OrnCalibration ),
  error_msg_printed( false ) {

  type_name = "H3DHapticsDevice";  
  database.initFields( this );

  followViewpoint->setValue( true );

  initialized->setValue( false, id );
  enabled->setValue( false, id );
  proxyWeighting->setValue( 0.95f );
  buttons->setValue( 0, id );
  hapticsRate->setValue( 0, id );

  positionCalibration->route( trackerPosition, id );
  devicePosition->route( trackerPosition, id );

  orientationCalibration->route( trackerOrientation, id );
  deviceOrientation->route( trackerOrientation, id );
  
  proxyPosition->route( weightedProxyPosition, id );
  trackerPosition->route( weightedProxyPosition, id );
  proxyWeighting->route( weightedProxyPosition, id );

  buttons->route( mainButton, id );
  buttons->route( secondaryButton, id );

  vp_initialized = false;
}

H3DHapticsDevice::ErrorCode H3DHapticsDevice::enableDevice() {
  if( hapi_device.get() ) {
    HAPI::HAPIHapticsDevice::ErrorCode e =
      hapi_device->enableDevice();
    if( e == HAPI::HAPIHapticsDevice::SUCCESS && !enabled->getValue() ) {
      enabled->setValue( true, id );
    }
    return e;
  }
  else return HAPI::HAPIHapticsDevice::NOT_INITIALIZED;
}

H3DHapticsDevice::ErrorCode H3DHapticsDevice::disableDevice() {
  if( hapi_device.get() ) {
    HAPI::HAPIHapticsDevice::ErrorCode e =
      hapi_device->disableDevice();
    if( e == HAPI::HAPIHapticsDevice::SUCCESS && enabled->getValue() ) {
      enabled->setValue( false, id );
    }
    return e;
  }
  else return HAPI::HAPIHapticsDevice::NOT_INITIALIZED;
}



H3DHapticsDevice::ErrorCode H3DHapticsDevice::initDevice() {
  if( !initialized->getValue() ) {
    if( hapi_device.get() ) {
      HAPI::HAPIHapticsDevice::ErrorCode e = hapi_device->initDevice();
      if( e == HAPI::HAPIHapticsDevice::SUCCESS ) {
        hapi_device->enableDevice();
        initialized->setValue( true, id );
      } else {
        if( !error_msg_printed ) {
          Console(4) << hapi_device->getLastErrorMsg() << endl;
          error_msg_printed = true;
        }
      }
      return e;
    } else {
      return HAPI::HAPIHapticsDevice::FAIL;
    }
  } else {
    return HAPI::HAPIHapticsDevice::SUCCESS;
  }
}

H3DHapticsDevice::ErrorCode H3DHapticsDevice::releaseDevice() {
  initialized->setValue( false, id );
  if( hapi_device.get() ) {
    hapi_device->disableDevice();
    return hapi_device->releaseDevice();
  } else {
    return HAPI::HAPIHapticsDevice::FAIL;
  }
}

void H3DHapticsDevice::renderEffects( 
                         const HapticEffectVector &effects ) {
  TimeStamp now_time;
  H3DTime dt = now_time - last_effect_change;
  last_effect_change = now_time;
  if( hapi_device.get() )
    hapi_device->setEffects( effects, dt );
}

void H3DHapticsDevice::renderShapes( 
                         const HapticShapeVector &shapes,
                         unsigned int layer ) {
  if( hapi_device.get() ) {

    HAPI::HAPIHapticsRenderer *hapi_renderer = 
      hapi_device->getHapticsRenderer( layer );

    if( !hapi_renderer ) {
      H3DHapticsRendererNode *h3d_renderer = hapticsRenderer->getValue();
      if( h3d_renderer ) {
        hapi_device->setHapticsRenderer( 
                                  h3d_renderer->getHapticsRenderer( layer ),
                                  layer );
      }
    }
    hapi_device->setShapes( shapes, layer );
    hapi_device->transferObjects();
  }
}

void H3DHapticsDevice::updateDeviceValues() {
   previuos_proxy_pos = proxyPosition->getValue();
  TimeStamp now = TimeStamp();
  TimeStamp dt = now - last_update_values;
  last_update_values = now;

  if( hapi_device.get() ) {
    H3DInt32 hr = hapi_device->getHapticsRate();
    hapticsRate->setValue( hr, id );
    hapticsLoopTime->setValue( hapi_device->getTimeSpentInLastLoop() ,id );
    HAPI::HAPIHapticsDevice::DeviceValues dv = 
      hapi_device->getRawDeviceValues();
    // convert to metres
    devicePosition->setValue( (Vec3f)dv.position * 1e-3, id);
    deviceOrientation->setValue( dv.orientation, id);
    force->setValue( (Vec3f)dv.force, id);
    torque->setValue( (Vec3f)dv.torque, id);
    buttons->setValue( dv.button_status, id );

    X3DViewpointNode *vp = X3DViewpointNode::getActive();
    if( followViewpoint->getValue() && vp ) {
      // Haptic device should follow the viewpoint.

      const Matrix4f vp_accFrw = vp->accForwardMatrix->getValue();

      if( !vp_initialized ) {
        // Store default matrices for viewpoint following.
        Vec3f default_vp_pos = vp_accFrw * vp->getFullPos();
        default_vp_pos_mtx[0][3] = default_vp_pos.x;
        default_vp_pos_mtx[1][3] = default_vp_pos.y;
        default_vp_pos_mtx[2][3] = default_vp_pos.z;

        default_vp_orn_mtx = vp_accFrw.inverse();
        vp_initialized = true;
      }

      // create matrix for new point
      Vec3f vp_full_pos = vp_accFrw * vp->getFullPos();
      Matrix4f translation_matrix_new;
      translation_matrix_new[0][3] = vp_full_pos.x;
      translation_matrix_new[1][3] = vp_full_pos.y;
      translation_matrix_new[2][3] = vp_full_pos.z;

      // create rotation matrix.
      Matrix4f vp_full_orn_mtx = vp_accFrw * Matrix4f( vp->getFullOrn() );
      Matrix4f rotation_matrix = vp_full_orn_mtx * default_vp_orn_mtx;

      // create the matrix used to adjust the positionCalibration
      Matrix4f adjust_matrix = translation_matrix_new *
        ( rotation_matrix * default_vp_pos_mtx.inverse() );

      adjustedPositionCalibration->
        setValue( adjust_matrix * positionCalibration->getValue() );

      hapi_device->setPositionCalibration( Matrix4f( 1e3, 0, 0, 0,
                                                     0, 1e3, 0, 0,
                                                     0, 0, 1e3, 0,
                                                     0, 0, 0, 1 ) *
                            adjustedPositionCalibration->rt_pos_calibration *
                                           Matrix4f( 1e-3f, 0, 0, 0,
                                                     0, 1e-3f, 0, 0,
                                                     0, 0, 1e-3f, 0,
                                                     0, 0, 0, 1 ) );
      
      // Create adjusted OrnCalibration and send to HAPI
      adjustedOrnCalibration->setValue(
        Rotation( vp_full_orn_mtx.getScaleRotationPart() ) *
        orientationCalibration->getValue() );
      hapi_device->
        setOrientationCalibration(adjustedOrnCalibration->rt_orn_calibration );
    }
    else {
      hapi_device->
        setPositionCalibration( Matrix4f( 1e3, 0, 0, 0,
                                          0, 1e3, 0, 0,
                                          0, 0, 1e3, 0,
                                          0, 0, 0, 1 ) *
                                positionCalibration->rt_pos_calibration *
                                Matrix4f( 1e-3f, 0, 0, 0,
                                          0, 1e-3f, 0, 0,
                                          0, 0, 1e-3f, 0,
                                          0, 0, 0, 1 ) );
      hapi_device->setOrientationCalibration(
        orientationCalibration->rt_orn_calibration );
    }
    //cerr << deviceOrientation->getValue() << endl;
    //cerr << trackerOrientation->getValue() << endl;

    vector< Vec3f > proxies;

    for( unsigned int layer = 0; layer < hapi_device->nrLayers(); layer++ ) {
      HAPI::HAPIHapticsRenderer *renderer = 
        hapi_device->getHapticsRenderer( layer );

      HAPI::HAPIProxyBasedRenderer *proxy_renderer = 
        dynamic_cast< HAPI::HAPIProxyBasedRenderer * >( renderer );
      if( proxy_renderer ) {
        Vec3f proxy_pos = (Vec3f)(proxy_renderer->getProxyPosition() * 1e-3);
        proxies.push_back( proxy_pos );
        if( layer == 0 ) {
          //cerr << proxy_pos << endl;
          proxyPosition->setValue( proxy_pos, id );
        }
      } else {
        Vec3f tracker_pos = trackerPosition->getValue(); 
        proxies.push_back( tracker_pos );
        if( layer == 0 ) {
          proxyPosition->setValue( tracker_pos, id );
        }
      }
    }

    proxyPositions->setValue( proxies, id );

    // find the index of the haptics device
    DeviceInfo *di = DeviceInfo::getActive();
    int device_index = -1;
    if( di ) {
      const NodeVector &devices = di->device->getValue();
      for( unsigned int i = 0; i < devices.size(); i++ ) {
        if( (Node *)devices[i] == this )
          device_index = i;
      }
    }
    assert( device_index != -1 );

    HAPI::HAPIHapticsRenderer::Contacts all_contacts;

    for( unsigned int layer = 0; layer < hapi_device->nrLayers(); layer++ ) {
      HAPI::HAPIHapticsRenderer *renderer = 
        hapi_device->getHapticsRenderer( layer );
      HAPI::HAPIHapticsRenderer::Contacts contacts;
      if( renderer ) {
        contacts = renderer->getContacts();
        all_contacts.insert( all_contacts.end(),
                             contacts.begin(),
                             contacts.end() );
      }
      for( HAPI::HAPIHapticsRenderer::Contacts::iterator i = contacts.begin();
           i != contacts.end(); i++ ) {
        X3DGeometryNode *geom =
          static_cast< X3DGeometryNode * >( (*i).first->getUserData() );
      
        // make sure all fields have the right size
        if( device_index > (int)geom->force->size() -1 )
          geom->force->resize( device_index + 1, Vec3f( 0, 0, 0 ), geom->id );
        if( device_index > (int)geom->contactPoint->size() -1 )
          geom->contactPoint->resize( device_index + 1, Vec3f( 0, 0, 0 ),
                                      geom->id );
        if( device_index > (int)geom->contactNormal->size() -1 )
          geom->contactNormal->resize( device_index + 1, Vec3f( 1, 0, 0 ),
                                       geom->id );
         if( device_index > (int)geom->contactTexCoord->size() -1 )
          geom->contactTexCoord->resize( device_index + 1, Vec3f( 0, 0, 0 ),
                                         geom->id );
        if( device_index > (int)geom->isTouched->size() -1 )
          geom->isTouched->resize( device_index + 1, false, geom->id );
        
        HAPI::HAPISurfaceObject::ContactInfo ci = (*i).second;

        // TODO: shpould be able to do it in a faster/better way.
        Matrix4d global_to_local = (*i).first->getInverse();
        Matrix3d global_vec_to_local = global_to_local.getRotationPart();

        Vec3f cp( ( global_to_local * ci.globalSurfaceContactPoint() )* 1e-3 );

        if( geom->contactPoint->getValueByIndex( device_index ) != cp )
          geom->contactPoint->setValue( device_index, cp, geom->id );

        Vec3f n( global_vec_to_local * ci.yAxis() );

        if( geom->contactNormal->getValueByIndex( device_index ) != n ) 
          geom->contactNormal->setValue( device_index, n, geom->id ); 

        if( geom->contactTexCoord->getValueByIndex( device_index ) !=
            ci.contactPointTexCoord() ) 
          geom->contactTexCoord->setValue( device_index,
                                           (Vec3f) ci.contactPointTexCoord(),
                                           geom->id ); 
        
        Vec3f f( global_vec_to_local * ci.globalForce() );

        if( geom->force->getValueByIndex( device_index ) != f ) 
          geom->force->setValue(device_index, f, geom->id ); 

        bool contact_last_time = 
          geom->isTouched->getValueByIndex( device_index );
        if( !contact_last_time ) 
          geom->isTouched->setValue( device_index, true, geom->id );
      }
    }

    for( HAPI::HAPIHapticsRenderer::Contacts::iterator j =
          last_contacts.begin();
         j != last_contacts.end(); j++ ) {
      bool still_in_contact = false;
      for( HAPI::HAPIHapticsRenderer::Contacts::iterator i =
            all_contacts.begin();
           i != all_contacts.end(); i++ ) {
        if( (*i).first->getUserData() == (*j).first->getUserData() ) {
          still_in_contact = true;
          break;
        }
      }
      if( !still_in_contact ) {
        X3DGeometryNode *geom = 
          static_cast< X3DGeometryNode * >((*j).first->getUserData() );
        geom->isTouched->setValue( device_index, false, geom->id );
        geom->force->setValue( device_index, Vec3f(), geom->id );
      }
    }
    last_contacts.swap( all_contacts );

    /*
    cerr << "F: " << devicePosition->getValue() * 1e3<< endl;
    cerr << "F: " << trackerPosition->getValue() * 1e3<< endl;
    cerr << "T: " << proxyPosition->getValue() * 1e3 << endl;
    cerr << "W: " << weightedProxyPosition->getValue()* 1e3<< endl;
    */
  }

  //cerr << hr << endl;
  // TODO: lock
  //proxyPosition->setValue( (Vec3f)proxy_position, id );
}




