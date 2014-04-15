# - Find TEEM
# Find the native TEEM headers and libraries.
#
#  Teem_INCLUDE_DIR -  where to find Teem.h, etc.
#  Teem_LIBRARIES    - List of libraries when using Teem.
#  Teem_FOUND        - True if Teem found.
IF( NOT WIN32 )
  SET( Teem_NO_LIBRARY_DEPENDS "YES" )
ENDIF( NOT WIN32 )
IF(H3DTeem_FIND_REQUIRED)
    FIND_PACKAGE(Teem QUIET REQUIRED)
ELSE(H3DTeem_FIND_REQUIRED)
    FIND_PACKAGE(Teem QUIET)
ENDIF(H3DTeem_FIND_REQUIRED)

IF( Teem_FOUND AND Teem_USE_FILE )
  # We do not use Teem_USE_FILE simply because it uses LINK_DIRECTORIES which is
  # discouraged nowadays. So we use the TeemConfig.cmake definitions to create
  # Teem_LIBRARIES and Teem_INCLUDE_DIR. If Teem_USE_FILE is ever updated we can
  # check the Teem_VERSION* cmake variables to update this module.
  # include( ${Teem_USE_FILE} ) should be the normal way to do this.
  # On windows Teem_USE_FILE does not work at all due to it not handling Release and Debug
  # sub directories.
  SET( Teem_INCLUDE_DIR ${Teem_INCLUDE_DIRS} )
  SET( Teem_LIBRARIES_tmp ${Teem_LIBRARIES} )
  # Do we need to loop here? In that case beware of optimized and debug keywords.
  IF( EXISTS "${Teem_LIBRARY_DIRS}/lib${Teem_LIBRARIES}.so" )
    SET( Teem_LIBRARIES "${Teem_LIBRARY_DIRS}/lib${Teem_LIBRARIES}.so" )
  ELSEIF( EXISTS "${Teem_LIBRARY_DIRS}/lib${Teem_LIBRARIES}.dylib" )
    SET( Teem_LIBRARIES "${Teem_LIBRARY_DIRS}/lib${Teem_LIBRARIES}.dylib" )
  ELSE( EXISTS "${Teem_LIBRARY_DIRS}/lib${Teem_LIBRARIES}.so" )
    # The library version is now assumed to be in a default location so we can actually use the UseFile.
    INCLUDE( ${Teem_USE_FILE} )
  ENDIF( EXISTS "${Teem_LIBRARY_DIRS}/lib${Teem_LIBRARIES}.so" )
ELSE( Teem_FOUND AND Teem_USE_FILE )
  SET( Teem_PNG "NO" CACHE BOOL "If teem is built with png support then this variable should be set to ON." )
  IF( WIN32 )
    SET( Teem_BZIP2 "YES" CACHE BOOL "If teem is built with bz2 support then this variable should be set to ON." )
    SET( Teem_ZLIB "YES" CACHE BOOL "If teem is built with zlib support then this variable should be set to ON." )
  ELSE( WIN32 )
    SET( Teem_BZIP2 "NO" CACHE BOOL "If teem is built with bz2 support then this variable should be set to ON." )
    SET( Teem_ZLIB "NO" CACHE BOOL "If teem is built with zlib support then this variable should be set to ON." )
  ENDIF( WIN32 )

  MARK_AS_ADVANCED( Teem_BZIP2 )
  MARK_AS_ADVANCED( Teem_PNG )
  MARK_AS_ADVANCED( Teem_ZLIB )

  GET_FILENAME_COMPONENT(module_file_path ${CMAKE_CURRENT_LIST_FILE} PATH )

  IF( CMAKE_CL_64 )
    SET( LIB "lib64" )
  ELSE( CMAKE_CL_64 )
    SET( LIB "lib32" )
  ENDIF( CMAKE_CL_64 )

  # Look for the header file.
  FIND_PATH(Teem_INCLUDE_DIR NAMES teem/nrrd.h
                             PATHS $ENV{H3D_EXTERNAL_ROOT}/include
                                   $ENV{H3D_ROOT}/../External/include
                                   ../../External/include
                                   /usr/local/include
                                   ${module_file_path}/../../../External/include
                                   ${module_file_path}/../../teem/include
                             DOC "Path in which the file teem/nrrd.h is located." )
  MARK_AS_ADVANCED(Teem_INCLUDE_DIR)

  # Look for the library.
  FIND_LIBRARY(Teem_LIBRARY NAMES teem 
                            PATHS $ENV{H3D_EXTERNAL_ROOT}/${LIB}
                                  $ENV{H3D_ROOT}/../External/${LIB}
                                  ../../External/${LIB}
                                  /usr/local/lib
                                  ${module_file_path}/../../../External/${LIB}
                                  ${module_file_path}/../../teem/lib
                            DOC "Path to teem library." )
  MARK_AS_ADVANCED(Teem_LIBRARY)

  IF( Teem_LIBRARY )
    SET( FOUND_Teem_LIBRARIES 1 )
  ELSE( Teem_LIBRARY )
    SET( FOUND_Teem_LIBRARIES 0 )
  ENDIF( Teem_LIBRARY )

  IF( Teem_ZLIB )
    FIND_PACKAGE(H3DZLIB)
    IF( NOT ZLIB_FOUND )
      SET( FOUND_Teem_LIBRARIES 0 )
    ENDIF( NOT ZLIB_FOUND )
  ENDIF( Teem_ZLIB )
  
  IF( Teem_ZLIB )
    FIND_PACKAGE(H3DZLIB)
    IF( NOT ZLIB_FOUND )
      SET( FOUND_Teem_LIBRARIES 0 )
    ENDIF( NOT ZLIB_FOUND )
  ENDIF( Teem_ZLIB )

  IF( Teem_PNG )
    FIND_PACKAGE(PNG)
    IF( NOT PNG_FOUND )
      SET( FOUND_Teem_LIBRARIES 0 )
    ENDIF( NOT PNG_FOUND )
  ENDIF( Teem_PNG )
  
  IF( Teem_BZIP2 )
    FIND_PACKAGE(H3DBZip2)
    IF( NOT BZIP2_FOUND )
      SET( FOUND_Teem_LIBRARIES 0 )
    ENDIF( NOT BZIP2_FOUND )
  ENDIF( Teem_BZIP2 )

  # Copy the results to the output variables.
  IF(Teem_INCLUDE_DIR AND FOUND_Teem_LIBRARIES)
    SET(Teem_FOUND 1)
    SET(Teem_LIBRARIES ${Teem_LIBRARY})

    IF( ZLIB_FOUND )
      SET(Teem_LIBRARIES ${Teem_LIBRARIES} ${ZLIB_LIBRARIES})
    ENDIF( ZLIB_FOUND )
    IF( PNG_FOUND )
      SET(Teem_LIBRARIES ${Teem_LIBRARIES} ${PNG_LIBRARIES})
    ENDIF( PNG_FOUND )
    IF( BZIP2_FOUND )
      SET(Teem_LIBRARIES ${Teem_LIBRARIES} ${BZIP2_LIBRARIES})
    ENDIF( BZIP2_FOUND )
    IF( AIR_LIBRARY )
      SET(Teem_LIBRARIES ${Teem_LIBRARIES} ${AIR_LIBRARY})
    ENDIF( AIR_LIBRARY )
    IF( BIFF_LIBRARY )
      SET(Teem_LIBRARIES ${Teem_LIBRARIES} ${BIFF_LIBRARY})
    ENDIF( BIFF_LIBRARY )

    SET(Teem_INCLUDE_DIR ${Teem_INCLUDE_DIR})
  ELSE(Teem_INCLUDE_DIR AND FOUND_Teem_LIBRARIES)
    SET(Teem_FOUND 0)
    SET(Teem_LIBRARIES)
    SET(Teem_INCLUDE_DIR)
  ENDIF(Teem_INCLUDE_DIR AND FOUND_Teem_LIBRARIES)
ENDIF( Teem_FOUND AND Teem_USE_FILE )

# Report the results.
IF(NOT Teem_FOUND)
  SET(Teem_DIR_MESSAGE
    "Teem was not found. Make sure Teem_LIBRARY are set and external libraries required by teem are found.")
  IF(Teem_FIND_REQUIRED)
    SET( Teem_DIR_MESSAGE
         "${Teem_DIR_MESSAGE} Teem is required to build.")
    MESSAGE(FATAL_ERROR "${Teem_DIR_MESSAGE}")
  ELSEIF(NOT Teem_FIND_QUIETLY)
    SET( Teem_DIR_MESSAGE
         "${Teem_DIR_MESSAGE} If you do not have the libraries you will not be able to load nrrd files.")
    MESSAGE(STATUS "${Teem_DIR_MESSAGE}")
  ENDIF(Teem_FIND_REQUIRED)
ENDIF(NOT Teem_FOUND)
