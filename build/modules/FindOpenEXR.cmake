# - Find OpenEXR
# Finds OpenEXR libraries from ILM for handling HRD / float image formats
#
#  OpenEXR_INCLUDE_DIR -  where to find OpenEXR headers
#  OpenEXR_LIBRARIES    - List of libraries when using OpenEXR.
#  OpenEXR_FOUND        - True if OpenEXR found.


#OpenEXR requires zlib.
FIND_PACKAGE(H3DZLIB)

SET( OpenEXRIncludeSearchPath "" )
SET( OpenEXRLibrarySearchPath "" )
include( H3DExternalSearchPath )
GET_FILENAME_COMPONENT( module_file_path ${CMAKE_CURRENT_LIST_FILE} PATH )
SET( CHECK_IF_H3D_EXTERNAL_MATCHES_VS_VERSION ON )
get_external_search_paths_h3d( OpenEXRIncludeSearchPath OpenEXRLibrarySearchPath ${module_file_path} )

# Look for the header file.
FIND_PATH( OpenEXR_INCLUDE_DIR NAMES OpenEXR/Iex.h
           PATHS ${OpenEXRIncludeSearchPath}
           DOC "Path in which the file OpenEXR/Iex.h is located." )

# Look for the library.
FIND_LIBRARY( OpenEXR_LIBRARY_IMF NAMES IlmImf 
              PATHS ${OpenEXRLibrarySearchPath}
              DOC "Path to IlmImf library." )

FIND_LIBRARY( OpenEXR_LIBRARY_THREAD NAMES IlmThread 
              PATHS ${OpenEXRLibrarySearchPath}
              DOC "Path to IlmThread library." )

FIND_LIBRARY( OpenEXR_LIBRARY_MATH NAMES Imath 
              PATHS ${OpenEXRLibrarySearchPath}
              DOC "Path to Imath library." )

FIND_LIBRARY( OpenEXR_LIBRARY_HALF NAMES Half 
              PATHS ${OpenEXRLibrarySearchPath}
              DOC "Path to Half library." )

FIND_LIBRARY( OpenEXR_LIBRARY_EX NAMES Iex 
              PATHS ${OpenEXRLibrarySearchPath}
              DOC "Path to Iex library." )

IF( WIN32 )
  FIND_LIBRARY( OpenEXR_DEBUG_LIBRARY_IMF NAMES IlmImf_d 
                PATHS ${OpenEXRLibrarySearchPath}
                DOC "Path to IlmImf library." )

  FIND_LIBRARY( OpenEXR_DEBUG_LIBRARY_THREAD NAMES IlmThread_d
                PATHS ${OpenEXRLibrarySearchPath}
                DOC "Path to IlmThread library." )

  FIND_LIBRARY( OpenEXR_DEBUG_LIBRARY_MATH NAMES Imath_d
                PATHS ${OpenEXRLibrarySearchPath}
                DOC "Path to Imath library." )

  FIND_LIBRARY( OpenEXR_DEBUG_LIBRARY_HALF NAMES Half_d
                PATHS ${OpenEXRLibrarySearchPath}
                DOC "Path to Half library." )

  FIND_LIBRARY( OpenEXR_DEBUG_LIBRARY_EX NAMES Iex_d
                PATHS ${OpenEXRLibrarySearchPath}
                DOC "Path to Iex library." )
  MARK_AS_ADVANCED(OpenEXR_DEBUG_LIBRARY_IMF OpenEXR_DEBUG_LIBRARY_THREAD OpenEXR_DEBUG_LIBRARY_MATH OpenEXR_DEBUG_LIBRARY_HALF OpenEXR_DEBUG_LIBRARY_EX )
ENDIF()

IF(ZLIB_FOUND AND OpenEXR_INCLUDE_DIR AND OpenEXR_LIBRARY_IMF AND OpenEXR_LIBRARY_THREAD AND OpenEXR_LIBRARY_MATH AND OpenEXR_LIBRARY_HALF AND OpenEXR_LIBRARY_EX)
  SET(OpenEXR_FOUND 1)
  IF( OpenEXR_DEBUG_LIBRARY_IMF AND OpenEXR_DEBUG_LIBRARY_THREAD AND OpenEXR_DEBUG_LIBRARY_MATH AND OpenEXR_DEBUG_LIBRARY_HALF AND OpenEXR_DEBUG_LIBRARY_EX )
    SET(OpenEXR_LIBRARIES optimized ${OpenEXR_LIBRARY_IMF} debug ${OpenEXR_DEBUG_LIBRARY_IMF}
                          optimized ${OpenEXR_LIBRARY_THREAD} debug ${OpenEXR_DEBUG_LIBRARY_THREAD}
                          optimized ${OpenEXR_LIBRARY_MATH} debug ${OpenEXR_DEBUG_LIBRARY_MATH}
                          optimized ${OpenEXR_LIBRARY_HALF} debug ${OpenEXR_DEBUG_LIBRARY_HALF}
                          optimized ${OpenEXR_LIBRARY_EX} debug ${OpenEXR_DEBUG_LIBRARY_EX})
  ELSE()
    SET(OpenEXR_LIBRARIES ${OpenEXR_LIBRARY_IMF} ${OpenEXR_LIBRARY_THREAD} ${OpenEXR_LIBRARY_MATH} ${OpenEXR_LIBRARY_HALF} ${OpenEXR_LIBRARY_EX})
  ENDIF()
  SET( OpenEXR_INCLUDE_DIR ${OpenEXR_INCLUDE_DIR} ${ZLIB_INCLUDE_DIR} )
  SET( OpenEXR_LIBRARIES ${OpenEXR_LIBRARIES} ${ZLIB_LIBRARIES} )
  IF (WIN32)
    ADD_DEFINITIONS(-DOPENEXR_DLL)
  ENDIF()
ENDIF()

# Report the results.
IF(NOT OpenEXR_FOUND)
  MESSAGE(STATUS "OpenEXR was not found. Handling OpenEXR images will not be supported. OpenEXR also requires zlib.")
ENDIF()

MARK_AS_ADVANCED(OpenEXR_INCLUDE_DIR OpenEXR_LIBRARY_IMF OpenEXR_LIBRARY_THREAD OpenEXR_LIBRARY_MATH OpenEXR_LIBRARY_HALF OpenEXR_LIBRARY_EX )
