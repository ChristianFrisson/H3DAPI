import argparse, os

testDefHeader = """
#  Contains the definitions for one or more test cases.
#  Every test case is specified with a section in this file and the name of that section will be the name of the test case
#  Required parameters:
#    x3d - Specifies the x3d file to run the test with.
#    baseline folder - Specifies where the baselines for this test are.
#    script - A .py file containing the test script itself.
#  Optional parameters:
#    starttime - Defaults to 1. How long h3d should wait after launching before it starts doing the test, specified in seconds.
#    timeout - Defaults to 1. A timeout that starts counting down immediately after h3d is launched. This should be longer than the expected time of your test, as a test will be considered failed if it times out. This timeout is primarily for when H3D has frozen, since the timeout will be the only way to shut down a frozen H3D automatically and continue with the testing. 

"""

testScriptHeader = """
from UnitTestUtil import *
from H3DInterface import *
from H3DUtils import *

\"""
  Test script file
  Any number of functions can be defined here. The test functions shouldn't take any parameters.
  
  Decorators are used to specify that a function is a test step and to specify what forms of validation should be used on that step.
  Test steps are executed in the order they are listed in this script file.
  
  The available decorators, and their parameters, are shown below.
  Parameters inside brackets [] are optional.
  In the case of start_time and run_time the highest start_time and the highest run_time specified by any decorator for one test step will be the value that is used.
  so if a function has one decorator that specifies start_time as 2 and another decorator that specifies it as 5 then the start_time that is used is 5. The same applies for run_time.
  
  Decorators:
    @console([start_time])
      Will wait the specified start_time and then execute the function. Any console output when the function is executed will be stored and validated
      If no start_time is specified then it will not wait before calling it and will instead call it immediately after the previous function is finished (or immediately after the test has started)
      Console output will also be stored as <testcase folder>/output/text/<case name>_<step_name>_console.txt
    
    @custom([start_time])
      Behaves the same as @console except instead of storing console output it will store all the strings passed to the function printCustom(string)
      printCustom(string) will only work in a test step function that has the @custom decorator and is only intended to be called by you inside your test steps
      The optional start_time parameter is the same as for @console
      Custom output will also be stored as <testcase folder>/output/text/<case name>_<step_name>_custom.txt
    
    @screenshot([start_time], [run_time])
      Will wait the specified start_time, execute the function, and then wait the specified run_time. After waiting it will take a screenshot that will be validated.
      Screenshot output will also be stored as <testcase folder>/output/renderings/<case name>_<step_name>.png
      
    @performance([start_time], run_time)
      Will wait the specified start_time, execute the function, and then record performance data during the specified run_time.
      Performance data is not in itself validated and as such is not output anywhere except the results database.
      The recorded performance data WILL BE IGNORED unless there is also another decorator that is validated successfully.
      On other words we will not store and display performance data for a test that has not been verified to give the expected output. @screenshot is recommended for this since we typically will only want to record performance data for a test that also rendered correctly.
\"""

@screenshot()
def firstStep():
  pass

"""

parser = argparse.ArgumentParser(description='Runs python tests')
parser.add_argument('--workingdir', dest='workingdir', 
                    help='The directory that any relative paths start from and where you want the output file to end up')
parser.add_argument('--x3ddir', dest='x3ddir', 
                    help='The directory the x3d files are in')
parser.add_argument('--testDefName', dest='testDefName', 
                    default="",
                    help='The name of the TestDef file (without file ending) to generate')
parser.add_argument('--generateEmptyScripts', dest='generateEmptyScripts', 
                    default=False,
                    help='Whether all the x3d files that do not have .py files with the same name should have sample .py files generated for them or not.')
args = parser.parse_known_args()[0]

if args.testDefName == "":
  print "You need to specify an output file. Use --testDefName."
  exit(1)


defFile = open(os.path.join(args.workingdir, args.testDefName + ".TestDef"), "w")

defFile.write(testDefHeader)
files_found = 0
scripts_generated = 0

for file in os.listdir(os.path.join(args.workingdir, args.x3ddir)):
  base, ext= os.path.splitext(file)
  if ext.lower() == '.x3d':
    has_script = os.path.exists(os.path.join(args.workingdir, args.x3ddir, base) + ".py")
    if not has_script:
      if args.generateEmptyScripts:
        new_script = open(os.path.join(args.workingdir, args.x3ddir, base) + ".py", 'w')
        new_script.write(testScriptHeader)
        new_script.flush()
        new_script.close()
        has_script = True
        scripts_generated += 1
    
    if has_script:
      files_found += 1
      defFile.write("[" + base + "]\n")
      defFile.write("x3d=" +  os.path.join(args.x3ddir, file) + "\n")
      defFile.write("script=" + os.path.join(args.x3ddir, base) + ".py\n")
      defFile.write("baseline folder=baseline\n")
      defFile.write("starttime=5\n")
      defFile.write("timeout=30\n\n")

defFile.flush()
defFile.close()

print "Found " + str(files_found) + " files."
print "Generated " + str(scripts_generated) + " scripts."