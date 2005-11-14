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
/// \file NavigationInfo.h
/// \brief Header file for NavigationInfo, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __NAVIGATIONINFO_H__
#define __NAVIGATIONINFO_H__

#include "X3DBindableNode.h"
#include "FieldTemplates.h"
#include "Exception.h"

namespace H3D {
  /// \ingroup Nodes
  /// \class NavigationInfo
  /// \brief The NavigationInfo node contains information describing 
  /// the physical characteristics of the viewer's avatar and viewing model.
  /// 
  /// Only the visibilityLimit and headlight are used in H3D API.
  /// 
  class H3DAPI_API NavigationInfo : public X3DBindableNode< NavigationInfo > {
  public:
    /// Constructor.
    NavigationInfo( Inst< SFSetBind > _set_bind         = 0,
                    Inst< SFNode    > _metadata         = 0,
                    Inst< SFTime    > _bindTime         = 0,
                    Inst< SFBool    > _isBound          = 0,
                    Inst< MFFloat   > _avatarSize       = 0,
                    Inst< SFBool    > _headlight        = 0,
                    Inst< SFFloat   > _speed            = 0,
                    Inst< MFString  > _transitionType   = 0,
                    Inst< MFString  > _type             = 0,
                    Inst< SFFloat   > _visibilityLimit  = 0 );
    
    /// The avatarSize field specifies the user's physical dimensions 
    /// in the world for the purpose of collision detection and terrain
    /// following.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> [0.25 1.6 0.75] \n
    auto_ptr< MFFloat > avatarSize;

    /// The headlight field specifies whether a headlight should be turned
    /// on. A headlight is a directional light that always points in the
    /// direction the user is looking.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> TRUE \n
    auto_ptr< SFBool > headlight;

    /// The speed field specifies the rate at which the viewer travels
    /// through a scene in metres per second. 
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 1.0 \n
    auto_ptr< SFFloat > speed;

    /// The transitionType field specifies an ordered list of paradigms
    /// that determine the manner in which the browser moves the viewer
    /// when a new Viewpoint node is bound. Not supported by H3D API.
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> "LINEAR: \n
    /// <b>Valid values:</b> "TELEPORT", "LINEAR", "ANIMATE"
    auto_ptr< MFString > transitionType;

    /// The type field specifies an ordered list of navigation paradigms 
    /// that specify a combination of navigation types and the initial
    /// navigation type.
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> ["EXAMINE", "ANY"]\n
    auto_ptr< MFString > type;
    
    /// Geometry beyond the visibilityLimit may not be rendered. A value
    /// of 0.0 indicates an infinite visibility limit. The visibilityLimit
    /// field is restricted to be greater than or equal to zero.
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 0.0 \n
    auto_ptr< SFFloat > visibilityLimit;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  };
}

#endif
