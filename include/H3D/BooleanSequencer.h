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
/// \file BooleanSequencer.h
/// \brief Header file for BooleanSequencer, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __BOOLEANSEQUENCER_H__
#define __BOOLEANSEQUENCER_H__

#include <H3D/X3DSequencerNode.h>
#include <H3D/MFBool.h>

namespace H3D {

  /// \ingroup X3DNodes
  /// \class BooleanSequencer
  /// \brief BooleanSequencer generates sequential SFBool events when driven 
  /// from a TimeSensor clock. Among other actions, it can enable/disable 
  /// lights and sensors, or bind/unbind Viewpoints and other 
  /// X3DBindableNodes via set_bind events.
  /// A BooleanSequencer shall be instanced for every node enabled or bound.
  ///
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/BooleanSequencer.x3d">BooleanSequencer.x3d</a>
  ///     ( <a href="examples/BooleanSequencer.x3d.html">Source</a> )
  ///
  /// \par Internal routes:
  /// \dotfile BooleanSequencer.dot 
  class H3DAPI_API BooleanSequencer : public X3DSequencerNode {
  public:

    /// Constructor.
    BooleanSequencer( Inst< SFNode               >  _metadata      = 0,
                      Inst< SFBool               > _next          = 0,
                      Inst< SFBool               > _previous     = 0,
                      Inst< SFFloat              > _set_fraction  = 0,
                      Inst< MFFloat              > _key           = 0,
                      Inst< KeyValues < MFBool > > _keyValue      = 0,
                      Inst< ValueChanged < SFBool, MFBool, BooleanSequencer > >
                        _value_changed = 0 );
               

    /// The keyValue field is made up of a list of FALSE and TRUE values
    /// which can be used for timed on and off switching.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> - \n
    /// 
    /// \dotfile BooleanSequencer_keyValue.dot 
    auto_ptr< KeyValues < MFBool > > keyValue;

    /// Each value in the keyValue field corresponds in order to the parameter
    /// value in the key field. The keys shall be monotonically non-decreasing,
    /// otherwise the results are undefined.
    ///
    /// <b>Access type:</b> outputOnly \n
    /// <b>Default value:</b> - \n
    /// 
    /// \dotfile BooleanSequencer_value_changed.dot 
    auto_ptr< ValueChanged < SFBool, MFBool,
                             BooleanSequencer > > value_changed;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  };
}

#endif
