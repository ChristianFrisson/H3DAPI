
//////////////////////////////////////////////////////////////////////////////
//    H3D API.   Copyright 2004, Daniel Evestedt, Mark Dixon
//    All Rights Reserved
//
/// \file HLDepthBufferShape.cpp
/// \brief cpp file for HLDepthBufferShape
///
//
//////////////////////////////////////////////////////////////////////////////
#include "HLDepthBufferShape.h"
#include "GL/glew.h"
#include "X3DChildNode.h"
#include "HLSurface.h"
#include "HLHapticsDevice.h"

using namespace H3D;
#ifdef HAVE_OPENHAPTICS

void HLDepthBufferShape::hlRender( HLHapticsDevice *hd ) {
#ifdef HAVE_OPENHAPTICS
  HLSurface *s = dynamic_cast< HLSurface * >( surface );
  if( s && closeEnoughToBound( hd->proxyPosition->getValue(), 
                               transform.inverse(), 
                               geometry ) ) {
    hlMatrixMode( HL_VIEWTOUCH );
    hlPushMatrix();
#if HL_VERSION_MAJOR_NUMBER >= 2
    hlPushAttrib( HL_MATERIAL_BIT | HL_TOUCH_BIT );
#endif
    const Matrix4f &m = transform;
    HLfloat vt[] = { m[0][0], m[1][0], m[2][0], 0,
		     m[0][1], m[1][1], m[2][1], 0,
		     m[0][2], m[1][2], m[2][2], 0,
		     m[0][3], m[1][3], m[2][3], 1 };
    hlLoadMatrixf( vt );
    s->hlRender( hd );
    hlTouchableFace( touchable_face );
    if( use_haptic_camera )
      hlEnable( HL_HAPTIC_CAMERA_VIEW );
    else
      hlDisable( HL_HAPTIC_CAMERA_VIEW );

    if( use_adaptive_viewport )
      hlEnable( HL_ADAPTIVE_VIEWPORT );
    else
      hlDisable( HL_ADAPTIVE_VIEWPORT );

    Matrix3f m3 = m.getScaleRotationPart();
    GLint front_face;
    bool negative_scaling = 
      ( ( m3.getRow( 0 ) % m3.getRow( 1 ) ) * m3.getRow(2) ) < 0;
    glGetIntegerv( GL_FRONT_FACE, &front_face );
    // if front_face == GL_CW then the GLWindow we render to is mirrored.
    // front_face has to be flipped each time a negative scaling is applied
    // in order for normals to be correct. 
    if( front_face == GL_CW ) {
      if( !negative_scaling )
        glFrontFace( GL_CCW );
    } else if( negative_scaling )
      glFrontFace( GL_CW );

    hlBeginShape( HL_SHAPE_DEPTH_BUFFER, getShapeId( hd ) );
    glClear( GL_DEPTH_BUFFER_BIT );
    geometry->displayList->callList( false );
    hlEndShape();
    glFrontFace( front_face );
#if HL_VERSION_MAJOR_NUMBER >= 2
    hlPopAttrib();
#endif
    hlPopMatrix();
  }
#endif
}
#endif
