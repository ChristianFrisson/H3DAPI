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
/// \file CoordinateDeformer.h
/// \brief Header file for CoordinateDeformer.
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __COORDINATEDEFORMER_H__
#define __COORDINATEDEFORMER_H__

#include <H3DCoordinateDeformerNode.h>
#include <H3DFunctionNode.h>
#include <SFFloat.h>
#include <MFBool.h>

namespace H3D {

  /// \ingroup H3DNodes
  /// \brief The CoordinateDeformer uses a H3DFunctionNode to determine
  /// the deformation.
  ///
  /// The distanceToDepth field specifies a function from the distance
  /// from the point of contact to the depth of the deformation. The depth
  /// is defined as a float 0 is no deformation at all, and 1 will result in
  /// a deformation that is as deep as penetration depth of the surface
  /// (i.e. the distance the haptics device has penetrated the surface.
  ///
  /// The plasticity field defines the plasticity. 0 means that the 
  /// deformation is non-plastic (resting points will not be changed) and
  /// 1 means that it is fully plasting (resting points will be the same
  /// as deformed points)
  class H3DAPI_API CoordinateDeformer : public H3DCoordinateDeformerNode {
  public:

    typedef TypedSFNode< H3DFunctionNode > SFFunctionNode;

    /// Constructor.       
    CoordinateDeformer( Inst< SFFunctionNode > _distanceToDepth  = 0,
                        Inst< SFFloat        > _plasticity       = 0 ):
      distanceToDepth( _distanceToDepth ),
      plasticity( _plasticity ),
      touched_last_time( false ) {

      type_name = "CoordinateDeformer";
      database.initFields( this );

      plasticity->setValue( 0 );
    }

    /// The deformation of the points will be calculated depending on
    /// the distanceToDepth field and the plasticity field.
    virtual void deformPoints( const vector< bool  > &is_touched,
                               const vector< Vec3f > &touch_point,
                               const vector< Vec3f > &touch_normal,
                               const vector< Vec3f > &touch_force,
                               const vector< Vec3f > &penetration_points,
                               const vector< Vec3f > &orig_points,
                               const vector< Vec3f > &resting_points,
                               const vector< Vec3f > &deformed__points,
                               vector< Vec3f > &new_resting_points,
                               vector< Vec3f > &new_deformed_points );

    /// The distanceToDepth field specifies a function from the distance
    /// from the point of contact to the depth of the deformation. The depth
    /// is defined as a float 0 is no deformation at all, and 1 will result in
    /// a deformation that is as deep as penetration depth of the surface
    /// (i.e. the distance the haptics device has penetrated the surface.
    ///
    /// <b> Access type: </b> inputOutput
    auto_ptr< SFFunctionNode > distanceToDepth;

    /// The plasticity field defines the plasticity. 0 means that the 
    /// deformation is non-plastic (resting points will not be changed) and
    /// 1 means that it is fully plasting (resting points will be the same
    /// as deformed points)    
    ///
    /// <b> Access type: </b> inputOutput
    /// <b> Default value: </b> 0.0
    auto_ptr< SFFloat > plasticity;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;

  protected:
    bool touched_last_time;
  };
}

#endif
