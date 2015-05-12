# - Find GLEW
# Find the native GLEW headers and libraries.
#
#  GLEW_INCLUDE_DIR -  where to find GLEW.h, etc.
#  GLEW_LIBRARIES    - List of libraries when using GLEW.
#  GLEW_FOUND        - True if GLEW found.

include( H3DExternalSearchPath )
GET_FILENAME_COMPONENT( module_file_path ${CMAKE_CURRENT_LIST_FILE} PATH )
get_external_search_paths_h3d( module_include_search_paths module_lib_search_paths ${module_file_path} )

# Look for the header file.
FIND_PATH(GLEW_INCLUDE_DIR NAMES GL/glew.h
                           PATHS ${module_include_search_paths}
                           DOC "Path in which the file GL/glew.h is located." )
MARK_AS_ADVANCED(GLEW_INCLUDE_DIR)

# Look for the library.
FIND_LIBRARY(GLEW_LIBRARY NAMES GLEW glew32
                                PATHS ${module_lib_search_paths}
                                DOC "Path to glew32 library." )
MARK_AS_ADVANCED(GLEW_LIBRARY)

IF( WIN32 AND PREFER_STATIC_LIBRARIES )
  set( module_include_search_paths "")
  set( module_lib_search_paths "")
  get_external_search_paths_h3d( module_include_search_paths module_lib_search_paths ${module_file_path} "static" )
  FIND_LIBRARY( GLEW_STATIC_LIBRARY NAMES glew32s
                                         PATHS ${module_lib_search_paths}
                                    DOC "Path to glew32 static library." )
  MARK_AS_ADVANCED(GLEW_STATIC_LIBRARY)
ENDIF( WIN32 AND PREFER_STATIC_LIBRARIES )

IF( GLEW_LIBRARY OR GLEW_STATIC_LIBRARY )
  SET( GLEW_LIBRARIES_FOUND 1 )
ENDIF( GLEW_LIBRARY OR GLEW_STATIC_LIBRARY )

# Copy the results to the output variables.
IF(GLEW_INCLUDE_DIR AND GLEW_LIBRARIES_FOUND)
  SET(GLEW_FOUND 1)
  IF( WIN32 AND PREFER_STATIC_LIBRARIES AND GLEW_STATIC_LIBRARY )
    SET(GLEW_LIBRARIES ${GLEW_STATIC_LIBRARY})
    SET( GLEW_STATIC 1 )
  ELSE( WIN32 AND PREFER_STATIC_LIBRARIES AND GLEW_STATIC_LIBRARY )
    SET(GLEW_LIBRARIES ${GLEW_LIBRARY})
  ENDIF( WIN32 AND PREFER_STATIC_LIBRARIES AND GLEW_STATIC_LIBRARY )
  SET(GLEW_INCLUDE_DIR ${GLEW_INCLUDE_DIR})
  SET(GLEW_INCLUDE_DIRS ${GLEW_INCLUDE_DIR})
ELSE(GLEW_INCLUDE_DIR AND GLEW_LIBRARIES_FOUND)
  SET(GLEW_FOUND 0)
  SET(GLEW_LIBRARIES)
  SET(GLEW_INCLUDE_DIR)
ENDIF(GLEW_INCLUDE_DIR AND GLEW_LIBRARIES_FOUND)

# Report the results.
IF(NOT GLEW_FOUND)
  SET(GLEW_DIR_MESSAGE
    "GLEW was not found. Make sure GLEW_LIBRARY and GLEW_INCLUDE_DIR are set to where you have your glew header and lib files. If you do not have the library you will not be able to use nodes that use OpenGL extensions.")
  IF(GLEW_FIND_REQUIRED)
      MESSAGE(FATAL_ERROR "${GLEW_DIR_MESSAGE}")
  ELSEIF(NOT GLEW_FIND_QUIETLY)
    MESSAGE(STATUS "${GLEW_DIR_MESSAGE}")
  ENDIF(GLEW_FIND_REQUIRED)
ENDIF(NOT GLEW_FOUND)
