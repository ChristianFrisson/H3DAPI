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
/// \file HapticLayeredGroup.h
/// \brief Header file for HapticLayeredGroup
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __HAPTICLAYEREDGROUP_H__
#define __HAPTICLAYEREDGROUP_H__

#include <H3D/X3DGroupingNode.h>

namespace H3D {

  /// \ingroup X3DNodes
  /// \class HapticLayeredGroup
  /// \brief HapticLayeredGroup is a grouping node that can be
  /// used to set different shapes for different layers of the haptic
  /// rendering. This can be used in order to simulate for example
  /// skin with bone under it. The skin would be simulated as a shape
  /// with a surface with low stiffness and inside it would be another
  /// shape using a harder surface (surface with higher stiffness).
  ///
  /// All childs of HapticLayeredGroup will be rendered in separate layers.
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/HapticLayeredGroup.x3d">HapticLayeredGroup.x3d</a>
  ///     ( <a href="examples/HapticLayeredGroup.x3d.html">Source</a> )
  class H3DAPI_API HapticLayeredGroup : public X3DGroupingNode {
  public:
    /// Constructor.
    HapticLayeredGroup( Inst< AddChildren    > _addChildren     = 0,
                        Inst< RemoveChildren > _removeChildren  = 0,
                        Inst< MFChild        > _children        = 0,
                        Inst< SFNode         > _metadata        = 0,
                        Inst< SFBound        > _bound           = 0,
                        Inst< SFVec3f        > _bboxCenter      = 0,
                        Inst< SFVec3f        > _bboxSize        = 0 );

    /// Handles the layering.
    virtual void traverseSG( TraverseInfo &ti );

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  };
}

#endif
