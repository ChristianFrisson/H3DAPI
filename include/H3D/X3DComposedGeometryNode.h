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
/// \file X3DComposedGeometryNode.h
/// \brief Header file for X3DComposedGeometryNode, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __X3DCOMPOSEDGEOMETRYNODE_H__
#define __X3DCOMPOSEDGEOMETRYNODE_H__

#include <H3D/X3DGeometryNode.h>
#include <H3D/X3DCoordinateNode.h>
#include <H3D/TextureCoordinateGenerator.h>
#include <H3D/X3DColorNode.h>
#include <H3D/X3DNormalNode.h>
#include <H3D/X3DVertexAttributeNode.h>
#include <H3D/DependentNodeFields.h>
#include <H3D/FogCoordinate.h>


namespace H3D {

  /// \ingroup AbstractNodes
  /// \class X3DComposedGeometryNode
  /// \brief This is the base node type for all composed 3D geometry in X3D. 
  ///
  /// A composed geometry node type defines an abstract type that composes
  /// geometry from a set of nodes that define individual
  /// components. Composed geometry may have color, coordinates, normal and
  /// texture coordinates supplied. The rendered output of the combination
  /// of these is dependent on the concrete node definition. However, in
  /// general, the following rules shall be applied for all nodes:
  ///
  /// - If the color field is not NULL, it shall contain an X3DColorNode node
  ///   whose colours are applied to the vertices or faces of the
  ///   X3DComposedGeometryNode as follows: 
  ///
  /// - If colorPerVertex is FALSE, colours are applied to each face. If
  ///   colorPerVertex is true, colours are applied to each vertex. 
  ///
  /// - If the color field is NULL, the geometry shall be rendered normally
  ///   using the Material and texture defined in the Appearance node (see
  ///   12.2.2 Appearance node for details). 
  ///
  /// - If normalPerVertex is FALSE, colours are applied to each face. If
  ///   normalPerVertex is true, colours are applied to each vertex. 
  ///
  /// - If the normal field is not NULL, it shall contain a Normal node whose
  ///   normals are applied to the vertices or faces of the
  ///   X3DComposedGeometryNode in a manner exactly equivalent to that
  ///   described above for applying colours to vertices/faces (where
  ///   normalPerVertex corresponds to colorPerVertex and normalIndex
  ///   corresponds to colorIndex). 
  ///
  /// - If the normal field is NULL, the browser shall automatically generate
  ///   normals in accordance with the node's definition. If the node does not
  ///   define a behaviour, the default is to generate an averaged normal for
  ///   all faces that share that vertex. 
  ///
  /// If the texCoord field is not NULL, it shall contain a
  /// TextureCoordinate node.
  ///
  /// If the attrib field is not empty it shall contain a list of
  /// X3DVertexAttributeNode instances with per-vertex attribute
  /// information for programmable shaders.
  ///
  /// If the fogCoord field is not empty, it shall contain a list 
  /// of per-vertex depth values for calculating fog depth.
  /// 
  /// \par Internal routes:
  /// \dotfile X3DComposedGeometryNode.dot 
  class H3DAPI_API X3DComposedGeometryNode : public X3DGeometryNode {
  public:
    /// The SFColorNode is dependent on the propertyChanged field of the 
    /// contained X3DColorNode.
    typedef  DependentSFNode< X3DColorNode,
                              FieldRef< X3DGeometricPropertyNode,
                                        Field,
                                        &X3DColorNode::propertyChanged > >  
    SFColorNode;
    
    /// The SFCoordinateNode is dependent on the propertyChanged field of the 
    /// contained X3DCoordinateNode.
    typedef DependentSFNode< X3DCoordinateNode,
                             FieldRef< X3DGeometricPropertyNode,
                                       Field,
                                       &X3DCoordinateNode::propertyChanged > > 
    SFCoordinateNode;
    
    /// The SFNormalNode is dependent on the propertyChanged field of the 
    /// contained X3DNormalNode.
    typedef DependentSFNode< X3DNormalNode,
                             FieldRef< X3DGeometricPropertyNode,
                                       Field,
                                       &X3DNormalNode::propertyChanged > > 
    SFNormalNode;

    /// The SFTextureCoordinteNode is dependent on the propertyChanged 
    /// field of the contained X3DTextureCoordinateNode.
    typedef DependentSFNode< 
                X3DTextureCoordinateNode,
                FieldRef< X3DGeometricPropertyNode,
                          Field,
                          &X3DTextureCoordinateNode::propertyChanged > > 
    SFTextureCoordinateNode;   

    /// The MFVertexAttributeNode is dependent on the propertyChanged
    /// field of the contained X3DVertexAttributeNode.
    class H3DAPI_API MFVertexAttributeNode : public DependentMFNode< 
                X3DVertexAttributeNode,
                FieldRef< X3DGeometricPropertyNode,
                          Field,
                          &X3DVertexAttributeNode::propertyChanged > > {
    };

    
   /// The SFFogCoordinate is dependent on the propertyChanged
    /// field of the contained FogCoordinate.
    typedef DependentSFNode< 
                FogCoordinate,
                FieldRef< X3DGeometricPropertyNode,
                          Field,
                          &FogCoordinate::propertyChanged > > 
    SFFogCoordinate;   

    /// Display list is extended in order to set front sidedness of 
    /// triangles outside the display list. This is due to that the 
    /// front face value depends the previous value of front face. If we did 
    /// not do this and the display list was used in several places where
    /// the front face differs there would be normal errors. The drawback with
    /// having it outside the display list is that you cannot call the render()
    /// function to render the geometry correctly, but MUST use the callList()
    /// function in DisplayList.
    class H3DAPI_API DisplayList: public X3DGeometryNode::DisplayList {
    public: 
      /// Perform front face code outside the display list.
      virtual void callList( bool build_list = true );
    };

    /// Start texture coordinate generation. Texture coordinates will be
    /// generated for all texture units used by the currently active texture.
    /// If tex_coord is NULL then texture coordinates will be generated 
    /// from the bounding box of the geometry as defined in the X3D 
    /// specification. If tex_coord is not NULL then we use the 
    /// TextureCoordinateGenerator to define the texture coordinate generation.
    virtual void startTexGen( X3DTextureCoordinateNode *tex_coord );

    /// Stop texture coordinate generation.
    virtual void stopTexGen( X3DTextureCoordinateNode *tex_coord );

    /// Render the texure coordinate with the given index from the 
    /// tc argument. If the currently active texture is a MultiTexture
    /// the texture coordinate will be rendered for the texture units of
    /// all textures in MultiTexture. tc must not be NULL. 
    virtual void renderTexCoord( int index, X3DTextureCoordinateNode *tc );

    /// Render the X3DTextureCoordinate as an array. If the currently 
    /// active texture is a MultiTexture it will be rendered for the
    /// texture units of all textures in MultiTexture. tc must not be NULL. 
    virtual void renderTexCoordArray( X3DTextureCoordinateNode *tc );

    /// Disable the arrays that were enabled in renderTexCoordArray().
    virtual void disableTexCoordArray( X3DTextureCoordinateNode *tc );

    /// Render the X3DTextureCoordinate as a vertex buffer object. If the
    /// currently  active texture is a MultiTexture it will be rendered for the
    /// texture units of all textures in MultiTexture. tc must not be NULL.
    virtual void renderTexCoordVertexBufferObject(
                   X3DTextureCoordinateNode *tc );

    /// Disable the vertex buffer objects that were enabled in
    /// renderTexCoordArray().
    virtual void disableTexCoordVertexBufferObject(
                   X3DTextureCoordinateNode *tc );

    /// Gets the matrix that transforms from object space to texture
    /// coordinate space in the default case when no texCoord node
    /// is specified. 
    virtual Matrix4f getDefaultTexGenMatrix();

    /// Constructor.
    X3DComposedGeometryNode( Inst< SFNode           > _metadata        = 0,
                             Inst< SFBound          > _bound           = 0,
                             Inst< DisplayList      > _displayList     = 0,
                             Inst< SFColorNode      > _color           = 0,
                             Inst< SFCoordinateNode > _coord           = 0,
                             Inst< SFNormalNode     > _normal          = 0,
                             Inst< SFTextureCoordinateNode > _texCoord = 0,
                             Inst< SFBool           > _ccw             = 0,
                             Inst< SFBool           > _colorPerVertex  = 0,
                             Inst< SFBool           > _normalPerVertex = 0,
                             Inst< SFBool           > _solid           = 0,
                             Inst< MFVertexAttributeNode > _attrib     = 0,
                             Inst< SFFogCoordinate     > _fogCoord     =0 );

    /// Contains an X3DColorNode whose colors are applied to the
    /// X3DComposedGeometryNode. If the color field is NULL, the
    /// geometry shall be rendered normally using the Material
    /// and texture defined in the Appearance node
    /// 
    /// <b>Access type:</b> inputOutput \n
    ///
    /// \dotfile X3DComposedGeometryNode_color.dot 
    auto_ptr< SFColorNode >  color;

    /// An X3DCoordinate node with the coordinates to use.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// 
    /// \dotfile X3DComposedGeometryNode_coord.dot 
    auto_ptr< SFCoordinateNode >  coord;
    
    /// Contains an X3DNormalNode which colors are applied to the
    /// X3DComposedGeometryNode. If the normal field is NULL, the
    /// normals are automatically generated in accordance with the
    /// node's definition. If the node does not define a behaviour,
    /// the default is to generate an averaged normal for all faces
    /// that share that vertex.
    /// 
    /// <b>Access type:</b> inputOutput \n
    ///
    /// \dotfile X3DComposedGeometryNode_normal.dot 
    auto_ptr< SFNormalNode >  normal;
    
    /// Contains an X3DTextureCoordinateNode which colors are applied
    /// to the X3DComposedGeometryNode.
    /// 
    /// <b>Access type:</b> inputOutput \n
    ///
    /// \dotfile X3DComposedGeometryNode_texCoord.dot 
    auto_ptr< SFTextureCoordinateNode >  texCoord;
    
    /// The ccw field defines the ordering of the vertex coordinates of 
    /// the geometry with respect to user-given or automatically generated
    /// normal vectors used in the lighting model equations. If ccw is TRUE,
    /// the normals shall follow the right hand rule; the orientation of
    /// each normal with respect to the vertices (taken in order) shall be
    /// such that the vertices appear to be oriented in a counterclockwise
    /// order when the vertices are viewed (in the local coordinate system
    /// of the Shape) from the opposite direction as the normal. If ccw is
    /// FALSE, the normals shall be oriented in the opposite direction. If
    /// normals are not generated but are supplied using a Normal node, and
    /// the orientation of the normals does not match the setting of the 
    /// ccw field, results are undefined.
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> TRUE \n
    /// 
    /// \dotfile X3DComposedGeometryNode_ccw.dot 
    auto_ptr< SFBool >  ccw;
    
    /// If TRUE the colors in the color field is applied per vertex,
    /// otherwise it is applied per face.
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> TRUE \n
    /// 
    /// \dotfile X3DComposedGeometryNode_colorPerVertex.dot 
    auto_ptr< SFBool >  colorPerVertex;

    /// If TRUE the normals in the normal field is applied per vertex,
    /// otherwise it is applied per face.
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> TRUE \n
    /// 
    /// \dotfile X3DComposedGeometryNode_normalPerVertex.dot 
    auto_ptr< SFBool >  normalPerVertex;

    /// The solid field determines whether one or both sides of each polygon
    /// shall be displayed. If solid is FALSE, each polygon shall be visible
    /// regardless of the viewing direction (i.e., no backface culling shall
    /// be done, and two sided lighting shall be performed to illuminate both
    /// sides of lit surfaces). If solid is TRUE, back face culling will be 
    /// performed.
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> TRUE \n
    ///
    /// \dotfile X3DComposedGeometryNode_ccw.dot 
    auto_ptr< SFBool >  solid;
    
    /// If the attrib field is not empty it shall contain a list of
    /// X3DVertexAttributeNode instances with per-vertex attribute
    /// information for programmable shaders.
    /// 
    /// <b>Access type:</b> inputOutput \n
    ///
    /// \dotfile X3DComposedGeometryNode_attrib.dot 
    auto_ptr< MFVertexAttributeNode > attrib;

    /// If the fogCoord field is not empty, it shall contain a list 
    /// of per-vertex depth values for calculating fog depth.
    /// 
    /// <b>Access type:</b> inputOutput \n
    ///
    /// \dotfile X3DComposedGeometryNode_fogCoord.dot 
    auto_ptr< SFFogCoordinate > fogCoord;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  };
}

#endif
