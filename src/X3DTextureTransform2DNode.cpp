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
/// \file X3DTextureTransform2DNode.cpp
/// \brief CPP file for X3DTextureTransform2DNode, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/X3DTextureTransform2DNode.h>

using namespace H3D;

H3DNodeDatabase X3DTextureTransform2DNode::database( 
        "X3DTextureTransform2DNode", 
        NULL,
        typeid( X3DTextureTransform2DNode ),
        &X3DTextureTransformNode::database 
        );

namespace X3DTextureTransform2DNodeInternals {
  FIELDDB_ELEMENT( X3DTextureTransform2DNode, center, INPUT_OUTPUT );
  FIELDDB_ELEMENT( X3DTextureTransform2DNode, rotation, INPUT_OUTPUT );
  FIELDDB_ELEMENT( X3DTextureTransform2DNode, scale, INPUT_OUTPUT );
  FIELDDB_ELEMENT( X3DTextureTransform2DNode, translation, INPUT_OUTPUT );
}

X3DTextureTransform2DNode::X3DTextureTransform2DNode( 
                                    Inst< DisplayList > _displayList,
                                    Inst< SFNode  >  _metadata,
                                    Inst< SFVec2f >  _center,
                                    Inst< SFFloat >  _rotation,
                                    Inst< SFVec2f >  _scale,
                                    Inst< SFVec2f >  _translation ) :
  X3DTextureTransformNode( _displayList, _metadata ),
  center     ( _center      ),
  rotation   ( _rotation    ),
  scale      ( _scale       ),
  translation( _translation ) {

  center->setValue( Vec2f( 0, 0 ) );
  rotation->setValue( 0 );
  scale->setValue( Vec2f( 1, 1 ) );
  translation->setValue( Vec2f( 0, 0 ) );
   
  type_name = "X3DTextureTransform2DNode";

  database.initFields( this );

  center->route( displayList );
  rotation->route( displayList );
  scale->route( displayList );
  translation->route( displayList );
}


