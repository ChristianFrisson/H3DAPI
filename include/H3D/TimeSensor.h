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
/// \file TimeSensor.h
/// \brief Header file for TimeSensor, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __TIMESENSOR_H__
#define __TIMESENSOR_H__

#include <H3D/X3DTimeDependentNode.h>
#include <H3D/SFFloat.h>

namespace H3D {

  /// \ingroup X3DNodes
  /// \class TimeSensor
  /// \brief TimeSensor nodes generate events as time passes. 
  ///
  /// TimeSensor nodes can be used for many purposes including: 
  ///
  ///  - driving continuous simulations and animations; 
  ///  - controlling periodic activities (e.g., one per minute); 
  ///  - initiating single occurrence events such as an alarm clock. 
  ///
  /// The TimeSensor node contains two discrete outputOnly fields: 
  /// isActive and cycleTime. The isActive outputOnly field sends TRUE when
  /// the TimeSensor node begins running, and FALSE when it stops running. 
  /// The cycleTime outputOnly field sends a time event at startTime and at 
  /// the beginning of each new cycle (useful for synchronization with other 
  /// time-based objects). The remaining outputOnly fields generate continuous
  /// events. The fraction_changed outputOnly field, an SFFloat in the closed
  /// interval [0,1], sends the completed fraction of the current cycle.
  /// The time outputOnly field sends the absolute time for a given 
  /// simulation tick.
  ///
  /// If the enabled field is TRUE, the TimeSensor node is enabled and may
  /// be running. If a set_enabled FALSE event is received while the 
  /// TimeSensor node is running, the sensor performs the following actions:
  /// - evaluates and sends all relevant outputs;
  /// - sends a FALSE value for isActive;
  /// - disables itself.
  /// 
  /// Input events on the fields of the TimeSensor node (e.g., set_startTime)
  /// are processed and their corresponding outputOnly fields 
  /// (e.g., startTime_changed) are sent regardless of the state of the
  /// enabled field. The remaining discussion assumes enabled is TRUE.
  ///
  /// The "cycle" of a TimeSensor node lasts for cycleInterval seconds. 
  /// The value of cycleInterval shall be greater than zero.
  ///
  /// A cycleTime outputOnly field can be used for synchronization purposes
  /// such as sound with animation. The value of a cycleTime event will be
  /// equal to the time at the beginning of the current cycle. A cycleTime
  /// event is generated at the beginning of every cycle, including the cycle
  /// starting at startTime. The first cycleTime event for a TimeSensor node
  /// can be used as an alarm (single pulse at a specified time).
  ///
  /// When a TimeSensor node becomes active, it generates an isActive = TRUE
  /// event and begins generating time, fraction_changed, and cycleTime events
  /// which may be routed to other nodes to drive animation or simulated
  /// behaviours. The behaviour at read time is described below. The time
  /// event sends the absolute time for a given tick of the TimeSensor node
  /// (SFTime/MFTime fields and events represent the number of seconds since
  /// midnight GMT January 1, 1970).
  ///
  /// fraction_changed events output a floating point value in the closed 
  /// interval [0, 1]. At startTime the value of fraction_changed is 0. After
  /// startTime, the value of fraction_changed in any cycle will progress
  /// through the range (0.0, 1.0]. At startTime + N × cycleInterval, for 
  /// N = 1, 2, ..., (i.e., at the end of every cycle), the value of 
  /// fraction_changed is 1.
  ///
  /// A TimeSensor node can be set up to be active at read time by specifying
  /// loop TRUE (not the default) and stopTime less than or equal to startTime
  /// (satisfied by the default values). The time events output absolute
  /// times for each tick of the TimeSensor node simulation. The time events 
  /// shall start at the first simulation tick greater than or equal to 
  /// startTime. time events end at stopTime, or at startTime + N × 
  /// cycleInterval for some positive integer value of N, or loop forever
  /// depending on the values of the other fields. An active TimeSensor node 
  /// shall stop at the first simulation tick when now = stopTime > startTime.
  ///
  /// No guarantees are made with respect to how often a TimeSensor node
  /// generates time events, but a TimeSensor node shall generate events at
  /// least at every simulation tick. TimeSensor nodes are guaranteed to 
  /// generate final time and fraction_changed events. If loop is FALSE at the
  /// end of the Nth cycleInterval and was TRUE at startTime + M cycleInterval
  /// for all 0 < M < N, the final time event will be generated with a value
  /// of (startTime + N × cycleInterval) or stopTime (if stopTime > startTime),
  /// whichever value is less. If loop is TRUE at the completion of every 
  /// cycle, the final event is generated as evaluated at stopTime (if
  /// stopTime > startTime) or never.
  /// 
  /// An active TimeSensor node ignores set_cycleInterval and set_startTime 
  /// events. An active TimeSensor node also ignores set_stopTime events for
  /// set_stopTime less than or equal to startTime. For example, if a
  /// set_startTime event is received while a TimeSensor node is active, that
  /// set_startTime event is ignored (the startTime field is not changed, and
  /// a startTime_changed event is not generated). If an active TimeSensor
  /// node receives a set_stopTime event that is less than the current time,
  /// and greater than startTime, it behaves as if the stopTime requested is
  /// the current time and sends the final events based on the current time
  /// (note that stopTime is set as specified in the field).
  /// 
  /// A TimeSensor read from a X3D file shall generate isActive TRUE, time 
  /// and fraction_changed events if the sensor is enabled and all conditions
  /// for a TimeSensor to be active are met.
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/Switch.x3d">Switch.x3d</a>
  ///     ( <a href="examples/Switch.x3d.html">Source</a> )
  ///
  class H3DAPI_API TimeSensor : public X3DTimeDependentNode {
  public:

    /// The TimeHandler field is specialized to include the handling
    /// of the TimeSensor specific fields.
    class H3DAPI_API TimeHandler: public X3DTimeDependentNode::TimeHandler {
    protected:
      /// Set the fields that need to be set due to the time changes
      /// since last loop.
      virtual void update();
      /// Activate the time node. Start generating time based events.
      virtual void activate( H3DTime time );
      /// The time that have elapsed in the current cycle.
      H3DTime elapsed_cycle_time;
    };
#ifdef __BORLANDC__
    friend class TimeHandler;
#endif

    /// The CycleInterval has the same conditions for update
    /// as the StartTime so use that field class definition.
    typedef StartTime CycleInterval;

    /// Constructor.
    TimeSensor( Inst< SFNode  > _metadata         = 0,
                Inst< SFBool  > _loop             = 0,
                Inst< SFTime  > _pauseTime        = 0,
                Inst< SFTime  > _resumeTime       = 0,
                Inst< StartTime > _startTime        = 0,
                Inst< StopTime  > _stopTime         = 0,
                Inst< SFTime  > _elapsedTime      = 0,
                Inst< SFBool  > _isActive         = 0,
                Inst< SFBool  > _isPaused         = 0,
                Inst< TimeHandler > _timeHandler = 0, 
                Inst< CycleInterval > _cycleInterval    = 0,
                Inst< SFTime  > _cycleTime        = 0,
                Inst< SFBool  > _enabled          = 0,
                Inst< SFFloat > _fraction_changed = 0,
                Inst< SFTime  > _time             = 0 );

    /// The cycleInterval field defines how long a cycle lasts in seconds.
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 1 \n
    auto_ptr< CycleInterval  >  cycleInterval;

    /// The cycleTime  field sends a time event at startTime and at 
    /// the beginning of each new cycle with the current time.
    ///
    /// <b>Access type:</b> outputOnly \n
    auto_ptr< SFTime  >  cycleTime;

    /// If the enabled field is TRUE, the TimeSensor node is enabled and may
    /// be running, otherwise no field updates are made.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> TRUE \n
    auto_ptr< SFBool  >  enabled;

    /// The fraction_changed field is the completed fraction of the
    /// current cycle, i.e. a value in the interval [0-1].
    ///
    /// <b>Access type:</b> outputOnly \n
    auto_ptr< SFFloat >  fraction_changed;

    /// The time field sends the absolute time for a given 
    /// simulation tick( i.e. for this scene-graph loop). 
    ///
    /// <b>Access type:</b> outputOnly \n
    auto_ptr< SFTime  >  time;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  protected:
    bool activate_through_enabled;
    bool previous_enabled;
  };
}

#endif
