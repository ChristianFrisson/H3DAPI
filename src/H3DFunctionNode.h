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
/// \file H3DFunctionNode.h
/// \brief Header file for H3DFunctionNode.
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __H3DFUNCTIONNODE_H__
#define __H3DFUNCTIONNODE_H__

#include "X3DNode.h"

namespace H3D {
  class H3DAPI_API H3DFunctionNode : public X3DNode {
  public:
    
    /// Constructor.
    H3DFunctionNode( Inst< SFNode>  _metadata = 0 ) :
      functionChanged( new Field ) {}

    /// Get the value as unsigned byte
    virtual unsigned char getChar(unsigned char x) = 0;
    ///
    virtual H3DFloat get(H3DFloat x) = 0;
    
    virtual string defaultXMLContainerField() {
      return "function";
    }
    
    /// Field that indicated whether the function has changed. Since
    /// H3DFunctionNode is an abstract type we do not know anything
    /// about which fields will be available. Subclasses must route in
    /// all fields that determines if the properties have changed.
    auto_ptr< Field > functionChanged;
  };
}

#endif
