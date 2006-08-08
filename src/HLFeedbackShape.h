
//////////////////////////////////////////////////////////////////////////////
//    H3D API.   Copyright 2004, Daniel Evestedt, Mark Dixon
//    All Rights Reserved
//
/// \file HLFeedbackShape.h
/// \brief Header file for HLFeedbackShape.
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __HLFEEDBACKSHAPE_H__
#define __HLFEEDBACKSHAPE_H__

#include "X3DGeometryNode.h"
#include "H3DSurfaceNode.h"
#include "HapticShape.h"
#include "HLShape.h"


namespace H3D {

  /// Class for using the render() function of an X3DGeometryNode for
  /// haptics rendering. This makes it very easy to implement simple
  /// haptics rendering of a geometry, since when the render() function
  /// is implemented to perform the graphical rendering. The shape is
  /// implemented as a HL_SHAPE_FEEDBACK_BUFFER shape in HLAPI.
  class H3DAPI_API HLFeedbackShape: public HapticShape, public HLShape  {
  public:
    #ifndef HAVE_OPENHAPTICS
    typedef int HLint;
    typedef enum { HL_FRONT, HL_BACK, HL_FRONT_AND_BACK } HLenum;
    #endif

    /// Constructor.
    HLFeedbackShape( X3DGeometryNode *_geometry,
                     H3DSurfaceNode *_surface,
                     const Matrix4f &_transform,
                     HLint _nr_vertices = -1,
                     HLenum _touchable_face = HL_FRONT_AND_BACK,
                     bool _use_haptic_camera = true ):
      HapticShape( _geometry, _surface, _transform ),
      nr_vertices( _nr_vertices ),
      touchable_face( _touchable_face ),
      use_haptic_camera( _use_haptic_camera ) {}
    
    /// This function performs all the HLAPI calls that are needed to render
    /// the shape. Uses HL_SHAPE_FEEDBACK_BUFFER to render the object.     
    virtual void hlRender( HLHapticsDevice *hd );
  protected:
    /// A upper bound on the number of triangles that will be rendered.
    /// Negative values will use the system default value.
    HLint nr_vertices;
    
    /// Which sides of the faces are touchable.
    HLenum touchable_face;
    
    /// Enable HL_HAPTIC_CAMERA_VIEW or not
    bool use_haptic_camera;
  };
}

#endif
