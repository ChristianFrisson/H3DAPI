
//////////////////////////////////////////////////////////////////////////////
//    H3D API.   Copyright 2004, Daniel Evestedt, Mark Dixon
//    All Rights Reserved
//
/// \file HLDepthBufferShape.h
/// \brief Header file for HLDepthBufferShape.
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __HLDEPTHBUFFERSHAPE_H__
#define __HLDEPTHBUFFERSHAPE_H__

#include "X3DGeometryNode.h"
#include "H3DSurfaceNode.h"
#include "HapticShape.h"
#include "HLShape.h"

namespace H3D {

  /// Class for using the render() function of an X3DGeometryNode for
  /// haptics rendering. This makes it very easy to implement simple
  /// haptics rendering of a geometry, since when the render() function
  /// is implemented to perform the graphical rendering. The shape is
  /// implemented as a HL_SHAPE_DEPTH_BUFFER shape in HLAPI.
  class H3DAPI_API HLDepthBufferShape: public HapticShape, public HLShape  {
  public:
    /// Constructor.
    HLDepthBufferShape( X3DGeometryNode *_geometry,
                        H3DSurfaceNode *_surface,
                        const Matrix4f &_transform,
                        HLenum _touchable_face = HL_FRONT_AND_BACK,
                        bool _use_haptic_camera = true,
                        bool _use_adaptive_viewport = true ):
      HapticShape( _geometry,_surface, _transform ),
      touchable_face( _touchable_face ),
      use_haptic_camera( _use_haptic_camera ),
      use_adaptive_viewport( _use_adaptive_viewport ) {}
    
    /// This function performs all the HLAPI calls that are needed to render
    /// the shape. Uses HL_SHAPE_FEEDBACK_BUFFER to render the object.     
    virtual void hlRender( HLHapticsDevice *hd );

    /// Which sides of the faces are touchable.
    HLenum touchable_face;
    
    /// Enable HL_HAPTIC_CAMERA_VIEW or not
    bool use_haptic_camera;

   /// Enable HL_ADAPTIVE_VIEWPORT or not
    bool use_adaptive_viewport;
  };
}

#endif
