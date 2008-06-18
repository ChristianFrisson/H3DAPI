# - Find GLEW
# Find the native GLEW headers and libraries.
#
#  GLEW_INCLUDE_DIR -  where to find GLEW.h, etc.
#  GLEW_LIBRARIES    - List of libraries when using GLEW.
#  GLEW_FOUND        - True if GLEW found.


# Look for the header file.
FIND_PATH(GLEW_INCLUDE_DIR NAMES GL/glew.h
                           PATHS $ENV{H3D_EXTERNAL_ROOT}/include
                                 $ENV{H3D_ROOT}/../External/include
                                 ../../External/include
                                 ${CMAKE_MODULE_PATH}/../../../External/include )
MARK_AS_ADVANCED(GLEW_INCLUDE_DIR)

# Look for the library.
FIND_LIBRARY(GLEW_LIBRARY NAMES GLEW glew32  
                                PATHS $ENV{H3D_EXTERNAL_ROOT}/lib
                                      $ENV{H3D_ROOT}/../External/lib
                                      ../../External/lib
                                      ${CMAKE_MODULE_PATH}/../../../External/lib )
MARK_AS_ADVANCED(GLEW_LIBRARY)

# Copy the results to the output variables.
IF(GLEW_INCLUDE_DIR AND GLEW_LIBRARY)
  SET(GLEW_FOUND 1)
  SET(GLEW_LIBRARIES ${GLEW_LIBRARY})
  SET(GLEW_INCLUDE_DIR ${GLEW_INCLUDE_DIR})
ELSE(GLEW_INCLUDE_DIR AND GLEW_LIBRARY)
  SET(GLEW_FOUND 0)
  SET(GLEW_LIBRARIES)
  SET(GLEW_INCLUDE_DIR)
ENDIF(GLEW_INCLUDE_DIR AND GLEW_LIBRARY)

# Report the results.
IF(NOT GLEW_FOUND)
  SET(GLEW_DIR_MESSAGE
    "GLEW was not found. Make sure GLEW_LIBRARY and GLEW_INCLUDE_DIR are set to where you have your glew header and lib files.")
  IF(GLEW_FIND_REQUIRED)
      MESSAGE(FATAL_ERROR "${GLEW_DIR_MESSAGE}")
  ELSEIF(NOT GLEW_FIND_QUIETLY)
    MESSAGE(STATUS "${GLEW_DIR_MESSAGE}")
  ENDIF(GLEW_FIND_REQUIRED)
ENDIF(NOT GLEW_FOUND)
