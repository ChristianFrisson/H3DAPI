#!/usr/bin/python

## Generate html-files displaying the contents of the example files in ../H3DAPI/examples/All
## Run the file from this folder.
## HTML files will be put in ../../doc/H3DAPI/html/examples

import os, sys
import re
from os.path import join

current_dir = os.getcwd()
examples_dir = current_dir + "/../examples/All"
if os.path.isdir( examples_dir ):
  output_dir = current_dir + "/../../doc"
  if not os.path.isdir( output_dir ):
    os.mkdir( output_dir )
  print output_dir

  output_dir = output_dir + "/H3DAPI"
  if not os.path.isdir( output_dir ):
    os.mkdir( output_dir )
  print output_dir

  output_dir = output_dir + "/html"
  if not os.path.isdir( output_dir ):
    os.mkdir( output_dir )
  print output_dir

  output_dir = output_dir + "/examples"
  if not os.path.isdir( output_dir ):
    os.mkdir( output_dir )
  print output_dir

  examples_doxygen_page = """//////////////////////////////////////////////////////////////////////////////
//    Copyright 2009, SenseGraphics AB
//
//    This file is part of H3DAPI.
//
//    H3DAPI is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    H3DAPI is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with H3DAPI; if not, write to the Free Software
//    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//    A commercial license is also available. Please contact us at 
//    www.sensegraphics.com for more information.
//
//
// \\file examples_doxygen_page
// \\brief Extra page listing all examples.
//
//
//////////////////////////////////////////////////////////////////////////////
/// \\page H3DExamples
/// \\section H3DExamplesList List of examples
"""

  for root, dirs, files in os.walk(examples_dir):
    if '.svn' in dirs:
      dirs.remove('.svn')  # don't visit CVS directories
    if 'branches' in dirs:
      dirs.remove('branches')  # don't visit branches directories
    if 'obj' in dirs:
      dirs.remove('obj')  # don't visit obj directories
    if 'vc7' in dirs:
      dirs.remove('vc7')  # don't visit vc7 directories
    if 'vc8' in dirs:
      dirs.remove('vc8')  # don't visit vc8 directories
    if 'vc9' in dirs:
      dirs.remove('vc9')  # don't visit vc9 directories
    for temp_file in files:
      file_name_no_end = temp_file.replace( ".x3d", "" )
      if( temp_file.endswith(".x3d") ):
        examples_doxygen_page = examples_doxygen_page + """/// - \\anchor %(fn_ne)s_x3d
///   <a href="../../../H3DAPI/examples/All/%(tf)s">%(tf)s</a>
///   ( <a href="examples/%(tf)s.html">Source</a> )
""" % { 'fn_ne': file_name_no_end, 'tf': temp_file }
        os.system ("highlight -i %s -o %s.html --include-style" % (join(root, temp_file), temp_file))
        os.system ("move %s.html %s" % (temp_file, output_dir))


    fhandle = open( current_dir + "/examples_doxygen_page", "w" )
    fhandle.write( examples_doxygen_page )
    fhandle.close()
else:
  print "The examples/All directory does not exist, this path was used: " + examples_dir
