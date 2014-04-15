
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
/// \file CoordBoundField.h
/// \brief Header file for CoordBoundField, specialiced field class.
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __COORDBOUNDFIELD_H__
#define __COORDBOUNDFIELD_H__

#include <H3D/H3DBoundedObject.h>
#include <H3D/X3DCoordinateNode.h>

namespace H3D {

  /// The CoordBoundField is specialized update itself from a X3DCoordinateNode.
  /// The resulting bound is a BoxBound encapsulating all the points.
  /// 
  class H3DAPI_API CoordBoundField: 
    public TypedField< H3DBoundedObject::SFBound,
                       TypedSFNode< X3DCoordinateNode > > {
  protected:
    /// Sets up a new BoxBound from the given coordinates.
    virtual void update();
  };
}


#endif
