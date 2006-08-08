//////////////////////////////////////////////////////////////////////////////
//    Copyright 2004, SenseGraphics AB
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
/// \file Extrusion.cpp
/// \brief CPP file for Extrusion, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include "Extrusion.h"

using namespace H3D;

H3DNodeDatabase Extrusion::database( 
        "Extrusion", 
        &(newInstance< Extrusion > ),
        typeid( Extrusion ),
        &X3DGeometryNode::database 
        );

namespace ExtrusionInternals {
  FIELDDB_ELEMENT( Extrusion, beginCap, INPUT_OUTPUT );
  FIELDDB_ELEMENT( Extrusion, ccw, INPUT_OUTPUT );
  FIELDDB_ELEMENT( Extrusion, convex, INPUT_OUTPUT );
  FIELDDB_ELEMENT( Extrusion, creaseAngle, INPUT_OUTPUT );
  FIELDDB_ELEMENT( Extrusion, crossSection, INPUT_OUTPUT );
  FIELDDB_ELEMENT( Extrusion, endCap, INPUT_OUTPUT );
  FIELDDB_ELEMENT( Extrusion, orientation, INPUT_OUTPUT );
  FIELDDB_ELEMENT( Extrusion, scale, INPUT_OUTPUT );
  FIELDDB_ELEMENT( Extrusion, solid, INPUT_OUTPUT );
  FIELDDB_ELEMENT( Extrusion, spine, INPUT_OUTPUT );
}

Extrusion::Extrusion( 
                              Inst< SFNode        > _metadata,
                              Inst< SFBound				> _bound,
                              Inst< DisplayList		> _displayList,             
                              Inst< SFBool				> _beginCap,
															Inst< SFBool				> _ccw,
                              Inst< SFBool				> _convex,
															Inst< SFFloat				> _creaseAngle,
                              Inst< MFVec2f				> _crossSection,
                              Inst< SFBool				> _endCap,
                              Inst< MFRotation		> _orientation,
                              Inst< MFVec2f				> _scale,
															Inst< SFBool				> _solid,
                              Inst< MFVec3f				> _spine,
															Inst< VertexVectors > _vertexVector ) :
  X3DGeometryNode( _metadata, _bound, _displayList ),
  beginCap       ( _beginCap     ),
  ccw						 ( _ccw          ),
  convex         ( _convex       ),
  creaseAngle    ( _creaseAngle  ),
  crossSection	 ( _crossSection ),
  endCap				 ( _endCap			 ),
  orientation    ( _orientation  ),
  scale					 ( _scale        ),
  solid					 ( _solid				 ),
  spine					 ( _spine				 ),
	vertexVector  ( _vertexVector ) {

  type_name = "Extrusion";

  database.initFields( this );

  beginCap ->setValue( true );
  ccw->setValue( true );
  convex->setValue( true );
  creaseAngle->setValue( 0 );
  crossSection->push_back( Vec2f( 1, 1 ) );
  crossSection->push_back( Vec2f( 1, -1 ) );
  crossSection->push_back( Vec2f( -1, -1 ) );
  crossSection->push_back( Vec2f( -1, 1 ) );
  crossSection->push_back( Vec2f( 1, 1 ) );
  endCap->setValue( true );
  orientation->push_back( Rotation( 0, 0, 1, 0 ) );
  scale->push_back( Vec2f( 1, 1 ) );
  solid->setValue( true );
  spine->push_back( Vec3f( 0, 0, 0 ) );
  spine->push_back( Vec3f( 0, 1, 0 ) );

  beginCap->route( displayList, id );
  ccw->route( displayList, id );
  convex->route( displayList, id );
  creaseAngle->route( displayList, id );
	crossSection->route( displayList, id );
  endCap->route( displayList, id );
  solid->route( displayList, id );
  spine->route( displayList, id );
	vertexVector->route( displayList, id );

	crossSection->route( vertexVector );
	orientation->route( vertexVector );
	scale->route( vertexVector );
	spine->route( vertexVector );

	vertexVector->route( bound );
}

	
void Extrusion::VertexVectors::update() {
	const vector< Vec2f > &cross_section = 
		static_cast< MFVec2f * >( routes_in[0] )->getValue();
	const vector< Rotation > &orientationVectors = 
		static_cast< MFRotation * >( routes_in[1] )->getValue();
	const vector< Vec2f > &scaleVectors = 
		static_cast< MFVec2f * >( routes_in[2] )->getValue();
	const vector< Vec3f > &spineVectors = 
    static_cast< MFVec3f * >( routes_in[3] )->getValue();

	Extrusion *theExtrusion = static_cast< Extrusion * >( getOwner() );

	H3DInt32 nrOfSpinePoints = spineVectors.size();
	H3DInt32 nrOfCrossSectionPoints = cross_section.size();
	H3DInt32 nrOfScaleValues = scaleVectors.size();
	H3DInt32 nrOfOrientationValues = orientationVectors.size();

	if( nrOfSpinePoints > 1 ) {
		vector< Vec3f > xAxis;
		vector< Vec3f > yAxis;
		vector< Vec3f > zAxis;
		vector< int > spineForward;
		vector< int > spineBackward;
		vector< Matrix3f > orientationMatrices;

		bool closedSpine = false;
		bool collinear = true;				// if the spine is collinear
		bool coincident = false;			// if every point on the spine is the same

		// Orientation and scale values have to be 1 or >= the number of spine points.
		// If there are more scale or orientation values than spine points
		// the excess values are ignored.
		if( nrOfOrientationValues < nrOfSpinePoints && nrOfOrientationValues > 1 ) {
			Console(3) << "Warning: Not enough orientation values in Extrusion node( "
				<< getName() << "). Node will not be rendered. " << endl;
			return;
		}

		if( nrOfScaleValues < nrOfSpinePoints && nrOfScaleValues > 1  ) {
			Console(3) << "Warning: Not enough scale values in Extrusion node( "
				<< getName() << "). Node will not be rendered. " << endl;
			return;
		}

		// To not get problems with Vectors later on. The excess values are ignored.
		if( nrOfOrientationValues > nrOfSpinePoints )
			nrOfOrientationValues = nrOfSpinePoints;
		if( nrOfScaleValues > nrOfSpinePoints )
			nrOfScaleValues = nrOfSpinePoints;

		// check if the spine is closed
		if( theExtrusion->coinc( spineVectors.front(), spineVectors.back() ) )
			closedSpine = true;

		// build vectors containing indicies to the first non-coincident spine. 
		// Backward and forward.
		// these vectors are used to calculate axis for the SCP around each spine.
		for( int i = 0; i < nrOfSpinePoints; i++ ) {
			int j = i;

			do {
				j++;

				if( j >= nrOfSpinePoints )
					j -= nrOfSpinePoints;
			} while( j!= i && theExtrusion->coinc( spineVectors[i], spineVectors[j] ) );

			if( !closedSpine && j < i )
				spineForward.push_back( nrOfSpinePoints - 1 );
			else
				spineForward.push_back(j);


			if( i == 0 && j == 0 )
				coincident = true;

			j = i;

			do {
				j--;

				if( j < 0 )
					j += nrOfSpinePoints;
			} while( j!= i && theExtrusion->coinc( spineVectors[i], spineVectors[j] ) );

			if( !closedSpine && j > i )
				spineBackward.push_back( 0 );
			else
				spineBackward.push_back(j);
		}

		// if the entire spine is coincident choose and arbitrary direction for the SCP.
		// if another direction is desired use the orientation values.
		// another node might be a better solution for this kind of shape.
		if( coincident ) {
			xAxis.push_back( Vec3f( 1, 0, 0 ) );
			yAxis.push_back( Vec3f( 0, 1, 0 ) );
			zAxis.push_back( Vec3f( 0, 0, 1 ) );

			for( int i = 0; i < nrOfSpinePoints; i++ )
			{
				xAxis.push_back( xAxis.front() );
				yAxis.push_back( yAxis.front() );
				zAxis.push_back( zAxis.front() );
			}
		}
		else {
			// calculate the Z-axes for the first spine point if the first spine point
			// is undetermined at this stage add dummy.
			if( closedSpine ) {
				Vec3f temp = ( spineVectors[ spineForward.front() ] - spineVectors[0] )
					.crossProduct( 
					spineVectors[ spineBackward.front() ] - spineVectors[0] );
				temp.normalizeSafe();
				zAxis.push_back( temp );
				// if one of the cross products calculated is not zero then the spine is 
				// not collinear.
				if( H3DAbs( zAxis.front().lengthSqr() ) >= Constants::f_epsilon )
					collinear = false;
			}
			else
				zAxis.push_back( Vec3f( 0, 0, 0) );

			// calculate z-axes for all spine points but the last and for the first 
			// spine point if it is undetermined.
			for( int  i = 1; i < nrOfSpinePoints - 1; i++ ) {
				Vec3f temp =( spineVectors[ spineForward[i] ] - spineVectors[i] )
					.crossProduct( 
					spineVectors[ spineBackward[i] ] - spineVectors[i] );
				temp.normalizeSafe();
				zAxis.push_back( temp );

				// minimize angle between positive zAxes
				H3DFloat dotProduc = zAxis[i].dotProduct( zAxis[i - 1] );
				if( H3DAbs( dotProduc ) >= Constants::f_epsilon && dotProduc < 0 )
					zAxis[i] = -zAxis[i];

				// first time a valid zAxes is calculated update the previous invalid ones
				if( H3DAbs( zAxis.back().lengthSqr() ) >= Constants::f_epsilon ) {
					collinear = false;
					if( H3DAbs( zAxis.front().lengthSqr() ) < Constants::f_epsilon ) {
						zAxis.front() = zAxis.back();
						for( int j = 1; 
							j < i && H3DAbs( zAxis[j].lengthSqr() ) < Constants::f_epsilon;
							j++ )
							zAxis[j] = zAxis.front();
					}
				}
				else
					zAxis.back() = zAxis[i-1];
			}

			// calculate z-axes for the last spine point
			if( closedSpine ) {
				zAxis.push_back( zAxis.front() );

				// minimize angle between positive zAxes
				H3DFloat dotProduc = zAxis.back().dotProduct( zAxis[ nrOfSpinePoints - 2 ] );
				if( H3DAbs( dotProduc ) >= Constants::f_epsilon && dotProduc < 0 )
					zAxis.back() = -zAxis.back();
			}
			else
				zAxis.push_back( zAxis.back() );

			if( collinear ) {
				// the y-axes is calculated like this since when the spine is collinear
				// it might be that the stepping forward to the first non-coincidential 
				// spine point might end up being the same spine point as if stepping 
				// backward to the first non-coincidential spine point
				// if the spine is closed.
				Vec3f temp = spineVectors[ spineForward.front() ] - spineVectors[0];
				temp.normalizeSafe();
				yAxis.push_back( temp );

				// create dummy x-axes.
				if( H3DAbs( -1 + yAxis.front().x ) < Constants::f_epsilon )
					xAxis.push_back( Vec3f(0, -1, 0) );
				else if( H3DAbs( 1 + yAxis.front().x ) < Constants::f_epsilon )
					xAxis.push_back( Vec3f(0, 1, 0) );
				else
					xAxis.push_back( Vec3f(1, 0, 0) );

				zAxis.clear();
				zAxis.push_back( xAxis.front().crossProduct( yAxis.front() ) );

				// create final x-axes cause the axis need to be perpendicular to eachother
				xAxis.front() = yAxis.front().crossProduct( zAxis.front() );
				xAxis.front().normalizeSafe();
				zAxis.front().normalizeSafe();

				Vec3f spineVector = spineVectors.back() - spineVectors.front();
				for ( int i = nrOfSpinePoints - 1; 
					i > 0 && H3DAbs( spineVector.lengthSqr() ) < Constants::f_epsilon;
					i-- ) {
						spineVector = ( spineVectors[i] - spineVectors.front() );
				}

				// maybe this is unneccesary. It might be enough to just flip the y-axis.
				spineVector.normalizeSafe();
				Matrix3f rotationMatrix( Rotation ( yAxis.front(), spineVector ) );
				yAxis.front() = rotationMatrix * yAxis.front();
				xAxis.front() = rotationMatrix * xAxis.front();
				zAxis.front() = rotationMatrix * zAxis.front();

				for( int i = 1; i < nrOfSpinePoints; i++ ) {
					xAxis.push_back( xAxis.front() );
					yAxis.push_back( yAxis.front() );
					zAxis.push_back( zAxis.front() );
				}
			}
			else {
				// axes for first spine point
				Vec3f temp;
				if( closedSpine ) {			
					temp = spineVectors[ spineForward.front() ]
					- spineVectors[ spineBackward.front() ];
					temp.normalizeSafe();
					yAxis.push_back( temp );
				}
				else {
					temp = spineVectors[ spineForward.front() ] - spineVectors[0];
					temp.normalizeSafe();
					yAxis.push_back( temp );
				}

				temp = yAxis.front().crossProduct( zAxis.front() );
				temp.normalizeSafe();
				xAxis.push_back( temp );

				// axes for all but the last and first spine point
				for( int i = 1; i < nrOfSpinePoints - 1; i++ ) {
					temp = spineVectors[ spineForward[i] ]
					- spineVectors[ spineBackward[i] ];
					temp.normalizeSafe();
					yAxis.push_back( temp );

					temp = yAxis[i].crossProduct( zAxis[i] );
					temp.normalizeSafe();
					xAxis.push_back( temp );
				}

				// axes for last spine point
				if( closedSpine )
					yAxis.push_back( yAxis.front() );
				else {
					temp = spineVectors.back() - spineVectors[ spineBackward.back() ];
					temp.normalizeSafe();
					yAxis.push_back( temp );
				}
				temp = yAxis.back().crossProduct( zAxis.back() );
				temp.normalizeSafe();
				xAxis.push_back( temp );
			}
		}

		for( int i = 0; i < nrOfOrientationValues; i++ )
			orientationMatrices.push_back( 
			static_cast< Matrix3f >( orientationVectors[i] ) );

		// calculate vertices.
		for( int i = 0; i < nrOfSpinePoints; i++ ) {
			for( int j = 0; j < nrOfCrossSectionPoints; j++ ) {
				Vec3f point0_x = scaleVectors[ i % nrOfScaleValues ].x
					* cross_section[j].x * xAxis[i];
				Vec3f point0_z = scaleVectors[ i % nrOfScaleValues ].y
					* cross_section[j].y * zAxis[i];
				Vec3f point0 = spineVectors[i] + 
					orientationMatrices[ i % nrOfOrientationValues ]
				* ( point0_x + point0_z );

				value.push_back( point0 );
			}
		}
	}
}

void Extrusion::render() {
  X3DGeometryNode::render();
	bool ccwcheck = ccw->getValue();
	H3DInt32 nrOfSpinePoints = spine->size();
  H3DInt32 nrOfCrossSectionPoints = crossSection->size();
  const vector< Vec2f > &cross_section = crossSection->getValue();
	const vector< Vec3f > &spineVectors = spine->getValue();
	H3DFloat crease_angle = creaseAngle->getValue();

	if( nrOfSpinePoints > 1 ) {
		vector< Vec3f > yAxis;
		vector< int > spineForward;
		vector< int > spineBackward;
		vector< Vec3f > normalVector;

		vector< H3DFloat > uTextureCoordinates;
		vector< H3DFloat > vTextureCoordinates;
		vector< Vec2f > textureCoordinatesCaps;

		bool closedSpine = false;
		bool collinear = true;				// if the spine is collinear
		bool coincident = false;			// if every point on the spine is the same
		H3DInt32 ifCapsAdd = 0;

		// determine which side of polygons is front and back face. Since the 
		// GLWindow renderCamera() function might scale the y-axis with -1 and
		// set the GL_FRONT_FACE to GL_CW if scaled and GL_CCW otherwise, we have
		// to check what the previous value is to determine what the GL_FRONT_FACE 
		// will be. This can also happen if a Transform node has negative scaling.
		GLint front_face;
		glGetIntegerv( GL_FRONT_FACE, &front_face );
		if( front_face == GL_CW ) {
			// we are in mirrored mode, so we have to set the front face
			// to the opposite in order for it to be right when mirrored.
			if( ccwcheck )
				glFrontFace( GL_CW );
			else  
				glFrontFace( GL_CCW );
		} else {
			if( ccw->getValue() )
				glFrontFace( GL_CCW );
			else 
				glFrontFace( GL_CW );
		}

		glShadeModel( GL_SMOOTH ); 

		// enable backface culling if solid is true
		if( solid->getValue() ) {
			glEnable( GL_CULL_FACE );
			glCullFace( GL_BACK );
		} else
			glDisable( GL_CULL_FACE );

		// check if the spine is closed
		if( coinc( spineVectors.front(), spineVectors.back() ) )
			closedSpine = true;

		// build vectors containing indicies to the first non-coincident spine. 
		// Backward and forward.
		// these vectors are used to calculate axis for the SCP around each spine.
		for( int i = 0; i < nrOfSpinePoints; i++ ) {
			int j = i;

			do {
				j++;

				if( j >= nrOfSpinePoints )
					j -= nrOfSpinePoints;
			} while( j!= i && coinc( spineVectors[i], spineVectors[j] ) );

			if( !closedSpine && j < i )
				spineForward.push_back( nrOfSpinePoints - 1 );
			else
				spineForward.push_back(j);

			if( i == 0 && j == 0 )
				coincident = true;

			j = i;

			do {
				j--;

				if( j < 0 )
					j += nrOfSpinePoints;
			} while( j!= i && coinc( spineVectors[i], spineVectors[j] ) );

			if( !closedSpine && j > i )
				spineBackward.push_back( 0 );
			else
				spineBackward.push_back(j);
		}

		// if the entire spine is coincident choose and arbitrary direction for the SCP.
		// if another direction is desired use the orientation values.
		// another node might be a better solution for this kind of shape.
		if( coincident ) {
			yAxis.push_back( Vec3f( 0, 1, 0 ) );
			yAxis.push_back( yAxis.front() );
		}
		else {

			// check if the spine is collinear
			if( closedSpine ) {
				Vec3f temp = ( spineVectors[ spineForward.front() ] - spineVectors[0] )
					.crossProduct( 
					spineVectors[ spineBackward.front() ] - spineVectors[0] );
				// if one of the cross products calculated is not zero then the spine is
				// not collinear.
				if( H3DAbs( temp.lengthSqr() ) >= Constants::f_epsilon )
					collinear = false;
			}

			for( int  i = 1; i < nrOfSpinePoints - 1 && collinear; i++ ) {
				Vec3f temp =( spineVectors[ spineForward[i] ] - spineVectors[i] )
					.crossProduct(
					spineVectors[ spineBackward[i] ] - spineVectors[i] );

				if( H3DAbs( temp.lengthSqr() ) >= Constants::f_epsilon ) {
					collinear = false;
				}
			}

			if( collinear ) {
				// the y-axes is calculated like this since when the spine is collinear
				// it might be that the stepping forward to the first non-coincidential 
				// spine point might end up being the same spine point as if stepping 
				// backward to the first non-coincidential spine point
				// if the spine is closed.
				Vec3f temp = spineVectors[ spineForward.front() ] - spineVectors[0];
				temp.normalizeSafe();
				yAxis.push_back( temp );

				Vec3f spineVector = spineVectors.back() - spineVectors.front();
				for ( int i = nrOfSpinePoints - 1;
					i > 0 && H3DAbs( spineVector.lengthSqr() ) < Constants::f_epsilon;
					i-- ) {
						spineVector = ( spineVectors[i] - spineVectors.front() );
				}

				// maybe this is unneccesary. It might be enough to just flip the y-axis.
				spineVector.normalizeSafe();
				Matrix3f rotationMatrix( Rotation ( yAxis.front(), spineVector ) );
				yAxis.front() = rotationMatrix * yAxis.front();
				yAxis.push_back( yAxis.front() );
			}
			else {

				// axes for first spine point
				Vec3f temp;
				if( closedSpine ) {			
					temp = spineVectors[ spineForward.front() ] -
						spineVectors[ spineBackward.front() ];
					temp.normalizeSafe();
					yAxis.push_back( temp );
				}
				else {
					temp = spineVectors[ spineForward.front() ] - spineVectors[0];
					temp.normalizeSafe();
					yAxis.push_back( temp );
				}

				// axes for last spine point
				if( closedSpine )
					yAxis.push_back( yAxis.front() );
				else {
					temp = spineVectors.back() - spineVectors[ spineBackward.back() ];
					temp.normalizeSafe();
					yAxis.push_back( temp );
				}
			}
		}

		// get the Vertices
		const vector< Vec3f > vertexvec = vertexVector->getValue();

		// Calculation of the u and v coordinates for the extrusion.
		// u is along cross section, v is along the spine.
		// For the caps u and v corresponds to x and z of the SCP
		// and the biggest difference in x or z maps to 0-1. Uniform scaling.
		H3DFloat spineLength = 0;
		vTextureCoordinates.push_back(0);
		for( int i = 0; i < nrOfSpinePoints - 1; i++ ) {
			spineLength += ( spineVectors[ i + 1 ] - spineVectors[i] ).length();
			vTextureCoordinates.push_back( spineLength );
		}

		for( int i = 0; i < nrOfSpinePoints; i++ ) {
			vTextureCoordinates[i] = vTextureCoordinates[i] / spineLength;
		}

		H3DFloat crossSectionLength = 0;
		H3DFloat crossSectionXMin;
		H3DFloat crossSectionXMax;
		H3DFloat crossSectionZMin;
		H3DFloat crossSectionZMax;
		H3DFloat crossSectionRange;
		uTextureCoordinates.push_back(0);
		for( int i = 0; i < nrOfCrossSectionPoints - 1; i++ ) {
			crossSectionLength += ( cross_section[ i + 1 ] - cross_section[i] ).length();
			uTextureCoordinates.push_back( crossSectionLength );
			if( i == 0 ) {
				crossSectionXMin = cross_section[i].x;
				crossSectionXMax = cross_section[i].x;
				crossSectionZMin = cross_section[i].y;
				crossSectionZMax = cross_section[i].y;
			}
			else {
				if( cross_section[i].x < crossSectionXMin )
					crossSectionXMin = cross_section[i].x;
				if( cross_section[i].x > crossSectionXMax )
					crossSectionXMax = cross_section[i].x;
				if( cross_section[i].y < crossSectionZMin )
					crossSectionZMin = cross_section[i].y;
				if( cross_section[i].y > crossSectionZMax )
					crossSectionZMax = cross_section[i].y;
			}
		}

		for( int i = 0; i < nrOfCrossSectionPoints; i++ ) {
			uTextureCoordinates[i] = uTextureCoordinates[i] / crossSectionLength;
		}

		if( crossSectionXMax - crossSectionXMin >= crossSectionZMax - crossSectionZMin )
			crossSectionRange = crossSectionXMax - crossSectionXMin;
		else
			crossSectionRange = crossSectionZMax - crossSectionZMin;

		for( int i = 0; i < nrOfCrossSectionPoints; i++ )
			textureCoordinatesCaps.push_back( Vec2f(
			( cross_section[i].x - crossSectionXMin ) / crossSectionRange,
			( cross_section[i].y - crossSectionZMin ) / crossSectionRange ) );

		// generate normals depending on if creaseangle is used or not.
		// if creaseangle is 0 normalVector contains the result 
		// from generateNormalsPerFace
		if( crease_angle < Constants::pi )
			generateNormalsPerVertex( normalVector,
			vertexvec,
			cross_section, 
			yAxis, 
			ccwcheck, 
			nrOfCrossSectionPoints, 
			nrOfSpinePoints, 
			closedSpine, 
			crease_angle,
			ifCapsAdd );
		else
			generateNormalsPerVertex( normalVector,
			vertexvec, 
			cross_section,
			yAxis,
			ccwcheck,
			nrOfCrossSectionPoints,
			nrOfSpinePoints,
			closedSpine,
			ifCapsAdd );


		bool useCreaseAngle = true;
		if( H3DAbs( crease_angle ) < Constants::f_epsilon )
			useCreaseAngle = false;

		// if there is a cap in the beginning, draw it.
		if( beginCap -> getValue() ) {
			Vec3f point_x;
			Vec3f point_z;
			Vec3f point;

			glBegin( GL_POLYGON );

			Vec3f theNormal;
			if( !useCreaseAngle ) {
				theNormal = normalVector.front();
				glNormal3f( theNormal.x, theNormal.y, theNormal.z );
			}

			for( int i = nrOfCrossSectionPoints - 1; i >= 0; i-- )
			{
				glTexCoord2d( textureCoordinatesCaps[i].x, textureCoordinatesCaps[i].y );
				if( useCreaseAngle ) {
					theNormal = normalVector[i];
					glNormal3f( theNormal.x, theNormal.y, theNormal.z );
				}

				point = vertexvec[i];

				glVertex3f( point.x, point.y , point.z );
			}
			glEnd();
		}

		// draw the quads of the body.
		for( int i = 0; i < nrOfSpinePoints - 1; i++ ) {
			for( int j = 0; j < nrOfCrossSectionPoints - 1; j++ ) {

				H3DInt32 lower = i * nrOfCrossSectionPoints + j;
				H3DInt32 upper = ( i + 1 ) * nrOfCrossSectionPoints + j;
				H3DInt32 quad_index = i * ( nrOfCrossSectionPoints - 1 ) + j;
				Vec3f point;
				Vec3f theNormal;

				if( !useCreaseAngle ) {
					Vec3f theNormal = normalVector[ ifCapsAdd + quad_index ];
					glNormal3f( theNormal.x, theNormal.y, theNormal.z );
				}

				glBegin( GL_QUADS );

				glTexCoord2d( uTextureCoordinates[j], vTextureCoordinates[i] );
				if( useCreaseAngle ) {
					if( crease_angle < Constants::pi )
						theNormal = normalVector[ ifCapsAdd +  quad_index * 4 ];
					else
						theNormal = normalVector[ lower ];
					glNormal3f( theNormal.x, theNormal.y, theNormal.z );
				}
				point = vertexvec[ lower ];
				glVertex3f( point.x, point.y, point.z );

				glTexCoord2d( uTextureCoordinates[ j + 1 ], vTextureCoordinates[i] );
				if( useCreaseAngle ) {
					if( crease_angle < Constants::pi )
						theNormal = normalVector[ ifCapsAdd +  quad_index * 4 + 1 ];
					else
						theNormal = normalVector[ lower + 1 ];
					glNormal3f( theNormal.x, theNormal.y, theNormal.z );
				}
				point = vertexvec[ lower + 1 ];
				glVertex3f( point.x, point.y, point.z );

				glTexCoord2d( uTextureCoordinates[ j + 1 ], vTextureCoordinates[ i + 1 ] );
				if( useCreaseAngle ) {
					if( crease_angle < Constants::pi )
						theNormal = normalVector[ ifCapsAdd +  quad_index * 4 + 2 ];
					else
						theNormal = normalVector[ upper + 1 ];
					glNormal3f( theNormal.x, theNormal.y, theNormal.z );
				}
				point = vertexvec[ upper + 1 ];
				glVertex3f( point.x, point.y, point.z );

				glTexCoord2d( uTextureCoordinates[j], vTextureCoordinates[ i + 1 ] );
				if( useCreaseAngle ) {
					if( crease_angle < Constants::pi )
						theNormal = normalVector[ ifCapsAdd +  quad_index * 4 + 3 ];
					else
						theNormal = normalVector[ upper ];
					glNormal3f( theNormal.x, theNormal.y, theNormal.z );
				}
				point = vertexvec[ upper ];
				glVertex3f( point.x, point.y, point.z );

				glEnd();
			}
		}

		// if there is a cap in the end, draw it.
		if( endCap -> getValue() ) {
			Vec3f point_x;
			Vec3f point_z;
			Vec3f point;

			glBegin( GL_POLYGON );

			Vec3f theNormal;
			if( !useCreaseAngle ) {
				theNormal = normalVector.back();
				glNormal3f( theNormal.x, theNormal.y, theNormal.z );
			}

			for(int i = 0; i < nrOfCrossSectionPoints; i++ )
			{
				glTexCoord2d( textureCoordinatesCaps[i].x, textureCoordinatesCaps[i].y );
				if( useCreaseAngle ) {
					theNormal = 
						normalVector[ normalVector.size() - nrOfCrossSectionPoints + i ];
					glNormal3f( theNormal.x, theNormal.y, theNormal.z );
				}

				point = vertexvec[ vertexvec.size() - nrOfCrossSectionPoints + i ];
				glVertex3f( point.x, point.y , point.z );
			}
			glEnd();
		}

		// Restore the front face to its previuos value.
		glFrontFace( front_face );
	}
}

vector< Vec3f > Extrusion::generateNormalsPerFace(  
											const vector < Vec3f > &vertex_vector,
											const vector < Vec2f > &cross_section,
											vector < Vec3f > &yAxis,
											bool ccwcheck,
											H3DInt32 nrOfCrossSectionPoints,
											H3DInt32 nrOfSpinePoints,
											bool closedCrossSection,
											H3DInt32 &ifCapsAdd ) {
	
	vector < Vec3f > normalVector;
	H3DInt32 closedCrossSectionInt = 0;
	H3DFloat ccwOrCw = 0;

	if( closedCrossSection )
		closedCrossSectionInt = 1;

	// to check which way the points are ordered looking in the negative
	// direction from the positive y-axis.
	for( int i = 0;
			 i < nrOfCrossSectionPoints - closedCrossSectionInt;
			 i++ )
		ccwOrCw += cross_section[i].y * 
							 cross_section[ ( i + 1 ) % nrOfCrossSectionPoints ].x -
							 cross_section[ ( i + 1 ) % nrOfCrossSectionPoints ].y *
							 cross_section[i].x;

	// calculate normal for the beginCap if it exists.
	if( beginCap -> getValue() ) {
		if( ( ccwcheck && ccwOrCw >= 0 ) || ( !ccwcheck && ccwOrCw < 0 ) )
			normalVector.push_back( 
				Vec3f( -yAxis.front().x, -yAxis.front().y, -yAxis.front().z ) );
		else
			normalVector.push_back( 
				Vec3f( yAxis.front().x, yAxis.front().y, yAxis.front().z ) );
		ifCapsAdd = 1;
	}

	// calculate one normal for each face of the main body
	for( int i = 0; i < nrOfSpinePoints - 1; i++ ) {
		for( int j = 0; j < nrOfCrossSectionPoints - 1; j++ ) {
			vector < H3DInt32 > theIndices;
			theIndices.push_back( i * nrOfCrossSectionPoints + j );
			theIndices.push_back( i * nrOfCrossSectionPoints + j + 1 );
			theIndices.push_back( ( i + 1 ) * nrOfCrossSectionPoints + j + 1 );
			theIndices.push_back( ( i + 1 ) * nrOfCrossSectionPoints + j );
			bool zeroNormal = true;
			Vec3f theNormal;

			for(int k = 4; k < 8 && zeroNormal; k++ ) {
				theNormal = calculateNormal( vertex_vector, 
																		 theIndices[ ( k + 1 ) % 4 ],
																		 theIndices[ k % 4 ],
																		 theIndices[ ( k - 1 ) % 4 ] );
				if( H3DAbs( theNormal.lengthSqr() ) > Constants::f_epsilon )
					zeroNormal = false;
			}
			theNormal.normalizeSafe();
			
			if( ccwcheck )
				normalVector.push_back( theNormal );
			else
				normalVector.push_back( -theNormal );
		}
	}
	
	// calculate normal for the endCap if it exists
	if( endCap -> getValue() ) {
		if( ( ccwcheck && ccwOrCw >= 0 ) || ( !ccwcheck && ccwOrCw < 0 ) )
			normalVector.push_back(
				Vec3f( yAxis.back().x, yAxis.back().y, yAxis.back().z ) );
		else
			normalVector.push_back(
				Vec3f( -yAxis.back().x, -yAxis.back().y, -yAxis.back().z ) );
	}

	return normalVector;
}

void Extrusion::generateNormalsPerVertex(
											vector < Vec3f > &normalVector,
                      const vector < Vec3f > &vertex_vector,
											const vector < Vec2f > &cross_section,
											vector < Vec3f > &yAxis,
											bool ccwcheck,
											H3DInt32 nrOfCrossSectionPoints,
											H3DInt32 nrOfSpinePoints,
											bool closedSpine,
											H3DInt32 &ifCapsAdd ) {
  
	bool closedCrossSection = false;
	if( coinc( cross_section.front(), cross_section.back() ) )
		closedCrossSection = true;
												
	vector< Vec3f > normalsPerFace = generateNormalsPerFace( 
																			vertex_vector,
															 			  cross_section,
															 			  yAxis,
																		  ccwcheck,
																			nrOfCrossSectionPoints,
																			nrOfSpinePoints,
																			closedCrossSection,
																			ifCapsAdd );

	// no separate normals are needed for the caps.
	for( int i = 0; i < nrOfSpinePoints; i++ ) {
		for( int j = 0; j < nrOfCrossSectionPoints; j++ ) {
			vector< H3DInt32 > theIndices;
			normalVector.push_back( Vec3f( 0, 0, 0 ) );
			bool beginCapSummed = false;
			bool endCapSummed = false;

			theIndices = findSurroundingFaces( i,
																				 j,
																				 closedSpine,
																				 nrOfSpinePoints, 
																				 closedCrossSection,
																				 nrOfCrossSectionPoints );

			for( int k = 0; k < 4; k++ ) {
				if( theIndices[k] == -1 && theIndices[ k + 4 ] != -2 && !beginCapSummed ) {
					normalVector.back() = normalVector.back() + normalsPerFace.front();
					beginCapSummed = true;
				}
				else if( theIndices[k] == -3 && theIndices[ k + 4 ] != -2 && !endCapSummed ) {
					normalVector.back() = normalVector.back() + normalsPerFace.back();
					endCapSummed = true;
				}
				else if( theIndices[k] != -2 && theIndices[ k + 4 ] != -2 &&
								 theIndices[k] != -1 && theIndices[k] != -3 )
					normalVector.back() = normalVector.back() +
																normalsPerFace[ ifCapsAdd + 
																								theIndices[k] * 
																								(nrOfCrossSectionPoints - 1) +
																								theIndices[ k + 4 ] ];
 			}
			normalVector.back().normalizeSafe();
		}
	}
}

void Extrusion::generateNormalsPerVertex( 
                      vector < Vec3f > &normalVector,
											const vector < Vec3f > &vertex_vector,
											const vector < Vec2f > &cross_section,
											vector < Vec3f > &yAxis,
											bool ccwcheck,
											H3DInt32 nrOfCrossSectionPoints,
											H3DInt32 nrOfSpinePoints,
											bool closedSpine,
											H3DFloat crease_angle,
											H3DInt32 &ifCapsAdd ) {
	
	bool closedCrossSection = false;
	if( coinc( cross_section.front(), cross_section.back() ) )
		closedCrossSection = true;

	vector< Vec3f > normalsPerFace = generateNormalsPerFace( 
																		 vertex_vector,
																		 cross_section,
																		 yAxis,
																		 ccwcheck,
																		 nrOfCrossSectionPoints,
																		 nrOfSpinePoints,
																		 closedCrossSection,
																		 ifCapsAdd );
	
	if( H3DAbs( crease_angle ) < Constants::f_epsilon )
		normalVector = normalsPerFace;
	else {
		H3DFloat cos_crease_angle = H3DCos( crease_angle );

		// normals for begincap
		if( beginCap->getValue() ) {

			Vec3f theFaceNormal = normalsPerFace.front();
			normalVector.push_back( theFaceNormal );
			Vec3f quad_normal = normalsPerFace[1];
			if( quad_normal * theFaceNormal > cos_crease_angle )
				normalVector.back() += quad_normal;
			if( closedCrossSection ) {
				quad_normal = 
					normalsPerFace[ ifCapsAdd + (nrOfCrossSectionPoints - 1) - 1 ];
				if( quad_normal * theFaceNormal > cos_crease_angle )
					normalVector.back() += quad_normal;
			}
			normalVector.back().normalizeSafe();
			
			for( int i = 1; i < nrOfCrossSectionPoints - 1; i++ )
			{
				normalVector.push_back( theFaceNormal );

				quad_normal = normalsPerFace[ ifCapsAdd + i ];
				if( quad_normal * theFaceNormal > cos_crease_angle )
					normalVector.back() += quad_normal;

				quad_normal = normalsPerFace[ ifCapsAdd + i - 1 ];
				if( quad_normal * theFaceNormal > cos_crease_angle )
					normalVector.back() += quad_normal;

				normalVector.back().normalizeSafe();
			}

			normalVector.push_back( theFaceNormal );
			quad_normal =
				normalsPerFace[ ifCapsAdd + (nrOfCrossSectionPoints - 1) - 1 ];
			if( quad_normal * theFaceNormal > cos_crease_angle )
				normalVector.back() += quad_normal;
			if( closedCrossSection ) {
				quad_normal = normalsPerFace[1];
				if( quad_normal * theFaceNormal > cos_crease_angle )
					normalVector.back() += quad_normal;
			}
			normalVector.back().normalizeSafe();
		}

		// normals for every face except the beginCap and endCap
		for( int i = 0; i < nrOfSpinePoints - 1; i++ ) {
			for( int j = 0; j < nrOfCrossSectionPoints - 1; j++ ) {
				vector< H3DInt32 > theIndices;
				Vec3f theFaceNormal = 
					normalsPerFace[ ifCapsAdd + i * (nrOfCrossSectionPoints - 1) + j ];
				bool beginCapSummed = false;
				bool endCapSummed = false;

				//	vertex 0
				normalVector.push_back( Vec3f( 0, 0, 0 ) );
				H3DInt32 indexI = i;
				H3DInt32 indexJ = j;
				theIndices = findSurroundingFaces( indexI,
																					 indexJ,
																					 closedSpine,
																					 nrOfSpinePoints,
																					 closedCrossSection,
																					 nrOfCrossSectionPoints );

				for( int k = 0; k < 4; k++ ) {
					if( theIndices[ k ] == -1 && 
							theIndices[ k + 4 ] != -2 &&
							!beginCapSummed ) {
						Vec3f quad_normal = normalsPerFace.front();
						if( quad_normal * theFaceNormal > cos_crease_angle )
							normalVector.back() += quad_normal;
						beginCapSummed = true;
					}
					else if( theIndices[ k ] == -3 && 
									 theIndices[ k + 4 ] != -2 && 
									 !endCapSummed ) {
						Vec3f quad_normal = normalsPerFace.back();
						if( quad_normal * theFaceNormal > cos_crease_angle )
							normalVector.back()+=quad_normal;
						endCapSummed = true;
					}
					else if( theIndices[ k ] != -2 && 
									 theIndices[ k + 4 ] != -2 &&
									 theIndices[ k ] != -1 &&
									 theIndices[ k ] != -3 ) {
							Vec3f quad_normal = 
								normalsPerFace[ ifCapsAdd + theIndices[k] *
																(nrOfCrossSectionPoints - 1) + 
																theIndices[ k + 4 ] ];
							if( quad_normal * theFaceNormal > cos_crease_angle )
								normalVector.back()+=quad_normal;
					}
				}
				normalVector.back().normalizeSafe();


				//	vertex 1
				beginCapSummed = false;
				endCapSummed = false;
				normalVector.push_back( Vec3f( 0, 0, 0 ) );
				indexI = i;
				indexJ = j + 1;
				theIndices = findSurroundingFaces( indexI,
																					 indexJ,
																					 closedSpine,
																					 nrOfSpinePoints,
																					 closedCrossSection,
																					 nrOfCrossSectionPoints );

				for( int k = 0; k < 4; k++ ) {
					if( theIndices[k] == -1 &&
							theIndices[ k + 4 ] != -2 &&
							!beginCapSummed ) {
						Vec3f quad_normal = normalsPerFace.front();
						if( quad_normal * theFaceNormal > cos_crease_angle )
							normalVector.back()+=quad_normal;
						beginCapSummed = true;
					}
					else if( theIndices[ k ] == -3 && 
									 theIndices[ k + 4 ] != -2 &&
									 !endCapSummed ) {
						Vec3f quad_normal = normalsPerFace.back();
						if( quad_normal * theFaceNormal > cos_crease_angle )
							normalVector.back()+=quad_normal;
						endCapSummed = true;
					}
					else if( theIndices[ k ] != -2 &&
									 theIndices[ k + 4 ] != -2 &&
									 theIndices[ k ] != -1 &&
									 theIndices[ k ] != -3 ) {		
							Vec3f quad_normal = 
								normalsPerFace[ ifCapsAdd + 
																theIndices[k] *
																(nrOfCrossSectionPoints - 1) +
																theIndices[ k + 4 ] ];
							if( quad_normal * theFaceNormal > cos_crease_angle )
								normalVector.back()+=quad_normal;
					}
				}
				normalVector.back().normalizeSafe();

				//	vertex 2
				beginCapSummed = false;
				endCapSummed = false;
				normalVector.push_back( Vec3f( 0, 0, 0 ) );
				indexI = i + 1;
				indexJ = j + 1;
				theIndices = findSurroundingFaces( indexI,
																					 indexJ,
																					 closedSpine,
																					 nrOfSpinePoints,
																					 closedCrossSection,
																					 nrOfCrossSectionPoints );

				for( int k = 0; k < 4; k++) {
					if( theIndices[ k ] == -1 && 
							theIndices[ k + 4 ] != -2 && 
							!beginCapSummed ) {
						Vec3f quad_normal = normalsPerFace.front();
						if( quad_normal * theFaceNormal > cos_crease_angle )
							normalVector.back()+=quad_normal;
						beginCapSummed = true;
					}
					else if( theIndices[ k ] == -3 &&
									 theIndices[ k + 4 ] != -2 &&
									 !endCapSummed ) {
						Vec3f quad_normal = normalsPerFace.back();
						if( quad_normal * theFaceNormal > cos_crease_angle )
							normalVector.back()+=quad_normal;
						endCapSummed = true;
					}
					else if( theIndices[ k ] != -2 &&
									 theIndices[ k + 4 ] != -2 &&
									 theIndices[ k ] != -1 &&
									 theIndices[ k ] != -3 ) {
							Vec3f quad_normal = 
								normalsPerFace[ ifCapsAdd +
																theIndices[k] *
																(nrOfCrossSectionPoints - 1) +
																theIndices[ k + 4 ] ];
							if( quad_normal * theFaceNormal > cos_crease_angle )
								normalVector.back()+=quad_normal;
					}
				}
				normalVector.back().normalizeSafe();

				//	vertex 3
				beginCapSummed = false;
				endCapSummed = false;
				normalVector.push_back( Vec3f( 0, 0, 0 ) );
				indexI = i + 1;
				indexJ = j;
				theIndices = findSurroundingFaces( indexI,
																					 indexJ,
																					 closedSpine,
																					 nrOfSpinePoints,
																					 closedCrossSection,
																					 nrOfCrossSectionPoints );

				for( int k = 0; k < 4; k++) {
					if( theIndices[k] == -1 && 
							theIndices[ k + 4 ] != -2 &&
							!beginCapSummed ) {
						Vec3f quad_normal = normalsPerFace.front();
						if( quad_normal * theFaceNormal > cos_crease_angle )
							normalVector.back()+=quad_normal;
						beginCapSummed = true;
					}
					else if( theIndices[k] == -3 &&
									 theIndices[ k + 4 ] != -2 &&
									 !endCapSummed ) {
						Vec3f quad_normal = normalsPerFace.back();
						if( quad_normal * theFaceNormal > cos_crease_angle )
							normalVector.back()+=quad_normal;
						endCapSummed = true;
					}
					else if( theIndices[k] != -2 &&
									 theIndices[ k + 4 ] != -2 &&
									 theIndices[k] != -1 &&
									 theIndices[k] != -3 ) {
							Vec3f quad_normal = 
								normalsPerFace[ ifCapsAdd +
																theIndices[k] *
																(nrOfCrossSectionPoints - 1) +
																theIndices[ k + 4 ] ];
							if( quad_normal * theFaceNormal > cos_crease_angle )
								normalVector.back()+=quad_normal;
					}
				}

				normalVector.back().normalizeSafe();
			}
		}

		// normals calculated for the endCap if needed.
		if( endCap->getValue() ) {

			H3DInt32 theLastVertexNormal = normalsPerFace.size() - 1;
			Vec3f theFaceNormal = normalsPerFace.back();
			normalVector.push_back( theFaceNormal );
			Vec3f quad_normal = 
				normalsPerFace[ theLastVertexNormal - nrOfCrossSectionPoints ];
			if( quad_normal * theFaceNormal > cos_crease_angle )
				normalVector.back() += quad_normal;
			if( closedCrossSection ) {
				quad_normal = 
					normalsPerFace[ theLastVertexNormal - nrOfCrossSectionPoints + 1 ];
				if( quad_normal * theFaceNormal > cos_crease_angle )
					normalVector.back() += quad_normal;
			}
			normalVector.back().normalizeSafe();

			for( int i = 1; i < nrOfCrossSectionPoints - 1; i++ )	{
				normalVector.push_back( theFaceNormal );

				quad_normal = 
					normalsPerFace[ theLastVertexNormal - nrOfCrossSectionPoints + i ];
				if( quad_normal * theFaceNormal > cos_crease_angle )
					normalVector.back() += quad_normal;

				quad_normal = 
					normalsPerFace[ theLastVertexNormal - nrOfCrossSectionPoints + i + 1 ];
				if( quad_normal * theFaceNormal > cos_crease_angle )
					normalVector.back() += quad_normal;

				normalVector.back().normalizeSafe();
			}

			normalVector.push_back( theFaceNormal );
			quad_normal = 
				normalsPerFace[ theLastVertexNormal  - nrOfCrossSectionPoints + 1 ];
			if( quad_normal * theFaceNormal > cos_crease_angle )
				normalVector.back() += quad_normal;
			if( closedCrossSection ) {
				quad_normal = 
					normalsPerFace[ theLastVertexNormal - nrOfCrossSectionPoints ];
				if( quad_normal * theFaceNormal > cos_crease_angle )
					normalVector.back() += quad_normal;
			}
			normalVector.back().normalizeSafe();
		}
		
		//	if there is a beginCap the ifCapsAdd needs to be set so that the right 
		//	vertices are picked when drawing.
		if( beginCap->getValue() ) {
			ifCapsAdd = nrOfCrossSectionPoints;
		}
	}
}

vector< H3DInt32 > Extrusion::findSurroundingFaces( 
																H3DInt32 i,
																H3DInt32 j,
																bool closedSpine,
																H3DInt32 nrOfSpinePoints,
																bool closedCrossSection,
																H3DInt32 nrOfCrossSectionPoints ) {
	vector< H3DInt32 > theIndices;

	//	check a number of conditions to find the indices to the surrouding faces.
	//	-1 = beginCap, -2 = endCap, -3 = no Cap and not closed.
	if( i == 0 && closedSpine ) {
		theIndices.push_back( nrOfSpinePoints - 2 );
		theIndices.push_back( nrOfSpinePoints - 2 );
	}
	else if( i == 0 && !closedSpine ) {
		if( beginCap->getValue() ) {
			theIndices.push_back(-1);
			theIndices.push_back(-1);
		}
		else {
			theIndices.push_back(-2);
			theIndices.push_back(-2);
		}
	}
	else {
		theIndices.push_back( i - 1 );
		theIndices.push_back( i - 1 );
	}

	if( i == nrOfSpinePoints - 1 && closedSpine ) {
		theIndices.push_back(0);
		theIndices.push_back(0);
	}
	else if( i == nrOfSpinePoints - 1 && !closedSpine ) {
		if( endCap->getValue() ) {
			theIndices.push_back(-3);
			theIndices.push_back(-3);
		}
		else {
			theIndices.push_back(-2);
			theIndices.push_back(-2);
		}
	}
	else {
		theIndices.push_back(i);
		theIndices.push_back(i);
	}

	if( j == 0 && closedCrossSection ) {
		theIndices.push_back( nrOfCrossSectionPoints - 2 );
		theIndices.push_back( nrOfCrossSectionPoints - 2 );
	}
	else if(  j == 0 && !closedCrossSection ) {
		theIndices.push_back(-2);
		theIndices.push_back(-2);
	}
	else {
		theIndices.push_back( j - 1 );
		theIndices.push_back( j - 1 );
	}

	if( j == nrOfCrossSectionPoints - 1 && closedCrossSection ) {
		theIndices.push_back(0);
		theIndices.push_back(0);
	}
	else if( j == nrOfCrossSectionPoints - 1 && !closedCrossSection ) {
		theIndices.push_back(-2);
		theIndices.push_back(-2);
	}
	else {
		theIndices.push_back(j);
		theIndices.push_back(j);
	}

	// to get the pairing right, otherwise we just have indices but duplicated.
	theIndices[5] = theIndices[6];
	theIndices[6] = theIndices[4];

	return theIndices;
}

void Extrusion::SFBound::update() {
  const vector< Vec3f > &vertex_vectors = 
    static_cast< MFVec3f * >( routes_in[0] )->getValue();
	
	BoxBound *bb = new BoxBound;
	bb->fitAroundPoints( vertex_vectors.begin(), vertex_vectors.end() );
	value = bb;
}

#ifdef USE_HAPTICS
void Extrusion::traverseSG( TraverseInfo &ti ) {
  if( ti.hapticsEnabled() && ti.getCurrentSurface() ) {
#ifdef HAVE_OPENHAPTICS
    ti.addHapticShapeToAll( getOpenGLHapticShape( 
                                                 ti.getCurrentSurface(),
                                                 ti.getAccForwardMatrix(),
                                                 ( vertexVector->size() ) * 4 ) );
#endif
  }
}
#endif
