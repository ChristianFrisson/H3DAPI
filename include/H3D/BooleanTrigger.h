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
/// \file BooleanTrigger.h
/// \brief Header file for BooleanTrigger, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __BOOLEANTRIGGER_H__
#define __BOOLEANTRIGGER_H__

#include <H3D/X3DTriggerNode.h>
#include <H3D/SFTime.h>
#include <H3D/SFBool.h>

namespace H3D {

  /// \ingroup X3DNodes
  /// \class BooleanTrigger
  /// \brief BooleanTrigger is a trigger node that generates boolean events 
  /// upon receiving time events.
  ///
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/BooleanTrigger.x3d">BooleanTrigger.x3d</a>
  ///     ( <a href="examples/BooleanTrigger.x3d.html">Source</a> )
  ///
  /// \par Internal routes:
  /// \dotfile BooleanTrigger.dot

  class H3DAPI_API BooleanTrigger : public X3DTriggerNode {
  public:

    /// If a time event is given, generate a true event.
    class H3DAPI_API SetBoolean: public TypedField < SFBool, SFTime > {
    protected:
      // always returns true
      virtual void update() {
        value = true;
      }
    };
#ifdef __BORLANDC__
    friend class SetBoolean;
#endif

    /// Constructor.
    BooleanTrigger( Inst< SFNode > _metadata        = 0,
                    Inst< SFTime > _set_triggerTime = 0,
                    Inst< SetBoolean > _triggerTrue  = 0);

    /// The triggerTrue event is generated when the 
    /// BooleanTrigger receives a set_triggerTime event.
    /// 
    /// <b>Access type:</b> inputOnly \n
    /// 
    /// \dotfile BooleanTrigger_set_triggerTime.dot
    auto_ptr< SFTime > set_triggerTime;

    /// Gives the boolean event.
    /// The value of triggerTrue shall always be TRUE.
    ///
    /// <b>Access type:</b> outputOnly \n
    /// 
    /// \dotfile BooleanTrigger_set_triggerTime.dot
    auto_ptr< SetBoolean > triggerTrue;

    /// The H3DNodedatabase for this node.
    static H3DNodeDatabase database;
  };
}

#endif
