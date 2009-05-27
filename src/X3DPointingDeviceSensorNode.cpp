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
/// \file X3DPointingDeviceSensorNode.cpp
/// \brief CPP file for X3DPointingDeviceSensorNode, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/X3DPointingDeviceSensorNode.h>
#include <H3D/H3DNavigation.h>

using namespace H3D;

MouseSensor * X3DPointingDeviceSensorNode::mouse_sensor = NULL;
Vec2f X3DPointingDeviceSensorNode::pos_device2D;
Vec3f X3DPointingDeviceSensorNode::near_plane_pos;
Vec3f X3DPointingDeviceSensorNode::far_plane_pos;
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
  setIsEnabled( new SetIsEnabled ),
  setIsActive( new SetIsActive ) {

  type_name = "X3DPointingDeviceSensorNode";
  database.initFields( this );

  description->setValue( "" );

  if( !mouse_sensor ) {
    mouse_sensor = new MouseSensor();
    pos_device2D = mouse_sensor->position->getValue();
  }    

  is_enabled = true;
  setIsEnabled->setOwner( this );
  setIsEnabled->setName( "setIsEnabled" );
  setIsEnabled->setValue( true );
  enabled->routeNoEvent( setIsEnabled );
  mouse_sensor->leftButton->routeNoEvent( setIsEnabled );

  setIsActive->setOwner( this );
  setIsActive->setName( "setIsActive" );
  mouse_sensor->leftButton->routeNoEvent( setIsActive );
  isOver->setValue( false, id );

  instances.push_back( this );
}

/// Destructor. 
X3DPointingDeviceSensorNode::~X3DPointingDeviceSensorNode() {
  instances.erase( find( instances.begin(), instances.end(), this ) );
  if( isActive->getValue() ) {
    number_of_active--;
    if( number_of_active < 0 )
      number_of_active = 0;
  }
  if( instances.empty() ) {
    delete mouse_sensor;
    mouse_sensor = NULL;
    number_of_active = 0;
  }
}

bool X3DPointingDeviceSensorNode::has2DPointingDeviceMoved( Vec2f & pos ) {
  Vec2f tempPos = mouse_sensor->position->getValue();
  if( ( tempPos - pos_device2D ).lengthSqr() > Constants::f_epsilon ) {
    pos_device2D.x = tempPos.x;
    pos_device2D.y = tempPos.y;
    pos = tempPos;
    return true;
  }
  return false;
}

void X3DPointingDeviceSensorNode::updateX3DPointingDeviceSensors( Node * n ) {
  if( !instances.empty() ) {
    Vec2f theMousePos;
    if( has2DPointingDeviceMoved( theMousePos ) ) {
      GLint viewport[4];
      GLdouble mvmatrix[16], projmatrix[16];
      GLdouble wx, wy, wz;
      glGetIntegerv( GL_VIEWPORT, viewport );
      glGetDoublev( GL_MODELVIEW_MATRIX, mvmatrix );
      glGetDoublev( GL_PROJECTION_MATRIX, projmatrix );

      theMousePos.y = viewport[3] - theMousePos.y - 1;
      gluUnProject( (GLdouble) theMousePos.x, (GLdouble) theMousePos.y,
        0.0, mvmatrix, projmatrix, viewport, &wx, &wy, &wz );
      near_plane_pos = Vec3f( (H3DFloat)wx, (H3DFloat)wy, (H3DFloat)wz );
      gluUnProject( (GLdouble) theMousePos.x, (GLdouble) theMousePos.y,
        1.0, mvmatrix, projmatrix, viewport, &wx, &wy, &wz );
      far_plane_pos = Vec3f( (H3DFloat)wx, (H3DFloat)wy, (H3DFloat)wz );

      LineIntersectResult result( true, true );
      if( n->lineIntersect( near_plane_pos, 
                            far_plane_pos,
                            result ) ) {
        int closest = 0;
        result.transformResult();
        if( result.theNodes.size() > 1 ) {
          H3DFloat closestDistance = 
            (H3DFloat)( result.result[closest].point -
                        near_plane_pos ).lengthSqr();
          for( unsigned int kl = 1; kl < result.theNodes.size(); kl++ ) {
            H3DFloat tempClose = 
              (H3DFloat)( result.result[kl].point -
                          near_plane_pos ).lengthSqr();
            if( tempClose < closestDistance ) {
              closestDistance = tempClose;
              closest = kl;
            }
          }
        }

        for( unsigned int i = 0; i < instances.size(); i++ ) {
          instances[i]->new_value = false;
          instances[i]->lowest_enabled = false;
        }

        LineIntersectResult::GeomX3DPtdMap::iterator found_ptds =
          result.geom_ptd_map.find( closest );
        if( found_ptds != result.geom_ptd_map.end() ) {
          for( list< Node * >::iterator i =
                 (*found_ptds).second.x3dptd.begin();
               i != (*found_ptds).second.x3dptd.end(); i++ ) {
            static_cast< X3DPointingDeviceSensorNode * >(*i)->new_value = true;
            
          }
          for( list< Node * >::iterator i =
                 (*found_ptds).second.lowest_enabled.begin();
               i != (*found_ptds).second.lowest_enabled.end(); i++ ) {
            static_cast< X3DPointingDeviceSensorNode * >(*i)->lowest_enabled
              = true;
          }
        }

        for( unsigned int i = 0; i < instances.size(); i++ ) {
          if( instances[i]->new_value )
            instances[i]->onIsOver( &result.result[ closest ],
              &(*found_ptds).second.global_to_local );
          else
            instances[i]->onIsOver();
        }
      }
      else {
        for( unsigned int i = 0; i < instances.size(); i++ ) {
          instances[i]->new_value = false;
          instances[i]->onIsOver();
        }
      }
    }
  }
}

bool X3DPointingDeviceSensorNode::anyIsOver() {
  for( unsigned int i = 0; i < instances.size(); ++i ) {
    if (instances[i]->isOver->getValue())
      return true;
  }
  return false;
}

bool X3DPointingDeviceSensorNode::anyIsActive() {
  for( unsigned int i = 0; i < instances.size(); ++i ) {
    if (instances[i]->isActive->getValue())
      return true;
  }
  return false;
}

void X3DPointingDeviceSensorNode::SetIsActive::update() {
  SFBool::update();
  X3DPointingDeviceSensorNode *ts = 
    static_cast< X3DPointingDeviceSensorNode * >( getOwner() );
  if( ts->is_enabled ) {
    bool itIsActive = false;
    if( ts->lowest_enabled ) {
      bool leftButton = 
        static_cast< SFBool * >( routes_in[0] )->getValue();
      bool isOver = ts->isOver->getValue();
      if( leftButton ) {
        if( !left_mouse_miss && 
          ( isOver || ts->isActive->getValue() ) )
          itIsActive = true;
        else
          left_mouse_miss = true;
      }
      else {
        itIsActive = false;
        left_mouse_miss = false;
      }
    }

    if( itIsActive != ts->isActive->getValue() ) {
      if( itIsActive ) {
        number_of_active++;
        H3DNavigation::disableDevice( H3DNavigation::MOUSE );
        //H3DNavigation::disableDevice( H3DNavigation::ALL );
        for( unsigned int i = 0; i < ts->instances.size(); i++ ) {
          if( !instances[i]->lowest_enabled )
            instances[i]->isOver->setValue( false, instances[i]->id );
        }
      }
      else {
        number_of_active--;
        if( number_of_active <= 0 ) {
          number_of_active = 0;
          //TODO: give option to only try to enable those that actually
          // existed before disable.
          //H3DNavigation::enableDevice( H3DNavigation::ALL );
          H3DNavigation::enableDevice( H3DNavigation::MOUSE );
        }
      }
      ts->isActive->setValue( itIsActive, ts->id );
    }
  }
}

void X3DPointingDeviceSensorNode::SetIsEnabled::update() {
  SFBool::update();
  bool _enabled = static_cast< SFBool * >( routes_in[0] )->getValue();
  bool leftButton = static_cast< SFBool * >( routes_in[1] )->getValue();
        
  X3DPointingDeviceSensorNode *pdsn = 
    static_cast< X3DPointingDeviceSensorNode * >( getOwner() );
  if( _enabled != pdsn->is_enabled ) {
    if( leftButton && _enabled && !pdsn->is_enabled ) { // TODO : This is a no-op
      pdsn->is_enabled = false;                         // This is a no-op
    }                                                   // This is a no-op   
    else {
      pdsn->is_enabled = _enabled;
    }
  }

  if( routes_in[0] == event.ptr && 
      !_enabled && pdsn->isActive->getValue( pdsn->id ) ) {
    pdsn->isActive->setValue( false, pdsn->id );
    number_of_active--;
    if( number_of_active <= 0 ) {
      number_of_active = 0;
      //TODO: give option to only try to enable those that actually
      // existed before disable.
      //H3DNavigation::enableDevice( H3DNavigation::ALL );
      H3DNavigation::enableDevice( H3DNavigation::MOUSE );
    }
  }
}
