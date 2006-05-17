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
/// \file OpenHapticsOptions.h
/// \brief Header file for OpenHapticsOptions.
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __OPENHAPTICSOPTIONS_H__
#define __OPENHAPTICSOPTIONS_H__

#include "X3DRenderOptionsNode.h"
#include "SFString.h"

namespace H3D {

  /// \ingroup Nodes
  /// \class OpenHapticsOptions
  /// \brief Specifies parameters to use for when rendering an object
  /// with OpenHaptics.
  ///
  class H3DAPI_API OpenHapticsOptions : public X3DRenderOptionsNode {
  public:
    
    /// Constructor.
    OpenHapticsOptions( Inst< SFNode >  _metadata = 0,
                        Inst< SFString > _GLShape = 0 );
    
    /// Specifies the OpenHaptics shape type to use when 
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
    auto_ptr< SFString > GLShape;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  };
}

#endif
