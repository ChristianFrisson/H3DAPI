# - Find TEEM
# Find the native TEEM headers and libraries.
#
#  TEEM_INCLUDE_DIR -  where to find TEEM.h, etc.
#  TEEM_LIBRARIES    - List of libraries when using TEEM.
#  TEEM_FOUND        - True if TEEM found.

IF(H3DTEEM_FIND_REQUIRED)
  IF( WIN32 )
    FIND_PACKAGE(TEEM QUIET REQUIRED)
  ELSE( WIN32 )
    FIND_PACKAGE(TEEM REQUIRED)
  ENDIF( WIN32 )
ELSE(H3DTEEM_FIND_REQUIRED)
  IF( WIN32 )
    FIND_PACKAGE(TEEM QUIET)
  ELSE( WIN32 )
    FIND_PACKAGE(TEEM)
  ENDIF( WIN32 )
ENDIF(H3DTEEM_FIND_REQUIRED)

IF( NOT TEEM_FOUND AND WIN32 )
  SET( TEEM_BZIP2 "YES" CACHE BOOL "If teem is built with bz2 support then this variable should be set to ON." )
  SET( TEEM_PNG "NO" CACHE BOOL "If teem is built with png support then this variable should be set to ON." )
  SET( TEEM_ZLIB "YES" CACHE BOOL "If teem is built with zlib support then this variable should be set to ON." )

  MARK_AS_ADVANCED( TEEM_BZIP2 )
  MARK_AS_ADVANCED( TEEM_PNG )
  MARK_AS_ADVANCED( TEEM_ZLIB )

  GET_FILENAME_COMPONENT(module_file_path ${CMAKE_CURRENT_LIST_FILE} PATH )

  IF( CMAKE_CL_64 )
    SET( LIB "lib64" )
  ELSE( CMAKE_CL_64 )
    SET( LIB "lib32" )
  ENDIF( CMAKE_CL_64 )

  # Look for the header file.
  FIND_PATH(TEEM_INCLUDE_DIR NAMES teem/nrrd.h
                             PATHS $ENV{H3D_EXTERNAL_ROOT}/include
                                   $ENV{H3D_ROOT}/../External/include
                                   ../../External/include
                                   /usr/local/include
                                   ${module_file_path}/../../../External/include
                                   ${module_file_path}/../../teem/include
                             DOC "Path in which the file teem/nrrd.h is located." )
  MARK_AS_ADVANCED(TEEM_INCLUDE_DIR)

  # Look for the library.
  FIND_LIBRARY(TEEM_LIBRARY NAMES teem 
                            PATHS $ENV{H3D_EXTERNAL_ROOT}/${LIB}
                                  $ENV{H3D_ROOT}/../External/${LIB}
                                  ../../External/${LIB}
                                  /usr/local/lib
                                  ${module_file_path}/../../../External/${LIB}
                                  ${module_file_path}/../../teem/lib
                            DOC "Path to teem library." )
  MARK_AS_ADVANCED(TEEM_LIBRARY)

  IF( TEEM_LIBRARY )
    SET( FOUND_TEEM_LIBRARIES 1 )
  ELSE( TEEM_LIBRARY )
    SET( FOUND_TEEM_LIBRARIES 0 )
  ENDIF( TEEM_LIBRARY )

  IF( TEEM_ZLIB )
    FIND_PACKAGE(H3DZLIB)
    IF( NOT ZLIB_FOUND )
      SET( FOUND_TEEM_LIBRARIES 0 )
    ENDIF( NOT ZLIB_FOUND )
  ENDIF( TEEM_ZLIB )
  
  IF( TEEM_ZLIB )
    FIND_PACKAGE(H3DZLIB)
    IF( NOT ZLIB_FOUND )
      SET( FOUND_TEEM_LIBRARIES 0 )
    ENDIF( NOT ZLIB_FOUND )
  ENDIF( TEEM_ZLIB )

  IF( TEEM_PNG )
    FIND_PACKAGE(PNG)
    IF( NOT PNG_FOUND )
      SET( FOUND_TEEM_LIBRARIES 0 )
    ENDIF( NOT PNG_FOUND )
  ENDIF( TEEM_PNG )
  
  IF( TEEM_BZIP2 )
    FIND_PACKAGE(H3DBZip2)
    IF( NOT BZIP2_FOUND )
      SET( FOUND_TEEM_LIBRARIES 0 )
    ENDIF( NOT BZIP2_FOUND )
  ENDIF( TEEM_BZIP2 )

  # Copy the results to the output variables.
  IF(TEEM_INCLUDE_DIR AND FOUND_TEEM_LIBRARIES)
    SET(TEEM_FOUND 1)
    SET(TEEM_LIBRARIES ${TEEM_LIBRARY})

    IF( ZLIB_FOUND )
      SET(TEEM_LIBRARIES ${TEEM_LIBRARIES} ${ZLIB_LIBRARIES})
    ENDIF( ZLIB_FOUND )
    IF( PNG_FOUND )
      SET(TEEM_LIBRARIES ${TEEM_LIBRARIES} ${PNG_LIBRARIES})
    ENDIF( PNG_FOUND )
    IF( BZIP2_FOUND )
      SET(TEEM_LIBRARIES ${TEEM_LIBRARIES} ${BZIP2_LIBRARIES})
    ENDIF( BZIP2_FOUND )
    IF( AIR_LIBRARY )
      SET(TEEM_LIBRARIES ${TEEM_LIBRARIES} ${AIR_LIBRARY})
    ENDIF( AIR_LIBRARY )
    IF( BIFF_LIBRARY )
      SET(TEEM_LIBRARIES ${TEEM_LIBRARIES} ${BIFF_LIBRARY})
    ENDIF( BIFF_LIBRARY )

    SET(TEEM_INCLUDE_DIR ${TEEM_INCLUDE_DIR})
  ELSE(TEEM_INCLUDE_DIR AND FOUND_TEEM_LIBRARIES)
    SET(TEEM_FOUND 0)
    SET(TEEM_LIBRARIES)
    SET(TEEM_INCLUDE_DIR)
  ENDIF(TEEM_INCLUDE_DIR AND FOUND_TEEM_LIBRARIES)
ENDIF( NOT TEEM_FOUND AND WIN32 ) 

# Report the results.
IF(NOT TEEM_FOUND)
  SET(TEEM_DIR_MESSAGE
    "TEEM was not found. Make sure TEEM_LIBRARY are set and external libraries required by teem are found.")
  IF(TEEM_FIND_REQUIRED)
    SET( TEEM_DIR_MESSAGE
         "${TEEM_DIR_MESSAGE} Teem is required to build.")
    MESSAGE(FATAL_ERROR "${TEEM_DIR_MESSAGE}")
  ELSEIF(NOT TEEM_FIND_QUIETLY)
    SET( TEEM_DIR_MESSAGE
         "${TEEM_DIR_MESSAGE} If you do not have the libraries you will not be able to load nrrd files.")
    MESSAGE(STATUS "${TEEM_DIR_MESSAGE}")
  ENDIF(TEEM_FIND_REQUIRED)
ENDIF(NOT TEEM_FOUND)
