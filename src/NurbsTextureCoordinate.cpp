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
/// \file NurbsTextureCoordinate.cpp
/// \brief CPP file for NurbsTextureCoordinate, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/NurbsTextureCoordinate.h>

using namespace H3D;

H3DNodeDatabase NurbsTextureCoordinate::database( 
                                   "NurbsTextureCoordinate", 
                                   &(newInstance<NurbsTextureCoordinate>),
                                   typeid( NurbsTextureCoordinate ),
                                   &X3DGeometricPropertyNode::database );

namespace NurbsTextureCoordinateInternals {
  FIELDDB_ELEMENT( NurbsTextureCoordinate, controlPoint, INPUT_OUTPUT );;
  FIELDDB_ELEMENT( NurbsTextureCoordinate, weight, INPUT_OUTPUT );
  FIELDDB_ELEMENT( NurbsTextureCoordinate, uDimension, INPUT_OUTPUT );
  FIELDDB_ELEMENT( NurbsTextureCoordinate, vDimension, INPUT_OUTPUT );
  FIELDDB_ELEMENT( NurbsTextureCoordinate, uKnot, INPUT_OUTPUT );
  FIELDDB_ELEMENT( NurbsTextureCoordinate, vKnot, INPUT_OUTPUT );
  FIELDDB_ELEMENT( NurbsTextureCoordinate, uOrder, INPUT_OUTPUT );
  FIELDDB_ELEMENT( NurbsTextureCoordinate, vOrder, INPUT_OUTPUT );
}


NurbsTextureCoordinate::NurbsTextureCoordinate( 
                                 Inst< SFNode    >  _metadata,
                                 Inst< MFVec2f  > _controlPoint,
                                 Inst< MFFloat  > _weight,
                                 Inst< SFInt32  > _uDimension,
                                 Inst< SFInt32  > _vDimension,
                                 Inst< MFDouble > _uKnot,
                                 Inst< MFDouble > _vKnot,
                                 Inst< SFInt32  > _uOrder,
                                 Inst< SFInt32  > _vOrder ) :
  X3DGeometricPropertyNode( _metadata ),
  controlPoint( _controlPoint ),
  weight( _weight ),
  uDimension( _uDimension ),
  vDimension( _vDimension ),
  uKnot( _uKnot ),
  vKnot( _vKnot ),
  uOrder( _uOrder ),
  vOrder( _vOrder ) {

  type_name = "NurbsTextureCoordinate";
  database.initFields( this );
  
  withWeights = NULL;
  u_knots = NULL;
  v_knots = NULL;

  uDimension->setValue( 0 );
  vDimension->setValue( 0 );
  uOrder->setValue( 3 );
  vOrder->setValue( 3 );

  controlPoint->route(propertyChanged);
  weight->route(propertyChanged);
  uDimension->route(propertyChanged);
  vDimension->route(propertyChanged);
  uKnot->route(propertyChanged);
  vKnot->route(propertyChanged);
  uOrder->route(propertyChanged);
  vOrder->route(propertyChanged);
}

  void NurbsTextureCoordinate::renderTexForNurbs( GLUnurbsObj *nurbs_object ) {
  H3DInt32 v_order = vOrder->getValue();
  H3DInt32 u_order = uOrder->getValue();
  H3DInt32 v_dimension = vDimension->getValue();
  H3DInt32 u_dimension = uDimension->getValue();

  // vOrder has to be 2 or greater to define the NurbsTextureCoordinate
  if( v_order < 2 ) {
    Console(3) << "Warning: vOrder is less than 2 "
               << "in NurbsTextureCoordinate node( "
               << getName() << "). Node will not be rendered. " << endl;
    return;
  }
  
  // uOrder has to be 2 or greater to define the NurbsTextureCoordinate
  if( u_order < 2 ) {
    Console(3) << "Warning: uOrder is less than 2 "
               << "in NurbsTextureCoordinate node( "
               << getName() << "). Node will not be rendered. " << endl;
    return;
  }

  if( v_dimension < v_order )  {
    Console(3) << "Warning: vDimension is less than vOrder "
               << "in NurbsTextureCoordinate node( "
               << getName() << "). Node will not be rendered. " << endl;
    return;
  }

  if( u_dimension < u_order )  {
    Console(3) << "Warning: uDimension is less than uOrder in "
               << "NurbsTextureCoordinate node( "
               << getName() << "). Node will not be rendered. " << endl;
    return;
  }

  if(u_knots != NULL ) 
    delete [] u_knots;
  u_knots = new GLfloat[ uKnot->size() ];

  if(v_knots != NULL ) 
    delete [] v_knots;
  v_knots = new GLfloat[ vKnot->size() ];

  const vector< H3DFloat > &theWeights = weight->getValue();
  const vector< Vec2f > &noWeights = controlPoint->getValue();

  if( noWeights.size() != (unsigned int) (u_dimension * v_dimension ) ){
    Console(3) << "Warning: The size of controlPoint does not match "
               << "vDimension * uDimension in NurbsTextureCoordinate node( "
               << getName() << "). Node will not be rendered. " << endl;
    return;
  }

  // construct the array with textureCoordinates.
  H3DInt32 sizeOfVertex = 3;
  if(withWeights != NULL ) 
    delete [] withWeights;
  withWeights = new GLfloat[ noWeights.size() * sizeOfVertex ];
  for( unsigned int i = 0; i < noWeights.size(); ++i ) {
    withWeights[ i * sizeOfVertex ] = (GLfloat)noWeights[i].x;
    withWeights[ i * sizeOfVertex + 1 ] = (GLfloat)noWeights[i].y;
    if ( theWeights.size() == 0 ||
         theWeights.size() < (unsigned int)( u_dimension * v_dimension ) ) {
      withWeights[ i * sizeOfVertex + 2 ] = (GLfloat)1.0;
    } else {
      withWeights[ i * sizeOfVertex + 2 ] = (GLfloat)theWeights[i];
    }
  }

  const vector< H3DDouble > &uk = uKnot->getValue();
  const vector< H3DDouble > &vk = vKnot->getValue();

  // check the knots in the u direction.
  bool generateUniform = true;
  if( uk.size() == (unsigned int)(u_dimension + u_order) )  {
    generateUniform = false;
    H3DInt32 consecutiveKnots = 0;
    for( unsigned int i = 0; i < uk.size(); ++i ){
      u_knots[i] = (GLfloat)uk[i];        
      if( i > 0 ) {
        if( u_knots[i] == u_knots[ i - 1 ] )
          ++consecutiveKnots;
        else
          consecutiveKnots = 0;
        if( consecutiveKnots > u_order - 1 )
          generateUniform = true;

        if( u_knots[ i - 1 ] > u_knots[i] )
          generateUniform = true;
      }
    }
  }

  if( generateUniform ) {
    H3DInt32 theSizeToUse = u_dimension + u_order ;
    if(u_knots != NULL ) 
      delete [] u_knots;
    u_knots = new GLfloat[ theSizeToUse ];
    Console(3) << "Warning: The uKnot array is not according to standard in NurbsTextureCoordinate node( "
      << getName() << "). A default uKnot array will be generated. " << endl;
    for( int i = 0; i < theSizeToUse; ++i )
      u_knots[i] = (GLfloat)( (H3DDouble)i / ( theSizeToUse - 1 ) );
  }

  // check the knots in the v direction.
  generateUniform = true;
  if( vk.size() == (unsigned int)(v_dimension + v_order) )  {
    generateUniform = false;
    H3DInt32 consecutiveKnots = 0;
    for( unsigned int i = 0; i < vk.size(); ++i ){
      v_knots[i] = (GLfloat)vk[i];        
      if( i > 0 ) {
        if( v_knots[i] == v_knots[ i - 1 ] )
          ++consecutiveKnots;
        else
          consecutiveKnots = 0;
        if( consecutiveKnots > v_order - 1 )
          generateUniform = true;

        if( v_knots[ i - 1 ] > v_knots[i] )
          generateUniform = true;
      }
    }
  }

  if( generateUniform ) {
    H3DInt32 theSizeToUse = v_dimension + v_order ;
    if(v_knots != NULL ) 
      delete [] v_knots;
    v_knots = new GLfloat[ theSizeToUse ];
    Console(3) << "Warning: The vKnot array is not according to standard in NurbsTextureCoordinate node( "
      << getName() << "). A default vKnot array will be generated. " << endl;
    for( int i = 0; i < theSizeToUse; ++i )
      v_knots[i] = (GLfloat)( (H3DDouble)i / ( theSizeToUse - 1 ) );
  }

  // make the call to openGL.
  gluNurbsSurface( nurbs_object, 
    uKnot->size(), u_knots, 
    vKnot->size(), v_knots,
    sizeOfVertex, sizeOfVertex*u_dimension,
    withWeights,
    u_order, v_order,
    GL_MAP2_TEXTURE_COORD_2 );
}
