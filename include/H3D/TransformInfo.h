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
/// \file TransformInfo.h
/// \brief Header file for TransformInfo.
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __TRANSFORMINFO_H__
#define __TRANSFORMINFO_H__

#include <H3D/X3DChildNode.h>
#include <H3D/SFMatrix4f.h>
#include <H3D/H3DDisplayListObject.h>

namespace H3D {

  /// \ingroup H3DNodes
  /// \brief The TransformInfo node provides information about the coordinate
  /// space it resides in in the scene graph. 
  /// 
  /// The accForwardMatrix field is the matrix that transforms from the local
  /// coordinate space where this Node resides in the scenegraph to 
  /// global space.
  /// 
  /// The accInverseMatrix field is the matrix that transforms from the 
  /// global coordinate space to the coordinate space this Node resides
  /// in in the scenegraph.
  ///
  /// The outputGLMatrices field determines if the OpenGL matrix fields 
  /// should be updated or not(the fields starting with gl). 
  /// Note that when this is true graphics caching will disabled for this
  /// node making all parent nodes not being cached. This is most of the
  /// time a non-issue though.
  ///
  /// The glModelViewMatrix is The OpenGL GL_MODELVIEW_MATRIX at the 
  /// current point of the transform hierarchy.
  ///
  /// The glModelViewMatrixInverse field is the inverse matrix of 
  /// glModelViewMatrix.
  ///
  /// The glProjectionMatrix is The OpenGL GL_PROJECTION_MATRIX at the 
  /// current point of the transform hierarchy.
  ///
  /// The glProjectionMatrixInverse field is the inverse matrix of 
  /// glProjectionMatrix.
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/TransformInfo.x3d">TransformInfo.x3d</a>
  ///     ( <a href="examples/TransformInfo.x3d.html">Source</a> )
  class H3DAPI_API TransformInfo : 
    public X3DChildNode,
    public H3DDisplayListObject {
  public:

    class H3DAPI_API Matrix4fInverse: public SFMatrix4f {
      virtual void update() {
        const Matrix4f &m = 
          static_cast< SFMatrix4f* >(event.ptr)->getValue();
        value = m.inverse();
      }
    };
    // matrix4f type matrix which accept modelView matrix and return 
    // its inverse transposed matrix
    class H3DAPI_API Matrix4fInverseTranspose : public SFMatrix4f{
      virtual void update ( ){
        const Matrix4f &m =
          static_cast<SFMatrix4f*>(event.ptr)->getValue ( );
        value = m.inverse ( ).transpose ( );
      }
    };

    /// Constructor.
    TransformInfo( Inst< SFNode      > _metadata  = 0,
                   Inst< SFMatrix4f  > _accForwardMatrix = 0,
                   Inst< SFMatrix4f  > _accInverseMatrix = 0,
                   Inst< DisplayList > _displayList    = 0,
                   Inst< SFBool      > _outputGLMatrices = 0,
                   Inst< SFMatrix4f  > _glModelViewMatrix = 0,
                   Inst< Matrix4fInverse > _glModelViewMatrixInverse = 0,
                   Inst< SFMatrix4f  > _glProjectionMatrix = 0,
                   Inst< Matrix4fInverse > _glProjectionMatrixInverse = 0,
                   Inst< Matrix4fInverseTranspose > _glModelViewMatrixInverseTranspose = 0 );

    /// Set the matrix fields from the values provided in ti.
    virtual void traverseSG( TraverseInfo &ti );

  virtual void render();

    /// The accForwardMatrix field is the matrix that transforms from the local
    /// coordinate space where this Node resides in the scenegraph to 
    /// global space.
    /// 
    /// <b>Access type:</b> outputOnly \n
    auto_ptr< SFMatrix4f >  accForwardMatrix;

    /// The accInverseMatrix field is the matrix that transforms from the 
    /// global coordinate space to the coordinate space this Node resides
    /// in in the scenegraph.
    /// 
    /// <b>Access type:</b> outputOnly \n
    auto_ptr< SFMatrix4f >  accInverseMatrix;

    /// Determines if the OpenGL matrix fields should be updated or not
    /// (the fields starting with gl). Note that when this is true 
    /// graphics caching will disabled for this node making all parent
    /// nodes not being cached. This is most of the time a non-issue though.
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> false \n
    auto_ptr< SFBool > outputGLMatrices;

    /// The OpenGL GL_MODELVIEW_MATRIX at the current point of the 
    /// transform hierarchy.
    /// 
    /// <b>Access type:</b> outputOnly \n
    auto_ptr< SFMatrix4f >  glModelViewMatrix;

    /// The inverse matrix of glModelViewMatrix
    /// 
    /// <b>Access type:</b> outputOnly \n
    auto_ptr< Matrix4fInverse >  glModelViewMatrixInverse;

    /// The OpenGL GL_PROJECTION_MATRIX at the current point of the 
    /// transform hierarchy.
    /// 
    /// <b>Access type:</b> outputOnly \n
    auto_ptr< SFMatrix4f >  glProjectionMatrix;

    /// The inverse matrix of glProjectionMatrix
    /// 
    /// <b>Access type:</b> outputOnly \n
    auto_ptr< Matrix4fInverse >  glProjectionMatrixInverse;

    /// The inverse transposed matrix of glModelViewMatrix, used to transform
    /// normal from object space to view/eye/camera space.
    auto_ptr< Matrix4fInverseTranspose > glModelViewMatrixInverseTranspose;
    
    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  };
}

#endif
