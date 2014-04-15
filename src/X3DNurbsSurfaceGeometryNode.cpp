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
/// \file X3DNurbsSurfaceGeometryNode.cpp
/// \brief CPP file for X3DNurbsSurfaceGeometryNode, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/X3DNurbsSurfaceGeometryNode.h>
#include <H3D/Coordinate.h>

using namespace H3D;

H3DNodeDatabase X3DNurbsSurfaceGeometryNode::database( 
                                   "X3DNurbsSurfaceGeometryNode", 
                                   NULL,
                                   typeid( X3DNurbsSurfaceGeometryNode ),
                                   &X3DParametricGeometryNode::database );

namespace X3DNurbsSurfaceGeometryNodeInternals {
  FIELDDB_ELEMENT( X3DNurbsSurfaceGeometryNode, controlPoint, INPUT_OUTPUT );
  FIELDDB_ELEMENT( X3DNurbsSurfaceGeometryNode, texCoord, INPUT_OUTPUT );
  FIELDDB_ELEMENT( X3DNurbsSurfaceGeometryNode, uTessellation, INPUT_OUTPUT );
  FIELDDB_ELEMENT( X3DNurbsSurfaceGeometryNode, vTessellation, INPUT_OUTPUT );
  FIELDDB_ELEMENT( X3DNurbsSurfaceGeometryNode, weight, INPUT_OUTPUT );
  FIELDDB_ELEMENT( X3DNurbsSurfaceGeometryNode, solid, INPUT_OUTPUT );
  FIELDDB_ELEMENT( X3DNurbsSurfaceGeometryNode, uClosed, INPUT_OUTPUT );
  FIELDDB_ELEMENT( X3DNurbsSurfaceGeometryNode, vClosed, INPUT_OUTPUT );
  FIELDDB_ELEMENT( X3DNurbsSurfaceGeometryNode, uDimension, INPUT_OUTPUT );
  FIELDDB_ELEMENT( X3DNurbsSurfaceGeometryNode, vDimension, INPUT_OUTPUT );
  FIELDDB_ELEMENT( X3DNurbsSurfaceGeometryNode, uKnot, INPUT_OUTPUT );
  FIELDDB_ELEMENT( X3DNurbsSurfaceGeometryNode, vKnot, INPUT_OUTPUT );
  FIELDDB_ELEMENT( X3DNurbsSurfaceGeometryNode, uOrder, INPUT_OUTPUT );
  FIELDDB_ELEMENT( X3DNurbsSurfaceGeometryNode, vOrder, INPUT_OUTPUT );
}


X3DNurbsSurfaceGeometryNode::X3DNurbsSurfaceGeometryNode( 
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
  X3DParametricGeometryNode( _metadata, _bound, _displayList, _isTouched,
                             _force, _contactPoint, _contactNormal ),
  controlPoint( _controlPoint ),
  texCoord( _texCoord ),
  uTessellation( _uTessellation ),
  vTessellation( _vTessellation ),
  weight( _weight ),
  solid( _solid ),
  uClosed( _uClosed ),
  vClosed( _vClosed ),
  uDimension( _uDimension ),
  vDimension( _vDimension ),
  uKnot( _uKnot ),
  vKnot( _vKnot ),
  uOrder( _uOrder ),
  vOrder( _vOrder ),
  nurbs_object( NULL ),
  printWarning( new Field ) {

  type_name = "X3DNurbsSurfaceGeometryNode";
  database.initFields( this );
  
  uTessellation->setValue( 0 );
  vTessellation->setValue( 0 );
  solid->setValue( true );
  uDimension->setValue( 0 );
  vDimension->setValue( 0 );
  uClosed->setValue( false );
  vClosed->setValue( false );
  uOrder->setValue( 3 );
  vOrder->setValue( 3 );

  controlPoint->route( bound );

  controlPoint->route( displayList );
  texCoord->route( displayList );
  uTessellation->route( displayList );
  vTessellation->route( displayList );
  weight->route( displayList );
  solid->route( displayList );
  uClosed->route( displayList );
  vClosed->route( displayList );
  uDimension->route( displayList );
  vDimension->route( displayList );
  uKnot->route( displayList );
  vKnot->route( displayList );
  uOrder->route( displayList );
  vOrder->route( displayList );

  vOrder->route( printWarning );
  uOrder->route( printWarning );
  vDimension->route( printWarning );
  uDimension->route( printWarning );
  weight->route( printWarning );
  uKnot->route( printWarning );
  vKnot->route( printWarning );
  controlPoint->route( printWarning );
}

void X3DNurbsSurfaceGeometryNode::render( ) {
  H3DInt32 v_order = vOrder->getValue();
  H3DInt32 u_order = uOrder->getValue();
  H3DInt32 v_dimension = vDimension->getValue();
  H3DInt32 u_dimension = uDimension->getValue();

  X3DTextureCoordinateNode *tex_coord_node = 
    dynamic_cast< X3DTextureCoordinateNode * > ( texCoord->getValue() );
  NurbsTextureCoordinate *nurbs_tex_coord = 
    dynamic_cast< NurbsTextureCoordinate * >( texCoord->getValue() );
  TextureCoordinateGenerator *tex_coord_gen = 
    dynamic_cast< TextureCoordinateGenerator * >( tex_coord_node );

  // vOrder has to be 2 or greater to define the nurbssurfacepatch
  if( v_order < 2 ) {
    if( !printWarning->isUpToDate() ) {
      Console(3) << "Warning: vOrder is less than 2 in " << getTypeName()
                 << " node( "  << getName() 
                 << "). Node will not be rendered. " << endl;
      printWarning->upToDate();
    }
    return;
  }

  // uOrder has to be 2 or greater to define the nurbssurfacepatch
  if( u_order < 2 ) {
    if( !printWarning->isUpToDate() ) {
      Console(3) << "Warning: uOrder is less than 2 in " << getTypeName()
                 << " node( "  << getName() 
                 << "). Node will not be rendered. " << endl;
      printWarning->upToDate();
    }
    return;
  }

  // vDimension has to be at least vOrder or greater 
  // to define the nurbssurfacepatch
  if( v_dimension < v_order )  {
    if( !printWarning->isUpToDate() ) {
      Console(3) << "Warning: vDimension is less than vOrder in "
        << getTypeName() << " node( "
        << getName() << "). Node will not be rendered. " << endl;
      printWarning->upToDate();
    }
    return;
  }

  // uDimension has to be at least uOrder or greater 
  // to define the nurbssurfacepatch
  if( u_dimension < u_order )  {
    if( !printWarning->isUpToDate() ) {
      Console(3) << "Warning: uDimension is less than uOrder in "
        << getTypeName() << " node( "
        << getName() << "). Node will not be rendered. " << endl;
      printWarning->upToDate();
    }
    return;
  }

  // create a new NurbsRendererer
  if( !nurbs_object ) nurbs_object = gluNewNurbsRenderer();
  Coordinate *coord_node = 
    dynamic_cast< Coordinate * >( controlPoint->getValue() );

  if( coord_node ) {

    GLfloat *u_knots = new GLfloat[ uKnot->size() ];
    GLfloat *v_knots = new GLfloat[ vKnot->size() ];

    const vector< H3DDouble > &theWeights = weight->getValue();
    const vector< Vec3f > &noWeights = coord_node->point->getValue();

    // another check to see that the nurbssurfacepatch is correctly defined
    if( noWeights.size() != (unsigned int)(u_dimension * v_dimension) )  {
      if( !printWarning->isUpToDate() ) {
        Console(3) << "Warning: The size of controlPoint does not match "
          << "vDimension * uDimension in " << getTypeName() << " node( "
          << getName() << "). Node will not be rendered. " << endl;
        printWarning->upToDate();
      }
      return;
    }


    // depending on if we have weights or not we can define withWeights
    // to be different sizes.
    GLfloat *withWeights;
    H3DInt32 map2Vertex3Or4;
    H3DInt32 sizeOfVertex;
    if( theWeights.size() < noWeights.size() ) {
      if( !printWarning->isUpToDate() ) {
        Console(3) << "Warning: The number of weight values is less than "
          << "the number of control points in " << getTypeName() << " node( "
          << getName() << "). Default weight 1.0 is assumed." << endl;
      }
      sizeOfVertex = 3;
      map2Vertex3Or4 = GL_MAP2_VERTEX_3;
      withWeights = new GLfloat[ noWeights.size() * sizeOfVertex ];
      for( unsigned int i = 0; i < noWeights.size(); ++i ) {
        withWeights[ i * sizeOfVertex ] = (GLfloat)noWeights[i].x;
        withWeights[ i * sizeOfVertex + 1 ] = (GLfloat)noWeights[i].y;
        withWeights[ i * sizeOfVertex + 2 ] = (GLfloat)noWeights[i].z;
      }
    }
    else {
      sizeOfVertex = 4;
      map2Vertex3Or4 = GL_MAP2_VERTEX_4;
      withWeights = new GLfloat[ noWeights.size() * sizeOfVertex ];
      for( unsigned int i = 0; i < noWeights.size(); ++i ) {
        withWeights[ i * sizeOfVertex ] = (GLfloat)noWeights[i].x;
        withWeights[ i * sizeOfVertex + 1 ] = (GLfloat)noWeights[i].y;
        withWeights[ i * sizeOfVertex + 2 ] = (GLfloat)noWeights[i].z;
        withWeights[ i * sizeOfVertex + 3 ] = (GLfloat)theWeights[i];
      }
    }

    const vector< H3DDouble > &uk = uKnot->getValue();
    const vector< H3DDouble > &vk = vKnot->getValue();

    // check the knot-vectors. If they are not according to standard
    // default uniform knot vectors will be generated.
    bool generateUniform = true;
    if( uk.size() == (unsigned int)(u_dimension + u_order) )  {
      generateUniform = false;
      H3DInt32 consecutiveKnots = 0;
      for( unsigned int i = 0; i < uk.size(); ++i ) {
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

    H3DInt32 uSizeToUse = (H3DInt32) uk.size() ;
    if( generateUniform ) {
      if( !printWarning->isUpToDate() && uSizeToUse != 0 ) {
        Console(3) << "Warning: The uKnot array is not according to standard in "
          << getTypeName() << " node( "
          << getName() << "). A default uKnot array will be generated. " << endl;
      }
      uSizeToUse = u_dimension + u_order;
      delete[] u_knots;
      u_knots = new GLfloat[ uSizeToUse ];
      for( int i = 0; i < uSizeToUse; ++i )
        u_knots[i] = (GLfloat)( (H3DDouble)i / ( uSizeToUse - 1 ) );
    }

    generateUniform = true;
    if( vk.size() == (unsigned int)(v_dimension + v_order) )  {
      generateUniform = false;
      H3DInt32 consecutiveKnots = 0;
      for( unsigned int i = 0; i < vk.size(); ++i ) {
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

    H3DInt32 vSizeToUse = (H3DInt32)vk.size() ;
    if( generateUniform ) {
      if( !printWarning->isUpToDate() && vSizeToUse != 0 ) {
        Console(3) << "Warning: The vKnot array is not according to standard in "
          << getTypeName() << " node( "
          << getName() << "). A default vKnot array will be generated. " << endl;
      }
      vSizeToUse = v_dimension + v_order;
      delete[] v_knots;
      v_knots = new GLfloat[ vSizeToUse ];
      for( int i = 0; i < vSizeToUse; ++i )
        v_knots[i] = (GLfloat)( (H3DDouble)i / ( vSizeToUse - 1 ) );
    }


    // If no texturecoordinatenode is defined, default unit square
    GLfloat *texCoordinates = NULL;
    GLfloat *texKnotU = NULL;
    GLfloat *texKnotV = NULL;
    if( !tex_coord_node && ! nurbs_tex_coord ) {
      texCoordinates = new GLfloat[ 12 ];
      texKnotU = new GLfloat[ 4 ];
      texKnotV = new GLfloat[ 4 ];

      texCoordinates[0] = (GLfloat)0;
      texCoordinates[1] = (GLfloat)0;

      texCoordinates[2] = (GLfloat)0;
      texCoordinates[3] = (GLfloat)1;

      texCoordinates[4] = (GLfloat)1;
      texCoordinates[5] = (GLfloat)0;

      texCoordinates[6] = (GLfloat)1;
      texCoordinates[7] = (GLfloat)1;

      texKnotU[0] = (GLfloat)u_knots[0];
      texKnotU[1] = (GLfloat)u_knots[0];
      texKnotU[2] = (GLfloat)u_knots[ uSizeToUse - 1 ];
      texKnotU[3] = (GLfloat)u_knots[ uSizeToUse - 1 ];

      texKnotV[0] = (GLfloat)v_knots[0];
      texKnotV[1] = (GLfloat)v_knots[0];
      texKnotV[2] = (GLfloat)v_knots[ vSizeToUse - 1 ];
      texKnotV[3] = (GLfloat)v_knots[ vSizeToUse - 1 ];
    }

    glEnable( GL_AUTO_NORMAL );

    gluNurbsProperty( nurbs_object, GLU_DISPLAY_MODE, GLU_FILL );
    gluBeginSurface( nurbs_object );

    renderBetweenBeginEnd(
                          nurbs_object,
                          tex_coord_node,
                          tex_coord_gen,
                          nurbs_tex_coord, 
                          texKnotU,
                          texKnotV,
                          texCoordinates, 
                          u_knots, 
                          v_knots, 
                          sizeOfVertex,
                          withWeights, 
                          uSizeToUse, 
                          vSizeToUse, 
                          map2Vertex3Or4 );

    gluEndSurface( nurbs_object );
    glDisable( GL_AUTO_NORMAL );
    delete [] u_knots;
    delete [] v_knots;
    delete [] withWeights;
    if( texCoordinates != NULL )
      delete []texCoordinates;
    delete [] texKnotU;
    if( texKnotV != NULL )
      delete [] texKnotV;
  }
  printWarning->upToDate();
}

void X3DNurbsSurfaceGeometryNode::traverseSG( TraverseInfo &ti ) {
  if( solid->getValue() ) {
    useBackFaceCulling( true );
  } else {
    useBackFaceCulling( false );
  }
  X3DParametricGeometryNode::traverseSG( ti );
}
