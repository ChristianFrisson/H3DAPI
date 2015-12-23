""" Contains the Process interface definition as well as implementations of it for Unix and Windows """

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

if platform.system() == 'Windows':
  import win32api
  import win32com.client


class Process ( object ):
  """ Interface for manipulating a process. """

  def launch ( self, command, cwd ):
    pass

  def testLaunch ( self, command, cwd, startup_time, shutdown_time, startup_time_multiplier, early_shutdown_file ):
    """ Makes sure early_shutdown_file doesn't exist, calls launch() and then waits for the specified startup and shutdown time. """
    pass
    
  def isRunning ( self ):
    """ Checks if the process associated with this object is running. """
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
    shell.AppActivate ( self.process_name )
    time.sleep ( 0.1 )
    shell.SendKeys(key)
    
  def kill ( self ):
    # for Windows XP
    kill_process = "taskkill /F /im "+ self.process_name
    os.system( kill_process )
    
    # If the test crashed, close the crash dialog box too
    shell = win32com.client.Dispatch("WScript.Shell")
    shell.AppActivate ( self.process_name )
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
