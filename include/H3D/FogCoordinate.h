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
/// \file FogCoordinate.h
/// \brief Header file for FogCoordinate.
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __FOGCOORDINATE_H__
#define __FOGCOORDINATE_H__

#include <H3D/X3DVertexAttributeNode.h>
#include <H3D/MFFloat.h>
#include <H3D/SFInt32.h>
#include <GL/glew.h>
#include <H3D/GLVertexAttributeObject.h>
#include <H3D/SFBool.h>

namespace H3D {
  /// \ingroup X3DNodes
  /// \class FogCoordinate
  /// \brief This node defines a set of explicit fog depths on a per-vertex
  /// basis. This depth value shall be applied per-vertex and used to replace
  /// the automatically generated depth. Fog coordinates take precedence over
  /// implicitly generated depths; specifying fog coordinates will result
  /// in the implicit depth (specified by the visibilityRange field of a
  /// node derived from X3DFogObject) being ignored. 
  ///
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/FogCoordinate.x3d">FogCoordinate.x3d</a>
  ///     ( <a href="examples/FogCoordinate.x3d.html">Source</a> )
  ///
  /// \par Internal routes:
  /// \dotfile FogCoordinate.dot
  //  
  // TODO: One depth value per vertex shall be supplied. If the user does not
  // provide a  sufficient number of depth values, the last value defined
  // shall be replicated for any further vertices. If too many depth values
  // are supplied, the excess depth values shall be ignored.

  class H3DAPI_API FogCoordinate: 
    public X3DGeometricPropertyNode,
    public GLVertexAttributeObject{
  public:
    /// Constructor.
    FogCoordinate(  Inst< MFFloat  > _depth         = 0,
                    Inst< SFNode   > _metadata      = 0);

    /// Destructor.
    virtual ~FogCoordinate();

    /// Perform the OpenGL commands to render fog until
    /// the given index.
    virtual void render( int depth_index );

    /// Perform the OpenGL commands to render the fog for
    /// the whole fog array.
    virtual void renderArray();

    /// Disable the array state enabled in renderArray().
    virtual void disableArray();

    /// Implement the method to specify data and releated information
    virtual void setAttributeData ( );

    /// VBO rendering implementation
    virtual void renderVBO ( );

    /// VBO disabling implementation
    virtual void disableVBO ( );

    /// check if need to render the vertex attribute
    virtual bool preRenderCheckFail ( );
    
    /// Returns the default xml containerField attribute value.
    /// For this node it is "fogCoord".
    ///
    virtual string defaultXMLContainerField() {
      return "fogCoord";
    }

    /// Depth used to replace the automatic depth.
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> [] \n
    ///
    /// \dotfile FogCoordinate_depth.dot
    auto_ptr< MFFloat >  depth;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;

  };
}

#endif
