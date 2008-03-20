# - Find zlib
# Find the native ZLIB headers and libraries.
# If none is found for Windows use the distributed one in External
#
#  H3DZLIB_INCLUDE_DIR -  where to find zlib.h, etc.
#  H3DZLIB_LIBRARIES    - List of libraries when using zlib.
#  H3DZLIB_FOUND        - True if zlib found.


FIND_PACKAGE(ZLIB)
IF(ZLIB_FOUND)
  SET(H3DZLIB_FOUND 1)
  SET(H3DZLIB_LIBRARIES ${ZLIB_LIBRARIES})
  SET(H3DZLIB_INCLUDE_DIR ${ZLIB_INCLUDE_DIR})
ELSE(ZLIB_FOUND)
  SET(H3DZLIB_FOUND 0)
  SET(H3DZLIB_LIBRARIES)
  SET(H3DZLIB_INCLUDE_DIR)
  
  IF(WIN32)
    # Look for the header file.
    FIND_PATH( H3DZLIB_INCLUDE_DIRS NAMES zlib.h
               PATHS $ENV{H3D_EXTERNAL_ROOT}/include  
                     $ENV{H3D_EXTERNAL_ROOT}/include/zlib
                     ../../External/include    
                     ../../External/include/zlib )
    
    # Look for the library.
    FIND_LIBRARY( H3DZLIB_LIBRARY NAMES zdll 
                  PATHS $ENV{H3D_EXTERNAL_ROOT}/lib
                        ../../External/lib )    
    
    IF(H3DZLIB_INCLUDE_DIRS AND H3DZLIB_LIBRARY)
      SET(H3DZLIB_FOUND 1)
      SET(H3DZLIB_LIBRARIES ${H3DZLIB_LIBRARY})
      SET(H3DZLIB_INCLUDE_DIR ${H3DZLIB_INCLUDE_DIRS})
    ENDIF(H3DZLIB_INCLUDE_DIRS AND H3DZLIB_LIBRARY)
    
  ENDIF(WIN32)
ENDIF(ZLIB_FOUND)

# Report the results.
IF(NOT H3DZLIB_FOUND)
  SET(H3DZLIB_DIR_MESSAGE
    "ZLIB was not found. Make sure ZLIB_LIBRARY and ZLIB_INCLUDE_DIRS are set if compressed files support is desired.")
  IF(NOT H3DZLIB_FIND_QUIETLY)
    MESSAGE(STATUS "${H3DZLIB_DIR_MESSAGE}")
  ELSE(NOT H3DZLIB_FIND_QUIETLY)
    IF(H3DZLIB_FIND_REQUIRED)
      SET(H3DZLIB_DIR_MESSAGE
          "ZLIB was not found. Make sure ZLIB_LIBRARY and ZLIB_INCLUDE_DIRS are set. ZLIB is required to build.")
      MESSAGE(FATAL_ERROR "${H3DZLIB_DIR_MESSAGE}")
    ENDIF(H3DZLIB_FIND_REQUIRED)
  ENDIF(NOT H3DZLIB_FIND_QUIETLY)
ENDIF(NOT H3DZLIB_FOUND)

MARK_AS_ADVANCED(H3DZLIB_INCLUDE_DIRS H3DZLIB_LIBRARY)
