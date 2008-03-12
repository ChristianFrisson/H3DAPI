# - Find FREEIMAGE
# Find the native FREEIMAGE headers and libraries.
#
#  FREEIMAGE_INCLUDE_DIR -  where to find FREEIMAGE.h, etc.
#  FREEIMAGE_LIBRARIES    - List of libraries when using FREEIMAGE.
#  FREEIMAGE_FOUND        - True if FREEIMAGE found.


# Look for the header file.
FIND_PATH(FREEIMAGE_INCLUDE_DIR NAMES FreeImage.h
                                PATHS $ENV{H3D_EXTERNAL_ROOT}/include
                                      $ENV{H3D_EXTERNAL_ROOT}/include/FreeImage/Dist
                                      ../../External/include
                                      ../../External/include/FreeImage/Dist
)

MARK_AS_ADVANCED(FREEIMAGE_INCLUDE_DIR)

# Look for the library.
FIND_LIBRARY(FREEIMAGE_LIBRARY NAMES freeimage
                               PATHS $ENV{H3D_EXTERNAL_ROOT}/lib
                                     ../../External/lib           
)

MARK_AS_ADVANCED(FREEIMAGE_LIBRARY)

# Copy the results to the output variables.
IF(FREEIMAGE_INCLUDE_DIR AND FREEIMAGE_LIBRARY)
  SET(FREEIMAGE_FOUND 1)
  SET(FREEIMAGE_LIBRARIES ${FREEIMAGE_LIBRARY})
  SET(FREEIMAGE_INCLUDE_DIRS ${FREEIMAGE_INCLUDE_DIR})
ELSE(FREEIMAGE_INCLUDE_DIR AND FREEIMAGE_LIBRARY)
  SET(FREEIMAGE_FOUND 0)
  SET(FREEIMAGE_LIBRARIES)
  SET(FREEIMAGE_INCLUDE_DIRS)
ENDIF(FREEIMAGE_INCLUDE_DIR AND FREEIMAGE_LIBRARY)

# Report the results.
IF(NOT FREEIMAGE_FOUND)
  SET(FREEIMAGE_DIR_MESSAGE
    "FREEIMAGE was not found. Make sure FREEIMAGE_LIBRARY and FREEIMAGE_INCLUDE_DIR are set to the directory of your FreeImage installation. If you do not have it many image formats will not be available to use as textures")
  IF(NOT FREEIMAGE_FIND_QUIETLY)
    MESSAGE(STATUS "${FREEIMAGE_DIR_MESSAGE}")
  ELSE(NOT FREEIMAGE_FIND_QUIETLY)
    IF(FREEIMAGE_FIND_REQUIRED)
      MESSAGE(FATAL_ERROR "${FREEIMAGE_DIR_MESSAGE}")
    ENDIF(FREEIMAGE_FIND_REQUIRED)
  ENDIF(NOT FREEIMAGE_FIND_QUIETLY)
ENDIF(NOT FREEIMAGE_FOUND)
