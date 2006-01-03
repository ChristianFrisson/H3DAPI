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
/// \file ProtoDeclaration.h
/// \brief Header file for ProtoDeclaration.
///
//
//////////////////////////////////////////////////////////////////////////////

#ifndef __PROTODECLARATION_H__
#define __PROTODECLARATION_H__

#include "X3DPrototypeInstance.h"
#include "PrototypeInstance.h"
#include <list>

namespace H3D {

  /// The ProtoDeclaration class containse information about a PROTO node from
  /// the X3D standard. It contains everything that is needed to now about the prototype
  /// and allows you to create new instances of the prototype with the newProtoInstance 
  /// function.
  class H3DAPI_API ProtoDeclaration {
  public:
    /// Class that contains information about a field that is part of the prototype
    /// declaration.
    class H3DAPI_API FieldDeclaration {
    public:
      FieldDeclaration( const string &_name = "",
                        const X3DTypes::X3DType &_type = X3DTypes::UNKNOWN_X3D_TYPE,
                        const Field::AccessType &_access_type = Field::INPUT_OUTPUT,
                        const string &_value = "" ) :
        name( _name ),
        type( _type ),
        access_type( _access_type ),
        value( _value ) {

      }

      string name;
      X3DTypes::X3DType type;
      Field::AccessType access_type;
      string value;
    };

    /// Constructor.
    /// \param The name of the prototype.
    /// \param String with the internal scenegraph of the prototype.
    ProtoDeclaration( const string &_name,
                      const string &_body = "" ) :
      name( _name ),
      body( _body ){}

    /// Get the string with the internal scenegraph of the prototype.
    const string &getProtoBody() {
      return body;
    }

    /// Set the string with the internal scenegraph of the prototype.
    void setProtoBody( const string &_body ) {
      body = _body;
    }

    /// Get the name of the prototype.
    const string& getName() {
      return name;
    }

    /// Get the name of the prototype.
    void setName( const string &_name ) {
      name = _name;
    }

    /// Add a field to the prototype.
    void addFieldDeclaration( const string &name,
                              const X3DTypes::X3DType &type,
                              const Field::AccessType &access_type,
                              const string &value = ""  ) {
      field_declarations.push_back( FieldDeclaration( name, type, access_type, value ) );
    }

    /// Get the field declaration of the given name.
    /// Returns true if such a field declaration exists, false otherwise
    FieldDeclaration *getFieldDeclaration( const string& name ) {
      for( list< FieldDeclaration >::iterator i = field_declarations.begin();
           i != field_declarations.end(); i++ ) {
        if( (*i).name == name ) {
          return &(*i);
        }
      }
      return NULL;
    }
    
    /// Create a new X3DPrototypeInstance instance using the ProtoDeclaration. 
    X3DPrototypeInstance *newProtoInstance();
  protected:
    string name;
    string body;
    std::list< FieldDeclaration > field_declarations;
  };

}

#endif
