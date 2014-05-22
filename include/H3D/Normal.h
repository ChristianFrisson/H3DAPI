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
/// \file Normal.h
/// \brief Header file for Normal, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __NORMAL_H__
#define __NORMAL_H__

#include <H3D/X3DNormalNode.h>
#include <H3D/FieldTemplates.h>
#include <GL/glew.h>
#include <H3D/MFVec3f.h>
#include <H3D/SFBool.h>

namespace H3D {

  /// \ingroup X3DNodes
  /// \class Normal
  /// \brief This node defines a set of 3D surface normal vectors to be used in the
  /// vector field of some geometry nodes.
  ///
  /// This node defines a set of 3D surface normal vectors to be used in the
  /// vector field of some geometry nodes (EXAMPLE  IndexedFaceSet and
  /// ElevationGrid). This node contains one multiple-valued field that
  /// contains the normal vectors. Normals shall be of unit length. 
  ///
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/Normal.x3d">Normal.x3d</a>
  ///     ( <a href="examples/Normal.x3d.html">Source</a> )
  ///
  /// \par Internal routes:
  /// \dotfile Normal.dot
  class H3DAPI_API Normal : public X3DNormalNode {
  public:

    /// Constructor.
    Normal( Inst< SFNode >  _metadata = 0,
            Inst< MFVec3f>  _vector   = 0 );

    /// Destructor
    virtual ~Normal();

    /// Gets the normal of a given index.
    virtual Vec3f getNormal( int index ) {
      return vector->getValueByIndex( index );
    }

    /// Perform the OpenGL commands to render a normal given the index
    /// of the normal. Installs the normal as a glNormal3f.
    virtual void render( int index ) {
      const Vec3f &v = vector->getValueByIndex( index );
      glNormal3f( v.x, v.y, v.z );
    }

    /// Perform the OpenGL commands to render all vertices as a vertex
    /// array.
    virtual void renderArray();

    /// Disable the array state enabled in renderArray().
    virtual void disableArray();

    /// Implement the method to specify data and releated information
    virtual void setAttributeData ( );

    /// VBO rendering implementation
    virtual void renderVBO ( );

    /// VBO disabling implementation
    virtual void disableVBO ( );

    /// Check if this vertex attribute needs to be rendered
    virtual bool preRenderCheckFail ( );

    /// Returns the number of normals this normal node can render.
    virtual unsigned int nrAvailableNormals() {
      return vector->size();
    }

    /// A vector of 3D normal vectors.
    ///
    /// <b>Access type: </b> inputOutput
    ///
    /// \dotfile Normal_vector.dot
    auto_ptr< MFVec3f >  vector;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;

  };
}

#endif
