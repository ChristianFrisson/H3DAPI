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
/// \file ProtoDeclaration.h
/// \brief Header file for ProtoDeclaration.
///
//
//////////////////////////////////////////////////////////////////////////////

#ifndef __PROTODECLARATION_H__
#define __PROTODECLARATION_H__

#include <H3D/X3DPrototypeInstance.h>
#include <H3D/PrototypeInstance.h>
#include <H3D/DEFNodes.h>
#include <list>

namespace H3D {
  namespace X3D {
    class PrototypeVector;
  }

  /// The ProtoDeclaration class contains information about a PROTO node from
  /// the X3D standard. It contains everything that is needed to now about the prototype
  /// and allows you to create new instances of the prototype with the newProtoInstance 
  /// function.
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/Prototype.x3d">Prototype.x3d</a>
  ///     ( <a href="examples/Prototype.x3d.html">Source</a> )
  ///   - <a href="../../../H3DAPI/examples/All/Surfaces.x3d">Surfaces.x3d</a>
  ///     ( <a href="examples/Surfaces.x3d.html">Source</a> )
  class H3DAPI_API ProtoDeclaration : public RefCountedClass {
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
        value( _value ),
        external( false ) {

      }

      string name;
      X3DTypes::X3DType type;
      Field::AccessType access_type;
      string value;
      bool external;
    };

    /// Constructor.
    /// \param _name The name of the prototype.
    /// \param _body String with the internal scenegraph of the prototype.
    /// \param _body_extra Contains extra proto bodies that are not part of the
    /// main proto body.
    /// \param _existing_protos Contains all the ProtoDeclarations that exists when this
    /// ProtoDeclaration is created.
    ProtoDeclaration( const string &_name,
                      const string &_body = "",
                      const vector<string > &_body_extra = vector<string>(),
                      X3D::PrototypeVector *_existing_protos = NULL );

    // destructor
    virtual ~ProtoDeclaration();

    /// Get the string with the internal scenegraph of the prototype.
    const string &getProtoBody() {
      return body;
    }

    /// Set the string with the internal scenegraph of the prototype.
    void setProtoBody( const string &_body ) {
      body = _body;
    }

    /// Add one part of proto body that is not the main proto
    /// body(i.e. not the first )
    void addProtoBodyExtra( const string &_body ) {
      body_extra.push_back( _body );
    }
    
    /// Get the proto body extras.
    const vector<string> & getProtoBodyExtra( ) {
      return body_extra;
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
    /// \param name The name of the field.
    /// \param type The type of the field.
    /// \param access_type The access_type of the field.
    /// \param value The value of the field.
    /// \returns An empty string if adding the field succeeded otherwise the
    /// string is an error message.
    string addFieldDeclaration( const string &name,
                                const X3DTypes::X3DType &type,
                                const Field::AccessType &access_type,
                                const string &value = ""  ) {
      FieldDeclaration *field_declaration = getFieldDeclaration( name );
      if( field_declaration ) {
        if( field_declaration->external ) {
          string error_message = "";
          if( field_declaration->type != type )
            error_message =
              "Warning: Value for \"type\" attribute of \"field\" element with name"
              + name + " does not match the value in found ExternProto. Attribute is ignored.";
          if( error_message == "" &&
              field_declaration->access_type != access_type )
            error_message =
              "Warning: Value for \"access_type\" attribute of \"field\" element with name"
              + name + " does not match the value in found ExternProto. Attribute is ignored.";
          if( value != "" )
            field_declaration->value = "";
          // No need to add field declaration. Just indicate that it is no
          // longer defined in external proto.
          field_declaration->external = false;
          return error_message;
        } else {
          return "\"field\" element with name " + name
                 + " already exists. The field will not be added.";
        }
      }
      field_declarations.push_back( FieldDeclaration( name, type, access_type, value ) );
      return "";
    }

    /// Get the field declaration of the given name.
    /// Returns true if such a field declaration exists, false otherwise
    FieldDeclaration *getFieldDeclaration( const string& name ) {
      for( list< FieldDeclaration >::iterator i = field_declarations.begin();
           i != field_declarations.end(); ++i ) {
        if( (*i).name == name ) {
          return &(*i);
        }
      }
      return NULL;
    }

    /// Call this to set the external property for all contained field
    /// declarations. The external property should be set if this proto
    /// was created in an ExternalProto and the property is used to know
    /// how default values should be set.
    void setFieldDeclarationsExternal( bool is_external ) {
      for( list< FieldDeclaration >::iterator i = field_declarations.begin();
           i != field_declarations.end(); ++i ) {
        (*i).external = is_external;
      }
    }
    
    /// Create a new X3DPrototypeInstance instance using the ProtoDeclaration.
    X3DPrototypeInstance *newProtoInstance();
  protected:
    string name;
    // The main body string, i.e. the first node in the proto body.
    string body;

    // The extra body string, i.e. all nodes on the same level 
    // as the main body but defined after it.
    vector< string > body_extra; 
    std::list< FieldDeclaration > field_declarations;

    // Contains ProtoDeclarations that existed when this protodeclaration was
    // created.
    X3D::PrototypeVector *existing_protos;

    AutoRef< Node > createProtoInstanceNodeX3D( PrototypeInstance *proto,
                                                X3D::DEFNodes *dn,
                                                const string &body_string );

    AutoRef< Node > createProtoInstanceNodeVRML( PrototypeInstance *proto,
                                                 X3D::DEFNodes *dn,
                                                 const string &body_string );
  };

}

#endif
