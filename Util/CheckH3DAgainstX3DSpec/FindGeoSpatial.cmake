# - Find UI
# Find the native UI headers and libraries.
#
#  GeoSpatial_INCLUDE_DIR -  where to find UI.h, etc.
#  GeoSpatial_LIBRARIES    - List of libraries when using UI.
#  GeoSpatial_FOUND        - True if UI found.

GET_FILENAME_COMPONENT(module_file_path ${CMAKE_CURRENT_LIST_FILE} PATH )

# Look for the header file.
FIND_PATH( GeoSpatial_INCLUDE_DIR NAMES H3D/GeoSpatial/GeoSpatial.h
           PATHS $ENV{H3D_ROOT}/../GeoSpatial/include
                 ../../../GeoSpatial/include
                 ${module_file_path}/../../../GeoSpatial/include )
MARK_AS_ADVANCED(GeoSpatial_INCLUDE_DIR)

# Look for the library.
IF( MSVC )
  SET( H3D_MSVC_VERSION 6 )
  SET( TEMP_MSVC_VERSION 1299 )
  WHILE( ${MSVC_VERSION} GREATER ${TEMP_MSVC_VERSION} )
    MATH( EXPR H3D_MSVC_VERSION "${H3D_MSVC_VERSION} + 1" )
    MATH( EXPR TEMP_MSVC_VERSION "${TEMP_MSVC_VERSION} + 100" )
  ENDWHILE( ${MSVC_VERSION} GREATER ${TEMP_MSVC_VERSION} )
  SET( GeoSpatial_NAME "GeoSpatial_vc${H3D_MSVC_VERSION}" )
ELSEIF(UNIX)
  SET( GeoSpatial_NAME h3dui )
ELSE()
  SET( GeoSpatial_NAME GeoSpatial )
ENDIF( MSVC )

SET( DEFAULT_LIB_INSTALL "lib" )
IF( WIN32 )
  SET( DEFAULT_LIB_INSTALL "lib32" )
  IF( CMAKE_SIZEOF_VOID_P EQUAL 8 )
    SET( DEFAULT_LIB_INSTALL "lib64" )
  ENDIF( CMAKE_SIZEOF_VOID_P EQUAL 8 )
ENDIF( WIN32 )

FIND_LIBRARY( GeoSpatial_LIBRARY NAMES ${GeoSpatial_NAME}
              PATHS $ENV{H3D_ROOT}/../GeoSpatial/${DEFAULT_LIB_INSTALL}
                    ../../../${DEFAULT_LIB_INSTALL}
                    ${CMAKE_MODULE_PATH}/../../../GeoSpatial/${DEFAULT_LIB_INSTALL} )

FIND_LIBRARY( GeoSpatial_DEBUG_LIBRARY NAMES ${GeoSpatial_NAME}_d
              PATHS $ENV{H3D_ROOT}/../GeoSpatial/${DEFAULT_LIB_INSTALL}
                    ../../../${DEFAULT_LIB_INSTALL}
                    ${CMAKE_MODULE_PATH}/../../../GeoSpatial/${DEFAULT_LIB_INSTALL} )
MARK_AS_ADVANCED(GeoSpatial_LIBRARY)
MARK_AS_ADVANCED(GeoSpatial_DEBUG_LIBRARY)

IF(GeoSpatial_LIBRARY OR GeoSpatial_DEBUG_LIBRARY)
  SET( HAVE_GeoSpatial_LIBRARY 1 )
ELSE(GeoSpatial_LIBRARY OR GeoSpatial_DEBUG_LIBRARY)
  SET( HAVE_GeoSpatial_LIBRARY 0 )
ENDIF(GeoSpatial_LIBRARY OR GeoSpatial_DEBUG_LIBRARY)

# Copy the results to the output variables.
IF(GeoSpatial_INCLUDE_DIR AND HAVE_GeoSpatial_LIBRARY)

  SET(GeoSpatial_FOUND 1)
  IF(GeoSpatial_LIBRARY)
    SET(GeoSpatial_LIBRARIES ${GeoSpatial_LIBRARIES} optimized ${GeoSpatial_LIBRARY} )
  ELSE(GeoSpatial_LIBRARY)
    SET(GeoSpatial_LIBRARIES ${GeoSpatial_LIBRARIES} optimized ${GeoSpatial_NAME} )
    MESSAGE( STATUS "GeoSpatial release libraries not found. Release build might not work." )
  ENDIF(GeoSpatial_LIBRARY)

  IF(GeoSpatial_DEBUG_LIBRARY)
    SET(GeoSpatial_LIBRARIES ${GeoSpatial_LIBRARIES} debug ${GeoSpatial_DEBUG_LIBRARY} )
  ELSE(GeoSpatial_DEBUG_LIBRARY)
    SET(GeoSpatial_LIBRARIES ${GeoSpatial_LIBRARIES} debug ${GeoSpatial_NAME}_d )
    MESSAGE( STATUS "GeoSpatial debug libraries not found. Debug build might not work." )
  ENDIF(GeoSpatial_DEBUG_LIBRARY)
  
  SET(GeoSpatial_INCLUDE_DIR ${GeoSpatial_INCLUDE_DIR} )
  SET(GeoSpatial_LIBRARIES ${GeoSpatial_LIBRARIES} )
ELSE(GeoSpatial_INCLUDE_DIR AND HAVE_GeoSpatial_LIBRARY)
  SET(GeoSpatial_FOUND 0)
  SET(GeoSpatial_LIBRARIES)
  SET(GeoSpatial_INCLUDE_DIR)
ENDIF(GeoSpatial_INCLUDE_DIR AND HAVE_GeoSpatial_LIBRARY)

# Report the results.
IF(NOT GeoSpatial_FOUND)
  SET(GeoSpatial_DIR_MESSAGE
    "GeoSpatial was not found. Make sure GeoSpatial_LIBRARY ( and/or GeoSpatial_DEBUG_LIBRARY ) and GeoSpatial_INCLUDE_DIR are set.")
  IF(GeoSpatial_FIND_REQUIRED)
    MESSAGE(FATAL_ERROR "${GeoSpatial_DIR_MESSAGE}")
  ELSEIF(NOT GeoSpatial_FIND_QUIETLY)
    MESSAGE(STATUS "${GeoSpatial_DIR_MESSAGE}")
  ENDIF(GeoSpatial_FIND_REQUIRED)
ENDIF(NOT GeoSpatial_FOUND)
