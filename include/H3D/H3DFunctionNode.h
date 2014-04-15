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
/// \file H3DFunctionNode.h
/// \brief Header file for H3DFunctionNode.
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __H3DFUNCTIONNODE_H__
#define __H3DFUNCTIONNODE_H__

#include <H3D/X3DNode.h>
#include <H3D/MFDouble.h>
#include <H3D/SFDouble.h>

#include <HAPI/HAPIFunctionObject.h>

namespace H3D {

  /// \ingroup H3DNodes 
  /// \class H3DFunctionNode
  /// \brief Base class for nodes evaluating function.
  /// The function is a function R^n -> R.
  /// 
  class H3DAPI_API H3DFunctionNode : public X3DNode {
  public:

    /// Field class that calculates the function value for the given input.
    /// routes_in[0] is the input field
    class H3DAPI_API Value: public TypedField< SFDouble, MFDouble > {
    protected:
      virtual void update();
    };

    /// Constructor.
    H3DFunctionNode( Inst< SFNode   >  _metadata = 0,
                     Inst< MFDouble  > _input     = 0,
                     Inst< Value    > _output     = 0 );

    /// Evaluate the function for the given input.
    virtual H3DDouble evaluate( H3DDouble *input ) = 0;

    /// Shortcut for functions just taking one input 
    inline H3DDouble evaluate( H3DDouble input ) {
      return evaluate( &input ); 
    }

    /// Same as evaluate(). For backwards compatability.
    inline H3DDouble get( H3DDouble input ) {
      return evaluate( &input ); 
    }

    /// Returns the number of input values the function takes.
    virtual unsigned int nrInputValues() = 0;

    /// Returns the function as a HAPIFunctionObject. Should return a new copy
    /// if the H3DFunctionNode stores a copy of HAPIFunctionObject since owner
    /// ship of the returned HAPIFunctionObject should be considered to belong
    /// to the caller of the function. Returns NULL if the H3DFunctionNode can not
    /// be represented as a HAPIFunctionObject.
    virtual HAPI::HAPIFunctionObject *getAsHAPIFunctionObject() { return NULL;}

    virtual string defaultXMLContainerField() {
      return "function";
    }
    
    /// Input value to the function to evaluate.
    /// 
    ///  <b>Access type:</b> inputOnly \n
    auto_ptr< MFDouble > input;

    /// Output value from the function when evaluated for the input values
    /// given in the input field.
    /// 
    ///  <b>Access type:</b> inputOnly \n
    auto_ptr< Value > output;

    /// Field that indicated whether the function has changed. Since
    /// H3DFunctionNode is an abstract type we do not know anything
    /// about which fields will be available. Subclasses must route in
    /// all fields that determines if the properties have changed.
    auto_ptr< Field > functionChanged;
  };
}

#endif
