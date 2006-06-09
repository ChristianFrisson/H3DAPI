//////////////////////////////////////////////////////////////////////////////
//    Copyright 2004, SenseGraphics AB
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
/// \file OpenHapticsSettings.h
/// \brief Header file for OpenHapticsSettings
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __OPENHAPTICSSETTINGS_H__
#define __OPENHAPTICSSETTINGS_H__

#include "X3DBindableNode.h"
#include "SFString.h"
#include "SFBool.h"

namespace H3D {
  /// \ingroup H3DNodes
  /// \class OpenHapticsSettings
  /// OpenHapticsSettings is a X3DBindableNode that specifies default settings
  /// for OpenHaptics haptics rendering.

  class H3DAPI_API OpenHapticsSettings : public X3DBindableNode {
  public:

    /// Construtor.
    OpenHapticsSettings( Inst< SFSetBind >  _set_bind        = 0,
                         Inst< SFNode    >  _metadata        = 0,
                         Inst< SFTime    >  _bindTime        = 0,
                         Inst< SFBool    >  _isBound         = 0,
                         Inst< SFString  >  _defaultGLShape  = 0,
                         Inst< SFString  >  _touchableFace  = 0,
                         Inst< SFBool    >  _useAdaptiveViewport = 0,
                         Inst< SFBool    >  _useHapticCameraView = 0 );

    /// Convenience function to get the top of the DeviceInfo stack.
    static inline OpenHapticsSettings *getActive() {
      return static_cast< OpenHapticsSettings * >
        ( X3DBindableNode::getActive( "OpenHapticsSettings" ) );
    }

    /// Specifies the default OpenHaptics shape type to use when 
    /// rendering shapes using OpenGL. If "FEEDBACK_BUFFER" feedback 
    /// buffer shapes are used and if "DEPTH_BUFFER" depth buffer shapes
    /// are used. Feedback buffer shapes give more accurate haptics 
    /// rendering since it uses the actual triangles that are rendered.
    /// However more complex models might slow things down considerably.
    /// Depth buffer shapes are not as dependent on the complexity of
    /// the geometry, but instead uses the depth buffer that is of the 
    /// same resolution no matter how many triangles are rendered into 
    /// it. 
    ///
    /// <b>Valid values: </b> "FEEDBACK_BUFFER" or "DEPTH_BUFFER" \n
    /// <b>Default value: </b> "FEEDBACK_BUFFER" \n
    /// <b>Access type: </b> inputOutput \n
    auto_ptr< SFString > defaultGLShape;

    /// Specifies which sides of the shapes to render haptically. If "BACK"
    /// only the back side of can be felt, "FRONT" only front side and 
    /// "FRONT_AND_BACK" both sides.
    ///
    /// <b>Valid values: </b> "BACK", "FRONT" or "FRONT_AND_BACK" \n
    /// <b>Default value: </b> "FRONT_AND_BACK" \n
    /// <b>Access type: </b> inputOutput \n
    auto_ptr< SFString  > touchableFace;

    /// The default setting for use of adaptive viewport.
    /// When using depth buffer shapes, performance may be improved by 
    /// enabling the adaptive viewport optimization. This optimization 
    /// limits the region of the depth buffer that is read into memory
    /// to be used for haptic rendering to the area near the current proxy
    /// position. The performance improvement will depend on the speed at
    /// which the graphics card is able to read the depth image from the
    /// on board memory of the graphics accelerator. On many graphics
    /// accelerators, reading data from the depth buffer can be very costly.
    ///
    /// <b>Default value: </b> true \n
    /// <b>Access type: </b> inputOutput \n
    auto_ptr< SFBool > useAdaptiveViewport;

    /// The default setting for use of haptic camera view.
    /// When the haptic camera view is enabled, HLAPI will automatically 
    /// modify the viewing parameters used when rendering a depth buffer
    /// or feedback buffer shape so that only a subset of the geometry near
    /// the proxy position will be rendered. When the haptic camera view is
    /// enabled, HLAPI modifies the OpenGL view frustum so that only the shape
    /// geometry near the proxy position is rendered.
    /// For feedback buffer shapes, this can dramatically increase performance
    /// by reducing the number of geometric primitives considered for haptic 
    /// rendering. The improvement will depend on the density of the geometry
    /// in the region around the proxy, since denser geometry will lead to a 
    /// larger number of primitives in the haptic view frustum.
    /// For depth buffer shapes, this offers less of a performance improvement,
    /// since once the primitives have been rendered to the depth buffer, the
    /// actual haptic rendering of a depth buffer image is not dependent on
    /// the number of primitives. That said, there is some performance benefit
    /// to considering only the geometry near the proxy when generating a
    /// depth buffer image. In addition, when using haptic camera view, 
    /// HLAPI generates a depth buffer image that is a subset of the full
    /// depth buffer used by the graphics window so as with adaptive viewport, 
    /// less data is read back from the depth buffer. If haptic camera view
    /// is enabled, the adaptive viewport setting is ignored. In addition, 
    /// for depth buffer shapes, using haptic camera view allows you to feel
    /// parts of the geometry that are not viewable from the graphics view.
    ///
    /// <b>Default value: </b> true \n
    /// <b>Access type: </b> inputOutput \n    
    auto_ptr< SFBool > useHapticCameraView;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  };
}

#endif
