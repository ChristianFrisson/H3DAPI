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
/// \file TouchSensor.h
/// \brief Header file for TouchSensor, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __TOUCHSENSOR_H__
#define __TOUCHSENSOR_H__

#include <H3D/X3DTouchSensorNode.h>
#include <H3D/SFVec2f.h>

namespace H3D {

  /// \ingroup X3DNodes
  /// \class TouchSensor
  /// \brief A TouchSensor node tracks the location and state of the pointing
  /// device and detects when the user points at geometry contained by 
  /// the TouchSensor node's parent group.
  ///
  /// As the user moves the bearing over the TouchSensor node's geometry,
  /// the point of intersection (if any) between the bearing and the geometry
  /// is determined. Each movement of the pointing device, while isOver is
  /// TRUE, generates hitPoint_changed, hitNormal_changed and
  /// hitTexCoord_changed events. hitPoint_changed events contain the 3D point
  /// on the surface of the underlying geometry, given in the TouchSensor
  /// node's coordinate system. hitNormal_changed events contain the surface
  /// normal vector at the hitPoint. hitTexCoord_changed events contain the
  /// texture coordinates of that surface at the hitPoint. The values of
  /// hitTexCoord_changed and hitNormal_changed events are computed as
  /// appropriate for the associated shape.
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/TouchSensor.x3d">TouchSensor.x3d</a>
  ///     ( <a href="examples/TouchSensor.x3d.html">Source</a> )
  ///   - <a href="../../../H3DAPI/examples/All/TouchSensor1.x3d">TouchSensor1.x3d</a>
  ///     ( <a href="examples/TouchSensor1.x3d.html">Source</a> )
  ///   - <a href="../../../H3DAPI/examples/All/TouchSensor2.x3d">TouchSensor2.x3d</a>
  ///     ( <a href="examples/TouchSensor2.x3d.html">Source</a> )
  class H3DAPI_API TouchSensor : 
    public X3DTouchSensorNode {
  public:

    /// Constructor.
    TouchSensor( Inst< SFString > _description = 0,
                        Inst< SFBool >  _enabled  = 0,
                        Inst< SFNode >  _metadata = 0,
                        Inst< SFVec3f > _hitNormal_changed = 0,
                        Inst< SFVec3f > _hitPoint_changed = 0,
                        Inst< SFVec2f > _hitTexCoord_changed = 0,
                        Inst< SFBool >  _isActive = 0,
                        Inst< SFBool > _isOver = 0,
                        Inst< SFTime > _touchTime = 0 );

    // Fields
    /// hitNormal_changed events contain the surface normal vector
    /// at the hitPoint. ( Given in TouchSensors local coordinates )
    /// 
    /// <b>Access type:</b> outputOnly \n
    auto_ptr< SFVec3f > hitNormal_changed;

    /// hitPoint_changed events contain the 3D point on the surface
    /// of the underlying geometry, given in the TouchSensor node's
    /// coordinate system.
    /// 
    /// <b>Access type:</b> outputOnly \n
    auto_ptr< SFVec3f > hitPoint_changed;

    /// hitTexCoord_changed events contain the texture coordinates of
    /// that surface at the hitPoint.
    /// 
    /// <b>Access type:</b> outputOnly \n
    auto_ptr< SFVec2f > hitTexCoord_changed;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;

  protected:
    // Called to generate isOver events and other events which dependens on
    // isOver.
    virtual void onIsOver( IntersectionInfo *result = 0,
                           Matrix4f *global_to_local = 0 );
  };
}
#endif
