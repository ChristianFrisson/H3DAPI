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
/// \file SuperShape.cpp
/// \brief CPP file for SuperShape, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/SuperShape.h>
#include "H3D/Coordinate.h"
#include "H3D/Normal.h"
#include "H3D/MultiTexture.h"
#include "H3D/GlobalSettings.h"
#include "H3D/GraphicsOptions.h"

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase SuperShape::database( 
                                     "SuperShape", 
                                     &(newInstance<SuperShape>), 
                                     typeid( SuperShape ),
                                     &X3DGeometryNode::database );

namespace SuperShapeInternals {
  FIELDDB_ELEMENT( SuperShape, ss1_m, INPUT_OUTPUT );
  FIELDDB_ELEMENT( SuperShape, ss1_a, INPUT_OUTPUT );
  FIELDDB_ELEMENT( SuperShape, ss1_b, INPUT_OUTPUT );
  FIELDDB_ELEMENT( SuperShape, ss1_n1, INPUT_OUTPUT );
  FIELDDB_ELEMENT( SuperShape, ss1_n2, INPUT_OUTPUT );
  FIELDDB_ELEMENT( SuperShape, ss1_n3, INPUT_OUTPUT );
  FIELDDB_ELEMENT( SuperShape, ss2_m, INPUT_OUTPUT );
  FIELDDB_ELEMENT( SuperShape, ss2_a, INPUT_OUTPUT );
  FIELDDB_ELEMENT( SuperShape, ss2_b, INPUT_OUTPUT );
  FIELDDB_ELEMENT( SuperShape, ss2_n1, INPUT_OUTPUT );
  FIELDDB_ELEMENT( SuperShape, ss2_n2, INPUT_OUTPUT );
  FIELDDB_ELEMENT( SuperShape, ss2_n3, INPUT_OUTPUT );
  FIELDDB_ELEMENT( SuperShape, resolution, INPUT_OUTPUT );
}


SuperShape::SuperShape( 
                       Inst< SFNode >  _metadata,
                       Inst< SFBound     > _bound,
                       Inst< SFFloat  > _ss1_m,
                       Inst< SFFloat  > _ss1_a,
                       Inst< SFFloat  > _ss1_b,
                       Inst< SFFloat  > _ss1_n1,
                       Inst< SFFloat  > _ss1_n2,
                       Inst< SFFloat  > _ss1_n3,

                       Inst< SFFloat  > _ss2_m,
                       Inst< SFFloat  > _ss2_a,
                       Inst< SFFloat  > _ss2_b,
                       Inst< SFFloat  > _ss2_n1,
                       Inst< SFFloat  > _ss2_n2,
                       Inst< SFFloat  > _ss2_n3,
                       Inst< SFInt32  > _resolution ) : 
  X3DGeometryNode( _metadata, _bound ),
  ss1_m ( _ss1_m  ), ss1_a ( _ss1_a  ), ss1_b ( _ss1_b  ),
  ss1_n1( _ss1_n1 ), ss1_n2( _ss1_n2 ), ss1_n3( _ss1_n3 ),
  ss2_m ( _ss2_m  ), ss2_a ( _ss2_a  ), ss2_b ( _ss2_b  ),
  ss2_n1( _ss2_n1 ), ss2_n2( _ss2_n2 ), ss2_n3( _ss2_n3 ),
  resolution( _resolution ),
  coord( new SFCoordinateNode ),
  normal( new SFNormalNode ) {

  type_name = "SuperShape";
  database.initFields( this );

  coord->setValue( new Coordinate );
  coord->setOwner( this );
  coord->setName( "coord" );
  normal->setValue( new Normal );

  ss1_m->setValue ( 4 );
  ss1_a->setValue ( 1 );
  ss1_b->setValue ( 1 );
  ss1_n1->setValue( 100 );
  ss1_n2->setValue( 100 );
  ss1_n3->setValue( 100 );

  ss2_m->setValue ( 4 );
  ss2_a->setValue ( 1 );
  ss2_b->setValue ( 1 );
  ss2_n1->setValue( 100 );
  ss2_n2->setValue( 100 );
  ss2_n3->setValue( 100 );

  resolution->setValue( 128 );

  ss1_m->route( coord );
  ss1_a->route( coord );
  ss1_b->route( coord );
  ss1_n1->route( coord );
  ss1_n2->route( coord );
  ss1_n3->route( coord );
   
  ss2_m->route( coord );
  ss2_a->route( coord );
  ss2_b->route( coord );
  ss2_n1->route( coord );
  ss2_n2->route( coord );
  ss2_n3->route( coord );
   
  resolution->route( coord );

  coord->route( bound );
  coord->route( displayList );
}

void SuperShape::render() {
  X3DGeometryNode::render();
  H3DInt32 res = resolution->getValue();
  if( res > 0 ) {

    bool prefer_vertex_buffer_object = false;
    if( GLEW_ARB_vertex_buffer_object ) {
      GraphicsOptions * go = NULL;
      getOptionNode( go );
      if( !go ) {
        GlobalSettings * gs = GlobalSettings::getActive();
        if( gs ) {
          gs->getOptionNode( go );
        }
      }
      if( go ) {
        prefer_vertex_buffer_object =
          go->preferVertexBufferObject->getValue();
      }
    }

    if( prefer_vertex_buffer_object ) {
      coord->getValue()->renderVertexBufferObject();
      normal->getValue()->renderVertexBufferObject();
    } else {
  //    X3DTextureCoordinateNode *tex_coord = 0;
      // always generate texture coordinate from bounding box
      //    startTexGen();
      coord->getValue()->renderArray();
      normal->getValue()->renderArray();
    }

    H3DInt32 size = res * ( res + 1 ) * 2;
    // draw each triangle strip from the arrays
    for( int i = 0; i < res; ++i ) {
      // render the triangle strip
      glDrawArrays( GL_TRIANGLE_STRIP, 
                    size * i / res, 
                    res * 2 + 2 );
    }

    if( prefer_vertex_buffer_object ) {
      coord->getValue()->disableVertexBufferObject();
      normal->getValue()->disableVertexBufferObject();
    } else {
      coord->getValue()->disableArray();
      normal->getValue()->disableArray();
    }
      //    stopTexGen();
  }
}

void SuperShape::traverseSG( TraverseInfo &ti ) {
  useBackFaceCulling( true );
  X3DGeometryNode::traverseSG( ti );
}


void SuperShape::SFCoordinateNode::update() {
  
  H3DFloat ss1_m = static_cast< SFFloat * >(routes_in[0])->getValue();
  H3DFloat ss1_a = static_cast< SFFloat * >(routes_in[1])->getValue();
  H3DFloat ss1_b = static_cast< SFFloat * >(routes_in[2])->getValue();
  H3DFloat ss1_n1 = static_cast< SFFloat * >(routes_in[3])->getValue();
  H3DFloat ss1_n2 = static_cast< SFFloat * >(routes_in[4])->getValue();
  H3DFloat ss1_n3 = static_cast< SFFloat * >(routes_in[5])->getValue();

  H3DFloat ss2_m = static_cast< SFFloat * >(routes_in[6])->getValue();
  H3DFloat ss2_a = static_cast< SFFloat * >(routes_in[7])->getValue();
  H3DFloat ss2_b = static_cast< SFFloat * >(routes_in[8])->getValue();
  H3DFloat ss2_n1 = static_cast< SFFloat * >(routes_in[9])->getValue();
  H3DFloat ss2_n2 = static_cast< SFFloat * >(routes_in[10])->getValue();
  H3DFloat ss2_n3 = static_cast< SFFloat * >(routes_in[11])->getValue();

  H3DInt32 res = static_cast< SFInt32 * >(routes_in[12])->getValue();

  H3DFloat long_step =  2 * (H3DFloat)(Constants::pi) / res;
  H3DFloat lat_step = (H3DFloat)(Constants::pi) / res;
  Vec3f vmax = Vec3f( 0, 0, 0 );
  Vec3f vmin = Vec3f( 0, 0, 0 );

  vector< Vec3f > the_points;
  vector< Vec3f > normals;
  
  // Generate coordinates as if it was several trianglestrips
  for(int lat_count = 0; lat_count < res; ++lat_count )
    {
      H3DFloat phi1 = (H3DFloat)lat_count * lat_step - 
                      ( (H3DFloat)(Constants::pi)/2 );
      H3DFloat phi2 = phi1 + lat_step;
      H3DFloat phi3 = phi2 + lat_step;
      H3DFloat phi0 = phi1 - lat_step;

      Vec3f fa;
      Vec3f fc;
      // New triangle strip is assumed from here.
      for(int long_count = 0;long_count<=res; ++long_count) {
        H3DFloat theta1 = (H3DFloat)long_count * long_step -
                          (H3DFloat)(Constants::pi);
        H3DFloat theta2 = theta1 + long_step;
        H3DFloat theta0 = theta1 - long_step;
        Vec3f p10 = SS( theta1, phi0,
                        ss1_m,  ss1_a,  
                        ss1_b,  ss1_n1, 
                        ss1_n2, ss1_n3, 
                        ss2_m,  ss2_a,  
                        ss2_b,  ss2_n1, 
                        ss2_n2, ss2_n3 );

        Vec3f p01 = SS( theta0, phi1,
                        ss1_m,  ss1_a,  
                        ss1_b,  ss1_n1, 
                        ss1_n2, ss1_n3, 
                        ss2_m,  ss2_a,  
                        ss2_b,  ss2_n1, 
                        ss2_n2, ss2_n3);
        Vec3f p11 = SS(theta1, phi1,
                       ss1_m,  ss1_a,  
                       ss1_b,  ss1_n1, 
                       ss1_n2, ss1_n3, 
                       ss2_m,  ss2_a,  
                       ss2_b,  ss2_n1, 
                       ss2_n2, ss2_n3);
        Vec3f p21 = SS(theta2, phi1,
                       ss1_m,  ss1_a,  ss1_b,
                       ss1_n1, ss1_n2, ss1_n3, 
                       ss2_m,  ss2_a,  ss2_b,
                       ss2_n1, ss2_n2, ss2_n3);

        Vec3f p02 = SS(theta0, phi2,
                       ss1_m,  ss1_a,  ss1_b,
                       ss1_n1, ss1_n2, ss1_n3, 
                       ss2_m,  ss2_a,  ss2_b,
                       ss2_n1, ss2_n2, ss2_n3);
        Vec3f p12 = SS(theta1, phi2,
                       ss1_m,  ss1_a,  ss1_b,
                       ss1_n1, ss1_n2, ss1_n3, 
                       ss2_m,  ss2_a,  ss2_b,
                       ss2_n1, ss2_n2, ss2_n3);
        Vec3f p22 = SS(theta2, phi2,
                       ss1_m,  ss1_a,  ss1_b,
                       ss1_n1, ss1_n2, ss1_n3, 
                       ss2_m,  ss2_a,  ss2_b,
                       ss2_n1, ss2_n2, ss2_n3);

        Vec3f p13 = SS(theta1, phi3,
                       ss1_m,  ss1_a,  ss1_b,
                       ss1_n1, ss1_n2, ss1_n3, 
                       ss2_m,  ss2_a,  ss2_b,
                       ss2_n1, ss2_n2, ss2_n3);
            
        Vec3f n;
        n =  (p12-p02).crossProduct(p12-p11);
        n += (p12-p11).crossProduct(p12-p22);
        n += (p12-p22).crossProduct(p12-p13);
        n += (p12-p13).crossProduct(p12-p02);
        n.normalizeSafe();
        normals.push_back( n );
        the_points.push_back( p12 );
      
        n =  (p11-p01).crossProduct(p11-p10);
        n += (p11-p10).crossProduct(p11-p21);
        n += (p11-p21).crossProduct(p11-p12);
        n += (p11-p12).crossProduct(p11-p01);
      
        n.normalizeSafe();
        normals.push_back( n );
        the_points.push_back( p11 );
      
        if ( p11.x > vmax.x ) vmax.x = p11.x;
        if ( p11.y > vmax.y ) vmax.y = p11.y;
        if ( p11.z > vmax.z ) vmax.z = p11.z;
        if ( p12.x > vmax.x ) vmax.x = p12.x;
        if ( p12.y > vmax.y ) vmax.y = p12.y;
        if ( p12.z > vmax.z ) vmax.z = p12.z;

        if ( p11.x < vmin.x ) vmin.x = p11.x;
        if ( p11.y < vmin.y ) vmin.y = p11.y;
        if ( p11.z < vmin.z ) vmin.z = p11.z;
        if ( p12.x < vmin.x ) vmin.x = p12.x;
        if ( p12.y < vmin.y ) vmin.y = p12.y;
        if ( p12.z < vmin.z ) vmin.z = p12.z;
      }
  }
  Coordinate * _coord = new Coordinate();
  _coord->point->setValue( the_points );
  value = _coord;
  SuperShape *ss = static_cast< SuperShape * >( getOwner() );
  static_cast< Normal * >( ss->normal->getValue() )->vector->setValue( normals );
  
}


void SuperShape::startTexGen() {
  BoxBound *box_bound = dynamic_cast< BoxBound * >( bound->getValue() );
  if( box_bound ) {
    const Vec3f &center = box_bound->center->getValue();
    const Vec3f &size = box_bound->size->getValue();

    H3DFloat sparams[4] = {0,0,0,0};
    H3DFloat tparams[4] = {0,0,0,0};
    H3DFloat rparams[4] = {0,0,0,0};

    H3DFloat size_vec[]   = { size.x, size.y, size.z };
    H3DFloat center_vec[] = { center.x, center.y, center.z };

    // these variables are set to an index representing 
    // sides of the bounding box. 0 is the x-axis, 1 the y-axis
    // and 2 the z-axis.
    int largest_side, middle_side, smallest_side;

    if( size.x >= size.y ) {
      if( size.x >= size.z ) {
        largest_side = 0; 
        if( size.y >= size.z ) {
          // size.x >= size.y >= size.z
          middle_side   = 1;
          smallest_side = 2;
        } else { 
          // size.x >= size.z > size.y
          middle_side   = 2;
          smallest_side = 1;
        }
      } else {
        // size.z > size.x >= size.y
        largest_side  = 2; 
        middle_side   = 0;
        smallest_side = 1;
      }
    } else {
      if( size.z >= size.y ) {
        // size.z >= size.y > size.x
        largest_side  = 2; 
        middle_side   = 1;
        smallest_side = 0;
      } else if( size.x >= size.z ) {
        // size.y > size.x >=size.z
        largest_side  = 0; 
        middle_side   = 2;
        smallest_side = 1;
      } else {
        // size.y > size.z > size.x
        largest_side  = 1; 
        middle_side   = 2;
        smallest_side = 0;
      }
    }

    H3DFloat largest_length = size_vec[ largest_side ];
    if( H3DAbs( largest_length ) > Constants::f_epsilon ) {
      // parameters for the s coordinate
      H3DFloat length_inv = 1/largest_length;
      sparams[ largest_side ] = length_inv;
      sparams[3] = 0.5f - center_vec[ largest_side ] / largest_length;

      // parameters for the t coordinate
      tparams[ middle_side ] = length_inv;
      H3DFloat tcenter = size_vec[ middle_side ] / (2*largest_length);
      tparams[3] = tcenter - center_vec[ middle_side ] / largest_length;

      // parameters for the r coordinate
      rparams[ smallest_side ] = -length_inv;
      H3DFloat rcenter = size_vec[ smallest_side ] / (2*largest_length);
      rparams[3] = rcenter + center_vec[ smallest_side ] / largest_length;
    } else {
      sparams[3] = 0.5;
      tparams[3] = size_vec[ middle_side ] / (2*largest_length);
      rparams[3] = size_vec[ smallest_side ] / (2*largest_length);
    }

    // TODO: maybe let the texture handle the texture coordinate
    // generation. Wait until implementation of TextureCoordinateGenerator
    // before deciding on how to do it.
    MultiTexture *mt = 
      dynamic_cast< MultiTexture * >( X3DTextureNode::getActiveTexture() );
    if( mt ) {
      size_t texture_units = mt->texture->size();
      for( size_t i = 0; i < texture_units; ++i ) {
        glActiveTexture( GL_TEXTURE0_ARB + (unsigned int) i );
        glTexGend( GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR );
        glTexGend( GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR );
        glTexGend( GL_R, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR );
        glTexGenfv( GL_S, GL_OBJECT_PLANE, sparams );
        glTexGenfv( GL_T, GL_OBJECT_PLANE, tparams );
        glTexGenfv( GL_R, GL_OBJECT_PLANE, rparams );
        glEnable( GL_TEXTURE_GEN_S );
        glEnable( GL_TEXTURE_GEN_T );
        glEnable( GL_TEXTURE_GEN_R );
      }
    } else {
      glTexGend( GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR );
      glTexGend( GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR );
      glTexGend( GL_R, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR );
      glTexGenfv( GL_S, GL_OBJECT_PLANE, sparams );
      glTexGenfv( GL_T, GL_OBJECT_PLANE, tparams );
      glTexGenfv( GL_R, GL_OBJECT_PLANE, rparams );
      glEnable( GL_TEXTURE_GEN_S );
      glEnable( GL_TEXTURE_GEN_T );
      glEnable( GL_TEXTURE_GEN_R );
    }
  } else {
    stringstream s;
    s << "Could not start texture coordinate generation in IndexedFaceSet. "
      << "Requires bound object of BoxBound type. ";
    throw Exception::H3DAPIException( s.str(), H3D_FULL_LOCATION );
  }
}

void SuperShape::stopTexGen() {
  MultiTexture *mt = 
    dynamic_cast< MultiTexture * >( X3DTextureNode::getActiveTexture() );
  if( mt ) {
    size_t texture_units = mt->texture->size();
    for( size_t i = 0; i < texture_units; ++i ) {
      glActiveTexture( GL_TEXTURE0_ARB + (unsigned int) i );
      glDisable( GL_TEXTURE_GEN_S );
      glDisable( GL_TEXTURE_GEN_T );
      glDisable( GL_TEXTURE_GEN_R );
    }
  } else {
    glDisable( GL_TEXTURE_GEN_S );
    glDisable( GL_TEXTURE_GEN_T );
    glDisable( GL_TEXTURE_GEN_R );
  }
}
