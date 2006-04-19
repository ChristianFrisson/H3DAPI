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
  H3DInt32 nrOfSpinePoints = spine->size();
  H3DInt32 nrOfCrossSectionPoints = crossSection->size();
	H3DInt32 nrOfScalePoints = scale->size();
  const vector< Vec2f > &cross_section = crossSection->getValue();
  const vector< Vec2f > &scaleVectors = scale->getValue();
  const vector< Vec3f > &spineVectors = spine->getValue();
  vector< Vec3f > xAxis;
  vector< Vec3f > yAxis;
  vector< Vec3f > zAxis;
	vector< int > spineForward;
	vector< int > spineBackward;
	bool closed = false;
	bool collinear = true;
	bool coincident = false;
	
	glShadeModel( GL_SMOOTH ); 

  // enable backface culling if solid is true
  if( solid->getValue() ) {
    glEnable( GL_CULL_FACE );
    glCullFace( GL_BACK );
  } else {
    glDisable( GL_CULL_FACE );
  }
	
	// check if the spine is closed
	if( coinc( spineVectors.front(), spineVectors.back() ) ) {
		closed = true;
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

	// calculate the Z-axes for the first spine point if the first spine point is
	// undetermined at this stage, if not add dummy.
	if( closed ) {
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
	if( closed ) {
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
		if( H3DAbs( 1 - yAxis.front().x ) < Constants::f_epsilon ) {
			xAxis.push_back( Vec3f(0, -1, 0) );
		}
		else if( H3DAbs( -1 - yAxis.front().x ) < Constants::f_epsilon ) {
			xAxis.push_back( Vec3f(0, 1, 0) );
		}
		else {
			xAxis.push_back( Vec3f(1, 0, 0) );
		}
		
		zAxis.clear();
		zAxis.push_back( xAxis.front().crossProduct( yAxis.front() ) );

		// create final x-axes cause the axis need to be perpendicular to eachother
		xAxis.front() = yAxis.front().crossProduct( zAxis.front() );

		for( int i = 1; i < nrOfSpinePoints; i++ ) {
			xAxis.push_back( xAxis.front() );
			yAxis.push_back( yAxis.front() );
			zAxis.push_back( zAxis.front() );
		}
	}
	else {
		Vec3f temp;
		if( closed ) {			
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

		if( closed ) {
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

	for( int i = 0; i < nrOfSpinePoints - 1; i++) {
		for( int j = 0; j < nrOfCrossSectionPoints - 1; j++ ) {
			Vec3f point0_x = scaleVectors[i%nrOfScalePoints].x * cross_section[j].x * xAxis[i];
			Vec3f point0_z = scaleVectors[i%nrOfScalePoints].y * cross_section[j].y * zAxis[i];
			Vec3f point0 = spineVectors[i] + point0_x + point0_z;

			Vec3f point1_x = scaleVectors[i%nrOfScalePoints].x * cross_section[j+1].x * xAxis[i];
			Vec3f point1_z = scaleVectors[i%nrOfScalePoints].y * cross_section[j+1].y * zAxis[i];
			Vec3f point1 = spineVectors[i] + point1_x + point1_z;

			Vec3f point2_x = scaleVectors[(i+1)%nrOfScalePoints].x * cross_section[j+1].x * xAxis[(i+1)];
			Vec3f point2_z = scaleVectors[(i+1)%nrOfScalePoints].y * cross_section[j+1].y * zAxis[(i+1)];
			Vec3f point2 = spineVectors[(i+1)] + point2_x + point2_z;

			Vec3f point3_x = scaleVectors[(i+1)%nrOfScalePoints].x * cross_section[j].x * xAxis[(i+1)];
			Vec3f point3_z = scaleVectors[(i+1)%nrOfScalePoints].y * cross_section[j].y * zAxis[(i+1)];
			Vec3f point3 = spineVectors[(i+1)] + point3_x + point3_z;

			Vec3f normal = (point2 - point1).crossProduct(point0 - point1);
			normal.normalizeSafe();

 			glBegin( GL_QUADS );
			glNormal3f( normal.x, normal.y, normal.z);
			glVertex3f( point0.x, point0.y , point0.z);
			glVertex3f( point1.x, point1.y , point1.z);
			glVertex3f( point2.x, point2.y , point2.z);
			glVertex3f( point3.x, point3.y , point3.z);
      glEnd();
		}
	}
}