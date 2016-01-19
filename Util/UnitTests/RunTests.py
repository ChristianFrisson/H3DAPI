"""
This script is supposed to run unit tests on every H3DAPI node. 

Requirements:
  You must install Imagemagick (and add to PATH) in order for image comparison to function, otherwise rendering tests will always fail.
  You must install the google app-engine python library as well as gspread.

"""

import os, sys
import shutil
import time
import platform
from Queue import Queue, Empty
from threading  import Thread
import tempfile
import string
import argparse
import math
import signal
import glob
import Image
import ConfigParser
import json
import MySQLdb
#import gspread
from TestResults import TestResults
from Variations import Option, Variation

import datetime

from collections import namedtuple
#from PIL import Images

#from oauth2client.client import GoogleCredentials

from ProcessWrapper import *

parser = argparse.ArgumentParser(
  description='Runs python tests')
parser.add_argument('--workingdir', dest='workingdir', 
                    default=os.getcwd().replace('\\', '/'),
                    help='The directory containing the unit tests.')
parser.add_argument('--output', dest='output',
                   default='output',
                   help='The directory to output the results to.')
parser.add_argument('--only_validate', dest='only_validate', action='store_true',
                    default=False,
                    help='Does not run the test cases, only goes through the output directory and compares the generated output. Will not report start/termination results. Can not be combined with only_generate.')
parser.add_argument('--only_generate', dest='only_generate', action='store_true',
                    default=False,
                    help='does not run validation on the generated output. Will not report rendering or performance comparison results. Can not be combined with only_validate.')
parser.add_argument('--timestamp', dest='timestamp', help='Format is YYYY-MM-DD HH:MM:SS. The time part of the datetime is optional. This datetime will be stored in the database for all the tests this script will run. If nothing is specified it will default to the current time.',
                    default=datetime.datetime.strftime(datetime.datetime.today(), "%Y-%m-%d %H:%M:%S"))                    
parser.add_argument('--dbhost', dest='dbhost', help='Address to database.')
parser.add_argument('--dbname', dest='dbname', help='Database name.')
parser.add_argument('--dbuser', dest='dbuser', help='Database user. Needs write-access to the database.')
parser.add_argument('--dbpass', dest='dbpass', help='Database password.')
parser.add_argument('--servername', dest='servername', help='The name of this server. All results from tests run by this script will be associated with this name.',
                    default='Unknown')
parser.add_argument('--RunTestsDir', dest='RunTestsDir', help='The location of UnitTestsUtil.py and UnitTestsBoilerplate.py. This is for the cases where RunTests.py is being run from a different directory, for example for targeting a specific build of H3D.',
                    default=os.path.dirname(os.path.realpath(__file__)).replace('\\', '/'))
args = parser.parse_args()


def createFilename(orig_url, var_name):
  """ Returns a valid file name based on the provided path and variable name. """
  valid_chars = "-_.() %s%s" % (string.ascii_letters, string.digits)
  filename= orig_url + var_name
  return ''.join(c for c in filename if c in valid_chars).strip ( '.' )

def isError ( result ):
  """ Returns true if one of the possible errors occurred. """
  return (result.errors > 0 or not result.starts_ok or not result.terminates_ok) and not result.skipped

def getExitCode ( results ):
  """ Returns an exit code based on the test results. """
  for result in results:
    for variation in result:
      v= variation[0]
      r= variation[1]
      if isError ( r ):
        return 1
  return 0
  
   
class TestCaseRunner ( object ):

  def __init__ ( self, filepath, startup_time= 10, shutdown_time= 5, testable_callback= None, fake_fullscreen_due_to_PIL_bug = True, error_reporter = None ):
    self.filepath= filepath
    self.startup_time= startup_time
    self.shutdown_time= shutdown_time
    self.testable_callback= testable_callback
    self.early_shutdown_file = '%s/test_complete' % (args.RunTestsDir)
    self.startup_time_multiplier = 1
    if h3d_process_name == "H3DLoad":
      self.load_flags = [] #"--no-fullscreen","--screen=800x600"]
    else:
      self.load_flags = []
    self.db = None
    # Used to get intermediate failed results in order to be able to cancel test early and still get some results.
    self.error_reporter = error_reporter

  def getProcess( self ):
    platform_system = platform.system()
    if platform_system == 'Windows':
      return ProcessWin32()
    elif platform_system == 'Linux':
      return ProcessUnix()

  def _countWarnings ( self, test_results ):
    outhead, outpart, outtail = test_results.std_out.partition("console_start")
    errhead, errpart, errtail = test_results.std_err.partition("console_start")
    outtail = outtail.partition("console_end")[2]
    errtail = errtail.partition("console_end")[2]
    haystack = outhead + outtail[outtail.find('\n')+1:] + errhead + errtail[errtail.find('\n')+1:]

    haystack= haystack.lower()
    return ( haystack.count ( "warning" ), haystack.count ( "error" ) )

  def launchTest ( self, url, cwd ):
    process = self.getProcess()
    process.launch ( [h3d_process_name + ".exe" if platform.system() == 'Windows' else h3d_process_name] + self.load_flags + [url], cwd )
    return process

  def testStartUp ( self, url, cwd, variation ):
    """ Returns true if the example can be started. """
    
    process = self.getProcess()
    return process.testLaunch ( [h3d_process_name + ".exe" if platform.system() == 'Windows' else h3d_process_name] + self.load_flags + [url], cwd, self.startup_time, self.shutdown_time, 1 if variation and variation.global_insertion_string_failed else self.startup_time_multiplier, self.early_shutdown_file )
  
  def runTestCase (self, filename, test_case, url, orig_url= None, var_name= "", variation = None):

    if orig_url is None:
      orig_url= url
    
    test_results= TestResults( )
    test_results.filename= filename
    test_results.name= test_case.name
    test_results.url= orig_url

    self.startup_time = test_case.starttime
    self.shutdown_time = test_case.runtime + 5
    cwd= os.path.abspath(os.path.split ( orig_url )[0])
    filename= os.path.abspath ( url )
    
    if os.path.isfile( self.early_shutdown_file ):
      os.remove( self.early_shutdown_file )
    process= self.launchTest ( url, cwd )
    for i in range( 0, int(test_case.runtime) ):
      time.sleep(1)
      if os.path.isfile( self.early_shutdown_file ) or not process.isRunning():
        break
    if not process.isRunning ():
      test_results.std_out= process.getStdOut()
      test_results.std_err= process.getStdErr()
      test_results.warnings, test_results.errors= self._countWarnings ( test_results )
      test_results.terminates_ok = os.path.isfile(self.early_shutdown_file)
      return test_results
   
    self.shutdown_timeout = 10
    time_slept = 0.0
    while time_slept < self.shutdown_timeout and process.isRunning():
      time.sleep(0.5)
      time_slept += 0.5
    
    if not process.isRunning ():
      test_results.terminates_ok= True
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
  

  def parseTestDefinitionFile(self, file_path):
    """
    Parses the specified test definition file and returns a list of namedtuples containing the following:      
      name: the name of this test case
      x3d: x3d file path
      baseline: baseline folder
      script: the test script
      All of these values default to None
    The list will contain one namedtuple for each Section in the specified definition file
    """
    confParser = ConfigParser.RawConfigParser(defaults={'x3d':None, 'baseline':None, 'script':None, 'runtime':1, 'starttime':1}, allow_no_value=True)
    try:
      confParser.read(file_path)
    except:
      print sys.exc_info()[0]
      return None
    result = []
    for sect in confParser.sections():
      if sect != 'Default':
        test_case = namedtuple('TestDefinition', ['name', 'filename', 'x3d', 'baseline', 'script', 'runtime', 'starttime'])
        test_case.name = sect
        test_case.x3d = confParser.get(sect, 'x3d')
        test_case.baseline = confParser.get(sect, 'baseline folder')
        test_case.script = confParser.get(sect, 'script')
        try: 
          test_case.runtime = confParser.getfloat(sect, 'timeout')
        except:
          test_case.runtime = 1
        try:
          test_case.starttime = confParser.getfloat(sect, 'starttime')
        except:
          test_case.starttime = 1

        result.append(test_case)
    return result

  def processTestDef(self, file, testCase, results, directory):
    """
        
    """
    output_dir = os.path.abspath(os.path.join(directory, "output"))
    rendering_dir = os.path.join(directory, output_dir, 'renderings')

    for dir in [output_dir, rendering_dir]:
      if not os.path.exists(dir):
        os.mkdir(dir)

                
    script = """
    <MetadataString DEF='TestCaseName' value='%s'/>
    <MetadataString DEF='TestCaseScriptFolder' value='%s'/>
    <MetadataString DEF='TestCaseScriptFilename' value='%s'/>
    <MetadataString DEF='TestCaseDefFolder' value='%s'/>
    <MetadataString DEF='TestBaseFolder' value='%s'/>
    <MetadataFloat DEF='StartTime' value='%f'/>
    <PythonScript DEF='TestScript' url='%s'></PythonScript>""" % (testCase.name,
                                                                  os.path.split(os.path.abspath(os.path.join(directory, testCase.script)))[0].replace('\\', '/'),
                                                                  os.path.splitext(os.path.split(testCase.script)[1])[0],
                                                                  os.path.abspath(directory),
                                                                  args.RunTestsDir,
                                                                  testCase.starttime,
                                                                  os.path.join(args.RunTestsDir, 'UnitTestBoilerplate.py'))
    v = Variation (testCase.name, script)
    if not args.only_validate:
      # Create a temporary x3d file containing our injected script
      success, variation_path= self._createVariationFile ( v, os.path.join(directory, testCase.x3d))
      # Run that H3DViewer with that x3d file
      result = self.runTestCase (file, testCase, variation_path, os.path.join(directory, testCase.x3d), v.name, v)
      print result.std_err
      print result.std_out
      print os.path.abspath(output_dir + '\\validation.txt')
      result.parseValidationFile(os.path.abspath(output_dir + '\\validation.txt'), os.path.abspath(os.path.join(directory, testCase.baseline)))
    else:
      result = TestResults('')
      result.filename= file
      result.name= testCase.name
      result.url= os.path.join(directory, testCase.x3d)

    if not args.only_validate:
      os.remove ( variation_path )

    return result

  def processAllTestDefinitions ( self, directory= ".", output_dir= ".", fileExtensions= [".testdef"] ):
    """
        
    """
    try:
      os.makedirs(self.filepath)
    except:
      pass
     
    results = []
    
    self.server_name = args.servername;
    self.ConnectDB()
    
    curs = self.db.cursor()
    curs.execute("SELECT id FROM servers WHERE server_name='%s'" % self.server_name)
    res = curs.fetchone()
    if res == None:
      print("Failed to obtain server id from db")
      return
    self.server_id = res[0]

    
    for root, dirs, files in os.walk(directory):
      for file in files:
        base, ext= os.path.splitext(file)
        if ext.lower() in fileExtensions:
          file_path= os.path.join(root,file)
          print "Checking " + file_path + " for tests"
          testCases = self.parseTestDefinitionFile(file_path)
          for testCase in testCases:
            if testCase != None and testCase.x3d != None and testCase.script != None:
              print "Testing: " + testCase.name
              case_results = self.processTestDef(file, testCase, results, root)
              results.append(case_results)
              testCase.filename = (os.path.relpath(file_path, directory)).replace('\'', '/') # This is used to set up the tree structure for the results page. It will store this parameter in the database as a unique identifier of this specific file of tests.
              if case_results != None:  
                self.UploadResultsToSQL(testCase, case_results, root)            
                
              
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
 


  def ConnectDB(self):
    if self.db == None:
      try:
        self.db = MySQLdb.connect(host=args.dbhost, db=args.dbname, user=args.dbuser, passwd=args.dbpass)
        self.db.autocommit = True
        curs = self.db.cursor()
        curs.execute("SELECT * FROM servers WHERE server_name='%s'" % self.server_name)
        if curs.fetchone() == None:
          curs.execute("INSERT INTO servers (server_name) VALUES ('%s')" % self.server_name)
        curs.close()
        self.db.commit()
      except Exception as e:
        print(str(e))        
  def UploadResultsToSQL(self, testCase, case_results, output_dir):
    self.ConnectDB()
    print "Uploading results."
    curs = self.db.cursor()
    # Then ensure the test file and test_case exists in the database
    curs.execute("SELECT id FROM test_files WHERE filename='%s'" % os.path.splitext(testCase.filename.replace("\\", "/"))[0])
    res = curs.fetchone()
    if res == None: # test_file doesn't exist, so add it
      curs.execute("INSERT INTO test_files (filename) VALUES ('%s')" % os.path.splitext(testCase.filename.replace("\\", "/"))[0]) 
      curs.execute("SELECT id FROM test_files WHERE filename='%s'" % os.path.splitext(testCase.filename.replace("\\", "/"))[0])
      res = curs.fetchone()
    testfile_id = res[0]
    
    
    # Also add this test run to the database
    if not hasattr(self, "test_run_id"):
      curs.execute("INSERT INTO test_runs (timestamp, server_id) VALUES (\"" + args.timestamp + "\", %d)" % (self.server_id))
      self.test_run_id = curs.lastrowid
      if res == None:
        print("Failed to insert test run in db")
        return


    # Now ensure the test_case exists in the database
    curs.execute("SELECT test_cases.id FROM test_cases JOIN test_files WHERE case_name='%s'" % testCase.name)
    res = curs.fetchone()
    if res == None:
      curs.execute("INSERT INTO test_cases (case_name) VALUES ('%s')" % testCase.name)
      curs.execute("SELECT id FROM test_cases WHERE case_name='%s'" % testCase.name)
      res = curs.fetchone()
    testcase_id = res[0]


    #StepResultTuple = namedtuple('StepResult', ['step_name', 'success', 'results']) # results is an array of results
    #PerformanceResultTuple = namedtuple("PerformanceResult", ['fps_min', 'fps_max', 'fps_avg', 'fps_mean', 'fps_full'])
    #RenderingResultTuple = namedtuple("RenderingResult", ['success', 'baseline_path','output_path', 'diff_path'])
    #ConsoleResultTuple = namedtuple("ConsoleResult", ['success', 'baseline_path', 'output', 'diff'])
    #CustomResultTuple = namedtuple("CustomResult", ['success', 'baseline_path', 'output', 'diff'])
    for step in case_results.step_list:
      #First ensure the test step exists in the database
      curs.execute("SELECT test_steps.id FROM test_steps WHERE step_name='%s' AND test_case_id=%d" % (self.db.escape_string(step.step_name), testcase_id))
      res = curs.fetchone()
      if res == None:
        curs.execute("INSERT INTO test_steps (step_name, test_case_id) VALUES ('%s', %d)" % (self.db.escape_string(step.step_name), testcase_id))
        curs.execute("SELECT id FROM test_steps WHERE step_name='%s'" % self.db.escape_string(step.step_name))
        res = curs.fetchone()
      teststep_id = res[0]

      for result in step.results:
        if type(result).__name__ == 'ErrorResult':
          curs.execute("INSERT INTO error_results (test_run_id, file_id, case_id, step_id, stdout, stderr) VALUES (%d, %d, %d, %d, '%s', '%s')" % (self.test_run_id, testfile_id, testcase_id, teststep_id, self.db.escape_string(result.stdout), self.db.escape_string(result.stderr)))

        elif type(result).__name__ == 'ConsoleResult':
          output_string = ''
          for line in result.output:
            output_string += line
          if result.success:
            curs.execute("INSERT INTO console_results (test_run_id, file_id, case_id, step_id, success, output) VALUES (%d, %d, %d, %d, 'Y', '%s')" % (self.test_run_id, testfile_id, testcase_id, teststep_id, self.db.escape_string(output_string)))
          else:
            # Go read the baseline file so we can add that to the output
            if os.path.exists(result.baseline_path):
              f = open(result.baseline_path)
              baseline_string = f.read()
              f.close()
            else:
              baseline_string = 'Baseline not found'
            curs.execute("INSERT INTO console_results (test_run_id, file_id, case_id, step_id, success, output, baseline, diff) VALUES (%d, %d, %d, %d, 'N', '%s', '%s', '%s')" % (self.test_run_id, testfile_id, testcase_id, teststep_id, self.db.escape_string(output_string), self.db.escape_string(baseline_string), self.db.escape_string(result.diff)))
        elif type(result).__name__ == 'CustomResult':
          output_string = ''
          for line in result.output:
            output_string += line
          if result.success:
            curs.execute("INSERT INTO custom_results (test_run_id, file_id, case_id, step_id, success, output) VALUES (%d, %d, %d, %d, 'Y', '%s')" % (self.test_run_id, testfile_id, testcase_id, teststep_id, self.db.escape_string(output_string)))
          else:
            # Go read the baseline file so we can add that to the output
            if os.path.exists(result.baseline_path):
              f = open(result.baseline_path)
              baseline_string = f.read()
              f.close()
            else:
              baseline_string = 'Baseline not found'
            curs.execute("INSERT INTO custom_results (test_run_id, file_id, case_id, step_id, success, output, baseline, diff) VALUES (%d, %d, %d, %d, 'N', '%s', '%s', '%s')" % (self.test_run_id, testfile_id, testcase_id, teststep_id, self.db.escape_string(output_string), self.db.escape_string(baseline_string), self.db.escape_string(result.diff)))

        elif type(result).__name__ == 'PerformanceResult':
          # Insert the performance results, but only if all the tests in this step succeeded!
          if step.success:
            curs.execute("INSERT INTO performance_results (test_run_id, file_id, case_id, step_id, min_fps, max_fps, avg_fps, mean_fps, full_case_data) VALUES (%d, %d, %d, %d, %s, %s, %s, %s, '%s')" % (self.test_run_id, testfile_id, testcase_id, teststep_id, result.fps_min, result.fps_max, result.fps_avg, result.fps_mean, result.fps_full))
        elif type(result).__name__ == 'RenderingResult':
            step_name = step.step_name
            # Fetch the current baseline so we can compare it to the one in the database
            if os.path.exists(result.baseline_path):
              baseline_file = open(result.baseline_path, 'rb')
              baseline_image = baseline_file.read()
              baseline_file.close()
              curs.execute("SELECT id, image FROM rendering_baselines WHERE file_id=%s AND case_id=%s AND step_id='%s'" % (testfile_id, testcase_id, teststep_id))
              res = curs.fetchone()
              if res == None:
                curs.execute(("INSERT INTO rendering_baselines (file_id, case_id, step_id, image) VALUES (%d, %d, %d" % (testfile_id, testcase_id, teststep_id)) + ", %s)", [baseline_image,])
              elif res[1] != baseline_image:
                curs.execute("UPDATE rendering_baselines SET image=%s WHERE id=%s", [baseline_image, res[0]])
            
            if not result.success: #validation failed, if possible we should upload both the rendering and the diff
              if result.diff_path != '':
                diff_file = open(result.diff_path, 'rb')
                diff_image = diff_file.read()
                diff_file.close()
              else:
                diff_image = 'NULL'
              if result.output_path != '':
                output_file = open(result.output_path, 'rb')
                output_image = output_file.read()
                output_file.close()
              else:
                output_image = 'NULL'
              curs.execute("INSERT INTO rendering_results (test_run_id, file_id, case_id, step_id, success, output_image, diff_image) VALUES (%d, %d, %d, %d" % (self.test_run_id, testfile_id, testcase_id, teststep_id) + ", 'N', %s, %s)", [output_image, diff_image]);
            else:
              curs.execute("INSERT INTO rendering_results (test_run_id, file_id, case_id, step_id, success) VALUES (%d, %d, %d, %d, 'Y')" % (self.test_run_id, testfile_id, testcase_id, teststep_id));

    curs.close()
    self.db.commit()

  


   
class TestReport ( object ):
  
  def reportResults ( self, results ):
    output= ""
    if args.only_generate:
      return output
    all_renderings_succeeded = True

    for result in results:
      for variation in result:
        v= variation[0]
        r= variation[1]
        output+= "Test results for: " + str(r.name) + " (" + v.name + ")\n"
        output+= "\n"
        if not args.only_validate:
          output+= "Started OK    : %s\n" % str(r.starts_ok)
          output+= "Exited OK     : %s\n" % str(r.terminates_ok)
        if r.result_type == 'rendering':
          output+= "renderings:\n"
          for i in range(0, len(r.renderings)):
            output+= "  %s [%s]\n" % (r.step_names[i], ("OK" if r.renderings_ok[i] else "Failed"))
          if False in r.renderings_ok:
            all_renderings_succeeded = False

    if r.result_type == 'rendering':
      output+= "\n%s\n" % ("All rendering comparisons OK!" if all_renderings_succeeded else "One or more rendering comparisons Failed!")
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
          if r.test_type == 'rendering':
            rendering = self._getFirstErrorrendering( r )
            if rendering == "":
              rendering = self._getrendering( r, count=1 )
          else:
            rendering = self._getrendering( r, count=1 )

          output+= rendering
          if not args.only_validate:
            output+= "<p>Start: " + self._getStatus ( r.started_ok ) + "; "
            output+= "Exit:" + self._getStatus ( r.terminated_ok ) + "; "
          if not args.only_generate:
            output+= "renderings: " + self._getrenderingStatus( r.renderings_ok ) + "</p>\n"
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

  def _getThumbnail(self, rendering_path, thumb_dir):
    if not os.path.exists(rendering_path):
      return ""
    else:
      im = Image.open(rendering_path)
      im.thumbnail((256,256), Image.ANTIALIAS)
      rendering_filename = os.path.split(rendering_path)[1]
      thumb_path = os.path.join(thumb_dir, os.path.splitext(rendering_filename)[0] + "_thumb.png")
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
      self._getrendering ( result, count = (1 if result.test_type == 'performance' else 0)),
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
  
  def _getrenderingStatus ( self, status_list):
    return "<span class='success'>OK</span>" if not False in status_list else "<span class='fail'>FAIL</span>"
    
  def _getrendering ( self, result, count = 0 ):
    """
    Iterates through the renderings from a testcase result and returns HTML code for them.
    If count is not > 0 then it will only get that many renderings. Otherwise it will get all available renderings.
    """
    res = "<p>"
    for i in range(0, len(result.renderings)):
      if result.test_type == 'rendering' and result.rendering_diffs[i] != "":
        img_path = result.rendering_diffs[i]
      else:
        img_path = result.renderings[i]
      thumb_path = self._getThumbnail(img_path, self.filepath)
      res += "<a href='" + os.path.relpath(img_path, self.filepath) + "'><image src='" + os.path.relpath(thumb_path, self.filepath) + "' /></a>"
      count -= 1
      if count == 0:
        break
    res += "</p>\n"
    return res

  def _getFirstErrorrendering( self, result ):
    """ Iterates through all the test steps of a rendering test and returns HTML code for the first one that failed and has a diff image. """
    for i in range(0, len(result.renderings_ok)):
      if not result.renderings_ok[i] and result.rendering_diffs[i] != '':
        img_path = result.rendering_diffs[i]
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
#time.sleep(3)


h3d_process_name = "H3DLoad"

exitCode= 0

def isTestable ( file_name , files_in_dir):
  return True

#html_reporter_errors= TestReportHTML( os.path.join(args.output, "reports"), only_failed= True )

print "Running these tests using: " + subprocess.check_output('where.exe ' + h3d_process_name + '.exe') # Run our test script and wait for it to finish executing


tester= TestCaseRunner( os.path.join(args.workingdir, ""), startup_time= 5, shutdown_time= 5, testable_callback= isTestable, error_reporter=None)
nresults = tester.processAllTestDefinitions(directory=args.workingdir, output_dir=args.output)

#reporter= TestReport()
#print reporter.reportResults ( results )

#html_reporter= TestReportHTML( os.path.join(args.output, "reports") )
#html_reporter.reportResults ( results )
  
#html_reporter_errors.reportResults ( results )

#c = getExitCode ( results )

#if c != 0:
#  exitCode= c
  