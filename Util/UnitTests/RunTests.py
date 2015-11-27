"""
This script is supposed to run unit tests on every H3DAPI node. 

Requirements:
  You must install Imagemagick (and add to PATH) in order for image comparison to function, otherwise screenshot tests will always fail.
  You must install the google app-engine python library as well as gspread.

"""

import os, sys
import subprocess
import time
import platform
from Queue import Queue, Empty
from threading  import Thread
import tempfile
import string
import re
import argparse
import math
import signal
import glob
import Image
import ConfigParser
import json
import gspread
from collections import namedtuple
#from PIL import Image

from oauth2client.client import GoogleCredentials

from ProcessWrapper import *

parser = argparse.ArgumentParser(
  description='Runs python tests')
parser.add_argument('--workingdir', dest='workingdir', 
                    default=os.getcwd(),
                    help='The directory containing the unit tests.')
parser.add_argument('--output', dest='output',
                   default='output',
                   help='The directory to output the results to.')
parser.add_argument('--only_validate', dest='only_validate', action='store_true',
                    default=False,
                    help='Does not run the test cases, only goes through the output directory and compares the generated output. Will not report start/termination results. Can not be combined with only_generate.')
parser.add_argument('--only_generate', dest='only_generate', action='store_true',
                    default=False,
                    help='does not run validation on the generated output. Will not report screenshot or performance comparison results. Can not be combined with only_validate.')                    
args = parser.parse_args()


def createFilename(orig_url, var_name):
  """ Returns a valid file name based on the provided path and variable name. """
  valid_chars = "-_.() %s%s" % (string.ascii_letters, string.digits)
  filename= orig_url + var_name
  return ''.join(c for c in filename if c in valid_chars).strip ( '.' )


def isError ( result ):
  """ Returns true if one of the possible errors occurred. """
  return (result.errors > 0 or not result.started_ok or not result.terminated_ok) and not result.skipped


def getExitCode ( results ):
  """ Returns an exit code based on the test results. """
  for result in results:
    for variation in result:
      v= variation[0]
      r= variation[1]
      if isError ( r ):
        return 1
  return 0
  

class Option ( object ): 
  """ Specifies the value of a field that is identified by its name and its container node. """
  def __init__ ( self, node_type, field_name, field_value, container_node = "Scene", add_if_missing = False ):
    self.node_type= node_type
    self.field_name= field_name
    self.field_value= field_value
    self.container_node = container_node
    self.add_if_missing = add_if_missing

    
class Variation ( object ):
  """ Specifies a set of Options to run a test with and provides functionality for inserting those into a given x3d string. """

  def __init__ ( self, name= "Default", insertion_string = "" ):
    self.name= name
    self.options= []
    self.global_insertion_string = insertion_string
    self.global_insertion_string_failed = False
    
  def parse ( self, input ):
    """ Parses the input x3d string and returns a new x3d string where this Variation's Options have been inserted into the appropriate nodes. """
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
  # Container class for all kinds of possible test results
  def __init__ ( self, fps_data_file = "" ):
    self.filename= ""
    self.case_name= ""
    self.test_type= ""
    self.url= ""
    self.std_out= ""
    self.std_err= ""
    self.started_ok= False
    self.terminated_ok= False
    self.screenshots_ok = []
    self.screenshots= []
    self.screenshot_diffs= []
    self.screenshot_thumbs= []
    self.step_names= []
    self.warnings= 0
    self.errors= 0
    self.skipped= False
    self.created_variation= False
    self.has_fps = False
    self.fps_min = ""
    self.fps_max = ""
    self.fps_mean = ""
    self.fps_avg = ""
    self.fps_data_file = fps_data_file
    self.perf_file_path = ""
    if not args.only_validate:
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
        self.has_fps = True
        fps_list_float.sort()
        self.fps_min = "%.2f" % fps_list_float[0]
        self.fps_max = "%.2f" % fps_list_float[len( fps_list_float ) - 1]
        self.fps_avg = "%.2f" % (math.fsum( fps_list_float ) / float( len( fps_list_float ) ) )
        self.fps_mean = "%.2f" % float( fps_list_float[len( fps_list_float ) / 2] )
      fps_filename = os.path.split(self.fps_data_file)[1]
      self.fps_datetime = time.strftime("%Y-%m-%d %H:%M:%S", time.localtime())
      self.perf_file_path = os.path.join(os.path.split(self.fps_data_file)[0], os.path.splitext(fps_filename)[0] + time.strftime("_%Y-%m-%d %H-%M-%S", time.localtime()))
      perf_file = open(self.perf_file_path, 'w')
      perf_file.write("\n".join( [self.fps_datetime, self.fps_min, self.fps_max, self.fps_avg, self.fps_mean]))
      perf_file.close()

      # Feed into a google docs spreadsheet: https://docs.google.com/spreadsheets/d/16FI0nGxOf5IKpN-2VtniEg3EQCBQ9VlppfHHt7DZ7Jc/
      #c = gspread.Client(auth=('sensegraphics.bot@gmail.com', 'Gxwb69mJMzvv6pvY4QYL'))
      #c.login()
      # new way to login as the old way are not supported anymore
      try:
        credentials = GoogleCredentials.get_application_default()
        credentials = credentials.create_scoped(['https://spreadsheets.google.com/feeds'])
        c = gspread.authorize(credentials)
        doc= '16FI0nGxOf5IKpN-2VtniEg3EQCBQ9VlppfHHt7DZ7Jc'
        spreadsheet=c.open_by_key( doc )
        sheet_name = os.path.splitext(self.filename)[0] + '_' + self.case_name
      except:
        pass

      try:
        worksheet = spreadsheet.worksheet (sheet_name)
      except gspread.exceptions.WorksheetNotFound:
        worksheet= spreadsheet.add_worksheet(sheet_name, 1, 1)
        worksheet.append_row ( ["Date/Time", "FPS min", "FPS max", "FPS avg", "FPS mean"] )
      worksheet.append_row([self.fps_datetime, self.fps_min, self.fps_max, self.fps_avg, self.fps_mean])

      
    
class TestCaseRunner ( object ):

  def __init__ ( self, filepath, startup_time= 10, shutdown_time= 5, testable_callback= None, fake_fullscreen_due_to_PIL_bug = True, error_reporter = None ):
    self.filepath= filepath
    self.startup_time= startup_time
    self.shutdown_time= shutdown_time
    self.testable_callback= testable_callback
    self.fps_data_file = '%s/fps.txt' % (os.getcwd())
    self.early_shutdown_file = '%s/test_complete' % (os.getcwd())
    self.screenshot_tmp_file = '%s/tmp_screenshot.png' % (os.getcwd())
    self.startup_time_multiplier = 10
    self.load_flags = []#"--no-fullscreen","--screen=800x600"]
    # Used to get intermediate failed results in order to be able to cancel test early and still get some results.
    self.error_reporter = error_reporter

  def launchTest ( self, url, cwd ):
    process = self.getProcess()
    process.launch ( ["H3DLoad.exe" if platform.system() == 'Windows' else "H3DLoad"] + self.load_flags + [url], cwd )
    return process

  def getProcess( self ):
    platform_system = platform.system()
    if platform_system == 'Windows':
      return ProcessWin32()
    elif platform_system == 'Linux':
      return ProcessUnix()

  def testStartUp ( self, url, cwd, variation ):
    """ Returns true if the example can be started. """
    
    process = self.getProcess()
    return process.testLaunch ( ["H3DLoad.exe" if platform.system() == 'Windows' else "H3DLoad"] + self.load_flags + [url], cwd, self.startup_time, self.shutdown_time, 1 if variation and variation.global_insertion_string_failed else self.startup_time_multiplier, self.early_shutdown_file )
    
  def runTestCase (self, filename, test_case, url, orig_url= None, var_name= "", variation = None ):

    if orig_url is None:
      orig_url= url
    
    test_results= TestResults( self.fps_data_file )
    test_results.filename= filename
    test_results.name= test_case.name
    test_results.test_type= test_case.type
    test_results.url= orig_url
    test_results.variation = variation
    
    cwd= os.path.split ( orig_url )[0]
    filename= os.path.abspath ( url )
    
    test_results.started_ok= self.testStartUp ( url, cwd, variation )
    
    if os.path.isfile( self.early_shutdown_file ):
      os.remove( self.early_shutdown_file )
    process= self.launchTest ( url, cwd )
    startup_time_multiplier = self.startup_time_multiplier
    for i in range( 0, startup_time_multiplier ):
      time.sleep(self.startup_time)
      if os.path.isfile( self.early_shutdown_file ) or not process.isRunning():
        break
    if not process.isRunning ():
      test_results.std_out= process.getStdOut()
      test_results.std_err= process.getStdErr()
      test_results.warnings, test_results.errors= self._countWarnings ( test_results )
      return test_results
    
    process.sendKey ( "{ESC}" )

    time_slept = 0.0
    while time_slept < self.shutdown_time and process.isRunning():
      time.sleep(0.5)
      time_slept += 0.5
    
    if not process.isRunning ():
      test_results.terminated_ok= True
      test_results.std_out= process.getStdOut()
      test_results.std_err= process.getStdErr()
      test_results.warnings, test_results.errors= self._countWarnings ( test_results )
      return test_results
    else:
      process.kill ()
      time_slept = 0
      while time_slept < self.shutdown_time and process.isRunning():
        time.sleep(0.5)
        time_slept += 0.5
      test_results.std_out= process.getStdOut()
      test_results.std_err= process.getStdErr()
      test_results.warnings, test_results.errors= self._countWarnings ( test_results )
      return test_results
  
  def _countWarnings ( self, test_results ):
    haystack= test_results.std_out + test_results.std_err
    haystack= haystack.lower()
    return ( haystack.count ( "warning" ), haystack.count ( "error" ) )
    
  def parseTestDefinitionFile(self, file_path):
    """
    Parses the specified test definition file and returns a list of namedtuples containing the following:      
      name: the name of this test case
      x3d: x3d file path
      type: test type
      baseline: baseline folder
      script: an optional test script
      All of these values default to None
    The list will contain one namedtuple for each Section in the specified definition file
    """
    confParser = ConfigParser.RawConfigParser(defaults={'x3d':None, 'type':None, 'baseline':None, 'script':None, 'runtime':'0'}, allow_no_value=True)
    try:
      confParser.read(file_path)
    except:
      print sys.exc_info()[0]
      return None
    result = []
    for sect in confParser.sections():
      test_case = namedtuple('TestDefinition', ['name','x3d', 'type', 'baseline', 'script', 'runtime']) 
      test_case.name = sect
      test_case.x3d = confParser.get(sect, 'x3d')
      test_case.type = confParser.get(sect, 'type')    
      test_case.baseline = confParser.get(sect, 'baseline')
      test_case.script = confParser.get(sect, 'script')
      test_case.runtime = confParser.get(sect, 'runtime')
      result.append(test_case)
    return result
  
  def validate_screenshot(self, screenshot_path='.', baseline_path='.', diff_path='.'):
    """ Compare screenshots """

    ret= { 'success': True, 'message': '', 'diff_file': ''}
    # Comparison thresholds
    fuzz= 3               # % of fuzz (increase to ignore small differences)
    error_threshold= 5    # Number of pixels that must differ in order to trigger a warning

    g= glob.glob(baseline_path)
    if g:
      baselinescreenshot= max(g, key= os.path.getmtime)
      #diff_name= "difference_%s_%s_r%s_%s.png" % (caseName, clientName, results['revision'], ret['time'].strftime('%b_%d_%Y_%H_%M_%S'))
      try:
        process= subprocess.Popen(
          ["compare",
           "-fuzz","%d%%"%fuzz,
           "-metric","AE",
           baseline_path,screenshot_path,diff_path], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
      except:
        print "WARNING: Image comparison not available! Probably Imagemagick is not installed, or not in the path."
        process= None

      if process:
        (output, err) = process.communicate()
        exit_code = process.wait()

        metric= None
        try:
          metric= int(err)
        except:
          ret['success']= False
          ret['message']+= (
            'WARNING: Image comparison failed!\n\nDetails:\n%s\n%s' % (screenshot_path, err) )

        if not metric is None and metric > error_threshold:
          ret['success']= False
          ret['message']+= (
            'WARNING: The screenshot ' +
            'is significantly different to the baseline. The differences are attached as ' + diff_path + '.\n' )
          ret['diff_file'] = ( os.path.abspath(diff_path) )

        else:
          # Test passed, we can clean up the file right now
          try:
            os.remove ( diff_path )
          except:
            print "WARNING: Could not remove " + diff_path

    else:
      print "WARNING: No screenshot baseline, skipping validation!"
    return ret
    if testCase.script != None:
      testCaseFile= open (os.path.join(directory, testCase.script), 'r')
      testCaseScript = testCaseFile.read()
    else:
      testCaseScript = ""
                
    script = "<PythonScript ><![CDATA[python:" + (boilerplateScript % (self.early_shutdown_file, os.path.join(screenshot_dir, testCase.name+"_"))) + '\n' + testCaseScript + "]]></PythonScript>"
                              
    variation_results= []
    v = Variation (testCase.name, script)
    if not args.only_validate:
      # Create a temporary x3d file containing our injected script
      success, variation_path= self._createVariationFile ( v, os.path.join(directory, testCase.x3d))
      # Run that H3DViewer with that x3d file
      result = self.runTestCase (file, testCase, variation_path, os.path.join(directory, testCase.x3d), v.name, v)
      result.created_variation= success
    else:
      result = TestResults('')
      result.filename= file
      result.name= testCase.name
      result.test_type= testCase.type
      result.url= os.path.join(directory, testCase.x3d)
      result.created_variation= True


  def processScreenshotTestCase(self, file, testCase, results, directory, output_dir, screenshot_dir, diff_dir, report_dir, boilerplateScript):
    """
        
    """
    if testCase.script != None:
      testCaseFile= open (os.path.join(directory, testCase.script), 'r')
      testCaseScript = testCaseFile.read()
    else:
      testCaseScript = ""
                
    script = "<PythonScript ><![CDATA[python:" + (boilerplateScript % (self.early_shutdown_file, os.path.join(screenshot_dir, os.path.splitext(file)[0]+"_"+testCase.name+"_"))) + '\n' + testCaseScript + "]]></PythonScript>"
                              
    variation_results= []
    v = Variation (testCase.name, script)
    if not args.only_validate:
      # Create a temporary x3d file containing our injected script
      success, variation_path= self._createVariationFile ( v, os.path.join(directory, testCase.x3d))
      # Run that H3DViewer with that x3d file
      result = self.runTestCase (file, testCase, variation_path, os.path.join(directory, testCase.x3d), v.name, v)
      result.created_variation= success
    else:
      result = TestResults('')
      result.filename= file
      result.name= testCase.name
      result.test_type= testCase.type
      result.url= os.path.join(directory, testCase.x3d)
      result.created_variation= True

    result.has_baseline= (testCase.baseline != None) and os.path.exists(os.path.join(output_dir, testCase.baseline))

    for screenshot_file in os.listdir(screenshot_dir):
      screenshot_base, screenshot_ext = os.path.splitext(screenshot_file)
      if screenshot_base.startswith(os.path.splitext(file)[0]+"_"+testCase.name+"_") and not screenshot_base.endswith('_thumb'):
        if screenshot_ext.lower() == '.png':
          diff_path = os.path.join(diff_dir, "diff_") + screenshot_file
          baseline_path = os.path.join(testCase.baseline, screenshot_file)
          if not args.only_generate:
            comp_result = self.validate_screenshot(os.path.join(screenshot_dir, screenshot_file), baseline_path, diff_path)
            result.screenshots_ok.append(comp_result['success'] and result.screenshots_ok)
          else:
            result.screenshots_ok.append(os.path.exists(diff_path))
          result.step_names.append(screenshot_base)
          result.screenshots.append(os.path.join(screenshot_dir, screenshot_file)) 
          if not args.only_generate and comp_result['diff_file'] != '':
            result.screenshot_diffs.append(os.path.join(diff_path, comp_result['diff_file']))
          else:
            result.screenshot_diffs.append('')
                
    if not args.only_validate:
      os.remove ( variation_path )
    variation_results.append ( ( v, result ) )
              
                      

    results.append ( variation_results )
            
    if self.error_reporter:
      self.error_reporter.reportResults( results )


  def processPerformanceTestCase(self, file, testCase, results, directory, output_dir, screenshot_dir, perf_dir, report_dir, boilerplateScript):
    """
        
    """
    if testCase.script != None:
      testCaseFile= open (os.path.join(directory, testCase.script), 'r')
      testCaseScript = testCaseFile.read()
    else:
      testCaseScript = ""
                
    screenshot_path = os.path.join(screenshot_dir, os.path.splitext(file)[0]+'_'+testCase.name+'.png')
    self.fps_data_file = os.path.join(perf_dir, os.path.splitext(file)[0]+'_'+testCase.name+'_perf.txt')
    script = "<PythonScript ><![CDATA[python:" + (boilerplateScript % (self.fps_data_file.replace('\\', '/'), self.early_shutdown_file.replace('\\', '/'), testCase.runtime.replace('\\', '/'), screenshot_path.replace('\\', '/'))) + '\n' + testCaseScript + "]]></PythonScript>"


    variation_results= []
    v = Variation (testCase.name, script)
    if not args.only_validate:
      # Create a temporary x3d file containing our injected script
      success, variation_path= self._createVariationFile ( v, os.path.join(directory, testCase.x3d))
      # Run that H3DViewer with that x3d file
      result = self.runTestCase (file, testCase, variation_path, os.path.join(directory, testCase.x3d), v.name, v)
      result.screenshots.append(screenshot_path)
      result.created_variation= success
      result.calculateFPS()
      #resultfile = os.open(output_dir,   json.dumps(result)
    else:
      result = TestResults(self.fps_data_file)
      result.filename= file
      result.name= testCase.name
      result.test_type= testCase.type
      result.url= os.path.join(directory, testCase.x3d)
      result.created_variation= True
      result.calculateFPS()
      result.screenshots.append(screenshot_path)
      
    results.append([(v, result)])

  def processAllTestDefinitions ( self, directory= ".", output_dir= ".", fileExtensions= [".testcase"] ):
    """
        
    """
    try:
      os.makedirs(self.filepath)
    except:
      pass
     
    results = []

    screenshotBoilerplateScriptFile = open(os.path.join(os.getcwd(), 'UnitTestScreenshotBoilerplate.py'), 'r')
    screenshotBoilerplateScript = screenshotBoilerplateScriptFile.read()

    performanceBoilerplateScriptFile = open(os.path.join(os.getcwd(), 'UnitTestPerformanceBoilerplate.py'), 'r')
    performanceBoilerplateScript = performanceBoilerplateScriptFile.read()
    
    
    screenshot_dir = os.path.join(output_dir, 'screenshots')
    diff_dir = os.path.join(output_dir, 'diffs')
    perf_dir = os.path.join(directory, output_dir, 'perf')
    report_dir = os.path.join(output_dir, 'reports')
    for dir in [output_dir, screenshot_dir, diff_dir, perf_dir, report_dir]:
      if not os.path.exists(dir):
        os.mkdir(dir)


    for root, dirs, files in os.walk(directory):
      for file in files:
        base, ext= os.path.splitext(file)
        if ext.lower() in fileExtensions:
          file_path= os.path.join(root,file)
          print "Checking " + file_path + " for tests"
          testCases = self.parseTestDefinitionFile(file_path)
          for testCase in testCases:
            if testCase != None and testCase.x3d != None and testCase.type != None:
              print "Testing: " + testCase.name
              if testCase.type == 'screenshot':
                result = self.processScreenshotTestCase(file, testCase, results, root, output_dir, screenshot_dir, diff_dir, report_dir, screenshotBoilerplateScript)
              elif testCase.type == 'performance':
                result = self.processPerformanceTestCase(file, testCase, results, root, output_dir, screenshot_dir, perf_dir, report_dir, performanceBoilerplateScript)              
            else:
              # Test skipped
              result= TestResults()
              result.name= file
              result.url= os.path.join(file)
              result.skipped= True
              results.append ( [(Variation(),result)] )
              
    return results
  
  def _isTestable ( self, file_path ):
    
    if self.testable_callback:
      return self.testable_callback ( file_path )
    else:
      return True
  
  def _createVariationFile ( self, variation, file_path ):
    
    orig_file= open ( file_path, 'r' )
    
    variation_file= tempfile.NamedTemporaryFile(suffix='.x3d', delete= False )
    original_contents= orig_file.read()
    variation_contents= variation.parse ( original_contents )
    variation_file.write ( variation_contents )
    variation_file.close()
    
    return (variation_contents!=original_contents, variation_file.name)
  
class TestReport ( object ):
  
  def reportResults ( self, results ):
    output= ""
    if args.only_generate:
      return output
    all_screenshots_succeeded = True

    for result in results:
      for variation in result:
        v= variation[0]
        r= variation[1]
        output+= "Test results for: " + str(r.name) + " (" + v.name + ")\n"
        output+= "\n"
        if not args.only_validate:
          output+= "Started OK    : %s\n" % str(r.started_ok)
          output+= "Exited OK     : %s\n" % str(r.terminated_ok)
        if r.test_type == 'screenshot':
          output+= "Screenshots:\n"
          for i in range(0, len(r.screenshots)):
            output+= "  %s [%s]\n" % (r.step_names[i], ("OK" if r.screenshots_ok[i] else "Failed"))
          if False in r.screenshots_ok:
            all_screenshots_succeeded = False

    if r.test_type == 'screenshot':
      output+= "\n%s\n" % ("All screenshot comparisons OK!" if all_screenshots_succeeded else "One or more screenshot comparisons Failed!")
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
            v.name + "_report.html" )
      
          output+= "<td>"
          output+= "<h3>" + v.name + ( "" if r.created_variation else " (Unchanged file)" ) + "</h3>"
          if r.test_type == 'screenshot':
            screenshot = self._getFirstErrorScreenshot( r )
            if screenshot == "":
              screenshot = self._getScreenshot( r, count=1 )
          else:
            screenshot = self._getScreenshot( r, count=1 )

          output+= screenshot
          if not args.only_validate:
            output+= "<p>Start: " + self._getStatus ( r.started_ok ) + "; "
            output+= "Exit:" + self._getStatus ( r.terminated_ok ) + "; "
          if not args.only_generate:
            output+= "Screenshots: " + self._getScreenshotStatus( r.screenshots_ok ) + "</p>\n"
          output+= "<p>Errors: " + self._getErrorCount ( r.errors, 'fail' ) + "; "
          output+= "Warnings:" + self._getErrorCount ( r.warnings, 'warn' ) + "</p>\n"
          if not args.only_generate and r.has_fps:
            output+= "<p>Graphics frame rate: " + self._getFPS( r.fps_mean ) + "</p>\n"
          output+= "<p>[<a href='" + os.path.split(report_path)[1] + "'>details</a>]</p>"
          output+= "</td>"
          
          f1= open ( report_path, 'w' )
          f1.write ( self._reportTestcase ( r ) )
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

  def _getThumbnail(self, screenshot_path, thumb_dir):
    if not os.path.exists(screenshot_path):
      return ""
    else:
      im = Image.open(screenshot_path)
      im.thumbnail((256,256), Image.ANTIALIAS)
      screenshot_filename = os.path.split(screenshot_path)[1]
      thumb_path = os.path.join(thumb_dir, os.path.splitext(screenshot_filename)[0] + "_thumb.png")
      im.save(thumb_path, "PNG")
      return thumb_path
    
  def _reportTestcase ( self, result ):   

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
    <pre>%s</pre>""" % (
      result.name,
      result.name, result.url,
      self._getScreenshot ( result, count = (1 if result.test_type == 'performance' else 0)),
      self._getStatus ( result.started_ok ), 
      self._getStatus ( result.terminated_ok ),
      result.std_out,
      result.std_err)
    if not args.only_generate and result.has_fps:
      """<h2>Graphics frame rate</h2>
      <pre>Min: %s</pre>
      <pre>Max: %s</pre>
      <pre>Avg: %s</pre>
      <pre>Mean: %s</pre>""" % (self._getFPS( result.fps_min ),
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
  width: 280px;
}

img {
  float: left;
}

"""
    
  def _getStatus ( self, status ):
    return "<span class='success'>OK</span>" if status else "<span class='fail'>FAIL</span>"
  
  def _getScreenshotStatus ( self, status_list):
    return "<span class='success'>OK</span>" if not False in status_list else "<span class='fail'>FAIL</span>"
    
  def _getScreenshot ( self, result, count = 0 ):
    """
    Iterates through the screenshots from a testcase result and returns HTML code for them.
    If count is not > 0 then it will only get that many screenshots. Otherwise it will get all available screenshots.
    """
    res = "<p>"
    for i in range(0, len(result.screenshots)):
      if result.test_type == 'screenshot' and result.screenshot_diffs[i] != "":
        img_path = result.screenshot_diffs[i]
      else:
        img_path = result.screenshots[i]
      thumb_path = self._getThumbnail(img_path, self.filepath)
      res += "<a href='" + os.path.relpath(img_path, self.filepath) + "'><image src='" + os.path.relpath(thumb_path, self.filepath) + "' /></a>"
      count -= 1
      if count == 0:
        break
    res += "</p>\n"
    return res

  def _getFirstErrorScreenshot( self, result ):
    """ Iterates through all the test steps of a screenshot test and returns HTML code for the first one that failed and has a diff image. """
    for i in range(0, len(result.screenshots_ok)):
      if not result.screenshots_ok[i] and result.screenshot_diffs[i] != '':
        img_path = result.screenshot_diffs[i]
        thumb_path = self._getThumbnail(img_path, self.filepath)
        return "<p><a href='" + os.path.relpath(img_path, self.filepath) + "'><image src='" + os.path.relpath(thumb_path, self.filepath) + "' /></a></p>\n"
    return ""
      
  def _getErrorCount ( self, nrErrors, errorType ):
    return ("<span class='success'>%s</span>" if nrErrors==0 else "<span class='"+errorType+"'>%s</span>") % nrErrors
  
  def _getFPS ( self, fps ):
    return ( "<span class='success'>%s</span>" % fps ) if fps != "" else "<span class='fail'>-</span>"
        

 

print ""
print "WARNING: Do not change the window focus, or perform other input until the test is complete!"
print ""
time.sleep(3)

exitCode= 0

def isTestable ( file_name , files_in_dir):
  return True

#html_reporter_errors= TestReportHTML( os.path.join(args.output, "reports"), only_failed= True )

tester= TestCaseRunner( os.path.join(args.workingdir, ""), startup_time= 5, shutdown_time= 5, testable_callback= isTestable, error_reporter=None)
results = tester.processAllTestDefinitions(directory=args.workingdir, output_dir=args.output)

reporter= TestReport()
print reporter.reportResults ( results )

html_reporter= TestReportHTML( os.path.join(args.output, "reports") )
html_reporter.reportResults ( results )
  
#html_reporter_errors.reportResults ( results )

c = getExitCode ( results )

if c != 0:
  exitCode= c
  