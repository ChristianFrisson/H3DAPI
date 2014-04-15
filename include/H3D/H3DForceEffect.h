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
/// \file H3DForceEffect.h
/// \brief Header file for H3DForceEffect
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __H3DFORCEEFFECT_H__
#define __H3DFORCEEFFECT_H__

#include <H3D/X3DChildNode.h>
#include <H3D/MFInt32.h>

namespace H3D {

  /// \ingroup Abstract nodes 
  /// \brief The base class for scenegraph force effects. 
  class H3DAPI_API H3DForceEffect: public X3DChildNode {
  public:
    /// Constructor
    H3DForceEffect( Inst< SFNode> _metadata = 0,
                    Inst< MFInt32 > _deviceIndex = 0 );

    /// A list of indices that tells which haptics devices to render
    /// the force on. If empty then the force will be rendered on all
    /// devices. Indices with values equal to or above the total number
    /// of haptics devices in the scene will be ignored. Values below
    /// 0 will be ignored.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> empty \n
    auto_ptr< MFInt32 > deviceIndex;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  };
}

#endif
