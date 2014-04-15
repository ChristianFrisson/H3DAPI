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
/// \file BooleanFilter.h
/// \brief Header file for BooleanFilter, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __BOOLEANFILTER_H__
#define __BOOLEANFILTER_H__

#include <H3D/X3DChildNode.h>
#include <H3D/SFBool.h>

namespace H3D {

  /// \ingroup X3DNodes
  /// \class BooleanFilter
  /// \brief BooleanFilter filters boolean events, allowing for selective 
  /// routing of TRUE or FALSE values and negation.
  ///
  /// When the set_boolean event is received, the BooleanFilter node
  /// generates two events: either inputTrue or inputFalse, based on the
  /// boolean value received; and inputNegate, which contains the negation
  /// of the value received.
  ///
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/BooleanFilter.x3d">BooleanFilter.x3d</a>
  ///     ( <a href="examples/BooleanFilter.x3d.html">Source</a> )
  ///
  /// \par Internal routes:
  /// \dotfile BooleanFilter.dot
  class H3DAPI_API BooleanFilter : public X3DChildNode {
  public:
    /// This field class takes as input SFBool fields and sets
    /// its value to the negated value of the input field.
    class H3DAPI_API NegateSFBool:  public SFBool {
      virtual void update() {
        BooleanFilter *bf = 
          static_cast< BooleanFilter * >( getOwner() );
        value = 
          !static_cast< SFBool * >( event.ptr )->getValue( bf->id );
      }
    };
#ifdef __BORLANDC__
    friend class NegateSFBool;
#endif

    /// The SetBoolean class is specialize to set the value of either
    /// the inputTrue or inputFalse field in the BooleanFilter node
    /// it resides in, depending on the value that is is given.
    class H3DAPI_API SetBoolean: public AutoUpdate< SFBool > {
    public:
      /// Sets either the inputTrue or inputFalse field depending
      /// on the new value.
      virtual void setValue( const bool &b, int id = 0 ) {
        SFBool::setValue( b );
        BooleanFilter *bf = 
          static_cast< BooleanFilter * >( getOwner() );
        if( value )
          bf->inputTrue->setValue( true, bf->id );
        else 
          bf->inputFalse->setValue( false, bf->id );
      }
    protected:
      /// Sets either the inputTrue or inputFalse field depending
      /// on the new value.
      virtual void update() {
        SFBool::update();
        BooleanFilter *bf = 
          static_cast< BooleanFilter * >( getOwner() );
        if( value )
          bf->inputTrue->setValue( true, bf->id );
        else 
          bf->inputFalse->setValue( false, bf->id );
      }
    };
#ifdef __BORLANDC__
    friend class SetBoolean;
#endif

    /// Constructor.
    BooleanFilter( Inst< SFNode       > _metadata    = 0,
                   Inst< SetBoolean   > _set_boolean = 0,
                   Inst< SFBool       > _inputFalse  = 0,
                   Inst< NegateSFBool > _inputNegate = 0,
                   Inst< SFBool       > _inputTrue   = 0 );

    /// The boolean field to filter.
    /// 
    /// <b>Access type:</b> inputOnly \n
    /// 
    /// \dotfile BooleanFilter_set_boolean.dot
    auto_ptr< SetBoolean > set_boolean;

    /// Generates an FALSE event if the set_boolean value is set to
    /// FALSE.
    /// 
    /// <b>Access type:</b> outputOnly \n
    /// 
    /// \dotfile BooleanFilter_inputFalse.dot
    auto_ptr< SFBool > inputFalse;

    /// This field contains the negation of the set_boolean field.
    /// 
    /// <b>Access type:</b> outputOnly \n
    /// 
    /// \dotfile BooleanFilter_inputNegate.dot
    auto_ptr< NegateSFBool > inputNegate;

    /// Generates an TRUE event if the set_boolean value is set to
    /// TRUE.
    /// 
    /// <b>Access type:</b> outputOnly \n
    /// 
    /// \dotfile BooleanFilter_inputTrue.dot
    auto_ptr< SFBool > inputTrue;

    /// The H3DNodedatabase for this node.
    static H3DNodeDatabase database;
  };
}

#endif
