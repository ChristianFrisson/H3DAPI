# - Find zlib
# Find the native ZLIB headers and libraries.
# If none is found for Windows use the distributed one in External
#
#  ZLIB_INCLUDE_DIR -  where to find zlib.h, etc.
#  ZLIB_LIBRARIES    - List of libraries when using zlib.
#  ZLIB_FOUND        - True if zlib found.


IF( WIN32 )
  SET(ZLIB_FIND_QUIETLY 1)
ENDIF( WIN32 )
IF(H3DZLIB_FIND_REQUIRED)
  FIND_PACKAGE(ZLIB REQUIRED)
ELSE(H3DZLIB_FIND_REQUIRED)
  FIND_PACKAGE(ZLIB)
ENDIF(H3DZLIB_FIND_REQUIRED)

IF( NOT ZLIB_FOUND AND WIN32)
  IF( CMAKE_CL_64 )
    SET( LIB "lib64" )
  ELSE( CMAKE_CL_64 )
    SET( LIB "lib32" )
  ENDIF( CMAKE_CL_64 )
  GET_FILENAME_COMPONENT(module_file_path ${CMAKE_CURRENT_LIST_FILE} PATH )
  # Look for the header file.
  FIND_PATH( ZLIB_INCLUDE_DIR NAMES zlib.h
             PATHS $ENV{H3D_EXTERNAL_ROOT}/include  
                   $ENV{H3D_EXTERNAL_ROOT}/include/zlib
                   $ENV{H3D_ROOT}/../External/include  
                   $ENV{H3D_ROOT}/../External/include/zlib
                   ../../External/include
                   ../../External/include/zlib
                   ${module_file_path}/../../../External/include
                   ${module_file_path}/../../../External/include/zlib
             DOC "Path in which the file zlib.h is located." )
  
  # Look for the library.
  FIND_LIBRARY( ZLIB_LIBRARY NAMES zlib 
                PATHS $ENV{H3D_EXTERNAL_ROOT}/${LIB}
                      $ENV{H3D_ROOT}/../External/${LIB}
                      ../../External/${LIB}
                      ${module_file_path}/../../../External/${LIB}
                DOC "Path to zlib library." )
  
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
  IF(H3DZLIB_FIND_REQUIRED)
      SET(LIB_DIR_MESSAGE
          "ZLIB was not found. Make sure ZLIB_LIBRARY and ZLIB_INCLUDE_DIR are set. ZLIB is required to build.")
      MESSAGE(FATAL_ERROR "${ZLIB_DIR_MESSAGE}")
  ELSEIF(NOT H3DZLIB_FIND_QUIETLY)
    MESSAGE(STATUS "${LIB_DIR_MESSAGE}")
  ENDIF(H3DZLIB_FIND_REQUIRED)
ENDIF(NOT ZLIB_FOUND)

MARK_AS_ADVANCED(ZLIB_INCLUDE_DIR ZLIB_LIBRARY)
