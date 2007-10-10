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
/// \file Viewpoint.cpp
/// \brief CPP file for Viewpoint, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/ViewpointGroup.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase ViewpointGroup::database( 
                                    "ViewpointGroup", 
                                    &(newInstance<ViewpointGroup>), 
                                    typeid( ViewpointGroup ),
                                    &X3DViewpointNode::database );

namespace ViewpointGroupInternals {
  FIELDDB_ELEMENT( ViewpointGroup, center, INPUT_OUTPUT );
  FIELDDB_ELEMENT( ViewpointGroup, children, INPUT_OUTPUT );
  FIELDDB_ELEMENT( ViewpointGroup, displayed, INPUT_OUTPUT );
  FIELDDB_ELEMENT( ViewpointGroup, size, INPUT_OUTPUT );
}



ViewpointGroup::ViewpointGroup(
                    Inst< SFSetBind >  _set_bind,
                    Inst< SFVec3f   >  _centerOfRotation,
                    Inst< SFString  >  _description,
                    Inst< SFBool    >  _jump,
                    Inst< SFNode    >  _metadata,
                    Inst< SFRotation>  _orientation,
                    Inst< SFVec3f   >  _position,
                    Inst< SFBool    >  _retainUserOffsets,
                    Inst< SFTime    >  _bindTime,
                    Inst< SFBool    >  _isBound,
                    Inst< SFMatrix4f > _accForwardMatrix,
                    Inst< SFMatrix4f > _accInverseMatrix,
                    Inst< SFVec3f >  _center,
                    Inst< MFChild >   _children,
                    Inst< SFBool >   _displayed,
                    Inst< SFVec3f >  _size ) :
  X3DViewpointNode( _set_bind, _centerOfRotation, _description, _jump,
                    _metadata, _orientation, _position,
                    _retainUserOffsets, _bindTime, _isBound,
                    _accForwardMatrix, _accInverseMatrix ),
  center( _center ),
  children( _children ),
  displayed( _displayed ),
  size( _size ) {
  
  type_name = "ViewpointGroup";
  database.initFields( this );

  displayed->setValue( true );
  center->setValue( Vec3f( 0, 0, 0 ) );
  size->setValue( Vec3f( 0, 0, 0 ) );

  X3DViewpointNode * vp = getActive();
  if( vp ) {
    last_position = vp->position->getValue() + vp->rel_pos;
  }
}

void ViewpointGroup::traverseSG( TraverseInfo &ti ) {
  if( displayed->getValue() ) {
    Vec3f the_center = center->getValue();
    if( the_center.lengthSqr() <= Constants::f_epsilon ) {
      display_in_list = true;
    }
    else {
      X3DViewpointNode * vp = getActive();
      if( vp ) {
        Vec3f vp_full_position = vp->getFullPos();
        Vec3f box_size_half = size->getValue() / 2;
        Vec3f box_min = the_center - box_size_half;
        Vec3f box_max = the_center + box_size_half;
        if( vp_full_position.x >= box_min.x && vp_full_position.x <= box_max.x
          && vp_full_position.y >= box_min.y && vp_full_position.y <= box_max.y
          && vp_full_position.z >= box_min.z && vp_full_position.z <= box_max.z
          ) {
            display_in_list = true;
        }
        else
          display_in_list = false;
      }
    }
  }
  else {
    display_in_list = false;
  }
}

bool ViewpointGroup::containsViewpoint( X3DViewpointNode *vp ) const {
  for( MFChild::const_iterator i = children->begin();
       i != children->end(); i++ ) {
    if( ViewpointGroup *g = dynamic_cast< ViewpointGroup * >( *i ) ) {
      if( g->containsViewpoint( vp ) ) return true;
    } else {
      if( (*i) == vp ) return true;
    }
  }
  return false;
}
