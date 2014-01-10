## \file AutomaticLessonCreator.py
## Used to create lesson files from automatically segmented jaw.
import sys
import os
import subprocess
import win32api
import win32com.client
from Queue import Queue, Empty
from threading  import Thread
import time

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

if len( sys.argv ) > 1:
  source_dir = sys.argv[1]
  if len( sys.argv ) > 2:
    target_dir = sys.argv[2]
  else:
    target_dir = source_dir
  
  log_file = None
  if len( sys.argv ) > 3:
    log_file_url = sys.argv[3]
    if os.path.split( log_file_url )[0] != "" and not os.path.isdir( os.path.split( log_file_url )[0] ):
      os.makedirs( os.path.split( log_file_url )[0] )
    log_file = open( log_file_url, 'w' )
  
  sleep_time = 1
  total_time = 0
  # Estimate time it will take to run this test.
  for root, dirs, files in os.walk( source_dir ):
    for file in files:
      if file.endswith( ".x3d" ):
        total_time = total_time + sleep_time
  
  print "THE CONVERSION WILL TAKE APPROXIMATELY ", str( total_time ), " seconds."
  
  cwd = os.getcwd()
  process= ProcessWin32()
  for root, dirs, files in os.walk( source_dir ):
    for file in files:
      if file.endswith( ".x3d" ):
        source_file = os.path.join( root, file )
        dest_file = source_file.replace( source_dir, target_dir )
        if os.path.split( dest_file )[0] != "" and not os.path.isdir( os.path.split( dest_file )[0] ):
          os.makedirs( os.path.split( dest_file )[0] )
        process.launch ( ["H3DLoad.exe"] + [os.path.join(cwd,"ifs2its.x3d")] + [source_file] + [dest_file], cwd )
        time.sleep(sleep_time)
        process.kill()
        if log_file:
          log_file.write( process.getStdOut() )
          log_file.write( process.getStdErr() )
          log_file.write( "---------------------" )
  if log_file:
    log_file.close()
  
else:
  print "Too few arguments given. Usage: ifstoitswalkdir.py sourceDirectory targetDirectory"
  