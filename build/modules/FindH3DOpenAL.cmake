# - Find OpenAL
# Find the native OpenAL headers and libraries.
# If none is found for Windows use the distributed one in External
#
#  OPENAL_INCLUDE_DIR -  where to find al.h, etc.
#  OPENAL_LIBRARIES    - List of libraries when using OpenAL.
#  OPENAL_FOUND        - True if OpenAL found.


FIND_PACKAGE(OpenAL)
IF(NOT OPENAL_FOUND AND WIN32)
  # Look for the header file.
  FIND_PATH( OPENAL_INCLUDE_DIR NAMES al.h
             PATHS $ENV{H3D_EXTERNAL_ROOT}/include  
                   $ENV{H3D_EXTERNAL_ROOT}/include/OpenAL
                   ../../External/include    
                   ../../External/include/OpenAL )
  
  # Look for the library.
  FIND_LIBRARY( OPENAL_LIBRARY NAMES OpenAL32
                PATHS $ENV{H3D_EXTERNAL_ROOT}/lib
                      ../../External/lib )    
 
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
    "OpenAL was not found. Make sure OPENAL_LIBRARY and OPENAL_INCLUDE_DIR are set if compressed files support is desired.")
  IF(NOT OPENAL_FIND_QUIETLY)
    MESSAGE(STATUS "${OPENAL_DIR_MESSAGE}")
  ELSE(NOT OPENAL_FIND_QUIETLY)
    IF(OPENAL_FIND_REQUIRED)
      SET(OPENAL_DIR_MESSAGE
          "OpenAL was not found. Make sure OPENAL_LIBRARY and OPENAL_INCLUDE_DIR are set. OpenAL is required to build.")
      MESSAGE(FATAL_ERROR "${OPENAL_DIR_MESSAGE}")
    ENDIF(OPENAL_FIND_REQUIRED)
  ENDIF(NOT OPENAL_FIND_QUIETLY)
ENDIF(NOT OPENAL_FOUND)

MARK_AS_ADVANCED(OPENAL_INCLUDE_DIR OPENAL_LIBRARY)
