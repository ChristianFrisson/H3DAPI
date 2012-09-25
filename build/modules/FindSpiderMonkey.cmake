# - Find SPIDERMONKEY
# Find the native SPIDERMONKEY headers and libraries.
#
#  SPIDERMONKEY_INCLUDE_DIR -  where to find SPIDERMONKEY.h, etc.
#  SPIDERMONKEY_LIBRARIES    - List of libraries when using SPIDERMONKEY.
#  SPIDERMONKEY_FOUND        - True if SPIDERMONKEY found.

GET_FILENAME_COMPONENT(module_file_path ${CMAKE_CURRENT_LIST_FILE} PATH )

IF( CMAKE_CL_64 )
  SET( LIB "lib64" )
ELSE( CMAKE_CL_64 )
  SET( LIB "lib32" )
ENDIF( CMAKE_CL_64 )

# Look for the header file.
FIND_PATH( SPIDERMONKEY_INCLUDE_DIR NAMES jsapi.h
           PATHS /usr/local/include
                 /usr/local/include/mozjs
                 /usr/local/include/js
                 /opt/local/include/js
                 /usr/include/mozjs
                 /usr/include/js
                 $ENV{H3D_EXTERNAL_ROOT}/include  
                 $ENV{H3D_EXTERNAL_ROOT}/include/js  
                 $ENV{H3D_ROOT}/../External/include  
                 $ENV{H3D_ROOT}/../External/include/js  
                 ../../External/include
                 ../../External/include/js
                 ${module_file_path}/../../../External/include
                 ${module_file_path}/../../../External/include/js
           DOC "Path in which the file jsapi.h is located." )
MARK_AS_ADVANCED(SPIDERMONKEY_INCLUDE_DIR)

# Look for the library.
# Does this work on UNIX systems? (LINUX)
IF(WIN32)
  FIND_LIBRARY( SPIDERMONKEY_LIBRARY NAMES js32
                PATHS $ENV{H3D_EXTERNAL_ROOT}/${LIB}
                      $ENV{H3D_ROOT}/../External/${LIB}
                      ../../External/${LIB}
                      ${module_file_path}/../../../External/${LIB}
                DOC "Path to js32 library." )
ELSE(WIN32)
  FIND_LIBRARY( SPIDERMONKEY_LIBRARY NAMES mozjs js
                                     DOC "Path to mozjs library." )
ENDIF(WIN32)
MARK_AS_ADVANCED(SPIDERMONKEY_LIBRARY)

# Copy the results to the output variables.
IF(SPIDERMONKEY_INCLUDE_DIR AND SPIDERMONKEY_LIBRARY)
  SET(SPIDERMONKEY_FOUND 1)
  SET(SPIDERMONKEY_LIBRARIES ${SPIDERMONKEY_LIBRARY})
  SET(SPIDERMONKEY_INCLUDE_DIR ${SPIDERMONKEY_INCLUDE_DIR})
ELSE(SPIDERMONKEY_INCLUDE_DIR AND SPIDERMONKEY_LIBRARY)
  SET(SPIDERMONKEY_FOUND 0)
  SET(SPIDERMONKEY_LIBRARIES)
  SET(SPIDERMONKEY_INCLUDE_DIR)
ENDIF(SPIDERMONKEY_INCLUDE_DIR AND SPIDERMONKEY_LIBRARY)

# Report the results.
IF(NOT SPIDERMONKEY_FOUND)
  SET(SPIDERMONKEY_DIR_MESSAGE
    "SPIDERMONKEY was not found. Make sure SPIDERMONKEY_LIBRARY and SPIDERMONKEY_INCLUDE_DIR are set.")
  IF(Spidermonkey_FIND_REQUIRED)
    MESSAGE(FATAL_ERROR "${SPIDERMONKEY_DIR_MESSAGE}")
  ELSEIF(NOT Spidermonkey_FIND_QUIETLY)
    MESSAGE(STATUS "${SPIDERMONKEY_DIR_MESSAGE}")
  ENDIF(Spidermonkey_FIND_REQUIRED)
ENDIF(NOT SPIDERMONKEY_FOUND)
