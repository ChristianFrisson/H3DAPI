# - Find zlib
# Find the native ZLIB headers and libraries.
# If none is found for Windows use the distributed one in External
#
#  ZLIB_INCLUDE_DIR -  where to find zlib.h, etc.
#  ZLIB_LIBRARIES    - List of libraries when using zlib.
#  ZLIB_FOUND        - True if zlib found.


FIND_PACKAGE(ZLIB)
IF( NOT ZLIB_FOUND AND WIN32)
  # Look for the header file.
  FIND_PATH( ZLIB_INCLUDE_DIR NAMES zlib.h
             PATHS $ENV{H3D_EXTERNAL_ROOT}/include  
                   $ENV{H3D_EXTERNAL_ROOT}/include/zlib
                   ../../External/include    
                   ../../External/include/zlib )
  
  # Look for the library.
  FIND_LIBRARY( ZLIB_LIBRARY NAMES zdll 
                PATHS $ENV{H3D_EXTERNAL_ROOT}/lib
                      ../../External/lib )    
  
  IF(ZLIB_INCLUDE_DIR AND ZLIB_LIBRARY)
    SET(ZLIB_FOUND 1)
    SET(ZLIB_LIBRARIES ${ZLIB_LIBRARY})
    SET(ZLIB_INCLUDE_DIR ${ZLIB_INCLUDE_DIR} )
  ENDIF(ZLIB_INCLUDE_DIR AND ZLIB_LIBRARY)
ENDIF( NOT ZLIB_FOUND AND WIN32)

# Report the results.
IF(NOT ZLIB_FOUND)
  SET(ZLIB_DIR_MESSAGE
    "ZLIB was not found. Make sure ZLIB_LIBRARY and ZLIB_INCLUDE_DIR are set if compressed files support is desired.")
  IF(ZLIB_FIND_REQUIRED)
      SET(LIB_DIR_MESSAGE
          "ZLIB was not found. Make sure ZLIB_LIBRARY and ZLIB_INCLUDE_DIR are set. ZLIB is required to build.")
      MESSAGE(FATAL_ERROR "${ZLIB_DIR_MESSAGE}")
  ELSEIF(NOT ZLIB_FIND_QUIETLY)
    MESSAGE(STATUS "${LIB_DIR_MESSAGE}")
  ENDIF(ZLIB_FIND_REQUIRED)
ENDIF(NOT ZLIB_FOUND)

MARK_AS_ADVANCED(ZLIB_INCLUDE_DIR ZLIB_LIBRARY)
