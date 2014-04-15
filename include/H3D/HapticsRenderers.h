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
/// \file HapticsRenderers.h
/// \brief Header file for several subclasses of H3DHapticsRendererNode
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __HAPTICSRENDERERS_H__
#define __HAPTICSRENDERERS_H__

// H3DApi includes
#include <H3D/H3DHapticsRendererNode.h>
#include <H3D/SFFloat.h>
#include <H3D/SFString.h>
#include <H3D/SFBool.h>
#include <H3D/FieldTemplates.h>
#include <H3D/MFNode.h>

// HAPI includes
#include <HAPI/RuspiniRenderer.h>
#include <HAPI/GodObjectRenderer.h>

namespace H3D {

  /// \ingroup H3DNodes
  /// \brief Haptics renderer using OpenHaptics to render objects.
  ///
  /// For example usage use H3DViewer to change between haptics renderers.
  /// The menu choice is "Rendering"->"Choose Haptics Renderer".
  class H3DAPI_API OpenHapticsRenderer: public H3DHapticsRendererNode {
  public:
    /// ShapeType specializes SFString to set the appropriate default
    /// OpenHaptics shape in the HAPI::OpenHapticsRenderer
    class H3DAPI_API ShapeType: public OnValueChangeSField< SFString > {
    protected:
      virtual void onValueChange( const string &v );
    };

    /// AdaptiveViewport specializes SFBool to set the appropriate default
    /// values in the HAPI::OpenHapticsRenderer used.
    class H3DAPI_API AdaptiveViewport: public OnValueChangeSField< SFBool > {
    protected:
      virtual void onValueChange( const bool &v );
    };

    
    /// CameraView specializes SFBool to set the appropriate default
    /// values in the HAPI::OpenHapticsRenderer used.
    class H3DAPI_API CameraView: public OnValueChangeSField< SFBool > {
    protected:
      virtual void onValueChange( const bool &v );
    };

    /// Constructor.
    OpenHapticsRenderer( Inst< ShapeType     > _defaultShapeType = 0,
                         Inst< AdaptiveViewport >  _defaultAdaptiveViewport = 0,
                         Inst< CameraView  >  _defaultHapticCameraView = 0 ) :
      defaultShapeType( _defaultShapeType ),
      defaultAdaptiveViewport( _defaultAdaptiveViewport ),
      defaultHapticCameraView( _defaultHapticCameraView ) {
      type_name = "OpenHapticsRenderer";
      database.initFields( this );

#ifndef HAVE_OPENHAPTICS
      Console(4) << "Cannot use OpenHapticsRenderer. HAPI compiled without"
     << " OpenHaptics support. Recompile HAPI with "
     << "HAVE_OPENHAPTICS defined"
     << " in order to use it." << endl;
#endif

      defaultShapeType->addValidValue( "FEEDBACK_BUFFER" );
      defaultShapeType->addValidValue( "DEPTH_BUFFER" );
      defaultShapeType->setValue( "FEEDBACK_BUFFER" );
      defaultAdaptiveViewport->setValue( true );
      defaultHapticCameraView->setValue( true );
    }

    /// The default shape type to use if it has not been specified with
    /// the OpenHapticsOptions node. 
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> "FEEDBACK_BUFFER" \n
    /// <b>Valid values: </b> "FEEDBACK_BUFFER" and "DEPTH_BUFFER"
    auto_ptr< SFString > defaultShapeType;

    /// The default value for use of adaptive viewport to use if it has not
    /// been specified with the OpenHapticsOptions node. 
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> true \n
    auto_ptr< SFBool > defaultAdaptiveViewport;

    /// The default value for use of haptic camera view to use if it has not
    /// been specified with the OpenHapticsOptions node. 
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> true \n
    auto_ptr< SFBool > defaultHapticCameraView;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  protected:
    /// Returns a new instance of HAPI::OpenHapticsRenderer
    virtual HAPI::HAPIHapticsRenderer *getNewHapticsRenderer();
  };

  /// \ingroup H3DNodes
  /// \brief Haptics renderer using the god object algorithm to render
  /// objects.
  /// Uses the HAPI::GodObjectRenderer class.
  ///
  /// For example usage use H3DViewer to change between haptics renderers.
  /// The menu choice is "Rendering"->"Choose Haptics Renderer".
  class H3DAPI_API GodObjectRenderer: public H3DHapticsRendererNode {
  public:

    /// Constructor.
    GodObjectRenderer() {
 
      type_name = "GodObjectRenderer";
      database.initFields( this );
    }

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  protected:
    /// Returns a new instance of HAPI::GodObjectRenderer
    virtual HAPI::HAPIHapticsRenderer *getNewHapticsRenderer() {
      return new HAPI::GodObjectRenderer;
    }
  };

  /// \ingroup H3DNodes
  /// \brief Haptics renderer using a variant of the Ruspini algorithm
  /// allowing you to specify a radius on the proxy.
  /// Uses the HAPI::RuspiniRenderer class.
  ///
  /// For example usage use H3DViewer to change between haptics renderers.
  /// The menu choice is "Rendering"->"Choose Haptics Renderer".
  class H3DAPI_API RuspiniRenderer: public H3DHapticsRendererNode {
  public:
    /// ProxyRadius specializes SFFloat to set the proxy radius
    /// in the HAPI::RuspiniRenderer used.
    class H3DAPI_API ProxyRadius: public OnValueChangeSField< SFFloat > {
      virtual void onValueChange( const H3DFloat &v );
    };

    class H3DAPI_API AlwaysFollowSurface: public OnValueChangeSField< SFBool > {
      virtual void onValueChange( const bool &b );
    };

    /// Constructor.
    RuspiniRenderer( Inst< ProxyRadius > _proxyRadius                 = 0,
                     Inst< AlwaysFollowSurface > _alwaysFollowSurface = 0 );

    /// The radius of the proxy in metres.
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 0.0025 \n
    auto_ptr< SFFloat > proxyRadius;

    /// Always move the proxy when the object that it is in contact with moves.
    ///
    /// Usually the proxy is only moved with the object if the object is 
    /// moving towards the proxy.
    ///
    /// Enabling this option stops the proxy from slipping on the surface when 
    /// the surface is moving tangentially. This helps when you want to drag
    /// an object using the surface friction between the surface and the 
    /// proxy.
    ///
    /// However, enabling this option has the side-effect that the proxy
    /// may fall through a surface where objects intersect.
    ///
    /// \note If enabled, dynamic objects should be consistently dynamic or
    ///       static. E.g., If the object's transform is updated by a slower
    ///       thread than the graphics thread, then the object may flicker
    ///       between static and dynamic states preventing correct behaviour.
    ///       This can happen using the RigidBodyPhysics component.
    ///       In such cases set the HapticsOptions dynamicMode field to 
    ///       "ALWAYS" to work around this.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> false \n
    auto_ptr< SFBool > alwaysFollowSurface;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  protected:
    /// Returns a new instance of HAPI::RuspiniRenderer
    virtual HAPI::HAPIHapticsRenderer *getNewHapticsRenderer() {
      return new HAPI::RuspiniRenderer( proxyRadius->getValue(), alwaysFollowSurface->getValue() );
    }
  };

#ifdef HAVE_CHAI3D
  /// \ingroup H3DNodes
  /// \brief Haptics renderer using Chai3D(www.chai3d.org)
  /// Uses the HAPI::Chai3DRenderer class.
  ///
  /// For example usage use H3DViewer to change between haptics renderers.
  /// The menu choice is "Rendering"->"Choose Haptics Renderer".
  class H3DAPI_API Chai3DRenderer: public H3DHapticsRendererNode {
  public:

    /// Constructor.
    Chai3DRenderer() {
      type_name = "Chai3DRenderer";
      database.initFields( this );
    }

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;

  protected:
    /// Returns a new instance of HAPI::Chai3DRenderer
    virtual HAPI::HAPIHapticsRenderer *getNewHapticsRenderer();
  };
#endif

  /// \ingroup H3DNodes
  /// \brief Haptics renderer which may contain other haptics renderer nodes.
  /// One for each haptics layer in the scene should be specified.
  ///
  /// The hapticsRenderer field contains a HapticsRendererNode for each layer.
  /// it is possible to mix and match renderers of different types. If no
  /// renderer is given for a specific layer then a GodObjectRenderer is
  /// used.
  ///
  /// To add a shape for a specific layer see HapticLayeredGroup.
  /// \todo Test that onAdd and onRemove works fine when a layeredrenderer
  /// is the active renderer to use (and not just switched for another
  /// renderer ) and hapticsRenderer field is changed.
  class H3DAPI_API LayeredRenderer: public H3DHapticsRendererNode {
  public:

    /// MFHapticsRendererNode extends TypedMFNode< H3DHapticsRendererNode >
    /// in order to change the haptics renderer for the used HAPIHapticsDevice
    /// when changing H3DHapticsRendererNode.
    class H3DAPI_API MFHapticsRendererNode: 
      public TypedMFNode< H3DHapticsRendererNode > {
    protected:
      
      virtual void onAdd( Node *n );

      virtual void onRemove( Node *n );
    };

    /// Constructor.
    LayeredRenderer( Inst< MFHapticsRendererNode > _hapticsRenderer = 0 );

    /// Get the haptics renderer to use for a certain layer.
    virtual HAPI::HAPIHapticsRenderer *
    getHapticsRenderer( unsigned int layer );

    /// This function should only be called if the HAPI::HAPIHapticsRenderer
    /// obtained for the given layer is removed from the HAPIDevice ( and
    /// therefore destroyed ).
    /// The reason for having this function is to not accidently leave
    /// a pointer to the destroyed instance in this node in case the node
    /// will be used again or elsewhere.
    virtual void hapticsRendererRemoved( unsigned int layer ) {
      hapticsRendererRemovedInternal( layer );
      if( layer < hapticsRenderer->size() )
        hapticsRenderer->getValueByIndex( layer )->
          hapticsRendererRemoved( 0 );
    }

    /// The hapticsRenderer field contains a HapticsRendererNode for each
    /// layer. it is possible to mix and match renderers of different types.
    /// If no renderer is given for a specific layer then a GodObjectRenderer 
    /// is used.
    /// 
    /// <b>Access type:</b> inputOutput \n
    auto_ptr< MFHapticsRendererNode > hapticsRenderer;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  protected:
    /// Returns a new instance of HAPI::RuspiniRenderer
    virtual HAPI::HAPIHapticsRenderer *getNewHapticsRenderer() {
      return new HAPI::GodObjectRenderer;
    }

    void hapticsRendererRemovedInternal( unsigned int layer, bool erase = false ) {
      if( erase ) {
        if( layer < renderers.size() ) {
          vector< HAPI::HAPIHapticsRenderer * >::iterator i = renderers.begin();
          i += layer;
          renderers.erase( i );
        }
      } else
        H3DHapticsRendererNode::hapticsRendererRemoved( layer );
    }
  };
}

#endif
