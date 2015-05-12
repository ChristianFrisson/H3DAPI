# - Find V8
# Find the native V8 headers and libraries.
#
#  V8_INCLUDE_DIR -  where to find V8.h, etc.
#  V8_LIBRARIES    - List of libraries when using V8.
#  V8_FOUND        - True if V8 found.

include( H3DExternalSearchPath )
GET_FILENAME_COMPONENT( module_file_path ${CMAKE_CURRENT_LIST_FILE} PATH )
get_external_search_paths_h3d( module_include_search_paths module_lib_search_paths ${module_file_path} "pthread" )

# Look for the header file.
FIND_PATH( V8_INCLUDE_DIR NAMES v8.h
           PATHS /usr/local/include
                 ${module_include_search_paths}
           DOC "Path in which the file v8.h is located." )
MARK_AS_ADVANCED(V8_INCLUDE_DIR)

# Look for the library.
# Does this work on UNIX systems? (LINUX)
FIND_LIBRARY( V8_LIBRARY NAMES v8
              PATHS ${module_lib_search_paths}
              DOC "Path to v8 library." )
MARK_AS_ADVANCED(V8_LIBRARY)

# Copy the results to the output variables.
IF(V8_INCLUDE_DIR AND V8_LIBRARY)
  SET(V8_FOUND 1)
  SET(V8_LIBRARIES ${V8_LIBRARY})
  SET(V8_INCLUDE_DIR ${V8_INCLUDE_DIR})
ELSE(V8_INCLUDE_DIR AND V8_LIBRARY)
  SET(V8_FOUND 0)
  SET(V8_LIBRARIES)
  SET(V8_INCLUDE_DIR)
ENDIF(V8_INCLUDE_DIR AND V8_LIBRARY)

# Report the results.
IF(NOT V8_FOUND)
  SET(V8_DIR_MESSAGE
    "V8 was not found. Make sure V8_LIBRARY and V8_INCLUDE_DIR are set.")
  IF(V8_FIND_REQUIRED)
    MESSAGE(FATAL_ERROR "${V8_DIR_MESSAGE}")
  ELSEIF(NOT V8_FIND_QUIETLY)
    MESSAGE(STATUS "${V8_DIR_MESSAGE}")
  ENDIF(V8_FIND_REQUIRED)
ENDIF(NOT V8_FOUND)
