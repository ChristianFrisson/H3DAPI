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
/// \file H3DRenderModeGroupNode.cpp
/// \brief CPP file for H3DRenderModeGroupNode.
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/H3DRenderModeGroupNode.h>

using namespace H3D;

H3DRenderModeGroupNode *H3DRenderModeGroupNode::current_render_mode_group = NULL;

H3DNodeDatabase H3DRenderModeGroupNode::database( 
        "H3DRenderModeGroupNode", 
        NULL,
        typeid( H3DRenderModeGroupNode ),
        &X3DGroupingNode::database 
        );

namespace H3DRenderModeGroupNodeInternals {
}

H3DRenderModeGroupNode::H3DRenderModeGroupNode( Inst< AddChildren    > _addChildren,
                                  Inst< RemoveChildren > _removeChildren,
                                  Inst< MFChild        > _children,
                                  Inst< SFNode         > _metadata,
                                  Inst< SFBound        > _bound,
                                  Inst< SFVec3f        > _bboxCenter,
                                  Inst< SFVec3f        > _bboxSize ) :
  X3DGroupingNode( _addChildren, _removeChildren, _children, _metadata, _bound,
                   _bboxCenter, _bboxSize ) {

  type_name = "H3DRenderModeGroupNode";
  database.initFields( this );
}

void H3DRenderModeGroupNode::render( ) {
  H3DRenderModeGroupNode *previous_node = current_render_mode_group;
  current_render_mode_group = this;
  X3DGroupingNode::render();
  current_render_mode_group = previous_node;
}

void H3DRenderModeGroupNode::traverseSG( TraverseInfo &ti ) {
  H3DRenderModeGroupNode *previous_node = current_render_mode_group;
  current_render_mode_group = this;
  X3DGroupingNode::traverseSG( ti );
  current_render_mode_group = previous_node;
}

