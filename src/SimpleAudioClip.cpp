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
/// \file SimpleAudioClip.cpp
/// \brief CPP file for SimpleAudioClip.
///
//
//////////////////////////////////////////////////////////////////////////////


#include <H3D/SimpleAudioClip.h>
#include <H3DUtil/TimeStamp.h>

using namespace H3D;

H3DNodeDatabase SimpleAudioClip::database( 
        "SimpleAudioClip", 
        &(newInstance< SimpleAudioClip > ),
        typeid( SimpleAudioClip ),
        &AudioClip::database 
        );

namespace SimpleAudioClipInternals {
  FIELDDB_ELEMENT( SimpleAudioClip, play, INPUT_OUTPUT );
  FIELDDB_ELEMENT( SimpleAudioClip, stop, INPUT_OUTPUT );
  FIELDDB_ELEMENT( SimpleAudioClip, pause, INPUT_OUTPUT );
  FIELDDB_ELEMENT( SimpleAudioClip, audioState, OUTPUT_ONLY );
}


SimpleAudioClip::SimpleAudioClip( 
                              Inst< SFString>  _description       ,
                              Inst< SFBool  >  _loop              ,
                              Inst< SFNode  >  _metadata          ,
                              Inst< SFTime  >  _pauseTime         ,
                              Inst< SFFloat >  _pitch             ,
                              Inst< SFTime  >  _resumeTime        ,
                              Inst< StartTime  >  _startTime      ,
                              Inst< StopTime  >  _stopTime        ,
                              Inst< MFString>  _url               ,
                              Inst<  SFTime  >  _duration_changed ,
                              Inst<  SFTime  >  _elapsedTime      ,
                              Inst<  SFBool  >  _isActive         ,
                              Inst<  SFBool  >  _isPaused         ,
                              Inst< TimeHandler > _time_handler   ,
                              Inst< SFBool       > _play          ,
                              Inst< SFBool       > _stop          ,
                              Inst< SFBool       > _pause         ,
                              Inst< SFString     > _audioState ):
  AudioClip( _description, _loop, _metadata, _pauseTime, _pitch, _resumeTime,
    _startTime, _stopTime, _url, _duration_changed, _elapsedTime, _isActive,
    _isPaused, _time_handler),
  play( _play ),
  stop( _stop ),
  pause( _pause ),
  audioState( _audioState ),
  updateAudioPlay( new UpdateAudioPlay ){
  type_name = "SimpleAudioClip";
  database.initFields( this );

  updateAudioPlay->setName( "updateAudioPlay" );
  updateAudioPlay->setOwner( this );
  
  audioState->addValidValue("STOPPED");
  audioState->addValidValue("PLAYING");
  audioState->addValidValue("PAUSED");
  audioState->setValue( "STOPPED", id );

  play->routeNoEvent( updateAudioPlay, id );
  stop->routeNoEvent( updateAudioPlay, id );
  pause->routeNoEvent( updateAudioPlay, id );
  url->routeNoEvent( updateAudioPlay, id );
  isActive->routeNoEvent( updateAudioPlay, id );

}


void SimpleAudioClip::UpdateAudioPlay::update(){
  SimpleAudioClip* sac = static_cast< SimpleAudioClip* >( getOwner() );
  // play
  if( event.ptr == routes_in[0] ) {
    // if currently is paused, resume , if not, directly start
    if( sac->isPaused->getValue() ) {
      sac->resumeTime->setValue( TimeStamp() );
    }else{
      sac->startTime->setValue( TimeStamp() );
    }
    sac->audioState->setValue( "PLAYING", sac->id );
    sac->play->setValue( false, sac->id );
  }
  // stop
  else if( event.ptr == routes_in[1] ) {
    sac->stopTime->setValue( TimeStamp() );
    sac->audioState->setValue( "STOPPED", sac->id );
    sac->stop->setValue( false, sac->id );
  }
  // pause
  else if( event.ptr == routes_in[2] ) {
    sac->pauseTime->setValue( TimeStamp() );
    sac->audioState->setValue( "PAUSED", sac->id );
    sac->pause->setValue( false, sac->id );
  }
  
  // url change
  else if( event.ptr == routes_in[3] ) {
    sac->audioState->setValue( "STOPPED", sac->id );
  }
  // isActive change
  else if( event.ptr == routes_in[4] ) {
    if (sac->audioState->getValue() == "PLAYING"&&!sac->isActive->getValue()){
      sac->audioState->setValue( "STOPPED", sac->id );
    }
  }
}
