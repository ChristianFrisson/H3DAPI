"""
This script is supposed to run unit tests on every H3DAPI node. 

NOTE: You must install Imagemagick (and add to PATH) in order for image comparison to function, otherwise screenshot tests will always fail

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
from collections import namedtuple
#from PIL import Image
from ProcessWrapper import *

parser = argparse.ArgumentParser(
  description='Runs python tests')
parser.add_argument('--workingdir', dest='workingdir',
                    default=os.getcwd(),
                    help='the directory containing the unit tests')
parser.add_argument('--output', dest='output',
                   default='reports',
                   help='the directory to output the results to')
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
    self.name= ""
    self.test_type= ""
    self.url= ""
    self.std_out= ""
    self.std_err= ""
    self.started_ok= False
    self.terminated_ok= False
    self.screenshots_ok= True
    self.screenshots= []
    self.screenshot_diffs= []
    self.screenshot_thumbs= []
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
    self.load_flags = ["--no-fullscreen","--screen=800x600"]
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
    test_results.type= test_case.type
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
      test_results.calculateFPS()
      return test_results
    
    if test_results.started_ok:   
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
      test_results.calculateFPS()
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
      test_results.calculateFPS()
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
    confParser = ConfigParser.RawConfigParser(defaults={'x3d':None, 'type':None, 'baseline':None, 'script':None}, allow_no_value=True)
    try:
      confParser.read(file_path)
    except:
      print sys.exc_info()[0]
      return None
    result = []
    for sect in confParser.sections():
      test_case = namedtuple('TestDefinition', ['name','x3d', 'type', 'baseline', 'script']) 
      test_case.name = sect
      test_case.x3d = confParser.get(sect, 'x3d')
      test_case.type = confParser.get(sect, 'type')    
      test_case.baseline = confParser.get(sect, 'baseline')
      test_case.script = confParser.get(sect, 'script')
      result.append(test_case)
    return result
  
  def validate_screenshot(self, output_path='.', baseline_path='.', diff_name='.'):
    """ Compare screenshots """

    ret= { 'success': True, 'message': '', 'diff_file': '', 'thumb': ''}
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
           baseline_path,output_path,diff_name], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
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
            'WARNING: Image comparison failed!\n\nDetails:\n%s\n%s' % (output, err) )

        if not metric is None and metric > error_threshold:
          ret['success']= False
          ret['message']+= (
            'WARNING: The screenshot ' +
            'is significantly different to the baseline. The differences are attached as ' + diff_name + '.\n' )
          ret['diff_file'] = ( os.path.abspath(diff_name) )
          im = Image.open(diff_name)
          if im:
            im.thumbnail((256,256), Image.ANTIALIAS)
            ret['thumb'] = os.path.join(os.path.split(diff_name)[0], os.path.split(diff_name)[0], os.path.splitext(os.path.split(diff_name)[1])[0]+"_thumb.png")
            im.save(ret['thumb'], "PNG")
        else:
          # Test passed, we can clean up the file right now
          im = Image.open(output_path)
          if im:
            im.thumbnail((256,256), Image.ANTIALIAS)
            ret['thumb'] = os.path.join(os.path.split(output_path)[0], os.path.split(output_path)[0], os.path.splitext(os.path.split(output_path)[1])[0]+"_thumb.png")
            im.save(ret['thumb'], "PNG")
          try:
            os.remove ( diff_name )
          except:
            print "WARNING: Could not remove " + diff_name

    else:
      print "WARNING: No screenshot baseline, skipping validation!"
      im = Image.open(output_path)
      if im:
        im.thumbnail((256,256), Image.ANTIALIAS)
        ret['thumb'] = os.path.join(os.path.split(output_path)[0], os.path.split(output_path)[0], os.path.splitext(os.path.split(output_path)[1])[0]+"_thumb.png")
        im.save(ret['thumb'], "PNG")
    return ret

  def processAllTestDefinitions ( self, directory= ".", fileExtensions= [".testcase"] ):
    try:
      os.makedirs(self.filepath)
    except:
      pass
     
    results = []

    boilerplateScriptFile = open(os.path.join(os.getcwd(), 'UnitTestBoilerplate.py'), 'r')
    boilerplateScript = boilerplateScriptFile.read()

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
              print testCase.x3d
              print testCase.type
              print testCase.baseline
              print testCase.script
              if testCase.type == 'screenshot':
                if testCase.script != None:
                  testCaseFile= open (os.path.join(root, testCase.script), 'r')
                  testCaseScript = testCaseFile.read()
                else:
                  testCaseScript = ""

                script = "<PythonScript ><![CDATA[python:" + (boilerplateScript % (self.early_shutdown_file, os.path.join(root, 'output', testCase.name+"_"))) + '\n' + testCaseScript + "]]></PythonScript>"
                #print script
              
                variation_results= []
                v = Variation (testCase.name, script)
                # Create a temporary x3d file containing our injected script
                success, variation_path= self._createVariationFile ( v, os.path.join(root, testCase.x3d))
                # Run that H3DViewer with that x3d file
                result = self.runTestCase (file, testCase, variation_path, os.path.join(root, testCase.x3d), v.name, v)

                for output_file in os.listdir(os.path.join(directory, 'output')):
                  output_base, output_ext = os.path.splitext(output_file)
                  if output_base.startswith(testCase.name+"_") and not output_base.endswith('_thumb'):
                    if output_ext.lower() == '.png':
                      diff_path = os.path.join(directory, "diffs", "diff_") + output_file
                      baseline_path = os.path.join(directory, testCase.baseline, output_file)
                      comp_result = self.validate_screenshot(os.path.join(directory, 'output', output_file), baseline_path, diff_path)
                      result.screenshots_ok = comp_result['success'] and result.screenshots_ok
                      result.screenshots.append(os.path.join(directory, 'output', output_file))
                      result.screenshot_thumbs.append(comp_result['thumb']) # thumb will be of the diff if the validation failed and of the original screenshot if it succeded
                      if comp_result['diff_file'] != '':
                        result.screenshot_diffs.append(os.path.join(diff_path, comp_result['diff_file']))
                      else:
                        result.screenshot_diffs.append('')
                result.created_variation= success
                variation_results.append ( ( v, result ) )
                os.remove ( variation_path )
                      

                results.append ( variation_results )
            
                if self.error_reporter:
                  self.error_reporter.reportResults( results )
            else:
              # Test skipped
              result= TestResults()
              result.name= file
              result.url= os.path.join(root, testCase.x3d)
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
  
    for result in results:
      for variation in result:
        v= variation[0]
        r= variation[1]
        output+= "Test results for: " + str(r.name) + " (" + v.name + ")\n"
        output+= "\n"
        output+= "Started OK    : %s\n" % str(r.started_ok)
        output+= "Exited OK     : %s\n" % str(r.terminated_ok)
        output+= "Screenshot OK : %s\n" % str(r.screenshots_ok)
      
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
          screenshot = self._getErrorScreenshot( r )
          if screenshot == "":
            screenshot = self._getScreenshot( r, count=1 )
          output+= screenshot
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
      self._getScreenshot ( result ),
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

img {
  float: left;
}

"""
    
  def _getStatus ( self, status ):
    return "<span class='success'>OK</span>" if status else "<span class='fail'>FAIL</span>"
    
  def _getScreenshot ( self, result, count = 0 ):
    res = "<p>"
    for i in range(0, len(result.screenshots)):
      if result.screenshot_diffs[i] != "":
        img_path = os.path.relpath(result.screenshot_diffs[i], self.filepath)
      else:
        img_path = os.path.relpath(result.screenshots[i], self.filepath)
      thumb_path = os.path.relpath(result.screenshot_thumbs[i], self.filepath)
      res += "<a href='" + img_path + "'><image src='" + thumb_path + "' /></a>"
      count -= 1
      if count == 0:
        break
    res += "</p>\n"
    return res

  def _getErrorScreenshot( self, result ):
    for i in range(0, len(result.screenshots)):
      if result.screenshot_diffs[i] != "":
        img_path = os.path.relpath(result.screenshot_diffs[i], self.filepath)
        thumb_path = os.path.relpath(result.screenshot_thumbs[i], self.filepath)
        return "<p><a href='" + img_path + "'><image src='" + thumb_path + "' /></a></p>\n"
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

html_reporter_errors= TestReportHTML( os.path.join(args.output, ""), only_failed= True )

tester= TestCaseRunner( os.path.join(args.workingdir, ""), startup_time= 5, shutdown_time= 5, testable_callback= isTestable, error_reporter=html_reporter_errors)
results = tester.processAllTestDefinitions(directory=args.workingdir)

reporter= TestReport()
print reporter.reportResults ( results )

html_reporter= TestReportHTML( os.path.join(args.output, "") )
html_reporter.reportResults ( results )
  
html_reporter_errors.reportResults ( results )

c = getExitCode ( results )

if c != 0:
  exitCode= c
  