# This python file outputs information to console as well to a file named
# h3d_testsuite_yyyymmdd_hhmmss.txt'
#"Files in H3DAPI\examples\TestSuite will be loaded by this file.
#Those files must contain a viewpoint named VP

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


generateGeometryFiles.GenerateGeometryFiles( "geometries" )

print_to_file = True
print_to_console = True

now = datetime.datetime.now()
#output_file_name = "h3d_testsuite_%d%02d%02d_%02d%02d%02d.txt" % (now.year, now.month, now.day, now.hour, now.minute, now.second )
output_file_name = "h3d_testsuite.txt"
output_file = None

test_file_duration = 10

# Each entry in the list consists of [nodename, fieldname, value0, value1.... valueN]
# Each permutation of values for fields in the list will be tested.
#fields_to_edit = [["GraphicsOptions", "preferVertexBufferObject", True, False ], [ "GraphicsOptions", "useCaching", True, False], ["GeometryBoundTreeOptions", "maxTrianglesInLeaf", 1, 10, 100 ] ]
fields_to_edit = [["GraphicsOptions", "preferVertexBufferObject", True, False ], [ "GraphicsOptions", "useCaching", True, False] ]

def printResult( string ):
  if print_to_file:
    output_file.write( string )
  if print_to_console:
    print string

content_group, = references.getValue()  

test_suite_directory = ""
if os.getenv("H3D_ROOT") != None:
  test_suite_directory = os.environ["H3D_ROOT"] + "\examples\TestSuite"

output_file = open( output_file_name, 'w' )
printResult( "Platform: " + str( platform.platform() ) + "\n" )
printResult( "Processor: " + str( platform.processor() ) + "\n" )
# Might have to add commands here to use some program to get information about graphics card.

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

class CheckFile( AutoUpdate( SFTime ) ):
  def __init__( self, files_to_check, test_file_duration ):
    AutoUpdate( SFTime ).__init__(self)
    self.files_to_check = files_to_check
    self.start_time = None
    self.test_file_duration = test_file_duration
    time.routeNoEvent( self )
    self.graphics_frame_rates = []
    self.haptics_frame_rates = []
    self.options_fields_index = None
    self.file_test_nr = 0
  
  def update( self, event ):
    global content_group, fields_to_edit
    if self.start_time == None:
      if len( self.files_to_check ) > 0:
        # load file
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
        if self.options_fields_index[0] >= len( fields_to_edit[0] ):
          self.options_fields_index[0] = 2
          self.files_to_check.pop(0)
          self.file_test_nr = 0
          
        if len( self.files_to_check ) > 0:
          file_url = os.path.join( self.files_to_check[0][0], self.files_to_check[0][1] )
          if self.file_test_nr == 0:
            printResult( "Testing file " + str( file_url ) + "\n" )
          self.file_test_nr = self.file_test_nr + 1
          printResult( str( self.files_to_check[0][1] ) + " test " + str( self.file_test_nr ) + "\n" )
          self.updateOptions()
          node, dn = createX3DNodeFromURL( file_url )
          content_group.children.setValue( [node] )
          dn["VP"].set_bind.setValue( True )
          self.graphics_frame_rate = []
          self.haptics_frame_rate = []
          self.start_time = python_time.time()
        else:
          printResult( "Testing done" )
          output_file.close()
    elif python_time.time() - self.start_time > self.test_file_duration:
      self.storeDataSample()
      self.writeData()
      # unload file
      #dn["VP"].set_bind.setValue( False )
      content_group.children.clear()
      self.start_time = None
    else:
      self.storeDataSample()
      
    return event.getValue()
  
  def storeDataSample( self ):
    scene = getCurrentScenes()[0]
    self.graphics_frame_rate.append( scene.frameRate.getValue() )
    di = getActiveDeviceInfo()
    if di:
      for i, device in enumerate( di.device.getValue() ):
        while i >= len( self.haptics_frame_rate ):
          self.haptics_frame_rate.append( [] )
        self.haptics_frame_rate[i].append( device.hapticsRate.getValue() )
  
  def calcualteFrameRateData( self, frame_rate_list ):
    return "    Min: " + str( frame_rate_list[0] ) + "\n" \
           "    Max: " + str( frame_rate_list[len( frame_rate_list ) - 1] ) + "\n" \
           "    Avg: " + str( math.fsum( frame_rate_list ) / len( frame_rate_list ) ) + "\n" \
           "    Mean: " + str( frame_rate_list[len( frame_rate_list ) / 2] ) + "\n"
  
  def writeData( self ):
    self.graphics_frame_rate.sort()
    data_string = "  Graphics frame rate: \n" + self.calcualteFrameRateData( self.graphics_frame_rate )
    if len( self.haptics_frame_rate ) > 0:
      data_string = data_string + "  Haptics frame rate: \n"
      for i, l in enumerate( self.haptics_frame_rate ):
        l.sort()
        data_string = data_string + "  Device " + str( i ) + ":\n" + self.calcualteFrameRateData( l )
    printResult( data_string + "\n" )

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
    printResult( data_string )

checkFile = CheckFile( files_to_check, test_file_duration )
