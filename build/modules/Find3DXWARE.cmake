# - Find 3dxware (www.3dconnexion.com)
#
#  3DXWARE_INCLUDE_DIR -  where to find si.h, siapp.h for Windows and xdrvlib.h, Xlib.h, Xutil.h, Xos.h, Xatom.h and keysym.h for LINUX.
#  3DXWARE_LIBRARIES    - List of libraries when using 3dxware.
#  3DXWARE_FOUND        - True if 3dxware is found.

GET_FILENAME_COMPONENT(module_file_path ${CMAKE_CURRENT_LIST_FILE} PATH )

IF( CMAKE_CL_64 )
  SET( LIB "lib64" )
ELSE( CMAKE_CL_64 )
  SET( LIB "lib32" )
ENDIF( CMAKE_CL_64 )

# Look for the header file.
FIND_PATH( 3DXWARE_INCLUDE_DIR NAMES si.h siapp.h H3D/xdrvlib.h X11/Xlib.h X11/Xutil.h X11/Xos.h X11/Xatom.h X11/keysym.h
           PATHS  /usr/local/include 
                 $ENV{H3D_EXTERNAL_ROOT}/include  
                 $ENV{H3D_EXTERNAL_ROOT}/include/3dconnexion/inc
                 $ENV{H3D_ROOT}/../External/include  
                 $ENV{H3D_ROOT}/../External/include/3dconnexion/inc
                 ../../External/include
                 ../../External/include/3dconnexion/inc
                 ${module_file_path}/../../../External/include
                 ${module_file_path}/../../../External/include/3dconnexion/inc
           DOC "Path in which the files si.h, siapp.h, H3D/xdrvlib.h, X11/Xlib.h, X11/Xutil.h, X11/Xos.h, X11/Xatom.h and X11/keysym.h are located." )
MARK_AS_ADVANCED(3DXWARE_INCLUDE_DIR)

# Look for the library siapp.
# TODO: Does this work on UNIX systems? (LINUX) I strongly doubt it. What are the libraries to find on linux?
FIND_LIBRARY( 3DXWARE_SIAPP_LIBRARY NAMES siapp
              PATHS $ENV{H3D_EXTERNAL_ROOT}/${LIB}
                    $ENV{H3D_ROOT}/../External/${LIB}
                    ../../External/${LIB}
                    ${module_file_path}/../../../External/${LIB}
              DOC "Path to siapp library." )
MARK_AS_ADVANCED(3DXWARE_SIAPP_LIBRARY)

# Look for the library spwmath.
# Does this work on UNIX systems? (LINUX)
FIND_LIBRARY( 3DXWARE_SPWMATH_LIBRARY NAMES spwmath
              PATHS $ENV{H3D_EXTERNAL_ROOT}/${LIB}
                    $ENV{H3D_ROOT}/../External/${LIB}
                    ../../External/${LIB}
                    ${module_file_path}/../../../External/${LIB}
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

