//////////////////////////////////////////////////////////////////////////////
//    Copyright 2004-2014, SenseGraphics AB
//
//    Any use, or distribution, of this file without permission from the
//    copyright holders is strictly prohibited. Please contact SenseGraphics,
//    www.sensegraphics.com, for more information.
//
//
/// \file RenderTargetTexture.cpp
/// \brief CPP file for RenderTargetTexture.
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/RenderTargetTexture.h>

using namespace H3D;

H3DNodeDatabase RenderTargetTexture::database( 
        "RenderTargetTexture", 
        &newInstance< RenderTargetTexture >,
        typeid( RenderTargetTexture ),
        &H3DSingleTextureNode::database 
        );

namespace RenderTargetTextureInternals {
  FIELDDB_ELEMENT( RenderTargetTexture, generator, INPUT_OUTPUT );
  FIELDDB_ELEMENT( RenderTargetTexture, index, INPUT_OUTPUT );
}

/// Constructor.
RenderTargetTexture::RenderTargetTexture( Inst< DisplayList          > _displayList,
                                          Inst< SFNode               > _metadata,
                                          Inst< SFFrameBufferTextureGenerator > _generator,
                                          Inst< SFInt32              > _index ):
  H3DSingleTextureNode( _displayList, _metadata ),
  generator( _generator ),
  index( _index ) {

  type_name = "RenderTargetTexture";
  database.initFields( this );

  index->setValue( 0 );

  displayList->setCacheMode( H3DDisplayListObject::DisplayList::OFF );

}

void RenderTargetTexture::preRender() {
  H3DSingleTextureNode::preRender();
  FrameBufferTextureGenerator *gen = generator->getValue();
  H3DInt32 target_index = index->getValue();
  if( gen && target_index < (H3DInt32)gen->colorTextures->size() ) {
    gen->colorTextures->getValueByIndex( target_index )->preRender();
  }
}

void RenderTargetTexture::postRender() {
  FrameBufferTextureGenerator *gen = generator->getValue();
  H3DInt32 target_index = index->getValue();
  if( gen && target_index < (H3DInt32)gen->colorTextures->size() ) {
    gen->colorTextures->getValueByIndex( target_index )->postRender();
  }
  H3DSingleTextureNode::postRender();  
}

void RenderTargetTexture::render() {
  FrameBufferTextureGenerator *gen = generator->getValue();
  H3DInt32 target_index = index->getValue();
  if( gen && target_index < (H3DInt32)gen->colorTextures->size() ) {
    gen->colorTextures->getValueByIndex( target_index )->render();
  }
}

void RenderTargetTexture::enableTexturing() {
  FrameBufferTextureGenerator *gen = generator->getValue();
  H3DInt32 target_index = index->getValue();
  if( gen && target_index < (H3DInt32)gen->colorTextures->size() ) {
    gen->colorTextures->getValueByIndex( target_index )->enableTexturing();
  }
}

void RenderTargetTexture::disableTexturing() {
  FrameBufferTextureGenerator *gen = generator->getValue();
  H3DInt32 target_index = index->getValue();
  if( gen && target_index < (H3DInt32)gen->colorTextures->size() ) {
    gen->colorTextures->getValueByIndex( target_index )->disableTexturing();
  }
}

GLuint RenderTargetTexture::getTextureId() {
  FrameBufferTextureGenerator *gen = generator->getValue();
  H3DInt32 target_index = index->getValue();
  if( gen && target_index < (H3DInt32)gen->colorTextures->size() ) {
    return gen->colorTextures->getValueByIndex( target_index )->getTextureId();
  }
  return 0;
}

GLuint RenderTargetTexture::getTextureUnit() {
  FrameBufferTextureGenerator *gen = generator->getValue();
  H3DInt32 target_index = index->getValue();
  if( gen && target_index < (H3DInt32)gen->colorTextures->size() ) {
    return gen->colorTextures->getValueByIndex( target_index )->getTextureUnit();
  }
  return 0;
}

GLenum RenderTargetTexture::getTextureTarget() {
  FrameBufferTextureGenerator *gen = generator->getValue();
  H3DInt32 target_index = index->getValue();
  if( gen && target_index < (H3DInt32)gen->colorTextures->size() ) {
    return gen->colorTextures->getValueByIndex( target_index )->getTextureTarget();
  }
  return GL_TEXTURE_2D;
}

bool RenderTargetTexture::makeResident () {
  FrameBufferTextureGenerator *gen = generator->getValue();
  H3DInt32 target_index = index->getValue();
  if( gen && target_index < (H3DInt32)gen->colorTextures->size() ) {
    return gen->colorTextures->getValueByIndex( target_index )->makeResident ();
  }
  return false;
}

void RenderTargetTexture::makeNonResident () {
  FrameBufferTextureGenerator *gen = generator->getValue();
  H3DInt32 target_index = index->getValue();
  if( gen && target_index < (H3DInt32)gen->colorTextures->size() ) {
    return gen->colorTextures->getValueByIndex( target_index )->makeNonResident ();
  }
}

bool RenderTargetTexture::isResident() {
  FrameBufferTextureGenerator *gen = generator->getValue();
  H3DInt32 target_index = index->getValue();
  if( gen && target_index < (H3DInt32)gen->colorTextures->size() ) {
    return gen->colorTextures->getValueByIndex( target_index )->isResident();
  }
  return false;
}

GLuint64 RenderTargetTexture::getTextureHandle() {
  FrameBufferTextureGenerator *gen = generator->getValue();
  H3DInt32 target_index = index->getValue();
  if( gen && target_index < (H3DInt32)gen->colorTextures->size() ) {
    return gen->colorTextures->getValueByIndex( target_index )->getTextureHandle();
  }
  return 0;
}

void RenderTargetTexture::invalidateTextureHandle () {
  FrameBufferTextureGenerator *gen = generator->getValue();
  H3DInt32 target_index = index->getValue();
  if( gen && target_index < (H3DInt32)gen->colorTextures->size() ) {
    gen->colorTextures->getValueByIndex( target_index )->invalidateTextureHandle();
  }
}

void RenderTargetTexture::inUse () {
  FrameBufferTextureGenerator *gen = generator->getValue();
  H3DInt32 target_index = index->getValue();
  if( gen && target_index < (H3DInt32)gen->colorTextures->size() ) {
    gen->colorTextures->getValueByIndex( target_index )->inUse();
  }
}

std::pair<H3DInt32,H3DInt32> H3D::RenderTargetTexture::getDefaultSaveDimensions()
{
  FrameBufferTextureGenerator *gen = generator->getValue();
  H3DInt32 target_index = index->getValue();
  if( gen && target_index < (H3DInt32)gen->colorTextures->size() ){
    GeneratedTexture * t = static_cast< GeneratedTexture* >( 
      gen->colorTextures->getValueByIndex( target_index ) );
    if( t->textureIdIsInitialized() ) {
      GLuint tex_id = t->getTextureId();
      glPushAttrib( GL_TEXTURE_BIT );
      glBindTexture( getTextureTarget(), tex_id );
      GLint h, w;
      glGetTexLevelParameteriv( getTextureTarget(), 0, GL_TEXTURE_WIDTH, &w );
      glGetTexLevelParameteriv( getTextureTarget(), 0, GL_TEXTURE_HEIGHT, &h);
      glPopAttrib();
      return std::pair<H3DInt32,H3DInt32> ( w, h );
    }
  }
  return H3DSingleTextureNode::getDefaultSaveDimensions();
}
