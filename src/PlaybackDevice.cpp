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
/// \file PlaybackDevice.cpp
/// \brief cpp file for PlaybackDevice
///
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/PlaybackDevice.h>
#include <HAPI/PlaybackHapticsDevice.h>

using namespace H3D;

H3DNodeDatabase PlaybackDevice::database ( "PlaybackDevice", 
                                           &(newInstance<PlaybackDevice>),
                                           typeid( PlaybackDevice ),
                                           &H3DHapticsDevice::database ); 

namespace PlaybackDeviceInternals {
  FIELDDB_ELEMENT( PlaybackDevice, url, INPUT_OUTPUT );
  FIELDDB_ELEMENT( PlaybackDevice, play, INPUT_OUTPUT );
  FIELDDB_ELEMENT( PlaybackDevice, binary, INPUT_OUTPUT );
  FIELDDB_ELEMENT( PlaybackDevice, seekToTime, INPUT_OUTPUT );
  FIELDDB_ELEMENT( PlaybackDevice, playbackSpeed, INPUT_OUTPUT );
  FIELDDB_ELEMENT( PlaybackDevice, playbackTime, OUTPUT_ONLY );
  FIELDDB_ELEMENT( PlaybackDevice, playing, OUTPUT_ONLY );
  FIELDDB_ELEMENT( PlaybackDevice, playbackData, INPUT_OUTPUT );
  FIELDDB_ELEMENT( PlaybackDevice, defaultDevicePosition, INPUT_OUTPUT );
  FIELDDB_ELEMENT( PlaybackDevice, defaultDeviceOrientation, INPUT_OUTPUT );
  FIELDDB_ELEMENT( PlaybackDevice, defaultDeviceVelocity, INPUT_OUTPUT );
  FIELDDB_ELEMENT( PlaybackDevice, defaultButtons, INPUT_OUTPUT );
}

/// Constructor.
PlaybackDevice::PlaybackDevice( 
          Inst< MFString        >  _url                   ,
          Inst< SFVec3f         > _devicePosition         ,
          Inst< SFRotation      > _deviceOrientation      ,
          Inst< TrackerPosition > _trackerPosition        ,
          Inst< TrackerOrientation > _trackerOrientation  ,
          Inst< SFMatrix4f      > _positionCalibration    ,
          Inst< SFRotation      > _orientationCalibration ,
          Inst< SFVec3f         > _proxyPosition          ,
          Inst< WeightedProxy   > _weightedProxyPosition  ,
          Inst< SFFloat         > _proxyWeighting         ,
          Inst< SFBool          > _mainButton             ,
          Inst< SFBool          > _secondaryButton        ,
          Inst< SFInt32         > _buttons                ,
          Inst< SFVec3f         > _force                  ,
          Inst< SFVec3f         > _torque                 ,
          Inst< SFInt32         > _inputDOF               ,
          Inst< SFInt32         > _outputDOF              ,
          Inst< SFInt32         > _hapticsRate            ,
          Inst< SFInt32         > _desiredHapticsRate     ,
          Inst< SFNode          > _stylus                 ,
          Inst< SFHapticsRendererNode > _hapticsRenderer  ,
          Inst< MFVec3f         > _proxyPositions         ,
          Inst< SFBool          > _followViewpoint        ,
          Inst< OnPlay          > _play                   ,
          Inst< SFBool          > _binary                 ,
          Inst< OnSeekToTime    > _seekToTime             ,
          Inst< OnPlaybackSpeed > _playbackSpeed          ,
          Inst< SFTime          > _playbackTime           ,
          Inst< SFBool          > _playing                ,
          Inst< MFString        > _playbackData           ,
          Inst< SFVec3f         > _defaultDevicePosition  ,
          Inst< SFRotation      > _defaultDeviceOrientation,
          Inst< SFVec3f         > _defaultDeviceVelocity  ,
          Inst< SFInt32         > _defaultButtons ):
  X3DUrlObject ( _url ),
  H3DHapticsDevice( _devicePosition, _deviceOrientation, _trackerPosition,
        _trackerOrientation, _positionCalibration, 
        _orientationCalibration, _proxyPosition,
        _weightedProxyPosition, _proxyWeighting, _mainButton,
                    _secondaryButton, _buttons,
        _force, _torque, _inputDOF, _outputDOF, _hapticsRate,
        _desiredHapticsRate,
        _stylus, _hapticsRenderer, _proxyPositions, _followViewpoint ),
  play ( _play ),
  binary ( _binary ),
  seekToTime ( _seekToTime ),
  playbackSpeed ( _playbackSpeed ),
  playbackTime ( _playbackTime ),
  playing ( _playing ),
  playbackData ( _playbackData ),
  defaultDevicePosition ( _defaultDevicePosition ),
  defaultDeviceOrientation ( _defaultDeviceOrientation ),
  defaultDeviceVelocity ( _defaultDeviceVelocity ),
  defaultButtons ( _defaultButtons ),
  playback_url_changed ( new Field ),
  default_values_changed ( new OnDefaultValuesChanged ) {

  type_name = "PlaybackDevice";
  database.initFields( this );

  default_values_changed->setName ( "default_values_changed" );
  default_values_changed->setOwner ( this );

  url->route ( playback_url_changed );
  playbackData->route ( playback_url_changed );

  defaultDevicePosition->route ( default_values_changed );
  defaultDeviceOrientation->route ( default_values_changed );
  defaultDeviceVelocity->route ( default_values_changed );
  defaultButtons->route ( default_values_changed );

  play->setValue ( false );
  binary->setValue ( false );
  seekToTime->setValue ( 0 );
  playbackSpeed->setValue ( 1 );
  playbackTime->setValue ( 0, id );
  playing->setValue ( false, id );
}

PlaybackDevice::~PlaybackDevice () {
  HAPI::PlaybackHapticsDevice* d= static_cast<HAPI::PlaybackHapticsDevice*>(getHAPIDevice());
  if ( d ) {
    d->closeRecording ();
  }
  if ( !tmp_filename.empty() ) {
    removeTmpFile ( tmp_filename );
  }
}

void PlaybackDevice::initialize () {
  H3DHapticsDevice::initialize ();

  hapi_device.reset( new HAPI::PlaybackHapticsDevice );

  // Ensure that playback is always started if play is set to
  // true from load, even if url parameter is set at load after the
  // play parameter.
  play->setValue ( play->getValue() );
}

void PlaybackDevice::updateDeviceValues () {
  H3DHapticsDevice::updateDeviceValues ();

  HAPI::PlaybackHapticsDevice* d= static_cast<HAPI::PlaybackHapticsDevice*>(getHAPIDevice());
  if ( d ) {
    bool is_playing= d->isPlaying();
    if ( is_playing != playing->getValue() ) {
      playing->setValue ( is_playing, id );
    }
    if ( is_playing ) {
      playbackTime->setValue ( d->getPlaybackTime(), id );
    }
  }
}

void PlaybackDevice::updateDefaultValues () {
  HAPI::PlaybackHapticsDevice* d= static_cast<HAPI::PlaybackHapticsDevice*>(getHAPIDevice());
  if ( d ) {
    HAPI::HAPIHapticsDevice::DeviceValues dv;
    dv.position= defaultDevicePosition->getValue();
    dv.orientation= defaultDeviceOrientation->getValue();
    dv.velocity= defaultDeviceVelocity->getValue();

    dv.button_status= defaultButtons->getValue();

    d->setDefaultDeviceValues ( dv );
  }
}

void PlaybackDevice::OnPlay::onNewValue( const bool& new_value ) {
  PlaybackDevice* node= static_cast<PlaybackDevice*>(getOwner());
  HAPI::PlaybackHapticsDevice* d= static_cast<HAPI::PlaybackHapticsDevice*>(node->getHAPIDevice());
  if ( d ) {

    if ( new_value ) {

      // Load new URL if required
      if ( !node->playback_url_changed->isUpToDate() ) {
        d->closeRecording();
        if ( !node->tmp_filename.empty() ) {
          node->removeTmpFile ( node->tmp_filename );
        }

        d->clearDataFields ();
        const std::vector < std::string > field_names= node->playbackData->getValue();
        for ( std::vector < std::string >::const_iterator i= field_names.begin(); i != field_names.end(); ++i ) {
          std::string name= *i;

          if ( name == "DEVICE_POSITION" ) {
            d->addDataField ( "RAW_POSITION" );
          } else if ( name == "DEVICE_ORIENTATION" ) {
            d->addDataField ( "RAW_ORIENTATION" );
          } else if ( name == "DEVICE_VELOCITY" ) {
            d->addDataField ( "RAW_VELOCITY" );
          } else if ( name == "BUTTONS" ) {
            d->addDataField ( "BUTTONS" );
          }
        }

        const std::vector < std::string > urls= node->url->getValue();
        for ( std::vector < std::string >::const_iterator i= urls.begin(); i != urls.end(); ++i ) {
          node->tmp_filename= node->resolveURLAsFile ( *i );
          if ( !node->tmp_filename.empty() ) {
            if ( d->loadRecording ( node->tmp_filename, node->binary->getValue() ) ) {
              node->setURLUsed ( *i );
            } else {
              node->setURLUsed ( "" );
            }
            break;
          }
        }
        node->updateDefaultValues ();
        node->playback_url_changed->upToDate();
      }
    }

    if ( new_value ) {
      node->default_values_changed->upToDate();
      d->startPlayback();
    } else {
      d->stopPlayback();
    }
  }
}

void PlaybackDevice::OnSeekToTime::onNewValue( const H3DTime& new_value ) {
  PlaybackDevice* node= static_cast<PlaybackDevice*>(getOwner());
  HAPI::PlaybackHapticsDevice* d= static_cast<HAPI::PlaybackHapticsDevice*>(node->getHAPIDevice());
  if ( d ) {
    d->seekToTime ( new_value );
  }
}

void PlaybackDevice::OnPlaybackSpeed::onNewValue( const H3DFloat& new_value ) {
  PlaybackDevice* node= static_cast<PlaybackDevice*>(getOwner());
  HAPI::PlaybackHapticsDevice* d= static_cast<HAPI::PlaybackHapticsDevice*>(node->getHAPIDevice());
  if ( d ) {
    d->setPlaybackSpeed ( new_value );
  }
}

void PlaybackDevice::OnDefaultValuesChanged::update () {
  PlaybackDevice* node= static_cast<PlaybackDevice*>(getOwner());
  node->updateDefaultValues ();
}