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
/// \file LoadSensor.cpp
/// \brief cpp file for LoadSensor, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////

#include <LoadSensor.h>

using namespace H3D;

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase LoadSensor::database("LoadSensor", 
                                    &(newInstance<LoadSensor>), 
                                    typeid( LoadSensor ),
                                    &X3DNetworkSensorNode::database );

namespace LoadSensorInternals {
  FIELDDB_ELEMENT( LoadSensor, timeOut, INPUT_OUTPUT );
  FIELDDB_ELEMENT( LoadSensor, watchList, INPUT_OUTPUT );
  FIELDDB_ELEMENT( LoadSensor, isLoaded, OUTPUT_ONLY );
  FIELDDB_ELEMENT( LoadSensor, loadTime, OUTPUT_ONLY );
  FIELDDB_ELEMENT( LoadSensor, progress, OUTPUT_ONLY );

}

LoadSensor::LoadSensor( Inst< SFBool > _enabled ,
                        Inst< SFNode > _metadata,
                        Inst< SFBool > _isActive,
                        Inst< SFTime > _timeOut ,
                        Inst< MFUrlObject > _watchList,
                        Inst< SFBool > _isLoaded,
                        Inst< SFTime > _loadTime,
                        Inst< SFFloat > _progress ) :
  X3DNetworkSensorNode( _enabled, _metadata, _isActive ),
  watchList( _watchList ),
  timeOut( _timeOut ),
  isLoaded( _isLoaded ),
  loadTime( _loadTime ),
  progress( _progress ),
  loading_last_loop( false ),
  finished_loading( false ) {
  type_name = "LoadSensor";
  database.initFields( this );
  
  timeOut->setValue( 0 );
  isLoaded->setValue( false, id );
  loadTime->setValue( 0, id );
  progress->setValue( 0, id );
}

void LoadSensor::traverseSG( TraverseInfo &ti ) {
  X3DNetworkSensorNode::traverseSG( ti );
  
  if( enabled->getValue() ) {

    H3DFloat load_progress = 1;
    bool all_loaded = true;
    bool is_active = true;
    bool failed = false;
    
    for( MFUrlObject::const_iterator i = watchList->begin();
         i != watchList->end(); i++ ) {
      X3DUrlObject *url_object = dynamic_cast< X3DUrlObject * >(*i);
	  if( url_object ) {
      X3DUrlObject::LoadStatus status = url_object->loadStatus();
      all_loaded = all_loaded && ( status == X3DUrlObject::LOADED );
      is_active = is_active && ( status == X3DUrlObject::LOADED ||
                                 status == X3DUrlObject::LOADING );
      failed = failed || ( status == X3DUrlObject::FAILED );
      if( url_object->loadProgress() < load_progress )
        load_progress = url_object->loadProgress();
	  }
    }
    
    if( !isActive->getValue() && is_active && !all_loaded ) {
      activation_time = TimeStamp();
      isActive->setValue( true, id );
      finished_loading = false;
    }
    
    H3DTime time_out = timeOut->getValue();
    H3DTime current_time = TimeStamp();
    H3DTime time_elapsed = activation_time - current_time;
    
    if( !finished_loading ) {
      if( all_loaded  ) {
        loadTime->setValue( current_time, id );
        progress->setValue( 1, id );
        isLoaded->setValue( true, id );
        isActive->setValue( false, id );
        finished_loading = true;
      } else if( ( time_out != 0 && time_elapsed > time_out ) || failed ) {
        isActive->setValue( false, id );
        isLoaded->setValue( false, id );
        finished_loading = true;
      }
    }
  }
}
 
