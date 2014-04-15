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
/// \file IntegerSequencer.h
/// \brief Header file for IntegerSequencer, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __INTEGERSEQUENCER_H__
#define __INTEGERSEQUENCER_H__

#include <H3D/X3DSequencerNode.h>
#include <H3D/MFInt32.h>
#include <H3D/SFInt32.h>

namespace H3D {

  /// \ingroup X3DNodes
  /// \class IntegerSequencer
  /// \brief The IntegerSequencer node generates sequential discrete SFInt32 
  /// events in response to each set_fraction, next, or previous event.
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/Switch.x3d">Switch.x3d</a>
  ///     ( <a href="examples/Switch.x3d.html">Source</a> )
  ///
  /// \par Internal routes:
  /// \dotfile IntegerSequencer.dot 
  class H3DAPI_API IntegerSequencer : public X3DSequencerNode {
  public:

    /// Constructor.
    IntegerSequencer( Inst< SFNode                 > _metadata      = 0,
                      Inst< SFBool                 > _next          = 0,
                      Inst< SFBool                 > _previous     = 0,
                      Inst< SFFloat                > _set_fraction  = 0,
                      Inst< MFFloat                > _key           = 0,
                      Inst< KeyValues < MFInt32 >  > _keyValue      = 0,
                      Inst< ValueChanged < SFInt32, MFInt32,
                                           IntegerSequencer > >
                        _value_changed  = 0 );

    /// The keyValue field is made up of a list of integer values.
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> - \n
    /// 
    /// \dotfile IntegerSequencer_keyValue.dot 
    auto_ptr< KeyValues < MFInt32 > > keyValue;

    /// Each value in the keyValue field corresponds in order to the parameter
    /// value in the key field. The keys shall be monotonically non-decreasing,
    /// otherwise the results are undefined.
    /// 
    /// <b>Access type:</b> outputOnly \n
    /// <b>Default value:</b> - \n
    /// 
    /// \dotfile IntegerSequencer_value_changed.dot 
    auto_ptr< ValueChanged < SFInt32, MFInt32, IntegerSequencer > >
      value_changed;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  };
}

#endif
