//////////////////////////////////////////////////////////////////////////////
//    Copyright 2004-2013, SenseGraphics AB
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
/// \file Shape.cpp
/// \brief CPP file for Shape, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/Shape.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase Shape::database( 
                                "Shape", 
                                &(newInstance<Shape>), 
                                typeid( Shape ),
				                &X3DShapeNode::database 
 );

namespace ShapeInternals {
}


Shape::Shape( 
             Inst<    SFAppearanceNode >  _appearance,
             Inst<    SFGeometryNode >  _geometry,
             Inst<    SFHapticGeometry >  _hapticGeometry,
             Inst<    SFNode >  _metadata,
             Inst<    SFBound              > _bound,  
             Inst< SFVec3f>  _bboxCenter,
             Inst< SFVec3f>  _bboxSize,
             Inst< DisplayList      > _displayList ) :
  X3DShapeNode( _appearance, _geometry, _hapticGeometry,_metadata,
                _bound,_bboxCenter, _bboxSize, _displayList ) {

  displayList->touch();
  type_name = "Shape";

}


