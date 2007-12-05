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
/// \file HapticsOptions.h
/// \brief Header file for HapticsOptions.
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __HAPTICSOPTIONS_H__
#define __HAPTICSOPTIONS_H__

#include <H3D/H3DOptionNode.h>
#include <H3D/SFString.h>
#include <H3D/SFBool.h>
#include <H3D/SFFloat.h>

namespace H3D {

  /// \ingroup H3DNodes
  /// \class HapticsOptions
  /// \brief Specifies parameters to use when rendering an object with haptics.
  ///
  class H3DAPI_API HapticsOptions : public H3DOptionNode {
  public:
    
    /// Constructor.
    HapticsOptions( Inst< SFNode >  _metadata = 0,
                    Inst< SFString  >  _touchableFace  = 0,
                    Inst< SFFloat   >  _maxDistance  = 0,
                    Inst< SFFloat   > _lookAheadFactor = 0,
                    Inst< SFBool    > _useBoundTree = 0 );
    
    /// Specifies which sides of the shapes to render haptically. If "BACK"
    /// only the back side of can be felt, "FRONT" only front side and 
    /// "FRONT_AND_BACK" both sides. "AS_GRAPHICS" will render the sides
    /// that are visible graphically.
    ///
    /// <b>Valid values: </b> "AS_GRAPHICS", "BACK", "FRONT" or 
    /// "FRONT_AND_BACK" \n
    /// <b>Default value: </b> "AS_GRAPHICS" \n
    /// <b>Access type: </b> inputOutput \n
    auto_ptr< SFString  > touchableFace;

    /// The maximum distance in metres the proxy can be from a primitive in 
    /// order for it to be rendered haptically. A negative value indicates 
    /// that primitives should always be rendered. 
    ///
    /// <b>Default value: </b> 0.01 \n
    /// <b>Access type: </b> inputOutput \n    
    auto_ptr< SFFloat > maxDistance;

    /// When determining if a primitive should be rendered haptically
    /// the primitive will be tested for intersection with a 
    /// sphere with radius maxDistance moving along the line segment
    /// from the current proxy position to another position. The other
    /// position is calculated as p = current_proxy_pos + 
    /// (current_proxy_pos - previous_proxy_pos) * lookAheadFactor. 
    ///
    /// <b>Default value: </b> 3 \n
    /// <b>Access type: </b> inputOutput \n    
    auto_ptr< SFFloat > lookAheadFactor;

    /// IF true the boundTree field is used in the X3DGeometryNodes in order
    /// to extract which triangles are close enough to the proxy. However if
    /// it is unwanted that the boundTree is used(e.g. because the geometry
    /// changes all the time and the overhead for rebuilding the boundTree
    /// is too large, this field can be set to false and the closest triangles
    /// will be extracted with OpenGL instead.
    ///
    /// <b>Default value: </b> true \n
    /// <b>Access type: </b> inputOutput \n    
    auto_ptr< SFBool > useBoundTree;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  };
}

#endif