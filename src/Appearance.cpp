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
/// \file Appearance.cpp
/// \brief CPP file for Appearance, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/Appearance.h>
#include "GL/glew.h"
#include <H3D/X3DTextureTransformNode.h>
#include <H3D/X3DTextureNode.h>
#include <H3D/X3DMaterialNode.h>
#include <H3D/LineProperties.h>
#include <H3D/MultiTexture.h>
#include <H3D/X3DShapeNode.h>
#include <H3D/X3DTexture2DNode.h>
#include <H3D/X3DTexture3DNode.h>
#include <H3D/NavigationInfo.h>
#include <H3DUtil/Console.h>

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
  FIELDDB_ELEMENT( Appearance, renderProperties, INPUT_OUTPUT );
  FIELDDB_ELEMENT( Appearance, shadow, INPUT_OUTPUT );
}

Appearance::Appearance( Inst< DisplayList            > _displayList,
                        Inst< SFFillProperties       > _fillProperties,
                        Inst< SFLineProperties       > _lineProperties,
                        Inst< SFMaterialNode         > _material,
                        Inst< SFNode                 > _metadata,
                        Inst< SFTextureNode          > _texture,
                        Inst< SFTextureTransformNode > _textureTransform,
                        Inst< SFSurface              > _surface,
                        Inst< MFShaderNode           > _shaders,
                        Inst< SFRenderProperties     > _renderProperties,
                        Inst< SFBool                 > _shadow ) :
X3DAppearanceNode( _displayList, _metadata, _surface ),
fillProperties  ( _fillProperties   ),
lineProperties  ( _lineProperties   ),
material        ( _material         ),
texture         ( _texture          ),
textureTransform( _textureTransform ),
shaders          ( _shaders           ),
renderProperties( _renderProperties ),
shadow          ( _shadow )  {
  
  type_name = "Appearance";
  
  database.initFields( this );
  
  shadow->setValue( false );

  fillProperties->route( displayList );
  lineProperties->route( displayList );
  material->route( displayList );
  texture->route( displayList );
  textureTransform->route( displayList );
  shaders->route( displayList );
  renderProperties->route( displayList );
  shadow->route( displayList );

}

void Appearance::render()     {
  X3DAppearanceNode::render();     
  
  X3DMaterialNode *m = material->getValue();
  if ( m ) m->displayList->callList();
  else glColor4f( 1, 1, 1, 1 );

  /// If an RGB, BGR, RGBA or BGRA texture the texture values should not be
  /// modulated with diffuseColor according to the X3D spec. So we set the 
  /// diffuse color to 1 1 1 in order to show the texture values as they are.
  /// The alpha value should be the one from material if 3 component texture
  /// and the one from the texture if 4-component texture.
  X3DTextureNode *texture_pt = X3DTextureNode::getActiveTexture();
  X3DTexture2DNode *texture2d = dynamic_cast< X3DTexture2DNode * >( texture_pt );
  X3DTexture3DNode *texture3d = dynamic_cast< X3DTexture3DNode * >( texture_pt );

  Image *image = NULL;
  if( texture2d ) {
    image = texture2d->image->getValue();
  } else if( texture3d ) {
    image = texture3d->image->getValue();
  }

  // in order to comply with X3D spec we need to set the material
  // diffuse color to 1 1 1 when RGB or RGBA texture in order to 
  // modulate it.
  if( image && shaders->empty() ) {
    Image::PixelType pixel_type = image->pixelType();
    if( pixel_type == Image::RGB ||
        pixel_type == Image::BGR ) {
      GLfloat _gl_material[4];
      glGetMaterialfv( GL_BACK, GL_DIFFUSE, _gl_material);

      _gl_material[0] = 1;
      _gl_material[1] = 1;
      _gl_material[2] = 1;

      glMaterialfv( GL_BACK, GL_DIFFUSE, _gl_material);

      glGetMaterialfv( GL_FRONT, GL_DIFFUSE, _gl_material);
      
      _gl_material[0] = 1;
      _gl_material[1] = 1;
      _gl_material[2] = 1;
      
      glMaterialfv( GL_FRONT, GL_DIFFUSE, _gl_material);
    } else if( pixel_type == Image::RGBA ||
               pixel_type == Image::BGRA ) {
      GLfloat _gl_material[4];
      _gl_material[0] = 1;
      _gl_material[1] = 1;
      _gl_material[2] = 1;
      _gl_material[3] = 1;
      glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, _gl_material );
    }
  }

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
    if( mt && mt->texture->size() > 0 ) 
      tt->renderForTextureUnits( 0, mt->texture->size() - 1 );
    else
      tt->renderForTextureUnit( 0 );
  } else {
    GLint saved_mode;
    glGetIntegerv( GL_MATRIX_MODE, &saved_mode );
    glMatrixMode( GL_TEXTURE );
    glLoadIdentity();
    glMatrixMode( saved_mode );
  }

  for( MFShaderNode::const_iterator i = shaders->begin();
       i != shaders->end();
       ++i ) {
    X3DShaderNode *s = static_cast< X3DShaderNode * >( *i );
    if ( s && s->isSelected->getValue() ) {
      s->displayList->callList();
      break;
    }
  }

  RenderProperties *rp = renderProperties->getValue();
  if ( rp ) rp->displayList->callList();

};

GLbitfield Appearance::getAffectedGLAttribs() {
  GLbitfield res = X3DAppearanceNode::getAffectedGLAttribs();     
  
  X3DMaterialNode *m = material->getValue();
  if ( m ) res = res | m->getAffectedGLAttribs();
  else {
    if( X3DShapeNode::disable_lighting_if_no_app ) {
      res = res | GL_LIGHTING_BIT;
    }
  }
  
  X3DTextureNode *t = texture->getValue();
  if ( t ) res = res | t->getAffectedGLAttribs();
  
  LineProperties *lp = lineProperties->getValue();
  if ( lp ) res = res | lp->getAffectedGLAttribs();
  
  FillProperties *fp = fillProperties->getValue();
  if ( fp ) res = res | fp->getAffectedGLAttribs();
  
  X3DTextureTransformNode *tt = textureTransform->getValue();
  if ( tt ) res = res | tt->getAffectedGLAttribs();
  
  for( MFShaderNode::const_iterator i = shaders->begin();
       i != shaders->end();
       ++i ) {
    X3DShaderNode *s = static_cast< X3DShaderNode * >( *i );
    if ( s ) {
      if( s->isSupported() ) {
        res = res | s->getAffectedGLAttribs();
        break;
      } 
    }
  }
  RenderProperties *rp = renderProperties->getValue();
  if ( rp ) res = res | rp->getAffectedGLAttribs();
  return res;
}

/// This function will be called by the X3DShapeNode before any rendering 
/// of geometry in order to set up all OpenGL states that are needed to 
/// for the appearance of the geometry.
void Appearance::preRender() {
  X3DAppearanceNode::preRender();     
  
  X3DMaterialNode *m = material->getValue();
  if ( m ) m->preRender();
  else {
    if( X3DShapeNode::disable_lighting_if_no_app ) {
      glDisable( GL_LIGHTING );
    }
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
       ++i ) {
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
  RenderProperties *rp = renderProperties->getValue();
  if ( rp ) rp->preRender();
}

/// This function will be called by the X3DShapeNode after the geometry
/// has been rendered to restore the states to what it was before 
/// the call to preRender().
void Appearance::postRender() {
  RenderProperties *rp = renderProperties->getValue();
  if ( rp ) rp->postRender();

  for( MFShaderNode::const_iterator i = shaders->begin();
       i != shaders->end();
       ++i ) {
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

  X3DAppearanceNode::postRender();     
}

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
       ++i ) {
   X3DShaderNode *sn = static_cast< X3DShaderNode * >( *i );
   if ( sn ) sn->traverseSG( ti );
  }
  RenderProperties *rp = renderProperties->getValue();
  if ( rp ) rp->traverseSG( ti );

  // print warning if too many light sources and fixed pipeline rendering
  GLint max_lights = 1;
  glGetIntegerv( GL_MAX_LIGHTS, &max_lights );

  NavigationInfo *ni = NavigationInfo::getActive();
  unsigned int nr_lights = ti.getActiveLightNodes().size();
  if( !ni || ni->headlight->getValue() ) {
    ++nr_lights;
  }
  if( shaders->size() == 0 && (GLint)nr_lights > max_lights )
      Console(4) << "Warning: Maximum number of lightsources (" << max_lights
                 << ") exceeded. Some lights will be ignored." << endl;
}


bool Appearance::isTransparent() {
  // if we have shaders, the shaders 
  X3DMaterialNode *m = material->getValue();
  if( !shaders->empty() ) {
    for( MFShaderNode::const_iterator i = shaders->begin(); i != shaders->end(); ++i ) {
      X3DShaderNode *shader = static_cast< X3DShaderNode * >( *i );
      if( shader->isTransparent(m) ) return true;
    }
    return false; 
  }

  if ( m )
    return m->isTransparent();
  else
    return false;
  
}
