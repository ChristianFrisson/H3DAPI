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
/// \file DefaultAppearance.h
/// \brief Header file for DefaultAppearance.
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __DEFAULTAPPEARANCE_H__
#define __DEFAULTAPPEARANCE_H__

#include <H3D/H3DOptionNode.h>
#include <H3D/Appearance.h>
#include <H3D/DependentNodeFields.h>

namespace H3D {

  /// \ingroup H3DNodes
  /// \class DefaultAppearance
  /// \brief Specifies the default appearance parameters to use for
  /// graphical rendering. The appearance parameters set here will be 
  /// the default parameters that are used if a part is not specified
  /// in the appearance field of a Shape node.
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/DefaultAppearance.x3d">DefaultAppearance.x3d</a>
  ///     ( <a href="examples/DefaultAppearance.x3d.html">Source</a> )
  class H3DAPI_API DefaultAppearance : public H3DOptionNode {
  public:
    /// SFAppearanceNode is dependent on the displayList field of its
    /// encapsulated X3DAppearanceNode node, i.e. an event from that
    /// field will trigger an event from the SFAppearanceNode as well.
    typedef DependentSFNode< Appearance,
                             FieldRef<H3DDisplayListObject, 
                                      H3DDisplayListObject::DisplayList,
                                      &H3DDisplayListObject::displayList >, 
                             true > 
    SFAppearance;

    
    /// Constructor.
    DefaultAppearance( Inst< SFNode  > _metadata = 0,
                       Inst< SFAppearance > _defaultAppearance = 0 );
    
    /// The default appearance node.
    ///
    /// <b>Default value: </b> NULL \n
    /// <b>Access type: </b> inputOutput \n
    auto_ptr< SFAppearance > defaultAppearance;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;

  protected:
    
  };
}

#endif
