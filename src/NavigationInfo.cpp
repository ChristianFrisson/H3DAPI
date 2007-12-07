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
/// \file NavigationInfo.cpp
/// \brief CPP file for NavigationInfo, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/NavigationInfo.h>
#include <H3D/Scene.h>
#include <H3D/H3DNavigation.h>
#include <H3D/GlobalSettings.h>
#include <H3D/CollisionOptions.h>

using namespace H3D;

AutoRef< X3DViewpointNode > NavigationInfo::old_vp( 0 );
bool NavigationInfo::linear_interpolate = false;
Vec3f NavigationInfo::goal_position = Vec3f();
Rotation NavigationInfo::goal_orientation = Rotation();
Vec3f NavigationInfo::old_vp_pos = Vec3f();
Rotation NavigationInfo::old_vp_orientation = Rotation();
list<NavigationInfo *> NavigationInfo::navigationInfos;
bool NavigationInfo::force_jump = false;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase NavigationInfo::database( 
                                    "NavigationInfo", 
                                    &(newInstance<NavigationInfo>), 
                                    typeid( NavigationInfo ),
				    &X3DBindableNode::database );

namespace NavigationInfoInternals {
  FIELDDB_ELEMENT( NavigationInfo, avatarSize, INPUT_OUTPUT );
  FIELDDB_ELEMENT( NavigationInfo, headlight, INPUT_OUTPUT );
  FIELDDB_ELEMENT( NavigationInfo, speed, INPUT_OUTPUT );
  FIELDDB_ELEMENT( NavigationInfo, transitionTime, INPUT_OUTPUT );
  FIELDDB_ELEMENT( NavigationInfo, transitionType, INPUT_OUTPUT );
  FIELDDB_ELEMENT( NavigationInfo, type, INPUT_OUTPUT );
  FIELDDB_ELEMENT( NavigationInfo, visibilityLimit, INPUT_OUTPUT );
  FIELDDB_ELEMENT( NavigationInfo, transitionComplete, OUTPUT_ONLY );
}



NavigationInfo::NavigationInfo( Inst< SFSetBind > _set_bind,
                                Inst< SFNode    > _metadata,
                                Inst< SFTime    > _bindTime,
                                Inst< SFBool    > _isBound,
                                Inst< MFFloat   > _avatarSize,
                                Inst< SFBool    > _headlight,
                                Inst< SFFloat   > _speed,
                                Inst< SFTime    > _transitionTime,
                                Inst< MFString  > _transitionType,
                                Inst< MFString  > _type,
                                Inst< SFFloat   > _visibilityLimit,
                                Inst< SFBool    > _transitionComplete ):
  X3DBindableNode( "NavigationInfo",_set_bind, _metadata, _bindTime,
                   _isBound ),
  avatarSize( _avatarSize ),
  headlight( _headlight  ),
  speed( _speed ),
  transitionTime( _transitionTime ),
  transitionType( _transitionType  ),
  type( _type ),
  visibilityLimit( _visibilityLimit ),
  transitionComplete( _transitionComplete ) {
  
  type_name = "NavigationInfo";
  database.initFields( this );

  avatarSize->push_back( 0.25f );
  avatarSize->push_back( 1.6f );
  avatarSize->push_back( 0.75f );
  headlight->setValue( true );
  speed->setValue( 1 );
  transitionTime->setValue( 1.0 );
  transitionType->push_back( "LINEAR" );  
  type->push_back( "EXAMINE" );
  type->push_back( "ANY" );
  visibilityLimit->setValue( 0 );

  last_time = TimeStamp();
  nav_type = "";

  navigationInfos.push_back( this );
}

void NavigationInfo::doNavigation( X3DViewpointNode * vp,
                                      X3DChildNode *topNode ) {
  Vec3f vp_pos = vp->position->getValue();
  Vec3f vp_full_pos = vp_pos + vp->rel_pos;
  Rotation vp_orientation = vp->orientation->getValue();
  Rotation vp_full_orientation = vp_orientation * vp->rel_orn;
  H3DTime current_time = Scene::time->getValue();
  last_time = current_time;
  string navigation_type = getUsedNavType();
  if( old_vp.get() && old_vp.get() != vp ) {
    // if the viewpoint is switched when a transition is going on
    // reset the old viewpoint and calculate the new transition from
    // current position and viewpoint.
    if( linear_interpolate ) {
      if( !old_vp->retainUserOffsets->getValue() ) {
        old_vp->rel_pos = goal_position;
        old_vp->rel_orn = goal_orientation;
      }
      linear_interpolate = false;
    }

    if( vp->jump->getValue() || force_jump ) {
      force_jump = false;

      string transition = "LINEAR";

      // checking which values are allowed. (transitionType is a MFString).
      if( !transitionType->empty() )
        transition = transitionType->getValueByIndex( 0 );

      if( transition == "TELEPORT" ) {
        transitionComplete->setValue( true, id );
      }
      else if( transition == "ANIMATE" ) {
        // there should be some other kind of behaviour here.
        transitionComplete->setValue( true, id );
      }
      else {
        linear_interpolate = true;
        const Matrix4f &vp_acc_inv_mtx = vp->accInverseMatrix->getValue();
        const Matrix4f &old_vp_acc_frw_mtx = 
          old_vp->accForwardMatrix->getValue();

        start_orientation = -vp_orientation * 
          ( (Rotation)vp_acc_inv_mtx.getScaleRotationPart() *
          ( (Rotation)old_vp_acc_frw_mtx.getScaleRotationPart()
          * old_vp_orientation ) );

        goal_orientation = vp->rel_orn;

        start_position = vp_acc_inv_mtx * 
          (old_vp_acc_frw_mtx * old_vp_pos) - vp_pos;
        goal_position = vp->rel_pos;
        move_direction = goal_position - start_position;
        start_time = current_time;
      }
    }
    old_vp.reset( vp );
  }
  else if( !old_vp.get() ) {
    old_vp.reset( vp );
    old_vp_pos = old_vp->getFullPos();
    old_vp_orientation = old_vp->getFullOrn();
  }

  // When a transition takes place navigationinfo negates external
  // use of setValue of the position field of a viewpoint
  if( linear_interpolate ) {
    H3DTime elapsed_time = current_time - start_time;
    H3DTime total_time = transitionTime->getValue();
    if( elapsed_time < total_time ) {
      H3DDouble interpolation = elapsed_time / total_time;
      vp->rel_pos =  start_position +
        move_direction * interpolation;
      vp->rel_orn = start_orientation.slerp( goal_orientation,
        (H3DFloat)interpolation );
    }
    else {
      linear_interpolate = false;
      transitionComplete->setValue( true, id );
      vp->rel_pos = goal_position;
      vp_full_pos = vp_pos + goal_position;
      vp->rel_orn = goal_orientation;
      vp_full_orientation = vp_orientation * goal_orientation;
    }
  }

  if( !linear_interpolate ) {
    bool use_collision = true;
    GlobalSettings *default_settings = GlobalSettings::getActive();
    if( default_settings ) {
      CollisionOptions * collision_option = 0;
      default_settings->getOptionNode( collision_option );
      if( collision_option )
        use_collision = collision_option->avatarCollision->getValue();
    }
    H3DNavigation::doNavigation( navigation_type, vp, topNode,
      use_collision, avatarSize->getValue(), speed->getValue() );
  }

  old_vp_pos = vp_full_pos;
  old_vp_orientation = vp_full_orientation;
  H3DNavigationDevices::setNavTypeForAll( navigation_type );
}

string NavigationInfo::getUsedNavType() {
  vector< string > navigation_types = type->getValue();

  bool hasAny = false;

  if( !nav_type.empty() ) {
    for( unsigned int i = 0; i < navigation_types.size(); i++ ) {
      if( navigation_types[i] == "ANY" ) {
        hasAny = true;
        break;
      }
    }
    if( hasAny ) {
      if( nav_type == "EXAMINE" ||
          nav_type == "WALK" ||
          nav_type == "FLY" ||
          nav_type == "LOOKAT" ||
          nav_type == "NONE" )
        return nav_type;
    }
    else {
      for( unsigned int i = 0; i < navigation_types.size(); i++ ) {
        if( nav_type == navigation_types[i] )
          return nav_type;
      }
    }
  }

  bool onlyANY = false;
  // Default behaviour
  for( unsigned int i = 0; i < navigation_types.size(); i++ ) {
    if( navigation_types[i] == "EXAMINE" ) {
      return "EXAMINE";
    }
    else if( navigation_types[i] == "WALK" ) {
      return "WALK";
    }
    else if( navigation_types[i] == "FLY" ) {
      return "FLY";
    }
    else if( navigation_types[i] == "LOOKAT" ) {
      return "LOOKAT";
    }
    else if( navigation_types[i] == "ANY" ) {
      onlyANY = true;
    }
    else if( navigation_types[i] == "NONE" ) {
      return "NONE";
    }
  }

  if( onlyANY )
    return "ANY";

  return "";
}
