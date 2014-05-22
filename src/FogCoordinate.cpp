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
/// \file FogCoordinate.cpp
/// \brief CPP file for FogCoordinate, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/FogCoordinate.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase FogCoordinate::database( 
                                   "FogCoordinate",
                                   &(newInstance<FogCoordinate>),
                                   typeid( FogCoordinate ),
                                   &X3DGeometricPropertyNode::database );

namespace FogCoordinateInternals {
  FIELDDB_ELEMENT( FogCoordinate, depth, INPUT_OUTPUT );
  FIELDDB_ELEMENT ( FogCoordinate, isDynamic, INPUT_OUTPUT );
}

FogCoordinate::FogCoordinate(   Inst< MFFloat  > _depth,
                                Inst< SFNode   > _metadata):
  X3DGeometricPropertyNode( _metadata ),
  GLVertexAttributeObject(VERTEXATTRIBUTE::FOGCOORD),
  depth( _depth ){

  type_name = "FogCoordinate";
  database.initFields( this );
  depth->route( propertyChanged );

  depth->route( vboFieldsUpToDate );
}

FogCoordinate::~FogCoordinate() {
}

// Perform the OpenGL commands to render fog until 
// the given index.
void FogCoordinate::render( int depth_index ) {
  if( GLEW_EXT_fog_coord ) {
    // according to FogCoordinate spec:
    // If the user does not provide a  sufficient number of depth values, 
    // the last value defined shall be replicated for any further vertices.
    if( depth_index >= (int) depth->size() && depth->size() > 0 ) {
      depth_index = depth->size() - 1;
    }
    
    const GLfloat d = depth->getValueByIndex( depth_index );
    glFogCoordfEXT( d);
  }
}

// Perform the OpenGL commands to render the fog for
// the whole fog array.
void FogCoordinate::renderArray() {
  if( GLEW_EXT_fog_coord  && !depth->empty() ) {
    glEnableClientState(GL_FOG_COORD_ARRAY);
    glFogCoordPointerEXT(GL_FLOAT, 0,
                     &(*depth->begin()) );
  }
}

void FogCoordinate::disableArray() {
  if( GLEW_EXT_fog_coord )
    glDisableClientState(GL_FOG_COORD_ARRAY);
}

bool FogCoordinate::preRenderCheckFail ( ){
  return GLVertexAttributeObject::preRenderCheckFail()||
    !GLEW_EXT_fog_coord || depth->empty ( );
}

void FogCoordinate::setAttributeData ( ){
  attrib_data = (GLvoid*)&(*depth->begin ( ));
  attrib_size = depth->size ( ) *sizeof(GLfloat);
}

void FogCoordinate::renderVBO ( ){
  glEnableClientState ( GL_FOG_COORD_ARRAY );
  if ( use_bindless )
  {
    glFogCoordFormatNV ( GL_FLOAT, 0 );
    glEnableClientState ( GL_VERTEX_ATTRIB_ARRAY_UNIFIED_NV );
    // vbo is dedicated for this vertex attribute, so there is no offset
    glBufferAddressRangeNV ( GL_FOG_COORD_ARRAY_ADDRESS_NV, 0, vbo_GPUaddr, attrib_size );
  } else
  {
    glFogCoordPointerEXT ( GL_FLOAT, 0, NULL );
  }
}

void FogCoordinate::disableVBO ( ){
  if ( use_bindless )
  {
    glDisableClientState ( GL_VERTEX_ATTRIB_ARRAY_UNIFIED_NV );
  }
  glDisableClientState ( GL_FOG_COORD_ARRAY );
}