# - Find FREETYPE
# Find the native FREETYPE headers and libraries.
#
#  FREETYPE_INCLUDE_DIR -  where to find FREETYPE.h, etc.
#  FREETYPE_LIBRARIES    - List of libraries when using FREETYPE.
#  FREETYPE_FOUND        - True if FREETYPE found.


# Look for the header file.
FIND_PATH(FREETYPE_INCLUDE_DIR NAMES freetype/freetype.h
                              PATHS $ENV{H3D_EXTERNAL_ROOT}/include  
                                    $ENV{H3D_EXTERNAL_ROOT}/include/freetype/include
                                    ../../External/include    
                                    ../../External/include/freetype/include)
MARK_AS_ADVANCED(FREETYPE_INCLUDE_DIR)

# Look for the library.
FIND_LIBRARY(FREETYPE_LIBRARY NAMES freetype freetype235
                              PATHS $ENV{H3D_EXTERNAL_ROOT}/lib
                                    ../../External/lib )
MARK_AS_ADVANCED(FREETYPE_LIBRARY)

# Copy the results to the output variables.
IF(FREETYPE_INCLUDE_DIR AND FREETYPE_LIBRARY)
  SET(FREETYPE_FOUND 1)
  SET(FREETYPE_LIBRARIES ${FREETYPE_LIBRARY})
  SET(FREETYPE_INCLUDE_DIR ${FREETYPE_INCLUDE_DIR})
ELSE(FREETYPE_INCLUDE_DIR AND FREETYPE_LIBRARY)
  SET(FREETYPE_FOUND 0)
  SET(FREETYPE_LIBRARIES)
  SET(FREETYPE_INCLUDE_DIR)
ENDIF(FREETYPE_INCLUDE_DIR AND FREETYPE_LIBRARY)

# Report the results.
IF(NOT FREETYPE_FOUND)
  SET(FREETYPE_DIR_MESSAGE
    "FREETYPE was not found. Make sure FREETYPE_LIBRARY and FREETYPE_INCLUDE_DIR are set to the directories containing the include and lib files for freetype. If you do not have it you will not be able to use the Text node.")
  IF(FREETYPE_FIND_REQUIRED)
    MESSAGE(FATAL_ERROR "${FREETYPE_DIR_MESSAGE}")
  ELSEIF(NOT FREETYPE_FIND_QUIETLY)
    MESSAGE(STATUS "${FREETYPE_DIR_MESSAGE}")
  ENDIF(FREETYPE_FIND_REQUIRED)
ENDIF(NOT FREETYPE_FOUND)
