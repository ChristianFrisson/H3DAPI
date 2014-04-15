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
//
/// \file TraverseInfo.h
/// \brief Header file for TraverseInfo.
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __TRAVERSEINFO_H__
#define __TRAVERSEINFO_H__

#include <H3D/H3DTypes.h>
#include <HAPI/HAPIForceEffect.h>
#include <HAPI/HAPIHapticShape.h>
#include <H3DUtil/AutoRefVector.h>
#include <sstream>
#include <stack>
#include <map>

using namespace std;

namespace H3D {
  // forward declarations
  class H3DSurfaceNode;
  class H3DHapticsDevice;
  class X3DGeometryNode;
  class X3DLightNode;

  typedef AutoRefVector< HAPI::HAPIHapticShape > HapticShapeVector;
  typedef AutoRefVector< HAPI::HAPIForceEffect > HapticEffectVector;

  /// TraverseInfo is a structure that is passed along when traversing the 
  /// scene graph. It contains information needed during the traversing, 
  /// including e.g. transform matrices, available haptics devices etc.
  /// Geometry nodes must register the shapes to render haptically
  /// by calls to the addHapticShape() function calls.
  /// 
  class H3DAPI_API TraverseInfo {
  public:
 
    /// Thrown when trying to get hold of a H3DHapticsDevice using an invalid
    /// device index.
    ///
    H3D_VALUE_EXCEPTION( int, InvalidHapticsDeviceIndex ); 

    /// Constructor.
    /// \param _haptics_devices The HapticDevice instances that are available
    /// to render HapticShapes on.
    /// 
    TraverseInfo( const vector< H3DHapticsDevice * > &_haptics_devices );

    /// Returns the number of haptics layers that are available and initialized.
    /// If you need more you will have to use the initializeLayers() function
    inline unsigned int nrLayers() {
      if( haptic_shapes.size() > 0 ) {
        return (unsigned int) haptic_shapes[0].size();
      } else {
        return 0;
      }
    }

    /// Make sure that the TraverseInfo object have enough resources allocated
    /// for the given amount of haptic layers.
    inline void initializeLayers( unsigned int nr_layers ) {
      for( unsigned int i = 0; i < haptic_shapes.size(); ++i ) {
        if( haptic_shapes[i].size() < nr_layers )
          haptic_shapes[i].resize( nr_layers );
      }
    }

    /// Set the currently active haptic layer. The currently active haptic layer
    /// determines which layer later calls to addHapticShape and addHapticShapeToAll
    /// should add the shapes to.
    inline void setCurrentLayer( unsigned int layer ) {
      current_layer = layer;
    }

    /// Get the currently active haptic layer.
    inline unsigned int getCurrentLayer() {
      return current_layer;
    }

    /// Returns the index of the given H3DHapticsDevice. -1 is returned
    /// if the H3DHapticsDevice is not in the vector of haptics devices.
    inline int getHapticsDeviceIndex( H3DHapticsDevice *hd ) {
      int index = 0;
      for( vector< H3DHapticsDevice * >::iterator i = haptics_devices.begin();
           i != haptics_devices.end();
           ++i, ++index ) {
        if( (*i) == hd ) return index;
      }
      return -1;
    } 

    /// Adds a HapticShape to be rendered by a H3DHapticsDevice.
    /// Shapes will only be added if hapticsEnabled() is true. 
    /// \param device_index The index of the H3DHapticsDevice to render the 
    /// shape on.
    /// \param shape The HapticShape to render.
    ///
    void addHapticShape( int device_index, 
                         HAPI::HAPIHapticShape *shape );

    /// Adds a HapticShape to be rendered by a H3DHapticsDevice.
    /// Shapes will only be added if hapticsEnabled() is true. 
    /// \param hd The H3DHapticsDevice to render the shape on.
    /// \param shape The HapticShape to render.
    ///
    inline void addHapticShape( H3DHapticsDevice *hd, 
                                HAPI::HAPIHapticShape *shape ) {
      int device_index = getHapticsDeviceIndex( hd );
      addHapticShape( device_index, shape );
    }
    
    /// Adds a HapticShape to be rendered by all H3DHapticsDevices.
    /// Shapes will only be added if hapticsEnabled() is true. 
    /// \param shape The HapticShape to render.
    ///
    void addHapticShapeToAll( HAPI::HAPIHapticShape *shape );

    /// Get the HapticShapes that has been added for the H3DHapticsDevice
    /// with the given device_index.
    ///
    inline const HapticShapeVector &getHapticShapes(int device_index ){
      if( device_index < 0 || device_index >= (int)haptics_devices.size() ) {
        stringstream s;
        s << "TraverseInfo only has " << (unsigned int) haptics_devices.size() 
          << " haptics device available. ";
        throw InvalidHapticsDeviceIndex( device_index,
                                         s.str(),
                                         H3D_FULL_LOCATION );
      }
      return haptic_shapes[ device_index ][ current_layer ];
    }

    /// Get the HapticShapes that has been added for the given H3DHapticsDevice
    ///
    inline const HapticShapeVector 
    &getHapticShapes( H3DHapticsDevice * hd ) {
      int device_index = getHapticsDeviceIndex( hd );
      return getHapticShapes( device_index );
    }

    /// Adds a HAPIForceEffect to be rendered by a H3DHapticsDevice.
    /// Effects will only be added if hapticsEnabled() is true. 
    /// \param device_index The index of the H3DHapticsDevice to render the 
    /// shape on.
    /// \param effect The HAPIForceEffect to render.
    ///
    void addForceEffect( int device_index, HAPI::HAPIForceEffect *effect );

    /// Adds a HAPIForceEffect to be rendered by a H3DHapticsDevice.
    /// Shapes will only be added if hapticsEnabled() is true. 
    /// \param hd The H3DHapticsDevice to render the shape on.
    /// \param effect The HAPIForceEffect to render.
    ///
    inline void addForceEffect( H3DHapticsDevice *hd, 
                                HAPI::HAPIForceEffect *effect ) {
      int device_index = getHapticsDeviceIndex( hd );
      addForceEffect( device_index, effect );
    }
    
    /// Adds a HAPIForceEffect to be rendered by all H3DHapticsDevices.
    /// Effects will only be added if hapticsEnabled() is true. 
    /// \param effect The HAPIForceEffect to render.
    ///
    void addForceEffectToAll( HAPI::HAPIForceEffect *effect );
                
    /// Get the HAPIForceEffects that has been added for the H3DHapticsDevice
    /// with the given device_index.
    ///
    inline const HapticEffectVector 
      &getForceEffects( int device_index){
      if( device_index < 0 || device_index >= (int)haptics_devices.size() ) {
        stringstream s;
        s << "TraverseInfo only has " << (unsigned int)haptics_devices.size() 
          << " haptics device available. ";
        throw InvalidHapticsDeviceIndex( device_index,
                                         s.str(),
                                         H3D_FULL_LOCATION );
      }
      return haptic_effects[ device_index ];
    }
    
    /// Get the HAPIForceEffects that has been added for the given 
    /// H3DHapticsDevice
    inline const HapticEffectVector
      &getForceEffects( H3DHapticsDevice * hd ) {
      int device_index = getHapticsDeviceIndex( hd );
      return getForceEffects( device_index );
    }
    
    /// Sets the surface that is active in the current traversal.
    inline void setCurrentSurface( H3DSurfaceNode *s ) {
      current_surface = s;
    }

    /// Get the surface that is active in the current traversal.
    inline H3DSurfaceNode * getCurrentSurface() {
      return current_surface;
    }
    
    inline H3DHapticsDevice *getHapticsDevice( int device_index ) {
      return haptics_devices[ device_index ];
    }

    /// Get the haptics devices that are available.
    inline const vector< H3DHapticsDevice * > &getHapticsDevices() {
      return haptics_devices;
    }

    /// When called, future calls to addHapticShapes() and addForceEffects() 
    /// functions will not add any shapes until enableHaptics() is called.
    inline void disableHaptics( int i = -1 ) {
      if( i == -1 ) {
        for( unsigned int j = 0; j < haptics_enabled.size(); ++j ) {
          haptics_enabled[j] = false;
        }
      } else {
        haptics_enabled[i] = false;
      }
    }

    /// When called, the addHapticShapes() and addForceEffects() functions
    /// calls will add HapticShapes to render. Haptics is enabled by default.
    inline void enableHaptics( int i = -1 ) {
      if( i == -1 ) {
        for( unsigned int j = 0; j < haptics_enabled.size(); ++j ) {
          haptics_enabled[j] = true;
        }
      } else {
        haptics_enabled[i] = true;
      }
    }

    /// Indicate that this node will not be part of the graphics 
    /// rendering traversal. This should be set by nodes that disable
    /// graphics rendering. It does not do anything except setting a 
    /// state in the TraverseInfo class indicating that this node will
    /// not be used in graphics rendering.
    inline void disableGraphics() {
      graphics_enabled = false;
    }

    /// Indicate that this node will be part of the graphics 
    /// rendering traversal. It does not do anything except setting a 
    /// state in the TraverseInfo class indicating that this node will
    /// be used in graphics rendering.
    inline void enableGraphics() {
      graphics_enabled = true;
    }
    
    /// If multi pass transparency is set to true the scene will be rendered
    /// three times graphically, once for all solid objects, once for the back
    /// side of transparent objects and once for the front face of 
    /// transparent objects. This is in order to avoid artifacts for 
    /// transparent objects when rendered int the wrong order.
    inline void setMultiPassTransparency( bool b ) {
      multi_pass_transparency = b;
    }

    /// See if multi pass transparency objects have been encountered.
    inline bool getMultiPassTransparency() {
      return multi_pass_transparency;
    }

    /// Check whether haptics is enabled for a device index 
    /// or not. If disabled the
    /// addHapticsShapes() functions will have no effect. Geometry
    /// nodes that add shapes should check that haptics is enabled 
    /// before doing any calculations deciding what to render. This is
    /// because any HapticShape created will not be rendered anyway
    /// so no computer resources should be used creating them.
    inline bool hapticsEnabled( unsigned int i = 0 ) {
      if( i < haptics_enabled.size() ) {
        return haptics_enabled[i];
      } else {
        return false;
      }
    }

    /// Returns true if haptics is enabled for all haptics devices.
    inline bool hapticsEnabledForAll() {
      for( unsigned int i = 0; i < haptics_enabled.size(); ++i ) {
        if( !haptics_enabled[i] ) return false;
      }
      return true;
    }

    /// Returns true if haptics is disabled for all haptics devices.
    inline bool hapticsDisabledForAll() {
      for( unsigned int i = 0; i < haptics_enabled.size(); ++i ) {
        if( haptics_enabled[i] ) return false;
      }
      return true;
    }

    /// Get a vector the same size as the haptics device vector
    /// indicating if haptics is disabled for that device or not
    inline const vector< bool >&getHapticsEnabled() {
      return haptics_enabled;
    }

    /// Set a vector indicating for each haptics device if haptics
    /// is disabled or enabled. The vector must be the same size
    /// as the number of haptics devices available. Returns -1 on
    /// error, 0 on success.
    inline int setHapticsEnabled( const vector< bool > &enabled) {
      if( enabled.size() != haptics_enabled.size() ) return -1;
      else {
        haptics_enabled = enabled;
        return 0;
      }
    }

    /// Check whether graphics is enabled or not. 
    inline bool graphicsEnabled() {
      return graphics_enabled;
    }

    /// This function should be called when we move into a nested
    /// coordinate space with the transformation matrices between
    /// the coordinate spaces.
    /// \param forward Transformation matrix between the space we
    /// move into to the previous space.
    /// \param inverse The inverse of forward.
    ///
    inline void pushMatrices( const Matrix4f &forward,
                              const Matrix4f &inverse ) {
      const TransformInfo &top = transform_stack.top();
      transform_stack.push( TransformInfo( top.acc_frw * forward,
                                           inverse * top.acc_inv ) );
    }

    /// This function should be called when we move out from a 
    /// nested coordinate space
    inline void popMatrices() {
      transform_stack.pop();
    }

    /// Get the accumulated forward transformation matrix, i.e. the
    /// transformation matrix from the local space at the current 
    /// traversal position to global space.
    inline const Matrix4f &getAccForwardMatrix() {
      return transform_stack.top().acc_frw;
    }

    /// Get the accumulated inverse transformation matrix, i.e. the
    /// transformation matrix from global space to the local space 
    /// at the current traversal position.
    inline const Matrix4f &getAccInverseMatrix() {
      return transform_stack.top().acc_inv;
    }
    
    /// Returns the number of times the given geometry has been added
    /// as the source of a haptic shape so far in the traversal.
    inline int getGeometryCount( X3DGeometryNode *geom ) {
      return geometry_count[geom];
    }

    /// Set user data in the traversinfo object. User data can be used e.g.
    /// to pass information to child nodes about values in parent nodes
    inline void setUserData( const string &key, void *data ) {
      user_data[key] = data;
    }

    /// Returns true of user data with the given key exists.
    inline bool haveUserData( const string &key ) {
      return user_data.find( key ) != user_data.end();
    }

    /// Delete user data with the given key. Returns 0 on success.
    inline int deleteUserData( const string &key ) {
      std::map<string, void *>::iterator i = user_data.find( key );
      if( i != user_data.end() ) {
        user_data.erase( i );
        return 0;
      } else {
        return -1;
      }
    }
    
    /// Get the user data value with the given key. Output in the data
    /// argument. Returns 0 on success.
    inline int getUserData( const string &key, void ** data ) {
      std::map<string, void *>::iterator i = user_data.find( key );
      if( i != user_data.end() ) {
        *data = (*i).second; 
        return 0;
      } else {
        return -1;
      }
    }

    /// Callback function type.
    typedef void (*CallbackFunc)(TraverseInfo &ti, void *data); 

    /// Adds a callback function that will be called after the entire scene 
    /// has been traversed 
    inline void addPostTraverseCallback( CallbackFunc func, void *data ) {
      post_traverse_callbacks.push_back( make_pair( func, data ) );
    }
      
    /// Call all functions added by the addPostTraverseCallback function.
    void callPostTraverseCallbacks();

    /// Class grouping X3DLightNode and its transformation matrix.
    class LightInfo {
    public:
      /// Constructor.
      LightInfo( X3DLightNode *, const Matrix4f &transform );
      /// Get the light node.
      X3DLightNode *getLight() const;
      /// Get the transformation matrix for the light.
      inline const Matrix4f &getLightTransform() const { return transform; }
    protected:
      AutoRef< Node > light;
      Matrix4f transform;
    };

    /// Used to get all the X3DLightNodes that are global.
    typedef vector< LightInfo > LightVector;

    /// Add a light that is active to the TraverseInfo object.
    /// The transform matrix is the transformation from the light's
    /// local space to world space.
    /// Should only be done by the light nodes themselves.
    void addActiveLightNode( X3DLightNode *light, const Matrix4f &transform );

    /// Remove a light that is active to the TraverseInfo object.
    /// Should only be done by the light nodes themselves.
    void removeActiveLightNode( X3DLightNode *light, const Matrix4f &transform );
    
    /// Returns the active lights.
    const LightVector &getActiveLightNodes();

  protected:
    LightVector x3dlightnode_vector;

    class TransformInfo {
    public:
      TransformInfo( const Matrix4f &_acc_frw,
                     const Matrix4f &_acc_inv ):
        acc_frw( _acc_frw ),
        acc_inv( _acc_inv ) {}
      Matrix4f acc_frw, acc_inv;
    };
    stack< TransformInfo > transform_stack;
    
    typedef std::list< std::pair< CallbackFunc, void * > > CallbackList;
    // A list of the callback functions to run after scene traversal.
    CallbackList post_traverse_callbacks;

    unsigned int current_layer;
    H3DSurfaceNode *current_surface;
    vector< H3DHapticsDevice * > haptics_devices;
    vector< vector< HapticShapeVector > > haptic_shapes;
    vector< HapticEffectVector > haptic_effects;
    bool graphics_enabled;
    bool multi_pass_transparency;
    // When the global haptics_enabled is true the following vector contains
    // per devceinformation about which devices have haptics enabled.
    vector< bool > haptics_enabled;

    typedef std::map< X3DGeometryNode *, int > GeometryCountMap;
    GeometryCountMap geometry_count;
    std::map< string, void * > user_data;
  };

};

#endif
