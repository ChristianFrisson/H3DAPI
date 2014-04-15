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
/// \file GraphicsOptions.cpp
/// \brief CPP file for GraphicsOptions.
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/GraphicsOptions.h>
#include <H3D/Scene.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase GraphicsOptions::database( "GraphicsOptions", 
             "GraphicsCachingOptions", 
             &(newInstance<GraphicsOptions>),
             typeid( GraphicsOptions ),
             &H3DOptionNode::database );

namespace GraphicsOptionsInternals {
  FIELDDB_ELEMENT( GraphicsOptions, useCaching, INPUT_OUTPUT );
  FIELDDB_ELEMENT( GraphicsOptions, cachingDelay, INPUT_OUTPUT );
  FIELDDB_ELEMENT( GraphicsOptions, cacheOnlyGeometries, INPUT_OUTPUT );
  FIELDDB_ELEMENT( GraphicsOptions, frustumCullingMode, INPUT_OUTPUT );
  FIELDDB_ELEMENT( GraphicsOptions, useDefaultShadows, INPUT_OUTPUT );
  FIELDDB_ELEMENT( GraphicsOptions, defaultShadowDarkness, INPUT_OUTPUT );
  FIELDDB_ELEMENT( GraphicsOptions, defaultShadowDepthOffset, INPUT_OUTPUT );
  FIELDDB_ELEMENT( GraphicsOptions, defaultShadowGeometryAlgorithm, INPUT_OUTPUT );
  FIELDDB_ELEMENT( GraphicsOptions, preferVertexBufferObject, INPUT_OUTPUT );
  FIELDDB_ELEMENT( GraphicsOptions, defaultShadowCaster, INPUT_OUTPUT );
}

GraphicsOptions::GraphicsOptions( 
                                 Inst< SFNode>  _metadata,
                                 Inst< SFBool  > _useCaching,
                                 Inst< SFInt32 > _cachingDelay,
                                 Inst< SFBool  > _cacheOnlyGeometryNodes,
                                 Inst< SFString > _frustumCullingMode,
                                 Inst< SFBool  > _useDefaultShadows,
                                 Inst< SFFloat > _defaultShadowDarkness,
                                 Inst< SFFloat > _defaultShadowDepthOffset,
                                 Inst< SFBool > _preferVertexBufferObject,
                                 Inst< SFString > _defaultShadowGeometryAlgorithm,
                                 Inst< SFShadowCaster > _defaultShadowCaster ) :
  H3DOptionNode( _metadata ),
  useCaching( _useCaching ),
  cachingDelay( _cachingDelay ),
  cacheOnlyGeometries( _cacheOnlyGeometryNodes ),
  frustumCullingMode( _frustumCullingMode ),
  useDefaultShadows( _useDefaultShadows ),
  defaultShadowDarkness( _defaultShadowDarkness ),
  defaultShadowDepthOffset( _defaultShadowDepthOffset ),
  preferVertexBufferObject( _preferVertexBufferObject ),
  defaultShadowGeometryAlgorithm( _defaultShadowGeometryAlgorithm ),
  defaultShadowCaster( _defaultShadowCaster ) {
  
  type_name = "GraphicsOptions";
  database.initFields( this );

  useCaching->route( updateOption );
  cachingDelay->route( updateOption );
  cacheOnlyGeometries->route( updateOption );
  frustumCullingMode->route( updateOption );
  useDefaultShadows->route( updateOption );
  defaultShadowDarkness->route( updateOption );
  defaultShadowDepthOffset->route( updateOption );
  preferVertexBufferObject->route( updateOption );
  defaultShadowGeometryAlgorithm->route( updateOption );
  defaultShadowCaster->route( updateOption );

  useCaching->setValue( true );
  cachingDelay->setValue( 5 );
  cacheOnlyGeometries->setValue( false );
  frustumCullingMode->addValidValue( "NO_CULLING" );
  frustumCullingMode->addValidValue( "GEOMETRY" );
  frustumCullingMode->addValidValue( "ALL" );
  frustumCullingMode->setValue( "NO_CULLING" );
  useDefaultShadows->setValue( true );
  defaultShadowDarkness->setValue( 0.4f );
  defaultShadowDepthOffset->setValue( 6 );
  preferVertexBufferObject->setValue( false );
  defaultShadowGeometryAlgorithm->addValidValue( "GEOMETRY_SHADER" );
  defaultShadowGeometryAlgorithm->addValidValue( "CPU" );
  defaultShadowGeometryAlgorithm->setValue( "GEOMETRY_SHADER" );

  if( !Scene::scenes.empty() ) {
    defaultShadowCaster->setValue( (*Scene::scenes.begin())->getDefaultShadowCaster() );
  }
}


