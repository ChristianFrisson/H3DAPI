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
/// \file Group.cpp
/// \brief CPP file for Group, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/Group.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase Group::database( "Group", 
                                 &(newInstance<Group>), 
                                 typeid( Group ),
                                 &X3DGroupingNode::database );


Group::Group( 
             Inst< AddChildren    > _addChildren,
             Inst< RemoveChildren > _removeChildren,
             Inst< MFChild        > _children,
             Inst< SFNode         > _metadata,
             Inst< SFBound        > _bound,
             Inst< SFVec3f        > _bboxCenter,
             Inst< SFVec3f        > _bboxSize ) :
  X3DGroupingNode( _addChildren, _removeChildren, _children, 
                   _metadata, _bound, _bboxCenter, _bboxSize ) {
  type_name = "Group";
}


