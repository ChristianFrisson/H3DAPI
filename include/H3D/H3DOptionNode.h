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
/// \file H3DOptionNode.h
/// \brief Header file for H3DOptionNode.
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __H3DOPTIONNODE_H__
#define __H3DOPTIONNODE_H__

#include <H3D/X3DNode.h>

namespace H3D {

  /// \ingroup AbstractNodes
  /// \class H3DOptionNode
  /// \brief This abstract node type indicates that the concrete nodes which 
  /// are instantiated based on it may be used in options fields.
  ///
  class H3DAPI_API H3DOptionNode : public X3DNode {
  public:
    
    /// Constructor.
    H3DOptionNode( Inst< SFNode>  _metadata = 0 );

    /// Returns the default xml containerField attribute value.
    /// For this node it is "options".
    virtual string defaultXMLContainerField() {
      return "options";
    }

    /// Field to collect update event from every option parameters,
    /// all inherited Option Node should route its option parameter fields to it
    auto_ptr< Field > updateOption;
  };
}

#endif
