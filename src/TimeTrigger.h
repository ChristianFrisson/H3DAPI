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
/// \file TimeTrigger.h
/// \brief Header file for TimeTrigger, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __TIMETRIGGER_H__
#define __TIMETRIGGER_H__

#include "X3DTriggerNode.h"
#include "SFBool.h"
#include "SFTime.h"

namespace H3D {

  /// \ingroup X3DNodes
  /// \class TimeTrigger
  /// \brief TimeTrigger is a trigger node that generates
	/// time events upon receiving boolean events.
  ///
  /// \par Internal routes:
  /// \dotfile TimeTrigger.dot
  
	class TimeTrigger : public X3DTriggerNode {
  public:

		/// The SetTriggerTime class is specialize to set the vaule of
		/// the triggerTime field to the current time.
    class SetTriggerTime: public AutoUpdate< TypedField < SFTime, SFBool > > {
    protected:
      virtual void update() {
        TimeTrigger *bf = 
          static_cast< TimeTrigger * >( getOwner() );
				TimeStamp theTime;
				value = theTime;
      }
    };

		friend class SetTriggerTime;

    /// Constructor.
    TimeTrigger( Inst< SFNode					> _metadata			= 0,
								 Inst< SFBool					> _set_boolean  = 0,
								 Inst< SetTriggerTime > _triggerTime	= 0 );

		/// When an event is received store the current 
		/// time in triggerTime.
    /// 
    /// <b>Access type:</b> inputOnly \n
    /// 
    /// \dotfile TimeTrigger__set_boolean.dot
    auto_ptr< SFBool > set_boolean;

    /// Contains the current time when an event has been generated
		///
		/// <b>Access type:</b> outputOnly \n
    /// 
    /// \dotfile TimeTrigger_triggerTime.dot
    auto_ptr< SetTriggerTime > triggerTime;

		/// The H3DNodedatabase for this node.
    static H3DNodeDatabase database;
  };
}

#endif
