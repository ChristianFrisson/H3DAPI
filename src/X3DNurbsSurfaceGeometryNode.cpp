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
/// \file X3DNurbsSurfaceGeometryNode.cpp
/// \brief CPP file for X3DNurbsSurfaceGeometryNode, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include "X3DNurbsSurfaceGeometryNode.h"

using namespace H3D;

H3DNodeDatabase X3DNurbsSurfaceGeometryNode::database( 
                                   "X3DNurbsSurfaceGeometryNode", 
                                   NULL,
                                   typeid( X3DNurbsSurfaceGeometryNode ),
                                   &X3DParametricGeometryNode::database );

namespace X3DNurbsSurfaceGeometryNodeInternals {
  FIELDDB_ELEMENT( X3DNurbsSurfaceGeometryNode, controlPoint, INPUT_OUTPUT );
  FIELDDB_ELEMENT( X3DNurbsSurfaceGeometryNode, texCoord, INPUT_OUTPUT );
  FIELDDB_ELEMENT( X3DNurbsSurfaceGeometryNode, uTessellation, INPUT_OUTPUT );
  FIELDDB_ELEMENT( X3DNurbsSurfaceGeometryNode, vTessellation, INPUT_OUTPUT );
  FIELDDB_ELEMENT( X3DNurbsSurfaceGeometryNode, weight, INPUT_OUTPUT );
  FIELDDB_ELEMENT( X3DNurbsSurfaceGeometryNode, solid, INPUT_OUTPUT );
  FIELDDB_ELEMENT( X3DNurbsSurfaceGeometryNode, uClosed, INPUT_OUTPUT );
  FIELDDB_ELEMENT( X3DNurbsSurfaceGeometryNode, vClosed, INPUT_OUTPUT );
  FIELDDB_ELEMENT( X3DNurbsSurfaceGeometryNode, uDimension, INPUT_OUTPUT );
  FIELDDB_ELEMENT( X3DNurbsSurfaceGeometryNode, vDimension, INPUT_OUTPUT );
  FIELDDB_ELEMENT( X3DNurbsSurfaceGeometryNode, uKnot, INPUT_OUTPUT );
  FIELDDB_ELEMENT( X3DNurbsSurfaceGeometryNode, vKnot, INPUT_OUTPUT );
  FIELDDB_ELEMENT( X3DNurbsSurfaceGeometryNode, uOrder, INPUT_OUTPUT );
  FIELDDB_ELEMENT( X3DNurbsSurfaceGeometryNode, vOrder, INPUT_OUTPUT );
}


X3DNurbsSurfaceGeometryNode::X3DNurbsSurfaceGeometryNode( 
                                 Inst< SFNode>  _metadata,
                                 Inst< SFBound > _bound,
                                 Inst< DisplayList > _displayList,
                                 Inst< MFBool > _isTouched,
                                 Inst< MFVec3f > _force,
                                 Inst< MFVec3f > _contactPoint,
                                 Inst< MFVec3f > _contactNormal,
                                 Inst< SFCoordinateNode > _controlPoint,
                                 Inst< SFTextureCoordinateNode > _texCoord,
                                 Inst< SFInt32     > _uTessellation,
                                 Inst< SFInt32     > _vTessellation,
                                 Inst< MFDouble    > _weight,
                                 Inst< SFBool      > _solid,
                                 Inst< SFBool      > _uClosed,
                                 Inst< SFBool      > _vClosed,
                                 Inst< SFInt32     > _uDimension,
                                 Inst< SFInt32     > _vDimension,
                                 Inst< MFDouble    > _uKnot,
                                 Inst< MFDouble    > _vKnot,
                                 Inst< SFInt32     > _uOrder,
                                 Inst< SFInt32     > _vOrder ) :
  X3DParametricGeometryNode( _metadata, _bound, _displayList, _isTouched,
                             _force, _contactPoint, _contactNormal ),
  controlPoint( _controlPoint ),
  texCoord( _texCoord ),
  uTessellation( _uTessellation ),
  vTessellation( _vTessellation ),
  weight( _weight ),
  solid( _solid ),
  uClosed( _uClosed ),
  vClosed( _vClosed ),
  uDimension( _uDimension ),
  vDimension( _vDimension ),
  uKnot( _uKnot ),
  vKnot( _vKnot ),
  uOrder( _uOrder ),
  vOrder( _vOrder ) {

  type_name = "X3DNurbsSurfaceGeometryNode";
  database.initFields( this );
  
  uTessellation->setValue( 0 );
  vTessellation->setValue( 0 );
  solid->setValue( true );
  uDimension->setValue( 0 );
  vDimension->setValue( 0 );
  uClosed->setValue( false );
  vClosed->setValue( false );
  uOrder->setValue( 3 );
  vOrder->setValue( 3 );

  controlPoint->route( bound );

  controlPoint->route( displayList );
  texCoord->route( displayList );
  uTessellation->route( displayList );
  vTessellation->route( displayList );
  weight->route( displayList );
  solid->route( displayList );
  uClosed->route( displayList );
  vClosed->route( displayList );
  uDimension->route( displayList );
  vDimension->route( displayList );
  uKnot->route( displayList );
  vKnot->route( displayList );
  uOrder->route( displayList );
  vOrder->route( displayList );
}
