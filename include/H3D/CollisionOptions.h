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
/// \file CollisionOptions.h
/// \brief Header file for CollisionOptions.
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __COLLISIONOPTIONS_H__
#define __COLLISIONOPTIONS_H__

#include <H3D/H3DOptionNode.h>
#include <H3D/SFBool.h>
#include <H3D/SFInt32.h>
#include <H3D/X3DGeometryNode.h>

namespace H3D {

  /// \ingroup H3DNodes
  /// \class CollisionOptions
  /// \brief Specifies parameters to use for graphical rendering of nodes.
  class H3DAPI_API CollisionOptions : public H3DOptionNode {
  public:
    
    /// Constructor.
    CollisionOptions( Inst< SFNode  > _metadata = 0,
                      Inst< SFBool  > _avatarCollision = 0 );
    
    /// The avatarCollision field specifies whether collision between
    /// the avatar and the world should be done. If false then it is possible
    /// to always navigate through every object in the scene. Setting this
    /// option to false will increase rendering performance but will also shut
    /// off the functionality of the Collision node (at least at the moment).
    ///
    /// <b>Default value: </b> true \n
    /// <b>Access type: </b> inputOutput \n
    auto_ptr< SFBool > avatarCollision;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;

  protected:
    
  };
}

#endif
