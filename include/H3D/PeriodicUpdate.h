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
/// \file PeriodicUpdate.h
/// \brief Header file for PeriodicUpdate, template field modifier
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __PERIODIC_UPDATE_H__
#define __PERIODIC_UPDATE_H__

#include <H3D/Scene.h>

namespace H3D {

  /// Abstract base class for all Fields that will be updated without
  /// anyone asking for a value. Each scene-graph loop the timeToUpdate()
  /// function will be called and if it returns true, the upToDate-function
  /// will be executed.
  class H3DAPI_API PeriodicUpdateField { 
  public:
    /// Destructor.
    virtual ~PeriodicUpdateField() {}

    /// Determines if it is time to check that the field is updated.
    /// This function will be called once per scenegraph loop and
    /// when it returns true the upToDate function will be run in the field.
    virtual bool timeToUpdate() = 0;
  };


  /// The PeriodicUpdate is a template modifier that changes the way the
  /// field is updated. Normally field updates are lazy so that the update
  /// function is only called if an event has been received and someone
  /// is asking for its value. Sometimes it is need however for the field
  /// to keep the value up-to-date always. This can be achieved by the 
  /// AutoUpdate template modifier, however it will update as soon as an 
  /// is received. This can be quite inefficient if in there are several
  /// fields routed to it that changes value and we only want it to update
  /// when all those fields has sent their events. With the PeriodicUpdate
  /// template modifier a field type can be changed so that it updates itself 
  /// periodically as defined by the user. There are two types of periodic
  /// updates, time based and count based. If the PeriodicUpdate
  /// field is set to be time based the field the value specified by setPeriod
  /// defines a time in seconds between updates, e.g. a value of 2 makes it 
  /// update every other second. If it is count based the field will update
  /// itself depending on how many scenegraph loop that has been executed
  /// since the last update. E.g. a value of 2 makes the field update every
  /// second scenegraph loop. The default behavior is for the field to update
  /// every scenegraph loop.
  /// \ingroup FieldTemplateModifiers
  template< class BaseFieldType >
  class PeriodicUpdate: public BaseFieldType,
                        public PeriodicUpdateField {
  public:
    /// Type defining how the period should be interpreted.
    typedef enum {
      TIME, /// The period is specified in seconds
      COUNT /// The period is specified in number of scenegraph loops.
    } PeriodType;
    
    /// Constructor.
    PeriodicUpdate( H3DFloat _period = 0, PeriodType _period_type = COUNT ):
      period_type( _period_type ),
      period( _period ),
      last_up_to_date( TimeStamp() ),
      counter( 0 ) {
      this->route( Scene::eventSink );
    }
    
    /// Destructor.
    ~PeriodicUpdate() {
      this->unroute( Scene::eventSink );
    }

    /// Determines if it is time to check that the field is updated.
    /// This function will be called once per scenegraph loop and
    /// when it returns true the upToDate function will be run in the field.
    virtual bool timeToUpdate() {
      if( period_type == TIME ) {
        return (TimeStamp() - last_up_to_date ) >= period;
      } else {
        if( counter >= period ) {
          counter = 0;
          return true;
        } else {
          ++counter;
          return false;
        }
      }
    }

    /// Set the type of the period.
    void setPeriodType( PeriodType type ) {
      period_type = type;
    }

    /// Set the period for the update.
    void setPeriod( H3DFloat _period ) {
      period = _period;
    }

    /// upToDate is specialized to record the time of the call to the 
    /// function in the last_up_to_date member.
    virtual void upToDate() {
      BaseFieldType::upToDate();
      last_up_to_date = TimeStamp();
    }

  protected:
    
    /// The tye of the period.
    PeriodType period_type;
    /// The period of automatic updates/
    H3DFloat period;
    /// The time of the last call to upToDate.
    TimeStamp last_up_to_date;
    /// The number of calls to timeToUpdate() since the last update.
    unsigned int counter;
  };
}

#endif
