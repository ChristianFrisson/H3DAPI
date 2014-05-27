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
/// \file DebugOptions.h
/// \brief Header file for DebugOptions.
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __DEBUGOPTIONS_H__
#define __DEBUGOPTIONS_H__

#include <H3D/H3DOptionNode.h>
#include <H3D/SFBool.h>
#include <H3D/SFInt32.h>

namespace H3D {

  /// \ingroup H3DNodes
  /// \class DebugOptions
  /// \brief Specifies parameters for if displaying debug information
  /// like bounding boxes, haptically rendered triangles etc.
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/DebugOptions.x3d">DebugOptions.x3d</a>
  ///     ( <a href="examples/DebugOptions.x3d.html">Source</a> )
  class H3DAPI_API DebugOptions : public H3DOptionNode {
  public:
    
    /// Constructor.
    DebugOptions( Inst< SFNode  > _metadata  = 0,
                  Inst< SFBool  > _drawBound = 0,
                  Inst< SFInt32 > _drawBoundTree = 0,
                  Inst< SFBool  > _drawHapticTriangles = 0,
                  Inst< SFBool  > _printShaderWarnings = 0 );
    
    /// If true the outline of the bounding box in the bound field of
    /// X3DGeometryNodes will be rendered.
    ///
    /// <b>Default value: </b> false \n
    /// <b>Access type: </b> inputOutput \n
    auto_ptr< SFBool  > drawBound;

    /// If -1, nothing will be drawn. If >= 0 the bounding boxes at that
    /// depth in the bound are drawn.
    ///
    /// <b>Default value: </b> -1 \n
    /// <b>Access type: </b> inputOutput \n
    auto_ptr< SFInt32  > drawBoundTree;

    /// If true the outline of the triangles currently being sent to be
    /// rendered by the haptics renderer is drawn.
    ///
    /// <b>Default value: </b> false \n
    /// <b>Access type: </b> inputOutput \n
    auto_ptr< SFBool  > drawHapticTriangles;

    /// If true then shaders will print its log to Console even if there are no
    /// errors.
    ///
    /// <b>Default value: </b> false \n
    /// <b>Access type: </b> inputOutput \n
    auto_ptr< SFBool  > printShaderWarnings;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  };
}

#endif
