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
/// \file ToggleGroup.h
/// \brief Header file for ToggleGroup.
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __TOGGLEGROUP_H__
#define __TOGGLEGROUP_H__

#include "X3DGroupingNode.h"

namespace H3D {

  /// \ingroup Groups
  /// \class ToggleGroup
  /// \brief The ToggleGroup node enables you to enable/disable haptics and 
  /// graphics rendering of its children nodes.
  ///
  /// The hapticsOn field specifies if nodes in the children field of 
  /// this group should be rendered haptically or not.
  ///
  /// The graphicsOn field specifies if nodes in the children field of 
  /// this group should be rendered graphically or not.
  class H3DAPI_API ToggleGroup : public X3DGroupingNode {
  public:
    /// Constructor.
    ToggleGroup( Inst< MFChild  > _addChildren    = 0,
                 Inst< MFChild  > _removeChildren = 0,
                 Inst< MFChild > _children       = 0,
                 Inst< SFNode  > _metadata       = 0,
                 Inst< SFBound > _bound          = 0,
                 Inst< SFVec3f > _bboxCenter     = 0,
                 Inst< SFVec3f > _bboxSize       = 0,
                 Inst< SFBool  > _hapticsOn      = 0,
                 Inst< SFBool  > _graphicsOn     = 0 );

    /// Haptics is disabled if hapticsOn is false.
    virtual void traverseSG( TraverseInfo &ti );

    /// Only renders children if graphicsOn is true. 
    virtual void render();

    /// The hapticsOn field specifies if nodes in the children field of 
    /// this group should be rendered haptically or not.
    ///
    /// <b>Default value:</b> TRUE /n
    /// <b>Access type: </b> inputOutput
    auto_ptr< SFBool >  hapticsOn;

    /// The graphicsOn field specifies if nodes in the children field of 
    /// this group should be rendered graphically or not.
    ///
    /// <b>Default value:</b> TRUE /n
    /// <b>Access type: </b> inputOutput
    auto_ptr< SFBool >  graphicsOn;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  };
}

#endif
