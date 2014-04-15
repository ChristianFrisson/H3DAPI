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
/// \file FullscreenRectangle.h
/// \brief Header file for FullscreenRectangle.
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __FULLSCREENRECTANGLE_H__
#define __FULLSCREENRECTANGLE_H__

#include <H3D/X3DGeometryNode.h>

namespace H3D {

  /// \ingroup X3DNodes
  /// \class FullscreenRectangle
  /// \brief The FullscreenRectangle node draws a rectangle covering
  /// the entire screen. 
  /// This can be useful e.g. to draw the output from the 
  /// FrameBufferTextureGenerator node.
  ///
  /// The zValue field specifies where in depth the rectangle will be drawn.
  /// This controls of it draws over what is already in the frame buffer or not.
  /// The value is between -1 and 1 where -1 means drawing at near plane and 1 
  /// at far plane.
  ///
  /// The screenAligned field determines if the rectangle should always been 
  /// drawn in relation to the screen(i.e. upperleft corner of rectangle is
  /// always mapped to upper left corner of the screen) or if it should be affected
  /// by transforms that flip coordinate axis such as using mirrored mode.
  /// Usually one sets screenAligned to true if using the FullscreenRectangle
  /// to show a generated frame buffer from e.g. FrameBufferTextureGenerator
  /// and false for other content that should be mirrored in mirrored mode.
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/FullscreenRectangle.x3d">FullscreenRectangle.x3d</a>
  ///     ( <a href="examples/FullscreenRectangle.x3d.html">Source</a> )
  ///
  /// \par Internal routes:
  /// \dotfile FullscreenRectangle.dot
  class H3DAPI_API FullscreenRectangle : 
  public X3DGeometryNode {
  public:

    /// Constructor.
    FullscreenRectangle( Inst< SFNode      > _metadata = 0,
                         Inst< SFBound     > _bound = 0,
                         Inst< DisplayList > _displayList = 0,
                         Inst< MFBool      > _isTouched = 0,
                         Inst< MFVec3f     > _force = 0,
                         Inst< MFVec3f     > _contactPoint = 0,
                         Inst< MFVec3f     > _contactNormal = 0,
                         Inst< SFFloat     > _zValue = 0,
                         Inst< SFBool      > _screenAligned = 0 );
   
    /// Renders the FullscreenRectangle using OpenGL.
    virtual void render();

    /// The number of triangles rendered by this geometry
    virtual int nrTriangles() {
      return 2;
    }

    /// The zValue field specifies where in depth the rectangle will be drawn.
    /// This controls of it draws over what is already in the frame buffer or not.
    /// The value is between -1 and 1 where -1 means drawing at near plane and 1 
    /// at far plane.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> -1
    /// <b>Valid values: </b>[-1 - 1]
    /// 
    /// \dotfile FullscreenRectangle_zValue.dot
    auto_ptr< SFFloat > zValue;

    /// The screenAligned field determines if the rectangle should always been 
    /// drawn in relation to the screen(i.e. upperleft corner of rectangle is
    /// always mapped to upper left corner of the screen) or if it should be affected
    /// by transforms that flip coordinate axis such as using mirrored mode.
    /// Usually one sets screenAligned to true if using the FullscreenRectangle
    /// to show a generated frame buffer from e.g. FrameBufferTextureGenerator
    /// and false for other content that should be mirrored in mirrored mode.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> false
    /// 
    /// \dotfile FullscreenRectangle_screenAligned.dot
    auto_ptr< SFBool > screenAligned;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;

  protected:
    // Internal variable used to store id for vertex of FullScreenRectangle
    static GLuint vbo_id[2];
    // Internal variable used to indicate if vbo_id is initialized
    static bool vbo_initialized;
    static vector< GLfloat > fullscreenRectangle_data;
    static vector< GLuint > fullscreenRectangle_index_data;

    void  fillVec3ToArray( const Vec3d V, vector<float>& array );
    void  fillVec3ToArray( const Vec3f V, vector<GLuint>& array );
  };
}

#endif
