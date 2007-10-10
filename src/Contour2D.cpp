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
/// \file Contour2D.cpp
/// \brief CPP file for Contour2D, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/Contour2D.h>

using namespace H3D;

H3DNodeDatabase Contour2D::database( 
	"Contour2D", 
	&(newInstance<Contour2D>),
	typeid( Contour2D ),
	&X3DNode::database );

namespace Contour2DInternals {
	FIELDDB_ELEMENT( Contour2D, addChildren, INPUT_ONLY );
	FIELDDB_ELEMENT( Contour2D, removeChildren, INPUT_ONLY );
	FIELDDB_ELEMENT( Contour2D, children, INPUT_OUTPUT );
}

Contour2D::Contour2D( Inst< SFNode >						_metadata,
											Inst< AddTheChildren		> _addChildren,
											Inst< RemoveTheChildren > _removeChildren,
											Inst< CheckTheChildren	> _children ) :
											X3DNode( _metadata),
											addChildren( _addChildren ),
											removeChildren( _removeChildren ),
											children( _children ) {

type_name = "Contour2D";
database.initFields( this );
}

void Contour2D::renderTrimmedCurve( GLUnurbsObj *nurbs_object ) {

	H3DInt32 childrenSize = children->size();
	NodeVector theChildren = children->getValue();
	bool closedContour = true;

	Vec2d thePoint;
	Vec2d theFrontPoint;

	// check if the contour is closed
	NurbsCurve2D *nurbs_curve_2D = 
			dynamic_cast< NurbsCurve2D * >( theChildren.front() );

	ContourPolyline2D *contour_poly_line_2D = 
		dynamic_cast< ContourPolyline2D * >( theChildren.front() );

	if( nurbs_curve_2D )
		thePoint = nurbs_curve_2D->controlPoint->getValue().back();

	if( contour_poly_line_2D )
		thePoint = contour_poly_line_2D->controlPoint->getValue().back();

	for( int i = 1; i < childrenSize; i++ ) {
		Vec2d theBackPoint;
		
		nurbs_curve_2D = 
			dynamic_cast< NurbsCurve2D * >( theChildren[i] );

		contour_poly_line_2D = 
			dynamic_cast< ContourPolyline2D * >( theChildren[i] );
		
		if( nurbs_curve_2D ) {
			theFrontPoint = nurbs_curve_2D->controlPoint->getValue().front();
			theBackPoint = nurbs_curve_2D->controlPoint->getValue().back();
		}

		if( contour_poly_line_2D ) {
			theFrontPoint = contour_poly_line_2D->controlPoint->getValue().front();
			theBackPoint = contour_poly_line_2D->controlPoint->getValue().back();
		}

		if( ( thePoint - theFrontPoint ).length() > Constants::d_epsilon ) {
			closedContour = false;
			break;
		}
		thePoint = theBackPoint;
	}

	nurbs_curve_2D = 
		dynamic_cast< NurbsCurve2D * >( theChildren.front() );

	contour_poly_line_2D = 
		dynamic_cast< ContourPolyline2D * >( theChildren.front() );

	if( nurbs_curve_2D ) {
		theFrontPoint = nurbs_curve_2D->controlPoint->getValue().front();
	}

	if( contour_poly_line_2D ) {
		theFrontPoint = contour_poly_line_2D->controlPoint->getValue().front();
	}

	if( ( thePoint - theFrontPoint ).length() > Constants::d_epsilon ) {
			closedContour = false;
		}

	if( !closedContour ) {
		Console(3) << "Warning: the children field does not define a closed curve "
							 << "in Contour2D node( "
               << getName() << "). Node will not be used. " << endl;
    return;
	}

	// start rendering the contour.
	gluBeginTrim( nurbs_object );

	GLfloat *knot = NULL;
	GLfloat *curve = NULL;
	
	for( int i = 0; i < childrenSize; i++ ) {
		nurbs_curve_2D = 
			dynamic_cast< NurbsCurve2D * >( theChildren[i] );

		contour_poly_line_2D = 
			dynamic_cast< ContourPolyline2D * >( theChildren[i] );
		
		// if the contoursection is a nurbsCurve2D a lot of checks has to be made.
		if( nurbs_curve_2D ) {
			H3DInt32 order = nurbs_curve_2D->order->getValue();
			
			// Order has to be 2 or greater to define the NurbsCurve2D
			if( order < 2 ) {
				Console(3) << "Warning: Order is less than 2 in NurbsCurve2D node( "
					<< nurbs_curve_2D->getName() << "). Node will not be rendered. " 
					<< endl;
				return;
			}

			const vector< Vec2d > &theCurve = nurbs_curve_2D->
																				controlPoint->
																				getValue();

			// Number of controlPoints has to be at least order 
			// to define the NurbsCurve2D
			if( theCurve.size() < (unsigned int)order )  {
				Console(3) << "Warning: the number of controlPoints is less than order"
					<< " in NurbsCurve2D node( "
					<< nurbs_curve_2D->getName() 
					<< "). Node will not be rendered. " << endl;
				return;
			}

			const vector< H3DDouble > &theKnot = nurbs_curve_2D->knot->getValue();
			if( knot != NULL )
				delete [] knot;
			knot = new GLfloat[ theKnot.size() ];
			// check the knot-vector. If it is not according to standard
			// a default uniform knot vector will be generated.
			bool generateUniform = true;
			if( theKnot.size() == theCurve.size() + order )  {
				generateUniform = false;
				H3DInt32 consecutiveKnots = 0;
				for( unsigned int i = 0; i < theKnot.size(); i++ ){
					knot[i] = (GLfloat)theKnot[i];				
					if( i > 0 ) {
						if( knot[i] == knot[ i - 1 ] )
							consecutiveKnots++;
						else
							consecutiveKnots = 0;
						if( consecutiveKnots > order - 1 )
							generateUniform = true;

						if( knot[ i - 1 ] > knot[i] )
							generateUniform = true;
					}
				}
			}

			H3DInt32 theSizeToUse = theKnot.size();
			if( generateUniform ) {
				theSizeToUse = theCurve.size() + order;
				Console(3) << "Warning: The Knot array is not according to standard in"
					<< " NurbsCurve2D node( "
					<< nurbs_curve_2D->getName() 
					<< "). A default Knot array will be generated. " << endl;
				if( knot != NULL )
					delete [] knot;
				knot = new GLfloat[ theSizeToUse ];
				for( int i = 0; i < theSizeToUse; i++ )
					knot[i] = (GLfloat)( (H3DDouble)i / ( theSizeToUse - 1 ) );
			}

			if( curve != NULL )
				delete [] curve;
			curve = new GLfloat[ theCurve.size() * 2 ];

			for( unsigned int j = 0; j < theCurve.size(); j++ ) {
				curve[ j * 2 ] = (GLfloat)theCurve[j].x;
				curve[ j * 2 + 1 ] = (GLfloat)theCurve[j].y;
			}

			gluNurbsCurve( nurbs_object,
										 theSizeToUse, knot,
										 2, curve,
										 nurbs_curve_2D->order->getValue(), 
										 GLU_MAP1_TRIM_2 );
		}
		if( contour_poly_line_2D ) {
			const vector< Vec2d > &theCurve = contour_poly_line_2D->
																				controlPoint->
																				getValue();
			if( curve != NULL )
				delete [] curve;
			curve = new GLfloat[ theCurve.size() * 2 ];

			for( unsigned int j = 0; j < theCurve.size(); j++ ) {
				curve[ j * 2 ] = (GLfloat)theCurve[j].x;
				curve[ j * 2 + 1 ] = (GLfloat)theCurve[j].y;
			}

			gluPwlCurve( nurbs_object, 
									 theCurve.size(), curve,
									 2, GLU_MAP1_TRIM_2 );
		}
	}
	gluEndTrim( nurbs_object );

	if( curve != NULL )
		delete [] curve;

	if( knot != NULL )
		delete [] knot;
}
