"""
This script finds runs and generates screenshots and reports for H3DAPI 
examples.

An HTML document is generated that shows the results from each example.

Tests: H3DAPI, H3DPhysics, MedX3D, UI
For H3DPhysics all engines are tested on all examples.

Return code 0 indicates no errors were detected running the examples.

Example usage:
 * Execute from working directory TestExamples
 > python TestExamples.py

Platform notes:
 Currently only implemented for Windows and linux.
 On linux you need to install xautomation tools to have xte command.
 xte is used to send Escape key to H3DLoad.

Additional requirements:
  * PIL: http://www.pythonware.com/products/pil/
  * pywin32: http://sourceforge.net/projects/pywin32/
 
usage: examples_test.py [-h] [--output OUTPUT] path
 
optional arguments:
  -h, --help       show this help message and exit
  --path PATH      root path to search for files
  --output OUTPUT  path to save output to
 
"""

import subprocess
import platform
import time
import os, sys
from Queue import Queue, Empty
from threading  import Thread
import tempfile
import string
import re
import argparse
import math
import signal
from PIL import Image


if platform.system() == 'Windows':
  from PIL import ImageGrab
  import win32api
  import win32com.client

# Find this directory.
test_suite_directory = "."
if os.getenv("H3D_ROOT") != None:
  test_suite_directory = os.environ["H3D_ROOT"] + "/examples/TestSuite"

geometries_directory = test_suite_directory + "/geometries"
sys.path.append( geometries_directory )
import generateGeometryFiles
# Generate geometry test files. They can be quite big.
generateGeometryFiles.GenerateGeometryFiles( geometries_directory, False )

parser = argparse.ArgumentParser(
  description='Finds runs and generates screenshots and reports for H3D examples.')
parser.add_argument('--output', dest='output',
                   default='examples_report',
                   help='path to save output to')
args = parser.parse_args()

def createFilename(orig_url, var_name):
  valid_chars = "-_.() %s%s" % (string.ascii_letters, string.digits)
  filename= orig_url + var_name
  return ''.join(c for c in filename if c in valid_chars).strip ( '.' )

def getExitCode ( results ):
  """ Returns an exit code based on the test results. """

  for result in results:
    for variation in result:
      v= variation[0]
      r= variation[1]
      if isError ( r ):
        return 1
  return 0
  
def isError ( result ):
  return (result.errors > 0 or not result.started_ok or not result.terminated_ok) and not result.skipped
  
class Process ( object ):
  
  def launch ( self, command, cwd ):
    pass

  def testLaunch ( self, command, cwd, startup_time, shutdown_time, startup_time_multiplier, early_shutdown_file ):
    pass
    
  def isRunning ( self ):
    pass
    
  def sendKey ( self, key ):
    pass
    
  def kill ( self ):
    pass
    
  def getStdOut ( self ):
    pass
    
  def getStdErr ( self ):
    pass
    
class ProcessWin32 ( Process ):

  def launch ( self, command, cwd ):
  
    def enqueue_output ( stdout, stderr, std_out_q, std_err_q ):
      for line in iter(stdout.readline, b''):
        std_out_q.put(line)
      stdout.close()
      for line in iter(stderr.readline, b''):
        std_err_q.put(line)
      stderr.close()
  
    self.process_name= command[0]
  
    shell = win32com.client.Dispatch("WScript.Shell")

    import ctypes
    ctypes.windll.kernel32.SetErrorMode(0)

    self.process= subprocess.Popen(command, stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE, cwd= cwd)
    
    self.std_out_q = Queue()
    self.std_err_q = Queue()
    self.monitor_output_thread = Thread(target= enqueue_output, args=(self.process.stdout, self.process.stderr, self.std_out_q, self.std_err_q))
    self.monitor_output_thread.start()
    
  def testLaunch ( self, command, cwd, startup_time, shutdown_time, startup_time_multiplier, early_shutdown_file ):
  
    import ctypes
    SEM_NOGPFAULTERRORBOX = 0x0002 # From MSDN
    ctypes.windll.kernel32.SetErrorMode(SEM_NOGPFAULTERRORBOX)
    subprocess_flags = 0x8000000 #win32con.CREATE_NO_WINDOW?
    
    self.process_name= command[0]
    
    if os.path.isfile( early_shutdown_file ):
      os.remove( early_shutdown_file )
    self.process= subprocess.Popen(command, stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE, cwd= cwd, creationflags= subprocess_flags )
    for i in range( 0, startup_time_multiplier ):
      time.sleep(startup_time)
      if os.path.isfile( early_shutdown_file ) or not self.isRunning():
        break
    success= self.isRunning()
    if success:
      self.kill()
      time.sleep(shutdown_time)
    return success
    
  def isRunning ( self ):
    # for Windows XP use this
    query_process = "tasklist /NH /FI \"IMAGENAME EQ "+ self.process_name +"\""
    
    # for Windows Vista use this
    #query_process = 'query process'
    
    #for Linux use this
    #query_process = 'ps -Af | grep ' + self.process_name + 
    
    p = subprocess.Popen( query_process, stdout=subprocess.PIPE, shell=True )
    output = p.communicate()
    
    return output[0].find(self.process_name) >= 0
    
  def sendKey ( self, key ):
    shell = win32com.client.Dispatch("WScript.Shell")
    shell.AppActivate ( "H3D" )
    time.sleep ( 0.1 )
    shell.SendKeys(key)
    
  def kill ( self ):
    # for Windows XP
    kill_process = "taskkill /F /im "+ self.process_name
    os.system( kill_process )
    
    # If the test crashed, close the crash dialog box too
    shell = win32com.client.Dispatch("WScript.Shell")
    shell.AppActivate ( "H3DLoad.exe" )
    shell.SendKeys("{Esc}")
    
  def getStdOut ( self ):
    output= ""
    try:
      while not self.std_out_q.empty():
        output+= self.std_out_q.get(timeout=.1)
    except Empty:
      pass
    return output
    
  def getStdErr ( self ):
    output= ""
    try:
      while not self.std_err_q.empty():
        output+= self.std_err_q.get(timeout=.1)
    except Empty:
      pass
    return output

class ProcessUnix ( Process ):

  def launch ( self, command, cwd ):
  
    def enqueue_output ( stdout, stderr, std_out_q, std_err_q ):
      for line in iter(stdout.readline, b''):
        std_out_q.put(line)
      stdout.close()
      for line in iter(stderr.readline, b''):
        std_err_q.put(line)
      stderr.close()
  
    self.process_name= command[0]

    self.process= subprocess.Popen(command, stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE, cwd= cwd)
    
    self.std_out_q = Queue()
    self.std_err_q = Queue()
    self.monitor_output_thread = Thread(target= enqueue_output, args=(self.process.stdout, self.process.stderr, self.std_out_q, self.std_err_q))
    self.monitor_output_thread.start()
    
  def testLaunch ( self, command, cwd, startup_time, shutdown_time, startup_time_multiplier, early_shutdown_file ):
  
    self.process_name= command[0]
    if os.path.isfile( early_shutdown_file ):
      os.remove( early_shutdown_file )
    self.process= subprocess.Popen(command, stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE, cwd= cwd )
    for i in range( 0, startup_time_multiplier ):
      time.sleep(startup_time)
      if os.path.isfile( early_shutdown_file ) or not self.isRunning():
        break
    success= self.isRunning()
    if success:
      self.kill()
      time.sleep(shutdown_time)
    return success
    
  def isRunning ( self ):
    #for Linux use this
    query_process = 'ps -Af | grep ' + self.process_name
    
    p = subprocess.Popen( query_process, stdout=subprocess.PIPE, shell=True )
    output = p.communicate()
    output0 = output[0]
    # remove ps and grep process lines.
    output0 = output0.replace( 'ps -Af | grep ' + self.process_name, "" )
    output0 = output0.replace( 'grep ' + self.process_name, "" )
    return output0.find(self.process_name) >= 0 and not (output0.find( '[' + self.process_name + '] <defunct>') >= 0 )
    
  def sendKey ( self, key ):
    time.sleep ( 0.1 )
    if key == "{ESC}" or key == "{Esc}":
#      device = uinput.Device([uinput.KEY_ESC])
#      device.emit_click(uinput.KEY_ESC)
      key_process = 'xte "key Escape"'
      p = subprocess.Popen( key_process, stdout=subprocess.PIPE, shell=True )
    else:
      raise Exception("Key " + key + " not handled exception")
    
  def kill ( self ):
    self.process.kill()
    
  def getStdOut ( self ):
    output= ""
    try:
      while not self.std_out_q.empty():
        output+= self.std_out_q.get(timeout=.1)
    except Empty:
      pass
    return output
    
  def getStdErr ( self ):
    output= ""
    try:
      while not self.std_err_q.empty():
        output+= self.std_err_q.get(timeout=.1)
    except Empty:
      pass
    return output


class Option ( object ):
  
  def __init__ ( self, node_type, field_name, field_value, container_node = "Scene", add_if_missing = False ):
    self.node_type= node_type
    self.field_name= field_name
    self.field_value= field_value
    self.container_node = container_node
    self.add_if_missing = add_if_missing
    
class Variation ( object ):

  def __init__ ( self, name= "Default", insertion_string = "" ):
    self.name= name
    self.options= []
    self.global_insertion_string = insertion_string
    self.global_insertion_string_failed = False
    
  def parse ( self, input ):
    replace_with= ""
    output= input
    reg_use = None
    reg_field = None
    self.global_insertion_string_failed = False
    
    def replace_callback(r):
      if r.group(1):
        if reg_use.search( r.group(0) ):
          # If USE found, then return the pattern as is.
          return r.group(0)
        field_match = reg_field.search( r.group(0) )
        if field_match:
          # If field is found, then simply return a string in which the field is changed.
          return r.group(0)[0:field_match.start()] + replace_with + r.group(0)[field_match.end():]
        else:
          # If field is not found, add it.
          return "<%s %s%s" % (r.group(1),r.group(2),replace_with)
      else:
        return r.group(0)
      
    if self.global_insertion_string != "":
      reg = re.compile( r'<Scene>' )
      output_after = reg.sub('<Scene>\n' + self.global_insertion_string, output)
      if output_after == output:
        self.global_insertion_string_failed = True
      else:
        output = output_after
          
    for o in self.options:
      for node_type in o.node_type:
        reg = re.compile(r'<('+node_type+r')\s([^>/]*)', re.DOTALL)
        reg_use = re.compile(r'\s+USE\s*=\s*')
        reg_field = re.compile(o.field_name+r'\s*=\s*(?:\'[^\']*\'|"[^"]*")' )
        if o.field_name == "":
          replace_with = ""
        else:
          replace_with= " %s=\"%s\"" % (o.field_name, o.field_value)
        output = reg.sub(replace_callback, output)
        
        if o.add_if_missing:
          reg = re.compile( r'<'+node_type+r'[^>]*>')
          if reg.search( output ) == None:
            reg = re.compile( r'(<'+o.container_node+r'[^>]*>\s*)' )
            replace_string = r'\1<' + node_type + ' '
            if o.field_name != "":
              replace_string = replace_string + o.field_name + '=\'' + o.field_value + '\' '
            replace_string = replace_string + '>\n'
            replace_string = replace_string + '</' + node_type + '>\n'
            output = reg.sub( replace_string, output )
    return output
    
class TestResults ( object ):
  
  def __init__ ( self, fps_data_file = "" ):
    self.name= ""
    self.url= ""
    self.std_out= ""
    self.std_err= ""
    self.started_ok= False
    self.terminated_ok= False
    self.screenshot= ""
    self.screenshot_thumb= ""
    self.warnings= 0
    self.errors= 0
    self.skipped= False
    self.created_variation= False
    self.fps_min = ""
    self.fps_max = ""
    self.fps_mean = ""
    self.fps_avg = ""
    self.fps_data_file = fps_data_file
    if os.path.isfile( self.fps_data_file ):
      os.remove( self.fps_data_file )
    self.variation = None

  def calculateFPS(self):
    if os.path.isfile( self.fps_data_file ):
      fps_file = open( self.fps_data_file )
      fps_list = fps_file.read().split()
      fps_file.close()
      fps_list_float = []
      for fps in fps_list:
        fps_list_float.append( float( fps ) )
      if len( fps_list_float ) > 0:
        fps_list_float.sort()
        self.fps_min = "%.2f" % fps_list_float[0]
        self.fps_max = "%.2f" % fps_list_float[len( fps_list_float ) - 1]
        self.fps_avg = "%.2f" % (math.fsum( fps_list_float ) / float( len( fps_list_float ) ) )
        self.fps_mean = "%.2f" % float( fps_list_float[len( fps_list_float ) / 2] )
      
    
class TestExamples ( object ):

  def __init__ ( self, filepath, startup_time= 10, shutdown_time= 5, testable_callback= None, fake_fullscreen_due_to_PIL_bug = True, error_reporter = None ):
    self.filepath= filepath
    self.startup_time= startup_time
    self.shutdown_time= shutdown_time
    self.testable_callback= testable_callback
    self.fps_data_file = '%s/fps.txt' % (os.getcwd())
    self.early_shutdown_file = '%s/test_complete' % (os.getcwd())
    self.screenshot_tmp_file = '%s/tmp_screenshot.png' % (os.getcwd())
    self.startup_time_multiplier = 10
    self.load_flags = ["-f"]
    self.fps_insertion_string = """<PythonScript >
          <![CDATA[python:from H3DInterface import *

class StoreFPS( AutoUpdate( SFFloat ) ):
  def __init__( self ):
    AutoUpdate( SFFloat ).__init__(self)
    self.fps_file = open( '%s', 'w' )
    self.early_shutdown_file = '%s'
    # Used to skip the first iterations.
    self.traverse_sg_counter = 0

  def update( self, event ):
    if self.traverse_sg_counter > 9 and self.traverse_sg_counter < 101:
      self.fps_file.write( str(event.getValue()) + " " )
      self.fps_file.flush()
      if self.traverse_sg_counter == 100:
        takeScreenshot( '%s' )
        shutdown_file = open( self.early_shutdown_file, 'w' )
        shutdown_file.write( "OK" )
        shutdown_file.close()
        self.traverse_sg_counter = 101
    if self.traverse_sg_counter < 100:
      self.traverse_sg_counter = self.traverse_sg_counter + 1
    return 0

storeFPS = StoreFPS()
scene = getCurrentScenes()[0]
scene.frameRate.routeNoEvent( storeFPS )
scene = None
]]>
          </PythonScript>""" % (self.fps_data_file, self.early_shutdown_file, self.screenshot_tmp_file)
    # Used to get intermediate failed results in order to be able to cancel test early and still get some results.
    self.error_reporter = error_reporter

  def launchExample ( self, url, cwd ):
    process = self.getProcess()
    process.launch ( ["H3DLoad.exe" if platform.system() == 'Windows' else "H3DLoad"] + self.load_flags + [url], cwd )
    return process

  def getProcess( self ):
    platform_system = platform.system()
    if platform_system == 'Windows':
      return ProcessWin32()
    elif platform_system == 'Linux':
      return ProcessUnix()

  # This function I found on stackoverflow.com here http://stackoverflow.com/questions/69645/take-a-screenshot-via-a-python-script-linux
  def getScreenByPIL(self):
    import ImageGrab
    img = ImageGrab.grab()
    return img

  # This function I found on stackoverflow.com here http://stackoverflow.com/questions/69645/take-a-screenshot-via-a-python-script-linux
  def getScreenByWx(self):
    import wx
    app = wx.App()  # Need to create an App instance before doing anything
    screen = wx.ScreenDC()
    size = screen.GetSize()
    bmp = wx.EmptyBitmap(size[0], size[1])
    mem = wx.MemoryDC(bmp)
    mem.Blit(0, 0, size[0], size[1], screen, 0, 0)
    #bmp.SaveFile('screenshot.png', wx.BITMAP_TYPE_PNG )
    myWxImage = wx.ImageFromBitmap( bmp )
    PilImage = Image.new( 'RGB', (myWxImage.GetWidth(), myWxImage.GetHeight()) )
    PilImage.fromstring( myWxImage.GetData() )
    del mem  # Release bitmap
    return PilImage

  def getScreenShot( self ):
    if platform.system() == 'Windows':
      return self.getScreenByPIL()
    else:
      return self.getScreenByWx()
    
  def testStartUp ( self, url, cwd, variation ):
    """ Returns true if the example can be started. """
    
    process = self.getProcess()
    return process.testLaunch ( ["H3DLoad.exe" if platform.system() == 'Windows' else "H3DLoad"] + self.load_flags + [url], cwd, self.startup_time, self.shutdown_time, 1 if variation and variation.global_insertion_string_failed else self.startup_time_multiplier, self.early_shutdown_file )
    
  def testExample ( self, name, url, orig_url= None, var_name= "", variation = None ):
    
    if orig_url is None:
      orig_url= url
    
    test_results= TestResults( self.fps_data_file )
    test_results.name= name
    test_results.url= orig_url
    test_results.variation = variation
    
    cwd= os.path.split ( orig_url )[0]
    filename= os.path.abspath ( url )
    
    test_results.started_ok= self.testStartUp ( filename, cwd, variation )
    
    if os.path.isfile( self.early_shutdown_file ):
      os.remove( self.early_shutdown_file )
    if os.path.isfile( self.screenshot_tmp_file ):
      os.remove( self.screenshot_tmp_file )
    process= self.launchExample ( filename, cwd )
    startup_time_multiplier = 1 if variation.global_insertion_string_failed else self.startup_time_multiplier
    for i in range( 0, startup_time_multiplier ):
      time.sleep(self.startup_time)
      if os.path.isfile( self.early_shutdown_file ) or not process.isRunning():
        break
    if not process.isRunning ():
      test_results.std_out= process.getStdOut()
      test_results.std_err= process.getStdErr()
      test_results.warnings, test_results.errors= self._countWarnings ( test_results )
      test_results.calculateFPS()
      return test_results
    
    if test_results.started_ok:
      im = None
      if os.path.isfile( self.screenshot_tmp_file ):
        im = Image.open( self.screenshot_tmp_file )
      else:
        im = self.getScreenShot()
      if im:
        test_results.screenshot= os.path.join(self.filepath,createFilename(orig_url, var_name)+".jpg")
        im.save(test_results.screenshot, "JPEG")
        im.thumbnail((256,256), Image.ANTIALIAS)
        test_results.screenshot_thumb= os.path.join(self.filepath,createFilename(orig_url, var_name)+"_thumb.jpg")
        im.save(test_results.screenshot_thumb, "JPEG")
    
      process.sendKey ( "{ESC}" )
      time.sleep(self.shutdown_time)
    
    if not process.isRunning ():
      test_results.terminated_ok= True
      test_results.std_out= process.getStdOut()
      test_results.std_err= process.getStdErr()
      test_results.warnings, test_results.errors= self._countWarnings ( test_results )
      test_results.calculateFPS()
      return test_results
    else:
      process.kill ()
      time.sleep(self.shutdown_time)
      test_results.std_out= process.getStdOut()
      test_results.std_err= process.getStdErr()
      test_results.warnings, test_results.errors= self._countWarnings ( test_results )
      test_results.calculateFPS()
      return test_results
  
  def _countWarnings ( self, test_results ):
    haystack= test_results.std_out + test_results.std_err
    haystack= haystack.lower()
    return ( haystack.count ( "warning" ), haystack.count ( "error" ) )
    
  
  def testAllExamples ( self, variations= None, directory= ".", fileExtensions= [".x3d"] ):
    try:
      os.makedirs(self.filepath)
    except:
      pass
    
    if variations:
      # modify variations list in order to add FPS pythonScript code.
      for v in variations:
        v.global_insertion_string = v.global_insertion_string + self.fps_insertion_string
    else:
      default_variation = Variation ( "Default", self.fps_insertion_string )
      variations = [default_variation]
 
    results= []
 
    for root, dirs, files in os.walk(directory):
      for file in files:
        base, ext= os.path.splitext(file)
        if ext.lower() in fileExtensions:
          file_path= os.path.join(root,file)
          if self._isTestable ( file_path ):
            print "Testing: " + file_path
            
            if variations:
              variation_results= []
              for v in variations:
                print "Variation: " + v.name
                success, variation_path= self._createVariation ( v, file_path )
                result= self.testExample ( file, variation_path, file_path, v.name, v )
                result.created_variation= success
                variation_results.append ( ( v, result ) )
                os.remove ( variation_path )
              results.append ( variation_results )
            else:
              results.append ( [( Variation(), self.testExample ( file, file_path ) )] )
            
            if self.error_reporter:
              self.error_reporter.reportResults( results )
          else:
            # Test skipped
            result= TestResults()
            result.name= file
            result.url= file_path
            result.skipped= True
            if variations:
              variation_results= []
              for v in variations:
                variation_results.append ( (v, result) )
              results.append ( variation_results )
            else:
              results.append ( [(Variation(),result)] )
              
    return results
  
  def _isTestable ( self, file_path ):
    
    if self.testable_callback:
      return self.testable_callback ( file_path )
    else:
      return True
  
  def _createVariation ( self, variation, file_path ):
    
    orig_file= open ( file_path, 'r' )
    
    variation_file= tempfile.NamedTemporaryFile ( delete= False )
    original_contents= orig_file.read()
    variation_contents= variation.parse ( original_contents )
    variation_file.write ( variation_contents )
    variation_file.close()
    
    return (variation_contents!=original_contents, variation_file.name)
  
class TestReport ( object ):
  
  def reportResults ( self, results ):
    output= ""
  
    for result in results:
      for variation in result:
        v= variation[0]
        r= variation[1]
        output+= "Test results for: " + str(r.name) + " (" + v.name + ")\n"
        output+= "\n"
        output+= "Started OK    : %s\n" % str(r.started_ok)
        output+= "Exited OK     : %s\n" % str(r.terminated_ok)
      
    return output
    
class TestReportHTML ( object ):
  
  def __init__ ( self, filepath, only_failed= False ):
    self.filepath= filepath
    self.only_failed= only_failed
  
  def reportResults ( self, results ):
  
    try:
      os.makedirs(self.filepath)
    except:
      pass
    
    f= open ( os.path.join(self.filepath,'default.css'), 'w' )
    f.write ( self._generateCSS() )
    f.close()
    
    f= open ( os.path.join(self.filepath,'index_failed.html' if self.only_failed else 'index.html'), 'w' )
  
    output= """
<html>
  <head>
    <title>Test results</title>
    <link rel="stylesheet" href="default.css" type="text/css" />
  </head>
  <body>
    <h1>Test results</h1>
    %s
    <table>
""" % ("<h2>Failed only</h2>" if self.only_failed else "")
  
    skipped_list= []
  
    for result in results:
      if result[0][1].skipped:
        skipped_list.append ( result[0][1] )
    
      if self._includeResult ( result ):
      
        output+= "<tr>"
        r= result[0][1]
        output+= "<td><h3>" + r.name + "</h3>\n"
        
        for variation in result:
          v= variation[0]
          r= variation[1]
      
          report_path= os.path.join (
            self.filepath,
            createFilename(r.url, v.name) + "_report.html" )
      
          output+= "<td>"
          output+= "<h3>" + v.name + ( "" if r.created_variation else " (Unchanged file)" ) + "</h3>"
          output+= self._getThumbnail ( r )
          output+= "<p>Start: " + self._getStatus ( r.started_ok ) + "; "
          output+= "Exit:" + self._getStatus ( r.terminated_ok ) + "</p>\n"
          output+= "<p>Errors: " + self._getErrorCount ( r.errors, 'fail' ) + "; "
          output+= "Warnings:" + self._getErrorCount ( r.warnings, 'warn' ) + "</p>\n"
          output+= "<p>Graphics frame rate: " + self._getFPS( r.fps_mean ) + "</p>\n"
          output+= "<p>[<a href='" + os.path.split(report_path)[1] + "'>details</a>]</p>"
          output+= "</td>"
          
          f1= open ( report_path, 'w' )
          f1.write ( self._reportExample ( r ) )
          f1.close()
          
        output+= "</tr>"
      
    output+= """
    </table>
    <h2>Skipped</h2>
    <ul>
    """
    if skipped_list:
      for r in skipped_list:
        output+= "<li>%s [%s]</li>" % (r.name, r.url)
    else:
      output+= "<li>None</li>"
      
    output+= """
    </ul>
  </body>
</html>"""
      
    f.write ( output )
    f.close()
      
    return output
    
  def _includeResult ( self, result ):
    for variation in result:
      if self._includeVariation ( variation[1] ):
        return True
    return False
    
  def _includeVariation ( self, variation ):
    return (variation.errors > 0 or variation.warnings > 0 or not variation.started_ok or not variation.terminated_ok or not self.only_failed) and not variation.skipped
    
  def _reportExample ( self, result ):
  
    output= """
<html>
  <head>
    <title>%s</title>
    <link rel="stylesheet" href="default.css" type="text/css" />
  </head>
  <body>
    <h1>%s</h1>
    <p>%s</p>
    %s
    <h2>Startup</h2>
    %s
    <h2>Shutdown</h2>
    %s
    <h2>Standard output</h2>
    <pre>%s</pre>
    <h2>Standard error</h2>
    <pre>%s</pre>
    <h2>Graphics frame rate</h2>
    <pre>Min: %s</pre>
    <pre>Max: %s</pre>
    <pre>Avg: %s</pre>
    <pre>Mean: %s</pre>
""" % (
      result.name,
      result.name, result.url,
      self._getThumbnail ( result ),
      self._getStatus ( result.started_ok ), 
      self._getStatus ( result.terminated_ok ),
      result.std_out,
      result.std_err,
      self._getFPS( result.fps_min ),
      self._getFPS( result.fps_max ),
      self._getFPS( result.fps_avg ),
      self._getFPS( result.fps_mean ))

    if result.variation and result.variation.name != "Default":
      output = output + \
               """  <h2>Variation settings:</h2>"""
      for i, option in enumerate( result.variation.options ):
        if option.field_name != "":
          output = output + \
                   "    <h3>Option %s:</h2>\n" % str(i) + \
                   "    <pre>Node: " + self._getFPS( ", ".join( option.node_type ) ) + "</pre>\n" + \
                   "    <pre>Field name: " + self._getFPS( option.field_name ) + "</pre>\n" + \
                   "    <pre>Field value: " + self._getFPS( option.field_value ) + "</pre>\n"

    output = output + \
"""  </body>
</html>"""
    return output
    
  def _generateCSS ( self ):
    return """
body {
  font-family: sans-serif;
}

.success {
  color: green;
}

.warn {
  color: orange;
}

.fail {
  color: red;
  font-weight: bold;
}

td, th {
  width: 260px;
}

"""
    
  def _getStatus ( self, status ):
    return "<span class='success'>OK</span>" if status else "<span class='fail'>FAIL</span>"
    
  def _getThumbnail ( self, result ):
    if result.screenshot != "":
      return "<p><a href='" + os.path.split(result.screenshot)[1] + "'><image src='" + os.path.split(result.screenshot_thumb)[1] + "' /></a></p>\n"
    else:
      return ""
      
  def _getErrorCount ( self, nrErrors, errorType ):
    return ("<span class='success'>%s</span>" if nrErrors==0 else "<span class='"+errorType+"'>%s</span>") % nrErrors
  
  def _getFPS ( self, fps ):
    return ( "<span class='success'>%s</span>" % fps ) if fps != "" else "<span class='fail'>-</span>"
        
def testH3DAPI (global_variations):

  def isTestable ( file_path ):
    return file_path.find ( 'TestAll' ) < 0 and file_path.find( 'SpaceTennis.x3d' ) < 0

  html_reporter_errors= TestReportHTML( os.path.join(args.output, "H3DAPI"), only_failed= True )

  tester= TestExamples( os.path.join(args.output, "H3DAPI"), startup_time= 10, shutdown_time= 10, testable_callback= isTestable, error_reporter=html_reporter_errors)
  results= tester.testAllExamples( global_variations, directory= "../../../H3DAPI/examples" )

  reporter= TestReport()
  print reporter.reportResults ( results )

  html_reporter= TestReportHTML( os.path.join(args.output, "H3DAPI") )
  html_reporter.reportResults ( results )
  
  html_reporter_errors.reportResults ( results )

  return getExitCode ( results )
  
def testH3DPhysics (global_variations):
      
  def isTestable ( file_path ):
    
    if file_path.find ( 'ParallelPhysicsEngines/x3d' ) >= 0 or file_path.find ( 'ParallelPhysicsEngines\\x3d' ) >= 0:
      return False
    f= open ( file_path, 'r' )
    contents= f.read ()
    f.close()
    return ( 
      contents.find ( 'RigidBodyCollection' ) > 0 or 
      contents.find ( 'PhysicsBodyCollection' ) > 0 )
  
  variations = list(global_variations)
  h3dphysics_header_string = ""
  header_file_name = "../../../H3DPhysics/include/H3D/H3DPhysics/H3DPhysics.h"
  if not os.path.isfile( header_file_name ):
    header_file_name = "/usr/local/include/H3D/H3DPhysics/H3DPhysics.h"
  if not os.path.isfile( header_file_name ):
    header_file_name = "/usr/include/H3D/H3DPhysics/H3DPhysics.h"
  if os.path.isfile( header_file_name ):
    h3dphysics_header_file = open( header_file_name, 'r' )
    h3dphysics_header_string = h3dphysics_header_file.read()
    h3dphysics_header_file.close()
  
  if h3dphysics_header_string == "" or h3dphysics_header_string.find( "define HAVE_ODE" ) != -1:
    ode_variation= Variation ( "ODE" )
    ode_variation.options.append ( Option ( ["RigidBodyCollection"], "physicsEngine", "ODE" ) )
    variations.append( ode_variation )

  if h3dphysics_header_string == "" or h3dphysics_header_string.find( "define HAVE_BULLET" ) != -1:
    bullet_variation= Variation ( "Bullet" )
    bullet_variation.options.append ( Option ( ["RigidBodyCollection","PhysicsBodyCollection"], "physicsEngine", "Bullet" ) )
    variations.append( bullet_variation )

  if h3dphysics_header_string == "" or h3dphysics_header_string.find( "define HAVE_PHYSX" ) != -1:
    physx_variation= Variation ( "PhysX" )
    physx_variation.options.append ( Option ( ["RigidBodyCollection","PhysicsBodyCollection"], "physicsEngine", "PhysX" ) )
    variations.append( physx_variation )

  if h3dphysics_header_string == "" or h3dphysics_header_string.find( "define HAVE_PHYSX3" ) != -1:
    physx3_variation= Variation ( "PhysX3" )
    physx3_variation.options.append ( Option ( ["RigidBodyCollection"], "physicsEngine", "PhysX3" ) )
    variations.append( physx3_variation )

  if h3dphysics_header_string == "" or h3dphysics_header_string.find( "define HAVE_SOFA" ) != -1:
    sofa_variation= Variation ( "SOFA" )
    sofa_variation.options.append ( Option ( ["RigidBodyCollection","PhysicsBodyCollection"], "physicsEngine", "SOFA" ) )
    variations.append( sofa_variation )

  tester= TestExamples( os.path.join(args.output, "H3DPhysics"), startup_time= 10, shutdown_time= 10, testable_callback= isTestable )
  results= tester.testAllExamples( variations, directory= "../../../H3DPhysics/examples" )

  reporter= TestReport()
  print reporter.reportResults ( results )

  html_reporter= TestReportHTML( os.path.join(args.output, "H3DPhysics") )
  html_reporter.reportResults ( results )

  html_reporter_errors= TestReportHTML( os.path.join(args.output, "H3DPhysics"), only_failed= True )
  html_reporter_errors.reportResults ( results )

  return getExitCode ( results )

def testMedX3D (global_variations):

  tester= TestExamples( os.path.join(args.output, "MedX3D"), startup_time= 10, shutdown_time= 10 )
  results= tester.testAllExamples( global_variations, directory= "../../../MedX3D/x3d" )

  reporter= TestReport()
  print reporter.reportResults ( results )

  html_reporter= TestReportHTML( os.path.join(args.output, "MedX3D") )
  html_reporter.reportResults ( results )

  html_reporter_errors= TestReportHTML( os.path.join(args.output, "MedX3D"), only_failed= True )
  html_reporter_errors.reportResults ( results )

  return getExitCode ( results )
  
def testUI (global_variations):

  tester= TestExamples( os.path.join(args.output, "UI"), startup_time= 10, shutdown_time= 10 )
  results= tester.testAllExamples( global_variations, directory= "../../../UI/x3d" )

  reporter= TestReport()
  print reporter.reportResults ( results )

  html_reporter= TestReportHTML( os.path.join(args.output, "UI") )
  html_reporter.reportResults ( results )

  html_reporter_errors= TestReportHTML( os.path.join(args.output, "UI"), only_failed= True )
  html_reporter_errors.reportResults ( results )

  return getExitCode ( results )

print ""
print "WARNING: Do not change the window focus, or perform other input until the test is complete!"
print ""
time.sleep(3)

def globalCombinations( fields_to_edit, the_list ):
  if len( the_list ) == 0:
    for i in range( 2, len(fields_to_edit[0]) ):
      the_list.append( [i] )
  else:
    if len(fields_to_edit[0]) > 3:
      the_list_length = len( the_list )
      for i in range( 0, len(fields_to_edit[0]) - 3 ):
        for j in range( 0, the_list_length ):
          the_list.append( list( the_list[j] ) )
    for i, j in enumerate( the_list ):
      j.append( i / ( len(fields_to_edit[0]) - 2 ) + 2 )
  if len( fields_to_edit ) > 1:
    the_list = globalCombinations( fields_to_edit[1:len(fields_to_edit)], the_list )

# Setting up variations that should be tested for.
fields_to_edit = [["GraphicsOptions", "preferVertexBufferObject", "true", "false" ], [ "GraphicsOptions", "useCaching", "true", "false"] ]
variation_combinations = []
globalCombinations( fields_to_edit, variation_combinations )
global_settings_variations = []
for c, combo in enumerate( variation_combinations ):
  variation = Variation( "Variation " + str( c ) )
  for i, j in enumerate( combo ):
    variation.options.append ( Option ( ["GlobalSettings"], "", "", "Scene", True ) )
    variation.options.append ( Option ( [fields_to_edit[i][0]], fields_to_edit[i][1], fields_to_edit[i][j], "GlobalSettings", True ) )
  global_settings_variations.append( variation )

print_shader_warnings_variation = Variation( "Variation shader warning" )
print_shader_warnings_variation.options.append ( Option ( ["GlobalSettings"], "", "", "Scene", True ) )
print_shader_warnings_variation.options.append ( Option ( ["DebugOptions"], "printShaderWarnings", "true", "GlobalSettings", True ) )
global_settings_variations.append( print_shader_warnings_variation )

exitCode= 0
c= testH3DAPI (global_settings_variations)
if c != 0:
  exitCode= c
  
c= testH3DPhysics (global_settings_variations)
if c != 0:
  exitCode= c

c= testMedX3D (global_settings_variations)
if c != 0:
  exitCode= c
  
c= testUI (global_settings_variations)
if c != 0:
  exitCode= c
