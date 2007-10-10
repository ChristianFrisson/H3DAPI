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
/// \file ClipPlane.h
/// \brief Header file for ClipPlane, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __CLIPPLANE_H__
#define __CLIPPLANE_H__

#include <H3D/X3DChildNode.h>
#include <H3D/H3DDisplayListObject.h>
#include <H3D/H3DRenderStateObject.h>
#include <H3D/SFVec4d.h>

namespace H3D {

  /// \ingroup H3DNodes
  /// \class ClipPlane
  /// \brief This node defines a single plane equation that will be used 
  /// to clip the geometry, i.e. remove parts of it from rendering.
  /// 
  /// The plane field defines a plane equation that will be used to clip
  /// the geometry. The points that are drawn are the points that satisfy the
  /// inequation: 
  /// plane.x*x+plane.y*y+plane.z*z+plane.w >= 0
  /// 
  /// Clipping only occurs of the enabled field is 'true'.
  /// 
  /// The clipHaptics field specifies if the clipping plane should clip
  /// the haptics if enabled, i.e. if the haptics device is in a position that
  /// is clipped by the plane no haptics will be rendered for
  /// the device and the clipGraphics field specifies if the graphics should
  /// be clipped when the ClipPlane is enabled.
  /// 
  /// \par Internal routes:
  /// \dotfile ClipPlane.dot
  class H3DAPI_API ClipPlane : 
    public X3DChildNode,
    public H3DDisplayListObject,
    public H3DRenderStateObject {
  public:
     
    /// Constructor.
    ClipPlane( Inst< SFNode  >  _metadata         = 0,
               Inst< SFBool  >  _enabled          = 0,
               Inst< SFVec4d >  _plane            = 0,
               Inst< SFBool  >  _clipHaptics      = 0,
               Inst< SFBool  >  _clipGraphics     = 0 );

    /// Turn the clip plane on.
    virtual void enableGraphicsState();

    /// Turn the clip plane off.
    virtual void disableGraphicsState();

    /// Disable haptics if device position is on the clipped side
    /// of the clip plane.
    virtual void enableHapticsState( TraverseInfo &ti );

    /// Reenable haptics.
    virtual void disableHapticsState( TraverseInfo &ti );
  
    /// Specifies if the clipping plane should be enabled or not.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> true \n
    /// 
    /// \dotfile ClipPlane_enabled.dot
    auto_ptr< SFBool >  enabled;

    /// The plane equation that will be used to clip the geometry. 
    /// The points that are drawn are the points that satisfy the
    /// inequation: 
    /// plane.x*x+plane.y*y+plane.z*z+plane.w >= 0
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> Vec4d( 0, 1, 0, 0 ) \n
    /// 
    /// \dotfile ClipPlane_plane.dot
    auto_ptr< SFVec4d >  plane;

    /// Specifies if the clipping plane should clip the haptics if
    /// enabled, i.e. if the haptics device is in a position that
    /// is clipped by the plane no haptics will be rendered for
    /// the device.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> true \n
    /// 
    /// \dotfile ClipPlane_clipHaptics.dot
    auto_ptr< SFBool >  clipHaptics;

    /// Specifies if the clipping plane should clip the graphics
    /// if enabled.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> true \n
    /// 
    /// \dotfile ClipPlane_enabled.dot
    auto_ptr< SFBool >  clipGraphics;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;

  protected:
    /// The index of the plane when rendered by OpenGL. -1 if currently
    /// not rendered.
    int plane_index;
    /// Was haptics enabled in the last call to enableHapticsState()?
    bool haptics_enabled;
    /// The maximum number of clip planes the graphics hardware supports.
    /// Set in the first call to enable().
    static GLint max_nr_clip_planes;
    /// The number of clip planes that are currently rendered.
    static int nr_active_clip_planes;
    
  };
}

#endif
