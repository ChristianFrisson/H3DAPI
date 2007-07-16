//////////////////////////////////////////////////////////////////////////////
//    Copyright 2004-2007, SenseGraphics AB
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
/// \file GraphicalRenderingOptions.cpp
/// \brief CPP file for GraphicalRenderingOptions.
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <GraphicsCachingOptions.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase GraphicsCachingOptions::database( "GraphicsCachingOptions", 
                                      &(newInstance<GraphicsCachingOptions>),
                                      typeid( GraphicsCachingOptions ),
                                      &H3DOptionNode::database );

namespace GraphicsCachingOptionsInternals {
  FIELDDB_ELEMENT( GraphicsCachingOptions, useCaching, INPUT_OUTPUT );
  FIELDDB_ELEMENT( GraphicsCachingOptions, cachingDelay, INPUT_OUTPUT );
  FIELDDB_ELEMENT( GraphicsCachingOptions, cacheOnlyGeometries, INPUT_OUTPUT );
}

GraphicsCachingOptions::GraphicsCachingOptions( 
                           Inst< SFNode>  _metadata,
                           Inst< SFBool  > _useCaching,
                           Inst< SFInt32 > _cachingDelay,
                           Inst< SFBool  > _cacheOnlyGeometryNodes ) :
  H3DOptionNode( _metadata ),
  useCaching( _useCaching ),
  cachingDelay( _cachingDelay ),
  cacheOnlyGeometries( _cacheOnlyGeometryNodes ) {
  
  type_name = "GraphicsCachingOptions";
  database.initFields( this );

  useCaching->setValue( true );
  cachingDelay->setValue( 5 );
  cacheOnlyGeometries->setValue( false );
}


