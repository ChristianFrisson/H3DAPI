//////////////////////////////////////////////////////////////////////////////
//    Copyright 2004-2007, SenseGraphics AB
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
/// \file X3DComposedGeometryNode.cpp
/// \brief CPP file for X3DComposedGeometryNode, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/X3DComposedGeometryNode.h>
#include <H3D/MultiTexture.h>
#include <H3D/MultiTextureCoordinate.h>

using namespace H3D;

H3DNodeDatabase X3DComposedGeometryNode::database( 
        "X3DComposedGeometryNode", 
        NULL,
        typeid( X3DComposedGeometryNode ),
        &X3DGeometryNode::database 
        );

namespace X3DComposedGeometryNodeInternals {
  FIELDDB_ELEMENT( X3DComposedGeometryNode, color, INPUT_OUTPUT );
  FIELDDB_ELEMENT( X3DComposedGeometryNode, coord, INPUT_OUTPUT );
  FIELDDB_ELEMENT( X3DComposedGeometryNode, normal, INPUT_OUTPUT );
  FIELDDB_ELEMENT( X3DComposedGeometryNode, texCoord, INPUT_OUTPUT );
  FIELDDB_ELEMENT( X3DComposedGeometryNode, ccw, INPUT_OUTPUT );
  FIELDDB_ELEMENT( X3DComposedGeometryNode, colorPerVertex, INPUT_OUTPUT );
  FIELDDB_ELEMENT( X3DComposedGeometryNode, normalPerVertex, INPUT_OUTPUT );
  FIELDDB_ELEMENT( X3DComposedGeometryNode, solid, INPUT_OUTPUT );
  FIELDDB_ELEMENT( X3DComposedGeometryNode, attrib, INPUT_OUTPUT );
  FIELDDB_ELEMENT( X3DComposedGeometryNode, fogCoord, INPUT_OUTPUT );
}

X3DComposedGeometryNode::X3DComposedGeometryNode( 
                              Inst< SFNode           > _metadata,
                              Inst< SFBound          > _bound,
                              Inst< DisplayList      > _displayList,             
                              Inst< SFColorNode             > _color,
                              Inst< SFCoordinateNode        > _coord,
                              Inst< SFNormalNode            > _normal,
                              Inst< SFTextureCoordinateNode > _texCoord,
                              Inst< SFBool                  > _ccw,
                              Inst< SFBool                  > _colorPerVertex,
                              Inst< SFBool                  > _normalPerVertex,
                              Inst< SFBool                  > _solid,
			                        Inst< MFVertexAttributeNode   > _attrib, 
                              Inst< SFFogCoordinate           > _fogCoord ) :
  X3DGeometryNode( _metadata, _bound, _displayList ),
  color          ( _color            ),
  coord          ( _coord            ),
  normal         ( _normal           ),
  texCoord       ( _texCoord         ),
  ccw            ( _ccw              ),
  colorPerVertex ( _colorPerVertex   ),
  normalPerVertex( _normalPerVertex  ),
  solid          ( _solid            ),
  attrib         ( _attrib           ),
  fogCoord         ( _fogCoord           ){

  type_name = "X3DComposedGeometryNode";

  database.initFields( this );

  ccw->setValue( true );
  colorPerVertex->setValue( true );
  normalPerVertex->setValue( true );
  solid->setValue( true );

  color->route( displayList );
  coord->route( displayList );
  normal->route( displayList );
  texCoord->route( displayList );
  ccw->route( displayList );
  colorPerVertex->route( displayList );
  normalPerVertex->route( displayList );
  solid->route( displayList );
  attrib->route( displayList );
  fogCoord->route( displayList );
}


void X3DComposedGeometryNode::DisplayList::callList( bool build_list ) {
  X3DComposedGeometryNode *cgn = 
    static_cast< X3DComposedGeometryNode * >( owner );
    
  bool ccw = cgn->ccw->getValue();  
  // determine which side of polygons is front and back face. Since the 
  // GLWindow renderCamera() function might scale the y-axis with -1 and
  // set the GL_FRONT_FACE to GL_CW if scaled and GL_CCW otherwise, we have
  // to check what the previous value is to determine what the GL_FRONT_FACE 
  // will be. This can also happen if a Transform node has negative scaling.
  GLint front_face;
  glGetIntegerv( GL_FRONT_FACE, &front_face );
  if( front_face == GL_CW ) {
    // we are in mirrored mode, so we have to set the front face
      // to the opposite in order for it to be right when mirrored.
    if( ccw )
      glFrontFace( GL_CW );
      else  
        glFrontFace( GL_CCW );
  } else {
    if( ccw )
      glFrontFace( GL_CCW );
    else 
      glFrontFace( GL_CW );
  }

  X3DGeometryNode::DisplayList::callList( build_list );
  
  // Restore the front face to its previuos value.
  glFrontFace( front_face );
}

Matrix4f X3DComposedGeometryNode::getDefaultTexGenMatrix() {

  BoxBound *box_bound = 
    dynamic_cast< BoxBound * >( bound->getValue() );
  if( box_bound ) {
    const Vec3f &center = box_bound->center->getValue();
    const Vec3f &size = box_bound->size->getValue();

    Matrix4f m(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0);
    H3DFloat *sparams = m[0];
    H3DFloat *tparams = m[1];
    H3DFloat *rparams = m[2];
      
    H3DFloat size_vec[]   = { size.x, size.y, size.z };
    H3DFloat center_vec[] = { center.x, center.y, center.z };
    
    // these variables are set to an index representing 
    // sides of the bounding box. 0 is the x-axis, 1 the y-axis
    // and 2 the z-axis.
    int largest_side, middle_side, smallest_side;
      
    if( size.x >= size.y ) {
      if( size.x >= size.z ) {
	largest_side = 0; 
	if( size.y >= size.z ) {
	  // size.x >= size.y >= size.z
	  middle_side   = 1;
	  smallest_side = 2;
	} else { 
	  // size.x >= size.z > size.y
	  middle_side   = 2;
	  smallest_side = 1;
	}
      } else {
	// size.z > size.x >= size.y
	largest_side  = 2; 
	middle_side   = 0;
	smallest_side = 1;
      }
    } else {
      if( size.z >= size.y ) {
	// size.z >= size.y > size.x
	largest_side  = 2; 
	middle_side   = 1;
	smallest_side = 0;
      } else if( size.x >= size.z ) {
	// size.y > size.x >=size.z
	largest_side  = 1; 
	middle_side   = 0;
	smallest_side = 2;
      } else {
	// size.y > size.z > size.x
	largest_side  = 1; 
	middle_side   = 2;
	smallest_side = 0;
      }
    }
    
    H3DFloat largest_length = size_vec[ largest_side ];
    if( H3DAbs( largest_length ) > Constants::f_epsilon ) {
      // parameters for the s coordinate
      H3DFloat length_inv = 1/largest_length;
      sparams[ largest_side ] = length_inv;
      sparams[3] = 0.5f - center_vec[ largest_side ] / largest_length;
      
      // parameters for the t coordinate
      tparams[ middle_side ] = length_inv;
      H3DFloat tcenter = size_vec[ middle_side ] / (2*largest_length);
      tparams[3] = tcenter - center_vec[ middle_side ] / largest_length;
      
      // parameters for the r coordinate
      rparams[ smallest_side ] = -length_inv;
      H3DFloat rcenter = size_vec[ smallest_side ] / (2*largest_length);
      rparams[3] = rcenter + center_vec[ smallest_side ] / largest_length;
    } else {
      sparams[3] = 0.5;
      tparams[3] = size_vec[ middle_side ] / (2*largest_length);
      rparams[3] = size_vec[ smallest_side ] / (2*largest_length);
    }
    return m;
  } else {
    stringstream s;
    s << "Could not calculate default  texture coordinate generation in IndexedFaceSet. "
      << "Requires bound object of BoxBound type. ";
    return Matrix4f();
  }
}

void X3DComposedGeometryNode::startTexGen( 
                            X3DTextureCoordinateNode *tex_coord_node ) {
  if( !tex_coord_node ) {
    
    Matrix4f m = getDefaultTexGenMatrix();
    
    H3DFloat *sparams = m[0];
    H3DFloat *tparams = m[1];
    H3DFloat *rparams = m[2];

    // TODO: maybe let the texture handle the texture coordinate
    // generation. Wait until implementation of TextureCoordinateGenerator
    // before deciding on how to do it.
    MultiTexture *mt = 
      dynamic_cast< MultiTexture * >( X3DTextureNode::getActiveTexture() );
    if( mt ) {
      size_t texture_units = mt->texture->size();
      for( size_t i = 0; i < texture_units; i++ ) {
	glActiveTexture( GL_TEXTURE0_ARB + (unsigned int) i );
	glTexGend( GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR );
	glTexGend( GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR );
	glTexGend( GL_R, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR );
	glTexGenfv( GL_S, GL_OBJECT_PLANE, sparams );
	glTexGenfv( GL_T, GL_OBJECT_PLANE, tparams );
	glTexGenfv( GL_R, GL_OBJECT_PLANE, rparams );
	glEnable( GL_TEXTURE_GEN_S );
	glEnable( GL_TEXTURE_GEN_T );
	glEnable( GL_TEXTURE_GEN_R );
      }
    } else {
      glTexGend( GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR );
      glTexGend( GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR );
      glTexGend( GL_R, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR );
      glTexGenfv( GL_S, GL_OBJECT_PLANE, sparams );
      glTexGenfv( GL_T, GL_OBJECT_PLANE, tparams );
      glTexGenfv( GL_R, GL_OBJECT_PLANE, rparams );
      glEnable( GL_TEXTURE_GEN_S );
      glEnable( GL_TEXTURE_GEN_T );
      glEnable( GL_TEXTURE_GEN_R );
    }
  } else {
    tex_coord_node->startTexGenForActiveTexture();
  }    
}

void X3DComposedGeometryNode::stopTexGen( 
                       X3DTextureCoordinateNode *tex_coord_node ) {
  if( !tex_coord_node ) {
    MultiTexture *mt = 
      dynamic_cast< MultiTexture * >( X3DTextureNode::getActiveTexture() );
    if( mt ) {
      size_t texture_units = mt->texture->size();
      for( size_t i = 0; i < texture_units; i++ ) {
        glActiveTexture( GL_TEXTURE0_ARB + (unsigned int) i );
        glDisable( GL_TEXTURE_GEN_S );
        glDisable( GL_TEXTURE_GEN_T );
        glDisable( GL_TEXTURE_GEN_R );
      }
    } else {
      glDisable( GL_TEXTURE_GEN_S );
      glDisable( GL_TEXTURE_GEN_T );
      glDisable( GL_TEXTURE_GEN_R );
    }
  } else {
    tex_coord_node->stopTexGenForActiveTexture();
  }
}

void X3DComposedGeometryNode::renderTexCoord( int index, 
                                              X3DTextureCoordinateNode *tc ) {
  tc->renderForActiveTexture( index );
}

void X3DComposedGeometryNode::renderTexCoordArray(  
                                      X3DTextureCoordinateNode *tc ) {
  tc->renderArrayForActiveTexture();
}

void X3DComposedGeometryNode::disableTexCoordArray(  
                                      X3DTextureCoordinateNode *tc ) {
  tc->disableArrayForActiveTexture();
}

void X3DComposedGeometryNode::renderTexCoordVertexBufferObject(
                                      X3DTextureCoordinateNode *tc ) {
  tc->renderVertexBufferObjectForActiveTexture();
}

void X3DComposedGeometryNode::disableTexCoordVertexBufferObject(
                                      X3DTextureCoordinateNode *tc ) {
  tc->disableVertexBufferObjectForActiveTexture();
}

