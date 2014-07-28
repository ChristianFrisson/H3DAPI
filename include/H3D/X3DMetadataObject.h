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
/// \file X3DMetadataObject.h
/// \brief Header file for X3DMetadataObject, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __X3DMETADATAOBJECT_H__
#define __X3DMETADATAOBJECT_H__

#include <H3D/SFString.h>

namespace H3D {

  /// \ingroup AbstractNodes
  /// \class X3DMetadataObject
  /// \brief This abstract interface is the basis for all metadata nodes. 
  /// The interface is inherited by all metadata nodes. The specification
  /// of the reference field is optional. If provided, it identifies the 
  /// metadata standard or other specification that defines the name field. 
  /// If the reference field is not provided or is empty, the meaning of
  /// the name field is considered implicit to the characters in the string.
  class H3DAPI_API X3DMetadataObject {
  public:
    /// Constructor.
    X3DMetadataObject( Inst< SFString > _name = 0,
                       Inst< SFString > _reference = 0 );

    /// Force polymorphic 
    /// E.g., to allow dynamic_cast to concrete metadata types
    virtual ~X3DMetadataObject () {}

    /// Metadata name.
    ///
    /// <b>Access type:</b> inputOutput
    auto_ptr< SFString > nameF;

    /// The specification of the reference field is optional. 
    /// If provided, it identifies the metadata standard or other 
    /// specification that defines the name field. 
    ///
    /// <b>Access type:</b> inputOutput
    auto_ptr< SFString > reference;
    
    /// Given a name, the first X3DMetadataObject that matches that name is returned.
    /// If no such X3DMetadataObject exists, NULL is returned.
    /// \param name The name of the X3DMetadataObject to get.
    /// \returns The first X3DMetadataObject in the metadata field that matches that name if it exists, NULL otherwise
    virtual X3DMetadataObject *getMetadataByName( const string &name );
  };
}

#endif
