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
/// \file H3DSurfaceNode.h
/// \brief Header file for H3DSurfaceNode
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __H3DSURFACENODE_H__
#define __H3DSURFACENODE_H__

#include <H3D/H3DApi.h>
#include <H3D/Node.h>
#include <HAPI/HAPISurfaceObject.h>

namespace H3D {

  /// \ingroup AbstractNodes
  /// \brief Base class for all surfaces. A H3DSurfaceNode decides how
  /// an object should feel when you touch it. 
  ///
  /// It can define e.g. stiffnesses and friction.
  ///
  class H3DAPI_API H3DSurfaceNode: public Node {
  public:

    /// Constructor.
    H3DSurfaceNode() : hapi_surface( 0 ) {}
  
    virtual string defaultXMLContainerField() {
      return "surface";
    }

    /// Get the surface to use for the haptics device with the
    /// specified index.
    virtual HAPI::HAPISurfaceObject *getSurface( unsigned int device_index = 0 ) {
      return hapi_surface.get();
    }

  protected:
    AutoRef< HAPI::HAPISurfaceObject > hapi_surface;
  };
}

#endif
