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
/// \file H3DAPI/src/HapticTexturesSurface.cpp
/// \brief cpp file for HapticTexturesSurface
///
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/HapticTexturesSurface.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase HapticTexturesSurface::database( 
                                         "HapticTexturesSurface",
                                         &(newInstance<HapticTexturesSurface>),
                                         typeid( HapticTexturesSurface ),
                                         &H3DFrictionalSurfaceNode::database );

namespace HapticTexturesSurfaceInternals {
  FIELDDB_ELEMENT( HapticTexturesSurface, stiffnessMap, INPUT_OUTPUT );
  FIELDDB_ELEMENT( HapticTexturesSurface, maxStiffness, INPUT_OUTPUT );
  FIELDDB_ELEMENT( HapticTexturesSurface, minStiffness, INPUT_OUTPUT );
  FIELDDB_ELEMENT( HapticTexturesSurface, dampingMap, INPUT_OUTPUT );
  FIELDDB_ELEMENT( HapticTexturesSurface, maxDamping, INPUT_OUTPUT );
  FIELDDB_ELEMENT( HapticTexturesSurface, minDamping, INPUT_OUTPUT );
  FIELDDB_ELEMENT( HapticTexturesSurface, staticFrictionMap, INPUT_OUTPUT );
  FIELDDB_ELEMENT( HapticTexturesSurface, maxStaticFriction, INPUT_OUTPUT );
  FIELDDB_ELEMENT( HapticTexturesSurface, minStaticFriction, INPUT_OUTPUT );
  FIELDDB_ELEMENT( HapticTexturesSurface, dynamicFrictionMap, INPUT_OUTPUT );
  FIELDDB_ELEMENT( HapticTexturesSurface, maxDynamicFriction, INPUT_OUTPUT );
  FIELDDB_ELEMENT( HapticTexturesSurface, minDynamicFriction, INPUT_OUTPUT );

}

HapticTexturesSurface::HapticTexturesSurface( 
        Inst< UpdateStiffness        > _stiffness,
        Inst< UpdateDamping          > _damping,
        Inst< UpdateStaticFriction   > _staticFriction,
        Inst< UpdateDynamicFriction  > _dynamicFriction,
        Inst< SFBool                 > _useRelativeValues,
        Inst< SFTexture2DNode        > _stiffnessMap,
        Inst< UpdateMinMaxParamValue > _maxStiffness,
        Inst< UpdateMinMaxParamValue > _minStiffness,
        Inst< SFTexture2DNode        > _dampingMap,
        Inst< UpdateMinMaxParamValue > _maxDamping,
        Inst< UpdateMinMaxParamValue > _minDamping,
        Inst< SFTexture2DNode        > _staticFrictionMap,
        Inst< UpdateMinMaxParamValue > _maxStaticFriction,
        Inst< UpdateMinMaxParamValue > _minStaticFriction,
        Inst< SFTexture2DNode        > _dynamicFrictionMap,
        Inst< UpdateMinMaxParamValue > _maxDynamicFriction,
        Inst< UpdateMinMaxParamValue > _minDynamicFriction ):
  H3DFrictionalSurfaceNode( _stiffness,
                            _damping,
                            _staticFriction,
                            _dynamicFriction,
                            _useRelativeValues ),
  stiffnessMap( _stiffnessMap ),
  maxStiffness( _maxStiffness ),
  minStiffness( _minStiffness ),
  dampingMap( _dampingMap ),
  maxDamping( _maxDamping ),
  minDamping( _minDamping ),
  staticFrictionMap( _staticFrictionMap ),
  maxStaticFriction( _maxStaticFriction ),
  minStaticFriction( _minStaticFriction ),
  dynamicFrictionMap( _dynamicFrictionMap ),
  maxDynamicFriction( _maxDynamicFriction ),
  minDynamicFriction( _minDynamicFriction ),
  setStiffnessPtr( new SetImagePtr ),
  setDampingPtr( new SetImagePtr ),
  setStaticFrictionPtr( new SetImagePtr ),
  setDynamicFrictionPtr( new SetImagePtr ) {
  type_name = "HapticTexturesSurface";
  database.initFields( this );

  stiffnessMap->parameter_type = HAPI::HapticTexturesSurface::STIFFNESS;
  dampingMap->parameter_type = HAPI::HapticTexturesSurface::DAMPING;
  staticFrictionMap->parameter_type =
    HAPI::HapticTexturesSurface::STATIC_FRICTION;
  dynamicFrictionMap->parameter_type =
    HAPI::HapticTexturesSurface::DYNAMIC_FRICTION;

  maxStiffness->setValue( 1.0f );
  maxStiffness->parameter_type = HAPI::HapticTexturesSurface::STIFFNESS;
  maxStiffness->max_value = true;
  minStiffness->setValue( 0.0f );
  minStiffness->parameter_type = HAPI::HapticTexturesSurface::STIFFNESS;
  minStiffness->max_value = false;

  maxDamping->setValue( 1.0f );
  maxDamping->parameter_type = HAPI::HapticTexturesSurface::DAMPING;
  maxDamping->max_value = true;
  minDamping->setValue( 0.0f );
  minDamping->parameter_type = HAPI::HapticTexturesSurface::DAMPING;
  minDamping->max_value = false;

  maxStaticFriction->setValue( 1.0f );
  maxStaticFriction->parameter_type =
    HAPI::HapticTexturesSurface::STATIC_FRICTION;
  maxStaticFriction->max_value = true;
  minStaticFriction->setValue( 0.0f );
  minStaticFriction->parameter_type =
    HAPI::HapticTexturesSurface::STATIC_FRICTION;
  minStaticFriction->max_value = false;

  maxDynamicFriction->setValue( 1.0f );
  maxDynamicFriction->parameter_type =
    HAPI::HapticTexturesSurface::DYNAMIC_FRICTION;
  maxDynamicFriction->max_value = true;
  minDynamicFriction->setValue( 0.0f );
  minDynamicFriction->parameter_type =
    HAPI::HapticTexturesSurface::DYNAMIC_FRICTION;
  minDynamicFriction->max_value = false;

  setStiffnessPtr->setOwner( this );
  setDampingPtr->setOwner( this );
  setStaticFrictionPtr->setOwner( this );
  setDynamicFrictionPtr->setOwner( this );

  setStiffnessPtr->parameter_type = HAPI::HapticTexturesSurface::STIFFNESS;
  setDampingPtr->parameter_type = HAPI::HapticTexturesSurface::DAMPING;
  setStaticFrictionPtr->parameter_type =
    HAPI::HapticTexturesSurface::STATIC_FRICTION;
  setDynamicFrictionPtr->parameter_type =
    HAPI::HapticTexturesSurface::DYNAMIC_FRICTION;
}

void HapticTexturesSurface::initialize() {
  H3DFrictionalSurfaceNode::initialize();
  
  X3DTexture2DNode * a_map = 
     static_cast< X3DTexture2DNode * >( stiffnessMap->getValue() );
  Image * stiffness_image = 0;
  if( a_map )
    stiffness_image = a_map->image->getValue();

  a_map = static_cast< X3DTexture2DNode * >( dampingMap->getValue() );
  Image * damping_image = 0;
  if( a_map )
    damping_image = a_map->image->getValue();

  a_map = static_cast< X3DTexture2DNode * >( staticFrictionMap->getValue() );
  Image * static_friction_image = 0;
  if( a_map )
    static_friction_image = a_map->image->getValue();

  a_map = static_cast< X3DTexture2DNode * >( dynamicFrictionMap->getValue() );
  Image * dynamic_friction_image = 0;
  if( a_map )
    dynamic_friction_image = a_map->image->getValue();

  H3DFloat max_stiffness = maxStiffness->getValue();
  H3DFloat min_stiffness = minStiffness->getValue();
  if( max_stiffness < min_stiffness ) {
    min_stiffness = max_stiffness;
    Console(3) << "Warning: Value of maxStiffness is smaller than value of "
               << "minStiffness in node " << getName() << ". Node will not "
               << "behave as intended." << endl;
  }

  H3DFloat max_damping = maxDamping->getValue();
  H3DFloat min_damping = minDamping->getValue();
  if( max_damping < min_damping ) {
    min_damping = max_damping;
    Console(3) << "Warning: Value of maxDamping is smaller than value of "
               << "minDamping in node " << getName() << ". Node will not "
               << "behave as intended." << endl;
  }

  H3DFloat max_static_friction = maxStaticFriction->getValue();
  H3DFloat min_static_friction = minStaticFriction->getValue();
  if( max_static_friction < min_static_friction ) {
    min_static_friction = max_static_friction;
    Console(3) << "Warning: Value of maxStaticFriction is smaller than value "
               << "of minStaticFriction in node " << getName() << ". Node "
               << "will not behave as intended." << endl;
  }
  
  H3DFloat max_dynamic_friction = maxDynamicFriction->getValue();
  H3DFloat min_dynamic_friction = minDynamicFriction->getValue();
  if( max_dynamic_friction < min_dynamic_friction ) {
    min_dynamic_friction = max_dynamic_friction;
    Console(3) << "Warning: Value of maxDynamicFriction is smaller than value "
               << "of minDynamicFriction in node " << getName() << ". Node "
               << "will not behave as intended." << endl;
  }

  hapi_surface.reset(
    new HAPI::HapticTexturesSurface( stiffness->getValue(),
                                     damping->getValue(),
                                     staticFriction->getValue(),
                                     dynamicFriction->getValue(),
                                     stiffness_image,
                                     max_stiffness,
                                     min_stiffness,
                                     damping_image,
                                     max_damping,
                                     min_damping,
                                     static_friction_image,
                                     max_static_friction,
                                     min_static_friction,
                                     dynamic_friction_image,
                                     max_dynamic_friction,
                                     min_dynamic_friction,
                                     useRelativeValues->getValue() ) );
}

void HapticTexturesSurface::SetImagePtr::update() {
   Image *image =
     static_cast< X3DTexture2DNode::SFImage * >(event.ptr)->getValue();
   HapticTexturesSurface *hms = 
     static_cast< HapticTexturesSurface * >( getOwner() );
   HAPI::HapticTexturesSurface * hapi_surface = 
     static_cast< HAPI::HapticTexturesSurface * >( hms->hapi_surface.get() );
   if( hapi_surface ) {
     hapi_surface->setParameterImage( image, parameter_type );
   }
 }

 void HapticTexturesSurface::SFTexture2DNode::onAdd( Node *n ) {
   SFTexture2DNodeBase::onAdd( n );
   X3DTexture2DNode *c = static_cast< X3DTexture2DNode* >( n );
   HapticTexturesSurface *o = static_cast< HapticTexturesSurface* >( owner );
   if( c ) {
     switch( parameter_type ) {
       case HAPI::HapticTexturesSurface::STIFFNESS: {
         c->image->route( o->setStiffnessPtr );
         break;
       }
       case HAPI::HapticTexturesSurface::DAMPING: {
         c->image->route( o->setDampingPtr );
         break;
       }
       case HAPI::HapticTexturesSurface::STATIC_FRICTION: {
         c->image->route( o->setStaticFrictionPtr );
         break;
       }
       case HAPI::HapticTexturesSurface::DYNAMIC_FRICTION: {
         c->image->route( o->setDynamicFrictionPtr );
         break;
       }
       default: {}
     }
   }
 }

 void HapticTexturesSurface::SFTexture2DNode::onRemove( Node *n ) {
   X3DTexture2DNode *c = static_cast< X3DTexture2DNode* >( n );
   HapticTexturesSurface *o = static_cast< HapticTexturesSurface* >( owner );
   if( c ) {
     switch( parameter_type ) {
       case HAPI::HapticTexturesSurface::STIFFNESS: {
         c->image->unroute( o->setStiffnessPtr );
         break;
       }
       case HAPI::HapticTexturesSurface::DAMPING: {
         c->image->unroute( o->setDampingPtr );
         break;
       }
       case HAPI::HapticTexturesSurface::STATIC_FRICTION: {
         c->image->unroute( o->setStaticFrictionPtr );
         break;
       }
       case HAPI::HapticTexturesSurface::DYNAMIC_FRICTION: {
         c->image->unroute( o->setDynamicFrictionPtr );
         break;
       }
       default: {}
     }
     HAPI::HapticTexturesSurface * hapi_surface = 
       static_cast< HAPI::HapticTexturesSurface * >( o->hapi_surface.get() );
     if( hapi_surface )
       hapi_surface->setParameterImage( 0, parameter_type );
   }
   SFTexture2DNodeBase::onRemove( n );
 }

void HapticTexturesSurface::UpdateMinMaxParamValue::
  onValueChange( const H3DFloat &v ) {
  HapticTexturesSurface *hts = 
    static_cast< HapticTexturesSurface * >( getOwner() );
  if( hts->hapi_surface.get() ) {
    // Can this part cause problems with updating?
    // Because of circular updates? It should not since
    // AutoUpdate should "prevent" this.

    // This is only used to not clutter the code with many Console(3)
    // warning strings.
    string error_first = "";
    string error_second = "";
    switch( parameter_type ) {
      case HAPI::HapticTexturesSurface::STIFFNESS: {
        if( max_value ) {
          if( v >= hts->minStiffness->getValue() )
            static_cast< HAPI::HapticTexturesSurface * >
              ( hts->hapi_surface.get() )
              ->setParameterMaxValue( v, parameter_type );
          else {
            error_first = "maxStiffness";
            error_second = "below the minStiffness";
          }
        } else {
          if( v <= hts->maxStiffness->getValue() )
            static_cast< HAPI::HapticTexturesSurface * >
              ( hts->hapi_surface.get() )
              ->setParameterMinValue( v, parameter_type );
          else {
            error_first = "minStiffness";
            error_second = "above the maxStiffness";
          }
        }
        break;
      }
      case HAPI::HapticTexturesSurface::DAMPING: {
        if( max_value ) {
          if( v >= hts->minDamping->getValue() )
            static_cast< HAPI::HapticTexturesSurface * >
              ( hts->hapi_surface.get() )
              ->setParameterMaxValue( v, parameter_type );
          else {
            error_first = "maxDamping";
            error_second = "below the minDamping";
          }
        } else {
          if( v <= hts->maxDamping->getValue() )
            static_cast< HAPI::HapticTexturesSurface * >
              ( hts->hapi_surface.get() )
              ->setParameterMinValue( v, parameter_type );
          else {
            error_first = "minDamping";
            error_second = "above the maxDamping";
          }
        }
        break;
      }
      case HAPI::HapticTexturesSurface::STATIC_FRICTION: {
        if( max_value ) {
          if( v >= hts->minStaticFriction->getValue() )
            static_cast< HAPI::HapticTexturesSurface * >
              ( hts->hapi_surface.get() )
              ->setParameterMaxValue( v, parameter_type );
          else {
            error_first = "maxStaticFriction";
            error_second = "below the minStaticFriction";
          }
        } else {
          if( v <= hts->maxStaticFriction->getValue() )
            static_cast< HAPI::HapticTexturesSurface * >
              ( hts->hapi_surface.get() )
              ->setParameterMinValue( v, parameter_type );
          else {
            error_first = "minStaticFriction";
            error_second = "above the maxStaticFriction";
          }
        }
        break;
      }
      case HAPI::HapticTexturesSurface::DYNAMIC_FRICTION: {
        if( max_value ) {
          if( v >= hts->minDynamicFriction->getValue() )
            static_cast< HAPI::HapticTexturesSurface * >
              ( hts->hapi_surface.get() )
              ->setParameterMaxValue( v, parameter_type );
          else {
            error_first = "maxDynamicFriction";
            error_second = "below the minDynamicFriction";
          }
        } else {
          if( v <= hts->maxDynamicFriction->getValue() )
            static_cast< HAPI::HapticTexturesSurface * >
              ( hts->hapi_surface.get() )
              ->setParameterMinValue( v, parameter_type );
          else {
            error_first = "minDynamicFriction";
            error_second = "above the maxDynamicFriction";
          }
        }
        break;
      }
      default: {}
    }

    if( error_first != "" ) {
      Console(3) << "Warning: Trying to set the value of the field "
                 << error_first << " to a value which is " << error_second
                 << " value. In node named " << hts->getName()
                 << ". Value will not be transferred "
                 << "to the haptic loop." << endl;
    }
  }
}

