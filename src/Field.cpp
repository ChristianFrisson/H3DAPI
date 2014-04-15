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
/// \file Field.cpp
/// \brief CPP file for the Field class.
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/Field.h>
#include <H3D/Node.h>
#include <H3D/Scene.h>
#include <algorithm>

#ifdef DEBUG
#include <iostream>
using namespace std;
#endif

using namespace H3D;

Field::Field( ) : 
  name( "" ),
  event( 0, 0 ),
  owner( NULL ),
  access_type( INPUT_OUTPUT ),
  access_check_on( true ),
  is_program_setting( false ) {
#ifdef DEBUG
  Console(1) << "Field(" << getFullName() << ")::Field()" << endl;
#endif
  update_lock = false;
  event_lock = false;
}

Field::~Field() {
  if( is_program_setting )
    Scene::removeProgramSetting( this );

#ifdef DEBUG
  Console(1) << "Field(" << getFullName() << ")::~Field()" << endl;
#endif
  // remove all routes
  while( routes_out.begin() != routes_out.end() ) {
    unroute( *routes_out.begin() );
  }

  // remove all routes
  while( routes_in.begin() != routes_in.end() ) {
    (*routes_in.begin())->unroute( this );
  }

  
}

//  Route management:
void Field::route( Field *f, int id ) {
  
#ifdef DEBUG
  Console(1) << "Field(" << getFullName() << ")::route()" << endl;
#endif
  // check that we have the correct access type
  checkAccessTypeRoute( f, id );

  // only call routeFrom if the route is a new route and
  // not already in routes_out 
  FieldSet::iterator i = std::find( routes_out.begin(), routes_out.end(), f );
  if( i == routes_out.end() ) {
    routes_out.push_back( f );
    try {
      f->routeFrom( this, id );
    } catch( ... ) {
      //route failed, unroute and throw
      i = std::find( routes_out.begin(), routes_out.end(), f );
      routes_out.erase( i );
      throw;
    }
    
    // create new event, with a new timestamp
    event.time_stamp = TimeStamp::now();
    Event e( this, event.time_stamp );
    event_lock = true;
    f->propagateEvent( e );
    event_lock = false;
  }
}

void Field::checkAccessTypeRoute( Field *f, int id ) {
  if( access_check_on && owner && owner->id != id ) {
    if( access_type == INPUT_ONLY ) {
      stringstream s;
      s << "Trying to set up a route from INPUT_ONLY field " 
        << getFullName() << " to the the field " 
        << f->getFullName();
      throw FieldAccessError( s.str() ); 
    }
  }
}

void Field::checkAccessTypeRouteFrom( Field *f, int id ) {
  if( access_check_on && owner && owner->id != id ) {
    if( access_type == INITIALIZE_ONLY ) {
      stringstream s;
      s << "Trying to set up a route from " << f->getFullName()
        << " to the INITIALIZE_ONLY field " 
        << getFullName();
      throw FieldAccessError( s.str() ); 
    } else if( access_type == OUTPUT_ONLY ) {
      stringstream s;
      s << "Trying to set up a route from " << f->getFullName()
        << " to the OUTPUT_ONLY field " 
        << getFullName();
      throw FieldAccessError( s.str() ); 
    }
  }
}

void Field::checkAccessTypeGet( int id ) {
  if( access_check_on && owner ) {
    // TODO: the check for routes out makes it possible to get the 
    // value of the INPUT_ONLY field if a route has been successfully
    // set up from it. This is needed in many cases. However it also 
    // allows a user to get the value from it even if they should not
    // be able to.
    if( access_type == INPUT_ONLY && 
        owner->id != id &&
        routes_out.empty() ) {
      stringstream s;
      s << "Trying to get the value of INPUT_ONLY field " 
        << getFullName() << " from outside the Node that contain it. ";
      throw Field::FieldAccessError( s.str() ); 
    } 
  }
}

void Field::checkAccessTypeSet( int id ) {
  if( access_check_on && owner ) {
    if( access_type == INITIALIZE_ONLY ) {
      if( owner->isInitialized() ) {
        stringstream s;
        s << "Trying to set the INITIALIZE_ONLY field " 
          << getFullName() << " after initialization. ";
        throw Field::FieldAccessError( s.str() ); 
      } 
    } else if( access_type == OUTPUT_ONLY ) {
      if( owner->id != id ) {
        stringstream s;
        s << "Trying to set the OUTPUT_ONLY field " 
          << getFullName() << " from outside the Node that contain it. ";
        throw Field::FieldAccessError( s.str() ); 
      } 
    }
  }
}

void Field::routeNoEvent( Field *f, int id ) {
#ifdef DEBUG
  Console(1) << "Field(" << getFullName() << ")::routeNoEvent()" << endl;
#endif
  // check that we have the correct access type
  checkAccessTypeRoute( f, id );
    
  // only call routeFrom if the route is a new route and
  // not already in routes_out
  FieldSet::iterator i = std::find( routes_out.begin(), routes_out.end(), f );
  
  if( i == routes_out.end() ) {
    routes_out.push_back( f );
    f->routeFrom( this, id );
  }
}

void Field::routeFrom( Field *f, int id ) {
#ifdef DEBUG
  Console(1) << "Field(" << getFullName() << ")::routeFrom()" << endl;
#endif
  // check that we have the correct access type
  checkAccessTypeRouteFrom( f, id );

  checkFieldType( f, (int)this->routes_in.size() );
  routes_in.push_back( f );
}


//  Route management:
Field * Field::replaceRoute( Field *f, unsigned int i, int id ) {
#ifdef DEBUG
  Console(1) << "Field(" << getFullName() << ")::route()" << endl;
#endif
  // check that we have the correct access type
  checkAccessTypeRoute( f, id );

  // only call routeFrom if the route is a new route and
  // not already in routes_out 
  FieldSet::iterator f_i = std::find( routes_out.begin(), routes_out.end(), f );
  if( f_i == routes_out.end() ) {
    routes_out.push_back( f );
    Field *replaced_field = f->replaceRouteFrom( this, i, id );

    // create new event, with a new timestamp
    event.time_stamp = TimeStamp::now();
    Event e( this, event.time_stamp );
    event_lock = true;
    f->propagateEvent( e );
    event_lock = false;
    return replaced_field;
  }
  return NULL;
}

Field * Field::replaceRouteNoEvent( Field *f, unsigned int i, int id ) {
#ifdef DEBUG
  Console(1) << "Field(" << getFullName() << ")::route()" << endl;
#endif
  // check that we have the correct access type
  checkAccessTypeRoute( f, id );

  // only call routeFrom if the route is a new route and
  // not already in routes_out 
  FieldSet::iterator f_i = std::find( routes_out.begin(), routes_out.end(), f );
  if( f_i == routes_out.end() ) {
    routes_out.push_back( f );
    Field *replaced_field = f->replaceRouteFrom( this, i, id );
    return replaced_field;
  }
  return NULL;
}

Field* Field::replaceRouteFrom( Field *f, unsigned int i, int id ) {
#ifdef DEBUG
  Console(1) << "Field(" << getFullName() << ")::routeFrom()" << endl;
#endif
  // check that we have the correct access type
  checkAccessTypeRouteFrom( f, id );
  
  checkFieldType( f, i );
  Field *old_value = routes_in[i];
  FieldSet::iterator f_i = std::find( old_value->routes_out.begin(), old_value->routes_out.end(), this );
  if( f_i != old_value->routes_out.end() ) {
    old_value->routes_out.erase( f_i );
  }
  routes_in[i] = f;
  return old_value;
}

void Field::unroute( Field *f ) {
#ifdef DEBUG
  Console(1) << "Field(" << getFullName() << ")::unroute()" << endl;
#endif
  FieldSet::iterator i = std::find( routes_out.begin(), routes_out.end(), f );
  if( i != routes_out.end() ) {
    routes_out.erase( i );
  }
  f->unrouteFrom( this );
  // if we are unrouting from the node that sent us
  // an event, then we need to cancel that event.
  if ( event.ptr == f )
    event.ptr = 0;
}

void Field::unrouteFrom( Field *f ) {
#ifdef DEBUG
  Console(1) << "Field(" << getFullName() << ")::unrouteFrom()" << endl;
#endif
  FieldVector::iterator i = std::find( routes_in.begin(), routes_in.end(), f );
  if( i != routes_in.end() ) {
    routes_in.erase( i );
  }
  if ( f == event.ptr ) event.ptr = 0;
}


void Field::touch() {
#ifdef DEBUG
  Console(1) << "Field(" << getFullName() << ")::touch()" << endl;
#endif
  // create new event, with a new timestamp
  event.time_stamp = TimeStamp::now();
  Event e( this, event.time_stamp );

  event_lock = true;
  for( unsigned int i = 0; i < routes_out.size(); ++i ) {
    Field *f = routes_out[i];
    f->propagateEvent( e );
  }
  event_lock = false;
}

// Event management:
void Field::startEvent() {
#ifdef DEBUG
  Console(1) << "Field(" << getFullName() << ")::startEvent()" << endl;
#endif
  // create new event, with a new timestamp
  event.time_stamp = TimeStamp::now();
  event.ptr = 0;
  Event e( this, event.time_stamp );

  event_lock = true;
  for( unsigned int i = 0; i < routes_out.size(); ++i ) {
    Field *f = routes_out[i];
    f->propagateEvent( e );
  }
  event_lock = false;
}

void Field::propagateEvent( Event e ) {
#ifdef DEBUG
  Console(1) << "Field(" << getFullName() << ")::propagateEvent()" << endl;
#endif
  if ( !event_lock && /*!event.ptr && */ e.time_stamp > event.time_stamp ) {
    event.time_stamp = e.time_stamp;
    event.ptr = e.ptr;
    event_lock = true;
    Event newe( this, event.time_stamp );
    for( unsigned int i = 0; i < routes_out.size(); ++i ) {
      Field *f = routes_out[i];
      f->propagateEvent( newe );
    }
    event_lock = false;
  }
}

void Field::upToDate() {
#ifdef DEBUG
  Console(1) << "Field< " << getFullName() << ")::upToDate()   event_ptr = " 
       << event.ptr << endl;
#endif
  if ( event.ptr && !update_lock ) {
    update_lock = true;
    update();
    this->event.ptr = NULL;
    update_lock = false;
  }
}

void Field::setOwner( Node *n ) {
  owner = n;
}

string Field::getFullName() { 
  if( owner ) {
    if( name == "" )
      return string( "Unknown " ) + getTypeName() + " in " + owner->getName();
    else 
      return owner->getName() + "." + name;
  } else {
    return getName();
  }
} 

string Field::getName() { 
  if( name == "" )
    return string( "Unknown " ) + getTypeName();
  else 
    return name;
} 

int Field::ownerId() {
  if( owner ) return owner->id;
  else return -1;
}
