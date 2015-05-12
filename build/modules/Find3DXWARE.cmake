# - Find 3dxware (www.3dconnexion.com)
#
#  3DXWARE_INCLUDE_DIR -  where to find si.h, siapp.h for Windows and xdrvlib.h, Xlib.h, Xutil.h, Xos.h, Xatom.h and keysym.h for LINUX.
#  3DXWARE_LIBRARIES    - List of libraries when using 3dxware.
#  3DXWARE_FOUND        - True if 3dxware is found.

include( H3DExternalSearchPath )
GET_FILENAME_COMPONENT( module_file_path ${CMAKE_CURRENT_LIST_FILE} PATH )
get_external_search_paths_h3d( module_include_search_paths module_lib_search_paths ${module_file_path} "3dconnexion/inc" )

# Look for the header file.
FIND_PATH( 3DXWARE_INCLUDE_DIR NAMES si.h siapp.h H3D/xdrvlib.h X11/Xlib.h X11/Xutil.h X11/Xos.h X11/Xatom.h X11/keysym.h
           PATHS  /usr/local/include 
                 ${module_include_search_paths}
           DOC "Path in which the files si.h, siapp.h, H3D/xdrvlib.h, X11/Xlib.h, X11/Xutil.h, X11/Xos.h, X11/Xatom.h and X11/keysym.h are located." )
MARK_AS_ADVANCED(3DXWARE_INCLUDE_DIR)

# Look for the library siapp.
# TODO: Does this work on UNIX systems? (LINUX) I strongly doubt it. What are the libraries to find on linux?
FIND_LIBRARY( 3DXWARE_SIAPP_LIBRARY NAMES siapp
              PATHS ${module_lib_search_paths}
              DOC "Path to siapp library." )
MARK_AS_ADVANCED(3DXWARE_SIAPP_LIBRARY)

# Look for the library spwmath.
# Does this work on UNIX systems? (LINUX)
FIND_LIBRARY( 3DXWARE_SPWMATH_LIBRARY NAMES spwmath
              PATHS ${module_lib_search_paths}
              DOC "Path to spwmath library." )
MARK_AS_ADVANCED(3DXWARE_SPWMATH_LIBRARY)

# Copy the results to the output variables.
IF(3DXWARE_INCLUDE_DIR AND 3DXWARE_SIAPP_LIBRARY AND 3DXWARE_SPWMATH_LIBRARY)
  SET(3DXWARE_FOUND 1)
  SET(3DXWARE_LIBRARIES ${3DXWARE_SIAPP_LIBRARY} ${3DXWARE_SPWMATH_LIBRARY})
  SET(3DXWARE_INCLUDE_DIR ${3DXWARE_INCLUDE_DIR})
ELSE(3DXWARE_INCLUDE_DIR AND 3DXWARE_SIAPP_LIBRARY AND 3DXWARE_SPWMATH_LIBRARY)
  SET(3DXWARE_FOUND 0)
  SET(3DXWARE_LIBRARIES)
  SET(3DXWARE_INCLUDE_DIR)
ENDIF(3DXWARE_INCLUDE_DIR AND 3DXWARE_SIAPP_LIBRARY AND 3DXWARE_SPWMATH_LIBRARY)

# Report the results.
IF(NOT 3DXWARE_FOUND)
  SET(3DXWARE_DIR_MESSAGE
    "3dxware (www.3dconnexion) was not found. Make sure 3DXWARE_SIAPP_LIBRARY, 3DXWARE_SPWMATH_LIBRARY and 3DXWARE_INCLUDE_DIR are set.")
  IF(3DXWARE_FIND_REQUIRED)
    MESSAGE(FATAL_ERROR "${3DXWARE_DIR_MESSAGE}")
  ELSEIF(NOT ${3DXWARE_FIND_QUIETLY})
    MESSAGE(STATUS "${3DXWARE_DIR_MESSAGE}")
  ENDIF(3DXWARE_FIND_REQUIRED)
ENDIF(NOT 3DXWARE_FOUND)

