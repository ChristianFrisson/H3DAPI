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
  FIELDDB_ELEMENT( Extrusion, beginCap, INITIALIZE_ONLY );
  FIELDDB_ELEMENT( Extrusion, ccw, INITIALIZE_ONLY );
  FIELDDB_ELEMENT( Extrusion, convex, INITIALIZE_ONLY );
  FIELDDB_ELEMENT( Extrusion, creaseAngle, INITIALIZE_ONLY );
  FIELDDB_ELEMENT( Extrusion, crossSection, INITIALIZE_ONLY );
  FIELDDB_ELEMENT( Extrusion, endCap, INITIALIZE_ONLY );
  FIELDDB_ELEMENT( Extrusion, orientation, INITIALIZE_ONLY );
  FIELDDB_ELEMENT( Extrusion, scale, INITIALIZE_ONLY );
  FIELDDB_ELEMENT( Extrusion, solid, INITIALIZE_ONLY );
  FIELDDB_ELEMENT( Extrusion, spine, INITIALIZE_ONLY );
}

Extrusion::Extrusion( 
                              Inst< SFNode      > _metadata,
                              Inst< SFBound     > _bound,
                              Inst< DisplayList > _displayList,             
                              Inst< SFBool		> _beginCap,
							  Inst< SFBool      > _ccw,
                              Inst< SFBool      > _convex,
							  Inst< SFFloat     > _creaseAngle,
                              Inst< MFVec2f		> _crossSection,
                              Inst< SFBool      > _endCap
                              Inst< MFRotation  > _orientation,
                              Inst< MFVec2f		> _scale,
							  Inst< SFBool      > _solid,
                              Inst< MFVec3f		> _spine ) :
  X3DGeometryNode( _metadata, _bound, _displayList ),
  beginCap       ( _beginCap     ),
  ccw			 ( _ccw          ),
  convex         ( _convex       ),
  creaseAngle    ( _creaseAngle  ),
  crossSection	 ( _crossSection ),
  endCap	     ( _endCap		 ),
  orientation    ( _orientation  ),
  scale			 ( _scale        ),
  solid			 ( _solid		 ),
  spline		 ( _spline       ) {

  type_name = "Extrusion";

  database.initFields( this );

  beginCap ->setValue( true );
  ccw->setValue( true );
  convex->setValue( true );
  creaseAngle->setValue( 0 );
  crossSection->pushback( Vec2f( 1, 1 ) );
  crossSection->pushback( Vec2f( 1, -1 ) );
  crossSection->pushback( Vec2f( -1, -1 ) );
  crossSection->pushback( Vec2f( -1, 1 ) );
  crossSection->pushback( Vec2f( 1, 1 ) );
  endCap->setValue( true );
  orientation->pushback( Rotation(0, 0, 1, 0) );
  scale->pushback( Vec2f( 1, 1 ) );
  solid->setValue( true );
  spine->pushback( Vec3f( 0, 0, 0 ) );
  spine->pushback( Vec3f( 0, 1, 0 ) );

  beginCap->route( displayList );
  ccw->route( displayList );
  convex->route( displayList );
  creaseAngle->route( displayList );
  crossSection->route( displayList );
  endCap->route( displayList );
  orientation->route( displayList );
  scale->route( displayList );
  solid->route( displayList );
  spine->route( displayList );
}

void Extrusion::render() {
  X3DGeometryNode::render();
  H3DInt32 nrOfSpinePoints = spine->size();
  H3DInt32 nrOfCrossSectionPoints = crossSection->size();
	H3DInt32 nrOfScalePoints = scale->size();
  const vector< Vec2f > &cross_section = crossSection->getValue();
  const vector< Vec2f > &scaleVectors = scale->getValue();
  const vector< Vec3f > &spineVectors = spine->getValue();
  const vector< Vec3f > xAxis;
  const vector< Vec3f > yAxis;
  const vector< Vec3f > zAxis;
	bool closed = false;

	if( spineVectors.front() == spineVectors.back() ) {
		closed = true;
	}
	
	glShadeModel( GL_SMOOTH ); 

  // enable backface culling if solid is true
  if( solid->getValue() ) {
    glEnable( GL_CULL_FACE );
    glCullFace( GL_BACK );
  } else {
    glDisable( GL_CULL_FACE );
  }
		
	//store all axes needed for calculations
	if (closed) {
		yAxis.push_back( ( spineVectors[1] - spineVectors[nrOfSpinePoints - 2] ).normalize() );
	}
	else {
		yAxis.push_back( ( spineVectors[1] - spineVectors[0] ).normalize() );
	}
	
	
	for( int  i = 0; i < nrOfSpinePoints - 1; i++ ) {

		if( i > 0 && i < nrOfSpinePoints - 2 ) {
			yAxis[i % 2] = ( spineVectors[i+1] - spineVectors[i-1] ).normalize();
			yAxis[(i + 1) % 2] = ( spineVectors[i+2] - spineVectors[i] ).normalize();
			zAxis[i % 2] = ((spineVectors[i+1] - spineVectors[i]).crossProduct((spineVectors[i-1] - spineVectors[i])).normalize();

			if ( zAxis[i % 2].dotProduct(zAxis[(i - 1) % 2]) < 0 ) {
				zAxis[i % 2] = -zAxis[i % 2];
			}

			zAxis[(i + 1) % 2] = ((spineVectors[i+2] - spineVectors[i+1]).crossProduct((spineVectors[i] - spineVectors[i+1])).normalize();

			if ( zAxis[(i + 1) % 2].dotProduct(zAxis[i % 2]) < 0 ) {
				zAxis[(i + 1) % 2] = -zAxis[(i + 1) % 2];
			}
		}
		else if (i == 0) {
			yAxis[1] = ( spineVectors[2] - spineVectors[0] ).normalize();
			zAxis[1] = ((spineVectors[2] - spineVectors[1]).crossProduct((spineVectors[0] - spineVectors[1])).normalize();
			
			if(closed) {
				yAxis[0] = ( spineVectors[1] - spineVectors[nrOfSpinePoints - 2] ).normalize();
				zAxis[0] = ((spineVectors[1] - spineVectors[0]).crossProduct((spineVectors[nrOfSpinePoints - 2] - spineVectors[0])).normalize();
				
				if ( zAxis[1].dotProduct(zAxis[0]) < 0 ) {
					zAxis[1] = -zAxis[1];
				}
			}
			else {
				yAxis[0] = ( spineVectors[1] - spineVectors[0] ).normalize();
				zAxis[0] = zAxis[1];
			}

		}
		else {
			
			yAxis[i % 2] = ( spineVectors[i+1] - spineVectors[i-1] ).normalize();
			zAxis[i % 2] = ((spineVectors[i+1] - spineVectors[i]).crossProduct((spineVectors[i-1] - spineVectors[i])).normalize();
			
			if ( zAxis[i % 2].dotProduct(zAxis[(i - 1) % 2]) < 0 ) {
				zAxis[i % 2] = -zAxis[i % 2];
			}

			if(closed) {
				yAxis[(i + 1) % 2] = ( spineVectors[1] - spineVectors[nrOfSpinePoints - 2] ).normalize();
				zAxis[(i + 1) % 2] = ((spineVectors[1] - spineVectors[0]).crossProduct((spineVectors[nrOfSpinePoints - 2] - spineVectors[0])).normalize();
				
				if ( zAxis[(i + 1) % 2].dotProduct(zAxis[i % 2]) < 0 ) {
					zAxis[(i + 1) % 2] = -zAxis[(i + 1) % 2];
				}
			}
			else {
				yAxis[(i + 1) % 2] = ( spineVectors[nrOfSpinePoints - 1] - spineVectors[nrOfSpinePoints - 2] ).normalize();
				zAxis[(i + 1) % 2] = zAxis[i % 2];
			}
		}
		xAxis[i % 2] = yAxis[i % 2].crossProduct(zAxis[i % 2]);
		xAxis[(i + 1) % 2] = yAxis[(i + 1) % 2].crossProduct(zAxis[(i + 1) % 2]);
		
    for( int j = 0; j < nrOfCrossSectionPoints - 1; j++ ) {
			scaleVectors[i%nrOfScalePoints].x * cross_section[j].x * xAxis[i%2];
			scaleVectors.[i%nrOfScalePoints].y * cross_section[j].y * yAxis[i%2];
 			glBegin( GL_QUADS );
        glVertex3f( spineVectors[i].x + , spineVectors[i].y + , spineVectors[i].z + ); 
        glVertex3f( spineVectors[i].x + , spineVectors[i].y + , spineVectors[i].z + ); 
        glVertex3f( spineVectors[i+1].x + , spineVectors[i+1].y + , spineVectors[i+1].z + ); 
        glVertex3f( spineVectors[i+1].x + , spineVectors[i+1].y + , spineVectors[i+1].z + ); 
      glEnd();
		}
  }
}