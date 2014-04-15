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
/// \file Collision.cpp
/// \brief CPP file for Collision, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/Collision.h>
#include <H3D/Scene.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase Collision::database( 
                                    "Collision", 
                                    &(newInstance<Collision>), 
                                    typeid( Collision ),
                                    &X3DGroupingNode::database );

namespace CollisionInternals {
  FIELDDB_ELEMENT( Collision, collideTime, OUTPUT_ONLY );
  FIELDDB_ELEMENT( Collision, enabled, INPUT_OUTPUT );
  FIELDDB_ELEMENT( Collision, isActive, OUTPUT_ONLY );
  FIELDDB_ELEMENT( Collision, proxy, INPUT_OUTPUT );
}



Collision::Collision( Inst< AddChildren    > _addChildren,
                      Inst< RemoveChildren > _removeChildren,
                      Inst< SFBool         > _enabled,
                      Inst< MFChild        > _children,
                      Inst< SFNode         > _metadata,
                      Inst< SFTime         > _collideTime,
                      Inst< SFBool         > _isActive,
                      Inst< SFBound        > _bound,
                      Inst< SFVec3f        > _bboxCenter,
                      Inst< SFVec3f        > _bboxSize,
                      Inst< SFChild        > _proxy) :
  X3DGroupingNode( _addChildren, _removeChildren, _children,
                   _metadata, _bound, _bboxCenter, _bboxSize ),
  collideTime    ( _collideTime ),
  proxy          ( _proxy ),
  enabled        ( _enabled ),
  isActive       ( _isActive ) {
  
  type_name = "Collision";
  database.initFields( this );

  enabled->setValue( true );
  isActive->setValue( false, id );
}

bool Collision::lineIntersect(
                  const Vec3f &from, 
                  const Vec3f &to,    
                  LineIntersectResult &result ) {
  if( result.override_no_collision || enabled->getValue() ) {
    X3DChildNode * temp_proxy = proxy->getValue();
    if( temp_proxy )
      return temp_proxy->lineIntersect( from, to, result );
    else
      return X3DGroupingNode::lineIntersect(  from, to, result );
  }
  return false;
}

void Collision::closestPoint( const Vec3f &p,
                              NodeIntersectResult &result ) {
  if( result.override_no_collision || enabled->getValue() ) {
    X3DChildNode * temp_proxy = proxy->getValue();
    if( temp_proxy )
      return temp_proxy->closestPoint( p, result );
    else
      return X3DGroupingNode::closestPoint( p, result );
  }
}

bool Collision::movingSphereIntersect( H3DFloat radius,
                                       const Vec3f &from, 
                                       const Vec3f &to,
                                       NodeIntersectResult &result ) {
  if( result.override_no_collision || enabled->getValue() ) {
    bool intersect = false;
    X3DChildNode * temp_proxy = proxy->getValue();
    if( temp_proxy )
      intersect = temp_proxy->movingSphereIntersect( radius, from, to,
                                                     result );
    else
      intersect = X3DGroupingNode::movingSphereIntersect( radius, from, to,
                                                          result );

    if( intersect ) {
      if( !isActive->getValue() ) {
        isActive->setValue( true, id );
        collideTime->setValue( Scene::time->getValue(), id );
      }
    }
    else {
      if( isActive->getValue() )
        isActive->setValue( false, id );
    }
    return intersect;
  }
  return false;
}
