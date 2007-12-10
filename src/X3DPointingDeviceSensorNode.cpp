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
  setIsEnabled( new SetIsEnabled ),
  setIsActive( new SetIsActive ),
  current_pt_id( -1 ),
  last_ti_ptr( 0 ) {

  type_name = "X3DPointingDeviceSensorNode";
  database.initFields( this );

  description->setValue( "" );

  if( !mouse_sensor ) {
    mouse_sensor = new MouseSensor();
    pos_device2D = mouse_sensor->position->getValue();
  }    

  is_enabled = true;
  setIsEnabled->setOwner( this );
  setIsEnabled->setValue( true );
  enabled->routeNoEvent( setIsEnabled );
  mouse_sensor->leftButton->routeNoEvent( setIsEnabled );

  setIsActive->setOwner(this);
  mouse_sensor->leftButton->routeNoEvent( setIsActive );
  isOver->routeNoEvent( setIsActive );

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

void X3DPointingDeviceSensorNode::addGeometryNode(
  Node * n, H3DInt32 geom_index ) {
  geometry_nodes[current_pt_id].push_back( make_pair( n, geom_index ) );
}

void X3DPointingDeviceSensorNode::clearGeometryNodes() {
  for( unsigned int i = 0; i < instances.size(); i++ ) {
    instances[i]->geometry_nodes.clear();
    instances[i]->pt_matrices.clear();
  }
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

      for( unsigned int i = 0; i < instances.size(); i++ ) {
        for( PtIdGeomIdMap::iterator j = 
             instances[i]->geometry_nodes.begin();
             j != instances[i]->geometry_nodes.end();
             j++ ) {
          for( unsigned int k = 0; k < (*j).second.size(); k++ ) {
            (*j).second[k].first->resetNodeDefUseId();
          }
        }
      }  

      LineIntersectResult result( false, true, true );
      if( n->lineIntersect( near_plane_pos, 
                            far_plane_pos,
                            result ) ) {
        int closest = 0;
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
          int temp_pt_id =
            instances[i]->findGeometry( result.theNodes[closest] );
          if( temp_pt_id != -1 ){
            instances[i]->onIsOver( true, result.result[closest], temp_pt_id );
          }
          else
            instances[i]->onIsOver( false,
                                    result.result[closest],
                                    temp_pt_id );
        }
      }
      else {
        for( unsigned int i = 0; i < instances.size(); i++ ) {
          IntersectionInfo result;
          int geometryIndex = -1;
          instances[i]->onIsOver( false, result, geometryIndex );
        }
      }
    }
  }

  for( unsigned int i = 0; i < instances.size(); i++ ) {
    for( PtIdGeomIdMap::iterator j = 
         instances[i]->geometry_nodes.begin();
         j != instances[i]->geometry_nodes.end();
         j++ ) {
      for( unsigned int k = 0; k < (*j).second.size(); k++ ) {
        (*j).second[k].first->resetNodeDefUseId();
      }
    }
  }
}

int X3DPointingDeviceSensorNode::
  findGeometry( pair< Node * , H3DInt32 > &geom ) {
  if( is_enabled ) {
    for( PtIdGeomIdMap::iterator i = geometry_nodes.begin();
         i != geometry_nodes.end(); i++ ) {
      for( unsigned int j = 0; j < (*i).second.size(); j++ ) {
        if( (*i).second[j].first == geom.first &&
            (*i).second[j].second == geom.second ) {
          return (*i).first;
        }
      }
    }
  }
  return -1;
}

void X3DPointingDeviceSensorNode::SetIsActive::update() {
  SFBool::update();
  X3DPointingDeviceSensorNode *ts = 
    static_cast< X3DPointingDeviceSensorNode * >( getOwner() );
  if( ts->is_enabled ) {
    bool itIsActive = false;
    bool leftButton = 
      static_cast< SFBool * >( routes_in[0] )->getValue();
    bool isOver = static_cast< SFBool * >( routes_in[1] )->getValue();
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

    if( itIsActive != ts->isActive->getValue() ) {
      if( itIsActive ) {
        number_of_active++;
        H3DNavigation::disableDevice( H3DNavigation::MOUSE );
      }
      else {
        number_of_active--;
        if( number_of_active == 0 )
          H3DNavigation::enableDevice( H3DNavigation::MOUSE );
      }
      ts->isActive->setValue( itIsActive, ts->id );
    }
  }
}

H3DInt32 X3DPointingDeviceSensorNode::increaseIndex( TraverseInfo &ti ) {
  if( last_ti_ptr != &ti ) {
    last_ti_ptr = &ti;
    current_pt_id = -1;
  }
  current_pt_id++;
  return current_pt_id;
}
