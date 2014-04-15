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
/// \file H3DNodeDatabase.cpp
/// \brief CPP file for H3DNodeDatabase
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/H3DNodeDatabase.h>
#include <H3D/Node.h>

using namespace H3D;

H3DNodeDatabase::H3DNodeDatabaseType *H3DNodeDatabase::database = 
H3DNodeDatabase::database;

bool H3DNodeDatabase::initialized = false;

H3DNodeDatabase::H3DNodeDatabase( const string &_name, 
                                  H3DCreateNodeFunc _createf,
                                  const type_info &_ti,
                                  H3DNodeDatabase *_parent ) :
name( _name ), 
createf( _createf ),
ti( _ti ),
parent( _parent ),
database_allocated_dynamic( false ) {
  if (!initialized) {
    database = new H3DNodeDatabaseType;
    initialized = true;
  }
  (*database)[ ti ] = this;
}

H3DNodeDatabase::H3DNodeDatabase( const string &_name, 
                                  const string &_alias,
                                  H3DCreateNodeFunc _createf,
                                  const type_info &_ti,
                                  H3DNodeDatabase *_parent ) :
name( _name ), 
createf( _createf ),
ti( _ti ),
parent( _parent ),
database_allocated_dynamic( false ) {
  if (!initialized) {
    database = new H3DNodeDatabaseType;
    initialized = true;
  }
  (*database)[ ti ] = this;
  addAlias( _alias );
}



H3DNodeDatabase::H3DNodeDatabase( const type_info &_ti,
                                  H3DNodeDatabase *_parent ) :
name( "" ),
createf( NULL ),
ti( _ti ),
parent( _parent ),
database_allocated_dynamic( false ) {
  if (!initialized) {
    database = new H3DNodeDatabaseType;
    initialized = true;
  }
}

H3DNodeDatabase::~H3DNodeDatabase(void){
  
  for(FieldDBType::const_iterator i = fields.begin(); i !=fields.end(); ++i){
    delete (*i).second;
  }

  if(initialized) {
    // remove entry from global database
    if( !database_allocated_dynamic ) {
      for( H3DNodeDatabaseType::iterator i = database->begin();
           i != database->end(); ++i ) {
        if( (*i).second == this ) {
          database->erase( i );
          break;
        }
      }
    }

    for( H3DNodeInstanceDatabase::iterator i = node_instance_database.begin();
         i != node_instance_database.end(); ++i ) {
      (*i).second->parent = NULL;
    }
    node_instance_database.clear();

    if( database_allocated_dynamic ) {
      // Using lookupTypeID since we can't be sure that the parent variable is still alive.
      H3DNodeDatabase *parent_db = H3DNodeDatabase::lookupTypeId( ti );
      if( parent_db ) {
        for( H3DNodeInstanceDatabase::iterator i = parent_db->node_instance_database.begin();
             i != parent_db->node_instance_database.end(); ++i ) {
          if( (*i).second == this ) {
            parent_db->node_instance_database.erase( i );
            break;
          }
        }
      }
    }

    // remove static database if last entry.
    if( database->empty() ){
      delete database;
      database = 0;
      initialized = false;
    }
  }
}


Node *H3DNodeDatabase::createNode( const string &name ) {
  H3DNodeDatabase *db = lookupName( name );
  if( !db || db->createf == NULL )
    return NULL;
  else
    return db->createf();
}

void H3DNodeDatabase::initFields( Node *n ) const {
  for( FieldDBType::const_iterator i = fields.begin(); i != fields.end(); ++i ) {
    FieldDBElement *fdb = (*i).second;
    if ( fdb ) {
      Field *f = fdb->getField( n );
      if( f ) { 
        f->setName( fdb->getName() );
        f->setOwner( n );
        f->setAccessType( fdb->getAccessType() );
      }
    }
  }
  if ( parent )
    parent->initFields( n );
}


Field *H3DNodeDatabase::getFieldHelp( const Node *n, const string &f ) const {
  for( FieldDBType::const_iterator i = fields.begin(); i != fields.end(); ++i ) {
    FieldDBElement *fdb = (*i).second;
    const string &name = (*i).first;
    if ( name == f )
      return fdb->getField( n );
  }
  if ( parent )
    return parent->getField( n, f );
  else
    return NULL;
}

Field *H3DNodeDatabase::getField( const Node *n, const string &name ) const {
  Field *f = getFieldHelp( n, name );
  if( f ) return f;
  
  // could not find the field with the given name. If the name starts with
  // "set_" try to remove that prefix.
  if( name.size() > 4 && name.substr( 0, 4 ) == "set_" ) {
    f = getFieldHelp( n, name.substr( 4, name.size() - 4 ) );
    if( f && f->getAccessType() == Field::INPUT_OUTPUT ) return f;
  }

  if( name.size() > 8 && name.substr( name.size() - 8, 8 ) == "_changed" ) {
    f = getFieldHelp( n, name.substr( 0, name.size() - 8 ) );
    if( f && f->getAccessType() == Field::INPUT_OUTPUT ) return f;
  }
  
  return NULL;
}

void H3DNodeDatabase::addField( FieldDBElement *f ) {
  string tmp_name = f->getName();
  if( database_allocated_dynamic ) {
    FieldDBConstIterator i = fieldDBBegin();
    for( ; fieldDBEnd() != i; ++i )
      if( tmp_name == (*i) ) break;
    if( i == fieldDBEnd() ) fields[tmp_name] = f;
  } else {
    // In the case of static intialized databases the lower-most node in
    // an inheritance hierarchy decides how a field should look. Therefore we
    // only check its own fields. The other reason is also that
    // fieldDBBegin() can't be used in the static initialization sequence
    // because of how the parent is referenced.
    if( fields.find( tmp_name ) == fields.end() ) fields[tmp_name] = f;
  }
}

H3DNodeDatabase *H3DNodeDatabase::lookupTypeId( const type_info &t ) {
  H3DNodeDatabaseType::iterator pos = database->find( t );
  if( pos == database->end() )
    return NULL;
  else
    return (*pos).second;
}

H3DNodeDatabase *H3DNodeDatabase::lookupName( const string &name ) {
   for( H3DNodeDatabaseType::iterator i = database->begin(); 
       i != database->end(); ++i ) {
    H3DNodeDatabase *n = (*i).second;
    // name is a match
    if ( n->name == name )
      return n;  

    // an alias is a match
    for( list< string >::iterator a = n->aliases.begin();
         a != n->aliases.end(); ++a ) {
      if( name == *a ) {
        return n;
      }
    }
  }
  return NULL; 
}

FieldDBElement::FieldDBElement( H3DNodeDatabase *_container,
                                const string &_name, 
                                const Field::AccessType _access ) :
container( _container ), name( _name ), access( _access ) {

}

bool H3DNodeDatabase::FieldDBConstIterator::operator==( 
                       FieldDBConstIterator iter ) {
  if( iter.status != status ) return false;
  
  if( status == END ) return true; 
  else if( status == LOCAL )
    return local_iterator == iter.local_iterator;
  else 
    // status = INHERITED
    return *inherited_iterator == *iter.inherited_iterator;
}

H3DNodeDatabase::FieldDBConstIterator &H3DNodeDatabase::FieldDBConstIterator::operator++() {
  if( status == END ) return *this;
  else if( status == LOCAL ) {
    ++local_iterator;
    if( local_iterator == ndb->fields.end() ) {
      if( ndb->parent  && ndb->parent->fieldDBBegin() != ndb->parent->fieldDBEnd() ) {
        *inherited_iterator = ndb->parent->fieldDBBegin();
        status = INHERITED;
      } else {
        status = END;
      }
    }
  } else {
    // status == INHERITED
    ++(*inherited_iterator);
     if( *inherited_iterator == ndb->parent->fieldDBEnd() )
      status = END;
  }
  return *this;
}


H3DNodeDatabase::FieldDBConstIterator::FieldDBConstIterator( 
                        H3DNodeDatabase * _ndb , bool is_end ):
  inherited_iterator( new FieldDBConstIterator ),
  ndb( _ndb ) {

  if( is_end ) {
    status = END;
  } else {
    local_iterator = ndb->fields.begin();
    if( local_iterator != ndb->fields.end() ) {
      status = LOCAL;
    } else {
      if( ndb->parent && ndb->parent->fieldDBBegin() != ndb->parent->fieldDBEnd() ) {
        *inherited_iterator = ndb->parent->fieldDBBegin();
        status = INHERITED;
      } else {
        status = END;
      }
    }
  }
}


void H3DNodeDatabase::clearDynamicFields() {
  for( FieldDBType::iterator i = fields.begin(); i != fields.end();  ) {
    DynamicFieldDBElement *fdb = 
      dynamic_cast< DynamicFieldDBElement * >( (*i).second );
    FieldDBType::iterator to_erase = i;
    ++i;
    if( fdb ) {
      fields.erase( to_erase );
      delete fdb;
    }
  }
}

void H3DNodeDatabase::clearDynamicFields( Node *n ) {
  for( FieldDBType::iterator i = fields.begin(); i != fields.end();  ) {
    DynamicFieldDBElement *fdb = 
      dynamic_cast< DynamicFieldDBElement * >( (*i).second );
    FieldDBType::iterator to_erase = i;
    ++i;
    // Only remove the entry if its contained field belongs to the
    // specified node.
    if( fdb && fdb->getField( n ) ) {
      fields.erase( to_erase );
      // Delete the DynamicFieldDBElement. It is not used anymore, and not
      // deleted anywhere else.
      delete fdb;
    } 
  }
}

H3DNodeDatabase::FieldDBConstIterator::FieldDBConstIterator( const FieldDBConstIterator &f ):
  status( f.status ),
  local_iterator( f.local_iterator ),
  inherited_iterator( new FieldDBConstIterator ),
  ndb( f.ndb ) {
  *inherited_iterator = *f.inherited_iterator;
}

H3DNodeDatabase::FieldDBConstIterator & H3DNodeDatabase::FieldDBConstIterator::operator=(const FieldDBConstIterator &f) {
  status = f.status;
  local_iterator = f.local_iterator;
  ndb = f.ndb;
  if( f.inherited_iterator.get() ) {
    if( inherited_iterator.get() == NULL ) 
      inherited_iterator.reset( new FieldDBConstIterator );
    *inherited_iterator = *f.inherited_iterator;
  } else {
    inherited_iterator.reset( NULL );
  }
  return *this;
}

H3DNodeDatabase::H3DNodeDatabase( const Node * n,
                                  H3DNodeDatabase *_parent ) :
name( _parent->name ),
createf( _parent->createf ),
ti( _parent->ti ),
parent( _parent ),
database_allocated_dynamic( true ) {
  if (!initialized) {
    database = new H3DNodeDatabaseType;
    initialized = true;
  }
  aliases = parent->aliases;
  parent->node_instance_database[n] = this;
}

H3DNodeDatabase *H3DNodeDatabase::lookupNodeInstance( const Node * n ) {
  H3DNodeDatabase *db_candidate = H3DNodeDatabase::lookupTypeId( typeid( *n ) );
  if( db_candidate ) {
    H3DNodeInstanceDatabase::iterator pos = db_candidate->node_instance_database.find( n );
    if( pos != db_candidate->node_instance_database.end() )
      return (*pos).second;
  }
  return db_candidate;
}
