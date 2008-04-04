# - Find FTGL
# Find the native FTGL headers and libraries.
#
#  FTGL_INCLUDE_DIR -  where to find FTGL.h, etc.
#  FTGL_LIBRARIES    - List of libraries when using FTGL.
#  FTGL_FOUND        - True if FTGL found.


# Look for the header file.
FIND_PATH(FTGL_INCLUDE_DIR NAMES FTGL/FTGL.h 
                           PATHS $ENV{H3D_EXTERNAL_ROOT}/include  
                                 ../../External/include)
MARK_AS_ADVANCED(FTGL_INCLUDE_DIR)

# Look for the library.
FIND_LIBRARY(FTGL_LIBRARY NAMES ftgl ftgl_dynamic_MTD
                          PATHS $ENV{H3D_EXTERNAL_ROOT}/lib
                                ../../External/lib)
MARK_AS_ADVANCED(FTGL_LIBRARY)

# Copy the results to the output variables.
IF(FTGL_INCLUDE_DIR AND FTGL_LIBRARY)
  SET(FTGL_FOUND 1)
  SET(FTGL_LIBRARIES ${FTGL_LIBRARY})
  SET(FTGL_INCLUDE_DIR ${FTGL_INCLUDE_DIR})
ELSE(FTGL_INCLUDE_DIR AND FTGL_LIBRARY)
  SET(FTGL_FOUND 0)
  SET(FTGL_LIBRARIES)
  SET(FTGL_INCLUDE_DIR)
ENDIF(FTGL_INCLUDE_DIR AND FTGL_LIBRARY)

# Report the results.
IF(NOT FTGL_FOUND)
  SET(FTGL_DIR_MESSAGE
    "FTGL was not found. Make sure FTGL_LIBRARY and FTGL_INCLUDE_DIR are set to the directories containing the include and lib files for FTGL. If you do not have the library you will not be able to use the Text node.")
  IF(NOT FTGL_FIND_QUIETLY)
    MESSAGE(STATUS "${FTGL_DIR_MESSAGE}")
  ELSE(NOT FTGL_FIND_QUIETLY)
    IF(FTGL_FIND_REQUIRED)
      MESSAGE(FATAL_ERROR "${FTGL_DIR_MESSAGE}")
    ENDIF(FTGL_FIND_REQUIRED)
  ENDIF(NOT FTGL_FIND_QUIETLY)
ENDIF(NOT FTGL_FOUND)
