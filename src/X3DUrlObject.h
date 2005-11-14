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
/// \file X3DUrlObject.h
/// \brief Header file for X3DUrlObject, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __X3DURLOBJECT_H__
#define __X3DURLOBJECT_H__

#include "Node.h"
#include "FieldTemplates.h"

namespace H3D {
  /// \ingroup AbstractInterface
  /// \class X3DUrlObject
  /// \brief This abstract interface is inherited by all nodes that 
  /// contain data located on the World Wide Web, such as AudioClip, 
  /// ImageTexture and Inline. 
  ///
  /// All url fields can hold multiple string values. The strings in these
  /// fields indicate multiple locations to search for data in decreasing
  /// order of preference. If the browser cannot locate or interpret the
  /// data specified by the first location, it shall try the second and
  /// subsequent locations in order until a location containing
  /// interpretable data is encountered. X3D browsers only have to interpret
  /// a single string. If no interpretable locations are found, the node
  /// type defines the resultant default behaviour. 
  /// 
  class H3DAPI_API X3DUrlObject {
  public:

    /// Constructor.
    X3DUrlObject( Inst< MFString>  _url = 0 );

    /// The urls in decreasing order of preference.
    auto_ptr< MFString >  url;
  };
}

#endif
