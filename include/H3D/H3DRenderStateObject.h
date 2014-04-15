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
/// \file H3DRenderStateObject.h
/// \brief Header file for H3DRenderStateObject.
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __H3DRENDERSTATEOBJECT_H__
#define __H3DRENDERSTATEOBJECT_H__

#include <H3D/H3DApi.h>

namespace H3D {
  /// \ingroup AbstractInterface
  /// \brief This abstract class is the base class for node types that 
  /// want to change the rendering state temporarily while rendering 
  /// sibling nodes in a X3DGroupingNode. 
  ///
  /// If put into the children field 
  /// of a X3DGroupingNode the enable*() function of all H3DRenderStateObjects
  /// will be called first,
  /// then the render()/traverseSG() function for all the children and
  /// then lastly the disable*() function for all H3DRenderStateObjects. 
  class H3DAPI_API H3DRenderStateObject {
  public:  
    /// Destructor.
    virtual ~H3DRenderStateObject(){}

    /// Enable the states that you want to before all other nodes of a 
    /// X3DGroupingNode's children field are rendered.
    virtual void enableGraphicsState() {}

    /// Disable the states that were enabled in the enableGraphicsState() 
    /// function.
    virtual void disableGraphicsState() {}

    /// Enable the states that you want to before all other nodes of a 
    /// X3DGroupingNode's children field are traversed with traverseSG.
    virtual void enableHapticsState( TraverseInfo &ti ) {}

    /// Reset the states that were changed in the enableHapticsState() 
    /// function
    virtual void disableHapticsState( TraverseInfo &ti ) {}
  };
}

#endif
