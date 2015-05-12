# - Find bzip2
# Find the native BZIP2 headers and libraries.
# If none is found for Windows use the distributed one in External
#
#  BZIP2_INCLUDE_DIR -  where to find bzlip.h, etc.
#  BZIP2_LIBRARIES    - List of libraries when using bzip2.
#  BZIP2_FOUND        - True if bzip2 found.

IF(H3DBZip2_FIND_REQUIRED)
  IF( WIN32 )
    FIND_PACKAGE(BZip2 QUIET REQUIRED)
  ELSE( WIN32 )
    FIND_PACKAGE(BZip2 REQUIRED)
  ENDIF( WIN32 )
ELSE(H3DBZip2_FIND_REQUIRED)
  IF( WIN32 )
    FIND_PACKAGE(BZip2 QUIET)
  ELSE( WIN32 )
    FIND_PACKAGE(BZip2)
  ENDIF( WIN32 )
ENDIF(H3DBZip2_FIND_REQUIRED)

IF( NOT BZIP2_FOUND AND WIN32)
  include( H3DExternalSearchPath )
  GET_FILENAME_COMPONENT( module_file_path ${CMAKE_CURRENT_LIST_FILE} PATH )
  get_external_search_paths_h3d( module_include_search_paths module_lib_search_paths ${module_file_path} "Bzip2" )
  
  # Look for the header file.
  FIND_PATH( BZIP2_INCLUDE_DIR NAMES bzlib.h
             PATHS ${module_include_search_paths}
             DOC "Path in which the file Bzip2.h is located." )
  
  # Look for the library.
  FIND_LIBRARY( BZIP2_LIBRARY NAMES libbz2 bz2 bzip2 
                PATHS ${module_lib_search_paths}
                DOC "Path to bzip2 library." )
  
  IF(BZIP2_INCLUDE_DIR AND BZIP2_LIBRARY)
    SET(BZIP2_FOUND 1)
    SET(BZIP2_LIBRARIES ${BZIP2_LIBRARY})
    SET(BZIP2_INCLUDE_DIR ${BZIP2_INCLUDE_DIR} )
  ENDIF(BZIP2_INCLUDE_DIR AND BZIP2_LIBRARY)
ENDIF( NOT BZIP2_FOUND AND WIN32)

# Report the results.
IF(NOT BZIP2_FOUND)
  SET(BZIP2_DIR_MESSAGE
    "BZIP2 was not found. Make sure BZIP2_LIBRARY (or BZIP2_LIBRARY_RELEASE and BZIP2_LIBRARY_DEBUG ) and BZIP2_INCLUDE_DIR are set if compressed files support is desired.")
  IF(H3DBZip2_FIND_REQUIRED)
      SET(BZIP2_DIR_MESSAGE
          "BZIP2 was not found. Make sure BZIP2_LIBRARY or BZIP2_LIBRARY_RELEASE and BZIP2_LIBRARY_DEBUG ) and BZIP2_INCLUDE_DIR are set. BZIP2 is required to build.")
      MESSAGE(FATAL_ERROR "${BZIP2_DIR_MESSAGE}")
  ELSEIF(NOT H3DBZIP2_FIND_QUIETLY)
    MESSAGE(STATUS "${BZIP2_DIR_MESSAGE}")
  ENDIF(H3DBZip2_FIND_REQUIRED)
ENDIF(NOT BZIP2_FOUND)

MARK_AS_ADVANCED(BZIP2_INCLUDE_DIR BZIP2_LIBRARY)
