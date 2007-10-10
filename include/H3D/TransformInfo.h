//////////////////////////////////////////////////////////////////////////////
//    Copyright 2004-2007, SenseGraphics AB
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
/// \file TransformInfo.h
/// \brief Header file for TransformInfo.
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __TRANSFORMINFO_H__
#define __TRANSFORMINFO_H__

#include <H3D/X3DChildNode.h>
#include <H3D/SFMatrix4f.h>

namespace H3D {

  /// \ingroup H3DNodes
  /// \brief The TransformInfo node provides information about the coordinate
  /// space it resides in in the scene graph. 
  /// 
  /// The accForwardMatrix field is the matrix that transforms from the local
  /// coordinate space where this Node resides in the scenegraph to 
  /// global space.
  /// 
  /// The accInverseMatrix field is the matrix that transforms from the 
  /// global coordinate space to the coordinate space this Node resides
  /// in in the scenegraph.
  class H3DAPI_API TransformInfo : 
    public X3DChildNode {
  public:
    /// Constructor.
    TransformInfo( Inst< SFNode     > _metadata  = 0,
                   Inst< SFMatrix4f > _accForwardMatrix = 0,
                   Inst< SFMatrix4f > _accInverseMatrix = 0 );

    /// Set the matrix fields from the values provided in ti.
    virtual void traverseSG( TraverseInfo &ti );

    /// The accForwardMatrix field is the matrix that transforms from the local
    /// coordinate space where this Node resides in the scenegraph to 
    /// global space.
    /// 
    /// <b>Access type:</b> outputOnly \n
    auto_ptr< SFMatrix4f >  accForwardMatrix;

    /// The accInverseMatrix field is the matrix that transforms from the 
    /// global coordinate space to the coordinate space this Node resides
    /// in in the scenegraph.
    /// 
    /// <b>Access type:</b> outputOnly \n
    auto_ptr< SFMatrix4f >  accInverseMatrix;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  };
}

#endif
