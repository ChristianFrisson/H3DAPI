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
/// \file StereoInfo.h
/// \brief Header file for StereoInfo, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __STEREOINFO_H__
#define __STEREOINFO_H__

#include <H3D/X3DBindableNode.h>
#include <H3D/SFFloat.h>
#include <H3D/SFRotation.h>

namespace H3D {
  /// \ingroup H3DNodes
  /// \class StereoInfo
  /// \brief The StereoInfo nis a bindable node that contains information
  /// used when rendering the scene in stereo.
  /// 
  /// The interocularDistance field specifies eye separation distance
  /// in metres, e.g. the distance between your two eyes.
  /// 
  /// The focalDistance field specifies the distance in metres at which
  /// objects in the scene will appear to be at zero parallax.
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/StereoInfo.x3d">StereoInfo.x3d</a>
  ///     ( <a href="examples/StereoInfo.x3d.html">Source</a> )
  class H3DAPI_API StereoInfo : public X3DBindableNode {
  public:
    /// Constructor.
    StereoInfo( Inst< SFSetBind  > _set_bind            = 0,
                Inst< SFNode     > _metadata            = 0,
                Inst< SFTime     > _bindTime            = 0,
                Inst< SFBool     > _isBound             = 0,
                Inst< SFFloat    > _interocularDistance = 0,
                Inst< SFFloat    > _focalDistance       = 0,
                Inst< SFRotation > _headTilt            = 0 );
    
    /// Convenience function to get the top of the StereoInfo stack.
    static inline StereoInfo *getActive() {
      return static_cast< StereoInfo * >( X3DBindableNode::getActive( "StereoInfo" ) );
    }

    /// The interocularDistance field specifies eye separation distance
    /// in metres, e.g. the distance between your two eyes.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 0.06 \n
    auto_ptr< SFFloat > interocularDistance;

    /// The focalDistance field specifies the distance in metres at which
    /// objects in the scene will appear to be at zero parallax.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 0.6 \n
    auto_ptr< SFFloat > focalDistance;

    /// The headTilt field specifies how the head is oriented in relation to
    /// to the current X3DViewpointNode viewing direction. The rotation is
    /// specified in local coordinate space of the viewpoint with the viewing
    /// direction being along the negative z axis and up begin positive y.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> Rotation(1,0,0,0) \n
    auto_ptr< SFRotation > headTilt;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  };
}

#endif
