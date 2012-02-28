# This python file outputs information to console as well to a file named
# h3d_testsuite_yyyymmdd_hhmmss.xml'. Open that file in a web browser to see the
# result.
#"Files in H3DAPI\examples\TestSuite will be loaded by this file.
# Those files must contain a viewpoint named VP
# Any node in those files whose name ends with TOUCH will be assumed to be named
# _fieldnameTOUCH where fieldname is the name of the field in the node that should be
# touched once per scenegraph node.

#Idea from Neil. Make program generate good settings file, or make a program that does.

# internal python module includes
import datetime
import os
import math
import time as python_time
import platform
import sys

# h3d includes
from H3DInterface import *
sys.path.append( "geometries" )
import generateGeometryFiles

# Settings for where to output the statistics.
print_to_file = True
print_to_console = True

# How long each file should be loaded.
test_file_duration = 10

# Each entry in the list consists of [nodename, fieldname, value0, value1.... valueN]
# All permutation of values for fields in the list will be tested.
#fields_to_edit = [["GraphicsOptions", "preferVertexBufferObject", True, False ], [ "GraphicsOptions", "useCaching", True, False], ["GeometryBoundTreeOptions", "maxTrianglesInLeaf", 1, 10, 100 ] ]
fields_to_edit = [["GraphicsOptions", "preferVertexBufferObject", True, False ], [ "GraphicsOptions", "useCaching", True, False] ]

# Template string used for generating the xsl-file that generates html-code from the xml file that contains the data stored by this program.
xsl_template_string = """<?xml version="1.0" encoding="ISO-8859-1"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

<xsl:template match="/">
  <!-- An xsl file uses XPath syntax for selecting nodes, read up on that to understand the code below.-->
  <html>
  <body>
  <h1>Test suite output</h1>
    <h3>System information:</h3>
    <text>%s
    </text>
    <h3>Settings data:</h3>
    <table border="1">
      <tr bgcolor="#9acd32">
        <th></th>
        <xsl:for-each select="testsuite/settings/option">
          <th><xsl:value-of select="name"/></th>
        </xsl:for-each>
      </tr>
      <xsl:for-each select="testsuite/settings/option">
        <xsl:if test="position()=1">
          <xsl:variable name="settings_count" select="count(value)" />
          <xsl:for-each select="value">
            <tr>
              <xsl:variable name="settings_value_pos" select="position()" />
              <td>Test<xsl:value-of select="$settings_value_pos"/></td>
              <xsl:for-each select="/testsuite/settings/option/value">
                <xsl:if test="position() mod $settings_count=$settings_value_pos mod $settings_count">
                  <td align="center"><xsl:value-of select="."/></td>
                </xsl:if>
              </xsl:for-each>
            </tr>
          </xsl:for-each>
        </xsl:if>
      </xsl:for-each>
    </table>
    <br />
    <br />
    <h3>Test results:</h3>
    <table border="1">
        <!-- Generate first row with filenames -->
        <tr bgcolor="#9acd32">
          <th>Filename</th>
          <xsl:for-each select="testsuite/file">
            <th colspan="%s"><xsl:value-of select="name"/></th>
          </xsl:for-each>
        </tr>
        <xsl:for-each select="testsuite/file">
          <!-- Use structure of first file as template to find data in other files -->
          <xsl:if test="position()=1">
            <xsl:variable name="output_per_file" select="count(output)" />
            <xsl:for-each select="output">
              <xsl:variable name="first_file_output_pos" select="position()" />
              <tr bgcolor="#1bcd32">
                <td><xsl:value-of select="name" /></td>
                <!-- Iterate through all outputs and select only those who fills the criteria below -->
                <xsl:for-each select="/testsuite/file/output">
                  <xsl:if test="position() mod $output_per_file=$first_file_output_pos mod $output_per_file">
                    <xsl:for-each select="type/name">
                      <td align="center"><xsl:value-of select="." /></td>
                    </xsl:for-each>
                  </xsl:if>
                </xsl:for-each>
              </tr>
              <xsl:for-each select="type">
                <xsl:if test="position()=1">
                  <xsl:variable name="value_per_type" select="count(value)" />
                  <xsl:for-each select="value">
                    <tr>
                      <xsl:variable name="type_value_pos" select="position()" />
                      <td>Test<xsl:value-of select="$type_value_pos"/></td>
                      <!-- Iterate through all files and select only those output whose position in that file
                           element corresponds to the position of the output element in the first file element.-->
                      <xsl:for-each select="/testsuite/file">
                        <xsl:for-each select="output">
                          <xsl:if test="position()=$first_file_output_pos">
                              <!-- Iterate through all values in this output element and select only those who fills the criteria below -->
                              <xsl:for-each select="type/value">
                                <xsl:if test="position() mod $value_per_type=$type_value_pos mod $value_per_type">
                                  <td align="center"><xsl:value-of select="."/></td>
                                </xsl:if>
                              </xsl:for-each>
                          </xsl:if>
                        </xsl:for-each>
                      </xsl:for-each>
                    </tr>
                  </xsl:for-each>
                </xsl:if>
              </xsl:for-each>
            </xsl:for-each>
          </xsl:if>
        </xsl:for-each>
    </table>
  </body>
  </html>
</xsl:template>
</xsl:stylesheet>"""

# Get group from x3d file.
content_group, = references.getValue()  

# Find this directory.
test_suite_directory = ""
if os.getenv("H3D_ROOT") != None:
  test_suite_directory = os.environ["H3D_ROOT"] + "\examples\TestSuite"

def printToConsole( to_print ):
  if print_to_console:
    print to_print

printToConsole( "Platform: " + str( platform.platform() ) + "\n" )
printToConsole( "Processor: " + str( platform.processor() ) + "\n" )
# Might have to add commands here to use some program to get information about graphics card.

# Create a list of files that should be checked.
files_to_check = []
if os.path.isdir( test_suite_directory ):
  for root, dirs, files in os.walk(test_suite_directory):
    if '.svn' in dirs:
      dirs.remove('.svn')  # don't visit CVS directories
    if 'TestAll.x3d' in files:
      files.remove( 'TestAll.x3d' ) # don't visit the base x3d file.
    
    for tempFile in files:
      if tempFile.endswith('.x3d'):
        files_to_check.append( [root, tempFile] )
else:
  print "The directory " + test_suite_directory + " does not exist. Check that H3D_ROOT is set correctly."

# Generate geometry test files. They can be quite big.
generateGeometryFiles.GenerateGeometryFiles( "geometries", False )

# Field class that call Field.touch() on all fields given to this
# class in the constructor.
class TouchFields( AutoUpdate( SFTime ) ):
  def __init__( self ):
    AutoUpdate( SFTime ).__init__(self)
    self.fields_to_touch = []
    time.routeNoEvent( self )

  def update( self, event ):
    for f in self.fields_to_touch:
      f.touch()
    return event.getValue()
  
  def addFieldToTouch( self, f ):
    self.fields_to_touch.append( f )

  def clearFieldToTouch( self ):
    self.fields_to_touch = []

# Field class that takes care of checking each file.
class CheckFile( AutoUpdate( SFTime ) ):

  # Contains statistics for each file.
  class FileStatistics:
    def __init__(self):
      # Lists, each value corresponds to one test
      self.graphics_min = []
      self.graphics_max = []
      self.graphics_avg = []
      self.graphics_mean = []
      # Will be lists of lists. Each list contains values for each test on a specific
      # haptics device.
      self.haptics_min = []
      self.haptics_max = []
      self.haptics_avg = []
      self.haptics_mean = []
    
    # Function that can be used to store graphics data.
    def addGraphicsData( self, min, max, avg, mean, args ):
      self.graphics_min.append( min )
      self.graphics_max.append( max )
      self.graphics_avg.append( avg )
      self.graphics_mean.append( mean )
    
    # Function that can be used to store haptics data.
    def addHapticsData( self, min, max, avg, mean, args ):
      if args >= len( self.haptics_min ):
        for i in range( len( self.haptics_min ), args + 1, 1 ):
          self.haptics_min.append([])
      if args >= len( self.haptics_max ):
        for i in range( len( self.haptics_max ), args + 1, 1 ):
          self.haptics_max.append([])
      if args >= len( self.haptics_avg ):
        for i in range( len( self.haptics_avg ), args + 1, 1 ):
          self.haptics_avg.append([])
      if args >= len( self.haptics_mean ):
        for i in range( len( self.haptics_mean ), args + 1, 1 ):
          self.haptics_mean.append([])
      self.haptics_min[args].append( min )
      self.haptics_max[args].append( max )
      self.haptics_avg[args].append( avg )
      self.haptics_mean[args].append( mean )
    
    # Help function for writeDataToXMLFile
    def writeDataUnitToXMLFile( self, file_handle, data_unit, data_unit_name ):
      file_handle.write( "      <type>\n"
                             "        <name>" + data_unit_name + "</name>\n" )
      for v in data_unit:
        file_handle.write( "        <value>" + v +"</value>\n" )
      file_handle.write( "      </type>\n")
    
    # Call this to write an xml element for this file.
    def writeDataToXMLFile( self, file_handle ):
      file_handle.write( "    <output>\n" )
      file_handle.write( "      <name>Graphics frame rate</name>\n" )
      self.writeDataUnitToXMLFile( file_handle, self.graphics_min, "min" )
      self.writeDataUnitToXMLFile( file_handle, self.graphics_max, "max" )
      self.writeDataUnitToXMLFile( file_handle, self.graphics_avg, "avg" )
      self.writeDataUnitToXMLFile( file_handle, self.graphics_mean, "mean" )
      file_handle.write( "    </output>\n" )
      for i in range( len( self.haptics_min ) ):
        file_handle.write( "    <output>\n" )
        file_handle.write( "      <name>Haptics rate, device " + str( i ) + "</name>\n" )
        self.writeDataUnitToXMLFile( file_handle, self.haptics_min[i], "min" )
        self.writeDataUnitToXMLFile( file_handle, self.haptics_max[i], "max" )
        self.writeDataUnitToXMLFile( file_handle, self.haptics_avg[i], "avg" )
        self.writeDataUnitToXMLFile( file_handle, self.haptics_mean[i], "mean" )
        file_handle.write( "    </output>\n" )

  # Constructor
  def __init__( self, files_to_check, test_file_duration ):
    AutoUpdate( SFTime ).__init__(self)
    global fields_to_edit
    # A list of [path, filename] entries for each file.
    self.files_to_check = files_to_check
    self.test_file_start_time = None
    # The time that each file should be loaded.
    self.test_file_duration = test_file_duration
    time.routeNoEvent( self )
    # Contains a list of index into the global variable fields_to_edit. Used to know
    # the state of each options field.
    self.options_fields_index = None
    # Used for printout to console
    self.file_test_nr = 0
    self.touchField = TouchFields()
    # Contains statistics for each file.
    self.statistics = dict()
    # The currently loaded file.
    self.current_file = ""
    expected_test_time = 1
    for f in fields_to_edit:
      expected_test_time = expected_test_time * ( len( f ) - 2 )
    expected_test_time = expected_test_time * len( files_to_check ) * float( test_file_duration )
    printToConsole( "START TESTING. Expected test duration is at least " + str( datetime.timedelta( seconds=expected_test_time ) ) + ".\n" )
  
  def update( self, event ):
    global content_group, fields_to_edit
    if self.test_file_start_time == None:
      if len( self.files_to_check ) > 0:
        # There are files to check, start by updating the index list of which options
        # should be changed.
        self.updateOptionsFieldsIndex()
        if self.options_fields_index[0] >= len( fields_to_edit[0] ):
          self.options_fields_index[0] = 2
          self.files_to_check.pop(0)
          self.file_test_nr = 0
        
        # If file was not removed because last check was already done then load file.
        if len( self.files_to_check ) > 0:
          file_url = os.path.join( self.files_to_check[0][0], self.files_to_check[0][1] )
          if self.file_test_nr == 0:
            printToConsole( "Testing file " + str( file_url ) + "\n" )
          self.file_test_nr = self.file_test_nr + 1
          printToConsole( str( self.files_to_check[0][1] ) + " test " + str( self.file_test_nr ) + "\n" )
          self.current_file = self.files_to_check[0][1]
          if self.current_file not in self.statistics:
            self.statistics[ self.current_file ] = self.FileStatistics()
          self.updateOptions()
          node, dn = createX3DNodeFromURL( file_url )
          content_group.children.setValue( [node] )
          dn["VP"].set_bind.setValue( True )
          self.graphics_frame_rate = []
          self.haptics_frame_rate = []
          # Find fields which should be touched each frame.
          for n in dn:
            if n.endswith( "TOUCH" ):
              pos = n.rfind( "_" )
              if pos != -1:
                f = dn[n].getField( n[ pos+1:len(n)-5 ] )
                if f:
                  self.touchField.addFieldToTouch( f )
          # Measure time here, since file is now loaded.
          self.test_file_start_time = python_time.time()
        else:
          # Testing done.
          printToConsole( "Testing done" )
          self.options_fields_index = None
          self.current_file = ""
          self.writeDataToXMLFile()
    elif python_time.time() - self.test_file_start_time > self.test_file_duration:
      # File should be closed, store last sample and write to file. Then
      # remove file.
      self.storeDataSample()
      self.writeDataToConsole()
      self.touchField.clearFieldToTouch()
      #dn["VP"].set_bind.setValue( False )
      content_group.children.clear()
      self.test_file_start_time = None
    else:
      # File is loaded, store data for later.
      self.storeDataSample()
      
    return event.getValue()
  
  # Stores data in internal variables.
  def storeDataSample( self ):
    scene = getCurrentScenes()[0]
    self.graphics_frame_rate.append( scene.frameRate.getValue() )
    di = getActiveDeviceInfo()
    if di:
      for i, device in enumerate( di.device.getValue() ):
        while i >= len( self.haptics_frame_rate ):
          self.haptics_frame_rate.append( [] )
        self.haptics_frame_rate[i].append( device.hapticsRate.getValue() )
  
  # Returns a string with frame rate data.
  def calculateFrameRateData( self, frame_rate_list, add_data_function, extra_arguments = "" ):
    global print_to_file
    min = "%.2f" % frame_rate_list[0]
    max = "%.2f" % frame_rate_list[len( frame_rate_list ) - 1]
    avg = "%.2f" % (math.fsum( frame_rate_list ) / float( len( frame_rate_list ) ) )
    mean = "%.2f" % float( frame_rate_list[len( frame_rate_list ) / 2] )
    if print_to_file:
      add_data_function( min, max, avg, mean, extra_arguments )
      
    return "    Min: " + min + "\n" \
           "    Max: " + max + "\n" \
           "    Avg: " + avg + "\n" \
           "    Mean: " + mean + "\n"
  
  # Write data to file and/or console.
  def writeDataToConsole( self ):
    self.graphics_frame_rate.sort()
    data_string = "  Graphics frame rate: \n" + self.calculateFrameRateData( self.graphics_frame_rate, self.statistics[self.current_file].addGraphicsData )
    if len( self.haptics_frame_rate ) > 0:
      data_string = data_string + "  Haptics frame rate: \n"
      for i, l in enumerate( self.haptics_frame_rate ):
        l.sort()
        data_string = data_string + "  Device " + str( i ) + ":\n" + self.calculateFrameRateData( l, self.statistics[self.current_file].addHapticsData, i )
    printToConsole( data_string + "\n" )
  
  # Write data to a file if it should be stored.
  def writeDataToXMLFile( self ):
    global print_to_file, fields_to_edit
    if print_to_file:
      # Generate output_file name.
      now = datetime.datetime.now()
      output_file_name = "h3d_testsuite_%d%02d%02d_%02d%02d%02d" % (now.year, now.month, now.day, now.hour, now.minute, now.second )
      #output_file_name = "h3d_testsuite"
      # Open file and output some information about system that the test is run on.
      output_file_xml = open( output_file_name + ".xml", 'w' )
      output_file_xml.write( "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n" \
                             "<?xml-stylesheet type=\"text/xsl\" href=\"" + output_file_name + ".xsl\"?>\n" \
                             "<testsuite>\n" \
                             "  <settings>\n" )
      settings_strings = []
      for f in fields_to_edit:
        settings_strings.append( [f[0] + "::" + f[1]] )
      while True:
        self.updateOptionsFieldsIndex()
        if self.options_fields_index[0] >= len( fields_to_edit[0] ):
          break
        for i,f in enumerate( fields_to_edit ):
          settings_strings[i].append( f[self.options_fields_index[i]] )
      for entry in settings_strings:
        output_file_xml.write( "    <option>\n" \
                               "      <name>" + entry[0] + "</name>\n" )
        for j in range( 1, len(entry), 1 ):
          output_file_xml.write( "      <value>" + str(entry[j]) + "</value>\n" )
        output_file_xml.write( "    </option>\n" )
      output_file_xml.write( "  </settings>\n" )
      nr_entries_per_test = 4
      nr_tests = 0
      for i, s in enumerate( self.statistics ):
        if i == 0:
          nr_tests = len( self.statistics[s].graphics_min )
        output_file_xml.write( "  <file>\n" )
        output_file_xml.write( "    <name>" + s + "</name>\n" )
        self.statistics[s].writeDataToXMLFile( output_file_xml )          
        output_file_xml.write( "  </file>\n" )
      output_file_xml.write( "</testsuite>\n" )
      output_file_xml.close()
      
      output_file_xsl = open( output_file_name + ".xsl", 'w' )
      output_file_xsl.write( xsl_template_string % ( "Platform: " + str( platform.platform() ) + "<br />\n           Processor: " + str( platform.processor() ), \
                                                     str( nr_entries_per_test )  ) )
      output_file_xsl.close()


  # Update fields in options node and print that information to file and/or console.
  def updateOptions( self ):
    global fields_to_edit
    global_settings = getActiveGlobalSettings()
    data_string = "Settings values:\n"
    for i, j in enumerate( self.options_fields_index ):
      option_found = False
      for options_node in global_settings.options.getValue():
        if options_node.getTypeName() == fields_to_edit[i][0]:
          f = options_node.getField( fields_to_edit[i][1] )
          if f:
            f.setValue( fields_to_edit[i][j] )
            option_found = True
            break
      if option_found:
        data_string = data_string + "  " + fields_to_edit[i][0] + "::" + fields_to_edit[i][1] + " = " + str( fields_to_edit[i][j] ) + "\n"
      else:
        print "WARNING: Option " + fields_to_edit[i][0] + " with field " + fields_to_edit[i][1] + " not found in GlobalSettings, check the fields_to_edit variable in TestAll.py"
    printToConsole( data_string )
  
  # Function used to increase one of the indices in self.options_fields_index.
  def updateOptionsFieldsIndex( self ):
    global fields_to_edit
    if self.options_fields_index == None:
      if len( fields_to_edit ) > 0:
        self.options_fields_index = [2] * len( fields_to_edit )
    else:
      for i in range( len( self.options_fields_index ) -1, -1, -1 ):
        self.options_fields_index[i] = self.options_fields_index[i] + 1
        if i != 0:
          if self.options_fields_index[i] >= len( fields_to_edit[i] ):
            self.options_fields_index[i] = 2
          else:
            break

# Create an instance of the class. Will start collecting data.
checkFile = CheckFile( files_to_check, test_file_duration )
