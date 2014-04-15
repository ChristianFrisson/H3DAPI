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
/// \file Group.h
/// \brief Header file for Group, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __GROUP_H__
#define __GROUP_H__

#include <H3D/X3DGroupingNode.h>

namespace H3D {

  /// \ingroup X3DNodes
  /// \class Group
  /// \brief A Group node contains children nodes without introducing a new
  ///  transformation. It is equivalent to a Transform node containing an
  ///  identity transform. 
  ///
  /// The bboxCenter and bboxSize fields specify a bounding box that
  /// encloses the Group node's children. This is a hint that may be used
  /// for optimization purposes. The results are undefined if the specified
  /// bounding box is smaller than the actual bounding box of the children
  /// at any time. A default bboxSize value, (-1, -1, -1), implies that the
  /// bounding box is not specified and, if needed, is calculated by the
  /// browser. 
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/Group.x3d">Group.x3d</a>
  ///     ( <a href="examples/Group.x3d.html">Source</a> )
  class H3DAPI_API Group : public X3DGroupingNode {
  public:
    /// Constructor.
    Group( Inst< AddChildren    > _addChildren    = 0,
           Inst< RemoveChildren > _removeChildren = 0,
           Inst< MFChild        > _children       = 0,
           Inst< SFNode         > _metadata       = 0,
           Inst< SFBound        > _bound          = 0,
           Inst< SFVec3f        > _bboxCenter     = 0,
           Inst< SFVec3f        > _bboxSize       = 0 );

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  };
}

#endif
