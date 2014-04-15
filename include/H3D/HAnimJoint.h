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
/// \file HAnimJoint.h
/// \brief Header file for HAnimJoint, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __HANIMJOINT_H__
#define __HANIMJOINT_H__

#include <H3D/HAnimDisplacer.h>
#include <H3D/Transform.h>
#include <H3D/MFFloat.h>
#include <H3D/MFInt32.h>

namespace H3D {

  /// \ingroup X3DNodes
  /// \class HAnimJoint
  /// \brief The HAnimJoint object is the fundamental building block that 
  /// represents body parts.
  ///
  /// The center field specifies a translation offset from the root of the
  /// overall humanoid body description and is not intended to 
  /// receive events in most cases. Since all of the HAnimJoint objects
  /// have their centre values defined in the same coordinate frame, the
  /// length of any segment can be determined by calculating the distance
  /// between the parent's joint centre and the child's HAnimJoint object
  /// centre. The exception will be segments at the ends of the fingers and
  /// toes, for which the end effector HAnimSite objects within the 
  /// HAnimSegment object shall be used. 
  ///
  /// The rotation field specifies a rotation of the coordinate system of
  /// the HAnimJoint object. 
  /// 
  /// The scale field specifies a non-uniform scale of the humanoid 
  /// figure coordinate system and the scale values shall be greater than
  /// zero. 
  ///
  /// The scaleOrientation specifies a rotation of the coordinate system
  /// of the humanoid figure before the scale. 
  ///
  /// The scaleOrientation applies only to the scale operation. 
  ///
  /// The translation field specifies a translation to the coordinate system.
  ///
  /// The skinCoordWeight field contains a list of floating point values
  /// between 0.0 and 1.0 that describe an amount of weighting that should
  /// be used to affect a particular vertex from the skinCoord field of the
  /// HAnimHumanoid object.  Each item in this list has a corresponding index
  /// value in the skinCoordIndex field of the HAnimJoint object indicating
  /// exactly which coordinate is to be influenced. 
  ///
  /// The ulimit and llimit fields of the HAnimJoint object specify the upper
  /// and lower joint rotation limits. Both fields are three-element float
  /// sequence containing separate values for the X, Y and Z rotation limits.
  /// The ulimit field stores the upper (i.e. maximum) values for rotation 
  /// around the X, Y and Z axes. The llimit field stores the lower (i.e., 
  /// minimum) values for rotation around those axes. The default values 
  /// for each of these fields is the null set indicating that the joint 
  /// is unconstrained.
  ///
  /// The limitOrientation field gives the orientation of the coordinate
  /// frame in which the ulimit and llimit values are to be interpreted. 
  /// The limitOrientation describes the orientation of a local coordinate
  /// frame, relative to the HAnimJoint object centre position described
  /// by the center field. 
  ///
  /// The stiffness field, if present, contains values ranging between 
  /// 0.0 and 1.0 that provide an inverse kinematics system how 
  /// much each degree of freedom should scale the calculated joint rotation
  /// at each step of the inverse kinematics solver.  
  ///
  /// The name field contains a name  that is used for 
  /// identifying the object.  
  /// 
  /// The displacers field contains a list of HAnimDisplacer objects that are
  /// used to morph the deformable mesh using the morph targets defined
  /// in the Displacer objects.
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/BoxMan.x3d">BoxMan.x3d</a>
  ///     ( <a href="examples/BoxMan.x3d.html">Source</a> )
  ///
  /// \par Internal routes:
  /// \dotfile HAnimJoint.dot
  class H3DAPI_API HAnimJoint : public Transform {
  public:

    /// The MFDisplacer is dependent on the propertyChanged field of the 
    /// contained HAnimDisplacer.
    typedef  DependentMFNode< HAnimDisplacer,
                              FieldRef< X3DGeometricPropertyNode,
                                        Field,
                                        &HAnimDisplacer::propertyChanged > >  
    MFDisplacer;

    /// Constructor.
    HAnimJoint( Inst< AddChildren >        _addChildren        = 0,
                Inst< RemoveChildren >     _removeChildren     = 0,
                Inst< SFVec3f    >         _center             = 0,
                Inst< MFChild    >         _children           = 0,
                Inst< SFNode     >         _metadata           = 0,
                Inst< SFRotation >         _rotation           = 0,
                Inst< SFVec3f    >         _scale              = 0,
                Inst< SFRotation >         _scaleOrientation   = 0,
                Inst< SFVec3f    >         _translation        = 0,
                Inst< SFBound    >         _bound              = 0,
                Inst< SFVec3f    >         _bboxCenter         = 0,
                Inst< SFVec3f    >         _bboxSize           = 0,
                Inst< SFTransformedBound > _transformedBound   = 0,
                Inst< Matrix     >         _matrix             = 0,
                Inst< SFMatrix4f >         _accumulatedForward = 0,
                Inst< SFMatrix4f >         _accumulatedInverse = 0,
                Inst< MFDisplacer >        _displacers         = 0,
                Inst< SFRotation  >        _limitOrientation   = 0,
                Inst< MFFloat     >        _llimit             = 0,
                Inst< MFFloat     >        _ulimit             = 0,
                Inst< SFString    >        _name               = 0,
                Inst< MFInt32     >        _skinCoordIndex     = 0,
                Inst< MFFloat     >        _skinCoordWeight    = 0,
                Inst< MFFloat     >        _stiffness          = 0
                );

    //virtual void render();
    virtual void traverseSG( TraverseInfo &ti );

    /// Settings for how to render joints.
    typedef enum {
      /// Render the joints only as spheres
      JOINTS, 
      /// Render the joints as spheres and connect them with lines in the
      /// hierarchy they have.
      SKELETON
    } RenderType;

    /// Render the joints and skeleton with OpenGL with the specified type.
    void renderSkeleton( RenderType type );

    /// The displacers field contains a list of HAnimDisplacer objects that are
    /// used to morph the deformable mesh using the morph targets defined
    /// in the Displacer objects.
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> [] \n
    /// 
    /// \dotfile HAnimJoint_displacers.dot  
    auto_ptr< MFDisplacer >  displacers;

    /// The limitOrientation field gives the orientation of the coordinate
    /// frame in which the ulimit and llimit values are to be interpreted. 
    /// The limitOrientation describes the orientation of a local coordinate
    /// frame, relative to the HAnimJoint object centre position described
    /// by the center field. 
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> Rotation( 0, 0, 1, 0 ) \n
    /// 
    /// \dotfile HAnimJoint_limitOrientation.dot  
    auto_ptr< SFRotation  >  limitOrientation;

    /// The llimit fields of the HAnimJoint object specify the 
    /// lower joint rotation limits. It is a three-element float
    /// sequence containing separate values for the X, Y and Z rotation limits.
    /// The default values for each of these fields is the null set 
    /// indicating that the joint is unconstrained.
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> [] \n
    /// 
    /// \dotfile HAnimJoint_llimit.dot  
    auto_ptr< MFFloat     >  llimit;

    /// The ulimit fields of the HAnimJoint object specify the 
    /// upper joint rotation limits. It is a three-element float
    /// sequence containing separate values for the X, Y and Z rotation limits.
    /// The default values for each of these fields is the null set 
    /// indicating that the joint is unconstrained.
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> [] \n
    /// 
    /// \dotfile HAnimJoint_llimit.dot 
    auto_ptr< MFFloat     >  ulimit;

    /// The name field contains a name  that is used for 
    /// identifying the object. 
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> "" \n
    /// 
    /// \dotfile HAnimJoint_name.dot 
    auto_ptr< SFString    >  name;

    /// The skinCoordIndex field contains a list of indices into the
    /// skinCoord field of the HAnimHumanoid object defining the
    /// coordinates to be influenced by this joint.
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> [] \n
    /// 
    /// \dotfile HAnimJoint_skinCoordIndex.dot
    auto_ptr< MFInt32     >  skinCoordIndex;

    /// The skinCoordWeight field contains a list of floating point values
    /// between 0.0 and 1.0 that describe an amount of weighting that should
    /// be used to affect a particular vertex from the skinCoord field of the
    /// HAnimHumanoid object.  Each item in this list has a corresponding index
    /// value in the skinCoordIndex field of the HAnimJoint object indicating
    /// exactly which coordinate is to be influenced. 
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> [] \n
    /// 
    /// \dotfile HAnimJoint_skinCoordWeight.dot 
    auto_ptr< MFFloat     >  skinCoordWeight;

    /// The stiffness field, if present, contains values ranging between 
    /// 0.0 and 1.0 that provide an inverse kinematics system how 
    /// much each degree of freedom should scale the calculated joint rotation
    /// at each step of the inverse kinematics solver.  
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> [0 0 0] \n
    /// 
    /// \dotfile HAnimJoint_stiffness.dot 
    auto_ptr< MFFloat     >  stiffness;

    /// The accumulated joint transform from the local coordinate system of 
    /// the joint to HAnimHumanoid local coordinate system
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> [1 0 0 0 
    ///                        0 1 0 0
    ///                        0 0 1 0
    ///                        0 0 0 1 ] \n
    /// 
    /// \dotfile HAnimJoint_accumulatedJointMatrix.dot 
    auto_ptr< SFMatrix4f  >  accumulatedJointMatrix;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
    
  };
}

#endif
