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
/// \file GraphicsOptions.h
/// \brief Header file for GraphicsOptions.
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __GRAPHICSOPTIONS_H__
#define __GRAPHICSOPTIONS_H__

#include <H3D/H3DOptionNode.h>
#include <H3D/SFBool.h>
#include <H3D/SFInt32.h>
#include <H3D/X3DGeometryNode.h>
#include <H3D/ShadowCaster.h>

namespace H3D {

  /// \ingroup H3DNodes
  /// \class GraphicsOptions
  /// \brief Specifies parameters to use for graphical rendering of nodes.
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/GraphicsOptions.x3d">GraphicsOptions.x3d</a>
  ///     ( <a href="examples/GraphicsOptions.x3d.html">Source</a> )
  class H3DAPI_API GraphicsOptions : public H3DOptionNode {
  public:

    typedef TypedSFNode< ShadowCaster > SFShadowCaster;
    
    /// Constructor.
    GraphicsOptions( Inst< SFNode  > _metadata = 0,
                     Inst< SFBool  > _useCaching = 0,
                     Inst< SFInt32 > _cachingDelay  = 0,
                     Inst< SFBool  > _cacheOnlyGeometryNodes = 0,
                     Inst< SFString > _frustumCullingMode = 0,
                     Inst< SFBool  > _useDefaultShadows = 0,
                     Inst< SFFloat > _defaultShadowDarkness = 0,
                     Inst< SFFloat > _defaultShadowDepthOffset = 0,
                     Inst< SFBool > _preferVertexBufferObject = 0,
                     Inst< SFString > _defaultShadowGeometryAlgorithm = 0,
                     Inst< SFShadowCaster > _defaultShadowCaster = 0 );
    
    bool cacheNode( Node *n ) {
      if( !useCaching->getValue() ) return false;
      if( !cacheOnlyGeometries->getValue() ) return true;

      // only X3DGeometryNodes are cached
      return dynamic_cast< X3DGeometryNode * >( n ) != NULL;
      
    }

    /// The useCaching field specifies if display lists should be used to cache
    /// OpenGL calls in order to speed up rendering of non-changing parts.
    ///
    /// <b>Default value: </b> true \n
    /// <b>Access type: </b> inputOutput \n
    auto_ptr< SFBool > useCaching;

    /// Specifies the number of frames the fields of a node that affect the 
    /// graphical rendering must be unchanged in order for a display list 
    /// cache to be built. E.g. a value of 3 means that a display list will be 
    /// build after rendering the node successfully 3 frames in a row without
    /// changes to any fields that affect the rendering.
    ///
    /// <b>Default value: </b> 3 \n
    /// <b>Access type: </b> inputOutput \n
    auto_ptr< SFInt32 > cachingDelay;


    /// If the cacheOnlyGeometries field is true only nodes that inherit
    /// from X3DGeometryNode will be cached. 
    ///
    /// <b>Default value: </b> false \n
    /// <b>Access type: </b> inputOutput \n
    auto_ptr< SFBool > cacheOnlyGeometries;

    /// Determines if view frustum culling should be made and if so at what
    /// level. View frustum culling will use the bounding box of a node
    /// and check it against the 6 planes of the view frustum to determine
    /// if it should be rendered or not. Everything that is outside the view
    /// frustum can be skipped.
    ///
    /// Valid values are:
    /// - "NO_CULLING" - no view frustum culling is done.
    /// - "GEOMETRY" - culling is done at the geometry level, i.e. only
    /// X3DGeometryNodes will perform culling
    /// - "ALL" - culling will be done on all nodes with a bounding box
    /// in the scene graph.
    ///
    /// <b>Default value: </b> "NO_CULLING" \n
    /// <b>Access type: </b> inputOutput \n
    auto_ptr< SFString > frustumCullingMode;

    /// Controls if shadows created by using the shadow field in Appearance
    /// nodes should be on or not. This option will only have effect if
    /// part of a GlobalSettings node.
    ///
    /// <b>Default value: </b> true \n
    /// <b>Access type: </b> inputOutput \n
    auto_ptr< SFBool > useDefaultShadows;

    /// Controls the shadow darkness of the default ShadowCaster that is 
    /// is used by shadows created by using the shadow field in Appearance
    /// nodes. This option will only have effect if part of a GlobalSettings
    /// node.
    ///
    /// <b>Default value: </b> 0.4 \n
    /// <b>Access type: </b> inputOutput \n
    auto_ptr< SFFloat > defaultShadowDarkness;

    /// The defaultShadowDepthOffset field controls the z offset of drawn 
    /// shadow volumes. The z offset will be r * defaultShadowDepthOffset
    /// where r is the smallest value that is guaranteed to produce a 
    /// resolvable z offset for a given implementation. If this value is
    /// set to too small there will be z-fighting between shadow and object
    /// that casts the shadow(flickering). If this happens increase this value.
    /// The value needed depends on the precision of the depth buffer.
    ///
    /// <b>Default value: </b> 6 \n
    /// <b>Access type: </b> inputOutput \n
    auto_ptr< SFFloat > defaultShadowDepthOffset;

    /// Controls rendering of certain geometries, such as IndexedTriangleSet.
    /// If true then the affected geometries are rendered using vertex buffer
    /// object otherwise they are rendered as vertex arrays. This can be used
    /// to increase performance for certain geometries, such as large
    /// IndexedTriangleSets. Vertex buffer objects are only used if the
    /// graphics card supports it. Note that useCaching should be set to false
    /// to get the biggest effect from vertex buffer objects.
    /// NOTE: At the moment, no check is done that
    /// sizeof( GLfloat ) == sizeof( H3DFloat ), the same is true for
    /// the pairs GLdouble/H3DDouble and GLuint/H3DInt32. If you use this
    /// feature, make sure that this is true.
    ///
    /// <b>Default value: </b> false \n
    /// <b>Access type: </b> inputOutput \n
    auto_ptr< SFBool > preferVertexBufferObject;

    /// The defaultShadowGeometryAlgorithm field controls how to calculate
    /// and draw the shadow volumes for arbitrary volumes(see ShadowGeometry).
    /// - "GEOMETRY_SHADER" means that all is done in a geometry shader on 
    ///   the graphics card (if supported). This is faster but could have
    ///   worse precision (depending on graphics card).
    /// - "CPU" - all calculations for the shadow volume is done on the CPU.
    ///
    /// <b>Default value: </b> "GEOMETRY_SHADER" \n
    /// <b>Valid values: </b> "CPU", "GEOMETRY_SHADER" \n
    /// <b>Access type: </b> inputOutput \n
    auto_ptr< SFString > defaultShadowGeometryAlgorithm;

    /// The defaultShadowCaster field contains the ShadowCaster node
    /// used for default shadows.
    ///
    /// <b>Access type: </b> outputOnly \n
    auto_ptr< SFShadowCaster > defaultShadowCaster;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  };
}

#endif
