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
/// \file GaussianFunction.h
/// \brief Header file for GaussianFunction
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __GAUSSIANFUNCTION_H__
#define __GAUSSIANFUNCTION_H__

#include <H3D/H3DFunctionNode.h>
#include <H3D/SFFloat.h>

namespace H3D {
  /// \ingroup H3DNodes
  /// \brief The function node implements a gaussian function.
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/DeformableShape.x3d">DeformableShape.x3d</a>
  ///     ( <a href="examples/DeformableShape.x3d.html">Source</a> )
  class H3DAPI_API GaussianFunction : public H3DFunctionNode {
  public:
    /// Constructor.       
    GaussianFunction( Inst< SFNode  > _metadata  = 0,
                      Inst< SFFloat > _center    = 0,
                      Inst< SFFloat > _amplitude = 0,
                      Inst< SFFloat > _width     = 0 );

    /// Evaluate the function for the given input.
    virtual H3DDouble evaluate( H3DDouble *input );

    /// Returns the number of input values the function takes.
    virtual unsigned int nrInputValues() { return 1; };

    /// Returns the function as a HAPIFunctionObject. Should return a new copy
    /// if the H3DFunctionNode stores a copy of HAPIFunctionObject since owner
    /// ship of the returned HAPIFunctionObject should be considered to belong
    /// to the caller of the function. Returns NULL if the H3DFunctionNode can not
    /// be represented as a HAPIFunctionObject.
    virtual HAPI::HAPIFunctionObject *getAsHAPIFunctionObject();

    /// The value for the center of the gaussian.
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 0 \n
    auto_ptr< SFFloat > center;

    /// The amplitude of the gaussian.
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 1 \n
    auto_ptr< SFFloat > amplitude;

    /// The width of the gaussian.
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 1 \n
    auto_ptr< SFFloat > width;

    static H3DNodeDatabase database;
  };
}

#endif
