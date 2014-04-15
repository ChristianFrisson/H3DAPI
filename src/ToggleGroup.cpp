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
/// \file ToggleGroup.cpp
/// \brief CPP file for ToggleGroup.
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/ToggleGroup.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase ToggleGroup::database( "ToggleGroup", 
                                       &(newInstance<ToggleGroup>), 
                                       typeid( ToggleGroup ),
                                       &X3DGroupingNode::database );

namespace ToggleGroupInternals {
  FIELDDB_ELEMENT( ToggleGroup, hapticsOn, INPUT_OUTPUT );
  FIELDDB_ELEMENT( ToggleGroup, graphicsOn, INPUT_OUTPUT );
  FIELDDB_ELEMENT( ToggleGroup, hapticsOnDevice, INPUT_OUTPUT );
  FIELDDB_ELEMENT( ToggleGroup, traverseOn, INPUT_OUTPUT );
}

ToggleGroup::ToggleGroup( Inst< AddChildren    > _addChildren,
                          Inst< RemoveChildren > _removeChildren,
                          Inst< MFChild        > _children,
                          Inst< SFNode         > _metadata,
                          Inst< SFBound        > _bound,
                          Inst< SFVec3f        > _bboxCenter,
                          Inst< SFVec3f        > _bboxSize,
                          Inst< SFBool         > _hapticsOn,
                          Inst< SFBool         > _graphicsOn,
                          Inst< MFBool         > _hapticsOnDevice,
                          Inst< SFBool         > _traverseOn) :
  X3DGroupingNode( _addChildren, _removeChildren, _children,
                   _metadata, _bound, _bboxCenter, _bboxSize ),
  hapticsOn( _hapticsOn ),
  graphicsOn( _graphicsOn ),
  hapticsOnDevice( _hapticsOnDevice ),
  traverseOn(_traverseOn){
  type_name = "ToggleGroup";
  database.initFields( this );

  hapticsOn->setValue( true );
  graphicsOn->setValue( true );

  graphicsOn->route( displayList );
  traverseOn->setValue( true );

}


void ToggleGroup::render() {
  if( graphicsOn->getValue() )
    X3DGroupingNode::render();
}

void ToggleGroup::traverseSG( TraverseInfo &ti ) {
  vector<bool> haptics_was_enabled = ti.getHapticsEnabled();
  bool graphics_was_enabled = ti.graphicsEnabled();
  const vector<bool> haptics_on_device = hapticsOnDevice->getValue();

  // We need to make sure that we do not enable haptics/graphics 
  // if it has already been disabled by parent node.

  if( haptics_on_device.empty() ) {
    if( !hapticsOn->getValue() ) {
      ti.disableHaptics();
    }
  } else {
    for( unsigned int i = 0; i < haptics_was_enabled.size(); ++i ) {
      if( haptics_was_enabled[i] ) {
        size_t index =  
          i < haptics_on_device.size() ? i : haptics_on_device.size() - 1;
        if( !haptics_on_device[index] ) ti.disableHaptics( i );
      }
    }
  }

  if( graphics_was_enabled && !graphicsOn->getValue() ) {
    ti.disableGraphics();
  }
  if( traverseOn->getValue() ) {
    X3DGroupingNode::traverseSG( ti );
  }

  // reset to previous values
  ti.setHapticsEnabled( haptics_was_enabled );
  if( graphics_was_enabled ) ti.enableGraphics();
}

bool ToggleGroup::lineIntersect( const Vec3f &from,
                                 const Vec3f &to,
                                 LineIntersectResult &result ) {
  if( result.collide_invisible || graphicsOn->getValue() ) {
    return X3DGroupingNode::lineIntersect( from, to, result );
  }
  return false;
}

void ToggleGroup::closestPoint( const Vec3f &p,
                                NodeIntersectResult &result ) {
  if( result.collide_invisible || graphicsOn->getValue() ) {
    X3DGroupingNode::closestPoint( p, result );
  }
}

bool ToggleGroup::movingSphereIntersect( H3DFloat radius,
                                         const Vec3f &from, 
                                         const Vec3f &to,
                                         NodeIntersectResult &result ) {
  if( result.collide_invisible || graphicsOn->getValue() ) {
    return X3DGroupingNode::movingSphereIntersect( radius, from, to, result );
  }
  return false;
}

