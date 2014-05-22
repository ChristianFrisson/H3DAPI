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
/// \file VisibilitySensor.h
/// \brief Header file for VisibilitySensor, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __VISIBILITYSENSOR_H__
#define __VISIBILITYSENSOR_H__

#include <H3D/X3DEnvironmentalSensorNode.h>
#include <H3D/SFVec3f.h>
#include <GL/glew.h>

namespace H3D {

  /// \ingroup AbstractNodes
  /// \class VisibilitySensor
  /// \brief  The VisibilitySensor node detects visibility changes of a
  ///  rectangular box as the user navigates the world. VisibilitySensor
  ///  is typically used to detect when the user can see a specific object
  ///  or region in the scene in order to activate or deactivate some 
  ///  behaviour or animation. The purpose is often to attract the 
  ///  attention of the user or to improve performance.
  ///
  ///  The enabled field enables and disables the VisibilitySensor node.
  ///  If enabled is set to FALSE, the VisibilitySensor node does not 
  ///  send events. If enabled is TRUE, the VisibilitySensor node detects
  ///  changes to the visibility status of the box specified and sends 
  ///  events through the isActive field. A TRUE event is output to isActive
  ///  when any portion of the box impacts the rendered view. A FALSE event
  ///  is sent when the box has no effect on the view. Browsers shall 
  ///  guarantee that, if isActive is FALSE, the box has absolutely no 
  ///  effect on the rendered view. Browsers may err liberally when isActive
  ///  is TRUE. For example, the box may affect the rendering.
  ///
  ///  The fields center and size specify the object space location of the
  ///  box centre and the extents of the box (i.e., width, height, and depth).
  ///  The VisibilitySensor node's box is affected by hierarchical 
  ///  transformations of its parents. The components of the size field
  ///  shall be greater than or equal to zero.
  ///
  ///  The enterTime event is generated whenever the isActive TRUE event
  ///  is generated, and exitTime events are generated whenever isActive
  ///  FALSE events are generated. A VisibilitySensor read from an X3D 
  ///  file shall generate isActive TRUE and enterTime events if the sensor
  ///  is enabled and the visibility box is visible. A VisibilitySensor 
  ///  inserted into the transformation hierarchy shall generate isActive
  ///  TRUE and enterTime events if the sensor is enabled and the visibility
  ///  box is visible. A VisibilitySensor removed from the transformation 
  ///  hierarchy shall generate isActive FALSE and exitTime events if the
  ///  sensor is enabled and the visibility box is visible.
  ///
  ///  Each VisibilitySensor node behaves independently of all other 
  ///  VisibilitySensor nodes. Every enabled VisibilitySensor node 
  ///  that is affected by the user's movement receives and sends events,
  ///  possibly resulting in multiple VisibilitySensor nodes receiving 
  ///  and sending events simultaneously. Unlike TouchSensor nodes, there
  ///  is no notion of a VisibilitySensor node lower in the scene graph 
  ///  "grabbing" events. Multiply instanced VisibilitySensor nodes 
  ///  (i.e., DEF/USE) use the union of all the boxes defined by their 
  ///  instances. An instanced VisibilitySensor node shall detect visibility
  ///  changes for all instances of the box and send events appropriately.
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/VisibilitySensor.x3d">VisibilitySensor.x3d</a>
  ///     ( <a href="examples/VisibilitySensor.x3d.html">Source</a> )
  ///
  /// \par Internal routes:
  /// \dotfile VisibilitySensor.dot
  class H3DAPI_API VisibilitySensor : public X3DEnvironmentalSensorNode {
  public:

    /// Constructor.
    VisibilitySensor( 
      Inst< SFNode >  _metadata   = 0,
      Inst< SFVec3f > _center     = 0,
      Inst< SFVec3f > _size       = 0,
      Inst< SFTime >  _enterTime  = 0,
      Inst< SFTime >  _exitTime   = 0,
      Inst< SFBool >  _enabled    = 0,
      Inst< SFBool >  _isActive   = 0,
      Inst< SFBool >  _viewFrustumMode = 0 );
    
    ~VisibilitySensor(){
      if ( prev_travinfoadr && glIsQuery( queryId ) )
        glDeleteQueriesARB(1, &queryId );
    }
    /// The SetTime class is specialize to set the value of either
    /// the inputTrue or inputFalse field in the BooleanFilter node
    /// it resides in, depending on the value that is is given.
    class H3DAPI_API SetTime: public AutoUpdate< SFBool > {
    public:
      /// Sets either the inputTrue or inputFalse field depending
      /// on the new value.
      virtual void setValue( const bool &b, int id = 0 ) {
        SFBool::setValue( b , id );
        VisibilitySensor *vs = 
          static_cast< VisibilitySensor * >( getOwner() );
        if( value )
          vs->enterTime->setValue( TimeStamp() , vs->id ); 
        else 
          vs->exitTime->setValue( TimeStamp() , vs->id ); 
      }
    protected:
      /// Sets either the inputTrue or inputFalse field depending
      /// on the new value.
      virtual void update() {
        SFBool::update();
        VisibilitySensor *vs = 
          static_cast< VisibilitySensor * >( getOwner() );
        if( value )
          vs->enterTime->setValue( TimeStamp() , vs->id );
        else 
          vs->exitTime->setValue( TimeStamp(), vs->id ); 
      }
    };


    // traverse func.
    virtual void traverseSG( TraverseInfo &ti ); 

    /// Sets enterTime if the set_boolean value is set to
    /// TRUE. Else sets exitTime
    /// 
    /// <b>Access type:</b> inputOnly \n
    auto_ptr< SetTime > set_time;

    /// If true, isActive field will be true as long as the sensor
    /// stays within the view frustum without being affected by the
    /// depth test. Otherwise depth test will affect the result.
    /// 
    /// Default Value False
    /// <b>Access type:</b> inputOutput \n
    auto_ptr< SFBool > viewFrustumMode;

    /// Adress of traverseInfo 
    /// only interested in adress, what it points to will be invalid
    TraverseInfo *prev_travinfoadr;

    //  Number of Instances(DEF/USE) cases in the previous traverseSG
    int prev_maxnoinstances;
    //  The variable keeping the instance no in a traverseSG
    int no_instance;

    //  List of NoInstances of visible instances
    vector < int > list;
    //  The threshold : an object is considered as visible if the no of
    //  visible pixels exceeds the threshold value
    int visib_pix_no_threshold;

    // Internal Vectors for previous position, orientation and
    // center of rotation of viewpoint
    Vec3f prev_vp_pos;
    Vec3f can_prev_vp_pos;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  
  protected:
    GLuint queryId;
  
  };
}

#endif
