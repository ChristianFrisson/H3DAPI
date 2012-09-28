# - Find OpenAL
# Find the native OpenAL headers and libraries.
# If none is found for Windows use the distributed one in External
#
#  OPENAL_INCLUDE_DIR -  where to find al.h, etc.
#  OPENAL_LIBRARIES    - List of libraries when using OpenAL.
#  OPENAL_FOUND        - True if OpenAL found.
IF( WIN32 )
  SET(OpenAL_FIND_QUIETLY 1)
ENDIF( WIN32 )

FIND_PACKAGE(OpenAL)

IF( CMAKE_CL_64 )
  SET( LIB "lib64" )
ELSE( CMAKE_CL_64 )
  SET( LIB "lib32" )
ENDIF( CMAKE_CL_64 )

IF(NOT OPENAL_FOUND AND WIN32)
  GET_FILENAME_COMPONENT(module_file_path ${CMAKE_CURRENT_LIST_FILE} PATH )
  # Look for the header file.
  FIND_PATH( OPENAL_INCLUDE_DIR NAMES AL/al.h
             PATHS $ENV{H3D_EXTERNAL_ROOT}/include
                   $ENV{H3D_ROOT}/../External/include
                   ../../External/include
                   ${module_file_path}/../../../External/include
             DOC "Path in which the file AL/al.h is located." )

  # Look for the library.
  FIND_LIBRARY( OPENAL_LIBRARY NAMES OpenAL32
                PATHS $ENV{H3D_EXTERNAL_ROOT}/${LIB}
                      $ENV{H3D_ROOT}/../External/${LIB}
                      ../../External/${LIB}
                      ${module_file_path}/../../../External/${LIB}
                DOC "Path to OpenAL32 library." )

  IF(OPENAL_INCLUDE_DIR AND OPENAL_LIBRARY)
    SET(OPENAL_FOUND 1)
    SET(OPENAL_LIBRARIES ${OPENAL_LIBRARY})
    SET(OPENAL_INCLUDE_DIR ${OPENAL_INCLUDE_DIR})
  ENDIF(OPENAL_INCLUDE_DIR AND OPENAL_LIBRARY)
ELSEIF( OPENAL_FOUND)
  # Apparently this variable is not set on linux.
  SET(OPENAL_LIBRARIES ${OPENAL_LIBRARY})
ENDIF(NOT OPENAL_FOUND AND WIN32)

# Report the results.
IF(NOT OPENAL_FOUND)
  SET(OPENAL_DIR_MESSAGE
    "OpenAL was not found. Make sure OPENAL_LIBRARY and OPENAL_INCLUDE_DIR are set.")
  IF(OpenAL_FIND_REQUIRED)
    SET(OPENAL_DIR_MESSAGE
        "${OPENAL_DIR_MESSAGE} OpenAL is required to build.")
    MESSAGE(FATAL_ERROR "${OPENAL_DIR_MESSAGE}")
  ELSEIF(NOT OpenAL_FIND_QUIETLY)
    MESSAGE(STATUS "${OPENAL_DIR_MESSAGE}")
  ENDIF(OpenAL_FIND_REQUIRED)
ENDIF(NOT OPENAL_FOUND)

MARK_AS_ADVANCED(OPENAL_INCLUDE_DIR OPENAL_LIBRARY)
