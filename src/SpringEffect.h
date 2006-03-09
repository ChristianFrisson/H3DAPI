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
/// \file SpringEffect.h
/// \brief Header file for SpringEffect
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __SPRINGEFFECT_H__
#define __SPRINGEFFECT_H__

#include "H3DForceEffect.h" 
#include "HapticSpring.h"
#include "SFVec3f.h"
#include "SFBool.h"
#include "SFFloat.h"
#include "SFInt32.h"

namespace H3D {
  /// \ingroup Nodes 
  /// \class SpringEffect
  /// SpringEffect is a localized haptic effect where the haptics device
  /// is pulled towards to a point in space in a spring like manner, i.e.
  /// the force is proportional to the the distance of the haptics device
  /// from the spring position.
  /// The effect will not start until the haptics device is within 
  /// startDistance from the spring position.
  /// The effect will stop when the haptics device is further away than
  /// escapeDistance from the spring position.
  /// deviceIndex decides on which haptics device this effect should be on.
  /// The force generated will be 
  /// force = (position - device_position) * springConstant
  ///
  class H3DAPI_API SpringEffect: public H3DForceEffect {
  public:
    /// Constructor
    SpringEffect( Inst< SFVec3f     > _position = 0,
                  Inst< SFVec3f     > _force = 0,
                  Inst< SFFloat     > _springConstant = 0,
                  Inst< SFFloat     > _startDistance = 0,
                  Inst< SFFloat     > _escapeDistance = 0,
                  Inst< SFBool      > _active = 0, 
                  Inst< SFInt32     > _deviceIndex = 0,
                  Inst< SFNode      >  _metadata = 0 );

    /// Adds the effect if within startDistance and removes it when 
    /// going outside escapeDistance.
    virtual void traverseSG( TraverseInfo &ti );


    /// The position of the spring.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> Vec3f( 0, 0, 0 ) \n
    auto_ptr< SFVec3f > position;

    /// The force applied by the spring to the haptics device since the last
    /// scenegraph update.
    ///
    /// <b>Access type:</b> outputOnly \n
    /// <b>Default value:</b> Vec3f( 0, 0, 0 ) \n
    auto_ptr< SFVec3f > force;
    
    /// The spring constant of the spring. 
    /// force = (position - device_position) * springConstant
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 100 \n
    auto_ptr< SFFloat > springConstant;

    /// When the haptics device comes within this distance the spring effect
    /// is activated.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 0.01 \n
    auto_ptr< SFFloat > startDistance;

    /// If the effect is active, the effect will be deactivated if the 
    /// haptics device moves outside this distance.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 0.01 \n
    auto_ptr< SFFloat > escapeDistance;

    /// Active is true when the spring effect is active.
    ///
    /// <b>Access type:</b> outputOnly \n
    /// <b>Default value:</b> FALSE \n
    auto_ptr< SFBool  > active;  
    
    /// The index of the haptics device that the effect is supposed to
    /// be rendered on.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 0 \n
    auto_ptr< SFInt32 > deviceIndex;  

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
    
    /// Internal haptic spring instance
    AutoRef< HapticSpring > haptic_spring;
  };
}

#endif
