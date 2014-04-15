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
/// \file Shape.h
/// \brief Header file for Shape, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __SHAPE_H__
#define __SHAPE_H__

#include <H3D/X3DShapeNode.h>

namespace H3D {
  /// \ingroup X3DNodes
  /// \class Shape
  /// \brief The Shape node has two fields, appearance and geometry, which 
  /// are used to create rendered objects in the world. 
  ///
  /// The appearance field contains an Appearance node that specifies the
  /// visual attributes (e.g., material and texture) to be applied to the
  /// geometry. The geometry field contains an X3DGeometryNode. The specified
  /// geometry node is rendered with the specified appearance nodes applied.
  /// If the geometry field is NULL, the object is not drawn.
  /// The hapticGeometry field also contains a X3DGeometryNode. If specified,
  /// this geometry will be used for haptic rendering, enabling separate 
  /// geometries for graphics and haptics. If hapticsGeometry is NULL, the 
  /// geometry in the geometry field will be used for haptics as well as 
  /// graphics.
  ///
  /// The bboxCenter and bboxSize fields specify a bounding box that
  /// encloses the Shape node's geometry. This is a hint that may be used
  /// for optimization purposes. The results are undefined if the specified
  /// bounding box is smaller than the actual bounding box of the geometry
  /// at any time. A default bboxSize value, (-1, -1, -1), implies that the
  /// bounding box is not specified and, if needed, is calculated by the
  /// browser. 
  ///
  /// \par Internal routes:
  /// \dotfile Shape.dot
  class H3DAPI_API Shape : public X3DShapeNode {
  public:
    
    Shape( Inst< SFAppearanceNode > _appearance     = 0,
           Inst< SFGeometryNode   > _geometry       = 0,
           Inst< SFHapticGeometry > _hapticGeometry = 0,
           Inst< SFNode           > _metadata       = 0,
           Inst< SFBound          > _bound          = 0,
           Inst< SFVec3f          > _bboxCenter     = 0,
           Inst< SFVec3f          > _bboxSize       = 0,
           Inst< DisplayList      > _displayList    = 0 );

    static H3DNodeDatabase database;

  };
}

#endif
