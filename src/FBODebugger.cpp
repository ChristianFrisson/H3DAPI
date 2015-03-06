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
/// \file FBODebugger.cpp
/// \brief CPP file for FBODebugger, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/FBODebugger.h>
#include <H3D/FrameBufferTextureGenerator.h>
#include <H3D/Appearance.h>
#include <H3D/Material.h>
#include <H3D/RenderProperties.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase FBODebugger::database( "FBODebugger", 
                                       &(newInstance<FBODebugger>), 
                                       typeid( FBODebugger ),
                                       &X3DChildNode::database );

namespace FBODebuggerInternals {
  FIELDDB_ELEMENT( FBODebugger, fbo, INPUT_OUTPUT );
  FIELDDB_ELEMENT( FBODebugger, buffer, INPUT_OUTPUT );
  FIELDDB_ELEMENT( FBODebugger, transparency, INPUT_OUTPUT );
  FIELDDB_ELEMENT( FBODebugger, saveToUrl, INPUT_OUTPUT );
  FIELDDB_ELEMENT( FBODebugger, saveSuccess, INPUT_OUTPUT );

  const string x3d_fbo = 
"<Group>   \n" 
"  <Shape> \n"
"    <Appearance DEF=\"APP\" > \n"
"      <Material DEF=\"MA\"/> \n"
"      <RenderProperties DEF=\"RP\" depthTestEnabled=\"FALSE\" blendEnabled=\"FALSE\" /> \n"
"    </Appearance> \n"
"    <FullscreenRectangle zValue=\"0.99\"/> \n"
"  </Shape> \n"
"</Group>\n";

}

FBODebugger::FBODebugger( Inst< SFNode            >  _metadata,
                          Inst< SFString          > _fbo,
                          Inst< SFString          > _buffer,
                          Inst< SFFloat           > _transparency,
                          Inst< UpdateSaveToURL   > _saveToUrl,
                          Inst< SFBool            > _saveSuccess) :
  X3DChildNode( _metadata ),
  fbo( _fbo ),
  buffer( _buffer ),
  render_target_texture( new RenderTargetTexture ),
  selected_texture(new SFTextreNode),
  transparency(_transparency),
  saveToUrl(_saveToUrl),
  saveSuccess(_saveSuccess),
  toggleBlend( new ToggleBlend ){
  type_name = "FBODebugger";
  database.initFields( this );

  fbo->addValidValue( "NONE" );
  fbo->setValue( "NONE" );

  buffer->addValidValue( "DEPTH" );
  buffer->addValidValue( "COLOR0" );
  buffer->addValidValue( "COLOR1" );
  buffer->setValue( "COLOR0" );

  transparency->setValue(0);

  saveSuccess->setValue ( false, id );

  current_fbo = "NONE";
  current_buffer = "COLOR0";

  texture_scene.reset( X3D::createX3DFromString( FBODebuggerInternals::x3d_fbo, &texture_scene_dn ) );
  Material *ma;
  texture_scene_dn.getNode( "MA", ma );
  if( ma ){
    transparency->route(ma->transparency);
  }
  toggleBlend->setName("toggleBlend");
  toggleBlend->setOwner(this);
  transparency->route(toggleBlend);

  
}



void FBODebugger::traverseSG( TraverseInfo &ti ) {

  toggleBlend->upToDate();

  // add the valid values for the fbo field
  if( FrameBufferTextureGenerator::fbo_nodes.size() != fbo->getValidValues().size() ) {
    fbo->clearValidValues();
    fbo->addValidValue( "NONE" );
    for( set< FrameBufferTextureGenerator * >::const_iterator i = FrameBufferTextureGenerator::fbo_nodes.begin(); 
      i != FrameBufferTextureGenerator::fbo_nodes.end(); i++ ) {
        FrameBufferTextureGenerator *fbo_node = *i;
        fbo->addValidValue( fbo_node->getName() );
    }
  }


  // find the FrameBufferTextureGenerator that matches the name in the fbo field
  FrameBufferTextureGenerator *selected_fbo_node = NULL;
  for( set< FrameBufferTextureGenerator * >::const_iterator i = FrameBufferTextureGenerator::fbo_nodes.begin(); 
    i != FrameBufferTextureGenerator::fbo_nodes.end(); i++ ) {
      FrameBufferTextureGenerator *fbo_node = *i;
      if( fbo_node->getName() == fbo->getValue() ) {
        selected_fbo_node = *i;
        break;
      }
  }

  if( selected_fbo_node ) {
    // choose the texture to be shown based on the buffer field.
    string selected_fbo_name = selected_fbo_node->getName();
    if( selected_fbo_name != current_fbo ||
      buffer->getValue() != current_buffer ) {
        Appearance *app;
        texture_scene_dn.getNode( "APP", app );
        if( app ) {
          const string &selected_buffer = buffer->getValue();
          if( selected_buffer == "DEPTH" ) {
            app->texture->setValue( selected_fbo_node->depthTexture->getValue() );
            selected_texture->setValue( selected_fbo_node->depthTexture->getValue() );
            if( !selected_fbo_node->generateDepthTexture->getValue() ) {
              Console(4) << "Warning (FBODebugger): Cannot show DEPTH texture as no depth texture is generated by \"" << selected_fbo_name << "\" fbo" << endl;
            }
          } else if( selected_buffer == "COLOR0" ) {
            if( selected_fbo_node->colorTextures->size() > 0 ) {
              app->texture->setValue( render_target_texture );
              render_target_texture->generator->setValue( selected_fbo_node );
              render_target_texture->index->setValue( 0 );
              selected_texture->setValue( render_target_texture );
            } else {
              Console(4) << "Warning (FBODebugger): Cannot show COLOR0 texture as no color textures is generated by \"" << selected_fbo_name << "\" fbo" << endl;
              app->texture->setValue( NULL );
              selected_texture->setValue( NULL );
            }
          } else if( selected_buffer == "COLOR1" ) {
            if( selected_fbo_node->colorTextures->size() > 1 ) {
              app->texture->setValue( render_target_texture.get() );
              render_target_texture->generator->setValue( selected_fbo_node );
              render_target_texture->index->setValue( 1 );
              selected_texture->setValue( render_target_texture );
            } else {
              Console(4) << "Warning (FBODebugger): Cannot show COLOR1 texture as no such color texture is generated by \"" << selected_fbo_name << "\" fbo" << endl;
              app->texture->setValue( NULL );
              selected_texture->setValue( NULL );
            }
          }
        }
    }
  }else{
    selected_texture->setValue( NULL );
  }

  // save the values of the currently selected fbo and buffer
  // replace saveToUrl and saveSuccess route to connect it with the currently
  // selected texture
  current_fbo = fbo->getValue();
  current_buffer = buffer->getValue();
  if( selected_texture->getValue() ) {
    saveToUrl->route(selected_texture->getValue()->saveToUrl);
    selected_texture->getValue()->saveSuccess->route(saveSuccess);
  }else{
    saveToUrl->unrouteAll();
  }
}

void FBODebugger::render() {
  if( fbo->getValue() != "NONE" ) {
    texture_scene->render();
  }
}


void FBODebugger::ToggleBlend::update(){
  FBODebugger* fd = static_cast<FBODebugger*>( getOwner() );
  // user selected transparency
  float _transparency = fd->transparency->getValue();
  RenderProperties* rp;
  fd->texture_scene_dn.getNode("RP", rp);
  if( rp ) {
    if( _transparency <= 0 ) {
      if( rp ) {
        rp->blendEnabled->setValue(false);
      }
    }else{
        rp->blendEnabled->setValue(true);
    }
  }
}