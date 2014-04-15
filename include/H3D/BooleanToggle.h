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
/// \file BooleanToggle.h
/// \brief Header file for BooleanToggle, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __BOOLEANTOGGLE_H__
#define __BOOLEANTOGGLE_H__

#include <H3D/X3DChildNode.h>
#include <H3D/SFBool.h>

namespace H3D {

  /// \ingroup X3DNodes
  /// \class BooleanToggle
  /// \brief BooleanToggle stores a boolean value for toggling on/off.
  /// When a set_boolean TRUE event is received, the BooleanToggle negates 
  /// the value of the toggle field and generates the corresponding toggle
  /// field output event. set_boolean FALSE events are ignored.
  /// The BooleanToggle can be reset to a specific state by directly setting
  /// the value of the inputOutput toggle field.
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/BooleanToggle.x3d">BooleanToggle.x3d</a>
  ///     ( <a href="examples/BooleanToggle.x3d.html">Source</a> )
  ///
  /// \par Internal routes:
  /// \dotfile BooleanToggle.dot
  class H3DAPI_API BooleanToggle : public X3DChildNode {
  public:

    /// The SetBoolean class is specialize to negate the value of the
    /// toggle field if a true event is given.
    class H3DAPI_API SetBoolean: public AutoUpdate< SFBool > {
    public:
      // Negates the toggle field if true.
      virtual void setValue( const bool &b, int id = 0 ) {
        SFBool::setValue( b );
        BooleanToggle *bf = 
          static_cast< BooleanToggle * >( getOwner() );
        if( value )
          bf->toggle->setValue( !( bf->toggle->getValue() ) );
      }
    protected:
      // Negates the vaule of the toggle field if true.
      virtual void update() {
        SFBool::update();
        BooleanToggle *bf = 
          static_cast< BooleanToggle * >( getOwner() );
        if( value )
          bf->toggle->setValue( !( bf->toggle->getValue() ) );
      }
    };
#ifdef __BORLANDC__
    friend class SetBoolean;
#endif

    /// Constructor.
    BooleanToggle( Inst< SFNode     > _metadata    = 0,
                   Inst< SetBoolean > _set_boolean = 0,
                   Inst< SFBool     > _toggle      = 0 );

    /// Negates the value of the toggle field.
    /// 
    /// <b>Access type:</b> inputOnly \n
    /// 
    /// \dotfile BooleanToggle_set_boolean.dot
    auto_ptr< SetBoolean > set_boolean;

    /// The boolean field to toggle.
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> FALSE \n
    /// 
    /// \dotfile BooleanToggle_toggle.dot
    auto_ptr< SFBool > toggle;

    /// The H3DNodedatabase for this node.
    static H3DNodeDatabase database;
  };
}

#endif
