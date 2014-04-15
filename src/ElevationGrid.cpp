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
/// \file ElevationGrid.cpp
/// \brief CPP file for ElevationGrid, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/ElevationGrid.h>
#include <H3D/MultiTexture.h>
#include <H3D/MultiTextureCoordinate.h>
#include <H3D/Normal.h>
#include <H3D/GlobalSettings.h>
#include <H3D/GraphicsOptions.h>

using namespace H3D;

H3DNodeDatabase ElevationGrid::database( 
        "ElevationGrid", 
        &(newInstance< ElevationGrid > ),
        typeid( ElevationGrid ),
        &X3DGeometryNode::database );

namespace ElevationGridInternals {
  FIELDDB_ELEMENT( ElevationGrid, color, INPUT_OUTPUT );
  FIELDDB_ELEMENT( ElevationGrid, normal, INPUT_OUTPUT );
  FIELDDB_ELEMENT( ElevationGrid, texCoord, INPUT_OUTPUT );
  FIELDDB_ELEMENT( ElevationGrid, ccw, INPUT_OUTPUT );
  FIELDDB_ELEMENT( ElevationGrid, colorPerVertex, INPUT_OUTPUT );
  FIELDDB_ELEMENT( ElevationGrid, normalPerVertex, INPUT_OUTPUT );
  FIELDDB_ELEMENT( ElevationGrid, solid, INPUT_OUTPUT );
  FIELDDB_ELEMENT( ElevationGrid, attrib, INPUT_OUTPUT );
  FIELDDB_ELEMENT( ElevationGrid, creaseAngle, INPUT_OUTPUT );
  FIELDDB_ELEMENT( ElevationGrid, xDimension, INPUT_OUTPUT );
  FIELDDB_ELEMENT( ElevationGrid, zDimension, INPUT_OUTPUT );
  FIELDDB_ELEMENT( ElevationGrid, xSpacing, INPUT_OUTPUT );
  FIELDDB_ELEMENT( ElevationGrid, zSpacing, INPUT_OUTPUT );
  FIELDDB_ELEMENT( ElevationGrid, height, INPUT_OUTPUT );
  FIELDDB_ELEMENT( ElevationGrid, fogCoord, INPUT_OUTPUT );
}

ElevationGrid::ElevationGrid( 
                              Inst< SFNode           > _metadata,
                              Inst< SFBound          > _bound,
                              Inst< DisplayList      > _displayList,             
                              Inst< SFColorNode             > _color,
                              Inst< SFNormalNode            > _normal,
                              Inst< SFTextureCoordinateNode > _texCoord,
                              Inst< SFBool                  > _ccw,
                              Inst< SFBool                  > _colorPerVertex,
                              Inst< SFBool                  > _normalPerVertex,
                              Inst< SFBool                  > _solid,
                              Inst< MFVertexAttributeNode   > _attrib,
                              Inst< AutoNormal       > _autoNormal,
                              Inst< SFFloat          > _creaseAngle,
                              Inst< SFInt32          > _xDimension,
                              Inst< SFInt32          > _zDimension,
                              Inst< SFFloat          > _xSpacing,
                              Inst< SFFloat          > _zSpacing,
                              Inst< MFFloat          > _height,
                              Inst< SFFogCoordinate  > _fogCoord  ) :
  X3DGeometryNode( _metadata, _bound, _displayList ),
  color          ( _color            ),
  normal         ( _normal           ),
  texCoord       ( _texCoord         ),
  ccw            ( _ccw              ),
  colorPerVertex ( _colorPerVertex   ),
  normalPerVertex( _normalPerVertex  ),
  solid          ( _solid            ),
  attrib         ( _attrib           ),
  creaseAngle    ( _creaseAngle      ),
  xDimension     ( _xDimension       ),
  zDimension     ( _zDimension       ),
  xSpacing       ( _xSpacing         ),
  zSpacing       ( _zSpacing         ),
  height         ( _height           ),
  autoNormal     ( _autoNormal       ),
  fogCoord       ( _fogCoord    ),
  vboFieldsUpToDate( new Field ),
  vbo_id( NULL ) {

  type_name = "ElevationGrid";

  database.initFields( this );

  ccw->setValue( true );
  colorPerVertex->setValue( true );
  normalPerVertex->setValue( true );
  solid->setValue( true );
  creaseAngle->setValue( 0 );
  xDimension->setValue( 0 );
  zDimension->setValue( 0 );
  xSpacing->setValue( 1.0f );
  zSpacing->setValue( 1.0f );

  color->route( displayList );
  normal->route( displayList );
  texCoord->route( displayList );
  ccw->route( displayList );
  colorPerVertex->route( displayList );
  normalPerVertex->route( displayList );
  solid->route( displayList );
  attrib->setName( "attrib" );
  attrib->route( displayList );
  xDimension->route( displayList );
  zDimension->route( displayList );
  xSpacing->route( displayList );
  zSpacing->route( displayList );
  height->route( displayList );
  fogCoord->route( displayList );
  creaseAngle->route( displayList );

  xDimension->route( bound );
  zDimension->route( bound );
  xSpacing->route( bound );
  zSpacing->route( bound );
  height->route( bound );

  autoNormal->setName( "autoNormal" );
  normalPerVertex->route( autoNormal );
  xDimension->route( autoNormal );
  zDimension->route( autoNormal );
  xSpacing->route( autoNormal );
  zSpacing->route( autoNormal );
  height->route( autoNormal );
  ccw->route( autoNormal );
  creaseAngle->route( autoNormal );

  vboFieldsUpToDate->setName( "vboFieldsUpToDate" );
  color->route( vboFieldsUpToDate );
  normal->route( vboFieldsUpToDate );
  texCoord->route( vboFieldsUpToDate );
  colorPerVertex->route( vboFieldsUpToDate );
  normalPerVertex->route( vboFieldsUpToDate );
  xDimension->route( vboFieldsUpToDate );
  zDimension->route( vboFieldsUpToDate );
  xSpacing->route( vboFieldsUpToDate );
  zSpacing->route( vboFieldsUpToDate );
  height->route( vboFieldsUpToDate );
  creaseAngle->route( vboFieldsUpToDate );

}

ElevationGrid::~ElevationGrid() {
  // Delete buffer if it was allocated.
  if( vbo_id ) {
    glDeleteBuffersARB( 2, vbo_id );
    delete [] vbo_id;
    vbo_id = NULL;
  }
}

void ElevationGrid::AutoNormal::update() {
  bool normals_per_vertex = 
    static_cast< SFBool * >( routes_in[0] )->getValue();
  H3DInt32 x_dim = static_cast< SFInt32 * >( routes_in[1] )->getValue();
  H3DInt32 z_dim = static_cast< SFInt32 * >( routes_in[2] )->getValue();
  H3DFloat x_spacing = static_cast< SFFloat * >( routes_in[3] )->getValue();
  H3DFloat z_spacing = static_cast< SFFloat * >( routes_in[4] )->getValue();
  const vector< H3DFloat > &height = 
    static_cast< MFFloat * >( routes_in[5] )->getValue();
  bool ccw = static_cast< SFBool * >( routes_in[6] )->getValue();
  H3DFloat crease_angle = static_cast< SFFloat * >( routes_in[7] )->getValue();
  
  if( normals_per_vertex && crease_angle > 0 ) {
    if( crease_angle < Constants::pi )
      value = generateNormalsPerVertex( x_dim, z_dim, x_spacing, z_spacing, 
                                        height, ccw, crease_angle );
    else 
      value = generateNormalsPerVertex( x_dim, z_dim, x_spacing, z_spacing, 
                                        height, ccw );
  } else {
    value = generateNormalsPerFace( x_dim, z_dim, x_spacing, z_spacing, 
                                    height, ccw );
  }
}

void ElevationGrid::DisplayList::callList( bool build_list ) {
  ElevationGrid *cgn = 
    static_cast< ElevationGrid * >( owner );
    
  bool ccw = cgn->ccw->getValue();  
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
    if( ccw )
      glFrontFace( GL_CW );
      else  
        glFrontFace( GL_CCW );
  } else {
    if( ccw )
      glFrontFace( GL_CCW );
    else 
      glFrontFace( GL_CW );
  }
  X3DGeometryNode::DisplayList::callList( build_list );
  
  // Restore the front face to its previuos value.
  glFrontFace( front_face );
}

void ElevationGrid::render() {
  X3DGeometryNode::render();
  H3DInt32 xdim = xDimension->getValue();
  H3DInt32 zdim = zDimension->getValue();
  H3DFloat xspace = xSpacing->getValue();
  H3DFloat zspace = zSpacing->getValue();
  bool color_per_vertex = colorPerVertex->getValue();
  X3DColorNode *color_node = color->getValue();
  X3DTextureCoordinateNode *tex_coord_node = texCoord->getValue();
  FogCoordinate *fog_coord_node = fogCoord->getValue();
  TextureCoordinateGenerator *tex_coord_gen = 
    dynamic_cast< TextureCoordinateGenerator * >( tex_coord_node );

  X3DNormalNode *normals = normal->getValue();
  if( !normals ) {
    normals = autoNormal->getValue();
  }

  const vector< H3DFloat > &heights = height->getValue();
  if( heights.size() < (unsigned long) xdim * zdim ) {
    Console(3) << "Warning: Not enough height values in ElevationGrid node( "
               << getName() << "). Node will not be rendered. " << endl;
    return;
  }

  if( xdim > 1 && zdim > 1 ) { 

    unsigned int quad_index = 0;
    unsigned int vertex_index = 0;

    if ( color_node ) {
      color_node->preRender();
    } 

    if( tex_coord_gen ) {
      startTexGen( tex_coord_gen );
    }

    // fog coordinates
    if( GLEW_EXT_fog_coord && fog_coord_node ) {
      glPushAttrib( GL_FOG_BIT );
      glFogi(GL_FOG_COORDINATE_SOURCE_EXT, GL_FOG_COORDINATE_EXT);
    }

    GLhandleARB shader_program = 0;
    // Set the attribute index to use for all vertex attributes
    if( GLEW_ARB_shader_objects && GLEW_ARB_vertex_shader ) {
      shader_program = glGetHandleARB( GL_PROGRAM_OBJECT_ARB );
      if( shader_program ) {
        for( unsigned int i = 0; i < attrib->size(); ++i ) {
          X3DVertexAttributeNode *attr = attrib->getValueByIndex( i );
          if( attr ) {
            GLint loc = 
              glGetAttribLocationARB( shader_program, 
                                      attr->name->getValue().c_str()); 
            attr->setAttribIndex( loc );
          }
        }
      }
    }

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

    if( prefer_vertex_buffer_object && ( !color_node || color_per_vertex ) &&
        normalPerVertex->getValue() &&
        ( normal->getValue() || creaseAngle->getValue() >= Constants::pi ) ) {
      // We can use vertex buffer object to create elevation grid. Note that if
      // something changes the normals vector during this iteration we will
      // likely have a crash. Maybe I should do something about it.
      // Use vertex buffer objects to create elevation grid.
      if( !vboFieldsUpToDate->isUpToDate() ) {
        vboFieldsUpToDate->upToDate();
        // Only create and transfer data when it has been modified.
        unsigned int nr_data_vertices = 3; // 3 floats per vertex.
        if( !tex_coord_node ) {
          // texture coordinates needs to be added.
          // add three floats per vertex;
          nr_data_vertices += 3;
        }
        GLsizei data_size = (GLsizei)( xdim * zdim * nr_data_vertices );
        GLfloat * vertices = new GLfloat[data_size];
        unsigned int nr_index_data = 6; // 2 triangles, 3 vertices/triangle.
        GLsizei index_data_size =
          (GLsizei)( ( xdim -1 ) * ( zdim - 1 ) * nr_index_data );
        GLuint * indices = new GLuint[index_data_size];
        int index_index = 0;
        for( int z = 0; z < zdim; ++z ) {
          for( int x = 0; x < xdim; ++x ) {
            int base_index = z * xdim + x;
            int vertex_index = base_index * nr_data_vertices;
            vertices[vertex_index] = x * xspace;
            vertices[vertex_index + 1] = heights[base_index];
            vertices[vertex_index + 2] = z * zspace;
            if( !tex_coord_node ) {
              vertices[vertex_index + 3] = x / (float)(xdim - 1);
              vertices[vertex_index + 4] = z / (float)(zdim - 1);
              vertices[vertex_index + 5] = 0;
            }

             
            if( z < zdim - 1 && x < xdim - 1 ) {
              indices[index_index] = base_index;
              indices[index_index + 1] = base_index + xdim;
              indices[index_index + 2] = base_index + 1;
              indices[index_index + 3] = base_index + 1;
              indices[index_index + 4] = base_index + xdim;
              indices[index_index + 5] = base_index + xdim + 1;
              index_index += 6;
            }
          }
        }
        if( !vbo_id ) {
          vbo_id = new GLuint[2];
          glGenBuffersARB( 2, vbo_id );
        }
        glBindBufferARB( GL_ARRAY_BUFFER_ARB, vbo_id[0] );
        glBufferDataARB( GL_ARRAY_BUFFER_ARB,
                         data_size * sizeof(GLfloat),
                         vertices, GL_STATIC_DRAW_ARB );
        glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, vbo_id[1] );
        glBufferDataARB( GL_ELEMENT_ARRAY_BUFFER_ARB,
                             index_data_size * sizeof(GLuint),
                             indices, GL_STATIC_DRAW_ARB );
        delete [] vertices;
        delete [] indices;
      } else {
        glBindBufferARB( GL_ARRAY_BUFFER_ARB, vbo_id[0] );
        glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, vbo_id[1] );
      }

      // Enable all states for vertex buffer objects.
      // Note that the data is interleaved since this supposedly should be
      // faster on some systems.
      int nr_data_items = 3;
      if( !tex_coord_node )
        nr_data_items += 3;
      glEnableClientState(GL_VERTEX_ARRAY);
      glVertexPointer( 3, GL_FLOAT, nr_data_items * sizeof(GLfloat), NULL );
      if( !tex_coord_node ) {
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glTexCoordPointer( 3, GL_FLOAT, nr_data_items * sizeof(GLfloat),
                           (GLvoid*)( 3 * sizeof(GLfloat) ) );
      } else if( !tex_coord_gen ) {
        tex_coord_node->renderVertexBufferObjectForActiveTexture();
      }
      normals->renderVertexBufferObject();
      if( color_node )
        color_node->renderVertexBufferObject();

      // Draw the triangles
      glDrawRangeElements( GL_TRIANGLES,
                           0,
                           (GLsizei)(zdim * xdim - 1),
                           (GLsizei)( ( xdim-1 ) * ( zdim-1 ) * 6),
                           GL_UNSIGNED_INT,
                           NULL );

      if( color_node )
        color_node->disableVertexBufferObject();
      normals->disableVertexBufferObject();
      // Disable state.
      if( !tex_coord_node ) {
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
      } else if( !tex_coord_gen ) {
        tex_coord_node->disableVertexBufferObjectForActiveTexture();
      }
      glDisableClientState(GL_VERTEX_ARRAY);
      glBindBufferARB( GL_ARRAY_BUFFER_ARB, 0 );
      glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, 0 );
    } else {
      for( int z = 0; z < zdim - 1; ++z ) {
        for( int x = 0; x < xdim - 1; ++x ) {
          if( color_node && !color_per_vertex ) {
            color_node->render( quad_index );
          }

          // set up normals if the normals are specified per face
          if( normals ) {
            if ( !normalPerVertex->getValue() || creaseAngle->getValue() <= 0 ) {
              normals->render( quad_index );
            }
          }

          glBegin( GL_QUADS );
        
          // vertex 0
          vertex_index = z * xdim + x;
          if( color_node && color_per_vertex ) {
            color_node->render( vertex_index );
          }
   
          if( !tex_coord_gen ) {
            if( tex_coord_node ) {
              tex_coord_node->renderForActiveTexture( vertex_index );
            } else {
              renderTexCoordForActiveTexture( Vec3f( x / (float)(xdim - 1), 
                                                     z / (float)(zdim - 1), 
                                                     0 ) );
            }
          }
              
          // Set up normals if the normals are specified per vertex.
          if( normals ) {
            if ( normalPerVertex->getValue() && creaseAngle->getValue() > 0 ) {
              if( normal->getValue() ) {
                normals->render( vertex_index );
              } else {
                // normals have been automatically generated.
                if( creaseAngle->getValue() < Constants::pi )
                  normals->render( quad_index * 4 );
                else 
                  normals->render( vertex_index );
              }
            }
          } 
        
          glVertex3f( x * xspace, heights[vertex_index], z * zspace ); 
          ++vertex_index;

          // vertex 1
          vertex_index = (z+1) * xdim + x;
          if( color_node && color_per_vertex ) {
            color_node->render( vertex_index );
          }
          if(  fog_coord_node ){
            fog_coord_node->render(vertex_index);
          }
          if( !tex_coord_gen ) {
            if( tex_coord_node ) {
              tex_coord_node->renderForActiveTexture( vertex_index );
            } else {
              renderTexCoordForActiveTexture( Vec3f( x / (float)(xdim - 1), 
                                                     (z+1) / (float) (zdim - 1),
                                                     0 ) );
            }
          }

          // Set up normals if the normals are specified per vertex.
          if( normals ) {
            if ( normalPerVertex->getValue() && creaseAngle->getValue() > 0 ) {
              if( normal->getValue() ) {
                normals->render( vertex_index );
              } else {
                // normals have been automatically generated.
                if( creaseAngle->getValue() < Constants::pi )
                  normals->render( quad_index * 4 + 1 );
                else 
                  normals->render( vertex_index );
              }
            }
          } 
          glVertex3f( x * xspace, heights[vertex_index], (z+1) * zspace ); 

          // vertex 2
          vertex_index = (z+1) * xdim + x + 1;
          if( color_node && color_per_vertex ) {
            color_node->render( vertex_index );
          }
          if( fog_coord_node){
            fog_coord_node->render(vertex_index);
          }
          if( !tex_coord_gen ) {
            if( tex_coord_node ) {
              tex_coord_node->renderForActiveTexture( vertex_index );
            } else {
              renderTexCoordForActiveTexture( Vec3f( (x+1) / (float)(xdim - 1), 
                                                     (z+1) / (float) (zdim - 1), 
                                                     0 ) );
            }
          }

          // Set up normals if the normals are specified per vertex.
          if( normals ) {
            if ( normalPerVertex->getValue() && creaseAngle->getValue() > 0 ) {
              if( normal->getValue() ) {
                normals->render( vertex_index );
              } else {
                // normals have been automatically generated.
                if( creaseAngle->getValue() < Constants::pi )
                  normals->render( quad_index * 4 + 2 );
                else 
                  normals->render( vertex_index );
              }
            }
          } 
          glVertex3f( (x+1) * xspace, heights[vertex_index], (z+1) * zspace ); 

          // vertex 3
          vertex_index = z * xdim + x + 1;
          if( color_node && color_per_vertex ) {
            color_node->render( vertex_index );
          }
          if( fog_coord_node){
            fog_coord_node->render(vertex_index);
          }
          if( !tex_coord_gen ) {
            if( tex_coord_node ) {
              tex_coord_node->renderForActiveTexture( vertex_index );
            } else {
              renderTexCoordForActiveTexture( Vec3f( (x+1) / (float)(xdim - 1), 
                                                     z / (float) (zdim - 1), 
                                                     0 ) );
            }
          }

          // Set up normals if the normals are specified per vertex.
          if( normals ) {
            if ( normalPerVertex->getValue() && creaseAngle->getValue() > 0 ) {
              if( normal->getValue() ) {
                normals->render( vertex_index );
              } else {
                // normals have been automatically generated.
                if( creaseAngle->getValue() < Constants::pi )
                  normals->render( quad_index * 4 + 3 );
                else 
                  normals->render( vertex_index );
              }
            }
          } 
          glVertex3f( (x+1) * xspace, heights[vertex_index], z * zspace ); 

          glEnd();
          ++quad_index;
        }
      }
    }

    if( GLEW_EXT_fog_coord && fog_coord_node ) {
      glPopAttrib();
    }

    if( tex_coord_gen ) {
      stopTexGen( tex_coord_gen );
    }
    if ( color_node ) {
      color_node->postRender();
    }

   
  } 
}

void ElevationGrid::startTexGen( 
                            TextureCoordinateGenerator *tex_coord_gen ) {
  if( !tex_coord_gen ) {
    BoxBound *box_bound = 
      dynamic_cast< BoxBound * >( bound->getValue() );
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
  }  else {
    MultiTexture *mt = 
      dynamic_cast< MultiTexture * >( X3DTextureNode::getActiveTexture() );
    if( mt ) {
      size_t texture_units = mt->texture->size();
      for( size_t i = 0; i < texture_units; ++i ) {
        glActiveTexture( GL_TEXTURE0_ARB + (unsigned int) i );
        tex_coord_gen->startTexGen();
      }
    } else {
      tex_coord_gen->startTexGen();
    }
    
  }    
}

void ElevationGrid::stopTexGen( 
                       TextureCoordinateGenerator *tex_coord_gen ) {
  MultiTexture *mt = 
    dynamic_cast< MultiTexture * >( X3DTextureNode::getActiveTexture() );
  if( !tex_coord_gen ) {
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
  } else {
    if( mt ) {
      size_t texture_units = mt->texture->size();
      for( size_t i = 0; i < texture_units; ++i ) {
        glActiveTexture( GL_TEXTURE0_ARB + (unsigned int) i );
        tex_coord_gen->stopTexGen();
      }
    } else {
      tex_coord_gen->stopTexGen();
    }
  }
}

X3DNormalNode *ElevationGrid::AutoNormal::generateNormalsPerVertex(  
                               H3DInt32 x_dim,
                               H3DInt32 z_dim,
                               H3DFloat x_spacing,
                               H3DFloat z_spacing,
                               const vector< H3DFloat > &height,
                               bool ccw ) {
  Normal *normal = new Normal;
  if( x_dim > 1 && z_dim > 1 ) {
    vector< Vec3f > normals( x_dim * z_dim, 
                             Vec3f( 0, 0, 0 ) );
    AutoRef< X3DNormalNode > normals_per_face;
    normals_per_face.reset( generateNormalsPerFace( x_dim, z_dim, 
                                                    x_spacing, z_spacing,
                                                    height, ccw ) );
    for( unsigned int face = 0; 
         face < normals_per_face->nrAvailableNormals(); ++face ) {
      int row = face / (x_dim-1);
      int col = face % (x_dim-1);
      Vec3f norm = normals_per_face->getNormal( face );
      normals[ row * x_dim + col ] += norm; 
      normals[ row * x_dim + col + 1] += norm; 
      normals[ (row+1) * x_dim + col ] += norm; 
      normals[ (row+1) * x_dim + col + 1] += norm; 
    }
    
    for( vector<Vec3f>::iterator i = normals.begin(); 
         i != normals.end(); 
         ++i ) {
      (*i).normalizeSafe();
    }
    normal->vector->setValue( normals );
  }
  return normal;
}

X3DNormalNode *ElevationGrid::AutoNormal::generateNormalsPerVertex( 
                                  H3DInt32 x_dim,
                                  H3DInt32 z_dim,
                                  H3DFloat x_spacing,
                                  H3DFloat z_spacing,
                                  const vector< H3DFloat > &height,
                                  bool ccw,
                                  H3DFloat crease_angle ) {
  Normal *normal = new Normal;
  if( x_dim > 1 && z_dim > 1 ) {
    AutoRef< X3DNormalNode > normals_per_face;
    normals_per_face.reset( generateNormalsPerFace( x_dim, z_dim,
                                                    x_spacing, z_spacing,
                                                    height, ccw ) );
    vector< Vec3f > normals( (x_dim -1) * (z_dim -1 ) * 4,
                             Vec3f( 0, 0, 0 ) );
    normals.clear();

    std::map< int, vector< Vec3f > > point_to_face_normal_map;

    H3DFloat cos_crease_angle = H3DCos( crease_angle );

    // build a map from each vertex to a vector of all the normals 
    // of the faces the vertex is a part of.
    for( int face = 0; face < (int)normals_per_face->nrAvailableNormals(); ++face ) {

      int quad_x_dim = x_dim - 1;
      int quad_z_dim = z_dim - 1;

      Vec3f norm = normals_per_face->getNormal( face );

      int left_quad = face - 1;
      if( face / (quad_x_dim) != left_quad / ( quad_x_dim ) ) left_quad = -1;
      int upper_quad = face - quad_x_dim;
      if( upper_quad < 0 ) upper_quad = -1;
      int right_quad = face + 1;
      if( face / (quad_x_dim) != right_quad / ( quad_x_dim ) ) right_quad = -1;
      int lower_quad = face + quad_x_dim;
      if( lower_quad >= quad_x_dim * quad_z_dim ) lower_quad = -1;

      // 0----3
      // |    |
      // |    |
      // 1----2

      // vertex 0
      Vec3f n = norm;      
      if( left_quad != -1 ) {
        Vec3f quad_normal = normals_per_face->getNormal( left_quad );
        if( quad_normal * norm > cos_crease_angle )
          n+=quad_normal;
      }
      if( left_quad != -1 && upper_quad != -1 ) {
        Vec3f quad_normal = normals_per_face->getNormal( upper_quad - 1 );
        if( quad_normal * norm > cos_crease_angle )
          n+=quad_normal;
      }
      if( upper_quad != -1 ) {
        Vec3f quad_normal = normals_per_face->getNormal( upper_quad );
        if( quad_normal * norm > cos_crease_angle )
          n+=quad_normal;
      }
      n.normalizeSafe();
      normals.push_back( n );

      // vertex 1
      n = norm;      
      if( left_quad != -1 ) {
        Vec3f quad_normal = normals_per_face->getNormal( left_quad );
        if( quad_normal * norm > cos_crease_angle )
          n+=quad_normal;
      }
      if( left_quad != -1 && lower_quad != -1 ) {
        Vec3f quad_normal = normals_per_face->getNormal( lower_quad - 1 );
        if( quad_normal * norm > cos_crease_angle )
          n+=quad_normal;
      }
      if( lower_quad != -1 ) {
        Vec3f quad_normal = normals_per_face->getNormal( lower_quad );
        if( quad_normal * norm > cos_crease_angle )
          n+=quad_normal;
      }
      n.normalizeSafe();
      normals.push_back( n );

      // vertex 2
      n = norm;      
      if( right_quad != -1 ) {
        Vec3f quad_normal = normals_per_face->getNormal( right_quad );
        if( quad_normal * norm > cos_crease_angle )
          n+=quad_normal;
      }
      if( right_quad != -1 && lower_quad != -1 ) {
        Vec3f quad_normal = normals_per_face->getNormal( lower_quad + 1 );
        if( quad_normal * norm > cos_crease_angle )
          n+=quad_normal;
      }
      if( lower_quad != -1 ) {
        Vec3f quad_normal = normals_per_face->getNormal( lower_quad );
        if( quad_normal * norm > cos_crease_angle )
          n+=quad_normal;
      }
      n.normalizeSafe();
      normals.push_back( n );

      // vertex 3
      n = norm;      
      if( right_quad != -1 ) {
        Vec3f quad_normal = normals_per_face->getNormal( right_quad );
        if( quad_normal * norm > cos_crease_angle )
          n+=quad_normal;
      }
      if( right_quad != -1 && upper_quad != -1 ) {
        Vec3f quad_normal = normals_per_face->getNormal( upper_quad + 1 );
        if( quad_normal * norm > cos_crease_angle )
          n+=quad_normal;
      }
      if( upper_quad != -1 ) {
        Vec3f quad_normal = normals_per_face->getNormal( upper_quad );
        if( quad_normal * norm > cos_crease_angle )
          n+=quad_normal;
      }
      n.normalizeSafe();
      normals.push_back( n );
    }
    normal->vector->setValue( normals );
  }
  return normal;
}

X3DNormalNode *ElevationGrid::AutoNormal::generateNormalsPerFace( 
  H3DInt32 x_dim,
                                  H3DInt32 z_dim,
                                  H3DFloat x_spacing,
                                  H3DFloat z_spacing,
                                  const vector< H3DFloat > &height,
                                  bool ccw ) {
  Normal *normal = new Normal;
  if( x_dim > 1 && z_dim > 1 ) {
    unsigned int quad_x_dim = x_dim -1;
    unsigned int quad_z_dim = z_dim -1;
    unsigned int nr_quads = quad_x_dim * quad_z_dim;

    // A----D
    // |    |
    // |    |
    // B----C

    vector< Vec3f > normals( nr_quads );
    normals.clear();

    for( unsigned int face = 0; face < nr_quads; ++face ) {
      Vec3f norm, AB, AD, CB, CD;
      H3DFloat A_height, B_height, C_height, D_height;
      int row = face / quad_x_dim;
      int col = face % quad_x_dim;

      int vertex_index = row * x_dim + col;

      A_height = height[ vertex_index ];
      B_height = height[ vertex_index + x_dim ];
      C_height = height[ vertex_index + x_dim + 1 ];
      D_height = height[ vertex_index + 1 ];
      
      AB = Vec3f( 0, B_height - A_height, z_spacing);
      AD = Vec3f( x_spacing, D_height - A_height, 0);

      CB = Vec3f( -x_spacing, B_height - C_height, 0);
      CD = Vec3f( 0, D_height - C_height, -z_spacing);

      Vec3f n0 = AB % AD;
      Vec3f n1 = CD % CB;
      norm = n0 + n1;
      try {
        norm.normalize();
      } catch ( const ArithmeticTypes::Vec3f::Vec3fNormalizeError & ) {
        norm = Vec3f( 0, 1, 0 );
      }
      normals.push_back( norm );
    }
    normal->vector->setValue( normals );
  }
  return normal;
}

void ElevationGrid::SFBound::update() {
  H3DInt32 xdim = static_cast< SFInt32 * >( routes_in[0] )->getValue();
  H3DInt32 zdim = static_cast< SFInt32 * >( routes_in[1] )->getValue();
  H3DFloat xspace = static_cast< SFFloat * >( routes_in[2] )->getValue();
  H3DFloat zspace = static_cast< SFFloat * >( routes_in[3] )->getValue();
  const vector< H3DFloat > &height = 
    static_cast< MFFloat * >( routes_in[4] )->getValue();

  // find the max and min height values.  
  H3DFloat max_h, min_h;
  if( height.size() > 0 ) {
    min_h = max_h = height[0];
    for( vector< H3DFloat >::const_iterator i = height.begin();
         i != height.end(); ++i ) {
      if( *i < min_h ) min_h = *i;
      else if( *i  > max_h ) max_h = *i;
    }
  } else {
    max_h = min_h = 0;
  }

  Vec3f size( (xdim - 1) * xspace, max_h - min_h, (zdim - 1) * zspace); 
  
  BoxBound *bb = new BoxBound;
  bb->size->setValue( size );
  bb->center->setValue( Vec3f( 0, min_h, 0 ) + size / 2 );
  value = bb;
}

void ElevationGrid::traverseSG( TraverseInfo &ti ) {
  X3DGeometryNode::traverseSG( ti );
  if( solid->getValue() ) {
    useBackFaceCulling( true );
  } else {
    useBackFaceCulling( false );
  }
}

