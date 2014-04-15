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
/// \file Extrusion.cpp
/// \brief CPP file for Extrusion, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/Extrusion.h>

using namespace H3D;

H3DNodeDatabase Extrusion::database( 
        "Extrusion", 
        &(newInstance< Extrusion > ),
        typeid( Extrusion ),
        &X3DGeometryNode::database );

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

  // returns true if the two points are coincident
  template <typename T>
  inline bool coinc(T a , T b) {
    return ( a - b ).length() < Constants::f_epsilon;
  }

  // calculate the normal to a face given three vertices in the face.
  inline Vec3f calculateNormal( const vector < Vec3f > &vertex_vector,
                                H3DInt32 right,
                                H3DInt32 middle,
                                H3DInt32 left) {
    return ( vertex_vector[ right ] - vertex_vector[ middle ] ).crossProduct(
             vertex_vector[ left ] - vertex_vector[ middle ] );
  }

  // Find the indices in the normals_per_face vector to the vertex defined
  // by i and j where 0 <= i < spine_size and 0 <= j < cross_section_size
  vector< H3DInt32 > findSurroundingNormalIndex( H3DInt32 i,
                                                 H3DInt32 j,
                                                 bool closed_spine,
                                                 H3DInt32 spine_size,
                                                 bool closed_cross_section,
                                                 H3DInt32 cross_section_size_minone,
                                                 int if_caps_add,
                                                 bool end_cap ) {
    H3DInt32 i0 = -1;
    H3DInt32 i_minone = -1;
    H3DInt32 j0 = -1;
    H3DInt32 j_minone = -1;
    if( i < spine_size - 1 )
      i0 = i;
    else if( closed_spine )
      i0 = 0;
    if( i > 0 )
      i_minone = i - 1;
    else if( closed_spine )
      i_minone = spine_size - 2;

    // Decrease cross_section_size to not have to write -1 everywhere we use it.
    if( j < cross_section_size_minone )
      j0 = j;
    else if( closed_cross_section )
      j0 = 0;
    if( j > 0 )
      j_minone = j - 1;
    else if( closed_cross_section )
      j_minone = cross_section_size_minone - 1;
    
    vector< H3DInt32 > found_indices;
    if( if_caps_add > 0 && i == 0 ) {
      found_indices.push_back( 0 );
    }

    if( j0 != -1 ) {
      if( i0 != -1 ) {
        found_indices.push_back( 2 * ( i0 * cross_section_size_minone + j0 ) + if_caps_add );
        found_indices.push_back( found_indices.back() + 1 );
      }

      if( i_minone != -1 ) {
        found_indices.push_back( 2 * ( i_minone * cross_section_size_minone + j0 ) + 1 + if_caps_add );
      }
    }

    if( j_minone != -1 ) {
      if( i0 != -1 ) {
        found_indices.push_back( 2 * ( i0 * cross_section_size_minone + j_minone ) + if_caps_add );
      }

      if( i_minone != -1 ) {
        found_indices.push_back( 2 * ( i_minone * cross_section_size_minone + j_minone ) + if_caps_add );
        found_indices.push_back( found_indices.back() + 1 );
      }
    }

    if( end_cap && i == spine_size - 1 ) {
      found_indices.push_back( ( spine_size - 1 ) * cross_section_size_minone * 2 + if_caps_add );
    }
    return found_indices;
  }
}

Extrusion::Extrusion(  Inst< SFNode      > _metadata,
                       Inst< SFBound     > _bound,
                       Inst< DisplayList > _displayList,
                       Inst< SFBool      > _beginCap,
                       Inst< SFBool      > _ccw,
                       Inst< SFBool      > _convex,
                       Inst< SFFloat     > _creaseAngle,
                       Inst< MFVec2f     > _crossSection,
                       Inst< SFBool      > _endCap,
                       Inst< MFRotation  > _orientation,
                       Inst< MFVec2f     > _scale,
                       Inst< SFBool      > _solid,
                       Inst< MFVec3f     > _spine ) :
  X3DGeometryNode( _metadata, _bound, _displayList ),
  beginCap       ( _beginCap     ),
  ccw             ( _ccw          ),
  convex         ( _convex       ),
  creaseAngle    ( _creaseAngle  ),
  crossSection   ( _crossSection ),
  endCap         ( _endCap       ),
  orientation    ( _orientation  ),
  scale           ( _scale        ),
  solid           ( _solid         ),
  spine           ( _spine         ),
  vertexVector( new VertexVector ),
  autoNormal( new AutoNormal ) {

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

  ccw->route( displayList, id );
  convex->route( displayList, id );
  solid->route( displayList, id );

  vertexVector->setName( "vertexVector" );
  vertexVector->setOwner( this );
  crossSection->route( vertexVector );
  orientation->route( vertexVector );
  scale->route( vertexVector );
  spine->route( vertexVector );

  vertexVector->route( bound );

  autoNormal->setName( "autoNormal" );
  autoNormal->setOwner( this );
  vertexVector->route( autoNormal );
  creaseAngle->route( autoNormal );
  beginCap->route( autoNormal );
  endCap->route( autoNormal );

  autoNormal->route( displayList, id );
}

void Extrusion::VertexVector::update() {
  value.clear();
  const vector< Vec2f > &cross_section = 
    static_cast< MFVec2f * >( routes_in[0] )->getValue();
  const vector< Rotation > &orn_vector = 
    static_cast< MFRotation * >( routes_in[1] )->getValue();
  const vector< Vec2f > &scale_vector = 
    static_cast< MFVec2f * >( routes_in[2] )->getValue();
  const vector< Vec3f > &spine_vector = 
    static_cast< MFVec3f * >( routes_in[3] )->getValue();

  Extrusion *extrusion = static_cast< Extrusion * >( getOwner() );

  H3DInt32 spine_size = (H3DInt32)spine_vector.size();
  H3DInt32 nr_of_cross_section_points = (H3DInt32)cross_section.size();
  H3DInt32 nr_of_scale_values = (H3DInt32)scale_vector.size();
  H3DInt32 nr_of_orn_values = (H3DInt32)orn_vector.size();

  if( spine_size > 1 ) {
    vector< Vec3f > x_axis;
    vector< Vec3f > y_axis;
    vector< Vec3f > z_axis;
    vector< int > spine_forward;
    vector< int > spine_backward;
    vector< Rotation > spine_orientations;

    bool closed_spine = false;
    bool collinear = true;        // if the spine is collinear
    bool coincident = false;      // if every point on the spine is the same

    // Orientation and scale values have to be 1 or >= the number of spine
    // points. If there are more scale or orientation values than spine points
    // the excess values are ignored.
    if( nr_of_orn_values < spine_size && nr_of_orn_values > 1 ) {
      Console(3)
        << "Warning: Not enough orientation values in Extrusion node( "
        << getName() << "). Node will not be rendered. " << endl;
      return;
    }

    if( nr_of_scale_values < spine_size && nr_of_scale_values > 1  ) {
      Console(3) << "Warning: Not enough scale values in Extrusion node( "
        << getName() << "). Node will not be rendered. " << endl;
      return;
    }

    // To not get problems with Vectors later on. The excess values
    // are ignored.
    if( nr_of_orn_values > spine_size )
      nr_of_orn_values = spine_size;
    if( nr_of_scale_values > spine_size )
      nr_of_scale_values = spine_size;

    // check if the spine is closed
    if( ExtrusionInternals::coinc( spine_vector.front(),
                                   spine_vector.back() ) )
      closed_spine = true;

    // build vectors containing indicies to the first non-coincident spine. 
    // Backward and forward.
    // these vectors are used to calculate axis for the SCP around each spine.
    for( int i = 0; i < spine_size; ++i ) {
      int j = i;

      do {
        ++j;

        if( j >= spine_size )
          j -= spine_size;
      } while( j!= i &&
               ExtrusionInternals::coinc( spine_vector[i], spine_vector[j] ) );

      if( !closed_spine && j < i )
        spine_forward.push_back( spine_size - 1 );
      else
        spine_forward.push_back(j);


      if( i == 0 && j == 0 )
        coincident = true;

      j = i;

      do {
        --j;

        if( j < 0 )
          j += spine_size;
      } while( j!= i && ExtrusionInternals::coinc( spine_vector[i],
                                                   spine_vector[j] ) );

      if( !closed_spine && j > i )
        spine_backward.push_back( 0 );
      else
        spine_backward.push_back(j);
    }

    // if the entire spine is coincident choose and arbitrary direction for
    // the SCP. If another direction is desired use the orientation values.
    // another node might be a better solution for this kind of shape.
    if( coincident ) {
      x_axis.push_back( Vec3f( 1, 0, 0 ) );
      y_axis.push_back( Vec3f( 0, 1, 0 ) );
      z_axis.push_back( Vec3f( 0, 0, 1 ) );

      for( int i = 0; i < spine_size; ++i )
      {
        x_axis.push_back( x_axis.front() );
        y_axis.push_back( y_axis.front() );
        z_axis.push_back( z_axis.front() );
      }
    } else {
      // calculate the Z-axes for the first spine point if the first spine
      // point is undetermined at this stage add dummy.
      if( closed_spine ) {
        Vec3f temp = ( spine_vector[ spine_forward.front() ] -
                       spine_vector[0] ).crossProduct( 
                       spine_vector[ spine_backward.front() ] -
                       spine_vector[0] );
        temp.normalizeSafe();
        z_axis.push_back( temp );
        // if one of the cross products calculated is not zero then the spine
        // is  not collinear.
        if( z_axis.back().length() >= Constants::f_epsilon )
          collinear = false;
      }
      else
        z_axis.push_back( Vec3f( 0, 0, 0) );

      // calculate z-axes for all spine points but the last and for the first 
      // spine point if it is undetermined.
      for( int  i = 1; i < spine_size - 1; ++i ) {
        Vec3f temp =( spine_vector[ spine_forward[i] ] - spine_vector[i] )
          .crossProduct( 
          spine_vector[ spine_backward[i] ] - spine_vector[i] );
        temp.normalizeSafe();
        z_axis.push_back( temp );

        // minimize angle between positive zAxes
        H3DFloat dot_product = z_axis[i].dotProduct( z_axis[i - 1] );
        if( H3DAbs( dot_product ) >= Constants::f_epsilon && dot_product < 0 )
          z_axis[i] = -z_axis[i];

        // first time a valid zAxes is calculated update the previous invalid
        // ones
        if( z_axis.back().length() >= Constants::f_epsilon ) {
          collinear = false;
          if( z_axis.front().length() < Constants::f_epsilon ) {
            z_axis.front() = z_axis.back();
            for( int j = 1; 
              j < i && z_axis[j].length() < Constants::f_epsilon;
              ++j )
              z_axis[j] = z_axis.front();
          }
        }
        else
          z_axis.back() = z_axis[i-1];
      }

      // calculate z-axes for the last spine point
      if( closed_spine ) {
        z_axis.push_back( z_axis.front() );

        // minimize angle between positive zAxes
        H3DFloat dot_product = z_axis.back().dotProduct(
          z_axis[ spine_size - 2 ] );
        if( H3DAbs( dot_product ) >= Constants::f_epsilon && dot_product < 0 )
          z_axis.back() = -z_axis.back();
      }
      else
        z_axis.push_back( z_axis.back() );

      if( collinear ) {
        // the y-axes is calculated like this since when the spine is collinear
        // it might be that the stepping forward to the first non-coincidential
        // spine point might end up being the same spine point as if stepping
        // backward to the first non-coincidential spine point
        // if the spine is closed.
        Vec3f temp = spine_vector[ spine_forward.front() ] - spine_vector[0];
        temp.normalizeSafe();
        y_axis.push_back( temp );

        // create dummy x-axes.
        if( H3DAbs( -1 + y_axis.front().x ) < Constants::f_epsilon )
          x_axis.push_back( Vec3f(0, -1, 0) );
        else if( H3DAbs( 1 + y_axis.front().x ) < Constants::f_epsilon )
          x_axis.push_back( Vec3f(0, 1, 0) );
        else
          x_axis.push_back( Vec3f(1, 0, 0) );

        z_axis.clear();
        z_axis.push_back( x_axis.front().crossProduct( y_axis.front() ) );

        // create final x-axes cause the axis need to be perpendicular to
        // eachother
        x_axis.front() = y_axis.front().crossProduct( z_axis.front() );
        x_axis.front().normalizeSafe();
        z_axis.front().normalizeSafe();

        Vec3f spine_dir = spine_vector.back() - spine_vector.front();
        for ( int i = spine_size - 1; 
          i > 0 && spine_dir.length() < Constants::f_epsilon;
          --i ) {
            spine_dir = ( spine_vector[i] - spine_vector.front() );
        }

        // maybe this is unneccesary. It might be enough to just flip the
        // y-axis.
        spine_dir.normalizeSafe();
        Matrix3f rotationMatrix( Rotation ( y_axis.front(), spine_dir ) );
        y_axis.front() = rotationMatrix * y_axis.front();
        x_axis.front() = rotationMatrix * x_axis.front();
        z_axis.front() = rotationMatrix * z_axis.front();

        for( int i = 1; i < spine_size; ++i ) {
          x_axis.push_back( x_axis.front() );
          y_axis.push_back( y_axis.front() );
          z_axis.push_back( z_axis.front() );
        }
      }
      else {
        // axes for first spine point
        Vec3f temp;
        if( closed_spine ) {      
          temp = spine_vector[ spine_forward.front() ]
          - spine_vector[ spine_backward.front() ];
          temp.normalizeSafe();
          y_axis.push_back( temp );
        }
        else {
          temp = spine_vector[ spine_forward.front() ] - spine_vector[0];
          temp.normalizeSafe();
          y_axis.push_back( temp );
        }

        temp = y_axis.front().crossProduct( z_axis.front() );
        temp.normalizeSafe();
        x_axis.push_back( temp );

        // axes for all but the last and first spine point
        for( int i = 1; i < spine_size - 1; ++i ) {
          temp = spine_vector[ spine_forward[i] ]
          - spine_vector[ spine_backward[i] ];
          temp.normalizeSafe();
          y_axis.push_back( temp );

          temp = y_axis[i].crossProduct( z_axis[i] );
          temp.normalizeSafe();
          x_axis.push_back( temp );
        }

        // axes for last spine point
        if( closed_spine )
          y_axis.push_back( y_axis.front() );
        else {
          temp = spine_vector.back() - spine_vector[ spine_backward.back() ];
          temp.normalizeSafe();
          y_axis.push_back( temp );
        }
        temp = y_axis.back().crossProduct( z_axis.back() );
        temp.normalizeSafe();
        x_axis.push_back( temp );
      }
    }

    for( int i = 0; i < nr_of_orn_values; ++i )
      spine_orientations.push_back( orn_vector[i] );

    // calculate vertices.
    for( int i = 0; i < spine_size; ++i ) {
      // Scp is the spine-aligned cross section plane, the orientation field should be
      // applied in the local coordinate system for that plane.
      Matrix3f scp_to_global( x_axis[i].x, y_axis[i].x, z_axis[i].x,
                             x_axis[i].y, y_axis[i].y, z_axis[i].y,
                             x_axis[i].z, y_axis[i].z, z_axis[i].z );
      x_axis[i] = scp_to_global * ( spine_orientations[ i % nr_of_orn_values ] * Vec3f( 1, 0, 0 ) );
      z_axis[i] = scp_to_global * ( spine_orientations[ i % nr_of_orn_values ] * Vec3f( 0, 0, 1 ) );
      for( int j = 0; j < nr_of_cross_section_points; ++j ) {
        Vec3f point0_x = scale_vector[ i % nr_of_scale_values ].x
          * cross_section[j].x * x_axis[i];
        Vec3f point0_z = scale_vector[ i % nr_of_scale_values ].y
          * cross_section[j].y * z_axis[i];
        Vec3f point0 = spine_vector[i] + 
        ( point0_x + point0_z );
        value.push_back( point0 );
      }
    }
  }
}

void Extrusion::render() {
  X3DGeometryNode::render();
  H3DInt32 spine_size = spine->size();

  if( spine_size > 1 ) {
    bool ccw_value = ccw->getValue();
    H3DInt32 nr_of_cross_section_points = crossSection->size();
    H3DFloat crease_angle = creaseAngle->getValue();

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
      if( ccw_value )
        glFrontFace( GL_CW );
      else  
        glFrontFace( GL_CCW );
    } else {
      if( ccw->getValue() )
        glFrontFace( GL_CCW );
      else 
        glFrontFace( GL_CW );
    }

    // get the Vertices
    const vector< Vec3f > vertexvec = vertexVector->getValue();
    Normal *auto_normal = autoNormal->getValue();
    const vector< Vec3f > &normals = auto_normal->vector->getValue();
    H3DInt32 if_caps_add = 0;
    bool begin_cap = beginCap->getValue();
    bool normal_per_vertex = crease_angle > Constants::f_epsilon;
    bool crease_angle_below_pi = crease_angle < Constants::pi;
    if( begin_cap ) {
      if( !normal_per_vertex )
        if_caps_add = 1;
      else if( crease_angle_below_pi )
        if_caps_add = nr_of_cross_section_points;
    }

    // if there is a cap in the beginning, draw it.
    if( begin_cap ) {
      glBegin( GL_POLYGON );

      if( !normal_per_vertex ) {
        const Vec3f &n = normals[0];
        glNormal3f( n.x, n.y, n.z );
      }

      for( int i = nr_of_cross_section_points - 1; i >= 0; --i )
      {
        renderTexCoordForActiveTexture( Vec3f( caps_tex_coord[i].x, 
                                               caps_tex_coord[i].y,
                                               0 ) );
        if( normal_per_vertex ) {
          const Vec3f &n = normals[i];
          glNormal3f( n.x, n.y, n.z );
        }
        const Vec3f &v = vertexvec[i];
        glVertex3f( v.x, v.y , v.z );
      }
      glEnd();
    }

    // draw the quads of the body.
    unsigned int quad_index = 0;
    Vec3f n, v;
    for( int i = 0; i < spine_size - 1; ++i ) {
      for( int j = 0; j < nr_of_cross_section_points - 1; ++j ) {
        H3DInt32 lower = i * nr_of_cross_section_points + j;
        H3DInt32 upper = ( i + 1 ) * nr_of_cross_section_points + j;

        glBegin( GL_TRIANGLES );
        if( !normal_per_vertex ) {
          n = normals[if_caps_add + quad_index];
          glNormal3f( n.x, n.y, n.z );
        }

        renderTexCoordForActiveTexture( Vec3f( u_tex_coord[j], 
                                               v_tex_coord[i],
                                               0 ) );
        if( normal_per_vertex ) {
          Vec3f n;
          if( crease_angle_below_pi )
            n = normals[ if_caps_add + quad_index * 6 ];
          else
            n = normals[ lower ];
          glNormal3f( n.x, n.y, n.z );
        }
        v = vertexvec[ lower ];
        glVertex3f( v.x, v.y, v.z );

        renderTexCoordForActiveTexture( Vec3f( u_tex_coord[ j + 1 ], 
                                               v_tex_coord[i],
                                               0 ) );
        if( normal_per_vertex ) {
          if( crease_angle < Constants::pi )
            n = normals[ if_caps_add + quad_index * 6 + 1];
          else
            n = normals[ lower + 1];
          glNormal3f( n.x, n.y, n.z );
        }
        v = vertexvec[ lower + 1 ];
        glVertex3f( v.x, v.y, v.z );

        renderTexCoordForActiveTexture( Vec3f( u_tex_coord[ j + 1 ], 
                                               v_tex_coord[ i + 1 ],
                                               0 ) );
        if( normal_per_vertex ) {
          if( crease_angle < Constants::pi )
            n = normals[ if_caps_add + quad_index * 6 + 2];
          else
            n = normals[ upper + 1];
          glNormal3f( n.x, n.y, n.z );
        }
        v = vertexvec[ upper + 1 ];
        glVertex3f( v.x, v.y, v.z );


        if( !normal_per_vertex ) {
          n = normals[if_caps_add + quad_index + 1];
          glNormal3f( n.x, n.y, n.z );
        }

        renderTexCoordForActiveTexture( Vec3f( u_tex_coord[j], 
                                               v_tex_coord[i],
                                               0 ) );
        if( normal_per_vertex ) {
          Vec3f n;
          if( crease_angle_below_pi )
            n = normals[ if_caps_add + quad_index * 6 + 3 ];
          else
            n = normals[ lower ];
          glNormal3f( n.x, n.y, n.z );
        }
        v = vertexvec[ lower ];
        glVertex3f( v.x, v.y, v.z );

        renderTexCoordForActiveTexture( Vec3f( u_tex_coord[ j + 1 ], 
                                               v_tex_coord[ i + 1 ],
                                               0 ) );
        if( normal_per_vertex ) {
          if( crease_angle < Constants::pi )
            n = normals[ if_caps_add + quad_index * 6 + 4];
          else
            n = normals[ upper + 1];
          glNormal3f( n.x, n.y, n.z );
        }
        v = vertexvec[ upper + 1 ];
        glVertex3f( v.x, v.y, v.z );

        renderTexCoordForActiveTexture( Vec3f( u_tex_coord[j], 
                                               v_tex_coord[ i + 1 ],
                                               0 ) );
        if( normal_per_vertex ) {
          if( crease_angle < Constants::pi )
            n = normals[ if_caps_add + quad_index * 6 + 5];
          else
            n = normals[ upper];
          glNormal3f( n.x, n.y, n.z );
        }
        v = vertexvec[ upper ];
        glVertex3f( v.x, v.y, v.z );

        glEnd();
        ++quad_index;
        if( !normal_per_vertex )
          ++quad_index;
      }
    }

    // if there is a cap in the end, draw it.
    if( endCap -> getValue() ) {

      glBegin( GL_POLYGON );

      Vec3f n, v;
      if( !normal_per_vertex ) {
        n = normals.back();
        glNormal3f( n.x, n.y, n.z );
      }

      for(int i = 0; i < nr_of_cross_section_points; ++i )
      {
        renderTexCoordForActiveTexture( Vec3f( caps_tex_coord[i].x, 
                                               caps_tex_coord[i].y,
                                               0 ) );
        if( normal_per_vertex ) {
          n = normals[ normals.size() - nr_of_cross_section_points + i ];
          glNormal3f( n.x, n.y, n.z );
        }

        v = vertexvec[ vertexvec.size() - nr_of_cross_section_points + i ];
        glVertex3f( v.x, v.y , v.z );
      }
      glEnd();
    }

    // Restore the front face to its previuos value.
    glFrontFace( front_face );
  }
}

void Extrusion::AutoNormal::update() {
  const vector< Vec3f > &vertex_vector =
    static_cast< MFVec3f * >( routes_in[0] )->getValue();
  H3DFloat crease_angle = static_cast< SFFloat * >( routes_in[1] )->getValue();
  Extrusion *extrusion = static_cast< Extrusion * >(getOwner());
  const vector< Vec3f > &spine_vector = extrusion->spine->getValue();
  bool closed_spine = false;
  bool collinear = true;        // if the spine is collinear
  bool coincident = false;      // if every point on the spine is the same

  int spine_size = (int) spine_vector.size();
  vector< int > spine_forward;
  vector< int > spine_backward;
  vector< Vec3f > y_axis;

  // check if the spine is closed
  if( ExtrusionInternals::coinc( spine_vector.front(),
                                 spine_vector.back() ) )
    closed_spine = true;

  // build vectors containing indicies to the first non-coincident spine. 
  // Backward and forward.
  // these vectors are used to calculate axis for the SCP around each spine.
  for( int i = 0; i < spine_size; ++i ) {
    int j = i;

    do {
      ++j;

      if( j >= spine_size )
          j -= spine_size;
    } while( j!= i && ExtrusionInternals::coinc( spine_vector[i],
                                                 spine_vector[j] ) );

    if( !closed_spine && j < i )
      spine_forward.push_back( spine_size - 1 );
    else
      spine_forward.push_back(j);
    if( i == 0 && j == 0 )
      coincident = true;

    j = i;

    do {
      --j;

      if( j < 0 )
        j += spine_size;
    } while( j!= i && ExtrusionInternals::coinc( spine_vector[i],
                                                 spine_vector[j] ) );

    if( !closed_spine && j > i )
      spine_backward.push_back( 0 );
    else
      spine_backward.push_back(j);
  }

  // if the entire spine is coincident choose and arbitrary direction for the
  // SCP. If another direction is desired use the orientation values.
  // another node might be a better solution for this kind of shape.
  if( coincident ) {
    y_axis.push_back( Vec3f( 0, 1, 0 ) );
    y_axis.push_back( y_axis.front() );
  }
  else {

    // check if the spine is collinear
    if( closed_spine ) {
      Vec3f temp = ( spine_vector[ spine_forward.front() ] - spine_vector[0] )
        .crossProduct( 
        spine_vector[ spine_backward.front() ] - spine_vector[0] );
      // if one of the cross products calculated is not zero then the spine is
      // not collinear.
      if( temp.length() >= Constants::f_epsilon )
        collinear = false;
    }

    for( int  i = 1; i < spine_size - 1; ++i ) {
      Vec3f temp =( spine_vector[ spine_forward[i] ] - spine_vector[i] )
        .crossProduct(
        spine_vector[ spine_backward[i] ] - spine_vector[i] );

      if( temp.length() >= Constants::f_epsilon ) {
        collinear = false;
        break;
      }
    }

    if( collinear ) {
      // the y-axes is calculated like this since when the spine is collinear
      // it might be that the stepping forward to the first non-coincidential 
      // spine point might end up being the same spine point as if stepping 
      // backward to the first non-coincidential spine point
      // if the spine is closed.
      Vec3f temp = spine_vector[ spine_forward.front() ] - spine_vector[0];
      temp.normalizeSafe();
      y_axis.push_back( temp );

      Vec3f spine_dir = spine_vector.back() - spine_vector.front();
      for ( int i = spine_size - 1;
        i > 0 && spine_dir.length() < Constants::f_epsilon;
        --i ) {
          spine_dir = ( spine_vector[i] - spine_vector.front() );
      }

      // maybe this is unneccesary. It might be enough to just flip the y-axis.
      spine_dir.normalizeSafe();
      Matrix3f rotationMatrix( Rotation ( y_axis.front(), spine_dir ) );
      y_axis.front() = rotationMatrix * y_axis.front();
      y_axis.push_back( y_axis.front() );
    }
    else {

      // axes for first spine point
      Vec3f temp;
      if( closed_spine ) {
        temp = spine_vector[ spine_forward.front() ] -
          spine_vector[ spine_backward.front() ];
        temp.normalizeSafe();
        y_axis.push_back( temp );
      }
      else {
        temp = spine_vector[ spine_forward.front() ] - spine_vector[0];
        temp.normalizeSafe();
        y_axis.push_back( temp );
      }

      // axes for last spine point
      if( closed_spine )
        y_axis.push_back( y_axis.front() );
      else {
        temp = spine_vector.back() - spine_vector[ spine_backward.back() ];
        temp.normalizeSafe();
        y_axis.push_back( temp );
      }
    }
  }

  const vector< Vec2f > &cross_section = extrusion->crossSection->getValue();
  if( crease_angle > Constants::f_epsilon ) {
    if( crease_angle < Constants::pi )
      value = generateNormalsPerVertex( vertex_vector, 
                                        extrusion->crossSection->getValue(),
                                        y_axis,
                                        extrusion->ccw->getValue(),
                                        (unsigned int) cross_section.size(),
                                        spine_size,
                                        closed_spine,
                                        crease_angle,
                           static_cast< SFBool * >(routes_in[2])->getValue(),
                           static_cast< SFBool * >(routes_in[3])->getValue() );
    else 
      value = generateNormalsPerVertex( vertex_vector, 
                                        extrusion->crossSection->getValue(),
                                        y_axis,
                                        extrusion->ccw->getValue(),
                                        (unsigned int) cross_section.size(),
                                        spine_size,
                                        closed_spine,
                           static_cast< SFBool * >(routes_in[2])->getValue(),
                           static_cast< SFBool * >(routes_in[3])->getValue() );
  } else {
    value = generateNormalsPerFace( vertex_vector,
                                    extrusion->crossSection->getValue(),
                                    y_axis,
                                    extrusion->ccw->getValue(),
                                    (unsigned int)cross_section.size(),
                                    spine_size,
                                    ExtrusionInternals::coinc(
                                      cross_section.front(),
                                      cross_section.back() ),
                           static_cast< SFBool * >(routes_in[2])->getValue(),
                           static_cast< SFBool * >(routes_in[3])->getValue() );
  }

  // Calculation of the u and v coordinates for the extrusion.
  // u is along cross section, v is along the spine.
  // For the caps u and v corresponds to x and z of the SCP
  // and the biggest difference in x or z maps to 0-1. Uniform scaling.
  extrusion->u_tex_coord.clear();
  extrusion->v_tex_coord.clear();
  extrusion->caps_tex_coord.clear();
  H3DFloat cross_section_length = 0;
  H3DFloat cross_section_xmin = cross_section[0].x;
  H3DFloat cross_section_xmax = cross_section[0].x;
  H3DFloat cross_section_zmin = cross_section[0].y;
  H3DFloat cross_section_zmax = cross_section[0].y;
  // First we, for each cross section point, add the distance between
  // the cross section point corresponding to the index and the first cross
  // section point
  extrusion->u_tex_coord.push_back(cross_section_length);
  for( unsigned int i = 1; i < cross_section.size(); ++i ) {
    cross_section_length += ( cross_section[i] - cross_section[i-1] ).length();
    extrusion->u_tex_coord.push_back( cross_section_length );
    if( cross_section[i].x < cross_section_xmin )
      cross_section_xmin = cross_section[i].x;
    if( cross_section[i].x > cross_section_xmax )
      cross_section_xmax = cross_section[i].x;
    if( cross_section[i].y < cross_section_zmin )
      cross_section_zmin = cross_section[i].y;
    if( cross_section[i].y > cross_section_zmax )
      cross_section_zmax = cross_section[i].y;
  }

  // Use the cross_section length to calculate the texture coordinate in
  // the u direction. Remember that u_tex_coord[i] currently contains the
  // distance from first point to the point at index i.
  for( unsigned int i = 0; i < cross_section.size(); ++i ) {
    extrusion->u_tex_coord[i] = extrusion->u_tex_coord[i] /
                                cross_section_length;
  }

  // Choose the biggest range since this should be the deciding factor for
  // the caps.
  H3DFloat cross_section_range = cross_section_xmax - cross_section_xmin;
  if( cross_section_range < cross_section_zmax - cross_section_zmin )
    cross_section_range = cross_section_zmax - cross_section_zmin;

  // Calculate u, v coordinates for the cap
  for( unsigned int i = 0; i < cross_section.size(); ++i )
    extrusion->caps_tex_coord.push_back( Vec3f(
      ( cross_section[i].x - cross_section_xmin ) / cross_section_range,
      ( cross_section[i].y - cross_section_zmin ) / cross_section_range,
      0 ) );

  // Calculate v-coordinates. First add the length for each index to
  // the first point, then use the calculated total length to change
  // the values to the [0,1] range.
  H3DFloat spine_length = 0;
  extrusion->v_tex_coord.push_back(spine_length);
  for( int i = 1; i < spine_size; ++i ) {
    spine_length += ( spine_vector[ i ] - spine_vector[i-1] ).length();
    extrusion->v_tex_coord.push_back( spine_length );
  }

  for( int i = 0; i < spine_size; ++i ) {
    extrusion->v_tex_coord[i] = extrusion->v_tex_coord[i] / spine_length;
  }
}

Normal * Extrusion::AutoNormal::generateNormalsPerFace(
                      const vector < Vec3f > &vertex_vector,
                      const vector < Vec2f > &cross_section,
                      vector < Vec3f > &y_axis,
                      bool ccw_value,
                      H3DInt32 nr_of_cross_section_points,
                      H3DInt32 spine_size,
                      bool closed_cross_section,
                      bool begin_cap,
                      bool end_cap ) {
  
  vector < Vec3f > normal_vector;
  H3DInt32 closed_cross_section_int = 0;
  H3DFloat ccw_or_cw = 0;

  if( closed_cross_section )
    closed_cross_section_int = 1;

  // to check which way the points are ordered looking in the negative
  // direction from the positive y-axis.
  for( int i = 0;
       i < nr_of_cross_section_points - closed_cross_section_int;
       ++i )
    ccw_or_cw += cross_section[i].y * 
               cross_section[ ( i + 1 ) % nr_of_cross_section_points ].x -
               cross_section[ ( i + 1 ) % nr_of_cross_section_points ].y *
               cross_section[i].x;

  // calculate normal for the beginCap if it exists.
  if( begin_cap ) {
    if( ( ccw_value && ccw_or_cw >= 0 ) || ( !ccw_value && ccw_or_cw < 0 ) )
      normal_vector.push_back( 
        Vec3f( -y_axis.front().x, -y_axis.front().y, -y_axis.front().z ) );
    else
      normal_vector.push_back( 
        Vec3f( y_axis.front().x, y_axis.front().y, y_axis.front().z ) );
  }

  // calculate one normal for each face of the main body
  for( int i = 0; i < spine_size - 1; ++i ) {
    for( int j = 0; j < nr_of_cross_section_points - 1; ++j ) {
      vector < H3DInt32 > indices;
      indices.push_back( i * nr_of_cross_section_points + j );
      indices.push_back( i * nr_of_cross_section_points + j + 1 );
      indices.push_back( ( i + 1 ) * nr_of_cross_section_points + j + 1 );
      indices.push_back( ( i + 1 ) * nr_of_cross_section_points + j );
      bool zero_normal = true;
      Vec3f first_normal = ExtrusionInternals::calculateNormal( vertex_vector,
                                                                indices[ 1 ],
                                                                indices[ 0 ],
                                                                indices[ 2 ] );
      Vec3f second_normal = ExtrusionInternals::calculateNormal( vertex_vector,
                                                                indices[ 2 ],
                                                                indices[ 0 ],
                                                                indices[ 3 ] );
      if( first_normal.length() > Constants::f_epsilon ) {
        zero_normal = false;
        if( second_normal.length() <= Constants::f_epsilon )
          second_normal = first_normal;
      } else if( second_normal.length() > Constants::f_epsilon ) {
        zero_normal = false;
        first_normal = second_normal;
      } else {
        Vec3f the_normal = ExtrusionInternals::calculateNormal( vertex_vector,
                                                                indices[ 1 ],
                                                                indices[ 0 ],
                                                                indices[ 3 ] );
        if( the_normal.length() > Constants::f_epsilon ) {
          zero_normal = false;
          first_normal = second_normal = the_normal;
        } else {
          the_normal = ExtrusionInternals::calculateNormal( vertex_vector,
                                                            indices[ 2 ],
                                                            indices[ 1 ],
                                                            indices[ 3 ] );
          if( the_normal.length() > Constants::f_epsilon ) {
            zero_normal = false;
            first_normal = second_normal = the_normal;
          }
        }
      }
      if( !zero_normal ) {
        first_normal.normalize();
        second_normal.normalize();
      }
      
      if( ccw_value ) {
        normal_vector.push_back( first_normal );
        normal_vector.push_back( second_normal );
      } else {
        normal_vector.push_back( -first_normal );
        normal_vector.push_back( second_normal );
      }
    }
  }
  
  // calculate normal for the endCap if it exists
  if( end_cap ) {
    if( ( ccw_value && ccw_or_cw >= 0 ) || ( !ccw_value && ccw_or_cw < 0 ) )
      normal_vector.push_back(
        Vec3f( y_axis.back().x, y_axis.back().y, y_axis.back().z ) );
    else
      normal_vector.push_back(
        Vec3f( -y_axis.back().x, -y_axis.back().y, -y_axis.back().z ) );
  }

  Normal *normal = new Normal;
  normal->vector->setValue( normal_vector );
  return normal;
}

Normal * Extrusion::AutoNormal::generateNormalsPerVertex(
                      const vector < Vec3f > &vertex_vector,
                      const vector < Vec2f > &cross_section,
                      vector < Vec3f > &y_axis,
                      bool ccw_value,
                      H3DInt32 nr_of_cross_section_points,
                      H3DInt32 spine_size,
                      bool closed_spine,
                      bool begin_cap,
                      bool end_cap ) {
  
  bool closed_cross_section = false;
  if( ExtrusionInternals::coinc( cross_section.front(),
                                 cross_section.back() ) )
    closed_cross_section = true;
                        
  AutoRef< Normal > normals_per_face_node;
  normals_per_face_node.reset( generateNormalsPerFace(
                                      vertex_vector,
                                       cross_section,
                                       y_axis,
                                      ccw_value,
                                      nr_of_cross_section_points,
                                      spine_size,
                                      closed_cross_section,
                                      begin_cap,
                                      end_cap ) );
  const vector< Vec3f > &normals_per_face =
    normals_per_face_node->vector->getValue();
  vector< Vec3f > normal_vector;
  int if_caps_add = 0;
  if( begin_cap )
    if_caps_add = 1;

  H3DInt32 nr_of_cross_section_points_minone = nr_of_cross_section_points - 1;
  // no separate normals are needed for the caps.
  for( int i = 0; i < spine_size; ++i ) {
    for( int j = 0; j < nr_of_cross_section_points; ++j ) {
      vector< H3DInt32 > indices;
      normal_vector.push_back( Vec3f( 0, 0, 0 ) );
      bool begin_cap_summed = false;
      bool end_cap_summed = false;

      indices = ExtrusionInternals::findSurroundingNormalIndex( i,
                                         j,
                                         closed_spine,
                                         spine_size, 
                                         closed_cross_section,
                                         nr_of_cross_section_points_minone,
                                         if_caps_add,
                                         end_cap );

      for( vector< H3DInt32 >::iterator k = indices.begin(); k != indices.end(); ++k ) {
        normal_vector.back() += normals_per_face[*k];
      }
      normal_vector.back().normalizeSafe();
    }
  }
  Normal *normal = new Normal;
  normal->vector->setValue( normal_vector );
  return normal;
}

Normal * Extrusion::AutoNormal::generateNormalsPerVertex(
                      const vector < Vec3f > &vertex_vector,
                      const vector < Vec2f > &cross_section,
                      vector < Vec3f > &y_axis,
                      bool ccw_value,
                      H3DInt32 nr_of_cross_section_points,
                      H3DInt32 spine_size,
                      bool closed_spine,
                      H3DFloat crease_angle,
                      bool begin_cap,
                      bool end_cap ) {
  
  bool closed_cross_section = false;
  if( ExtrusionInternals::coinc( cross_section.front(),
                                 cross_section.back() ) )
    closed_cross_section = true;

  AutoRef< Normal > normals_per_face_node;
  normals_per_face_node.reset( generateNormalsPerFace( 
                                     vertex_vector,
                                     cross_section,
                                     y_axis,
                                     ccw_value,
                                     nr_of_cross_section_points,
                                     spine_size,
                                     closed_cross_section,
                                     begin_cap,
                                     end_cap ) );
  const vector< Vec3f > &normals_per_face =
    normals_per_face_node->vector->getValue();

  H3DFloat cos_crease_angle = H3DCos( crease_angle );
  vector< Vec3f > normal_vector;
  int if_caps_add = 0;

  // normals for begincap
  if( begin_cap ) {
    if_caps_add = 1;

    Vec3f face_normal = normals_per_face.front();
    normal_vector.push_back( face_normal );
    Vec3f quad_normal = normals_per_face[1];
    if( quad_normal * face_normal > cos_crease_angle )
      normal_vector.back() += quad_normal;
    quad_normal = normals_per_face[2];
    if( quad_normal * face_normal > cos_crease_angle )
      normal_vector.back() += quad_normal;
    if( closed_cross_section ) {
      quad_normal = 
        normals_per_face[ if_caps_add + 2* (nr_of_cross_section_points - 2) ];
      if( quad_normal * face_normal > cos_crease_angle )
        normal_vector.back() += quad_normal;
    }
    normal_vector.back().normalizeSafe();

    for( int i = 1; i < nr_of_cross_section_points - 1; ++i )
    {
      normal_vector.push_back( face_normal );

      quad_normal = normals_per_face[ if_caps_add + 2 * i ];
      if( quad_normal * face_normal > cos_crease_angle )
        normal_vector.back() += quad_normal;

      quad_normal = normals_per_face[ if_caps_add + 2 * i + 1 ];
      if( quad_normal * face_normal > cos_crease_angle )
        normal_vector.back() += quad_normal;

      quad_normal = normals_per_face[ if_caps_add + 2 * (i-1) ];
      if( quad_normal * face_normal > cos_crease_angle )
        normal_vector.back() += quad_normal;

      normal_vector.back().normalizeSafe();
    }

    normal_vector.push_back( face_normal );
    quad_normal =
      normals_per_face[ if_caps_add + 2 * (nr_of_cross_section_points - 2) ];
    if( quad_normal * face_normal > cos_crease_angle )
      normal_vector.back() += quad_normal;
    if( closed_cross_section ) {
      quad_normal = normals_per_face[1];
      if( quad_normal * face_normal > cos_crease_angle )
        normal_vector.back() += quad_normal;
      quad_normal = normals_per_face[2];
      if( quad_normal * face_normal > cos_crease_angle )
        normal_vector.back() += quad_normal;
    }
    normal_vector.back().normalizeSafe();
  }

  H3DInt32 nr_of_cross_section_points_minone = nr_of_cross_section_points - 1;
  // normals for every face except the beginCap and endCap
  for( int i = 0; i < spine_size - 1; ++i ) {
    for( int j = 0; j < nr_of_cross_section_points - 1; ++j ) {
      vector< H3DInt32 > indices;
      H3DInt32 lower_index = if_caps_add +
                             2 * ( i * (nr_of_cross_section_points - 1) + j );
      Vec3f face0_normal = normals_per_face[ lower_index ];
      Vec3f face1_normal = normals_per_face[ lower_index + 1 ];
      bool begin_cap_summed = false;
      bool end_cap_summed = false;

      //  vertex 0
      normal_vector.push_back( Vec3f( 0, 0, 0 ) );
      Vec3f face1_vertex0( 0, 0, 0 );
      H3DInt32 i_index = i;
      H3DInt32 j_index = j;
      indices = ExtrusionInternals::findSurroundingNormalIndex( i_index,
                                           j_index,
                                           closed_spine,
                                           spine_size,
                                           closed_cross_section,
                                           nr_of_cross_section_points_minone,
                                           begin_cap, end_cap );

      for( vector< H3DInt32 >::iterator k = indices.begin(); k != indices.end(); ++k ) {
        const Vec3f &quad_normal = normals_per_face[*k];
        if( quad_normal * face0_normal > cos_crease_angle )
          normal_vector.back() += quad_normal;
        if( quad_normal * face1_normal > cos_crease_angle )
          face1_vertex0 += quad_normal;
      }
      normal_vector.back().normalizeSafe();
      face1_vertex0.normalizeSafe();


      //  vertex 1
      begin_cap_summed = false;
      end_cap_summed = false;
      normal_vector.push_back( Vec3f( 0, 0, 0 ) );
      i_index = i;
      j_index = j + 1;
      indices = ExtrusionInternals::findSurroundingNormalIndex( i_index,
                                           j_index,
                                           closed_spine,
                                           spine_size,
                                           closed_cross_section,
                                           nr_of_cross_section_points_minone,
                                           begin_cap, end_cap );

      for( vector< H3DInt32 >::iterator k = indices.begin(); k != indices.end(); ++k ) {
        const Vec3f &quad_normal = normals_per_face[*k];
        if( quad_normal * face0_normal > cos_crease_angle )
          normal_vector.back() += quad_normal;
      }
      normal_vector.back().normalizeSafe();

      //  vertex 2
      begin_cap_summed = false;
      end_cap_summed = false;
      normal_vector.push_back( Vec3f( 0, 0, 0 ) );
      Vec3f face1_vertex2( 0, 0, 0 );
      i_index = i + 1;
      j_index = j + 1;
      indices = ExtrusionInternals::findSurroundingNormalIndex( i_index,
                                           j_index,
                                           closed_spine,
                                           spine_size,
                                           closed_cross_section,
                                           nr_of_cross_section_points_minone,
                                           begin_cap, end_cap );

      for( vector< H3DInt32 >::iterator k = indices.begin(); k != indices.end(); ++k ) {
        const Vec3f &quad_normal = normals_per_face[*k];
        if( quad_normal * face0_normal > cos_crease_angle )
          normal_vector.back() += quad_normal;
        if( quad_normal * face1_normal > cos_crease_angle )
          face1_vertex2 += quad_normal;
      }
      normal_vector.back().normalizeSafe();
      face1_vertex2.normalizeSafe();

      //  vertex 3
      begin_cap_summed = false;
      end_cap_summed = false;
      normal_vector.push_back( face1_vertex0 );
      normal_vector.push_back( face1_vertex2 );
      normal_vector.push_back( Vec3f( 0, 0, 0 ) );
      i_index = i + 1;
      j_index = j;
      indices = ExtrusionInternals::findSurroundingNormalIndex( i_index,
                                           j_index,
                                           closed_spine,
                                           spine_size,
                                           closed_cross_section,
                                           nr_of_cross_section_points_minone,
                                           begin_cap, end_cap );

      for( vector< H3DInt32 >::iterator k = indices.begin(); k != indices.end(); ++k ) {
        const Vec3f &quad_normal = normals_per_face[*k];
        if( quad_normal * face1_normal > cos_crease_angle )
          normal_vector.back() += quad_normal;
      }

      normal_vector.back().normalizeSafe();
    }
  }

    // normals calculated for the endCap if needed.
  if( end_cap ) {
    H3DInt32 theLastVertexNormal = (H3DInt32)normals_per_face.size() - 1;
    Vec3f face_normal = normals_per_face.back();
    normal_vector.push_back( face_normal );
    Vec3f quad_normal = 
      normals_per_face[ theLastVertexNormal - 2 * ( nr_of_cross_section_points - 1 ) + 1 ];
    if( quad_normal * face_normal > cos_crease_angle )
      normal_vector.back() += quad_normal;
    if( closed_cross_section ) {
      quad_normal = 
        normals_per_face[ theLastVertexNormal - 1 ];
      if( quad_normal * face_normal > cos_crease_angle )
        normal_vector.back() += quad_normal;
      quad_normal = 
        normals_per_face[ theLastVertexNormal - 2 ];
      if( quad_normal * face_normal > cos_crease_angle )
        normal_vector.back() += quad_normal;
    }
    normal_vector.back().normalizeSafe();

    for( int i = 1; i < nr_of_cross_section_points - 1; ++i )  {
      normal_vector.push_back( face_normal );

      quad_normal = 
        normals_per_face[ theLastVertexNormal -
                          2 * ( nr_of_cross_section_points - i - 1 ) + 1 ];
      if( quad_normal * face_normal > cos_crease_angle )
        normal_vector.back() += quad_normal;

      int low_index = theLastVertexNormal -
                          2 * ( nr_of_cross_section_points - i );
      quad_normal = normals_per_face[ low_index ];
      if( quad_normal * face_normal > cos_crease_angle )
        normal_vector.back() += quad_normal;

      quad_normal = normals_per_face[ low_index + 1 ];
      if( quad_normal * face_normal > cos_crease_angle )
        normal_vector.back() += quad_normal;

      normal_vector.back().normalizeSafe();
    }

    normal_vector.push_back( face_normal );
    quad_normal = normals_per_face[ theLastVertexNormal - 2 ];
    if( quad_normal * face_normal > cos_crease_angle )
      normal_vector.back() += quad_normal;
    quad_normal = normals_per_face[ theLastVertexNormal - 1 ];
    if( quad_normal * face_normal > cos_crease_angle )
      normal_vector.back() += quad_normal;
    if( closed_cross_section ) {
      quad_normal = 
        normals_per_face[ theLastVertexNormal - 2 * ( nr_of_cross_section_points - 1 ) + 1 ];
      if( quad_normal * face_normal > cos_crease_angle )
        normal_vector.back() += quad_normal;
    }
    normal_vector.back().normalizeSafe();
  }

  Normal * normal = new Normal;
  normal->vector->setValue( normal_vector );
  return normal;
}

void Extrusion::SFBound::update() {
  const vector< Vec3f > &vertex_vectors = 
    static_cast< MFVec3f * >( routes_in[0] )->getValue();
  
  BoxBound *bb = new BoxBound;
  bb->fitAroundPoints( vertex_vectors.begin(), vertex_vectors.end() );
  value = bb;
}

void Extrusion::traverseSG( TraverseInfo &ti ) {
  X3DGeometryNode::traverseSG( ti );
  // enable backface culling if solid is true
  if( solid->getValue() ) useBackFaceCulling( true );
  else useBackFaceCulling( false );
}
