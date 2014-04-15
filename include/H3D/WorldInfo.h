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
/// \file WorldInfo.h
/// \brief Header file for WorldInfo, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __WORLDINFO_H__
#define __WORLDINFO_H__

#include <H3D/X3DInfoNode.h>
#include <H3D/MFString.h>
#include <H3D/SFString.h>

namespace H3D {

  /// \class WorldInfo
  /// \brief The WorldInfo node contains information about the world. 
  /// This node is strictly for documentation purposes and has no effect on
  /// the visual appearance or behaviour of the world. The title field is
  /// intended to store the name or title of the world so that browsers can
  /// present this to the user (perhaps in the window border). Any other
  /// information about the world can be stored in the info field, such 
  /// as author information, copyright, and usage instructions. This is
  /// the base node type for all nodes that contain only 
  /// information without visual semantics.
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/WorldInfo.x3d">WorldInfo.x3d</a>
  ///     ( <a href="examples/WorldInfo.x3d.html">Source</a> )
  class H3DAPI_API WorldInfo : public X3DInfoNode {
  public:
    /// Constructor.
    WorldInfo( Inst< SFNode   > _metadata = 0,
               Inst< MFString > _info     = 0,
               Inst< SFString > _title    = 0 );

    /// Extra information about the world, e.g. author information, 
    /// copyright, and usage instructions.
    ///
    /// <b>Access type:</b> initializeOnly \n
    /// <b>Default value:</b> [] \n
    auto_ptr< MFString > info;

    /// The title of the world.
    ///
    /// <b>Access type:</b> initializeOnly \n
    /// <b>Default value:</b> "" \n
    auto_ptr< SFString > title;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  };
}

#endif
