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
/// \file DynamicTransform.h
/// \brief Header file for DynamicTransform.
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __DYNAMICTRANSFORM_H__
#define __DYNAMICTRANSFORM_H__

#include <H3D/MatrixTransform.h>
#include <H3D/PeriodicUpdate.h>
#include <H3D/RK4.h>
#include <H3D/SFMatrix3f.h>
#include <H3D/SFQuaternion.h>

namespace H3D {

  /// \ingroup H3DNodes
  /// \class DynamicTransform
  /// \brief The DynamicTransform node is a Shape container that has basic
  /// properties for defining rigid body motion. This includes
  /// a position, orientation, mass, etc.
  ///
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/DynamicTransform.x3d">DynamicTransform.x3d</a>
  ///     ( <a href="examples/DynamicTransform.x3d.html">Source</a> )
  ///
  /// \par Internal routes:
  /// \dotfile DynamicTransform.dot
  class H3DAPI_API DynamicTransform : public MatrixTransform {
  public:

    /// Time dependant field to perform the RK4 integration for determining
    /// the motion of the dynamic transform
    /// routes_in[0] time field
    ///
    class H3DAPI_API SFMotion: 
      public  TypedField< H3D::SFVec3f, Types< SFTime > >  {
    public:
      
      SFMotion(): last_t( 0 ) {
      }
      
      H3DTime last_t;
    protected:
      /// Update the matrix from the fields in the Transform node.
      virtual void update();

      virtual void updateState( LMState &state, H3DTime dt );
    };

    /// Specialize the SFMatrix4f to update the matrix from the
    /// fields in the Transform node.
    /// routes_in[0] orientation field
    /// routes_in[1] position field
    ///
    class H3DAPI_API SFMatrix4f: 
      public TypedField< H3D::SFMatrix4f,
                         Types< SFRotation, SFVec3f > >{
    protected:
 
      /// Update the matrix from the fields in the DynamicTransform node.
      virtual void update();
    };

    /// Calculates the velocity based on the mass and momentum. 
    /// vel = mom / mass
    ///
    /// routes_in[0] is the mass
    /// routes_in[1] is the momentum
    class H3DAPI_API SFVelocity: public TypedField< SFVec3f, 
                                                    Types< SFFloat, 
                                                           SFVec3f > > {
      virtual void update() {
        H3DFloat mass = 
          static_cast< SFFloat * >( routes_in[0] )->getValue();
        const Vec3f &momentum = 
          static_cast< SFVec3f * >( routes_in[1] )->getValue();
        value = momentum / mass;
      }
    };

    /// Calculates the spin (time derivative of the orientation)
    ///
    /// routes_in[0] is the angularVelocity
    /// routes_in[1] is the orientation
    class H3DAPI_API SFSpin: public TypedField< SFQuaternion, 
                                                Types< SFVec3f, 
                                                       SFRotation > > {
      virtual void update() {
        const Vec3f & ang_vel = 
          static_cast< SFVec3f * >( routes_in[0] )->getValue();
        const Rotation &orn = 
          static_cast< SFRotation * >( routes_in[1] )->getValue();
        value = 0.5 * Quaternion(ang_vel.x, ang_vel.y, ang_vel.z, 0) * (Quaternion)orn;
      }
    };

    /// Calculates the angular velocity based on the inertia tensor and angular
    /// momentum. 
    ///
    /// routes_in[0] is the inertiaTensor
    /// routes_in[1] is the angularMomentum
    class H3DAPI_API SFAngularVelocity: public TypedField< SFVec3f, 
                                                           Types< SFMatrix3f,
                                                                  SFVec3f > > {
      virtual void update() {
        const Matrix3f &inertia_tensor = 
          static_cast< SFMatrix3f * >( routes_in[0] )->getValue();
        const Vec3f &ang_momentum = 
          static_cast< SFVec3f * >( routes_in[1] )->getValue();
        value = inertia_tensor.inverse() * ang_momentum;
      }
    };

     /// Constructor.
    DynamicTransform( Inst< AddChildren        > _addChildren        = 0,
                      Inst< RemoveChildren     > _removeChildren     = 0,
                      Inst< MFChild            > _children           = 0,
                      Inst< SFNode             > _metadata           = 0,
                      Inst< SFBound            > _bound              = 0,
                      Inst< SFVec3f            > _bboxCenter         = 0,
                      Inst< SFVec3f            > _bboxSize           = 0,
                      Inst< SFTransformedBound > _transformedBound   = 0,
                      Inst< SFMatrix4f         > _matrix             = 0,
                      Inst< SFMatrix4f         > _accumulatedForward = 0,
                      Inst< SFMatrix4f         > _accumulatedInverse = 0,
                      Inst< SFVec3f            > _position           = 0,
                      Inst< SFRotation         > _orientation        = 0,
                      Inst< SFVelocity         > _velocity           = 0,
                      Inst< SFVec3f            > _momentum           = 0,
                      Inst< SFVec3f            > _force              = 0,
                      Inst< SFAngularVelocity  > _angularVelocity    = 0,
                      Inst< SFVec3f            > _angularMomentum    = 0, 
                      Inst< SFSpin             > _spin               = 0,
                      Inst< SFVec3f            > _torque             = 0,
                      Inst< SFFloat            > _mass               = 0,
                      Inst< SFMatrix3f         > _inertiaTensor      = 0,
                      Inst< SFMotion           > _motion             = 0 );

    /// Specifies the position of the shape
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> Vec3f( 0, 0, 0 ) \n
    ///
    /// \dotfile DynamicTransform_position.dot
    auto_ptr<    SFVec3f    >  position;

    /// Specifies the orientation of the shape
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> Rotation( 0, 0, 1, 0 ) \n
    ///
    /// \dotfile DynamicTransform_orientation.dot
    auto_ptr<    SFRotation >  orientation;

    /// Specifies the velocity in m/s
    ///
    /// <b>Access type:</b> outputOnly \n
    ///
    /// \dotfile DynamicTransform_velocity.dot
    auto_ptr<    SFVec3f    >  velocity;
    
    /// Specifies the momentum
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> Vec3f( 0, 0, 0 ) \n
    ///
    /// \dotfile DynamicTransform_momentum.dot
    auto_ptr<    SFVec3f    >  momentum;
    
    /// Specifies the force currently acting on the dynamic
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> Vec3f( 0, 0, 0 ) \n
    ///
    /// \dotfile DynamicTransform_force.dot
    auto_ptr<    SFVec3f    >  force;

    /// Specifies the angular velocity of the dynamic
    ///
    /// <b>Access type:</b> outputOnly \n
    ///
    /// \dotfile DynamicTransform_angularVelocity.dot
    auto_ptr<    SFVec3f    >  angularVelocity;

    /// Specifies the angular momentum of the dynamic
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> Vec3f( 0, 0, 0 ) \n
    ///
    /// \dotfile DynamicTransform_angularMomentum.dot
    auto_ptr<    SFVec3f    >  angularMomentum;

    /// Specifies the spin of the dynamic
    ///
    /// <b>Access type:</b> outputOnly \n
    ///
    /// \dotfile DynamicTransform_spin.dot
    auto_ptr<    SFSpin    >  spin;

    /// Specifies the torque force currently acting on the dynamic
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> Vec3f( 0, 0, 0 ) \n
    ///
    /// \dotfile DynamicTransform_torque.dot
    auto_ptr<    SFVec3f    >  torque;

    /// Specifies the mass, in kg
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 1.0 \n
    ///
    /// \dotfile DynamicTransform_mass.dot
    auto_ptr<    SFFloat    >  mass;

    /// Specifies the inertia tensor
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> Matrix3f( 1, 0, 0, 0, 1, 0, 0, 0, 1 ) \n
    ///
    /// \dotfile DynamicTransform_inertiaTensor.dot
    auto_ptr<    SFMatrix3f  >  inertiaTensor;

    /// Dynamic update algorithm. Only accessable in C++.
    auto_ptr<    SFMotion    >  motion;
    
    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  };
}

#endif
