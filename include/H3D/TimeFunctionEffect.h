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
/// \file TimeFunctionEffect.h
/// \brief Header file for TimeFunctionEffect
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __TIMEFUNCTIONEFFECT_H__
#define __TIMEFUNCTIONEFFECT_H__

#include <H3D/H3DForceEffect.h>
#include <H3D/H3DFunctionNode.h>
#include <H3D/MFInt32.h>

namespace H3D {
  /// \ingroup H3DNodes 
  /// \class TimeFunctionEffect
  /// TimeFunctionEffect takes as input three functions that controls
  /// the force in x, y and z direction. The functions must depend on only
  /// one variable (time), but of course the function expression could be "0".
  ///
  /// A simple example. xFunction, yFunction and zFunction all depends on the
  /// variable "t". xFunction evaluates according to the expression "sin(t)".
  /// yFunction and zFunction always evaluates to 0. This will give the effect
  /// that at time t the force rendered on the haptcis device will be
  /// (sin(t), 0, 0).
  ///
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/TimeFunctionEffect.x3d">TimeFunctionEffect.x3d</a>
  ///     ( <a href="examples/TimeFunctionEffect.x3d.html">Source</a> )
  class H3DAPI_API TimeFunctionEffect: public H3DForceEffect {
  public:

    typedef TypedSFNode< H3DFunctionNode > SFFunctionNode;

    /// Constructor
    TimeFunctionEffect( Inst< SFFunctionNode > _xFunction   = 0,
                        Inst< SFFunctionNode > _yFunction   = 0,
                        Inst< SFFunctionNode > _zFunction   = 0,
                        Inst< MFInt32        > _deviceIndex = 0,
                        Inst< SFNode         > _metadata    = 0 );

    /// Adds the force effect.
    virtual void traverseSG( TraverseInfo &ti );

    /// This function controls the force in the x-direction.
    /// Input to the function will be system time in milliseconds.
    ///
    /// <b>Access type:</b> inputOutput \n
    auto_ptr< SFFunctionNode > xFunction;

    /// This function controls the force in the y-direction.
    /// Input to the function will be system time in milliseconds.
    ///
    /// <b>Access type:</b> inputOutput \n
    auto_ptr< SFFunctionNode > yFunction;
    
    /// This function controls the force in the z-direction.
    /// Input to the function will be system time in milliseconds.
    ///
    /// <b>Access type:</b> inputOutput \n
    auto_ptr< SFFunctionNode > zFunction;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  };
}

#endif
