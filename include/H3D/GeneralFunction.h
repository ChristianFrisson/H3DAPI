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
/// \file GeneralFunction.h
/// \brief Header file for GeneralFunction.
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __GENERALFUNCTION_H__
#define __GENERALFUNCTION_H__

// H3D API includes
#include <H3D/H3DFunctionNode.h>
#include <H3D/SFString.h>

#ifdef HAVE_FPARSER

namespace HAPI {
  // Forward declaration.
  class ParsedFunction;
}

namespace H3D {
  /// \ingroup H3DNodes
  /// \brief The function node implements a general function R^n -> R
  /// by specifying it by a string.
  ///
  /// The function string understood by the class is very similar to the 
  /// C-syntax.
  /// Arithmetic float expressions can be created from float literals, 
  /// variables or functions using the following operators in this order of 
  /// precedence:
  ///
  /// ()             expressions in parentheses first
  /// A^B            exponentiation (A raised to the power B)
  /// -A             unary minus
  /// !A             unary logical not (result is 1 if int(A) is 0, else 0)
  /// A*B  A/B  A%B  multiplication, division and modulo
  /// A+B  A-B       addition and subtraction
  /// A=B  A!=B  A<B  A<=B  A>B  A>=B
  ///                comparison between A and B (result is either 0 or 1)
  /// A&B            result is 1 if int(A) and int(B) differ from 0, else 0
  /// A|B            result is 1 if int(A) or int(B) differ from 0, else 0
  ///
  /// Since the unary minus has higher precedence than any other operator, for
  /// example the following expression is valid: x*-y
  ///
  /// The class supports these functions:
  ///
  /// abs(A)    : Absolute value of A. If A is negative, returns -A otherwise
  ///             returns A.
  /// acos(A)   : Arc-cosine of A. Returns the angle, measured in radians,
  ///             whose cosine is A.
  /// acosh(A)  : Same as acos() but for hyperbolic cosine.
  /// asin(A)   : Arc-sine of A. Returns the angle, measured in radians, whose
  ///             sine is A.
  /// asinh(A)  : Same as asin() but for hyperbolic sine.
  /// atan(A)   : Arc-tangent of (A). Returns the angle, measured in radians,
  ///             whose tangent is (A).
  /// atan2(A,B): Arc-tangent of A/B. The two main differences to atan() is
  ///             that it will return the right angle depending on the signs of
  ///             A and B (atan() can only return values betwen -pi/2 and pi/2),
  ///             and that the return value of pi/2 and -pi/2 are possible.
  /// atanh(A)  : Same as atan() but for hyperbolic tangent.
  /// ceil(A)   : Ceiling of A. Returns the smallest integer greater than A.
  ///             Rounds up to the next higher integer.
  ///  cos(A)   : Cosine of A. Returns the cosine of the angle A, where A is
  ///            measured in radians.
  /// cosh(A)   : Same as cos() but for hyperbolic cosine.
  /// cot(A)    : Cotangent of A (equivalent to 1/tan(A)).
  /// csc(A)    : Cosecant of A (equivalent to 1/sin(A)).
  /// eval(...) : This a recursive call to the function to be evaluated. The
  ///            number of parameters must be the same as the number of parameters
  ///              taken by the function. Must be called inside if() to avoid
  ///              infinite recursion.
  /// exp(A)    : Exponential of A. Returns the value of e raised to the power
  ///            A where e is the base of the natural logarithm, i.e. the
  ///              non-repeating value approximately equal to 2.71828182846.
  ///  floor(A)  : Floor of A. Returns the largest integer less than A. Rounds
  ///              down to the next lower integer.
  ///  if(A,B,C) : If int(A) differs from 0, the return value of this function is B,
  ///              else C. Only the parameter which needs to be evaluated is
  ///              evaluated, the other parameter is skipped; this makes it safe to
  ///              use eval() in them.
  ///  int(A)    : Rounds A to the closest integer. 0.5 is rounded to 1.
  ///  log(A)    : Natural (base e) logarithm of A.
  ///  log10(A)  : Base 10 logarithm of A.
  ///  max(A,B)  : If A>B, the result is A, else B.
  ///  min(A,B)  : If A<B, the result is A, else B.
  ///  sec(A)    : Secant of A (equivalent to 1/cos(A)).
  ///  sin(A)    : Sine of A. Returns the sine of the angle A, where A is
  ///              measured in radians.
  ///  sinh(A)   : Same as sin() but for hyperbolic sine.
  ///  sqrt(A)   : Square root of A. Returns the value whose square is A.
  ///  tan(A)    : Tangent of A. Returns the tangent of the angle A, where A
  ///              is measured in radians.
  ///  tanh(A)   : Same as tan() but for hyperbolic tangent.
  ///
  ///
  ///  Examples of function string understood by the class:
  ///
  ///  "1+2"
  ///  "x-1"
  ///  "-sin(sqrt(x^2+y^2))"
  ///  "sqrt(x*x + y*y)"
  ///
  /// An example of a recursive function is the factorial function:
  ///
  /// "if(n>1, n*eval(n-1), 1)"
  ///
  /// Below are some examples for how to specify fields for some
  /// functions:
  /// Function       Function field Params field
  /// f(x) = 2*x         "2*x"         "x"
  /// f(s,t) = s + t     "s+t"         "s,t"
  /// f(a) = cos(a)      "cos(a)"      "a"
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/TimeFunctionEffect.x3d">TimeFunctionEffect.x3d</a>
  ///     ( <a href="examples/TimeFunctionEffect.x3d.html">Source</a> )
  ///
  /// \par Internal routes:
  /// \dotfile GeneralFunction.dot
  class H3DAPI_API GeneralFunction : public H3DFunctionNode {
  public:
    /// \brief Class for updating an HAPI::ParsedFunction depending
    /// on field input value.
    /// routes_in[0] is the function field
    /// routes_in[1] is the params field
    class H3DAPI_API SFFunctionObject: 
      public TypedField< SField< HAPI::ParsedFunction * >,
                         Types< SFString, SFString > > {
    public:
      /// Constructor.
      SFFunctionObject();

      ~SFFunctionObject();

    protected:
      virtual void update();
    };

    /// Constructor.
    GeneralFunction( Inst< SFNode  > _metadata  = 0,
                     Inst< SFString > _function = 0,
                     Inst< SFString > _param    = 0,
                     Inst< SFFunctionObject > _functionObject = 0 );    


    /// Returns the number of input values the function takes.
    virtual unsigned int nrInputValues();

    /// Returns the function as a HAPIFunctionObject. Should return a new copy
    /// if the H3DFunctionNode stores a copy of HAPIFunctionObject since owner
    /// ship of the returned HAPIFunctionObject should be considered to belong
    /// to the caller of the function. Returns NULL if the H3DFunctionNode can not
    /// be represented as a HAPIFunctionObject.
    virtual HAPI::HAPIFunctionObject *getAsHAPIFunctionObject();

    /// Evaluate the function.
    H3DDouble evaluate(H3DDouble *x );

    /// The function string. E.g.
    /// Function  Function string
    /// f(x) = 2*x         "2*x"        
    /// f(s,t) = s + t     "s+t"         
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> "x" \n
    ///
    /// \dotfile GeneralFunction_function.dot
    auto_ptr< SFString > function;

    /// The function parameter string. Decides the
    /// order of the unknowns when evaluating the function
    /// E.g.
    /// 
    /// f(x) = 2*x         "x"        
    /// f(s,t) = s + t     "s,t"     
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 1 \n
    ///
    /// \dotfile GeneralFunction_params.dot
    auto_ptr< SFString > params;

    /// The H3DNodeDatabase for this object.
    static H3DNodeDatabase database;

  protected:
    /// Field with the function object to use to evaluate the function.
    auto_ptr< SFFunctionObject > functionObject;
  };
}

#endif
#endif
