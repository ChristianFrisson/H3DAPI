//////////////////////////////////////////////////////////////////////////////
//    Copyright 2004, SenseGraphics AB
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
/// \file TexGenGeometry.cpp
/// \brief CPP file for TexGenGeometry, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include "TexGenGeometry.h"
#include "HLShape.h"
#include "HapticShape.h"

using namespace H3D;

H3DNodeDatabase TexGenGeometry::database( "TexGenGeometry", 
                                          &(newInstance< TexGenGeometry >),
                                          typeid( TexGenGeometry ),
                                          &X3DGeometryNode::database );

namespace TexGenGeometryInternals {
  FIELDDB_ELEMENT( TexGenGeometry, geometry, INPUT_OUTPUT );
  FIELDDB_ELEMENT( TexGenGeometry, texCoord, INPUT_OUTPUT );
}

TexGenGeometry::TexGenGeometry( 
                                 Inst< SFNode>  _metadata,
                                 Inst< SFBound > _bound,
                                 Inst< DisplayList > _displayList,
                                 Inst< MFBool > _isTouched,
                                 Inst< MFVec3f > _force,
                                 Inst< MFVec3f > _contactPoint,
                                 Inst< MFVec3f > _contactNormal,
                                 Inst< SFGeometryNode > _geometry,
                                 Inst< SFTexCoordGenNode > _texCoord ) :
  X3DGeometryNode( _metadata, _bound, _displayList, _isTouched,
                   _force, _contactPoint, _contactNormal ),
  geometry( _geometry ),
  texCoord( _texCoord ) {

  type_name = "TexGenGeometry";
  database.initFields( this );
}

void TexGenGeometry::traverseSG( TraverseInfo &ti ) {
  X3DGeometryNode *g = geometry->getValue();
  if( g ) g->traverseSG( ti );
}

void TexGenGeometry::render() {
  X3DGeometryNode *g = geometry->getValue();
  TextureCoordinateGenerator *tex_gen = texCoord->getValue();
  if( tex_gen ) tex_gen->startTexGen();
  if( g ) g->displayList->callList();
  if( tex_gen ) tex_gen->stopTexGen();
}


