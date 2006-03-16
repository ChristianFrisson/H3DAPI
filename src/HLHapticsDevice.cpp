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
/// \file HLHapticsDevice.cpp
/// \brief cpp file for HLHapticsDevice
///
//
//////////////////////////////////////////////////////////////////////////////

#include "HLHapticsDevice.h"
#include "HLShape.h"
#include "GL/glew.h"

#ifdef HAVE_OPENHAPTICS
#ifdef _MSC_VER
#pragma comment( lib, "hd.lib" )
#pragma comment( lib, "hl.lib" )
#pragma comment( lib, "hdu.lib" )
#pragma comment( lib, "hlu.lib" )
#endif
#endif

#ifdef MACOSX
#include <mach/mach_init.h>
#include <mach/thread_policy.h>
#include <mach/thread_act.h>
#endif

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase HLHapticsDevice::database( 
                                           "HLHapticsDevice", 
                                           &(newInstance<HLHapticsDevice>),
                                           typeid( HLHapticsDevice ),
                                           &H3DHapticsDevice::database
                                           );

namespace HLHapticsDeviceInternal {
  FIELDDB_ELEMENT( HLHapticsDevice, deviceName, INPUT_OUTPUT );
  FIELDDB_ELEMENT( HLHapticsDevice, secondaryButton, OUTPUT_ONLY );
  FIELDDB_ELEMENT( HLHapticsDevice, deviceLog,  INPUT_OUTPUT );

#ifdef MACOSX
  static int init_realtime=0;
#endif

#ifdef HAVE_OPENHAPTICS
  // Structure to return values from the HDDataCallback callback function.
  struct HDData {
    HDdouble force[3], torque[3];
    HDint update_rate;
  };

  
  // Callback function for getting data from HD API that changes 
  // between each scene graph loop. Used by 
  // HLHapticsDevice::updateDeviceValues()
  HDCallbackCode HDCALLBACK HDDataCallback( void *data ) {
    HDData *hd_data = static_cast< HDData * >( data );
    hdGetDoublev( HD_LAST_FORCE, hd_data->force );
    hdGetDoublev( HD_LAST_TORQUE, hd_data->torque );
    hdGetIntegerv( HD_UPDATE_RATE, &hd_data->update_rate );
    return HD_CALLBACK_DONE;
  } 
  
  // Callback function for rendering force effects on the 
  // HLHapticsDevice.  
  HDCallbackCode HDCALLBACK forceEffectCallback( void *data ) {
    HLHapticsDevice *hd = static_cast< HLHapticsDevice * >( data );
  
#ifdef MACOSX_SCHEDULER
    // set thread priority
    if ( !init_realtime ) {
      struct thread_time_constraint_policy ttcpolicy;
      int ret;
      ttcpolicy.period=      100000; // HZ/160;
      ttcpolicy.computation=  20000; // HZ/3300;
      ttcpolicy.constraint=  100000; //HZ/2200;
      ttcpolicy.preemptible=1;
      if ((ret=thread_policy_set( mach_thread_self(),
                                  THREAD_TIME_CONSTRAINT_POLICY, (thread_policy_t)&ttcpolicy,
                                  THREAD_TIME_CONSTRAINT_POLICY_COUNT)) != KERN_SUCCESS) {
        Console(4) << "set_realtime() failed" << endl;
        return 0;
      }
      init_realtime = 1;
    }
#endif

    // get current values from HD API 
    HLdouble tmp[16];
    hdGetDoublev( HD_CURRENT_TRANSFORM, tmp );
    // Workaround for a mysterious problem with Linux / GCC
    H3DFloat m0, m1, m2, m4,m5, m6, m8, m9, m10;
    m0  = (H3DFloat) tmp[0];
    m1  = (H3DFloat) tmp[1];
    m2  = (H3DFloat) tmp[2];
    
    m4  = (H3DFloat) tmp[4];
    m5  = (H3DFloat) tmp[5];
    m6  = (H3DFloat) tmp[6];
    
    m8  = (H3DFloat) tmp[8];
    m9  = (H3DFloat) tmp[9];
    m10 = (H3DFloat) tmp[10];
    
    Rotation rot( Matrix3f( m0, m4, m8,
			    m1, m5, m9,
			    m2, m6, m10 ) );
    Vec3f pos( (H3DFloat) tmp[12], (H3DFloat) tmp[13], (H3DFloat) tmp[14] );
    hdGetDoublev( HD_CURRENT_VELOCITY, tmp );
    Vec3f vel( (H3DFloat) tmp[0], (H3DFloat) tmp[1], (H3DFloat) tmp[2] );

    // Dump out values to log file
    // BAF2 format uses uncalibrated position / orientation data.
    if ( hd->log.get() )
      hd->log->writeLog( pos, rot );

    // apply the calibration matrices to get the values to
    // in the H3D API coordinate space.
    pos = hd->positionCalibration->rt_pos_calibration * pos;
    vel = hd->positionCalibration->rt_pos_calibration * vel;
    rot = hd->orientationCalibration->rt_orn_calibration * rot;

    TimeStamp dt = TimeStamp() - hd->last_effect_change;
    HapticForceEffect::EffectInput input( pos, vel, rot, dt );
    HapticForceEffect::EffectOutput output;

    // calculate the forces generated by the force effects from the
    // last loop that are to be interpolated. 
    HapticEffectVector & last_effects = hd->getLastForceEffects(); 
    for( HapticEffectVector::const_iterator i = last_effects.begin();
         i != last_effects.end();
         i++ ) {
      if( (*i)->isInterpolated() )
        output = output + (*i)->calculateForces( input );
    }

    double weighting = dt / hd->last_loop_time;
    if( weighting > 1 ) weighting = 1;
    // the previous force effects are to be decreased as time goes by.
    output = output * ( 1 - weighting );

    // calculate the forces generated by the active force effects
    HapticEffectVector & current_effects = hd->getCurrentForceEffects(); 
    for( HapticEffectVector::const_iterator i = current_effects.begin();
         i != current_effects.end();
         i++ ) {
      if( (*i)->isInterpolated() )
        output = output + (*i)->calculateForces( input ) * weighting;
      else
        output = output + (*i)->calculateForces( input );
    }
    
    output.force = 
      hd->positionCalibration->rt_inv_pos_rotation * output.force;

    // add the resulting force and torque to the rendered force.
    HDdouble force[3];
    hdGetDoublev( HD_CURRENT_FORCE, force );
    force[0] += output.force.x; 
    force[1] += output.force.y; 
    force[2] += output.force.z; 
    hdSetDoublev( HD_CURRENT_FORCE, force );

    HDdouble torque[3];
    hdGetDoublev( HD_CURRENT_TORQUE, torque );
    torque[0] += output.torque.x; 
    torque[1] += output.torque.y; 
    torque[2] += output.torque.z; 
    hdSetDoublev( HD_CURRENT_TORQUE, torque );

    
    return HD_CALLBACK_CONTINUE;
  }

  // Callback function for changing force effect that are to be rendered.
  HDCallbackCode HDCALLBACK changeForceEffects( void *_data ) {
    void * * data = static_cast< void * * >( _data );
    HLHapticsDevice *hd = static_cast< HLHapticsDevice * >( data[0] );
    HapticEffectVector* effects  = 
      static_cast< HapticEffectVector * >( data[1] );
    hd->getLastForceEffects().swap( hd->getCurrentForceEffects() );
    hd->getCurrentForceEffects().swap( *effects );
    TimeStamp now = TimeStamp();
    hd->last_loop_time = now - hd->last_effect_change;
    hd->last_effect_change = now;
    return HD_CALLBACK_DONE;
  }

  string getHDErrorCodeString( HDerror error_code ) {
    switch ( error_code ) {
    case HD_SUCCESS: return "HD_SUCCESS";
      
      // Function errors 
    case HD_INVALID_ENUM: return "HD_INVALID_ENUM";
    case HD_INVALID_VALUE: return "HD_INVALID_VALUE";
    case HD_INVALID_OPERATION: return "HD_INVALID_OPERATION";
    case HD_INVALID_INPUT_TYPE: return "HD_INVALID_INPUT_TYPE";
    case HD_BAD_HANDLE: return "HD_BAD_HANDLE";
      
      // Force errors
    case HD_WARM_MOTORS: return "HD_WARM_MOTORS";
    case HD_EXCEEDED_MAX_FORCE: return "HD_EXCEEDED_MAX_FORCE";
    case HD_EXCEEDED_MAX_VELOCITY: return "HD_EXCEEDED_MAX_VELOCITY";
    case HD_FORCE_ERROR: return "HD_FORCE_ERROR";

      // Device errors
    case HD_DEVICE_FAULT: return "HD_DEVICE_FAULT";
    case HD_DEVICE_ALREADY_INITIATED: return "HD_DEVICE_ALREADY_INITIATED";
    case HD_COMM_ERROR: return "HD_COMM_ERROR";
    case HD_COMM_CONFIG_ERROR: return "HD_COMM_CONFIG_ERROR";
    case HD_TIMER_ERROR: return "HD_TIMER_ERROR";
      
      // Haptic rendering context
    case HD_ILLEGAL_BEGIN: return "HD_ILLEGAL_BEGIN";
    case HD_ILLEGAL_END: return "HD_ILLEGAL_END";
    case HD_FRAME_ERROR: return "HD_FRAME_ERROR";
      
      // Scheduler errors
    case HD_INVALID_PRIORITY: return "HD_INVALID_PRIORITY";
    case HD_SCHEDULER_FULL: return "HD_SCHEDULER_FULL";

      // Licensing errors
#if ( HD_VERSION_MAJOR_NUMBER > 1 || (HD_VERSION_MAJOR_NUMBER == 1 && HD_VERSION_MINOR_NUMBER >= 01 ) )
    case HD_INVALID_LICENSE: return "HD_INVALID_LICENSE";
#endif
    default: return "Unknown HD Error Code";
    }
  }

  string getHLErrorString( HLerror error ) {
    if( error.errorCode == HL_DEVICE_ERROR ) {
      stringstream s;
      s << "HL_DEVICE_ERROR( " 
        << getHDErrorCodeString( error.errorInfo.errorCode )
        << " )";
      return s.str();
    } else {
      return error.errorCode;
    }
  }

  // Callback function that starts a new hd frame. It is used in order to 
  // encapsulate all HD API callback function within a hdBeginFrame/hdEndFrame
  // pair in order to only get one frame per loop.
  HDCallbackCode HDCALLBACK beginFrameCallback( void *data ) {
    HLHapticsDevice *hd = static_cast< HLHapticsDevice * >( data );
    hdBeginFrame( hd->getDeviceHandle() );
    return HD_CALLBACK_CONTINUE;
  }

  // Callback function that ends a hd frame. It is used in order to 
  // encapsulate all HD API callback function within a hdBeginFrame/hdEndFrame
  // pair in order to only get one frame per loop.
  HDCallbackCode HDCALLBACK endFrameCallback( void *data ) {
    HLHapticsDevice *hd = static_cast< HLHapticsDevice * >( data );
    hdEndFrame( hd->getDeviceHandle() );
    return HD_CALLBACK_CONTINUE;
  }
}

// Callback function for setting the button fields when button events
// occur
void HLCALLBACK HLHapticsDevice::hlButtonCallback( HLenum event,
                                                   HLuint object,
                                                   HLenum thread,
                                                   HLcache *cache,
                                                   void *data ) {
  HLHapticsDevice *hd = static_cast< HLHapticsDevice * >( data );
  if( event == HL_EVENT_1BUTTONDOWN ) {
    hd->mainButton->setValue( true, hd->id );
    if ( hd->log.get() )
      hd->log->writeMessage( "Button 1 pressed" );
  } else if( event == HL_EVENT_1BUTTONUP ) {
    hd->mainButton->setValue( false, hd->id );
    if ( hd->log.get() )
      hd->log->writeMessage( "Button 1 released" );
  } else if( event ==  HL_EVENT_2BUTTONDOWN ) {
    hd->secondaryButton->setValue( true, hd->id );
    if ( hd->log.get() )
      hd->log->writeMessage( "Button 2 pressed" );
  } else if( event == HL_EVENT_2BUTTONUP ) {
    hd->secondaryButton->setValue( false, hd->id );
    if ( hd->log.get() )
      hd->log->writeMessage( "Button 2 released" );
  }
}
void HLHapticsDevice::renderShapes( const HapticShapeVector &objects  ) {
  hlMakeCurrent( haptic_context );

  for( HapticShapeVector::const_iterator i = objects.begin();
       i != objects.end();
       i++ ) {
    HLShape *hl = dynamic_cast< HLShape * >( *i );
    if( hl ) {    
      hl->hlRender( this );
     }
  }
  
  // check for any errors
  HLerror error;
  while ( HL_ERROR(error = hlGetError()) ) {
    Console(4) << HLHapticsDeviceInternal::getHLErrorString( error )
               << endl;
  }   
  current_shapes = objects;
}

void HLHapticsDevice::renderEffects( 
                  const HapticEffectVector &effects ) {
  hlMakeCurrent( haptic_context );  
  // make a copy of the effects vector since it is swapped in
  // the callback.
  HapticEffectVector effects_copy( effects );
  typedef void *pp;
  void * param[] = { this, &effects_copy };
  // change the current_force_effects vector to render the new effects.
  hdScheduleSynchronous( HLHapticsDeviceInternal::changeForceEffects,
                         param,
                         HD_DEFAULT_SCHEDULER_PRIORITY );
  // check for any errors
  HLerror error;
  while ( HL_ERROR(error = hlGetError()) ) {
    Console(4) << HLHapticsDeviceInternal::getHLErrorString( error )
               << endl;
  }      
}

void HLHapticsDevice::initDevice() {
  if( !initialized->getValue() ) {
    device_handle = hdInitDevice( deviceName->getValue().c_str() );
    HDErrorInfo error = hdGetError();
    if ( HD_DEVICE_ERROR( error ) ) {
      Console(4) << "Warning: Could not init Phantom device. Error code: "
        << HLHapticsDeviceInternal::getHDErrorCodeString( error.errorCode ) 
        << ". Internal error code: " << error.internalErrorCode << endl;
      return;
    }   
    
    hdEnable(HD_FORCE_OUTPUT);
    
    HDCallbackCode handle =
      hdScheduleAsynchronous( HLHapticsDeviceInternal::forceEffectCallback,
                              this,
                              HD_DEFAULT_SCHEDULER_PRIORITY );
    hd_handles.push_back( handle );
    handle = hdScheduleAsynchronous( HLHapticsDeviceInternal::beginFrameCallback,
                                     this,
                                     HD_MAX_SCHEDULER_PRIORITY );
    hd_handles.push_back( handle );
    handle = hdScheduleAsynchronous( HLHapticsDeviceInternal::endFrameCallback,
                                     this,
                                     HD_MIN_SCHEDULER_PRIORITY );
    hd_handles.push_back( handle );
    
    HLThread *hl_thread = HLThread::getInstance();

    // Create a haptic context for the device. The haptic context maintains 
    // the state that persists between frame intervals and is used for
    // haptic rendering.

    haptic_context = hlCreateContext( device_handle );
    hlMakeCurrent( haptic_context );  
    thread = hl_thread;
    hl_thread->setActive( true );

    hlEnable(HL_HAPTIC_CAMERA_VIEW);
     
    HLint tmp_int;
    hdGetIntegerv( HD_INPUT_DOF, &tmp_int );
    inputDOF->setValue( tmp_int, id );
    
    hdGetIntegerv( HD_OUTPUT_DOF, &tmp_int );
    outputDOF->setValue( tmp_int, id );
    
    hlAddEventCallback( HL_EVENT_1BUTTONDOWN, 
                        HL_OBJECT_ANY,
                        HL_CLIENT_THREAD,
                        HLHapticsDevice::hlButtonCallback,
                        this );
    hlAddEventCallback( HL_EVENT_1BUTTONUP, 
                        HL_OBJECT_ANY,
                        HL_CLIENT_THREAD,
                        HLHapticsDevice::hlButtonCallback,
                        this );
    hlAddEventCallback( HL_EVENT_2BUTTONDOWN, 
                        HL_OBJECT_ANY,
                        HL_CLIENT_THREAD,
                        HLHapticsDevice::hlButtonCallback,
                        this );
    hlAddEventCallback( HL_EVENT_2BUTTONUP, 
                        HL_OBJECT_ANY,
                        HL_CLIENT_THREAD,
                        HLHapticsDevice::hlButtonCallback,
                        this );

    H3DHapticsDevice::initDevice();
  }
}

void HLHapticsDevice::disableDevice() {
  if ( log.get() )
	  log->closeLog();
  if( initialized->getValue() ) {
    H3DHapticsDevice::disableDevice();
    hlMakeCurrent( NULL );
    if( haptic_context )
      hlDeleteContext( haptic_context );
    for( vector< HDCallbackCode >::iterator i = hd_handles.begin();
         i != hd_handles.end();
         i++ ) {
      hdUnschedule(*i);
    }
    hdDisableDevice( device_handle );
    haptic_context = NULL;
    device_handle = 0;
    HLThread *hl_thread = static_cast< HLThread * >( thread );
    hl_thread->setActive( false );
    thread = NULL;
  }
}

void HLHapticsDevice::updateDeviceValues() {

  // update real-time reference to DeviecLog
	DeviceLog * dl = static_cast< DeviceLog* >( deviceLog->getValue() );
	if ( !log.get() && dl ) {
		log.reset( dl );
		log->openLog();
	} else if ( log.get() && log.get() != dl ) {
		log->closeLog();
		log.reset( dl );
		if( log.get() )
		  log->openLog();		
	}

  // button values are set via event callback functions
  hlMakeCurrent( haptic_context );  

  HLdouble rotation[4];
  hlGetDoublev( HL_DEVICE_ROTATION, rotation );
  deviceOrientation->setValue( Rotation( Quaternion( (H3DFloat) rotation[1], 
                                                     (H3DFloat) rotation[2], 
                                                     (H3DFloat) rotation[3], 
                                                     (H3DFloat) rotation[0] ) ), id );
  HLdouble pos[3];
  hlGetDoublev( HL_DEVICE_POSITION, pos );
  devicePosition->setValue( Vec3f( (H3DFloat) pos[0], 
                                   (H3DFloat) pos[1],
                                   (H3DFloat) pos[2] ), id );  
  
  const Matrix4f &pcal = positionCalibration->getValue().inverse();
  HLdouble m[16] = { pcal[0][0], pcal[1][0], pcal[2][0], pcal[3][0], 
                     pcal[0][1], pcal[1][1], pcal[2][1], pcal[3][1], 
                     pcal[0][2], pcal[1][2], pcal[2][2], pcal[3][2], 
                     pcal[0][3], pcal[1][3], pcal[2][3], pcal[3][3] }; 
  hlMatrixMode( HL_TOUCHWORKSPACE );
  hlLoadMatrixd( m );

  hlGetDoublev( HL_PROXY_POSITION, pos );
  proxyPosition->setValue( Vec3f( (H3DFloat) pos[0],
                                  (H3DFloat) pos[1], 
                                  (H3DFloat) pos[2] ), id );

  // get the values from hd api not acessable from hl api
  HLHapticsDeviceInternal::HDData hddata;
  hdScheduleSynchronous( HLHapticsDeviceInternal::HDDataCallback,
                         &hddata,
                         HD_DEFAULT_SCHEDULER_PRIORITY );

  force->setValue( Vec3f( (H3DFloat)hddata.force[0],
                          (H3DFloat)hddata.force[1],
                          (H3DFloat)hddata.force[2] ), 
                   id );
  torque->setValue( Vec3f( (H3DFloat)hddata.torque[0],
                           (H3DFloat)hddata.torque[1],
                           (H3DFloat)hddata.torque[2] ),
                    id );
  hapticsRate->setValue( hddata.update_rate, id );
}
#include "HLShape.h"

void HLHapticsDevice::preRender() {
  HLShape::resetShapeIdDB();
  glMatrixMode( GL_MODELVIEW );
  glPushMatrix();
  glLoadIdentity();
  glClear(GL_DEPTH_BUFFER_BIT);   
  hlMatrixMode( HL_TOUCHWORKSPACE );
  hlPushMatrix();
  hlMatrixMode( HL_VIEWTOUCH );
  hlPushMatrix();

  hlMakeCurrent( haptic_context );  
  hlBeginFrame();
  hlCheckEvents();
  /*
   if( current_shapes.size() > 0 ) {
    HLboolean b;
    HLdouble d[3];
    HLShape *shape = dynamic_cast< HLShape * >( current_shapes[0] );
    if( shape ) {
      hlGetShapeBooleanv( shape->getShapeId(),
                         HL_PROXY_IS_TOUCHING,
                         &b );
      }
      }*/

  // check for any errors
  HLerror error;
  while ( HL_ERROR(error = hlGetError()) ) {
    Console(4) << HLHapticsDeviceInternal::getHLErrorString( error )
               << endl;
  }      

  
}

void HLHapticsDevice::postRender() { 
  glMatrixMode( GL_MODELVIEW );
  glPopMatrix();
  
  hlMakeCurrent( haptic_context );  
  hlEndFrame(); 
  hlMatrixMode( HL_VIEWTOUCH );
  hlPopMatrix();
  hlMatrixMode( HL_TOUCHWORKSPACE );
  hlPopMatrix();

   // check for any errors
  HLerror error;
  while ( HL_ERROR(error = hlGetError()) ) {
    Console(4) << HLHapticsDeviceInternal::getHLErrorString( error )
               << endl;
  }   
  #endif   
}

