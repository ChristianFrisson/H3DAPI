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
/// \file ArcClose2D.cpp
/// \brief CPP file for ArcClose2D, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/ArcClose2D.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase ArcClose2D::database( "ArcClose2D", 
                               &(newInstance<ArcClose2D>), 
                               typeid( ArcClose2D ),
                               &X3DGeometryNode::database );

namespace ArcClose2DInternals {
  FIELDDB_ELEMENT( ArcClose2D, endAngle, INPUT_OUTPUT );
  FIELDDB_ELEMENT( ArcClose2D, startAngle, INPUT_OUTPUT );
  FIELDDB_ELEMENT( ArcClose2D, radius, INPUT_OUTPUT );
  FIELDDB_ELEMENT( ArcClose2D, closureType, INPUT_OUTPUT );
  FIELDDB_ELEMENT( ArcClose2D, solid, INPUT_OUTPUT );
}


ArcClose2D::ArcClose2D( Inst< SFNode      > _metadata,
                        Inst< SFBound     > _bound,
                        Inst< DisplayList > _displayList,
                        Inst< MFBool      > _isTouched,
                        Inst< MFVec3f     > _force,
                        Inst< MFVec3f     > _contactPoint,
                        Inst< MFVec3f     > _contactNormal,
                        Inst< SFFloat     > _endAngle,
                        Inst< SFFloat     > _startAngle,
                        Inst< SFFloat     > _radius,
                        Inst< SFString    > _closureType,
                        Inst< SFBool      > _solid ):
  X3DGeometryNode( _metadata, _bound, _displayList, _isTouched,
                   _force, _contactPoint, _contactNormal ),
  endAngle( _endAngle ),
  startAngle( _startAngle ),
  radius( _radius ),
  closureType( _closureType ),
  solid( _solid ) {

  type_name = "ArcClose2D";
  database.initFields( this );

  endAngle->setValue( (H3DFloat) Constants::pi/2 );
  startAngle->setValue( 0.f );
  radius->setValue( 1.f );
  closureType->addValidValue( "PIE" );
  closureType->addValidValue( "CHORD" );
  closureType->setValue( "PIE" );
  solid->setValue( false );

  radius->route( bound );

  endAngle->route( displayList );
  startAngle->route( displayList );
  radius->route( displayList );
  closureType->route( displayList );
  solid->route( displayList );
}

void ArcClose2D::render() {

  H3DFloat start_angle = startAngle->getValue();
  H3DFloat end_angle = endAngle->getValue();
  H3DFloat r = radius->getValue();
  const string &closure_type = closureType->getValue();
  Vec2f start_point = Vec2f( 0.f, 0.f );
  Vec2f start_point_tc = Vec2f( 0.5f, 0.5f );

  // if start_angle and end_angle is the same the closure_type should
  // be ignored and a circle be drawn.
  if( start_angle == end_angle ) {
    start_angle = 0.f;
    end_angle = (H3DFloat)Constants::pi * 2;
  } else if( closure_type == "CHORD" ) {
    Vec2f start( r * H3DCos(start_angle ),
                 r * H3DSin(start_angle ) );
    Vec2f end( r * H3DCos(end_angle ),
               r * H3DSin(end_angle ) );
    Vec2f start_tc( start.x / (r*2) + 0.5f, start.y / (r*2) + 0.5f );
    Vec2f end_tc( end.x / (r*2) + 0.5f, end.y / (r*2) + 0.5f );

    start_point = (start + end)/2.f;
    start_point_tc = ( start_tc + end_tc ) / 2.f;
  } else if( closure_type != "PIE" ) {
    Console(3) << "Warning: The field \"closureType\" in ArcClose2D node( "
        << getName() << ") is not correctly defined. The value \"PIE\" will be used. " << endl;
  }
  
  H3DFloat theta, angle_increment;
  H3DFloat x, y;
  H3DFloat nr_segments = 40;

  angle_increment = (H3DFloat) Constants::pi*2 / nr_segments;

  glBegin( GL_TRIANGLE_FAN );
  glNormal3f( 0.f, 0.f, 1.f );
  renderTexCoordForActiveTexture( Vec3f( start_point_tc.x, 
                                         start_point_tc.y, 0 ) );
  glVertex2f( start_point.x, start_point.y );

  for ( theta = start_angle; 
        theta < end_angle;
        theta += angle_increment ) {
    x = r * H3DCos(theta);
    y = r * H3DSin(theta);
    renderTexCoordForActiveTexture( Vec3f( x /(r*2) + 0.5f, 
                                           y / (r*2) + 0.5f, 0 ) );
    glVertex2f (x, y);
  }

  // add the vertex at end_angle.
  theta = end_angle;
  x = r * H3DCos(theta);
  y = r * H3DSin(theta);
  renderTexCoordForActiveTexture( Vec3f( x /(r*2) + 0.5f, 
                                         y / (r*2) + 0.5f, 0 ) );
  glVertex2f (x, y);

 
  glEnd ();
}

void ArcClose2D::traverseSG( TraverseInfo &ti ) {
  X3DGeometryNode::traverseSG( ti );
  if( solid->getValue() ) {
    useBackFaceCulling( true );
  } else {
    useBackFaceCulling( false );
  }
}

