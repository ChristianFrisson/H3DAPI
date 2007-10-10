//////////////////////////////////////////////////////////////////////////////
//    Copyright 2004-2007, SenseGraphics AB
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
/// \file MetadataInteger.h
/// \brief Header file for MetadataInteger, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __METADATAINTEGER_H__
#define __METADATAINTEGER_H__

#include <H3D/X3DMetadataObject.h>
#include <H3D/X3DNode.h>
#include <H3D/MFInt32.h>

namespace H3D {

  /// \class MetadataInteger
  /// \brief  The metadata provided by this node is contained in the 
  /// integers of the value field.
  class H3DAPI_API MetadataInteger: 
    public X3DNode,
    public X3DMetadataObject {
  public:

    typedef TypedSFNodeObject< X3DMetadataObject > SFMetadataNode;

    /// Constructor.
    MetadataInteger( Inst< SFString       > _name      = 0,
                     Inst< SFString       > _reference = 0,
                     Inst< SFMetadataNode > _metadata  = 0,
                     Inst< MFInt32       > _value     = 0);

    
    /// Returns the default xml containerField attribute value.
    /// For this node it is "metadata".
    ///
    virtual string defaultXMLContainerField() {
      return "metadata";
    }

    /// X3DMetadataObject with more meta data.
    /// 
    /// <b>Access type:</b> inputOutput
    auto_ptr< SFMetadataNode > metadata;

    /// The metadata integer values.
    ///
    /// <b>Access type:</b> inputOutput
    auto_ptr< MFInt32 > value;
    
    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  };
}

#endif
