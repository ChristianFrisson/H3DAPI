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
/// \file HapticsRenderers.cpp
/// \brief CPP file for several subclasses of H3DHapticsRendererNode
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/HapticsRenderers.h>

#ifdef HAVE_OPENHAPTICS
#include <HAPI/OpenHapticsRenderer.h>
#endif

#ifdef HAVE_CHAI3D
#include <HAPI/Chai3DRenderer.h>
#endif

using namespace H3D;

H3DNodeDatabase OpenHapticsRenderer::database( 
                           "OpenHapticsRenderer", 
                           &(newInstance<OpenHapticsRenderer>), 
                           typeid( OpenHapticsRenderer ),
                           &H3DHapticsRendererNode::database );

H3DNodeDatabase GodObjectRenderer::database( 
                           "GodObjectRenderer", 
                           &(newInstance<GodObjectRenderer>), 
                           typeid( GodObjectRenderer ),
                           &H3DHapticsRendererNode::database );

H3DNodeDatabase RuspiniRenderer::database( 
                           "RuspiniRenderer", 
                           &(newInstance<RuspiniRenderer>), 
                           typeid( RuspiniRenderer ),
                           &H3DHapticsRendererNode::database );

#ifdef HAVE_CHAI3D
H3DNodeDatabase Chai3DRenderer::database( 
                           "Chai3DRenderer", 
                           &(newInstance<Chai3DRenderer>), 
                           typeid( Chai3DRenderer ),
                           &H3DHapticsRendererNode::database );

/// Returns a new instancs of HAPI::Chai3DRenderer
HAPI::HAPIHapticsRenderer *Chai3DRenderer::getNewHapticsRenderer() {
  return new HAPI::Chai3DRenderer;
}

#endif

namespace HapticsRendererInternals {
  FIELDDB_ELEMENT( RuspiniRenderer, proxyRadius, INPUT_OUTPUT );

  FIELDDB_ELEMENT( OpenHapticsRenderer, defaultShapeType, INPUT_OUTPUT );
  FIELDDB_ELEMENT( OpenHapticsRenderer, defaultAdaptiveViewport, INPUT_OUTPUT );
  FIELDDB_ELEMENT( OpenHapticsRenderer, defaultHapticCameraView, INPUT_OUTPUT );
}

RuspiniRenderer::RuspiniRenderer( Inst< ProxyRadius > _proxyRadius ) :
  proxyRadius( _proxyRadius ) {
  
  type_name = "RuspiniRenderer";
  database.initFields( this );
  
  proxyRadius->setValue( (H3DFloat) 0.0025 );
}
  
void RuspiniRenderer::ProxyRadius::onValueChange( const H3DFloat &v ) {
  RuspiniRenderer *ruspini_node = 
    static_cast< RuspiniRenderer * >( getOwner() );
  for( unsigned int i = 0; i < ruspini_node->renderers.size(); i++ ) {
    HAPI::RuspiniRenderer *r = 
      static_cast< HAPI::RuspiniRenderer * >(ruspini_node->getHapticsRenderer( i ) );
    r->setProxyRadius( v );
  }
}

void OpenHapticsRenderer::ShapeType::onValueChange( const string &v ) {
#ifdef HAVE_OPENHAPTICS
  OpenHapticsRenderer *oh_node = 
    static_cast< OpenHapticsRenderer * >( getOwner() );

  for( unsigned int i = 0; i < oh_node->renderers.size(); i++ ) {
    HAPI::OpenHapticsRenderer *r = 
      static_cast< HAPI::OpenHapticsRenderer * >(oh_node->getHapticsRenderer( i ) );
  
    if( v == "FEEDBACK_BUFFER" ) {
      r->setDefaultShapeType( HAPI::OpenHapticsRenderer::OpenHapticsOptions::FEEDBACK_BUFFER );
    } else if( v == "DEPTH_BUFFER" ) {
      r->setDefaultShapeType( HAPI::OpenHapticsRenderer::OpenHapticsOptions::DEPTH_BUFFER );    
    } else {
      Console(4) << "Warning: Invalid OpenHaptics shape type: "
                 << v 
                 << ". Must be \"FEEDBACK_BUFFER\" or \"DEPTH_BUFFER\" "
                 << "(in \"" << getName() << "\")" << endl;
      r->setDefaultShapeType( HAPI::OpenHapticsRenderer::OpenHapticsOptions::FEEDBACK_BUFFER );
    }
  }
#endif
}

void OpenHapticsRenderer::AdaptiveViewport::onValueChange( const bool &v ) {
#ifdef HAVE_OPENHAPTICS
  OpenHapticsRenderer *oh_node = 
    static_cast< OpenHapticsRenderer * >( getOwner() );
  for( unsigned int i = 0; i < oh_node->renderers.size(); i++ ) {
    HAPI::OpenHapticsRenderer *r = 
      static_cast< HAPI::OpenHapticsRenderer * >(oh_node->getHapticsRenderer( i ) );
    r->setDefaultAdaptiveViewport( v );
  }
#endif
}

void OpenHapticsRenderer::CameraView::onValueChange( const bool &v ) {
#ifdef HAVE_OPENHAPTICS
  OpenHapticsRenderer *oh_node = 
    static_cast< OpenHapticsRenderer * >( getOwner() );
  for( unsigned int i = 0; i < oh_node->renderers.size(); i++ ) {
    HAPI::OpenHapticsRenderer *r = 
      static_cast< HAPI::OpenHapticsRenderer * >(oh_node->getHapticsRenderer( i ) );
    r->setDefaultHapticCameraView( v );
  }
#endif
}

HAPI::HAPIHapticsRenderer *OpenHapticsRenderer::getNewHapticsRenderer() {
#ifdef HAVE_OPENHAPTICS
  return new HAPI::OpenHapticsRenderer;
#else
  return NULL;
#endif
}
