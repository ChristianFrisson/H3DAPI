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
/// \file MetadataSet.h
/// \brief Header file for MetadataSet, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __METADATASET_H__
#define __METADATASET_H__

#include <H3D/X3DMetadataObject.h>
#include <H3D/X3DNode.h>
#include <H3D/MFNode.h>

namespace H3D {

  /// \class MetadataSet
  /// \brief  The metadata provided by this node is contained in 
  /// the metadata nodes of the value field.
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/MetadataSet.x3d">MetadataSet.x3d</a>
  ///     ( <a href="examples/MetadataSet.x3d.html">Source</a> )
  class H3DAPI_API MetadataSet: 
    public X3DNode,
    public X3DMetadataObject {
  public:

    typedef TypedSFNodeObject< X3DMetadataObject > SFMetadataNode;
    typedef TypedMFNodeObject< X3DMetadataObject > MFMetadataNode;

    /// Constructor.
    MetadataSet( Inst< SFString       > _name      = 0,
                 Inst< SFString       > _reference = 0,
                 Inst< SFMetadataNode > _metadata  = 0,
                 Inst< MFMetadataNode > _value     = 0);
    
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

    /// The metadata X3DMetadataObjects.
    ///
    /// <b>Access type:</b> inputOutput
    auto_ptr< MFMetadataNode > value;
    
    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;

    /// Given a name, the first X3DMetadataObject that matches that name is returned.
    /// If no such X3DMetadataObject exists, NULL is returned.
    /// \param name The name of the X3DMetadataObject to get.
    /// \returns The first X3DMetadataObject in the metadata field that matches that name if it exists, NULL otherwise
    virtual X3DMetadataObject *getMetadataByName( const string &name );
  };
}

#endif
