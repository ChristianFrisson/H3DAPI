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
/// \file NavigationInfo.cpp
/// \brief CPP file for NavigationInfo, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/NavigationInfo.h>
#include <H3D/Scene.h>
#include <H3D/GlobalSettings.h>

using namespace H3D;

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
  FIELDDB_ELEMENT( NavigationInfo, headlightShadows, INPUT_OUTPUT );
  FIELDDB_ELEMENT( NavigationInfo, nearVisibilityLimit, INPUT_OUTPUT );
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
                                Inst< SFBool    > _transitionComplete,
                                Inst< SFBool    > _headlightShadows,
                                Inst< SFFloat   > _nearVisibilityLimit ):
  X3DBindableNode( "NavigationInfo",_set_bind, _metadata, _bindTime,
                   _isBound ),
  avatarSize( _avatarSize ),
  headlight( _headlight  ),
  speed( _speed ),
  transitionTime( _transitionTime ),
  transitionType( _transitionType  ),
  type( _type ),
  visibilityLimit( _visibilityLimit ),
  transitionComplete( _transitionComplete ),
  headlightShadows( _headlightShadows ),
  nearVisibilityLimit( _nearVisibilityLimit ) {
  
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
  headlightShadows->setValue( false );
  nearVisibilityLimit->setValue( 0 );

  nav_type = "";
}

string NavigationInfo::getUsedNavType() {
  vector< string > navigation_types = type->getValue();

  bool hasAny = false;

  if( !nav_type.empty() ) {
    for( unsigned int i = 0; i < navigation_types.size(); ++i ) {
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
      for( unsigned int i = 0; i < navigation_types.size(); ++i ) {
        if( nav_type == navigation_types[i] )
          return nav_type;
      }
    }
  }

  bool onlyANY = false;
  // Default behaviour
  for( unsigned int i = 0; i < navigation_types.size(); ++i ) {
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
