# This script copies all dependencies for H3DViewer into a self-containing
# bundle for distribution on other computers. It will look for 

import subprocess
import re
import shutil
import os
import sys 

#osx_app = "/Applications/H3DViewer.app"

# Path to the H3DViewer app.
# The CPack bundle package maker sets the CMAKE_INSTALL_PREFIX to 
# Contents/Resources which is used as working directory.

osx_app = "../../../H3DViewer.app"
start_executable = osx_app + "/Contents/Resources/bin/H3DViewer"
plugin_dir = osx_app + "/Contents/Resources/Plugins"
library_dir = osx_app + "/Contents/Resources/lib"

# Given the path (as a string) to an executable or shared library it returns 
# a generator with all shared library dependencies for that file.
def otool(s):
    o = subprocess.Popen(['/usr/bin/otool', '-L', s], stdout=subprocess.PIPE)

    for l in o.stdout:
        if l[0] == '\t':
            s = l.split(' ', 1)[0][1:]
            if s.startswith("/usr/local/lib/") or s.startswith("/opt/local/lib" ) or s.startswith("/Users"):
                yield s


# find all plugin libraries
plugins = os.listdir(plugin_dir )
plugins = [ os.path.join( plugin_dir, elem ) for elem in plugins ] 

# create the start set of libraries and binaries for which to get all dependencies for
need = set([start_executable] + plugins )

# find all dependencies
done = set()
while need:
    needed = set(need)
    need = set()
    for f in needed:
        need.update(otool(f))
    done.update(needed)
    need.difference_update(done)

# a list of all the libraries needed 
needed_libraries = sorted(done)

#print needed_libraries

# make sure that lib dir exists
if not os.path.exists (library_dir):
  os.mkdir(library_dir)

# For each library, copy it into the bundle and update the install names
# of all its dependent libraries to be inside the bundle instead of its original location
for library in needed_libraries:
    print "Library: ", library

    # Copy libraries to app
    src_dir, src_file = os.path.split( library )
    src_full = library

    dest_dir = library_dir

    # Do not copy files that are already in the app.
    if( src_full.startswith( osx_app ) ):
        dest_dir = src_dir

    dest_full = os.path.join( dest_dir, src_file )
    if( src_full != dest_full ):
        shutil.copy2( src_full, dest_full )
    
    # Change the install name for all dependencies to be the path in the bundle.

    # Sometimes the install_name_tool seems to fail without getting an error 
    # message. We counter that by running it again until it succeeeds.
    counter = 2
    max_iterations = 2000

    while(counter > 1 and counter < max_iterations ):
      counter = 0
      deps = otool(dest_full)
      for dep in deps:
          counter = counter + 1
          dep_dir, dep_file = os.path.split( dep )
          o = subprocess.Popen(["/usr/bin/install_name_tool", "-change", dep, "@executable_path/../lib/" + dep_file, dest_full], stdout=subprocess.PIPE)


    if( counter >= max_iterations ):
      sys.exit(1)
        #print o
#        print "/usr/bin/install_name_tool", "-change", dep, "@executable_path/../lib/" + dep_file, dest_full

#    o = subprocess.Popen(['/usr/bin/otool', '-L', "/Applications/H3DViewer.app/Contents/Resources/lib/libxcb.1.dylib"], stdout=subprocess.PIPE)

#for f in sorted(done):
#    print f
