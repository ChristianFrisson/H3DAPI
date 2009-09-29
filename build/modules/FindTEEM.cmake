# - Find TEEM
# Find the native TEEM headers and libraries.
#
#  TEEM_INCLUDE_DIR -  where to find TEEM.h, etc.
#  TEEM_LIBRARIES    - List of libraries when using TEEM.
#  TEEM_FOUND        - True if TEEM found.
#  
#  In the future when the teem library fixes their CMakeLists.txt so that
#  the moss library is installed correctly then it is possible to rename
#  this module into FindTeemH3D (or something) and use the syntax below
#  to try to find TEEM. The CMakeLists.txt for MedX3D would have to be changed
#  to call the new module name instead to avoid conflicts.
#  FIND_PATH(TEEM_DIR NAMES TEEMUse.cmake
#                   PATHS /usr/local/lib/TEEM-1.9 )
#  MARK_AS_ADVANCED(TEEM_DIR)
#
#  IF( TEEM_DIR )
#    FIND_PACKAGE( TEEM )
#    IF( TEEM_FOUND )
#      INCLUDE( ${TEEM_USE_FILE} )
#    ELSE( TEEM_FOUND )
#      #Try to find libraries like the code below.
#    ENDIF( TEEM_FOUND
#  ENDIF( TEEM_DIR )
#
#
#

IF( NOT WIN32 AND UNIX )
  SET( TEEM_BZIP2 "NO" CACHE BOOL "If teem is built with bz2 support then this variable should be set to ON." )
  SET( TEEM_PNG "NO" CACHE BOOL "If teem is built with png support then this variable should be set to ON." )
  SET( TEEM_ZLIB "NO" CACHE BOOL "If teem is built with zlib support then this variable should be set to ON." )
ELSE(  NOT WIN32 AND UNIX )
  SET( TEEM_BZIP2 "YES" CACHE BOOL "If teem is built with bz2 support then this variable should be set to ON." )
  SET( TEEM_PNG "YES" CACHE BOOL "If teem is built with png support then this variable should be set to ON." )
  SET( TEEM_ZLIB "YES" CACHE BOOL "If teem is built with zlib support then this variable should be set to ON." )
ENDIF( NOT WIN32 AND UNIX )
MARK_AS_ADVANCED( TEEM_BZIP2 )
MARK_AS_ADVANCED( TEEM_PNG )
MARK_AS_ADVANCED( TEEM_ZLIB )

GET_FILENAME_COMPONENT(module_file_path ${CMAKE_CURRENT_LIST_FILE} PATH )

# Look for the header file.
FIND_PATH(TEEM_INCLUDE_DIR NAMES teem/nrrd.h
                           PATHS $ENV{H3D_EXTERNAL_ROOT}/include
                                 $ENV{H3D_ROOT}/../External/include
                                 ../../External/include
                                 /usr/local/include
                                 ${module_file_path}/../../../External/include
                                 ${module_file_path}/../../teem/include )
MARK_AS_ADVANCED(TEEM_INCLUDE_DIR)

# Look for the library.
FIND_LIBRARY(TEEM_LIBRARY NAMES teem 
                          PATHS $ENV{H3D_EXTERNAL_ROOT}/lib
                                $ENV{H3D_ROOT}/../External/lib
                                ../../External/lib
                                /usr/local/lib
                                ${module_file_path}/../../../External/lib
                                ${module_file_path}/../../teem/lib )
MARK_AS_ADVANCED(TEEM_LIBRARY)

IF( TEEM_LIBRARY )
  SET( FOUND_TEEM_LIBRARIES 1 )
ELSE( TEEM_LIBRARY )
  SET( FOUND_TEEM_LIBRARIES 0 )
ENDIF( TEEM_LIBRARY )

IF( TEEM_ZLIB )
  FIND_LIBRARY(Z_LIBRARY NAMES z
                         PATHS $ENV{H3D_EXTERNAL_ROOT}/lib
                               $ENV{H3D_ROOT}/../External/lib
                               ../../External/lib
                               /usr/lib
                               ${module_file_path}/../../../External/lib
                               ${module_file_path}/../../teem/lib )
  MARK_AS_ADVANCED(Z_LIBRARY)
  IF( NOT Z_LIBRARY )
    SET( FOUND_TEEM_LIBRARIES 0 )
  ENDIF( NOT Z_LIBRARY )
ENDIF( TEEM_ZLIB )

IF( TEEM_PNG )
  FIND_LIBRARY(PNG_LIBRARY NAMES png
                           PATHS $ENV{H3D_EXTERNAL_ROOT}/lib
                                 $ENV{H3D_ROOT}/../External/lib
                                  ../../External/lib
                                  /usr/lib
                                  ${module_file_path}/../../../External/lib
                                  ${module_file_path}/../../teem/lib )
  MARK_AS_ADVANCED(PNG_LIBRARY)
  IF( NOT PNG_LIBRARY )
    SET( FOUND_TEEM_LIBRARIES 0 )
  ENDIF( NOT PNG_LIBRARY )
ENDIF( TEEM_PNG )

IF( TEEM_BZIP2 )
  FIND_LIBRARY(BZ2_LIBRARY NAMES bz2
                           PATHS $ENV{H3D_EXTERNAL_ROOT}/lib
                                 $ENV{H3D_ROOT}/../External/lib
                                 ../../External/lib
                                 /usr/lib
                                 ${module_file_path}/../../../External/lib
                                 ${module_file_path}/../../teem/lib )
  MARK_AS_ADVANCED(BZ2_LIBRARY)
  IF( NOT BZ2_LIBRARY )
    SET( FOUND_TEEM_LIBRARIES 0 )
  ENDIF( NOT BZ2_LIBRARY )
ENDIF( TEEM_BZIP2 )

# Copy the results to the output variables.
IF(TEEM_INCLUDE_DIR AND FOUND_TEEM_LIBRARIES)
  SET(TEEM_FOUND 1)
  SET(TEEM_LIBRARIES ${TEEM_LIBRARY})

  IF( Z_LIBRARY )
    SET(TEEM_LIBRARIES ${TEEM_LIBRARIES} ${Z_LIBRARY})
  ENDIF( Z_LIBRARY )
  IF( PNG_LIBRARY )
    SET(TEEM_LIBRARIES ${TEEM_LIBRARIES} ${PNG_LIBRARY})
  ENDIF( PNG_LIBRARY )
  IF( BZ2_LIBRARY )
    SET(TEEM_LIBRARIES ${TEEM_LIBRARIES} ${BZ2_LIBRARY})
  ENDIF( BZ2_LIBRARY )
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

# Report the results.
IF(NOT TEEM_FOUND)
  SET(TEEM_DIR_MESSAGE
    "TEEM was not found. Make sure TEEM_LIBRARY, PNG_LIBRARY, Z_LIBRARY, BZ2_LIBRARY and TEEM_INCLUDE_DIR are set to where you have your teem header and different lib files. If you do not have the libraries you will not be able to use the NrrdImageLoader node. Some of the libraries only need to be set if teem should have support for those features.")
  IF(TEEM_FIND_REQUIRED)
      MESSAGE(FATAL_ERROR "${TEEM_DIR_MESSAGE}")
  ELSEIF(NOT TEEM_FIND_QUIETLY)
    MESSAGE(STATUS "${TEEM_DIR_MESSAGE}")
  ENDIF(TEEM_FIND_REQUIRED)
ENDIF(NOT TEEM_FOUND)
