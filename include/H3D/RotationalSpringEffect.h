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
/// \file RotationalSpringEffect.h
/// \brief Header file for RotationalSpringEffect
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __ROTATIONALSPRINGEFFECT_H__
#define __ROTATIONALSPRINGEFFECT_H__

#include <H3D/H3DForceEffect.h> 
#include <HAPI/HapticRotationalSpring.h>
#include <H3D/SFVec3f.h>
#include <H3D/MFVec3f.h>
#include <H3D/SFBool.h>
#include <H3D/SFFloat.h>
#include <H3D/SFInt32.h>

namespace H3D {
  /// \ingroup H3DNodes 
  /// \class RotationalSpringEffect
  /// RotationalSpringEffect is a localized haptic effect where the haptics device
  /// is rotated towards a certain axis of rotation in a spring like manner i.e.
  /// the torque is proportional to the the angle between the desired axis and
  /// the rotated default axis ( 0, 0, 1 ). The default axis is rotated by the
  /// haptics device orientation.
  /// The effect will not start until the haptics device is within 
  /// startDistance from the spring position.
  /// The effect will stop when the haptics device is further away than
  /// escapeDistance from the spring position.
  /// deviceIndex decides on which haptics device this effect should be on.
  /// The force generated will be 
  /// force = (position - device_position) * springConstant - 
  /// damping * device_velocity
  ///
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/RotationalSpringEffect.x3d">RotationalSpringEffect.x3d</a>
  ///     ( <a href="examples/RotationalSpringEffect.x3d.html">Source</a> )
  class H3DAPI_API RotationalSpringEffect: public H3DForceEffect {
  public:
    /// Constructor
    RotationalSpringEffect( Inst< SFVec3f     > _defaultAxis    = 0,
                            Inst< MFVec3f     > _torque         = 0,
                            Inst< SFFloat     > _springConstant = 0,
                            Inst< SFBool      > _enabled        = 0,
                            Inst< MFInt32     > _deviceIndex    = 0,
                            Inst< SFNode      >  _metadata      = 0,
                            Inst< SFFloat     > _damping        = 0 );

    /// Adds the effect if within startDistance and removes it when 
    /// going outside escapeDistance.
    /// \param ti The TraverseInfo object containing information about the
    /// traversal.
    virtual void traverseSG( TraverseInfo &ti );

    /// The desired axis of the spring, that is the axis towards which the
    /// torque should be directed.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> Vec3f( 0, 0, 1 ) \n
    auto_ptr< SFVec3f > desiredAxis;

    /// The torque applied by the spring to the haptics device since the last
    /// scenegraph update.
    ///
    /// <b>Access type:</b> outputOnly \n
    /// <b>Default value:</b> Vec3f( 0, 0, 0 ) \n
    auto_ptr< MFVec3f > torque;
    
    /// The spring constant of the spring. 
    /// torque = -rotation_diff_euler_angles * spring_constant -
    /// damping * device_angular_velocity.
    /// Where the rotation_diff_euler_angles are the euler angles for
    /// the rotation from the rotated default axis to the desired axis.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 100 \n
    auto_ptr< SFFloat > springConstant;

    /// The enabled field should be set to true when the force effect should be
    /// enabled and active.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> TRUE \n
    auto_ptr< SFBool > enabled;

    /// The damping constant to use in the torque calculation.
    ///
    /// torque = -rotation_diff_euler_angles * spring_constant -
    /// damping * device_angular_velocity.
    /// Where the rotation_diff_euler_angles are the euler angles for
    /// the rotation from the rotated default axis to the desired axis.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 0 \n
    auto_ptr< SFFloat > damping;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
    
    /// Internal haptic spring instance
    AutoRefVector< HAPI::HapticRotationalSpring > haptic_rotational_spring;
  protected:
    bool last_enabled;
  };
}

#endif
