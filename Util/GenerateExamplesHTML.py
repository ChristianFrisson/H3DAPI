#!/usr/bin/python

## Generate html-files displaying the contents of the example files in ../H3DAPI/examples/All
## Run the file from this folder.
## HTML files will be put in ../../doc/H3DAPI/html/examples

import os, sys
import re
from os.path import join

current_dir = os.getcwd()
# The list at position 6 is used in a quite special way. If those directories exist in the dirs directory
# then all other directories are removed.
projects_list = [["H3DAPI", ["/../examples/All"], "/../../doc", current_dir, "examples", "../../../H3DAPI/examples"],
                 ["H3DPhysics", ["/../../H3DPhysics/examples/RigidBody", "/../../H3DPhysics/examples/softbody"], "/../../H3DPhysics/doc", current_dir + "/../../H3DPhysics/doc", "examples", "../../examples" ],
                 ["MedX3D", ["/../../MedX3D/x3d"], "/../../MedX3D/doc", current_dir + "/../../MedX3D/doc", "x3d", "../../x3d"],
                 ["UI", ["/../../UI/x3d"], "/../../UI/doc", current_dir + "/../../UI/doc", "x3d", "../../x3d"]]

for a_project in projects_list:
  examples_doxygen_page = """//////////////////////////////////////////////////////////////////////////////
    //    Copyright 2009-2013, SenseGraphics AB
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
    // \\file %sexamples_doxygen_page
    // \\brief Extra page listing all examples.
    //
    //
    //////////////////////////////////////////////////////////////////////////////
    /// \\page %sExamples
    /// \\section %sExamplesList List of examples
    """ % (a_project[0], a_project[0], a_project[0] )
    
  for ex_dir in a_project[1]:
    examples_dir = current_dir + ex_dir
    print "Processing ", examples_dir
    if os.path.isdir( examples_dir ):
      output_dir = current_dir + a_project[2]
      if not os.path.isdir( output_dir ):
        os.mkdir( output_dir )

      if a_project[0] == "H3DAPI":
        output_dir = output_dir + "/" + a_project[0]
        if not os.path.isdir( output_dir ):
          os.mkdir( output_dir )

      output_dir = output_dir + "/html"
      if not os.path.isdir( output_dir ):
        os.mkdir( output_dir )

      output_dir = output_dir + "/" + a_project[4]
      if not os.path.isdir( output_dir ):
        os.mkdir( output_dir )

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
            index_to_examples_dir = root.find( a_project[4] )
            file_path_stripped = join( root, temp_file )[index_to_examples_dir + len(a_project[4]): len(join( root, temp_file ))]
            file_path_stripped = file_path_stripped.replace( "\\", "/" )
            examples_doxygen_page = examples_doxygen_page + """/// - \\anchor %(fn_ne)s_x3d
    ///   <a href="%(rel_path)s%(stripped_file)s">%(tf)s</a>
    ///   ( <a href="%(source_dir)s/%(tf)s.html">Source</a> )
    """ % { 'fn_ne': file_name_no_end, 'rel_path' : a_project[5],'tf': temp_file, 'source_dir' : a_project[4], 'stripped_file' : file_path_stripped }
            os.system ("highlight -i %s -o %s.html --include-style" % (join(root, temp_file), temp_file))
            os.system ("move %s.html %s" % (temp_file, output_dir))

    else:
      print "The " + examples_dir + " directory does not exist."
  
  fhandle = open( a_project[3] + "/" + a_project[0] + "examples_doxygen_page", "w" )
  fhandle.write( examples_doxygen_page )
  fhandle.close()
