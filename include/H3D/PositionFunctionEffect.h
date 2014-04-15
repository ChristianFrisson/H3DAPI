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
/// \file PositionFunctionEffect.h
/// \brief Header file for PositionFunctionEffect
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __POSITIONFUNCTIONEFFECT_H__
#define __POSITIONFUNCTIONEFFECT_H__

#include <H3D/H3DForceEffect.h>
#include <H3D/H3DFunctionNode.h>
#include <H3D/MFInt32.h>

namespace H3D {
  /// \ingroup H3DNodes 
  /// \class PositionFunctionEffect
  /// PositionFunctionEffect takes as input three functions that controls
  /// the force in x, y and z direction. The functions must depend on three
  /// variables, but of course not all of them have to be used in the function
  /// expression. That is, a function can be "0" and depend on the variables
  /// "r,s,t". The input to the functions will be the position of the haptics
  /// device (x, y, z).
  ///
  /// A simple example. xFunction, yFunction and zFunction all depends on the
  /// variables "x,y,z". xFunction evaluates according to the expression "y".
  /// yFunction and zFunction always evaluates to 0. This will give the effect
  /// that when the haptics device position is (x1, y1, z1) there will be a
  /// force (y1, 0, 0) rendered on the haptics device provided that the haptics
  /// device is only affected by the PositionFunctionEffect.
  ///
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/PositionFunctionEffect.x3d">PositionFunctionEffect.x3d</a>
  ///     ( <a href="examples/PositionFunctionEffect.x3d.html">Source</a> )
  class H3DAPI_API PositionFunctionEffect: public H3DForceEffect {
  public:

    typedef TypedSFNode< H3DFunctionNode > SFFunctionNode;

    /// Constructor
    PositionFunctionEffect( Inst< SFFunctionNode > _xFunction   = 0,
                            Inst< SFFunctionNode > _yFunction   = 0,
                            Inst< SFFunctionNode > _zFunction   = 0,
                            Inst< MFInt32        > _deviceIndex = 0,
                            Inst< SFNode         > _metadata    = 0 );

    /// Adds the force effect.
    virtual void traverseSG( TraverseInfo &ti );

    /// This function controls the force in the x-direction.
    /// Input to the function will be the position of the haptics
    /// device (x, y, z).
    ///
    /// <b>Access type:</b> inputOutput \n
    auto_ptr< SFFunctionNode > xFunction;

    /// This function controls the force in the y-direction.
    /// Input to the function will be the position of the haptics
    /// device (x, y, z).
    ///
    /// <b>Access type:</b> inputOutput \n
    auto_ptr< SFFunctionNode > yFunction;
    
    /// This function controls the force in the z-direction.
    /// Input to the function will be the position of the haptics
    /// device (x, y, z).
    ///
    /// <b>Access type:</b> inputOutput \n
    auto_ptr< SFFunctionNode > zFunction;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  };
}

#endif
