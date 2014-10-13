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
/// \file X3DProgrammableShaderObject.h
/// \brief Header file for X3DProgrammableShaderObject.
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __X3DPROGRAMMABLESHADEROBJECT_H__
#define __X3DPROGRAMMABLESHADEROBJECT_H__

#include <H3D/H3DDynamicFieldsObject.h>
#include <H3D/RefCountMField.h>
#include <H3D/FieldTemplates.h>

namespace H3D {
  /// \ingroup AbstractInterface
  /// \class X3DProgrammableShaderObject
  /// This abstract node type is the base type for all node types that 
  /// specify arbitrary fields for interfacing with per-object attribute 
  /// values.
  ///
  /// A concrete X3DProgrammableShaderObject node instance is used to program
  /// behaviour for a shader in a scene. The shader is able to receive and
  /// process events that are sent to it. Each event that can be received shall
  /// be declared in the shader node using the same field syntax as is used in
  /// a prototype definition: 
  ///
  /// inputOnly type name
  ///
  /// The type can be any of the standard X3D fields. Name shall be an 
  /// identifier that is unique for this shader node and is used to map
  /// the value to the shader program's uniform variable of the same name. If
  /// a shader program does not have a matching uniform variable, the field
  /// value is ignored. 
  ///
  /// outputOnly fields are not required to generate output events from a 
  /// shader. Current hardware shader technology does not support this
  /// capability, though future versions may.
  ///
  /// It is recommended that user-defined field or event names defined in 
  /// shader nodes follow the naming conventions described in ISO/IEC 19775-2.
  class H3DAPI_API X3DProgrammableShaderObject: 
    public H3DDynamicFieldsObject {
  public:
    /// Constructor. 
    X3DProgrammableShaderObject( H3DNodeDatabase *_database );

    /// Add a shader field uniform
    virtual bool addField( const string &name,
                           const Field::AccessType &access,
                           Field *field );
    
    /// Remove a shader field uniform
    virtual bool removeField ( const string& _name );

    /// Remove all dynamic fields that have been previously added.
    virtual void clearFields();

    /// Destructor.
    virtual ~X3DProgrammableShaderObject();

    /// True if we should use bindless textures
    static bool use_bindless_textures;

  protected:

    /// For bindless textures handle the updating of the
    /// textures list of all shader fields that contain the texture
    class UpdateTextures : public AutoUpdate< Field > {
    public:
      virtual ~UpdateTextures ();
      void clearField ( Field& _field );

    protected:
      virtual void update();

      typedef std::map < Field*, NodeVector > FieldToNodes;
      FieldToNodes fields_to_textures;
    };

    auto_ptr < UpdateTextures > update_textures;

    /// True if use_bindless_textures has already be set once
    static bool use_bindless_textures_set;
  };
}

#endif
