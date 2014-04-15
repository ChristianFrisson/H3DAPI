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
/// \file X3DTimeDependentNode.h
/// \brief Header file for X3DTimeDependentNode, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __X3DTIMEDEPENDENTNODE_H__
#define __X3DTIMEDEPENDENTNODE_H__

#include <H3D/X3DChildNode.h>
#include <H3D/SFTime.h>
#include <H3D/SFBool.h>

namespace H3D {

  /// \class X3DTimeDependentNode
  /// \brief This abstract node type is the base node type from which all
  /// time-dependent nodes are derived. 
  class H3DAPI_API X3DTimeDependentNode : public X3DChildNode {
  public:
    
    /// The TimeHandler field updates once per scene-graph loop to 
    /// monitor the current time and updates the fields in the
    /// owner X3DTimeDependentNode if any state changes has to be 
    /// made. It sets fields in the X3DTimeDependentNode accordingly.
    ///
    /// routes_in[0] Scene.time
    class H3DAPI_API TimeHandler: public AutoUpdate< SFTime > {
    public:

      /// Activate the time node. Start generating time based events.
      virtual void activate( H3DTime time );

      /// Deactivate the time node. No events will be generated after 
      /// a call to this function.
      virtual void deactivate( H3DTime time );

    protected:
      /// This function handles all logic for state changes and 
      /// field updates to the X3DTimeDependentNode. It will be called
      /// once per scene-graph loop to set the fields in the time node
      /// depending on the current time.
      virtual void update();
    };
#ifdef __BORLANDC__
    friend class TimeHandler;
#endif

    /// The StartTime field is a specialization of the SFTime field
    /// so that the value cannot be changed when the isActive field 
    /// of the X3DTimeDependentNode it resides in is true. This is 
    /// according to the X3D spec.
    class StartTime: public SFTime {
    public:
      /// Overrides the setValue function so that the value is only
      /// changed if the owner X3DTimeDependentNode is inactive. 
      virtual void setValue( const H3DDouble &time, int id = 0 ) {
        X3DTimeDependentNode *time_node = 
          static_cast< X3DTimeDependentNode * >( getOwner() );
        if( !time_node->isActive->getValue() ) {
          SFTime::setValue( time, id );
        }
      } 

    protected:
      /// Overrides the propagateEvent function so that the event is only
      /// propagated if the owner X3DTimeDependentNode is inactive. 
      virtual void propagateEvent( Event e ) {
        X3DTimeDependentNode *time_node = 
          static_cast< X3DTimeDependentNode * >( getOwner() );
        if( !time_node->isActive->getValue() ) {
          SFTime::propagateEvent( e );
        }
      }
    };


    /// The StopTime field is a specialization of the SFTime field
    /// so that the value is ignored if the X3DTimeDependentNode is active
    /// and stopTime <= startTime as according to the X3D spec.
    class StopTime: public SFTime {
    public:
     
      /// Update is overriden to deactivate the time node if the 
      /// value of the field updates to the to the current time.
      virtual void update() {
        SFTime::update();
        X3DTimeDependentNode *time_node = 
          static_cast< X3DTimeDependentNode * >( getOwner() );
        if( time_node->isActive->getValue() &&
            time_node->timeHandler->getValue() == value ) {
          time_node->timeHandler->deactivate( value );
        } 
      }
    };
#ifdef __BORLANDC__
    friend class StopTime;
#endif

    /// Constructor.
    X3DTimeDependentNode( Inst< SFNode > _metadata    = 0,
                          Inst< SFBool > _loop        = 0,
                          Inst< SFTime > _pauseTime   = 0,
                          Inst< SFTime > _resumeTime  = 0,
                          Inst< StartTime > _startTime   = 0,
                          Inst< StopTime  > _stopTime    = 0,
                          Inst< SFTime > _elapsedTime = 0,
                          Inst< SFBool > _isActive    = 0,
                          Inst< SFBool > _isPaused    = 0,
                          Inst< TimeHandler > _timeHandler = 0 );
    
    /// Activates the node if the conditions for a time node to be active
    /// from the start are met. 
    virtual void initialize();

    /// This function will be called when the node is paused.
    virtual void onPause() {}

    /// This function will be called when the node is resumed after
    /// it has been in paused state.
    virtual void onResume() {}

    /// This function will be called when the time node is started.
    virtual void onStart() {}

    /// This function will be called when the time node is stopped.
    virtual void onStop() {}

    /// If loop is TRUE at the end of a cycle, execution continues into
    /// the next cycle, otherwise it stops.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> FALSE \n
    auto_ptr< SFBool > loop;

    /// A time at which to pause and stop generating events. While an 
    /// active time-dependent node is paused, it generates a TRUE isPaused
    /// event and ceases to generate all other output events, while 
    /// maintaining (or "freezing") its state (holding the last output
    /// values and the clock's internal time when the pausing conditions
    /// are met).
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 0 \n
    auto_ptr< SFTime > pauseTime;

    /// A time at which to start generating events again after being
    /// paused.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 0 \n
    auto_ptr< SFTime > resumeTime;

    /// A time-dependent node is inactive until its startTime is reached.
    /// When current time becomes greater than or equal to startTime, 
    /// an isActive TRUE event is generated and the time-dependent node
    /// becomes active.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 0 \n
    auto_ptr< StartTime > startTime;

    /// An active time-dependent node will become inactive when stopTime
    /// is reached if stopTime > startTime. The value of stopTime is ignored
    /// if stopTime = startTime.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 0 \n
    auto_ptr< StopTime > stopTime;

    /// The elapsedTime outputOnly field delivers the current elapsed
    /// time since the X3DTimeDependentNode was activated and running, 
    /// cumulative in seconds and not counting any time while in a paused
    /// state.
    ///
    /// <b>Access type:</b> outputOnly \n
    auto_ptr< SFTime > elapsedTime;

    /// An isActive TRUE event is generated when the node becomes active and
    /// an isActive FALSE event is generated when it becomes inactive. 
    /// These are the only times at which an isActive event is generated.
    /// In particular, isActive events are not sent at each tick of a 
    /// simulation.
    ///
    /// <b>Access type:</b> outputOnly \n
    auto_ptr< SFBool > isActive;

    /// An isPaused TRUE event is generated when the node becomes paused and
    /// an isPaused FALSE event is generated when it is resumed. 
    ///
    /// <b>Access type:</b> outputOnly \n
    auto_ptr< SFBool > isPaused;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;

  protected:
    auto_ptr< TimeHandler > timeHandler;
  };
}

#endif
