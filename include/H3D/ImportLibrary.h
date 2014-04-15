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
/// \file ImportLibrary.h
/// \brief Header file for ImportLibrary, H3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////

#ifndef __IMPORTLIBRARY_H__
#define __IMPORTLIBRARY_H__

#include <H3D/X3DChildNode.h>
#include <H3D/X3DUrlObject.h>
#include <H3D/MFString.h>

namespace H3D {
  /// \ingroup H3DNodes 
  /// \class ImportLibrary
  /// The ImportLibrary node makes it possible from a X3D file to load
  /// a dynamically linked library in order to e.g. access nodes defined
  /// in that library. This is very useful when creating specific toolkits
  /// for different areas such as volume rendering nodes etc. The X3D file
  /// just needs to use the ImportLibrary node to import it and after that
  /// the nodes defined in the toolkit can be used in the X3D file.
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/ImportLibrary.x3d">ImportLibrary.x3d</a>
  ///     ( <a href="examples/ImportLibrary.x3d.html">Source</a> )
  class H3DAPI_API ImportLibrary : public X3DChildNode,
                                   public X3DUrlObject {
  public:
    /// Constructor.
    ImportLibrary( Inst< SFNode >  _metadata = 0,
                   Inst< MFString > _url = 0 );

    /// Initialize loads all the dynamic libraries specified in the library
    /// field.
    virtual void initialize();

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  };
}

#endif
