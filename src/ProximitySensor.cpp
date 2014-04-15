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
/// \file ProximitySensor.cpp
/// \brief CPP file for ProximitySensor, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/ProximitySensor.h>

using namespace H3D;

H3DNodeDatabase ProximitySensor::database( 
        "ProximitySensor", 
        &(newInstance<ProximitySensor>), 
        typeid( ProximitySensor ),
        &X3DEnvironmentalSensorNode::database 
        );

namespace ProximitySensorInternals {
  FIELDDB_ELEMENT( ProximitySensor, centerOfRotation_changed, OUTPUT_ONLY );
  FIELDDB_ELEMENT( ProximitySensor, orientation_changed, OUTPUT_ONLY );
  FIELDDB_ELEMENT( ProximitySensor, position_changed, OUTPUT_ONLY );
}

ProximitySensor::ProximitySensor( Inst< SFNode > _metadata ,
                Inst< SFVec3f > _center ,
                Inst< SFBool > _enabled ,
                Inst< SFVec3f > _size ,
                Inst< SFTime > _enterTime ,
                Inst< SFTime > _exitTime ,
                Inst< SFBool > _isActive ,
                Inst< SFVec3f > _centerOfRotation_changed,
                Inst< SFRotation > _orientation_changed,
                Inst< SFVec3f > _position_changed) :

                X3DEnvironmentalSensorNode( _metadata ,_center,
                  _enabled, _size, _enterTime, _exitTime,
                  _isActive),
                centerOfRotation_changed(
                  _centerOfRotation_changed),
                orientation_changed( _orientation_changed ),
                position_changed( _position_changed ),
                set_time( new SetTime ){

  type_name = "ProximitySensor";
  database.initFields( this );

  set_time->setOwner( this );
  set_time->setName( "set_time" );
  isActive->route( set_time );
  prev_travinfoadr = 0;
  
}
void ProximitySensor::traverseSG( TraverseInfo &ti ) {
 
  if( enabled->getValue() &&
    ( size->getValue().x > 0.0 &&
      size->getValue().y > 0.0 &&
      size->getValue().z > 0.0 ))
  {

    // First instance of the DEF/USE ProximitySensors in the scene
    if( prev_travinfoadr != &ti)
      prev_vp_pos = can_prev_vp_pos;

    // Active viewpoint
    X3DViewpointNode *vp = X3DViewpointNode::getActive();
    
    // Local position of viewpoint
    Vec3f loc_vp_pos = vp->totalPosition->getValue();
    
    const Matrix4f &accFM_vp = vp->accForwardMatrix->getValue();
    
    // Global position of viewpoint
    Vec3f glob_vp_pos =  accFM_vp * loc_vp_pos;
      
    const Matrix4f &accInvMatrix = ti.getAccInverseMatrix();
    
    // Current and previous positions of viewpoint wrt
    // coordinate system of proximitysensor
    Vec3f vp_pos_wrt_pr =  accInvMatrix * glob_vp_pos;
    Vec3f prev_vp_pos_wrt_pr = accInvMatrix * prev_vp_pos;

    // Local orientation of viewpoint
    Rotation loc_vp_orn = vp->totalOrientation->getValue();
    Rotation vp_rot = 
    (Rotation)vp->accForwardMatrix->getValue().getRotationPart();

    // Global orientation of viewpoint
    
    Rotation glob_vp_orn = vp_rot * loc_vp_orn;

    Rotation ps_rot = 
        (Rotation)ti.getAccInverseMatrix().getRotationPart(); 

    // Orientation of viewpoint wrt coordinate system of proximitysensor
    
    Rotation vp_orn_wrt_pr = ps_rot * glob_vp_orn;

    // Center of rotation of viewpoint
    Vec3f loc_vp_cor = vp->centerOfRotation->getValue();
    Vec3f glob_vp_cor =  accFM_vp * loc_vp_cor;
    Vec3f vp_cor_wrt_pr =  accInvMatrix * glob_vp_cor;
      
    H3DFloat cx = center->getValue().x;
    H3DFloat cy = center->getValue().y;
    H3DFloat cz = center->getValue().z;

    H3DFloat x = size->getValue().x / 2.0f;
    H3DFloat y = size->getValue().y / 2.0f;
    H3DFloat z = size->getValue().z / 2.0f;
    
    if (prev_vp_pos_wrt_pr.x >= cx - x && prev_vp_pos_wrt_pr.x <= cx + x &&
      prev_vp_pos_wrt_pr.y >= cy - y && prev_vp_pos_wrt_pr.y <= cy + y &&
      prev_vp_pos_wrt_pr.z >= cz - z && prev_vp_pos_wrt_pr.z <= cz + z ) 
    {
      if (vp_pos_wrt_pr.x < cx - x || vp_pos_wrt_pr.x > cx + x ||
        vp_pos_wrt_pr.y < cy - y || vp_pos_wrt_pr.y > cy + y ||
        vp_pos_wrt_pr.z < cz - z || vp_pos_wrt_pr.z > cz + z ) 
      {        
        // Viewpoint exits from the proximity sensor
        isActive->setValue( false , id);          
      }
      else
      {
        position_changed->setValue( Vec3f( vp_pos_wrt_pr ), id );
        orientation_changed->setValue( vp_orn_wrt_pr , id  );

        NavigationInfo *ni = NavigationInfo::getActive();
        if(ni)
        {
          bool containLookat = false;

          vector< string > navigation_types = ni->type->getValue();
          
          for(unsigned int i = 0; i<navigation_types.size(); ++i) {
            if( navigation_types[i] == "LOOKAT" ||
              navigation_types[i] == "ANY") 
                containLookat = true;
          }
          if( containLookat )
            centerOfRotation_changed->setValue(Vec3f( vp_cor_wrt_pr ), id );
        }
      
      }
    
    }
    else
    {

      if( vp_pos_wrt_pr.x >= cx - x && vp_pos_wrt_pr.x <= cx + x &&
        vp_pos_wrt_pr.y >= cy - y && vp_pos_wrt_pr.y <= cy + y &&
        vp_pos_wrt_pr.z >= cz - z && vp_pos_wrt_pr.z <= cz + z )
      {  
        // Viewpoint enters to the proximity sensor
        isActive->setValue( true , id );
      }
    }
  
    can_prev_vp_pos = glob_vp_pos;
    prev_travinfoadr = &ti;
  }
}

