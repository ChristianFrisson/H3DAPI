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
/// \file TexGenGeometry.h
/// \brief Header file for TexGenGeometry.
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __TEXGENGEOMETRY_H__
#define __TEXGENGEOMETRY_H__

#include <H3D/X3DGeometryNode.h>
#include <H3D/TextureCoordinateGenerator.h>
#include <H3D/DependentNodeFields.h>

namespace H3D {

  /// \ingroup H3DNodes
  /// \class TexGenGeometry
  /// \brief The TexGenGeometry node is a wrapper node that lets you specify
  /// a TextureCoordinateGenerator node to generate the texture coordinates
  /// of a geometry instead of using the default ones.
  ///
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/TexGenGeometry.x3d">TexGenGeometry.x3d</a>
  ///     ( <a href="examples/TexGenGeometry.x3d.html">Source</a> )
  ///
  /// \par Internal routes:
  /// \dotfile TexGenGeometry.dot
  class H3DAPI_API TexGenGeometry : 
    public X3DGeometryNode {
  public:

    /// The SFTextureCoordinteNode is dependent on the propertyChanged 
    /// field of the contained TextureCoordinateGenerator.
    typedef DependentSFNode< 
                TextureCoordinateGenerator,
                FieldRef< X3DGeometricPropertyNode,
                          Field,
                          &X3DGeometricPropertyNode::propertyChanged > > 
    SFTexCoordGenNode;   
    
    /// The SFGeometryNode keeps a route between the contained 
    /// X3DGeometryNode's bound field and the bound field ot the 
    /// TexGenGeometry.
    class SFGeometryNode: 
      public DependentSFNode< X3DGeometryNode, 
      FieldRef< H3DDisplayListObject,
      H3DDisplayListObject::DisplayList,
                            &H3DDisplayListObject::displayList >, 
      true > {
    public:
      ~SFGeometryNode() { value = NULL; }
    protected:
      virtual void onAdd( Node *n ) {
        DependentSFNode< X3DGeometryNode, 
                         FieldRef< H3DDisplayListObject,
          H3DDisplayListObject::DisplayList,
          &H3DDisplayListObject::displayList >, 
          true >::onAdd( n );
        X3DGeometryNode *g = dynamic_cast< X3DGeometryNode * >( n );
        X3DGeometryNode *own = 
          dynamic_cast< X3DGeometryNode * >( getOwner() );
        if( g && own ) g->bound->route( own->bound ); 
      }

      virtual void onRemove( Node *n ) {
        DependentSFNode< X3DGeometryNode, 
                         FieldRef< H3DDisplayListObject,
          H3DDisplayListObject::DisplayList,
          &H3DDisplayListObject::displayList >, 
          true >::onRemove( n );
        X3DGeometryNode *g = dynamic_cast< X3DGeometryNode * >( n );
        X3DGeometryNode *own = 
          dynamic_cast< X3DGeometryNode * >( getOwner() );
        if( g && own ) g->bound->unroute( own->bound ); 
      }
    };

    /// Constructor.
    TexGenGeometry( Inst< SFNode      >  _metadata = 0,
                    Inst< SFBound     > _bound = 0,
                    Inst< DisplayList > _displayList = 0,
                    Inst< MFBool      > _isTouched = 0,
                    Inst< MFVec3f     > _force = 0,
                    Inst< MFVec3f     > _contactPoint = 0,
                    Inst< MFVec3f     > _contactNormal = 0,
                    Inst< SFGeometryNode > _geometry = 0,
                    Inst< SFTexCoordGenNode > _texCoord = 0 );

    /// Starts texture coord generation and renders the geometry.
    virtual void render();

    /// Traverse the scenegraph.
    virtual void traverseSG( TraverseInfo &ti );

    /// The number of triangles renderered in this geometry.
    virtual int nrTriangles() {
      X3DGeometryNode *g = geometry->getValue();
      if( g ) return g->nrTriangles();
      else return 0;
    }

    /// The geometry field contains the X3DGeometryNode that we want to 
    /// generate texture coordinates for.
    ///
    /// <b>Access type: </b> inputOutput \n
    /// 
    /// \dotfile TexGenGeometry_geometry.dot
    auto_ptr< SFGeometryNode > geometry;

    /// The texCoord field contains the TextureCoordinateGenerator node
    /// to use to generate texture coordinates.
    /// 
    /// <b>Access type:</b> inputOutput \n
    ///
    /// \dotfile TexGenGeometry_texCoord.dot
    auto_ptr< SFTexCoordGenNode > texCoord;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  };
}

#endif
