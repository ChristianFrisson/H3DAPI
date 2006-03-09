//////////////////////////////////////////////////////////////////////////////
//    Copyright 2004, SenseGraphics AB
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

#include "H3DFunctionNode.h"
#include "SFFloat.h"

namespace H3D {
  /// \ingroup Nodes
  /// \brief The function node implements a gaussian function.
  class H3DAPI_API GaussianFunction : public H3DFunctionNode {
  public:
    /// Constructor.       
    GaussianFunction( Inst< SFNode  > _metadata  = 0,
                      Inst< SFFloat > _center    = 0,
                      Inst< SFFloat > _amplitude = 0,
                      Inst< SFFloat > _width     = 0 );    

    /** Get the value as unsigned byte */
    inline unsigned char getChar(unsigned char x){
      return (unsigned char)( 255.0f*get((H3DFloat)(x/255.0f)) );
    }
    /** Get the value as mgFloat */
    H3D::H3DFloat get(H3DFloat x);

    /// The amplitude of the gaussian.
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
