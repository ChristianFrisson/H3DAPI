import time
import random
import math
import os

# Files generated when this variable is true might not load on your system.
generate_super_big_files = False

# Class that can be used to generate geometry files.
class GenerateGeometryFiles:
  # Constructor
  # \param sub_dir Path to a subdirectory in which files should be stored.
  # \param regenerate_if_exists If true files are regenerated even if they exists.
  def __init__( self, sub_dir, regenerate_if_exists = True ):
    global generate_super_big_files
    self.sub_dir = sub_dir
    self.regenerate_if_exists = regenerate_if_exists
    if self.sub_dir != "" and ( not self.sub_dir.endswith( "/" ) ) and ( not self.sub_dir.endswith( "\\" ) ):
      self.sub_dir = self.sub_dir + "/"
    begin_time = time.time()
    random.seed( begin_time )
    # 2 triangles ITS
    self.generateITSFile( 2, 2, 0.4, 0.4, 1 )
    self.generateITSFile( 2, 2, 0.4, 0.4, 500 )

    # 100 triangles ITS
    self.generateITSFile( 11, 6, 0.4, 0.4, 1 )
    self.generateITSFile( 11, 6, 0.4, 0.4, 500 )

    # 1000 triangles ITS
    self.generateITSFile( 11, 51, 0.4, 0.4, 1 )
    self.generateITSFile( 11, 51, 0.4, 0.4, 500 )

    # 10000 triangles ITS
    self.generateITSFile( 101, 51, 0.4, 0.4, 1 )
    self.generateITSFile( 101, 51, 0.4, 0.4, 500 )

    # 100000 triangles ITS
    self.generateITSFile( 101, 501, 0.4, 0.4, 1 )
    if generate_super_big_files:
      self.generateITSFile( 101, 501, 0.4, 0.4, 500 )

    # 1000000 triangles ITS
    self.generateITSFile( 1001, 501, 0.4, 0.4, 1 )
    if generate_super_big_files:
      self.generateITSFile( 1001, 501, 0.4, 0.4, 500 )

    print "All files successfully generated in " + str( time.time() - begin_time ) + " seconds."

  # Generate xml syntax for IndexedTriangleSet and write to file.
  # Nr of triangles will be 2 * (columns - 1) * (rows - 1)
  def generateITSSyntax( self, file_handle, x_size, y_size, data ):
    columns, rows, line_prefix = data

    step_c = x_size / (columns-1)
    step_r = y_size / (rows-1)

    tc_step_c = 1.0/ (columns-1)
    tc_step_r = 1.0/ (rows-1)
    step_z = step_c
    if step_r < step_z:
      step_z = step_r
    step_z = step_z / 4
    
    file_handle.write( line_prefix + "<IndexedTriangleSet index=\"" )
    nr_index_sixlets = 0
    for c in range( columns - 1 ):
      for r in range( rows - 1 ):
        v0 = rows * c + r
        v1 = rows * ( c + 1 ) + r
        v2 = v0 + 1
        v3 = v1 + 1
        file_handle.write( str(v0) + " " + str(v3) + " " + str(v2) + ", " + str(v0) + " " + str(v1) + " " + str(v3) )
        nr_index_sixlets = nr_index_sixlets + 1
        if nr_index_sixlets < ( columns - 1 ) * ( rows - 1 ):
          file_handle.write( ", " )
    
    file_handle.write( "\" >\n" + line_prefix + "  <Coordinate point=\"" )

    nr_coords = 0
    for c in range( columns ):
      for r in range( rows ):
        file_handle.write( str( step_c * c - x_size / 2 ) + " " + str( step_r * r - y_size/2 ) + " %f" % ( random.random() * step_z ) )
        nr_coords = nr_coords + 1
        if nr_coords < columns * rows:
          file_handle.write( ", " )
    
    file_handle.write( "\" />\n" + line_prefix + "  <TextureCoordinate point=\"" )
    
    nr_coords = 0
    for c in range( columns ):
      for r in range( rows ):
        file_handle.write( str( tc_step_c * c ) + " " + str( tc_step_r * r ) )
        nr_coords = nr_coords + 1
        if nr_coords < columns * rows:
          file_handle.write( ", " )
   
    file_handle.write( "\" />\n" + line_prefix + "</IndexedTriangleSet>\n" )

  # Generate xml syntax for a shape containing a geometry that is defined by the geometry_function.
  # \return A z value of a viewpoint with position (0,0,z) calculated such that all shapes are visible.
  def generateShapeSyntax( self, file_handle, line_prefix, nr_shapes, x_size, y_size, geometry_function, geometry_args ):
    padding_x = x_size / 100.0
    padding_y = y_size / 100.0
    columns = int( math.sqrt( nr_shapes ) )
    rows = columns
    while columns * rows < nr_shapes:
      rows = rows + 1
    step_c = padding_x + x_size
    step_r = padding_y + y_size
    step_z = step_c
    if step_r < step_z:
      step_z = step_r
    min_x = -( columns - 1 ) * ( x_size +  padding_x ) / 2
    min_y = -( rows - 1 ) * ( y_size +  padding_y ) / 2
    line_prefix_shape = line_prefix + "  "
    geometry_args.append( line_prefix_shape + "  " )
    for c in range( columns ):
      for r in range( rows ):
        file_handle.write( line_prefix + "<Transform translation=\"" )
        file_handle.write( str( min_x + step_c * c ) + " " + str( min_y + step_r * r ) + " %f" % ( random.random() * step_z ) )
        file_handle.write( "\" >\n" )
        file_handle.write( line_prefix_shape + "<Shape>\n" + \
                           line_prefix_shape + "  <Appearance>\n" + \
                           line_prefix_shape + "    <Material/>\n" + \
                           #line_prefix_shape + "    <SmoothSurface/>\n" + \
                           line_prefix_shape + "  </Appearance>\n" )
      
        geometry_function( file_handle, x_size, y_size, geometry_args )
        file_handle.write( line_prefix_shape + "</Shape>\n" )
        file_handle.write( line_prefix + "</Transform>\n" )
    vp_z_value_x = 2 * ( -min_x + x_size / 2 ) / math.sin( math.pi / 4 )
    vp_z_value_y = 2 * ( -min_y + y_size / 2 ) / math.sin( math.pi / 4 )
    if vp_z_value_x > vp_z_value_y:
      return vp_z_value_x
    else:
      return vp_z_value_y

  # Generate xml syntax for an X3D scene.
  def generateX3DSyntax( self, file_handle, meta_title, meta_description, meta_author, content_function, content_args  ):
    file_handle.write( "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n" + \
                       "<X3D profile='Full' version='3.2'>\n" + \
                       "  <head>\n" + \
                       "    <meta name='title' content='" + meta_title + "' />\n" + \
                       "    <meta name='description' content='" + meta_description + "' />\n" + \
                       "    <meta name='author' content='" + meta_author + "' />\n" + \
                       "  </head>\n" + \
                       "  <Scene>\n" )
    nr_content_nodes = content_args.pop(0)
    x_size = content_args.pop(0)
    y_size = content_args.pop(0)
    geom_function = content_args.pop(0)
    vp_z_value = content_function( file_handle, "    ", nr_content_nodes, x_size, y_size, geom_function, content_args )
    file_handle.write( "    <Viewpoint DEF='VP' position='0 0 " + str( vp_z_value ) + "' />\n"
                       "  </Scene>\n" + \
                       "</X3D>\n" )

  # Generate indexedtriangleset files.
  def generateITSFile( self, columns, rows, x_size, y_size, nr_its ):
    before_time = time.time()
    nr_triangles = 2 * (columns-1) * (rows-1)
    if nr_its > 1:
      filename = str( nr_its ) + "IndexedTriangleSet" + str( nr_triangles ) + "Triangles.x3d"
      meta_description = "A file containing " + str( nr_its ) + " IndexedTriangleSet with " + str( nr_triangles ) +" triangles each."
    else:
      filename = "IndexedTriangleSet" + str( nr_triangles ) + "Triangles.x3d"
      meta_description = "A file containing an IndexedTriangleSet with " + str( nr_triangles ) +" triangles."
    filename = self.sub_dir + filename
    if self.regenerate_if_exists or not os.path.isfile( filename ):
      print "Generating file " + filename
      file_handle = open( filename, 'w' )
      self.generateX3DSyntax( file_handle, filename, meta_description, "SenseGraphics AB, 2012", self.generateShapeSyntax, [nr_its, x_size, y_size, self.generateITSSyntax, columns, rows] )
      file_handle.close()
      print "Done, took " + str( time.time() - before_time ) + "s\n"
