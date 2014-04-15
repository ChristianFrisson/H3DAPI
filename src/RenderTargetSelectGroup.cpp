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
/// \file RenderTargetSelectGroup.cpp
/// \brief CPP file for RenderTargetSelectGroup.
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/RenderTargetSelectGroup.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase RenderTargetSelectGroup::database( "RenderTargetSelectGroup", 
                                 &(newInstance<RenderTargetSelectGroup>), 
                                 typeid( RenderTargetSelectGroup ),
                                 &X3DGroupingNode::database );

namespace RenderTargetSelectGroupInternals {
  FIELDDB_ELEMENT( RenderTargetSelectGroup, renderTargets, INPUT_OUTPUT );
}


RenderTargetSelectGroup::RenderTargetSelectGroup( 
             Inst< AddChildren    > _addChildren,
             Inst< RemoveChildren > _removeChildren,
             Inst< MFChild        > _children,
             Inst< SFNode         > _metadata,
             Inst< SFBound        > _bound,
             Inst< SFVec3f        > _bboxCenter,
             Inst< SFVec3f        > _bboxSize,
             Inst< MFInt32        > _renderTargets ) :
  X3DGroupingNode( _addChildren, _removeChildren, _children, 
       _metadata, _bound, _bboxCenter, _bboxSize ),
  renderTargets( _renderTargets ),
  support_message_displayed( false ) {

  type_name = "RenderTargetSelectGroup";
  database.initFields( this );
  
}


void RenderTargetSelectGroup::render() {

  // Check that we have hardware support
  if( !GLEW_EXT_framebuffer_object ) {
    if( !support_message_displayed ) {
      Console(4) << "Warning: Frame Buffer Objects not supported by your graphics card "
     << "(EXT_frame_buffer_object). RenderTargetSelectGroup nodes have no effect. " 
     << endl;
      support_message_displayed = true;
    }
    return;
  }

  if( !GLEW_ARB_draw_buffers ) {
    if( !support_message_displayed ) {
      Console(4) << "Warning: Your graphics card does not support multiple "
     << "render targets(ARB_draw_buffers). RenderTargetSelectGroup nodes have no effect. " << endl;
      support_message_displayed = true;
    }
    return;
  }

  glPushAttrib( GL_COLOR_BUFFER_BIT);
 
  const vector< H3DInt32 > &render_targets = renderTargets->getValue();

  // initialize draw buffers array.
  if( !draw_buffers.get() ) {
    GLint max_draw_buffers, max_color_attachments;
    glGetIntegerv( GL_MAX_DRAW_BUFFERS_ARB, &max_draw_buffers );
    glGetIntegerv( GL_MAX_COLOR_ATTACHMENTS_EXT, &max_color_attachments );
    max_nr_draw_buffers = H3DMin( max_draw_buffers, max_color_attachments );
    draw_buffers.reset( new GLenum[max_nr_draw_buffers] );
  }

  if( render_targets.size() > 0 ) {
    // construct current draw buffers array

    // reset all draw buffers to NONE.
    for( size_t i = 0; i < (size_t)max_nr_draw_buffers; ++i ) {
      draw_buffers.get()[i] = GL_NONE;
    }

    // set the specified draw buffers to the color attachement specified by renderTargets.
    int draw_buffer_size = 0;
    for( size_t i = 0; i < render_targets.size(); ++i ) {
      int target = render_targets[i];
      if( target >= 0 && target < max_nr_draw_buffers ) {
  draw_buffers.get()[target] = GL_COLOR_ATTACHMENT0_EXT + target;
  if( target >= draw_buffer_size ) draw_buffer_size = target + 1;
      } else {
  Console(4) << "Warning: Invalid render target: " << target 
       << ". Targets supported by your graphics card are between 0 and "
       << max_nr_draw_buffers - 1 << "( in RenderTargetSelectGroup). " << endl;
      }

    }

    // change draw buffers.
    glDrawBuffersARB( draw_buffer_size, draw_buffers.get() );
  } else {
    // no targets specified, set to draw to no color buffer.
    glDrawBuffer( GL_NONE );
  }

  X3DGroupingNode::render();

  glPopAttrib();
}
