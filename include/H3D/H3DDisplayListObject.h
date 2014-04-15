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
/// \file H3DDisplayListObject.h
/// \brief Header file for H3DDisplayListObject.
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __H3DDISPLAYLISTOBJECT_H__
#define __H3DDISPLAYLISTOBJECT_H__

#include <GL/glew.h>
#include <H3D/H3DApi.h>
#include <H3D/Field.h>
#include <H3D/Instantiate.h>
#include <H3D/FieldTemplates.h>
#include <H3D/SFBool.h>
#include <H3D/SFTime.h>

namespace H3D {
  /// \ingroup AbstractInterface
  /// \brief This abstract interface class is the base class for all node
  /// types that wants to create an OpenGL display list for the OpenGL
  /// calls it makes in its render() function. 
  ///
  /// Subclasses should set the owner and name of the 
  /// displayList field. Fields can be routed to the displayList field in
  /// order to break the cache and create a new display list when values
  /// have changed. In order to make use of the display list the callList()
  /// function of the displayList field should be called instead of the 
  /// render() function.
  
  class GraphicsOptions;
  class H3DAPI_API H3DDisplayListObject {
  public:

    /// We use the Field's lazy evaluation mechanisms to manage the
    /// GL display lists for rendering, giving automatic cache breaking
    /// when dependencies are modified.
    class H3DAPI_API DisplayList : public Field {
    protected:
      /// The IsActive field is updated each scenegraph loop, since
      /// Scene::time is routed to it to see if the callList() function
      /// was called during or since the last loop. 
      class IsActive: public AutoUpdate< TypedField< SFBool, SFTime > > {
        public:
        /// Constructor.
        IsActive():
          active( true ) {}
        
        /// Updates the value of the field.
        virtual void update() {
          value = active;
          active = false;
        }

        friend class DisplayList;

      protected:
        /// Should be called from callList().
        void callListCalled() {
          setValue( true );
          active = true;
        }
        bool active;
      };
      
    public:
      /// \brief The entries in CacheMode enumerator is used for checking if
      /// caching should be done or not.
      typedef enum {
        /// Caching is on.
        ON, 
        /// Caching is off.
        OFF,
        /// Caching is depending on the GraphicalRenderingOptions in use.
        OPTIONS
      } CacheMode;

      /// \brief The entries in FrustumCullingMode enumerator is used for
      /// checking if view frustum culling should be done.
      typedef CacheMode FrustumCullingMode;
        
      /// Constructor
      DisplayList();

      /// Destructor.
      ~DisplayList() {
        if( display_list ) glDeleteLists( display_list, 1 );
      }
      
      /// Set the cache mode.
      void setCacheMode( CacheMode m  ) {
        cache_mode = m;
      }

      /// Get the cache mode.
      CacheMode getCacheMode( ) {
        return cache_mode;
      }

      /// Set the frustum culling mode.
      void setFrustumCullingMode( FrustumCullingMode m  ) {
        frustum_culling_mode = m;
      }

      /// Get the frustum culling mode.
      FrustumCullingMode getFrustumCullingMode( ) {
        return frustum_culling_mode;
      }

      /// Returns true if caching is in use and false otherwise.
      virtual bool usingCaching();

      /// Returns true if view frustum culling is on.
      bool usingFrustumCulling();

      /// Returns true if the bounding box of the owner of this field
      /// lies outside the view frustum. 
      bool isOutsideViewFrustum();

      /// Force a rebuild of all display lists created.
      static void rebuildAllDisplayLists();

      /// Returns the number of loops the DisplayList must render without
      /// receiving an event before a display is built.
      virtual unsigned int cachingDelay();

      /// If called the display list will be rebuilt on next call to
      void breakCache();

      /// Calls tryBuildDisplayList().
      virtual void update();
      
      /// When the event is propagated the display list is invalidated.
      virtual void propagateEvent( Event e );
      
      /// Calls the OpenGL call list for if it exists. If it does
      /// not exist we try to build it and execute it. 
      virtual void callList( bool build_list = true );

      /// Gets the pointer to the last field that created an event to 
      /// this field 
      inline Field *getEventPointer() {
        return event.ptr;
      }

      /// Returns true if the Field given has generated an event to this
      /// field since the last call to the update() function.
      inline bool hasCausedEvent( Field *f ) {
        return event_fields.find( f ) != event_fields.end();
      }

      /// Returns true if the Field given has generated an event to this
      /// field since the last call to the update() function.
      template< class FieldType >
      inline bool hasCausedEvent( auto_ptr< FieldType > &f ) {
        return hasCausedEvent( f.get() );
      }

      /// initialize previous graphic option
      void initGraphicOption();

    protected:
      bool childrenCachesReady( bool consider_active_field );

      /// Returns true if we have a valid display list built that can
      /// be called.
      inline virtual bool haveValidDisplayList() {
        return have_valid_display_list;
      }
      
      /// Try to build a display list. Requires that the display lists
      /// of all H3DDisplayListNode instances that are routed to us
      /// are valid in order to build the cache. 
      /// \param cache_broken true if this call is made when the cache has just
      /// been broken, e.g. from the update() function.
      /// \returns true if the list was successfully built, false otherwise.
      bool tryBuildDisplayList( bool cache_broken ); 

    protected:
      /// The fields that has generated an event since the last call to
      /// update()
      set< Field * > event_fields;

      /// Display lists will not be built as long as this counter > 0.
        unsigned int delay_cache_counter;

      /// OpenGL display list identifier.
      GLuint display_list;

      /// The mode for caching. 
      CacheMode cache_mode;

      /// The mode for view frustum culling. 
      FrustumCullingMode frustum_culling_mode;
      
      /// True if the display list has been built successfully.
      bool have_valid_display_list;
      
      /// The isActive field is true if the callList() function has been called
      /// in the last scenegraph loop, and false otherwise.
      auto_ptr< IsActive > isActive;

      /// This field is routed to all instances of DisplayList and can be used 
      /// to force a rebuild of all display lists.
      static auto_ptr< Field > break_list_field; 
      
      /// previous graphic option
      GraphicsOptions* graphic_options_previous;
      
      /// default cache delay value
      static const int cache_delay_default = 3;

      /// flag to require resetting delay_cache_counter
      bool reset_delay_cache_counter;

      friend class H3DDisplayListObject;
    };
    
    /// Constructor.
    H3DDisplayListObject( Inst< DisplayList > _displayList = 0 );
    
    /// The DisplayList instance handling the OpenGL caching of this object. 
    auto_ptr< DisplayList > displayList;
  };
}

#endif
