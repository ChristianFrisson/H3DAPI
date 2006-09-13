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
/// \file H3DDisplayListObject.h
/// \brief Header file for H3DDisplayListObject.
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __H3DDISPLAYLISTOBJECT_H__
#define __H3DDISPLAYLISTOBJECT_H__

#include "H3DApi.h"
#include "Field.h"
#include "Instantiate.h"
#include "GL/glew.h"
#include "FieldTemplates.h"
#include "SFBool.h"
#include "SFTime.h"

#if defined(_MSC_VER) || defined(__BORLANDC__)
#pragma comment( lib, "OpenGL32.lib" )
#pragma comment( lib, "glu32.lib" )
#pragma comment( lib, "glew32.lib" )
#endif

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
      typedef enum {
        /// Caching is on.
        ON, 
        /// Caching is off.
        OFF,
        /// Caching is depending in the GraphicalRenderingOptions in use
        OPTIONS
      } CacheMode;
        
      /// Constructor
      DisplayList();

      /// Destructor.
      ~DisplayList() {
        if( display_list ) glDeleteLists( display_list, 1 );
      }
      
      /// Set the cache mode.
      void setCacheMode( CacheMode m ) {
        cache_mode = m;
      }

      /// Get the cache mode.
      CacheMode setCacheMode( ) {
        return cache_mode;
      }

      /// Returns true if caching is in use and false otherwise.
      bool usingCaching();

      /// Returns the number of loops the DisplayList must render without
      /// receiving an event before a display is built.
      unsigned int cachingDelay();

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

    protected:
      bool childrenCachesReady( bool consider_active_field );

      /// Returns true if we have a valid display list built that can
      /// be called.
      inline bool haveValidDisplayList() {
        return have_valid_display_list;
      }
      
      /// Try to build a display list. Requires that the display lists
      /// of all H3DDisplayListNode instances that are routed to us
      /// are valid in order to build the cache. 
      /// \param cache_broken true if this call is made when the cache has just
      /// been broken, e.g. from the update() function.
      /// \returns true if the list was successfully built, false otherwise.
      bool tryBuildDisplayList( bool cache_broken ); 

    private:
      /// The fields that has generated an event since the last call to
      /// update()
      set< Field * > event_fields;

      /// Display lists will not be built as long as this counter > 0.
        unsigned int delay_cache_counter;

      /// OpenGL display list identifier.
      GLuint display_list;

      /// The mode for caching. 
      CacheMode cache_mode;
      
      /// True if the display list has been built successfully.
      bool have_valid_display_list;
      
      /// The isActive field is true if the callList() function has been called
      /// in the last scenegraph loop, and false otherwise.
      auto_ptr< IsActive > isActive;

      friend class H3DDisplayListObject;
    };
    
    /// Constructor.
    H3DDisplayListObject( Inst< DisplayList > _displayList = 0 );
    
    /// The DisplayList instance handling the OpenGL caching of this object. 
    auto_ptr< DisplayList > displayList;
  };
}

#endif
