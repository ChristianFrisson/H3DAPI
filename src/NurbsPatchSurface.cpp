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
/// \file NurbsPatchSurface.cpp
/// \brief CPP file for NurbsPatchSurface, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <NurbsPatchSurface.h>
#include <HLFeedbackShape.h>

using namespace H3D;

H3DNodeDatabase NurbsPatchSurface::database( 
	"NurbsPatchSurface", 
	&(newInstance<NurbsPatchSurface>),
	typeid( NurbsPatchSurface ),
	&X3DNurbsSurfaceGeometryNode::database );

namespace NurbsPatchSurfaceInternals {

}


NurbsPatchSurface::NurbsPatchSurface( 
																		 Inst< SFNode>  _metadata,
																		 Inst< SFBound > _bound,
																		 Inst< DisplayList > _displayList,
																		 Inst< MFBool > _isTouched,
																		 Inst< MFVec3f > _force,
																		 Inst< MFVec3f > _contactPoint,
																		 Inst< MFVec3f > _contactNormal,
																		 Inst< SFCoordinateNode > _controlPoint,
																		 Inst< TexCoordOrNurbsTex > _texCoord,
																		 Inst< SFInt32     > _uTessellation,
																		 Inst< SFInt32     > _vTessellation,
																		 Inst< MFDouble    > _weight,
																		 Inst< SFBool      > _solid,
																		 Inst< SFBool      > _uClosed,
																		 Inst< SFBool      > _vClosed,
																		 Inst< SFInt32     > _uDimension,
																		 Inst< SFInt32     > _vDimension,
																		 Inst< MFDouble    > _uKnot,
																		 Inst< MFDouble    > _vKnot,
																		 Inst< SFInt32     > _uOrder,
																		 Inst< SFInt32     > _vOrder ) :
X3DNurbsSurfaceGeometryNode( _metadata, _bound, _displayList, _isTouched,
														_force, _contactPoint, _contactNormal,
														_controlPoint, _texCoord, _uTessellation,
														_vTessellation, _weight, _solid, _uClosed,
														_vClosed, _uDimension, _vDimension, _uKnot,
														_vKnot, _uOrder, _vOrder ) {

															type_name = "NurbsPatchSurface";
															database.initFields( this );

}


void NurbsPatchSurface::renderBetweenBeginEnd( 
																GLUnurbsObj *nurbs_object,
																X3DTextureCoordinateNode *tex_coord_node,
																TextureCoordinateGenerator *tex_coord_gen,
																NurbsTextureCoordinate *nurbs_tex_coord, 
																GLfloat *texKnotU,
																GLfloat *texKnotV,
																GLfloat *texCoordinates, 
																GLfloat *u_knots, 
																GLfloat *v_knots, 
																H3DInt32 &sizeOfVertex,
																GLfloat *withWeights, 
																H3DInt32 &uSizeToUse, 
																H3DInt32 &vSizeToUse, 
																H3DInt32 &map2Vertex3Or4 ) {

	// define the texture coordinates
	if( tex_coord_node ) {
		if( tex_coord_gen ) {
			tex_coord_gen->startTexGen();
		}
	} 
	else if( nurbs_tex_coord ) {
		nurbs_tex_coord->renderTexForNurbs( nurbs_object );
	}
	else {
		gluNurbsSurface( nurbs_object, 
			4, texKnotU, 
			4, texKnotV,
			2	, 2 * 2,
			texCoordinates,
			2, 2,
			GL_MAP2_TEXTURE_COORD_2 );
	}

	// define the surface.
	gluNurbsSurface( nurbs_object, 
		uSizeToUse, u_knots, 
		vSizeToUse, v_knots,
		sizeOfVertex, sizeOfVertex*uDimension->getValue(),
		withWeights,
		uOrder->getValue(), vOrder->getValue(),
		map2Vertex3Or4 );

}

