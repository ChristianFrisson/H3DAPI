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
/// \file HapticLayeredGroup.cpp
/// \brief CPP file for HapticLayeredGroup
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/HapticLayeredGroup.h>
#include <H3D/H3DRenderStateObject.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase HapticLayeredGroup::database( "HapticLayeredGroup", 
                                 &(newInstance<HapticLayeredGroup>), 
                                 typeid( HapticLayeredGroup ),
                                 &X3DGroupingNode::database );


HapticLayeredGroup::HapticLayeredGroup( Inst< AddChildren    > _addChildren,
                                        Inst< RemoveChildren > _removeChildren,
                                        Inst< MFChild        > _children,
                                        Inst< SFNode         > _metadata,
                                        Inst< SFBound        > _bound,
                                        Inst< SFVec3f        > _bboxCenter,
                                        Inst< SFVec3f        > _bboxSize ) :
  X3DGroupingNode( _addChildren, _removeChildren, _children, 
                   _metadata, _bound, _bboxCenter, _bboxSize ) {
  type_name = "HapticLayeredGroup";
}


void HapticLayeredGroup::traverseSG( TraverseInfo &ti ) {
  for( MFNode::const_iterator i = children->begin();
       i != children->end(); ++i ) {
    H3DRenderStateObject *l = dynamic_cast< H3DRenderStateObject* >( *i );
    if ( l ) {
      l->enableHapticsState( ti );
    }
  }

  unsigned int current_layer = ti.getCurrentLayer();
  ti.initializeLayers( children->size() );

  // not using iterators since they can become invalid if the 
  // traversal changes the children field while iterating.
  const NodeVector &c = children->getValue();
  for( unsigned int i = 0; i < c.size(); ++i ) {
    if( c[i] ) {
      ti.setCurrentLayer( i );
      c[i]->traverseSG( ti );
    }
  }

  ti.setCurrentLayer( current_layer );

  for( MFNode::const_reverse_iterator i = children->rbegin();
       i != children->rend(); ++i ) {
    H3DRenderStateObject *l = dynamic_cast< H3DRenderStateObject* >( *i );
    if ( l ) {
      l->disableHapticsState( ti );
    }
  }
}

