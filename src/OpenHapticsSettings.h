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
                         Inst< SFString  >  _defaultGLShape  = 0 );

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

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  };
}

#endif
