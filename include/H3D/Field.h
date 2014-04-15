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
/// \file Field.h
/// \brief Contains the Field class.
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __FIELD_H__
#define __FIELD_H__

#include <iostream>
#include <string>
#include <set>
#include <vector>
#include <H3D/H3DApi.h>
#include <H3D/H3DTypes.h>
#include <H3DUtil/TimeStamp.h>
#include <H3D/X3DTypes.h>
#include <memory>

using namespace std;

namespace H3D {
  // forward declaration
  class Node;

  /// The Field class.
  class H3DAPI_API Field {
  public:
    /// Exception thrown when performing actions that conflict with the 
    /// field's access type.
    H3D_API_EXCEPTION( FieldAccessError );

    /// The different access types that a field can have.
    typedef enum {
      /// The value of the field can only be changed before the Node that
      /// is the owner of the field has been initialized. If no owner exists
      /// it works as an INPUT_OUTPUT field.
      INITIALIZE_ONLY,
      /// The value of the field can only read, not set. Only the Node that
      /// is the owner of the field can change its value by providing its
      /// id to the setValue() function. Also only the owner can set up routes
      /// to the field. If no owner exists it works as an INPUT_OUTPUT field.
      OUTPUT_ONLY,
      /// The value of the field can only set, not read. Only the Node that
      /// is the owner of the field can read its value by providing its
      /// id to the getValue() function. Also only the owner can set up routes
      /// from the field. If no owner exists it works as an INPUT_OUTPUT field.
      INPUT_ONLY,
      /// No restrictions on access to the field values or routes.
      INPUT_OUTPUT 
    } AccessType;

    /// The Event struct encapsulates an event in the field network. It
    /// consists of the Field causing the event and a TimeStamp for when
    /// the event was generated.
    struct H3DAPI_API Event {
      Event( Field *_ptr, const TimeStamp &_time_stamp ):
        ptr( _ptr ),
        time_stamp( _time_stamp ) {}
      /// The Field that caused the event.
      Field *ptr;
      /// The time of the event.
      TimeStamp time_stamp;       
    };    

    /// Constructor.
    Field();
    
    /// Destructor.
    virtual ~Field();
        
    /// Check that a given input field is of the correct type. Needed
    /// for TypedField.
    /// \param f The field to check the type for
    /// \param index The index of the field in the routes_in member.
    virtual void checkFieldType( Field *f, int index ) {}

    /// Sets local the name of the field. Will be used in error reporting.
    void setName( string _name ) { name = _name; }

    /// Gets the name of the field. 
    string getName();

    /// Get the full name of the field including the name of the owner
    /// node if it exists (ownername.fieldname). 
    string getFullName();

    /// Returns a string name for this field type.
    virtual string getTypeName() { return classTypeName(); }

    /// Returns the X3DType of the field.
    virtual X3DTypes::X3DType getX3DType() { return X3DTypes::UNKNOWN_X3D_TYPE; }

    /// Returns a string name for this field type. Must have a static
    /// version in order to give relevant error messages in TypedField.
    static string classTypeName() { return "Field"; }

    /// Set up a route from this field to the field given as argument.
    /// An event will be generated to that field.
    virtual void route( Field*, int id = 0 );

    /// Set up a route from this field to the field given as argument.
    /// No event will be generated to that field.
    virtual void routeNoEvent( Field*, int id = 0 );
    
    /// Remove the route from this field to the field given as argument.
    virtual void unroute( Field* );

    /// Replace the i:th route to f with a route from this field.
    /// An event will be generated to that field.
    virtual Field *replaceRoute( Field* f, unsigned int i, int id = 0 );

    /// Replace the i:th route to f with a route from this field.
    /// No event will be generated to that field.
    virtual Field *replaceRouteNoEvent( Field* f, unsigned int i, int id = 0 );

    /// Set up a route from this field to the field residing in the auto_ptr
    /// given as argument. An event will be generated to that field.
    template< class F >
    inline void route( auto_ptr< F > &f, int id = 0 ) { 
      route( f.get(), id ); }

    /// Set up a route from this field to the field residing in the auto_ptr
    /// given as argument. No event will be generated to that field.
    template< class F >
    inline void routeNoEvent( auto_ptr< F > &f, int id = 0 ) { 
      routeNoEvent( f.get(), id ); 
    }

    /// Remove the route from this field to the field residing in the auto_ptr
    /// given as argument.
    template< class F >
    void unroute( auto_ptr< F > &f ) { unroute( f.get() ); }

    /// Remove all the routes from this field.
    inline void unrouteAll() {
      while( routes_out.begin() != routes_out.end() ) {
        unroute( *routes_out.begin() );
      }
    }

    /// Check that the field is up-to-date, if not update() is called 
    /// to make it up-to-date.
    virtual void upToDate();
    
    /// Returns true if the field is up to date, i.e. it has not pending 
    /// events.
    inline bool isUpToDate() {
      return event.ptr == NULL;
    }

    /// Generates an event from this field.
    virtual void touch();

    /// Get the Node that contains this field.
    inline Node *getOwner() {
      return owner;
    }

    /// Set the Node that contains this field.
    void setOwner( Node *n );

    /// Set the access type of the field.
    inline void setAccessType( AccessType _access_type ) {
      access_type = _access_type;
    }

    /// Get the access type of the field.
    inline AccessType getAccessType() {
      return access_type;
    }

    typedef vector< Field * > FieldSet;
    typedef vector< Field * > FieldVector;

    /// Returns true if this field is routed to the field given
    /// as argument.
    inline bool routesTo( Field *f ) {
      for( FieldSet::const_iterator i = routes_out.begin();
           i != routes_out.end(); ++i ) {
        if( (*i) == f ) return true;
      }
      return false;
    }

    /// Returns true if the field given as argument is routed to
    /// this field.
    inline bool hasRouteFrom( Field *f ) {
      for( FieldVector::const_iterator i = routes_in.begin();
           i != routes_in.end(); ++i ) {
        if( (*i) == f ) return true;
      }
      return false;
    }

    /// Get the Fields that are routed to this Field.
    inline const FieldVector &getRoutesIn() {
      return routes_in;
    }
    
    /// Get the Fields this Field is routed to.
    inline const FieldSet &getRoutesOut() {
      return routes_out;
    }

    /// Get the latest event.
    inline const Event &getLatestEvent() {
      return event;
    }

    /// Set access type checking on or off. Access types is checked
    /// when getting/setting values and setting up routes. 
    inline void setAccessCheck( bool on ) {
      access_check_on = on;
    }

    /// Returns true if checking of access types is on.
    inline bool isAccessCheckOn() {
      return access_check_on;
    }

  protected:
    /// Start a new event from this field. Any pending events from incoming
    /// routes will be removed, so that the current value is the value of
    /// the field.
    virtual void startEvent();

    /// Propagates an event to all Fields we are routed to.
    virtual void propagateEvent( Event e );
    
    /// Set up a route to this field from the field given as argument.
    virtual void routeFrom( Field*, int id );

    /// Remove the route to this field from the field given as argument.
    virtual void unrouteFrom( Field* );

    /// Replace the i:th route to this field with the field given as argument.
    virtual Field *replaceRouteFrom( Field* f, unsigned int i, int id );

    /// This function will be called to update the field if it has a 
    /// pending event when the upToDate() function is called.
    virtual void update() {}

    /// Check the access type is correct for performing a route from
    /// this field to f.
    /// \throws FieldAccessError if the field has the wrong access type.
    void checkAccessTypeRoute( Field *f, int id );

    /// Check the access type is correct for performing a route from
    /// f to this field.
    /// \throws FieldAccessError if the field has the wrong access type.
    void checkAccessTypeRouteFrom( Field *f, int id );

    /// Check the access type is valid for getting the value of a field.
    /// \throws FieldAccessError if the field has the wrong access type.
    void checkAccessTypeGet( int id );

    /// Check the access type is valid for changing the value of a field.
    /// \throws FieldAccessError if the field has the wrong access type.
    void checkAccessTypeSet( int id );

    /// Gets the id of the owner node, -1 if no owner.
    int ownerId();

    /// The name of the field.
    string name;

    /// If true, stops events from being propagated. This is used to prevent
    /// event propagation to go around in circles in an infinite loop.
    bool event_lock;

    /// If true, stops the update() function from being called in the
    /// upToDate() function.
    bool update_lock;

    /// The Fields that this field is routed to.
    FieldSet routes_out;
    /// The Field that are routed to this field.
    FieldVector routes_in;
    /// The last event that happened.
    Event event;
    /// The node that contain this field. NULL if it is a stand alone
    /// field.
    Node *owner;
    /// The type of access to the field.
    AccessType access_type;
    /// If true, access type checks will be done when getting/setting
    /// values and setting up routes. If false, everything will be allowed.
    bool access_check_on;

    friend class Scene;

    /// Returns true if this field is part of program settings fields
   /// in scene.
    bool isProgramSetting() {
      return is_program_setting;
    }

    /// Set it this field is used by program settings fields in scene
    /// or not.
    void markProgramSetting( bool used_as_setting ) {
      is_program_setting = used_as_setting;
    }

    bool is_program_setting;
  };

  /// This is a field which value can be set by a string from the X3D 
  /// parser. All ParsableFields values can be specified in X3D files.
  class H3DAPI_API ParsableField: public Field {
  public:
    /// Set the value of the field given a string. By default
    /// we try to parse the values according to the X3D/XML 
    /// specification.
    virtual void setValueFromString( const string &s ) = 0;
    /// Get the value of the field as a string. If the field contains
    /// multiple values the separator string is used between the values.
    inline virtual string getValueAsString( const string& separator = " " ) {
      return "";
    }
  };

  /// This is a field which value can be set by a string from the X3D 
  /// parser. All ParsableFields values can be specified in X3D files.
  class H3DAPI_API ParsableMField: public ParsableField {
  public:
    /// Set the value of the field given a string. By default
    /// we try to parse the values according to the X3D/XML 
    /// specification.
    virtual size_t getSize( ) = 0;
    /// Get the value of the field as a string. If the field contains
    /// multiple values the separator string is used between the values.
    inline virtual string getElementAsString( size_t element ) {
      return "";
    }
    /// Add a new element to an MField from a string value.
    inline virtual void addElementFromString( const string &s ) = 0;
  };


}

#endif

