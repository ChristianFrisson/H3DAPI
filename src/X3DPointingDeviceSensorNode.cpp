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
/// \file X3DPointingDeviceSensorNode.cpp
/// \brief CPP file for X3DPointingDeviceSensorNode, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/X3DPointingDeviceSensorNode.h>
#include <H3D/H3DNavigation.h>
#include <H3D/GlobalSettings.h>
#include <H3D/CollisionOptions.h>

using namespace H3D;

bool X3DPointingDeviceSensorNode::last_primary_button_value = false;
Vec3f X3DPointingDeviceSensorNode::last_from;
Vec3f X3DPointingDeviceSensorNode::last_to;
vector< X3DPointingDeviceSensorNode * > X3DPointingDeviceSensorNode::instances=
vector< X3DPointingDeviceSensorNode * >();
int X3DPointingDeviceSensorNode::number_of_active = 0;

H3DNodeDatabase X3DPointingDeviceSensorNode::database( 
        "X3DPointingDeviceSensorNode", 
        NULL,
        typeid( X3DPointingDeviceSensorNode ),
        &X3DSensorNode::database 
        );

namespace X3DPointingDeviceSensorNodeInternals {
  FIELDDB_ELEMENT( X3DPointingDeviceSensorNode, description, INPUT_OUTPUT );
  FIELDDB_ELEMENT( X3DPointingDeviceSensorNode, isOver, OUTPUT_ONLY );
}

X3DPointingDeviceSensorNode::X3DPointingDeviceSensorNode( 
                              Inst< SFString > _description,
                              Inst< SFBool >  _enabled,
                              Inst< SFNode >  _metadata,
                              Inst< SFBool >  _isActive,
                              Inst< SFBool >  _isOver ) :
  X3DSensorNode( _enabled, _metadata, _isActive ),
  description ( _description  ),
  isOver( _isOver ),
  new_value( false ),
  lowest_enabled( false ),
  setIsEnabledField( new SetIsEnabled ),
  is_enabled( true ) {

  type_name = "X3DPointingDeviceSensorNode";
  database.initFields( this );

  description->setValue( "" );

  setIsEnabledField->setOwner( this );
  enabled->route( setIsEnabledField );

  isOver->setValue( false, id );

  instances.push_back( this );
}

// Destructor. 
X3DPointingDeviceSensorNode::~X3DPointingDeviceSensorNode() {
  // Remove this instance fromt he instance vector.
  instances.erase( find( instances.begin(), instances.end(), this ) );
  // Reset states if active is true.
  if( isActive->getValue() ) {
    --number_of_active;
    if( number_of_active < 0 )
      number_of_active = 0;
  }
  // Make sure that number_of_active is zero if there are no pointing device
  // sensors in the scene.
  if( instances.empty() ) {
    number_of_active = 0;
  }
}

void X3DPointingDeviceSensorNode::updateX3DPointingDeviceSensors(
  Node * n, const Vec3f &from, const Vec3f &to ) {
  if( !instances.empty() ) {
    bool collide_invisible = true;
    bool collide_collision = true;
    GlobalSettings *default_settings = GlobalSettings::getActive();
    if( default_settings ) {
      CollisionOptions * col_opt;
      default_settings->getOptionNode( col_opt );
      if( col_opt ) {
        collide_invisible =
          col_opt->sensorCollideToggleGraphicsOff->getValue();
        collide_collision =
          col_opt->sensorCollideCollisionFalse->getValue();
      }
    }
    // Intersect from and to with geometries.
    LineIntersectResult result( collide_collision, true,
                                NULL, collide_invisible );
    if( n->lineIntersect( from, 
                          to,
                          result ) ) {
      // Find out which intersection is the closest.
      int closest = 0;
      result.transformResult();
      if( result.theNodes.size() > 1 ) {
        H3DFloat closestDistance = 
          (H3DFloat)( result.result[closest].point -
                      from ).lengthSqr();
        for( unsigned int kl = 1; kl < result.theNodes.size(); ++kl ) {
          H3DFloat tempClose = 
            (H3DFloat)( result.result[kl].point -
                        from ).lengthSqr();
          if( tempClose < closestDistance ) {
            closestDistance = tempClose;
            closest = kl;
          }
        }
      }

      // Reset states for the all instances.
      for( unsigned int i = 0; i < instances.size(); ++i ) {
        instances[i]->new_value = false;
        instances[i]->lowest_enabled = false;
      }

      // Get the list of pointing device sensors active for the closest
      // geometry.
      LineIntersectResult::GeomX3DPtdMap::iterator found_ptds =
        result.geom_ptd_map.find( closest );
      if( found_ptds != result.geom_ptd_map.end() ) {
        // Go through list of X3DPointingDeviceSensor nodes that belong to the
        // closest geometry and indicate that those needs to have a new value
        // set (to cause isOver events).
        for( list< Node * >::iterator i =
               (*found_ptds).second.x3dptd.begin();
             i != (*found_ptds).second.x3dptd.end(); ++i ) {
          static_cast< X3DPointingDeviceSensorNode * >(*i)->new_value = true;
        }
        // Go through list of X3DPointingDeviceSensor nodes that belong to the
        // closest geometry and indicate that these are the lowest enabled
        // pointing device sensors and therefore needs updating.
        for( list< Node * >::iterator i =
               (*found_ptds).second.lowest_enabled.begin();
             i != (*found_ptds).second.lowest_enabled.end(); ++i ) {
          static_cast< X3DPointingDeviceSensorNode * >(*i)->lowest_enabled
            = true;
        }
      }

      for( unsigned int i = 0; i < instances.size(); ++i ) {
        if( instances[i]->new_value )
          instances[i]->onIsOver( &result.result[ closest ],
            &(*found_ptds).second.global_to_local );
        else
          instances[i]->onIsOver();
      }
    } else {
      // No intersection with any geometries are found. Reset the new_value
      // state and call onIsOver(). Note that onIsOver for most
      // X3DPointingDeviceSensors only sets event if
      // the pointing device sensor is enabled and active or if it is enabled
      // and no other pointing device sensors are active.
      for( unsigned int i = 0; i < instances.size(); ++i ) {
        instances[i]->new_value = false;
        instances[i]->onIsOver();
      }
    }

    // Call the setDragOutputEvents function. Most pointing device sensors
    // update some events only when it is active, i.e. all X3DDragSensorNodes.
    for( unsigned int i = 0; i < instances.size(); ++i ) {
      instances[i]->setDragOutputEvents( instances[i]->enabled->getValue(),
                                         from, to );
    }
  }
  last_from = from;
  last_to = to;
}

bool X3DPointingDeviceSensorNode::anyIsOver() {
  // Check if isOver is true for any of the sensors.
  for( unsigned int i = 0; i < instances.size(); ++i ) {
    if (instances[i]->isOver->getValue())
      return true;
  }
  return false;
}

bool X3DPointingDeviceSensorNode::anyIsActive() {
  // Check if isActive is true for any of the sensors.
  for( unsigned int i = 0; i < instances.size(); ++i ) {
    if (instances[i]->isActive->getValue())
      return true;
  }
  return false;
}

void X3DPointingDeviceSensorNode::updateButtonDependentFields( bool primary_button ) {
  // For each instance call setIsEnabled and setIsActive since they depend on
  // the value of the primary button device. Then set the static variable
  // so that some update fields (SetIsEnabled for example) can use it.
  if( last_primary_button_value != primary_button ) {
    for( unsigned int i = 0; i < instances.size(); ++i ) {
      instances[i]->setIsEnabled( primary_button );
      instances[i]->setIsActive( primary_button );
    }
    last_primary_button_value = primary_button;
  }
}

void X3DPointingDeviceSensorNode::setIsActive( bool primary_button ) {
  if( is_enabled ) {
    bool itIsActive = false;
    // Check if the sensor should still be active.
    if( lowest_enabled ) {
      bool is_over = isOver->getValue();
      if( primary_button ) {
        if( is_over || isActive->getValue() )
          itIsActive = true;
      } else {
        itIsActive = false;
      }
    }

    // If value differs from isActive field value then update variables and
    // fields.
    if( itIsActive != isActive->getValue() ) {
      if( itIsActive ) {
        ++number_of_active;
        H3DNavigation::disableDevice( H3DNavigation::MOUSE );
        //H3DNavigation::disableDevice( H3DNavigation::ALL );
        for( unsigned int i = 0; i < instances.size(); ++i ) {
          if( !instances[i]->lowest_enabled )
            instances[i]->isOver->setValue( false, instances[i]->id );
        }
      }  else {
        --number_of_active;
        if( number_of_active <= 0 ) {
          number_of_active = 0;
          //TODO: give option to only try to enable those that actually
          // existed before disable.
          //H3DNavigation::enableDevice( H3DNavigation::ALL );
          H3DNavigation::enableDevice( H3DNavigation::MOUSE );
        }
      }
      isActive->setValue( itIsActive, id );
      setDragOutputEvents( enabled->getValue(),
                           last_from, last_to );
    }
  }
}

void X3DPointingDeviceSensorNode::setIsEnabled( bool primary_button ) {
  bool _enabled = enabled->getValue();
  // Check if pointing device sensor should still be enabled.
  if( _enabled != is_enabled ) {
    if( !_enabled || ( _enabled && !primary_button ) )
      is_enabled = _enabled;
  }

  if( !_enabled ) {
    // If not enabled and isOver and isActive are true then set them to false.
    if( isOver->getValue() )
      isOver->setValue( false, id );
    if( isActive->getValue( id ) ) {
      isActive->setValue( false, id );
      // Call one last time with enable set to true to have a chance to reset
      // variables to how they should be when isActive is false.
      setDragOutputEvents( true, last_from, last_to );
      --number_of_active;
      if( number_of_active <= 0 ) {
        number_of_active = 0;
        //TODO: give option to only try to enable those that actually
        // existed before disable.
        //H3DNavigation::enableDevice( H3DNavigation::ALL );
        H3DNavigation::enableDevice( H3DNavigation::MOUSE );
      }
    }
  }
}

