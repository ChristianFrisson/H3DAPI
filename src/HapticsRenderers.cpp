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
/// \file HapticsRenderers.cpp
/// \brief CPP file for several subclasses of H3DHapticsRendererNode
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/HapticsRenderers.h>
#include <H3D/DeviceInfo.h>

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

H3DNodeDatabase LayeredRenderer::database( 
                           "LayeredRenderer", 
                           &(newInstance<LayeredRenderer>), 
                           typeid( LayeredRenderer ),
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
  FIELDDB_ELEMENT( RuspiniRenderer, alwaysFollowSurface, INPUT_OUTPUT );

  FIELDDB_ELEMENT( OpenHapticsRenderer, defaultShapeType, INPUT_OUTPUT );
  FIELDDB_ELEMENT( OpenHapticsRenderer, defaultAdaptiveViewport, INPUT_OUTPUT );
  FIELDDB_ELEMENT( OpenHapticsRenderer, defaultHapticCameraView, INPUT_OUTPUT );

  FIELDDB_ELEMENT( LayeredRenderer, hapticsRenderer, INPUT_OUTPUT );
}

RuspiniRenderer::RuspiniRenderer( Inst< ProxyRadius > _proxyRadius,
                                  Inst< AlwaysFollowSurface > _alwaysFollowSurface ) :
  proxyRadius( _proxyRadius ),
  alwaysFollowSurface ( _alwaysFollowSurface ) {
  
  type_name = "RuspiniRenderer";
  database.initFields( this );
  
  proxyRadius->setValue( (H3DFloat) 0.0025 );
  alwaysFollowSurface->setValue ( false );
}

void RuspiniRenderer::ProxyRadius::onValueChange( const H3DFloat &v ) {
  RuspiniRenderer *ruspini_node = 
    static_cast< RuspiniRenderer * >( getOwner() );
  for( unsigned int i = 0; i < ruspini_node->renderers.size(); ++i ) {
    HAPI::RuspiniRenderer *r = 
      static_cast< HAPI::RuspiniRenderer * >(ruspini_node->getHapticsRenderer( i ) );
    r->setProxyRadius( v );
  }
}

void RuspiniRenderer::AlwaysFollowSurface::onValueChange( const bool &b ) {
  RuspiniRenderer *ruspini_node = 
    static_cast< RuspiniRenderer * >( getOwner() );
  for( unsigned int i = 0; i < ruspini_node->renderers.size(); ++i ) {
    HAPI::RuspiniRenderer *r = 
      static_cast< HAPI::RuspiniRenderer * >(ruspini_node->getHapticsRenderer( i ) );
    r->setAlwaysFollowSurface( b );
  }
}

void OpenHapticsRenderer::ShapeType::onValueChange( const string &v ) {
#ifdef HAVE_OPENHAPTICS
  OpenHapticsRenderer *oh_node = 
    static_cast< OpenHapticsRenderer * >( getOwner() );

  for( unsigned int i = 0; i < oh_node->renderers.size(); ++i ) {
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
  for( unsigned int i = 0; i < oh_node->renderers.size(); ++i ) {
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
  for( unsigned int i = 0; i < oh_node->renderers.size(); ++i ) {
    HAPI::OpenHapticsRenderer *r = 
      static_cast< HAPI::OpenHapticsRenderer * >(oh_node->getHapticsRenderer( i ) );
    r->setDefaultHapticCameraView( v );
  }
#endif
}

HAPI::HAPIHapticsRenderer *OpenHapticsRenderer::getNewHapticsRenderer() {
#ifdef HAVE_OPENHAPTICS
  const string &default_shape_type = defaultShapeType->getValue();
  // The fourth argument is false because we want to use OpenGL MODELVIEW
  // matrix for setting matrices in OpenHaptics.
  if( default_shape_type == "FEEDBACK_BUFFER" ) {
      return new HAPI::OpenHapticsRenderer(
        HAPI::OpenHapticsRenderer::OpenHapticsOptions::FEEDBACK_BUFFER,
        defaultAdaptiveViewport->getValue(),
        defaultHapticCameraView->getValue(),
        false );
    } else if( default_shape_type == "DEPTH_BUFFER" ) {
      return new HAPI::OpenHapticsRenderer(
        HAPI::OpenHapticsRenderer::OpenHapticsOptions::DEPTH_BUFFER,
        defaultAdaptiveViewport->getValue(),
        defaultHapticCameraView->getValue(),
        false );
    } else {
      Console(4) << "Warning: Invalid OpenHaptics shape type: "
                 << default_shape_type
                 << ". Must be \"FEEDBACK_BUFFER\" or \"DEPTH_BUFFER\" "
                 << "(in \"" << getName() << "\")" << endl;
      return new HAPI::OpenHapticsRenderer(
        HAPI::OpenHapticsRenderer::OpenHapticsOptions::FEEDBACK_BUFFER,
        defaultAdaptiveViewport->getValue(),
        defaultHapticCameraView->getValue(),
        false );
    }
#else
  return NULL;
#endif
}

LayeredRenderer::LayeredRenderer( Inst< MFHapticsRendererNode > _hapticsRenderer ) :
  hapticsRenderer( _hapticsRenderer ) {
  
  type_name = "LayeredRenderer";
  database.initFields( this );
}

/// Get the haptics renderer to use for a certain layer.
HAPI::HAPIHapticsRenderer * LayeredRenderer::getHapticsRenderer(
  unsigned int layer ) {
  if( renderers.size() < layer + 1 ) {
    renderers.resize( layer + 1, NULL );
  }
  if( !renderers[layer] ) {
    const NodeVector &haptics_renderers = hapticsRenderer->getValue();
    if( layer < haptics_renderers.size() &&
        haptics_renderers[layer] ) {
      renderers[layer] =
        static_cast< H3DHapticsRendererNode * >(haptics_renderers[layer])
        ->getHapticsRenderer(0);
    } else
      renderers[layer] = getNewHapticsRenderer();
  }

  return renderers[layer];
}

struct LayeredRendererCallbackSafeData {
public:
  LayeredRendererCallbackSafeData( int _index,
    HAPI::HAPIHapticsDevice * _hapi_device,
    HAPI::HAPIHapticsRenderer * _current_renderer ) :
    index( _index ),
    current_renderer( _current_renderer ),
    hapi_device( _hapi_device )
  {
  }

  int index;
  HAPI::HAPIHapticsDevice * hapi_device;
  HAPI::HAPIHapticsRenderer * current_renderer;
};

PeriodicThread::CallbackCode setHapticsRendererSafe( void *data ) {
  LayeredRendererCallbackSafeData * d =
    static_cast< LayeredRendererCallbackSafeData * >(data);
  if( d->hapi_device->getHapticsRenderer( d->index ) ==
      d->current_renderer )
    d->hapi_device->setHapticsRenderer( NULL, d->index );
  return PeriodicThread::CALLBACK_DONE;
}

void LayeredRenderer::MFHapticsRendererNode::onAdd( Node *n ) {
  TypedMFNode< H3DHapticsRendererNode >::onAdd( n );
  LayeredRenderer *layered_renderer =
    static_cast< LayeredRenderer * >( getOwner() );
  HAPI::HAPIHapticsRenderer *current_hapi_renderer =
    value.size() < layered_renderer->renderers.size() ?
      layered_renderer->renderers[value.size()] : NULL;
  layered_renderer->hapticsRendererRemovedInternal( value.size() );
  DeviceInfo * di = DeviceInfo::getActive();
  if( di ) {
    const NodeVector &devices = di->device->getValue();
    for( unsigned int i = 0; i < devices.size(); ++i ) {
      H3DHapticsDevice * h3d_device =
        static_cast< H3DHapticsDevice * >(devices[i]);
      LayeredRenderer *layered_renderer =
        static_cast< LayeredRenderer * >( getOwner() );
      if( h3d_device && h3d_device->hapticsRenderer->getValue() == layered_renderer ) {
        if( HAPI::HAPIHapticsDevice* hapi_device= h3d_device->getHAPIDevice() ) {
          if ( PeriodicThreadBase* ht= hapi_device->getThread() ) {
            // Node is not added yet, but it will be, therefore use value.size() as index
            // not value.size() - 1
            // We have to remove the current layer from the HAPI device since it might
            // be so that a user specifies less renderers than there are layers and
            // there might be a default GodObjectRenderer for the layer for which
            // this added node should be used.
            LayeredRendererCallbackSafeData data( value.size(), hapi_device, current_hapi_renderer );
            ht->synchronousCallback ( &setHapticsRendererSafe, &data );
          }
        }
      }
    }
  }
}

void LayeredRenderer::MFHapticsRendererNode::onRemove( Node *n ) {
  LayeredRenderer *layered_renderer =
    static_cast< LayeredRenderer * >( getOwner() );
  vector< H3DHapticsDevice * > renderer_in_these_devices;
  DeviceInfo * di = DeviceInfo::getActive();
  if( di ) {
    const NodeVector &devices = di->device->getValue();
    for( unsigned int i = 0; i < devices.size(); ++i ) {
      H3DHapticsDevice * h3d_device =
        static_cast< H3DHapticsDevice * >(devices[i]);
      LayeredRenderer *layered_renderer =
        static_cast< LayeredRenderer * >( getOwner() );
      if( h3d_device && h3d_device->hapticsRenderer->getValue() == layered_renderer ) {
        renderer_in_these_devices.push_back( h3d_device );
      }
    }
  }
  for( unsigned int i = 0; i < size(); ++i ) {
    if( value[i] == n ) {
      layered_renderer->hapticsRendererRemovedInternal( i, true );
      HAPI::HAPIHapticsRenderer *current_hapi_renderer =
        i < layered_renderer->renderers.size() ?
          layered_renderer->renderers[i] : NULL;
      static_cast< H3DHapticsRendererNode *>(n)->hapticsRendererRemoved(0);
      for( unsigned int j = 0; j < renderer_in_these_devices.size(); ++j ) {
        if( HAPI::HAPIHapticsDevice* hapi_device =
            renderer_in_these_devices[j]->getHAPIDevice() ) {
          if ( PeriodicThreadBase* ht= hapi_device->getThread() ) {
            LayeredRendererCallbackSafeData data( i, hapi_device, current_hapi_renderer );
            ht->synchronousCallback ( &setHapticsRendererSafe, &data );
          }
        }
      }
    }
  }
  TypedMFNode< H3DHapticsRendererNode >::onRemove( n );
}

