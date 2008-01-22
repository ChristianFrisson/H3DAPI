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
/// \file DepthMapSurface.cpp
/// \brief cpp file for DepthMapSurface
///
//
//////////////////////////////////////////////////////////////////////////////
#include "H3D/DepthMapSurface.h"
#include <HAPI/DepthMapSurface.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase DepthMapSurface::database( 
                                            "DepthMapSurface", 
                                            &(newInstance<DepthMapSurface>),
                                            typeid( DepthMapSurface ),
                                            &H3DVariableDepthSurface::database
                                            );

namespace DepthMapSurfaceInternals {
  FIELDDB_ELEMENT( DepthMapSurface, depthMap, INPUT_OUTPUT );
  FIELDDB_ELEMENT( DepthMapSurface, maxDepth, INPUT_OUTPUT );
  FIELDDB_ELEMENT( DepthMapSurface, whiteIsOut, INPUT_OUTPUT );  
}

DepthMapSurface::DepthMapSurface(
                    Inst< UpdateStiffness       > _stiffness,
                    Inst< UpdateDamping         > _damping,
									  Inst< UpdateStaticFriction  > _staticFriction,
									  Inst< UpdateDynamicFriction > _dynamicFriction,
									  Inst< SFTexture2DNode       > _depthMap,
									  Inst< SFFloat               > _maxDepth,
                    Inst< SFBool                > _whiteIsOut ):
  H3DVariableDepthSurface( _stiffness, _damping,
                           _staticFriction, _dynamicFriction ),
  depthMap( _depthMap ),
  maxDepth( _maxDepth ),
  whiteIsOut( _whiteIsOut ),
  setImagePtr( new SetImagePtr ){
  
  type_name = "DepthMapSurface";
  database.initFields( this );
  maxDepth->setValue( 0.001f );
  whiteIsOut->setValue( true );

  setImagePtr->setOwner( this );
}
 void DepthMapSurface::initialize() {
   H3DVariableDepthSurface::initialize();
   X3DTexture2DNode * height_map = 
     static_cast< X3DTexture2DNode * >( depthMap->getValue() );
   Image * temp_image = 0;
   if( height_map )
     temp_image = height_map->image->getValue();

   hapi_surface.reset( new HAPI::DepthMapSurface( 
                          mmStiffness->getValue() ?
                            stiffness->getValue() :
                            stiffness->getValue() * 0.001,
                          damping->getValue()* 0.001,
                          staticFriction->getValue(),
                          dynamicFriction->getValue(),
                          temp_image,
                          maxDepth->getValue() * 1000,
                          whiteIsOut->getValue() ) );

}

 void DepthMapSurface::SetImagePtr::update() {
   Image *image =
     static_cast< X3DTexture2DNode::SFImage * >(routes_in[0])->getValue();
   DepthMapSurface *hms = 
     static_cast< DepthMapSurface * >( getOwner() );
   HAPI::DepthMapSurface * hapi_surface = 
     static_cast< HAPI::DepthMapSurface * >( hms->hapi_surface.get() );
   if( hapi_surface )
     hapi_surface->setImage( image );
 }

 void DepthMapSurface::SFTexture2DNode::onAdd( Node *n ) {
   SFTexture2DNodeBase::onAdd( n );
   X3DTexture2DNode *c = static_cast< X3DTexture2DNode* >( n );
   DepthMapSurface *o = static_cast< DepthMapSurface* >( owner );
   if( c ) {
     c->image->route( o->setImagePtr );
   }
 }

 void DepthMapSurface::SFTexture2DNode::onRemove( Node *n ) {
   X3DTexture2DNode *c = static_cast< X3DTexture2DNode* >( n );
   DepthMapSurface *o = static_cast< DepthMapSurface* >( owner );
   if( c ) {
     c->image->unroute( o->setImagePtr );
     HAPI::DepthMapSurface * hapi_surface = 
       static_cast< HAPI::DepthMapSurface * >( o->hapi_surface.get() );
     if( hapi_surface )
       hapi_surface->setImage( 0 );
   }
   SFTexture2DNodeBase::onRemove( n );
 }
