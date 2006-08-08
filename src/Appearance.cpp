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
/// \file Appearance.cpp
/// \brief CPP file for Appearance, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include "Appearance.h"
#include "GL/glew.h"
#include "X3DTextureTransformNode.h"
#include "X3DTextureNode.h"
#include "X3DMaterialNode.h"
#include "LineProperties.h"
#include "MultiTexture.h"
#include "Console.h"

using namespace H3D;


// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase Appearance::database( "Appearance", 
                                      &(newInstance<Appearance>),
                                      typeid( Appearance ),
                                      &X3DAppearanceNode::database );

namespace AppearanceInternals {
  FIELDDB_ELEMENT( Appearance, fillProperties, INPUT_OUTPUT );
  FIELDDB_ELEMENT( Appearance, lineProperties, INPUT_OUTPUT );
  FIELDDB_ELEMENT( Appearance, material, INPUT_OUTPUT );
  FIELDDB_ELEMENT( Appearance, texture, INPUT_OUTPUT );
  FIELDDB_ELEMENT( Appearance, textureTransform, INPUT_OUTPUT );
  FIELDDB_ELEMENT( Appearance, shaders, INPUT_OUTPUT );
}


Appearance::Appearance( Inst< DisplayList            > _displayList,
                        Inst< SFFillProperties       > _fillProperties,
                        Inst< SFLineProperties       > _lineProperties,
                        Inst< SFMaterialNode         > _material,
                        Inst< SFNode                 > _metadata,
                        Inst< SFTextureNode          > _texture,
                        Inst< SFTextureTransformNode > _textureTransform
#ifdef USE_HAPTICS
												, Inst< SFSurface              > _surface
#endif
												, Inst< MFShaderNode           > _shaders ) :
X3DAppearanceNode( _displayList, _metadata
#ifdef USE_HAPTICS
									, _surface
#endif
									),
fillProperties  ( _fillProperties   ),
lineProperties  ( _lineProperties   ),
material        ( _material         ),
texture         ( _texture          ),
textureTransform( _textureTransform ),
shaders          ( _shaders           ) {
  
  type_name = "Appearance";
  
  database.initFields( this );
  
  fillProperties->route( displayList );
  lineProperties->route( displayList );
  material->route( displayList );
  texture->route( displayList );
  textureTransform->route( displayList );
  shaders->route( displayList );
}

void Appearance::render()     {
  X3DAppearanceNode::render();     
  
  X3DMaterialNode *m = material->getValue();
  if ( m ) m->displayList->callList();

  X3DTextureNode *t = texture->getValue();
  if ( t ) t->displayList->callList();

  LineProperties *lp = lineProperties->getValue();
  if ( lp ) lp->displayList->callList();

  FillProperties *fp = fillProperties->getValue();
  if ( fp ) fp->displayList->callList();

  // the render() function of the texture transform
  // must be called after the texture has been rendered
  // since the TextureTransform render function performs
  // operations depending on if the texture is a MultiTexture
  // or not.
  X3DTextureTransformNode *tt = textureTransform->getValue();
  if ( tt ) {
    MultiTexture *mt = dynamic_cast< MultiTexture * >( t );
    if( mt ) 
      tt->renderForTextureUnits( 0, mt->texture->size() - 1 );
    else
      tt->render();
  } else {
    GLint saved_mode;
    glGetIntegerv( GL_MATRIX_MODE, &saved_mode );
    glMatrixMode( GL_TEXTURE );
    glLoadIdentity();
    glMatrixMode( saved_mode );
  }

  for( MFShaderNode::const_iterator i = shaders->begin();
       i != shaders->end();
       i++ ) {
    X3DShaderNode *s = static_cast< X3DShaderNode * >( *i );
    if ( s && s->isSelected->getValue() ) {
      s->displayList->callList();
      break;
    }
  }
};


/// This function will be called by the X3DShapeNode before any rendering 
/// of geometry in order to set up all OpenGL states that are needed to 
/// for the appearance of the geometry.
void Appearance::preRender() {
  X3DAppearanceNode::preRender();     
  
  X3DMaterialNode *m = material->getValue();
  if ( m ) m->preRender();
  else {
    glPushAttrib( GL_LIGHTING_BIT );
    glDisable( GL_LIGHTING );
  }

  X3DTextureNode *t = texture->getValue();
  if ( t ) t->preRender();

  LineProperties *lp = lineProperties->getValue();
  if ( lp ) lp->preRender();

  FillProperties *fp = fillProperties->getValue();
  if ( fp ) fp->preRender();

  X3DTextureTransformNode *tt = textureTransform->getValue();
  if ( tt ) tt->preRender();

  for( MFShaderNode::const_iterator i = shaders->begin();
       i != shaders->end();
       i++ ) {
   X3DShaderNode *s = static_cast< X3DShaderNode * >( *i );
   if ( s ) {
     if( s->isSupported() ) {
       s->setSelected( true );
       s->preRender();
       break;
     } else {
       Console(3) << "Warning: Shader node \"" << s->getName() 
                  << "\" does not support the \"" << s->language->getValue() 
                  << "\" language. Shader will be ignored." << endl;
       s->setSelected( false );
     }
   }
 }
}

/// This function will be called by the X3DShapeNode after the geometry
/// has been rendered to restore the states to what it was before 
/// the call to preRender().
void Appearance::postRender() {
  for( MFShaderNode::const_iterator i = shaders->begin();
       i != shaders->end();
       i++ ) {
    X3DShaderNode *s = static_cast< X3DShaderNode * >( *i );
    if ( s && s->isSelected->getValue() ) {
      s->postRender();
      break;
    }
  }

  X3DTextureTransformNode *tt = textureTransform->getValue();
  if ( tt ) tt->postRender();

  FillProperties *fp = fillProperties->getValue();
  if ( fp ) fp->postRender();

  LineProperties *lp = lineProperties->getValue();
  if ( lp ) lp->postRender();

  X3DTextureNode *t = texture->getValue();
  if ( t ) t->postRender();

  X3DMaterialNode *m = material->getValue();
  if ( m ) m->postRender();
  else glPopAttrib();

  X3DAppearanceNode::postRender();     
}

#ifdef USE_HAPTICS
void Appearance::traverseSG( TraverseInfo &ti ) {
  X3DAppearanceNode::traverseSG( ti );     
  
  X3DMaterialNode *m = material->getValue();
  if ( m ) m->traverseSG( ti );
  X3DTextureNode *t = texture->getValue();
  if ( t ) t->traverseSG( ti );
  LineProperties *lp = lineProperties->getValue();
  if ( lp ) lp->traverseSG( ti );
  FillProperties *fp = fillProperties->getValue();
  if ( fp ) fp->traverseSG( ti );
  X3DTextureTransformNode *tt = textureTransform->getValue();
  if ( tt ) tt->traverseSG( ti );
  H3DSurfaceNode *s = surface->getValue();
  if( s ) s->traverseSG( ti );
  
  for( MFShaderNode::const_iterator i = shaders->begin();
       i != shaders->end();
       i++ ) {
   X3DShaderNode *sn = static_cast< X3DShaderNode * >( *i );
   if ( sn ) sn->traverseSG( ti );
  }
}
#endif
