# - Find FREEIMAGE
# Find the native FREEIMAGE headers and libraries.
#
#  FREEIMAGE_INCLUDE_DIR -  where to find FREEIMAGE.h, etc.
#  FREEIMAGE_LIBRARIES    - List of libraries when using FREEIMAGE.
#  FREEIMAGE_FOUND        - True if FREEIMAGE found.

GET_FILENAME_COMPONENT(module_file_path ${CMAKE_CURRENT_LIST_FILE} PATH )

IF( CMAKE_CL_64 )
  SET( LIB "lib64" )
ELSE( CMAKE_CL_64 )
  SET( LIB "lib32" )
ENDIF( CMAKE_CL_64 )

# Look for the header file.
FIND_PATH(FREEIMAGE_INCLUDE_DIR NAMES FreeImage.h
                                PATHS $ENV{H3D_EXTERNAL_ROOT}/include
                                      $ENV{H3D_EXTERNAL_ROOT}/include/FreeImage/Dist
                                      $ENV{H3D_ROOT}/../External/include
                                      $ENV{H3D_ROOT}/../External/include/FreeImage/Dist
                                      ../../External/include
                                      ../../External/include/FreeImage/Dist
                                      ${module_file_path}/../../../External/include
                                      ${module_file_path}/../../../External/include/FreeImage/Dist
                                DOC "Path in which the file FreeImage.h is located." )

MARK_AS_ADVANCED(FREEIMAGE_INCLUDE_DIR)

# Look for the library.
FIND_LIBRARY(FREEIMAGE_LIBRARY NAMES freeimage
                               PATHS $ENV{H3D_EXTERNAL_ROOT}/${LIB}
                                     $ENV{H3D_ROOT}/../External/${LIB}
                                     ../../External/${LIB}
                                     ${module_file_path}/../../../External/${LIB}
                               DOC "Path to freeimage library." )
MARK_AS_ADVANCED(FREEIMAGE_LIBRARY)

IF( WIN32 AND PREFER_STATIC_LIBRARIES )
  SET( FREEIMAGE_STATIC_LIBRARY_NAME FreeImage_vc7 )
  
  IF( MSVC80 )
    SET( FREEIMAGE_STATIC_LIBRARY_NAME FreeImage_vc8 )
  ELSEIF( MSVC90 )
    SET( FREEIMAGE_STATIC_LIBRARY_NAME FreeImage_vc9 )
  ELSEIF( MSVC10 )
    SET( FREEIMAGE_STATIC_LIBRARY_NAME FreeImage_vc10 )
  ENDIF( MSVC80 )
  
  FIND_LIBRARY( FREEIMAGE_STATIC_LIBRARY NAMES ${FREEIMAGE_STATIC_LIBRARY_NAME}
                                         PATHS $ENV{H3D_EXTERNAL_ROOT}/${LIB}/static
                                               $ENV{H3D_ROOT}/../External/${LIB}/static
                                               ../../External/${LIB}/static
                                               ${module_file_path}/../../../External/${LIB}/static
                                         DOC "Path to freeimage static release library (windows only). For this configuration it might be called ${FREEIMAGE_STATIC_LIBRARY_NAME}" )
  MARK_AS_ADVANCED(FREEIMAGE_STATIC_LIBRARY)
  
  IF( FREEIMAGE_STATIC_LIBRARY )
    SET( FREEIMAGE_STATIC_LIBRARIES_FOUND 1 )
  ENDIF( FREEIMAGE_STATIC_LIBRARY )
  
  FIND_LIBRARY( FREEIMAGE_STATIC_DEBUG_LIBRARY NAMES ${FREEIMAGE_STATIC_LIBRARY_NAME}_d
                                               PATHS $ENV{H3D_EXTERNAL_ROOT}/${LIB}/static
                                                     $ENV{H3D_ROOT}/../External/${LIB}/static
                                                     ../../External/${LIB}/static
                                                     ${module_file_path}/../../../External/${LIB}/static
                                               DOC "Path to freeimage static debug library (windows only). For this configuration it might be called ${FREEIMAGE_STATIC_LIBRARY_NAME}_d" )
  MARK_AS_ADVANCED(FREEIMAGE_STATIC_DEBUG_LIBRARY)
    
  IF( FREEIMAGE_STATIC_DEBUG_LIBRARY )
    SET( FREEIMAGE_STATIC_LIBRARIES_FOUND 1 )
  ENDIF( FREEIMAGE_STATIC_DEBUG_LIBRARY )
ENDIF( WIN32 AND PREFER_STATIC_LIBRARIES )

IF( FREEIMAGE_LIBRARY OR FREEIMAGE_STATIC_LIBRARIES_FOUND )
  SET( FREEIMAGE_LIBRARIES_FOUND 1 )
ENDIF( FREEIMAGE_LIBRARY OR FREEIMAGE_STATIC_LIBRARIES_FOUND )

# Copy the results to the output variables.
IF(FREEIMAGE_INCLUDE_DIR AND FREEIMAGE_LIBRARIES_FOUND)
  SET(FREEIMAGE_FOUND 1)

  IF( WIN32 AND PREFER_STATIC_LIBRARIES AND FREEIMAGE_STATIC_LIBRARIES_FOUND )
    IF(FREEIMAGE_STATIC_LIBRARY)
      SET(FREEIMAGE_LIBRARIES optimized ${FREEIMAGE_STATIC_LIBRARY} )
    ELSE(FREEIMAGE_STATIC_LIBRARY)
      SET(FREEIMAGE_LIBRARIES optimized ${FREEIMAGE_STATIC_LIBRARY_NAME} )
      MESSAGE( STATUS "FreeImage static release libraries not found. Release build might not work." )
    ENDIF(FREEIMAGE_STATIC_LIBRARY)

    IF(FREEIMAGE_STATIC_DEBUG_LIBRARY)
      SET(FREEIMAGE_LIBRARIES ${FREEIMAGE_LIBRARIES} debug ${FREEIMAGE_STATIC_DEBUG_LIBRARY} )
    ELSE(FREEIMAGE_STATIC_DEBUG_LIBRARY)
      SET(FREEIMAGE_LIBRARIES ${FREEIMAGE_LIBRARIES} debug ${FREEIMAGE_STATIC_LIBRARY_NAME}_d )
      MESSAGE( STATUS "FreeImage static debug libraries not found. Debug build might not work." )
    ENDIF(FREEIMAGE_STATIC_DEBUG_LIBRARY)

    SET( FREEIMAGE_LIB 1 )
  ELSE( WIN32 AND PREFER_STATIC_LIBRARIES AND FREEIMAGE_STATIC_LIBRARIES_FOUND )
    SET(FREEIMAGE_LIBRARIES ${FREEIMAGE_LIBRARY})
  ENDIF( WIN32 AND PREFER_STATIC_LIBRARIES AND FREEIMAGE_STATIC_LIBRARIES_FOUND )

  SET(FREEIMAGE_INCLUDE_DIR ${FREEIMAGE_INCLUDE_DIR})
ELSE(FREEIMAGE_INCLUDE_DIR AND FREEIMAGE_LIBRARIES_FOUND)
  SET(FREEIMAGE_FOUND 0)
  SET(FREEIMAGE_LIBRARIES)
  SET(FREEIMAGE_INCLUDE_DIR)
ENDIF(FREEIMAGE_INCLUDE_DIR AND FREEIMAGE_LIBRARIES_FOUND)

# Report the results.
IF(NOT FREEIMAGE_FOUND)
  SET( FREEIMAGE_DIR_MESSAGE
       "FREEIMAGE was not found. Make sure FREEIMAGE_LIBRARY")
  IF( WIN32 )
  SET( FREEIMAGE_DIR_MESSAGE
       "${FREEIMAGE_DIR_MESSAGE} ( and/or FREEIMAGE_STATIC_LIBRARY/FREEIMAGE_STATIC_DEBUG_LIBRARY )")
  ENDIF( WIN32 )
  SET( FREEIMAGE_DIR_MESSAGE
       "${FREEIMAGE_DIR_MESSAGE} and FREEIMAGE_INCLUDE_DIR are set.")
  IF(FreeImage_FIND_REQUIRED)
    SET( FREEIMAGE_DIR_MESSAGE
         "${FREEIMAGE_DIR_MESSAGE} FREEIMAGE is required to build.")
    MESSAGE(FATAL_ERROR "${FREEIMAGE_DIR_MESSAGE}")
  ELSEIF(NOT FreeImage_FIND_QUIETLY)
    SET( FREEIMAGE_DIR_MESSAGE
         "${FREEIMAGE_DIR_MESSAGE} If you do not have it many image formats will not be available to use as textures.")
    MESSAGE(STATUS "${FREEIMAGE_DIR_MESSAGE}")
  ENDIF(FreeImage_FIND_REQUIRED)
ENDIF(NOT FREEIMAGE_FOUND)
