# - Find AUDIOFILE
# Find the native AUDIOFILE headers and libraries.
#
#  AUDIOFILE_INCLUDE_DIR -  where to find AUDIOFILE.h, etc.
#  AUDIOFILE_LIBRARIES    - List of libraries when using AUDIOFILE.
#  AUDIOFILE_FOUND        - True if AUDIOFILE found.

include( H3DExternalSearchPath )
GET_FILENAME_COMPONENT( module_file_path ${CMAKE_CURRENT_LIST_FILE} PATH )
get_external_search_paths_h3d( module_include_search_paths module_lib_search_paths ${module_file_path} "libaudiofile" )

# Look for the header file.
FIND_PATH( AUDIOFILE_INCLUDE_DIR NAMES audiofile.h
           PATHS /usr/local/include
                 ${module_include_search_paths}
           DOC "Path in which the file audiofile.h is located." )
MARK_AS_ADVANCED(AUDIOFILE_INCLUDE_DIR)

# Look for the library.
# Does this work on UNIX systems? (LINUX)
FIND_LIBRARY( AUDIOFILE_LIBRARY NAMES audiofile
              PATHS ${module_lib_search_paths}
              DOC "Path to audiofile library." )
MARK_AS_ADVANCED(AUDIOFILE_LIBRARY)

# Copy the results to the output variables.
IF(AUDIOFILE_INCLUDE_DIR AND AUDIOFILE_LIBRARY)
  SET(AUDIOFILE_FOUND 1)
  SET(AUDIOFILE_LIBRARIES ${AUDIOFILE_LIBRARY})
  SET(AUDIOFILE_INCLUDE_DIR ${AUDIOFILE_INCLUDE_DIR})
ELSE(AUDIOFILE_INCLUDE_DIR AND AUDIOFILE_LIBRARY)
  SET(AUDIOFILE_FOUND 0)
  SET(AUDIOFILE_LIBRARIES)
  SET(AUDIOFILE_INCLUDE_DIR)
ENDIF(AUDIOFILE_INCLUDE_DIR AND AUDIOFILE_LIBRARY)

# Report the results.
IF(NOT AUDIOFILE_FOUND)
  SET(AUDIOFILE_DIR_MESSAGE
    "AUDIOFILE was not found. Make sure AUDIOFILE_LIBRARY and AUDIOFILE_INCLUDE_DIR are set.")
  IF(Audiofile_FIND_REQUIRED)
    MESSAGE(FATAL_ERROR "${AUDIOFILE_DIR_MESSAGE}")
  ELSEIF(NOT Audiofile_FIND_QUIETLY)
    MESSAGE(STATUS "${AUDIOFILE_DIR_MESSAGE}")
  ENDIF(Audiofile_FIND_REQUIRED)
ENDIF(NOT AUDIOFILE_FOUND)
