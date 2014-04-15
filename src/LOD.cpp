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
/// \file LOD.cpp
/// \brief CPP file for LOD, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/LOD.h>
#include <H3D/X3DViewpointNode.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase LOD::database("LOD", 
                              &(newInstance<LOD>), 
                              typeid( LOD ),
                              &MatrixTransform::database );

namespace LODInternals {
  FIELDDB_ELEMENT( LOD, level_changed, OUTPUT_ONLY );
  FIELDDB_ELEMENT( LOD, center, INPUT_OUTPUT );
  FIELDDB_ELEMENT( LOD, forceTransitions, INPUT_OUTPUT );
  FIELDDB_ELEMENT( LOD, range, INPUT_OUTPUT );
}

LOD::LOD( 
         Inst< AddChildren > _addChildren,
         Inst< RemoveChildren > _removeChildren,
         Inst< MFChild > _children,
         Inst< SFNode  > _metadata,
         Inst< SFBound > _bound,
         Inst< SFInt32 > _level_changed,
         Inst< SFVec3f > _bboxCenter,
         Inst< SFVec3f > _bboxSize,
         Inst< SFVec3f > _center,
         Inst< SFBool  > _forceTransitions,
         Inst< MFFloat > _range ) :
MatrixTransform( _addChildren, _removeChildren, _children, 
                _metadata, _bound, _bboxCenter, _bboxSize),
                level_changed( _level_changed ),
    display_index(new SFInt32()),
                center( _center ),
                forceTransitions( _forceTransitions ),
                range( _range ) {

                  type_name = "LOD";
                  database.initFields( this );
                  display_index->route( displayList );
                  forceTransitions->setValue( false );
}


void LOD::traverseSG( TraverseInfo &ti ){
  X3DViewpointNode *vp = X3DViewpointNode::getActive();
  // goes from the viewports local coordinates to the global
  // then from global to LOD local coordinates 
  Matrix4f vp_to_LOD = 
    ti.getAccInverseMatrix() * 
    vp->accForwardMatrix->getValue();

  //gets the position of the vp in LOD local coordinates.
  Vec3f vp_pos_LOD = vp_to_LOD *
    vp->totalPosition->getValue();

  //calculates the distance (d) from the vp position to centre
  H3DFloat d = (vp_pos_LOD - center->getValue()).length();
  int n = range->size();
  int noOfChildren = children->size();
  // if there are too few children the last fields in the 
  // range will be ignored and the last range level will
  // be used for the lowest levels of details.
  if (noOfChildren<n+1){
    n= (noOfChildren-1);
  }
  
  vector< H3DFloat > range_vector=(range->getValue());
  
  if(forceTransitions->getValue()){
    // if forceTransitions is TRUE the LOD should always change 
    // to the closest LOD, comparing d to the range_vector[i]
    for(int i=0; i<n-1; ++i){
      if( d < range_vector[0]){
        if(display_index->getValue() !=0){
          display_index->setValue(0);
        }
      }
      else if(d>= range_vector[i] && d< range_vector[i+1]){
        if(display_index->getValue()!=(i+1)){
          display_index->setValue(i+1);
        }
      }
      else if(d >= range_vector[n-1]){
        if(display_index->getValue() != i){
            display_index->setValue(n);
        }
      }
    }
  }
  else{
    // if forceTransitions is FALSE the distance between the 
    // range values are compared (range_d). If the range_d 
    // is LARGER than min_distance(=0.5) the LOD will change 
    // level i.e work in the same way as if forceTransitions 
    // = true. If range_d is SMALLER than min_distance the 
    // precious LOD will be shown (the dispay_index will not 
    // be changed) until a level on a larger distance occurs.
    //
    // ### WARNING min_distance is hardcoded to 0.5... ###

    vector <H3DFloat> range_d;
    H3DFloat min_distance = 0.5;
    for(int i=0; i<n-1; ++i){
      range_d.push_back(range_vector[i+1] - range_vector[i]);
      if(range_d[i] > min_distance){
        if( d < range_vector[0]){
          if(display_index->getValue() !=0){
            display_index->setValue(0);
          }
        }
        else if(d>= range_vector[i] && d< range_vector[i+1]){
          if(display_index->getValue()!=(i+1)){
            display_index->setValue(i+1);
          }
        }
        else if(d >= range_vector[n-1]){
          if(display_index->getValue() != i){
            display_index->setValue(n);
          }
        }
      }
    }
  }
}


void LOD::render(){ 
  H3DDisplayListObject *c = dynamic_cast< H3DDisplayListObject* >
    ( children->getValueByIndex(display_index->getValue()) );
  if ( c ) {
    c->displayList->callList();
  } else {
    children->getValueByIndex(display_index->getValue())->render(); 
  }
}

bool LOD::lineIntersect(
                  const Vec3f &from, 
                  const Vec3f &to,    
                  LineIntersectResult &result ) {
  int index = display_index->getValue();
  if( index >= 0 && index < (int)children->size() - 1 ) return false;
  X3DChildNode *child_node = children->getValueByIndex( index );
  if( child_node ) return child_node->lineIntersect(from, to, result);
  return false;
}

void LOD::closestPoint(
                  const Vec3f &p,
                  NodeIntersectResult &result ) {
  int index = display_index->getValue();
  if( index >= 0 && index < (int)children->size() - 1 ) return;
  X3DChildNode *child_node = children->getValueByIndex( index );
  if( child_node ) child_node->closestPoint( p, result );
}

bool LOD::movingSphereIntersect(
                  H3DFloat radius,
                  const Vec3f &from,
                  const Vec3f &to,
                  NodeIntersectResult &result ) {
  int index = display_index->getValue();
  if( index >= 0 && index < (int)children->size() - 1 ) return false;
  X3DChildNode *child_node = children->getValueByIndex( index );
  if( child_node ) return child_node->movingSphereIntersect( radius, from,
                                                             to, result );
  return false;
}

