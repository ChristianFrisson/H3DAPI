import time
import random
import math
import os

# Files generated when this variable is true might not load on your system.
generate_super_big_files = False
generate_big_files = False

# Class that can be used to generate geometry files.
class GenerateGeometryFiles:
  class FunctionArguments:
    def __init__( self, file_handle = None, shape_function = None, nr_shapes = None, \
                  appearance_function = None,  geometry_function = None, x_size = None, \
                  y_size = None, columns = None, rows = None,
                  meta_title = "", meta_description = "", meta_author = "SenseGraphics AB, 2012", \
                  line_prefix = "    ", node_names = None ):
      # File to write to.
      self.file_handle = file_handle
      # Shapes information
      self.shape_function = shape_function
      self.nr_shapes = nr_shapes
      self.shape_container_line_prefix = line_prefix
      self.shape_line_prefix = line_prefix + "  "
      # Appearance information
      self.appearance_function = appearance_function
      self.appearance_line_prefix = self.shape_line_prefix + "  "
      # Geometry information
      self.geometry_function = geometry_function
      self.x_size = x_size
      self.y_size = y_size
      self.columns = columns
      self.rows = rows
      self.geometry_line_prefix = self.shape_line_prefix + "    "
      # X3D information
      self.meta_title = meta_title
      self.meta_description = meta_description
      self.meta_author = meta_author
      # other information
      # Contains node type names and their names. Each time a name is used then a number will be added to start of the
      # name in order to give each node a unique name in a file. May not be properly used everywhere yet, currently
      # only in generateITSSyntax/generateIFSSyntax
      # The list should look like this [[nodetypename0, nodename0, counter0], [nodetypename1, nodename1, counter1] ... [nodetypenameN, nodenameN, counterN] ]
      # Where nodetypename is the type name of the node, nodename is the name the node should have, the actual name will be counter_nodename
      self.node_names = node_names
  
    def getGeometryArguments( self ):
      return [self.file_handle, self.x_size, self.y_size, self.columns, self.rows, self.geometry_line_prefix]
    
    def getAppearanceArguments( self ):
      return [self.file_handle, self.appearance_line_prefix]
      
    def getShapeArguments( self ):
      return [self.file_handle, self.nr_shapes, self.shape_line_prefix, self.x_size, self.y_size, self.geometry_function, self.appearance_function]
    
    def getX3DSyntaxArguments( self ):
      return [self.file_handle, self.meta_title, self.meta_description, self.meta_author, self.shape_function]
    
    def generateNodeNameString( self, index ):
      prefix = str( self.node_names[index][2] ) + "_"
      self.node_names[index][2] = self.node_names[index][2] + 1
      return "DEF=\"" + prefix + self.node_names[index][1] + "\" "
      
      
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
    self.generateITSFile( self.FunctionArguments( nr_shapes = 1, \
                                                  x_size = 0.4, \
                                                  y_size = 0.4, \
                                                  columns = 2, \
                                                  rows = 2 ) )
    self.generateITSFile( self.FunctionArguments( nr_shapes = 500, \
                                                  x_size = 0.4, \
                                                  y_size = 0.4, \
                                                  columns = 2, \
                                                  rows = 2 ) )

    # 100 triangles ITS
    self.generateITSFile( self.FunctionArguments( nr_shapes = 1, \
                                                  x_size = 0.4, \
                                                  y_size = 0.4, \
                                                  columns = 11, \
                                                  rows = 6 ) )
    self.generateITSFile( self.FunctionArguments( nr_shapes = 500, \
                                                  x_size = 0.4, \
                                                  y_size = 0.4, \
                                                  columns = 11, \
                                                  rows = 6 ) )

    # 1000 triangles ITS
    self.generateITSFile( self.FunctionArguments( nr_shapes = 1, \
                                                  x_size = 0.4, \
                                                  y_size = 0.4, \
                                                  columns = 11, \
                                                  rows = 51 ) )
    self.generateITSFile( self.FunctionArguments( nr_shapes = 500, \
                                                  x_size = 0.4, \
                                                  y_size = 0.4, \
                                                  columns = 11, \
                                                  rows = 51 ) )
    # 1000 triangles IFS
    self.generateIFSFile( self.FunctionArguments( nr_shapes = 1, \
                                                  x_size = 0.4, \
                                                  y_size = 0.4, \
                                                  columns = 11, \
                                                  rows = 51 ) )
    self.generateIFSFile( self.FunctionArguments( nr_shapes = 500, \
                                                  x_size = 0.4, \
                                                  y_size = 0.4, \
                                                  columns = 11, \
                                                  rows = 51 ) )

    # 10000 triangles ITS
    self.generateITSFile( self.FunctionArguments( nr_shapes = 1, \
                                                  x_size = 0.4, \
                                                  y_size = 0.4, \
                                                  columns = 101, \
                                                  rows = 51 ) )
    self.generateITSFile( self.FunctionArguments( nr_shapes = 500, \
                                                  x_size = 0.4, \
                                                  y_size = 0.4, \
                                                  columns = 101, \
                                                  rows = 51 ) )
    # 10000 triangles IFS
    self.generateIFSFile( self.FunctionArguments( nr_shapes = 1, \
                                                  x_size = 0.4, \
                                                  y_size = 0.4, \
                                                  columns = 101, \
                                                  rows = 51 ) )
    self.generateIFSFile( self.FunctionArguments( nr_shapes = 1, \
                                                  x_size = 0.4, \
                                                  y_size = 0.4, \
                                                  columns = 101, \
                                                  rows = 51 ) )

    # 100000 triangles ITS
    self.generateITSFile(self.FunctionArguments( nr_shapes = 1, \
                                                  x_size = 0.4, \
                                                  y_size = 0.4, \
                                                  columns = 101, \
                                                  rows = 501 ) )
    self.generateITSFile( self.FunctionArguments( nr_shapes = 1, \
                                                  x_size = 0.4, \
                                                  y_size = 0.4, \
                                                  columns = 101, \
                                                  rows = 501,
                                                  node_names = [["Coordinate", "COORD_pointTOUCH", 0]] ), True, "PhongTouchedPointField", " and a PhongShader and also touched point field" )
    self.generateITSFile( self.FunctionArguments( nr_shapes = 1, \
                                                  x_size = 0.4, \
                                                  y_size = 0.4, \
                                                  columns = 101, \
                                                  rows = 501,
                                                  node_names = [["Coordinate", "COORD_pointTOUCH", 0]] ), False, "TouchedPointField", " and a touched point field" )
    self.generateITSFile( self.FunctionArguments( nr_shapes = 1, \
                                                  x_size = 0.4, \
                                                  y_size = 0.4, \
                                                  columns = 101, \
                                                  rows = 501 ), True, "PhongStatic", " and a PhongShader" )
    if generate_super_big_files:
      self.generateITSFile( self.FunctionArguments( nr_shapes = 500, \
                                                    x_size = 0.4, \
                                                    y_size = 0.4, \
                                                    columns = 101, \
                                                    rows = 501 ) )

    # 100000 triangles IFS
    self.generateIFSFile( self.FunctionArguments( nr_shapes = 1, \
                                                  x_size = 0.4, \
                                                  y_size = 0.4, \
                                                  columns = 101, \
                                                  rows = 501 ) )

    # 1000000 triangles ITS
    if generate_big_files:
      self.generateITSFile( self.FunctionArguments( nr_shapes = 1, \
                                                    x_size = 0.4, \
                                                    y_size = 0.4, \
                                                    columns = 1001, \
                                                    rows = 501 ) )
    if generate_super_big_files:
      self.generateITSFile( self.FunctionArguments( nr_shapes = 500, \
                                                    x_size = 0.4, \
                                                    y_size = 0.4, \
                                                    columns = 1001, \
                                                    rows = 501 ) )

    # 1000000 triangles IFS
    if generate_big_files:
      self.generateIFSFile( self.FunctionArguments( nr_shapes = 1, \
                                                    x_size = 0.4, \
                                                    y_size = 0.4, \
                                                    columns = 1001, \
                                                    rows = 501 ) )

    print "All files successfully generated in " + str( time.time() - begin_time ) + " seconds."
    print ""

  # Generate xml syntax for IndexedTriangleSet and write to file.
  # Nr of triangles will be 2 * (columns - 1) * (rows - 1)
  def generateITSSyntax( self, function_arguments ):
    file_handle, x_size, y_size, columns, rows, line_prefix = function_arguments.getGeometryArguments()
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
          if nr_index_sixlets % 8 == 0:
            file_handle.write( ",\n" + line_prefix + "                           " )
          else:
            file_handle.write( ", " )
    
    node_name = ""
    if function_arguments.node_names:
      for i, l in enumerate( function_arguments.node_names ):
        if l[0] == "Coordinate":
          node_name = function_arguments.generateNodeNameString( i )
    file_handle.write( "\" >\n" + line_prefix + "  <Coordinate " + node_name + "point=\"" )

    # The reason for having two loops is that writing sequentially to file is much much
    # faster than storing an intermediate result in a string.
    nr_coords = 0
    for c in range( columns ):
      for r in range( rows ):
        file_handle.write( str( step_c * c - x_size / 2 ) + " " + str( step_r * r - y_size/2 ) + " %f" % ( random.random() * step_z ) )
        nr_coords = nr_coords + 1
        if nr_coords < columns * rows:
          if nr_coords % 5 == 0:
            file_handle.write( ",\n" + line_prefix + "                     " )
          else:
            file_handle.write( ", " )
    
    file_handle.write( "\" />\n" + line_prefix + "  <TextureCoordinate point=\"" )
    
    nr_coords = 0
    for c in range( columns ):
      for r in range( rows ):
        file_handle.write( str( tc_step_c * c ) + " " + str( tc_step_r * r ) )
        nr_coords = nr_coords + 1
        if nr_coords < columns * rows:
          if nr_coords % 5 == 0:
            file_handle.write( ",\n" + line_prefix + "                            " )
          else:
            file_handle.write( ", " )
   
    file_handle.write( "\" />\n" + line_prefix + "</IndexedTriangleSet>\n" )

  # Generate xml syntax for a shape containing a geometry that is defined by the geometry_function.
  # \return A z value of a viewpoint with position (0,0,z) calculated such that all shapes are visible.
  def generateShapeSyntax( self, function_arguments ):
    file_handle, nr_shapes, line_prefix, x_size, y_size, geometry_function, appearance_function = function_arguments.getShapeArguments()
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
    container_line_prefix = function_arguments.shape_container_line_prefix
    for c in range( columns ):
      for r in range( rows ):
        file_handle.write( container_line_prefix + "<Transform translation=\"" )
        file_handle.write( str( min_x + step_c * c ) + " " + str( min_y + step_r * r ) + " %f" % ( random.random() * step_z ) )
        file_handle.write( "\" >\n" )
        file_handle.write( line_prefix + "<Shape>\n" )
        appearance_function( function_arguments )      
        geometry_function( function_arguments )
        file_handle.write( line_prefix + "</Shape>\n" )
        file_handle.write( container_line_prefix + "</Transform>\n" )
    vp_z_value_x = 2 * ( -min_x + x_size / 2 ) / math.sin( math.pi / 4 )
    vp_z_value_y = 2 * ( -min_y + y_size / 2 ) / math.sin( math.pi / 4 )
    if vp_z_value_x > vp_z_value_y:
      return vp_z_value_x
    else:
      return vp_z_value_y

  # Generate xml syntax for an X3D scene.
  def generateX3DSyntax( self, function_arguments ):
    file_handle, meta_title, meta_description, meta_author, content_function = function_arguments.getX3DSyntaxArguments()
    file_handle.write( "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n" + \
                       "<X3D profile='Full' version='3.2'>\n" + \
                       "  <head>\n" + \
                       "    <meta name='title' content='" + meta_title + "' />\n" + \
                       "    <meta name='description' content='" + meta_description + "' />\n" + \
                       "    <meta name='author' content='" + meta_author + "' />\n" + \
                       "  </head>\n" + \
                       "  <Scene>\n" )
    vp_z_value = content_function( function_arguments )
    file_handle.write( "    <Viewpoint DEF='VP' position='0 0 " + str( vp_z_value ) + "' />\n" )
    if function_arguments.node_names:
      for i, l in enumerate( function_arguments.node_names ):
        if l[1].endswith( "TOUCH" ):
          pos = l[1].rfind( "_" )
          if pos != -1:
            field_name = l[1][ pos+1:len(l[1])-5 ]
          file_handle.write( """    <PythonScript >
    <%s USE="%s_%s" containerField="references" />
          <![CDATA[python:from H3DInterface import *

the_node, = references.getValue()

class TouchField( AutoUpdate( SFTime ) ):
  def __init__( self, field_to_touch ):
    AutoUpdate( SFTime ).__init__(self)
    self.field_to_touch = field_to_touch
    time.routeNoEvent( self )

  def update( self, event ):
    self.field_to_touch.touch()
    return event.getValue()

touchField = TouchField(the_node.%s)
]]>
          </PythonScript>""" % (l[0], str(i), l[1], field_name) )
    file_handle.write( "  </Scene>\n" + \
                       "</X3D>\n" )

  # Generate indexedtriangleset files.
  def generateITSFile( self, function_arguments, with_phong = False, filename_suffix = "", meta_description_suffix = "" ):
    before_time = time.time()
    nr_triangles = 2 * (function_arguments.columns-1) * (function_arguments.rows-1)
    if function_arguments.nr_shapes > 1:
      filename = str( function_arguments.nr_shapes ) + "IndexedTriangleSet" + str( nr_triangles ) + "Triangles" + filename_suffix + ".x3d"
      function_arguments.meta_description = "A file containing " + str( function_arguments.nr_shapes ) + " IndexedTriangleSet with " + str( nr_triangles ) +" triangles each" + meta_description_suffix + "."
    else:
      filename = "IndexedTriangleSet" + str( nr_triangles ) + "Triangles" + filename_suffix + ".x3d"
      function_arguments.meta_description = "A file containing an IndexedTriangleSet with " + str( nr_triangles ) +" triangles" + meta_description_suffix + "."
    filename = self.sub_dir + filename
    if self.regenerate_if_exists or not os.path.isfile( filename ):
      print "Generating file " + filename
      file_handle = open( filename, 'w' )
      function_arguments.file_handle = file_handle
      function_arguments.shape_function = self.generateShapeSyntax
      if with_phong:
        function_arguments.appearance_function = self.generateAppearanceSyntaxWithPhong
      else:
        function_arguments.appearance_function = self.generateAppearanceSyntax
      function_arguments.geometry_function = self.generateITSSyntax
      function_arguments.meta_title = filename
      self.generateX3DSyntax( function_arguments )
      file_handle.close()
      print "Done, took " + str( time.time() - before_time ) + "s\n"
  
  # Generate IndexedFaceSet files.
  def generateIFSFile( self, function_arguments, with_phong = False, filename_suffix = "", meta_description_suffix = "" ):
    before_time = time.time()
    nr_triangles = 2 * (function_arguments.columns-1) * (function_arguments.rows-1)
    if function_arguments.nr_shapes > 1:
      filename = str( function_arguments.nr_shapes ) + "IndexedFaceSet" + str( nr_triangles ) + "Faces" + filename_suffix + ".x3d"
      function_arguments.meta_description = "A file containing " + str( function_arguments.nr_shapes ) + " IndexedFaceSet with " + str( nr_triangles ) +" faces each" + meta_description_suffix + "."
    else:
      filename = "IndexedFaceSet" + str( nr_triangles ) + "Faces" + filename_suffix + ".x3d"
      function_arguments.meta_description = "A file containing an IndexedFaceSet with " + str( nr_triangles ) +" faces" + meta_description_suffix + "."
    filename = self.sub_dir + filename
    if self.regenerate_if_exists or not os.path.isfile( filename ):
      print "Generating file " + filename
      file_handle = open( filename, 'w' )
      function_arguments.file_handle = file_handle
      function_arguments.shape_function = self.generateShapeSyntax
      if with_phong:
        function_arguments.appearance_function = self.generateAppearanceSyntaxWithPhong
      else:
        function_arguments.appearance_function = self.generateAppearanceSyntax
      function_arguments.geometry_function = self.generateIFSSyntax
      function_arguments.meta_title = filename
      self.generateX3DSyntax( function_arguments )
      file_handle.close()
      print "Done, took " + str( time.time() - before_time ) + "s\n"

  # Generate xml syntax for IndexedTriangleSet and write to file.
  # Nr of triangles will be 2 * (columns - 1) * (rows - 1)
  def generateIFSSyntax( self, function_arguments ):
    file_handle, x_size, y_size, columns, rows, line_prefix = function_arguments.getGeometryArguments()

    step_c = x_size / (columns-1)
    step_r = y_size / (rows-1)

    tc_step_c = 1.0/ (columns-1)
    tc_step_r = 1.0/ (rows-1)
    step_z = step_c
    if step_r < step_z:
      step_z = step_r
    step_z = step_z / 4
    
    file_handle.write( line_prefix + "<IndexedFaceSet coordIndex=\"" )
    nr_index_sixlets = 0
    for c in range( columns - 1 ):
      for r in range( rows - 1 ):
        v0 = rows * c + r
        v1 = rows * ( c + 1 ) + r
        v2 = v0 + 1
        v3 = v1 + 1
        file_handle.write( str(v0) + " " + str(v3) + " " + str(v2) + " -1, " + str(v0) + " " + str(v1) + " " + str(v3) )
        nr_index_sixlets = nr_index_sixlets + 1
        if nr_index_sixlets < ( columns - 1 ) * ( rows - 1 ):
          if nr_index_sixlets % 8 == 0:
            file_handle.write( "-1,\n" + line_prefix + "                            " )
          else:
            file_handle.write( "-1, " )
    
    file_handle.write( "\" creaseAngle='4' normalPerVertex='true' >\n" + line_prefix + "  <Coordinate point=\"" )

    # The reason for having two loops is that writing sequentially to file is much much
    # faster than storing an intermediate result in a string.
    nr_coords = 0
    for c in range( columns ):
      for r in range( rows ):
        file_handle.write( str( step_c * c - x_size / 2 ) + " " + str( step_r * r - y_size/2 ) + " %f" % ( random.random() * step_z ) )
        nr_coords = nr_coords + 1
        if nr_coords < columns * rows:
          if nr_coords % 5 == 0:
            file_handle.write( ",\n" + line_prefix + "                     " )
          else:
            file_handle.write( ", " )
    
    file_handle.write( "\" />\n" + line_prefix + "  <TextureCoordinate point=\"" )
    
    nr_coords = 0
    for c in range( columns ):
      for r in range( rows ):
        file_handle.write( str( tc_step_c * c ) + " " + str( tc_step_r * r ) )
        nr_coords = nr_coords + 1
        if nr_coords < columns * rows:
          if nr_coords % 5 == 0:
            file_handle.write( ",\n" + line_prefix + "                            " )
          else:
            file_handle.write( ", " )
   
    file_handle.write( "\" />\n" + line_prefix + "</IndexedFaceSet>\n" )

  def generateAppearanceSyntax( self, function_arguments ):
    file_handle, line_prefix = function_arguments.getAppearanceArguments()
    file_handle.write( line_prefix + "  <Appearance>\n" + \
                       line_prefix + "    <Material/>\n" )
    file_handle.write( line_prefix + "  </Appearance>\n" )
  
  def generateAppearanceSyntaxWithPhong( self, function_arguments ):
    file_handle, line_prefix = function_arguments.getAppearanceArguments()
    file_handle.write( line_prefix + "  <Appearance>\n" + \
                       line_prefix + "    <Material/>\n" )
    file_handle.write( line_prefix + "    <PhongShader normalMapCoordSpace=\"TANGENT\" normalMapMatrix=\"2 0 0 -1   0 -2 0 1  0 0 2 -1  0 0 0 1\">\n" + \
                       line_prefix + "      <ImageTexture containerField=\"normalMap\" url=\"../../All/images/stone_wall_normal_map.bmp\" />\n" + \
                       line_prefix + "      <ImageTexture containerField=\"diffuseMap\" url=\"../../All/images/stone_wall.bmp\" />\n" + \
                       line_prefix + "    </PhongShader>\n" )
    file_handle.write( line_prefix + "  </Appearance>\n" )
