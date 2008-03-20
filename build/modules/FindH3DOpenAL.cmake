# - Find OpenAL
# Find the native OpenAL headers and libraries.
# If none is found for Windows use the distributed one in External
#
#  H3DOPENAL_INCLUDE_DIR -  where to find al.h, etc.
#  H3DOPENAL_LIBRARIES    - List of libraries when using OpenAL.
#  H3DOPENAL_FOUND        - True if OpenAL found.


FIND_PACKAGE(OpenAL)
IF(OPENAL_FOUND)
  SET(H3DOPENAL_FOUND 1)
  SET(H3DOPENAL_LIBRARIES ${OPENAL_LIBRARY})
  SET(H3DOPENAL_INCLUDE_DIR ${OPENAL_INCLUDE_DIR})
ELSE(OPENAL_FOUND)
  SET(H3DOPENAL_FOUND 0)
  SET(H3DOPENAL_LIBRARIES)
  SET(H3DOPENAL_INCLUDE_DIR)
  
  IF(WIN32)
    # Look for the header file.
    FIND_PATH( H3DOPENAL_INCLUDE_DIRS NAMES al.h
               PATHS $ENV{H3D_EXTERNAL_ROOT}/include  
                     $ENV{H3D_EXTERNAL_ROOT}/include/OpenAL
                     ../../External/include    
                     ../../External/include/OpenAL )
    
    # Look for the library.
    FIND_LIBRARY( H3DOPENAL_LIBRARY NAMES OpenAL32
                  PATHS $ENV{H3D_EXTERNAL_ROOT}/lib
                        ../../External/lib )    
    
    IF(H3DOPENAL_INCLUDE_DIRS AND H3DOPENAL_LIBRARY)
      SET(H3DOPENAL_FOUND 1)
      SET(H3DOPENAL_LIBRARIES ${H3DOPENAL_LIBRARY})
      SET(H3DOPENAL_INCLUDE_DIR ${H3DOPENAL_INCLUDE_DIRS})
    ENDIF(H3DOPENAL_INCLUDE_DIRS AND H3DOPENAL_LIBRARY)
    
  ENDIF(WIN32)
ENDIF(OPENAL_FOUND)

# Report the results.
IF(NOT H3DOPENAL_FOUND)
  SET(H3DOPENAL_DIR_MESSAGE
    "OpenAL was not found. Make sure OPENAL_LIBRARY and OPENAL_INCLUDE_DIRS are set if compressed files support is desired.")
  IF(NOT H3DOPENAL_FIND_QUIETLY)
    MESSAGE(STATUS "${H3DOPENAL_DIR_MESSAGE}")
  ELSE(NOT H3DOPENAL_FIND_QUIETLY)
    IF(H3DOPENAL_FIND_REQUIRED)
      SET(H3DOPENAL_DIR_MESSAGE
          "OpenAL was not found. Make sure OPENAL_LIBRARY and OPENAL_INCLUDE_DIRS are set. OpenAL is required to build.")
      MESSAGE(FATAL_ERROR "${H3DOPENAL_DIR_MESSAGE}")
    ENDIF(H3DOPENAL_FIND_REQUIRED)
  ENDIF(NOT H3DOPENAL_FIND_QUIETLY)
ENDIF(NOT H3DOPENAL_FOUND)

MARK_AS_ADVANCED(H3DOPENAL_INCLUDE_DIRS H3DOPENAL_LIBRARY OPENAL_INCLUDE_DIR OPENAL_LIBRARY)
