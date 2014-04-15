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
/// \file Sound.cpp
/// \brief CPP file for Sound, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/Sound.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase Sound::database( 
                                "Sound", 
                                &(newInstance<Sound>), 
                                typeid( Sound ),
                                &X3DSoundNode::database );

namespace SoundInternals {
  FIELDDB_ELEMENT( Sound, direction, INPUT_OUTPUT );
  FIELDDB_ELEMENT( Sound, intensity, INPUT_OUTPUT );
  FIELDDB_ELEMENT( Sound, location, INPUT_OUTPUT );
  FIELDDB_ELEMENT( Sound, maxBack, INPUT_OUTPUT );
  FIELDDB_ELEMENT( Sound, maxFront, INPUT_OUTPUT );
  FIELDDB_ELEMENT( Sound, minBack, INPUT_OUTPUT );
  FIELDDB_ELEMENT( Sound, minFront, INPUT_OUTPUT );
  FIELDDB_ELEMENT( Sound, priority, INPUT_OUTPUT );
  FIELDDB_ELEMENT( Sound, source, INPUT_OUTPUT );
  FIELDDB_ELEMENT( Sound, spatialize, INITIALIZE_ONLY );
}

#ifdef HAVE_OPENAL
bool Sound::alut_initialized = false;
ALCdevice *Sound::al_device = NULL;
ALCcontext *Sound::al_context = NULL;
#endif

Sound::Sound( 
             Inst< SFNode  > _metadata,
             Inst< SFVec3f > _direction,
             Inst< SFFloat > _intensity,
             Inst< SFVec3f > _location,
             Inst< SFFloat > _maxBack,
             Inst< SFFloat > _maxFront,
             Inst< SFFloat > _minBack,
             Inst< SFFloat > _minFront,
             Inst< SFFloat > _priority,
             Inst< SFSoundSourceNode  > _source,
             Inst< SFBool  > _spatialize,
             Inst< ALSoundSetup > _soundSetup ) :
  X3DSoundNode( _metadata ),
  direction ( _direction  ),
  intensity ( _intensity  ),
  location  ( _location   ),
  maxBack   ( _maxBack    ),
  maxFront  ( _maxFront   ),
  minBack   ( _minBack    ),
  minFront  ( _minFront   ),
  priority  ( _priority   ),
  source    ( _source     ),
  spatialize( _spatialize ),
  soundSetup( _soundSetup ),
  accForwardMatrix( new SFMatrix4f ) {

  database.initFields( this );
  type_name = "Sound";

  soundSetup->setName( "soundSetup" );
  soundSetup->setOwner( this );

  accForwardMatrix->setName( "accForwardMatrix" );
  accForwardMatrix->setOwner( this );

  direction->setValue( Vec3f( 0, 0, 1 ) );
  intensity->setValue( 1 );
  location->setValue( Vec3f( 0, 0, 0 ) );
  maxBack->setValue( 10 );
  maxFront->setValue( 10 );
  minBack->setValue( 1 );
  minFront->setValue( 1 );
  priority->setValue( 0 );
  spatialize->setValue( true );

  
  intensity->route( soundSetup );
  direction->route( soundSetup );
  location->route( soundSetup );
  maxBack->route( soundSetup );
  minBack->route( soundSetup );
  maxFront->route( soundSetup );
  minFront->route( soundSetup );
  accForwardMatrix->route( soundSetup );
  
#ifdef HAVE_OPENAL
  if( !al_device )
    al_device = alcOpenDevice(NULL);
  if( !al_device ) {
    Console(4) << "Warning: Could not find audio device for use with Sound node."
               << endl;
  }
  if( !al_context )
    al_context = alcCreateContext( al_device, NULL);
  
  if( !al_context ) {
    Console(4) << "Warning: Could not create OpenAL context for use with Sound node."
               << endl;
  }
  alcMakeContextCurrent(al_context);
  if( !al_source ) {
    alGenSources( 1, &al_source );
    }
  alSourcef( al_source, AL_GAIN, 0 );
#else
  Console(4) << "Warning: H3D API compiled without OpenAL. Sound nodes"
             << " will be unusable." << endl;
#endif
}

Sound::~Sound() {
#ifdef HAVE_OPENAL
  if( al_source )
    alDeleteSources(1, &al_source );
#endif
}

void Sound::traverseSG( TraverseInfo &ti ) {
  X3DSoundSourceNode *sound_source = source->getValue();
  if( sound_source ) {
    sound_source->traverseSG( ti );
  }
  accForwardMatrix->setValue( ti.getAccForwardMatrix() );
}

void Sound::ALrender() {
#ifdef HAVE_OPENAL
  if( !al_context || !al_device ) return;

  Vec3f listener_pos = Vec3f( 0, 0, 0 );

  // set up listener properties.
  X3DViewpointNode *vp = X3DViewpointNode::getActive();
  if( vp ) {
    const Matrix4f &vp_to_global =  vp->accForwardMatrix->getValue();
    const Matrix3f &vp_to_global_rot = vp_to_global.getRotationPart();
    listener_pos = vp_to_global * vp->totalPosition->getValue(); 
    Rotation vp_orn = vp->totalOrientation->getValue();
    Vec3f listener_up  = vp_to_global_rot * (Matrix3f) vp_orn * 
      Vec3f( 0, 1, 0 ); 
    Vec3f listener_lookat  = vp_to_global_rot * (Matrix3f)vp_orn
      * Vec3f( 0, 0, -1 ); 
    alListener3f( AL_POSITION, 
                  listener_pos.x, listener_pos.y, listener_pos.z ); 
    ALfloat listener_orn[] = 
      { listener_lookat.x, listener_lookat.y, listener_lookat.z,
        listener_up.x, listener_up.y, listener_up.z };
    alListenerfv( AL_ORIENTATION, listener_orn );
  }

  // convert all directions and positions to global space

  const Matrix4f &local_to_global = accForwardMatrix->getValue();
  Vec3f sound_pos = local_to_global * location->getValue();

  Vec3f dir = 
    local_to_global.getRotationPart() *
    direction->getValue();
  dir.normalizeSafe();
  
  Vec3f to_listener = listener_pos - sound_pos;
  // the distance between listener and sound source
  H3DFloat distance = to_listener.length();

  to_listener.normalizeSafe();

  // the angle in the ellipse from between the direction vector and
  // the vector from sound source an listener
  H3DFloat theta = (H3DFloat)( Constants::pi - H3DAcos( dir * to_listener ) );

  // clamp values within allowed border
  H3DFloat min_back = minBack->getValue();
  H3DFloat min_front = minFront->getValue();
  H3DFloat max_back = maxBack->getValue();
  H3DFloat max_front = maxFront->getValue();

  if( max_back < 0 ) max_back = 0;
  if( max_front < 0 ) max_front = 0;
  
  if( min_back < 0 ) min_back = 0;
  else if( min_back > max_back ) min_back = max_back;
  if( min_front < 0 ) min_front = 0;
  else if( min_front > max_front ) min_front = max_front;

  // calculate the distance from the sound location and the max ellipse
  // in the direction of the listener using the ellipse equation.
  H3DFloat max_r = 0;
  H3DFloat a = ( max_front + max_back ) / 2; 
  H3DFloat c = ( max_front - max_back ) / 2;  
  if( a > Constants::f_epsilon ) {
    H3DFloat e = c / a;
    if( e < 1.0f && e > -1.0f  ) {
      max_r = ( a * ( 1 - e*e ) ) / (1 + e * H3DCos( theta ) );
    }
  }

  // calculate the distance from the sound location and the min ellipse
  // in the direction of the listener using the ellipse equation.
  H3DFloat min_r = 0;
  a = ( min_front + min_back ) / 2; 
  c = ( min_front - min_back ) / 2;  
  if( a > Constants::f_epsilon ) {
    H3DFloat e = c / a;
    if( e < 1.0f && e > -1.0f  ) {
      min_r = ( a * ( 1 - e*e ) ) / (1 + e * H3DCos( theta ) );
    }
  }
  
  ALfloat gain = intensity->getValue();
  if( distance > max_r ) gain = 0;
  else if( distance > min_r ) {
    H3DFloat diff = max_r - min_r;
    if( diff > Constants::f_epsilon ) {
      // attenuation according to X3D spec
      H3DFloat attenuation_db = -20 * ( distance - min_r ) / diff;
      // convert to OpenAL attenuation which is linear and not
      // db based.
      H3DFloat attenuation = H3DPow( 10.f, attenuation_db / 20 );
      gain = gain * attenuation;
      // gain = gain * ( max_r - distance ) / ( max_r - min_r );
    }
  }

  alDistanceModel( AL_NONE );
  if( spatialize->getValue() ) {
    alSource3f( al_source, AL_POSITION, 
                sound_pos.x, sound_pos.y, sound_pos.z );
  } else {
    // spatialize is FALSE, so put the sound in the same position
    // as the listener to avoid spatialize effects.
    alSource3f( al_source, AL_POSITION, 
                listener_pos.x, listener_pos.y, listener_pos.z );
  }
  alSourcef( al_source, AL_GAIN, gain );
#endif
}

void Sound::SFSoundSourceNode::onAdd( Node *n ) {
  TypedSFNode< X3DSoundSourceNode >::onAdd( n );
  X3DSoundSourceNode *sound_source = static_cast< X3DSoundSourceNode * >( n );
  X3DSoundNode *sound_node = static_cast< X3DSoundNode * >( getOwner() );
  if( sound_source ) {
    sound_source->registerSoundNode( sound_node );
  }
}

void Sound::SFSoundSourceNode::onRemove( Node *n ) {
  X3DSoundSourceNode *sound_source = static_cast< X3DSoundSourceNode * >( n );
  X3DSoundNode *sound_node = static_cast< X3DSoundNode * >( getOwner() );
  if( sound_source ) {
    sound_source->unregisterSoundNode( sound_node );
  }
  TypedSFNode< X3DSoundSourceNode >::onRemove( n );
}
