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
/// \file X3DSoundNode.h
/// \brief Header file for X3DSoundNode, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __X3DSOUNDNODE_H__
#define __X3DSOUNDNODE_H__

#include <H3D/X3DChildNode.h>
#ifdef HAVE_OPENAL
#if defined( MACOSX )
#include <OpenAL/al.h>
#else
#include <AL/al.h>
#endif
#endif

namespace H3D {

  /// \ingroup AbstractNodes
  /// \class X3DSoundNode
  /// \brief This abstract node type is the base for all Sound nodes.
  ///
  /// 
  class H3DAPI_API X3DSoundNode : public X3DChildNode {
  public:
    
    /// Constructor.
    X3DSoundNode( Inst< SFNode>  _metadata = 0 );
#ifdef HAVE_OPENAL
    /// Get the OpenAL source id that is used for playing this source.
    inline ALuint getALSourceId() { return al_source; }

  protected:
    // the handle of the OpenAL source that is used for this node.
    ALuint al_source;
#endif
  };
}

#endif
