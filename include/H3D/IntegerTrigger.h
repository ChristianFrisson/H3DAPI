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
/// \file IntegerTrigger.h
/// \brief Header file for IntegerTrigger, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __INTEGERTRIGGER_H__
#define __INTEGERTRIGGER_H__

#include <H3D/X3DTriggerNode.h>
#include <H3D/SFBool.h>
#include <H3D/SFInt32.h>

namespace H3D {

  /// \ingroup X3DNodes
  /// \class IntegerTrigger
  /// \brief IntegerTrigger handles single field Boolean 
  /// events to set an integer value for the output event. Upon receiving a 
  /// set_boolean event, the IntegerTrigger node will generate a triggerValue
  /// event with the current value of integerKey. 
  /// This is useful for connecting environmental events to 
  /// the Switch node's whichChoice.
  ///
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/IntegerTrigger.x3d">IntegerTrigger.x3d</a>
  ///     ( <a href="examples/IntegerTrigger.x3d.html">Source</a> )
  ///
  /// \par Internal routes:
  /// \dotfile IntegerTrigger.dot
  
  class H3DAPI_API IntegerTrigger : public X3DTriggerNode {
  public:

    /// The SetTriggerValue class is specialize to set the vaule of
    /// the triggerValue field to the value of the field integerKey.
    class H3DAPI_API SetTriggerValue: public AutoUpdate< TypedField < SFInt32, SFBool > > {
    protected:
      virtual void update() {
        IntegerTrigger *bf = 
          static_cast< IntegerTrigger * >( getOwner() );
        value =  bf->integerKey->getValue();
      }
    };
#ifdef __BORLANDC__
    friend class SetTriggerValue;
#endif

    /// Constructor.
    IntegerTrigger( Inst< SFNode          > _metadata      = 0,
                    Inst< SFBool          > _set_boolean  = 0,
                    Inst< SFInt32          > _integerKey    = 0,
                    Inst< SetTriggerValue > _triggerValue = 0);

    /// When an event is recieved generate triggerValue event with current 
    /// value of integerKey 
    /// 
    /// <b>Access type:</b> inputOnly \n
    /// 
    /// \dotfile IntegerTrigger_set_boolean.dot
    auto_ptr< SFBool > set_boolean;

    /// Contains the value triggerValue will be set to.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// 
    /// \dotfile IntegerTrigger_integerKey.dot
    auto_ptr< SFInt32 > integerKey;

    /// TriggerValue will be set if a set_boolean event is recieved.
    ///
    /// <b>Access type:</b> outputOnly \n
    /// 
    /// \dotfile IntegerTrigger_triggerValue.dot
    auto_ptr< SetTriggerValue > triggerValue;

    /// The H3DNodedatabase for this node.
    static H3DNodeDatabase database;
  };
}

#endif
