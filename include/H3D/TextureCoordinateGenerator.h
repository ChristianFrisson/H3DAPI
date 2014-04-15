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
/// \file TextureCoordinateGenerator.h
/// \brief Header file for TextureCoordinateGenerator, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __TEXTURECOORDINATEGENERATOR_H__
#define __TEXTURECOORDINATEGENERATOR_H__

#include <H3D/X3DTextureCoordinateNode.h>
#include <H3D/SFString.h>
#include <H3D/MFFloat.h>

namespace H3D {

  /// \ingroup X3DNodes
  /// \class TextureCoordinateGenerator
  /// \brief TextureCoordinateGenerator supports the automatic generation 
  /// of texture coodinates for geometric shapes. 
  ///
  /// This node only implements some of the modes that are specified in the
  /// X3D specification.
  ///
  /// This node can be used to set the texture coordinates for a node with
  /// a texCoord field.
  ///
  /// The mode field describes the algorithm used to compute texture
  /// coordinates.
  ///
  /// The available modes in H3D API are:
  /// <table>
  /// <tr><th colspan=2>H3D API specific modes (not part of the X3D 
  /// standard)<th></tr>
  /// <tr><td>"GL_SPHERE"<td>Sphere mapping using the GL_SPHERE_MAP
  /// function in OpenGL. See documentation for glTexGen for details </tr>
  /// <tr><td>"MATRIX"<td>Specify directly the matrix for transforming
  /// from vertex coordinates to texture coordinates. The matrix values
  /// are given in the parameter field and 12 values are required. The 
  /// resulting texture coordinates will be: \n
  /// s = p[0] * v.x + p[1] * v.y + p[2] * v.z + p[3] \n
  /// t = p[4] * v.x + p[5] * v.y + p[6] * v.z + p[7] \n
  /// r = p[8] * v.x + p[9] * v.y + p[10] * v.z + p[11] \n \n
  /// where p[i] is the i:th value of the parameter field and v
  /// is a vertex.
  /// </tr>
  /// <tr><th colspan=2>X3D modes<th></tr>
  /// <tr><td>"CAMERASPACEPOSITION"<td>Use the vertex position, transformed
  /// to camera space, as input texture coordinates</tr>
  /// <tr><td>"COORD"<td>use vertex coordinates</tr>
  /// <tr><td>"COORD-EYE"<td>use vertex coordinates transformed to 
  /// <tr><td>"CAMERASPACEREFLECTIONVECTOR"<td>Use the reflection vector, 
  /// transformed to camera space, as input texture coordinates.>/tr> 
  /// <tr><td>"CAMERASPACENORMAL"<td>Use the vertex normal, transformed to
  /// camera space, as input texture coordinates, resulting coordinates are
  /// in -1 to 1 range.</tr>
  /// </table>
  /// 
  /// The parameter field gives input paramters that can be used by the
  /// algorithm specified by the mode field.
  ///
  /// \par Internal routes:
  /// \dotfile TextureCoordinateGenerator.dot
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/TextureCoordinateGenerator.x3d">TextureCoordinateGenerator.x3d</a>
  ///     ( <a href="examples/TextureCoordinateGenerator.x3d.html">Source</a> )
  ///   - <a href="../../../H3DAPI/examples/All/TextureCoordinateGenerator1.x3d">TextureCoordinateGenerator1.x3d</a>
  ///     ( <a href="examples/TextureCoordinateGenerator1.x3d.html">Source</a> )
  class H3DAPI_API TextureCoordinateGenerator : 
    public X3DTextureCoordinateNode {
  public:

    /// Constructor.
    TextureCoordinateGenerator( Inst< SFNode   > _metadata  = 0,
                                Inst< SFString > _mode      = 0,
                                Inst< MFFloat  > _parameter = 0 );
    
    /// Returns true
    virtual bool supportsTexGen() {
      return true;
    } 

    /// Start hardware texture coordinate generation. Algorithm depends
    /// on the value of the mode field.
    virtual void startTexGen();

    /// Stop the texture coordinate generation started with startTexGen().
    virtual void stopTexGen();

    /// The mode field describes the algorithm used to compute texture
    /// coordinates.
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> "GL_SPHERE" \n
    ///
    /// \dotfile TextureCoordinateGenerator_mode.dot 
    auto_ptr< SFString >  mode;

    /// Parameters that can be used as input to the texture coordinate
    /// algorithm.
    /// 
    /// <b>Access type:</b> inputOutput \n
    ///
    /// \dotfile TextureCoordinateGenerator_parameter.dot 
    auto_ptr< MFFloat >  parameter;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  };
}

#endif
