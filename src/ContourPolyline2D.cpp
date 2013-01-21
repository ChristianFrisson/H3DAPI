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
/// \file ContourPolyline2D.cpp
/// \brief CPP file for ContourPolyline2D, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/ContourPolyline2D.h>

using namespace H3D;

H3DNodeDatabase ContourPolyline2D::database( 
	"ContourPolyline2D", 
	&(newInstance<ContourPolyline2D>),
	typeid( ContourPolyline2D ),
	&X3DNurbsControlCurveNode::database );

namespace ContourPolyline2DInternals {
}

ContourPolyline2D::ContourPolyline2D( Inst< SFNode	 > _metadata,
																			Inst< MFVec2d  > _controlPoint ):
											X3DNurbsControlCurveNode( _metadata, _controlPoint ) {

type_name = "ContourPolyline2D";
database.initFields( this );
}
