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
                              Inst< SFNode      > _metadata,
                              Inst< SFBound     > _bound,
                              Inst< DisplayList > _displayList,             
                              Inst< SFBool			> _beginCap,
															Inst< SFBool      > _ccw,
                              Inst< SFBool      > _convex,
															Inst< SFFloat     > _creaseAngle,
                              Inst< MFVec2f			> _crossSection,
                              Inst< SFBool      > _endCap,
                              Inst< MFRotation  > _orientation,
                              Inst< MFVec2f			> _scale,
															Inst< SFBool      > _solid,
                              Inst< MFVec3f			> _spine ) :
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
  spine					 ( _spine				 ) {

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
  orientation->push_back( Rotation(0, 0, 1, 0) );
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
  orientation->route( displayList, id );
  scale->route( displayList, id );
  solid->route( displayList, id );
  spine->route( displayList, id );
}

void Extrusion::render() {
  X3DGeometryNode::render();
	bool ccwcheck = ccw->getValue();
	H3DInt32 nrOfSpinePoints = spine->size();
  H3DInt32 nrOfCrossSectionPoints = crossSection->size();
	H3DInt32 nrOfScaleValues = scale->size();
	H3DInt32 nrOfOrientationValues = orientation->size();
  const vector< Vec2f > &cross_section = crossSection->getValue();
  const vector< Vec2f > &scaleVectors = scale->getValue();
  const vector< Vec3f > &spineVectors = spine->getValue();
	const vector< Rotation > &orientationVectors = orientation->getValue();
  vector< Vec3f > xAxis;
  vector< Vec3f > yAxis;
  vector< Vec3f > zAxis;
	vector< Vec3f > vertexVector;
	vector< Vec3f > normalVector;
	vector< Vec3f > capNormal;
	vector< int > spineForward;
	vector< int > spineBackward;
	vector< Matrix3f > orientationMatrices;
	bool closedSpine = false;					// if the spine is closed
	bool collinear = true;				// if the spine is collinear
	bool coincident = false;			// if every point on the spine is the same
	H3DInt32 closedCrossSection = 0;
	
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
  } else {
    glDisable( GL_CULL_FACE );
  }
	
	if( nrOfOrientationValues > nrOfSpinePoints ) {
		nrOfOrientationValues = nrOfSpinePoints;
	}

	if( nrOfScaleValues > nrOfSpinePoints ) {
		nrOfScaleValues = nrOfSpinePoints;
	}

	// check if the spine is closed
	if( coinc( spineVectors.front(), spineVectors.back() ) ) {
		closedSpine = true;
	}

	// build vectors containing indicies to the first non-coincident spine. Backward and forward.
	for(int i = 0; i < nrOfSpinePoints; i++) {
    int j = i;

		do {
			j++;
      
			if( j >= nrOfSpinePoints) {
				j -= nrOfSpinePoints;
			}
		} while( j!= i && coinc( spineVectors[i], spineVectors[j] ) );
		
		spineForward.push_back(j);

		if( i == 0 && j == 0){
			coincident = true;
		}
		
		j = i;

		do {
			j--;
      
			if( j < 0 ) {
				j += nrOfSpinePoints;
			}
		} while( j!= i && coinc( spineVectors[i], spineVectors[j] ) );

		spineBackward.push_back(j);
	}

	if(coincident) {
		xAxis.push_back( Vec3f( 1, 0, 0 ) );
		yAxis.push_back( Vec3f( 0, 1, 0 ) );
		zAxis.push_back( Vec3f( 0, 0, 1 ) );

		for( int i = 0; i < nrOfSpinePoints; i++)
		{
			xAxis.push_back( xAxis.front() );
			yAxis.push_back( yAxis.front() );
			zAxis.push_back( zAxis.front() );
		}
	}
	else {
		// calculate the Z-axes for the first spine point if the first spine point is
		// undetermined at this stage, if not add dummy.
		if( closedSpine ) {
			Vec3f temp = ( spineVectors[spineForward.front()] - spineVectors[0] ).crossProduct( spineVectors[spineBackward.front()] - spineVectors[0] );
			temp.normalizeSafe();
			zAxis.push_back( temp );
			if( H3DAbs( zAxis.front().lengthSqr() ) >= Constants::f_epsilon ) {
				collinear = false;
			}
		}
		else {
			zAxis.push_back( Vec3f( 0, 0, 0) );
		}

		// calculate z-axes for all spine points but the last and for the first spine point if it is undetermined.
		for( int  i = 1; i < nrOfSpinePoints - 1; i++ ) {
			Vec3f temp =(spineVectors[spineForward[i]] - spineVectors[i]).crossProduct( spineVectors[spineBackward[i]] - spineVectors[i] );
			temp.normalizeSafe();
			zAxis.push_back( temp );

			// minimize angle between positive zAxes
			H3DFloat dotProduc = zAxis[i].dotProduct(zAxis[i - 1]);
			if ( H3DAbs( dotProduc ) >= Constants::f_epsilon && dotProduc < 0 ) {
				zAxis[i] = -zAxis[i];
			}	

			// first time a valid zAxes is calculated update the previous invalid ones
			if( H3DAbs( zAxis.back().lengthSqr() ) >= Constants::f_epsilon ) {
				collinear = false;
				if( H3DAbs( zAxis.front().lengthSqr() ) < Constants::f_epsilon ) {
					zAxis.front() = zAxis.back();
					for( int j = 1; j < i && H3DAbs( zAxis[j].lengthSqr() ) < Constants::f_epsilon ; j++ ) {
						zAxis[j] = zAxis.front();
					}
				}
			}
			else {
				zAxis.back() = zAxis[i-1];
			}
		}

		// calculate z-axes for the last spine point
		if( closedSpine ) {
			zAxis.push_back( zAxis.front() );

			// minimize angle between positive zAxes
			H3DFloat dotProduc = zAxis.back().dotProduct(zAxis[nrOfSpinePoints - 2]);
			if ( H3DAbs( dotProduc ) >= Constants::f_epsilon && dotProduc < 0 ) {
				zAxis.back() = -zAxis.back();
			}	
		}
		else {
			zAxis.push_back( zAxis.back() );
		}

		if( collinear ) {
			// the y-axes is calculated like this since when the spine is collinear
			// it might be that the stepping forward to the first non-coincidential 
			// spine point might end up being the same spine point as if stepping 
			// backward to the first non-coincidential spine point.
			Vec3f temp = spineVectors[spineForward.front()] - spineVectors[0];
			temp.normalizeSafe();
			yAxis.push_back( temp );

			// create dummy x-axes.
			if( H3DAbs( -1 + yAxis.front().x ) < Constants::f_epsilon ) {
				xAxis.push_back( Vec3f(0, -1, 0) );
			}
			else if( H3DAbs( 1 + yAxis.front().x ) < Constants::f_epsilon ) {
				xAxis.push_back( Vec3f(0, 1, 0) );
			}
			else {
				xAxis.push_back( Vec3f(1, 0, 0) );
			}

			zAxis.clear();
			zAxis.push_back( xAxis.front().crossProduct( yAxis.front() ) );

			// create final x-axes cause the axis need to be perpendicular to eachother
			xAxis.front() = yAxis.front().crossProduct( zAxis.front() );

			Vec3f spineVector = ( spineVectors.back() - spineVectors.front() );
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
			Vec3f temp;
			if( closedSpine ) {			
				temp = spineVectors[spineForward.front()] - spineVectors[spineBackward.front()];
				temp.normalizeSafe();
				yAxis.push_back( temp );
			}
			else {
				temp = spineVectors[spineForward.front()] - spineVectors[0];
				temp.normalizeSafe();
				yAxis.push_back( temp );
			}

			temp = yAxis.front().crossProduct( zAxis.front() );
			temp.normalizeSafe();
			xAxis.push_back( temp );

			for( int i = 1; i < nrOfSpinePoints - 1; i++) {
				temp = spineVectors[spineForward[i]] - spineVectors[spineBackward[i]];
				temp.normalizeSafe();
				yAxis.push_back( temp );

				temp = yAxis[i].crossProduct( zAxis[i] );
				temp.normalizeSafe();
				xAxis.push_back( temp );
			}

			if( closedSpine ) {
				yAxis.push_back( yAxis.front() );
			}
			else {
				temp = spineVectors[spineBackward.back()] - spineVectors.back();
				temp.normalizeSafe();
				yAxis.push_back( temp );
			}
			temp = yAxis.back().crossProduct( zAxis.back() );
			temp.normalizeSafe();
			xAxis.push_back( temp );
		}
	}

	for( int i = 0; i < nrOfOrientationValues; i++ ) {
		orientationMatrices.push_back( static_cast< Matrix3f >(orientationVectors[i]) );
	}
	
	// check if the crossSection is closed
	if( coinc( cross_section.front(), cross_section.back() ) ) {
		closedCrossSection = 1;
	}

	// to check which way the points are ordered looking in the negative direction from the positive y-axis.
	H3DFloat ccwOrCw = 0;

	for( int i = 0; i < nrOfCrossSectionPoints - closedCrossSection; i++ ) {
		ccwOrCw += cross_section[i].y * cross_section[(i+1) % nrOfCrossSectionPoints].x - cross_section[(i+1) % nrOfCrossSectionPoints].y * cross_section[i].x;
	}

	if( beginCap -> getValue() ) {
		Vec3f point_x;
		Vec3f point_z;
		Vec3f point;

		glBegin( GL_POLYGON );

		
		if( ( ccwcheck && ccwOrCw >= 0 ) || ( !ccwcheck && ccwOrCw < 0 ) ) {
			glNormal3f( -yAxis.front().x, -yAxis.front().y, -yAxis.front().z);
			capNormal.push_back( Vec3f( -yAxis.front().x, -yAxis.front().y, -yAxis.front().z ) );
			capNormal.push_back( Vec3f( yAxis.front().x, yAxis.front().y, yAxis.front().z ) );
		}
		else {
			glNormal3f( yAxis.front().x, yAxis.front().y, yAxis.front().z);
			capNormal.push_back( Vec3f( yAxis.front().x, yAxis.front().y, yAxis.front().z ) );
			capNormal.push_back( Vec3f( -yAxis.front().x, -yAxis.front().y, -yAxis.front().z ) );
		}

		for(int i = nrOfCrossSectionPoints - 1; i >= 0; i--)
		{
			point_x = scaleVectors.front().x * cross_section[i].x * xAxis.front();
			point_z = scaleVectors.front().y * cross_section[i].y * zAxis.front();
			point = spineVectors.front() + orientationMatrices.front() * (point_x + point_z);

			glVertex3f( point.x, point.y , point.z);
		}
    glEnd();
	}

	for( int i = 0; i < nrOfSpinePoints; i++) {
		for( int j = 0; j < nrOfCrossSectionPoints; j++ ) {
			Vec3f point0_x = scaleVectors[i%nrOfScaleValues].x * cross_section[j].x * xAxis[i];
			Vec3f point0_z = scaleVectors[i%nrOfScaleValues].y * cross_section[j].y * zAxis[i];
			Vec3f point0 = spineVectors[i] + orientationMatrices[i%nrOfOrientationValues] * (point0_x + point0_z);

			vertexVector.push_back( point0 );
/*
			Vec3f point1_x = scaleVectors[i%nrOfScaleValues].x * cross_section[j+1].x * xAxis[i];
			Vec3f point1_z = scaleVectors[i%nrOfScaleValues].y * cross_section[j+1].y * zAxis[i];
			Vec3f point1 = spineVectors[i] + orientationMatrices[i%nrOfOrientationValues] * (point1_x + point1_z);

			Vec3f point2_x = scaleVectors[(i+1)%nrOfScaleValues].x * cross_section[j+1].x * xAxis[(i+1)];
			Vec3f point2_z = scaleVectors[(i+1)%nrOfScaleValues].y * cross_section[j+1].y * zAxis[(i+1)];
			Vec3f point2 = spineVectors[i+1] + orientationMatrices[(i+1)%nrOfOrientationValues] * (point2_x + point2_z);

			Vec3f point3_x = scaleVectors[(i+1)%nrOfScaleValues].x * cross_section[j].x * xAxis[(i+1)];
			Vec3f point3_z = scaleVectors[(i+1)%nrOfScaleValues].y * cross_section[j].y * zAxis[(i+1)];
			Vec3f point3 = spineVectors[i+1] + orientationMatrices[(i+1)%nrOfOrientationValues] * (point3_x + point3_z);

			Vec3f normal = (point2 - point1).crossProduct(point0 - point1);
			normal.normalizeSafe();
			
			if( ccwcheck ) {
				glNormal3f( normal.x, normal.y, normal.z);
			}
			else {
				glNormal3f( -normal.x, -normal.y, -normal.z);
			}

 			glBegin( GL_QUADS );
			
			glVertex3f( point0.x, point0.y , point0.z);
			glVertex3f( point1.x, point1.y , point1.z);
			glVertex3f( point2.x, point2.y , point2.z);
			glVertex3f( point3.x, point3.y , point3.z);
      glEnd();*/
		}
	}
	
	for( int i = 0; i < nrOfSpinePoints; i++) {
		for( int j = 0; j < nrOfCrossSectionPoints; j++ ) {
			vector< Vec3f > temporary;
			temporary.push_back( Vec3f( 0, 0, 0 ) );
			temporary.push_back( Vec3f( 0, 0, 0 ) );
			temporary.push_back( Vec3f( 0, 0, 0 ) );
			temporary.push_back( Vec3f( 0, 0, 0 ) );
			/*Vec3f iMinus_J;
			Vec3f i_JPlus;
			Vec3f iPlus_J;
			Vec3f i_JMinus;*/


			if( i == 0 ) { 
				if( closedSpine ) {
					temporary[0] = ( nrOfSpinePoints - 2 ) * nrOfSpinePoints + j;
				}
				else {
					temporary[0] = i * nrOfSpinePoints + j;
				}
				temporary[2] = ( i + 1 ) * nrOfSpinePoints + j;
			}
			else if( i == nrOfSpinePoints - 1 ) {
				if( closedSpine ) {
					temporary[2] = nrOfSpinePoints + j;
				}
				else {
					temporary[2] = i * nrOfSpinePoints + j;
				}
				temporary[0] = ( i - 1 ) * nrOfSpinePoints + j;
			}
			else {
				temporary[0] = ( i - 1 ) * nrOfSpinePoints + j;
				temporary[2] = ( i + 1 ) * nrOfSpinePoints + j;
			}


			if( j == 0 ) { 
				if( closedCrossSection == 1 ) {
					temporary[3] = i * nrOfSpinePoints + nrOfCrossSectionPoints - 2;
				}
				else {
					temporary[3] = i * nrOfSpinePoints + j;
				}
				temporary[2] = ( i + 1 ) * nrOfSpinePoints + j;
			}
			else if( j == nrOfSpinePoints - 1 ) {
				if( closedCrossSection == 1 ) {
					temporary[2] = nrOfSpinePoints + j;
				}
				else {
					temporary[2] = i * nrOfSpinePoints + j;
				}
				temporary[0] = ( i - 1 ) * nrOfSpinePoints + j;
			}
			else {
				temporary[1] = i * nrOfSpinePoints + j + 1;
				temporary[3] = i * nrOfSpinePoints + j - 1;
			}
		}
	}


	for( int i = 0; i < nrOfSpinePoints - 1; i++) {
		for( int j = 0; j < nrOfCrossSectionPoints - 1; j++ ) {

			glBegin( GL_QUADS );
			
			H3DInt32 lower = i * nrOfCrossSectionPoints + j;
			H3DInt32 upper = ( i + 1 ) * nrOfCrossSectionPoints + j;
			H3DInt32 inverseNormal = 1;

			if( !ccwcheck ) {
				inverseNormal = -1;
			}
			
			Vec3f theNormal = ( vertexVector[ lower + 1 ] - 
													vertexVector[ lower ] ).crossProduct(
													vertexVector[ upper ] - 
													vertexVector[ lower ] );
			theNormal.normalizeSafe();
			//normalVector.push_back( inverseNormal * theNormal );
			theNormal = inverseNormal * theNormal;
			glNormal3f( theNormal.x, theNormal.y, theNormal.z );
			glVertex3f( vertexVector[ lower ].x, vertexVector[ lower ].y, vertexVector[ lower ].z  );
			

			theNormal = ( vertexVector[ upper + 1 ] - 
										vertexVector[ lower + 1 ] ).crossProduct(
										vertexVector[ lower ] - 
										vertexVector[ lower + 1 ] );
			theNormal.normalizeSafe();
			theNormal = inverseNormal * theNormal;
			//normalVector.push_back( inverseNormal * theNormal );
			glNormal3f( theNormal.x, theNormal.y, theNormal.z );
			glVertex3f( vertexVector[ lower + 1 ].x, vertexVector[ lower + 1 ].y, vertexVector[ lower + 1 ].z  );


			theNormal = ( vertexVector[ upper ] - 
										vertexVector[ upper + 1 ] ).crossProduct(
										vertexVector[ lower + 1 ] - 
										vertexVector[ upper + 1 ] );
			theNormal.normalizeSafe();
			theNormal = inverseNormal * theNormal;
			//normalVector.push_back( inverseNormal * theNormal );
			glNormal3f( theNormal.x, theNormal.y, theNormal.z );
			glVertex3f( vertexVector[ upper + 1 ].x, vertexVector[ upper + 1 ].y, vertexVector[ upper + 1 ].z  );


			theNormal = ( vertexVector[ lower ] - 
										vertexVector[ upper ] ).crossProduct(
										vertexVector[ upper + 1 ] - 
										vertexVector[ upper ] );
			theNormal.normalizeSafe();
			theNormal = inverseNormal * theNormal;
			//normalVector.push_back( inverseNormal * theNormal );
			glNormal3f( theNormal.x, theNormal.y, theNormal.z );
			glVertex3f( vertexVector[ upper ].x, vertexVector[ upper ].y, vertexVector[ upper ].z  );
			
			glEnd();
		}
	}

	if( endCap -> getValue() ) {
		Vec3f point_x;
		Vec3f point_z;
		Vec3f point;

		glBegin( GL_POLYGON );

		if( ( ccwcheck && ccwOrCw >= 0 ) || ( !ccwcheck && ccwOrCw < 0 ) ) {
			glNormal3f( yAxis.front().x, yAxis.front().y, yAxis.front().z);
		}
		else {
			glNormal3f( -yAxis.front().x, -yAxis.front().y, -yAxis.front().z);
		}

		for(int i = 0; i < nrOfCrossSectionPoints; i++)
		{
			point_x = scaleVectors.back().x * cross_section[i].x * xAxis.back();
			point_z = scaleVectors.back().y * cross_section[i].y * zAxis.back();
			point = spineVectors.back() + orientationMatrices.back() * (point_x + point_z);

			glVertex3f( point.x, point.y , point.z);
		}
		glEnd();
	}

	// Restore the front face to its previuos value.
  glFrontFace( front_face );
}