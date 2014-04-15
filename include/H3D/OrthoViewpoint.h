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
/// \file OrthoViewpoint.h
/// \brief Header file for OrthoViewpoint, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __ORTHOVIEWPOINT_H__
#define __ORTHOVIEWPOINT_H__

#include <H3D/X3DViewpointNode.h>
#include <H3D/FieldTemplates.h>
#include <H3D/MFFloat.h>
#include <H3DUtil/Exception.h>

namespace H3D {
  /// \ingroup X3DNodes
  /// \class OrthoViewpoint
  /// \brief The OrthoViewpoint node defines a viewpoint that provides an
  /// orthographic view of the scene. An orthographic view is one in which
  /// all projectors are parallel to the projector  from centerOfRotation
  /// to position.
  ///
  /// The fieldOfView field specifies minimum and maximum extents of the view
  /// in units of the local coordinate system. A small field of view roughly
  /// corresponds to a telephoto lens; a large field of view roughly
  /// corresponds to a wide-angle lens. The minimum an maximum values in each
  /// direction of the field of view shall have the relationship
  /// minimum < maximum. The value of fieldOfView represents the minimum
  /// viewing extent in any direction axis perpendicular to the view.
  ///
  /// The retainAspectRatio field determines if the view should retain
  /// the same aspect ratio as the current window or if the values
  /// in fieldOfView should be used directly. If true the view frustum
  /// can be extended in one direction in order to have the same aspect
  /// ratio as the window it is being rendered into. If false, the
  /// values in fieldOfView will always be used as is.
  ///
  /// A browser with a rectangular viewing projection has the following
  /// relationship:
  ///
  /// (display width) / (display height) = (maximum_x - minimum_x) / (maximum_y - minimum_y)
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/OrthoViewpoint.x3d">OrthoViewpoint.x3d</a>
  ///     ( <a href="examples/OrthoViewpoint.x3d.html">Source</a> )
  class H3DAPI_API OrthoViewpoint : public X3DViewpointNode {
  public:
    /// Constructor.
    OrthoViewpoint( 
              Inst< SFSetBind >  _set_bind         = 0,
              Inst< SFVec3f   >  _centerOfRotation = 0,
              Inst< SFString  >  _description      = 0,
              Inst< MFFloat   >  _fieldOfView      = 0,
              Inst< SFBool    >  _jump             = 0,
              Inst< SFNode    >  _metadata         = 0,
              Inst< SFOrientation>  _orientation      = 0,
              Inst< SFPosition >  _position         = 0,
              Inst< SFBool    >  _retainUserOffsets = 0,
              Inst< SFTime    >  _bindTime         = 0,
              Inst< SFBool    >  _isBound          = 0,
              Inst< SFMatrix4f > _accForwardMatrix = 0,
              Inst< SFMatrix4f > _accInverseMatrix = 0,
              Inst< SFBool    > _retainAspectRatio = 0 );


    // calculate the horizontal and vertical field of view components
    // as defined by the X3D spec for OrthoViewpoint.
    virtual bool windowFromfieldOfView( H3DFloat width, H3DFloat height,
                                        H3DFloat clip_near,
                                        H3DFloat &top, H3DFloat &bottom,
                                        H3DFloat &right, H3DFloat &left );

    /// The viewpoint needs to setup a projection mode, typically done by
    /// calling glFrustum but different viewpoints might want different
    /// behaviours.
    virtual void setupProjection( EyeMode eye_mode,
                                  H3DFloat width, H3DFloat height,
                                  H3DFloat clip_near, H3DFloat clip_far,
                                  StereoInfo * stereo_info = 0 );

    /// The fieldOfView field specifies minimum and maximum extents of the view
    /// in units of the local coordinate system. The minimum an maximum values
    /// in each direction of the field of view shall have the relationship
    /// minimum < maximum.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> -1, -1, 1, 1 \n
    auto_ptr< MFFloat    > fieldOfView;

    /// The retainAspectRatio field determines if the view should retain
    /// the same aspect ratio as the current window or if the values
    /// in fieldOfView should be used directly. If true the view frustim
    /// can be extended in one direction in order to have the same aspect
    /// ratio as the window it is being rendered into. If false, the
    /// values in fieldOfView will always be used as is.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> true \n
    auto_ptr< SFBool > retainAspectRatio;
    
    static H3DNodeDatabase database;
  };
}

#endif
