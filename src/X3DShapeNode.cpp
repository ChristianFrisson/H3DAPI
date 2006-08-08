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
/// \file X3DShapeNode.cpp
/// \brief CPP file for X3DShapeNode, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include "X3DShapeNode.h"
#include "X3DTextureNode.h"

using namespace H3D;

H3DNodeDatabase X3DShapeNode::database( 
        "X3DShapeNode", 
        NULL,
        typeid( X3DShapeNode ),
        &X3DChildNode::database 
        );

namespace X3DShapeNodeInternals {
  FIELDDB_ELEMENT( X3DShapeNode, appearance, INPUT_OUTPUT );
  FIELDDB_ELEMENT( X3DShapeNode, geometry, INPUT_OUTPUT );
  FIELDDB_ELEMENT( X3DShapeNode, hapticGeometry, INPUT_OUTPUT );
  FIELDDB_ELEMENT( X3DShapeNode, bboxCenter, INITIALIZE_ONLY );
  FIELDDB_ELEMENT( X3DShapeNode, bboxSize, INITIALIZE_ONLY );
}

X3DShapeNode::X3DShapeNode( 
                           Inst< SFAppearanceNode > _appearance,
                           Inst< SFGeometryNode   > _geometry,
                           Inst< SFHapticGeometry > _hapticGeometry,
                           Inst< SFNode           > _metadata,
                           Inst< SFBound          > _bound,
                           Inst< SFVec3f          > _bboxCenter,
                           Inst< SFVec3f          > _bboxSize ) :
  X3DChildNode( _metadata ),
  X3DBoundedObject( _bound, _bboxCenter, _bboxSize ),
  appearance( _appearance ),
  geometry  ( _geometry   ),
  hapticGeometry( _hapticGeometry ),
  use_geometry_bound( false ) {

  geometry->owner = this;

  type_name = "X3DShapeNode";
  database.initFields( this );

  displayList->setOwner( this );
  bound->setOwner( this );
  
  appearance->route( displayList );
  geometry->route( displayList );
}


void X3DShapeNode::render() {
  X3DChildNode::render();
  X3DAppearanceNode *a = appearance->getValue();
  X3DGeometryNode *g = geometry->getValue();
  
  GLboolean lighting_on;
  glGetBooleanv( GL_LIGHTING, &lighting_on );
  if ( a ) {
    a->preRender();
    a->displayList->callList();
  } else {
    glDisable( GL_LIGHTING );
  }
  if ( g ) g->displayList->callList();
  if( a ) a->postRender();
  else if( lighting_on )
    glEnable( GL_LIGHTING );
};

#ifdef USE_HAPTICS
void X3DShapeNode::traverseSG( TraverseInfo &ti ) {
  Node *a = appearance->getValue();
  Node *g = geometry->getValue();
  Node *hg = hapticGeometry->getValue();
  if ( a ) a->traverseSG( ti );
  if( hg ) ti.disableHaptics();
  if ( g ) g->traverseSG( ti );
  if( hg ) {
    ti.enableHaptics();
    hg->traverseSG( ti );
  }
  // the surface should only be available to the geometry of the shape node
  // so we remove it when the geometry has been rendered.
  ti.setCurrentSurface( NULL );
}
#endif