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
/// \file NurbsCurve2D.cpp
/// \brief CPP file for NurbsCurve2D, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/NurbsCurve2D.h>

using namespace H3D;

H3DNodeDatabase NurbsCurve2D::database( 
	"NurbsCurve2D", 
	&(newInstance<NurbsCurve2D>),
	typeid( NurbsCurve2D ),
	&X3DNurbsControlCurveNode::database );

namespace NurbsCurve2DInternals {
	FIELDDB_ELEMENT( NurbsCurve2D, tessellation, INPUT_OUTPUT );
	FIELDDB_ELEMENT( NurbsCurve2D, weight, INPUT_OUTPUT );
	FIELDDB_ELEMENT( NurbsCurve2D, knot, INPUT_OUTPUT );
	FIELDDB_ELEMENT( NurbsCurve2D, order, INPUT_OUTPUT );
	FIELDDB_ELEMENT( NurbsCurve2D, closed, INPUT_OUTPUT );
}

NurbsCurve2D::NurbsCurve2D( Inst< SFNode	 > _metadata,
														Inst< MFVec2d  > _controlPoint,
														Inst< SFInt32	 > _tessellation,
														Inst< MFDouble > _weight,
														Inst< MFDouble > _knot,
														Inst< SFInt32	 > _order,
														Inst< SFBool	 > _closed ):
											X3DNurbsControlCurveNode( _metadata, _controlPoint ),
											tessellation( _tessellation ),
											weight( _weight ),
											knot( _knot ),
											order( _order ),
											closed( _closed ) {
type_name = "NurbsCurve2D";
database.initFields( this );

tessellation->setValue( 0 );
order->setValue( 3 );
closed->setValue( false );
}
