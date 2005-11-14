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
/// \file X3DBoundedObject.h
/// \brief Header file for X3DBoundedObject.
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __X3DBOUNDEDOBJECT_H__
#define __X3DBOUNDEDOBJECT_H__

#include "H3DBoundedObject.h"

namespace H3D {
  /// \ingroup AbstractInterface
  /// \brief This X3D abstract interface type is basis for all X3D node
  /// types that have  bounds specified as part of the definition.
  ///
  /// The bboxCenter and bboxSize fields specify a bounding box that 
  /// encloses the Node. This is a hint that may be used for optimization
  /// purposes. The results are undefined if the specified bounding box is
  /// smaller than the actual bounding box of the Node at any time.
  /// A default bboxSize value, (-1, -1, -1), implies that the bounding box
  /// is not specified and, if needed, is calculated automatically. 
  ///
  class H3DAPI_API X3DBoundedObject: public H3DBoundedObject {
  public:
    X3DBoundedObject( Inst< SFBound >_bound = 0,
                      Inst< SFVec3f >  _bboxCenter = 0,
                      Inst< SFVec3f >  _bboxSize = 0 ):
      H3DBoundedObject( _bound ),
      bboxCenter( _bboxCenter ),
      bboxSize( _bboxSize ) {

      bboxCenter->setName( "bboxCenter" );
      bboxSize->setName( "bboxSize" );

      bboxCenter->setValue( Vec3f( 0, 0, 0 ) );
      bboxSize->setValue( Vec3f( -1, -1, -1 ) );
    }

    /// Specifies a the center of an axis-aligned bounding box 
    /// enclosing the node. 
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> Vec3f( 0, 0, 0 ) \n
    auto_ptr< SFVec3f >  bboxCenter;

    /// Specifies a the size of an axis-aligned bounding box 
    /// enclosing the node.
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> Vec3f( -1, -1, -1 ) \n
    auto_ptr< SFVec3f >  bboxSize;

  };
}

#endif
