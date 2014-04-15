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
/// \file Viewpoint.h
/// \brief Header file for Viewpoint, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __VIEWPOINT_H__
#define __VIEWPOINT_H__

#include <H3D/X3DViewpointNode.h>
#include <H3D/FieldTemplates.h>
#include <H3DUtil/Exception.h>

namespace H3D {
  /// \ingroup X3DNodes
  /// \class Viewpoint
  /// \brief The Viewpoint node defines a viewpoint that provides a perspective view
  /// of the scene. A perspective view is one in which all projectors are coalesce at
  /// position.
  ///
  /// The fieldOfView field specifies a preferred minimum viewing angle from this
  /// viewpoint in radians. A small field of view roughly corresponds to a telephoto
  /// lens; a large field of view roughly corresponds to a wide-angle lens. The field
  /// of view shall be greater than zero and smaller than pi. The value of fieldOfView
  /// represents the minimum viewing angle in any direction axis perpendicular to the
  /// view.
  /// A browser with a rectangular viewing projection has the following relationship:
  ///   display width    tan(FOVhorizontal/2) \n
  ///   -------------- = -------------------  \n
  ///   display height   tan(FOVvertical/2)   \n
  ///
  /// where the smaller of display width or display height determines which angle
  /// equals the fieldOfView (the larger angle is computed using the relationship
  /// described above). The larger angle shall not exceed pi and may force the smaller
  /// angle to be less than fieldOfView in order to sustain the aspect ratio.
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/Viewpoint.x3d">Viewpoint.x3d</a>
  ///     ( <a href="examples/Viewpoint.x3d.html">Source</a> )
  ///   - <a href="../../../H3DAPI/examples/All/Viewpoint1.x3d">Viewpoint1.x3d</a>
  ///     ( <a href="examples/Viewpoint1.x3d.html">Source</a> )
  class H3DAPI_API Viewpoint : public X3DViewpointNode {
  public:
    /// Constructor.
    Viewpoint( 
              Inst< SFSetBind >  _set_bind         = 0,
              Inst< SFVec3f   >  _centerOfRotation = 0,
              Inst< SFString  >  _description      = 0,
              Inst< SFFloat   >  _fieldOfView      = 0,
              Inst< SFBool    >  _jump             = 0,
              Inst< SFNode    >  _metadata         = 0,
              Inst< SFOrientation>  _orientation      = 0,
              Inst< SFPosition >  _position         = 0,
              Inst< SFBool    >  _retainUserOffsets = 0,
              Inst< SFTime    >  _bindTime         = 0,
              Inst< SFBool    >  _isBound          = 0,
              Inst< SFMatrix4f > _accForwardMatrix = 0,
              Inst< SFMatrix4f > _accInverseMatrix = 0 );


    // calculate the horizontal and vertical field of view components
    // as defined by the X3D spec for Viewpoint.
    virtual bool windowFromfieldOfView( H3DFloat width, H3DFloat height,
                                        H3DFloat clip_near,
                                        H3DFloat &top, H3DFloat &bottom,
                                        H3DFloat &right, H3DFloat &left );

    /// Set up perspective projection matrix. clip_far = -1 means far 
    /// plane at infinity.
    virtual void setupProjection( EyeMode eye_mode,
                                  H3DFloat width, H3DFloat height,
                                  H3DFloat clip_near, H3DFloat clip_far,
                                  StereoInfo * stereo_info = 0 );

    /// The fieldOfView field specifies a preferred minimum viewing angle
    /// from this viewpoint in radians. 
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> pi/4 \n
    /// <b>Value range:</b> (0,pi) \n
    auto_ptr< SFFloat    > fieldOfView;
    
    static H3DNodeDatabase database;
  };
}

#endif
