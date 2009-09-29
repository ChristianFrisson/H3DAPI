#!/usr/bin/python

## Modify header files to include example files in doxygen comments.
## Outputs a list of example files that do not exist in any headers.
## Will output coordaxes.x3d since it is inlined in other x3d files.

import os, sys
import re
from os.path import join

current_dir = os.getcwd()
examples_dir = current_dir + "/../examples/All"
examples = []
if os.path.isdir( examples_dir ):
  print "Handling examples directory " + examples_dir

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
      if( temp_file.endswith(".x3d") ):
        examples.append( temp_file.replace( ".x3d", "" ) )
else:
  print "The examples/All directory does not exist, this path was used: " + examples_dir

headers_dir = current_dir + "/../include/H3D"
if os.path.isdir( headers_dir ):
  print "Handling headers directory " + headers_dir

  for root, dirs, files in os.walk(headers_dir):
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
      if( not temp_file.startswith("X3D") and not temp_file.startswith("H3D") and temp_file.endswith(".h") ):
        file_name_no_end = temp_file.replace( ".h", "" )
        name_count = examples.count( file_name_no_end )
        if name_count > 0:
          # name count should not exceed 1
          if( name_count > 1 ):
            print "name_count is above 1, there should not be two files with the same name in the H3DAPI/examples/All directory"

          fhandle = open( join( headers_dir, temp_file ), "rb" )
          code = fhandle.read()
          fhandle.close()

          new_string1 = """  ///   - <a href="../../../H3DAPI/examples/All/%s.x3d">%s.x3d</a>""" %( file_name_no_end, file_name_no_end )
          new_string2 = """  ///     ( <a href="examples/%s.x3d.html">""" % ( file_name_no_end )

          if code.find( new_string1 ) == -1 and code.find( new_string2 ) == -1:
            old_string1 = """/// \par Internal routes:
  /// \dotfile %s.dot""" % ( file_name_no_end )
            old_string2 = """class H3DAPI_API %s""" % ( file_name_no_end )
            new_string = """///
  /// <b>Examples:</b>
""" + new_string1 + "\n" + new_string2 + "Source</a> )\n  "
            new_code = code.replace( old_string1, new_string + "///\n  " + old_string1 )

            if new_code == code:
              new_code = code.replace( old_string2, new_string + old_string2 )

            if new_code != code:
              fhandle = open( join( headers_dir, temp_file ), "wb" )
              fhandle.write( new_code )
              fhandle.close()
              examples.remove( file_name_no_end )
          else:
            examples.remove( file_name_no_end )

  # check if the ones left in examples were added at an earlier
  # time
  for root, dirs, files in os.walk(headers_dir):
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
      if( not temp_file.startswith("X3D") and not temp_file.startswith("H3D") and temp_file.endswith(".h") ):
        fhandle = open( join( headers_dir, temp_file ), "rb" )
        code = fhandle.read()
        fhandle.close()
        examples_to_remove = []
        for i in examples:
          find_string1 = """  ///   - <a href="../../../H3DAPI/examples/All/%s.x3d">%s.x3d</a>""" % (i, i)
          find_string2 = """  ///     ( <a href="examples/%s.x3d.html">""" % (i)
          if code.find( find_string1 ) != -1 and code.find( find_string2 ) != -1:
            examples_to_remove.append( i )

        for i in examples_to_remove:
          examples.remove( i )

  print "These are the example files that are not assigned in the documentation"
  for i in examples:
    print i + ".x3d"
else:
  print "The directory for H3D headers does not exist, this path was used: " + headers_dir
