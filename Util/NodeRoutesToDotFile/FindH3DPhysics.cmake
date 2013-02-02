# - Find H3DPhysics
# Find the native H3DPhysics headers and libraries.
#
#  H3DPhysics_INCLUDE_DIR -  where to find H3DPhysics.h, etc.
#  H3DPhysics_LIBRARIES    - List of libraries when using H3DPhysics.
#  H3DPhysics_FOUND        - True if H3DPhysics found.

GET_FILENAME_COMPONENT(module_file_path ${CMAKE_CURRENT_LIST_FILE} PATH )

# Look for the header file.
FIND_PATH( H3DPhysics_INCLUDE_DIR NAMES H3D/H3DPhysics/PhysicsEngineThread.h
           PATHS $ENV{H3D_ROOT}/../H3DPhysics/include
                 ../../../H3DPhysics/include
                 ${module_file_path}/../../../H3DPhysics/include )
MARK_AS_ADVANCED(H3DPhysics_INCLUDE_DIR)

# Look for the library.
IF( MSVC )
  SET( H3D_MSVC_VERSION 6 )
  SET( TEMP_MSVC_VERSION 1299 )
  WHILE( ${MSVC_VERSION} GREATER ${TEMP_MSVC_VERSION} )
    MATH( EXPR H3D_MSVC_VERSION "${H3D_MSVC_VERSION} + 1" )
    MATH( EXPR TEMP_MSVC_VERSION "${TEMP_MSVC_VERSION} + 100" )
  ENDWHILE( ${MSVC_VERSION} GREATER ${TEMP_MSVC_VERSION} )
  SET( H3DPhysics_NAME "H3DPhysics_vc${H3D_MSVC_VERSION}" )
ELSE(MSVC)
  SET( H3DPhysics_NAME H3DPhysics )
ENDIF( MSVC )

SET( DEFAULT_LIB_INSTALL "lib" )
IF( WIN32 )
  SET( DEFAULT_LIB_INSTALL "lib32" )
  IF( CMAKE_SIZEOF_VOID_P EQUAL 8 )
    SET( DEFAULT_LIB_INSTALL "lib64" )
  ENDIF( CMAKE_SIZEOF_VOID_P EQUAL 8 )
ENDIF( WIN32 )

FIND_LIBRARY( H3DPhysics_LIBRARY NAMES ${H3DPhysics_NAME}
              PATHS $ENV{H3D_ROOT}/../H3DPhysics/${DEFAULT_LIB_INSTALL}
                    ../../../${DEFAULT_LIB_INSTALL}
                    ${CMAKE_MODULE_PATH}/../../../H3DPhysics/${DEFAULT_LIB_INSTALL}
                    $ENV{H3D_ROOT}/../${DEFAULT_LIB_INSTALL} )

FIND_LIBRARY( H3DPhysics_DEBUG_LIBRARY NAMES ${H3DPhysics_NAME}_d
              PATHS $ENV{H3D_ROOT}/../H3DPhysics/${DEFAULT_LIB_INSTALL}
                    ../../../${DEFAULT_LIB_INSTALL}
                    ${CMAKE_MODULE_PATH}/../../../H3DPhysics/${DEFAULT_LIB_INSTALL}
                    $ENV{H3D_ROOT}/../${DEFAULT_LIB_INSTALL} )
MARK_AS_ADVANCED(H3DPhysics_LIBRARY)
MARK_AS_ADVANCED(H3DPhysics_DEBUG_LIBRARY)

IF(H3DPhysics_LIBRARY OR H3DPhysics_DEBUG_LIBRARY)
  SET( HAVE_H3DPhysics_LIBRARY 1 )
ELSE(H3DPhysics_LIBRARY OR H3DPhysics_DEBUG_LIBRARY)
  SET( HAVE_H3DPhysics_LIBRARY 0 )
ENDIF(H3DPhysics_LIBRARY OR H3DPhysics_DEBUG_LIBRARY)

# Copy the results to the output variables.
IF(H3DPhysics_INCLUDE_DIR AND HAVE_H3DPhysics_LIBRARY)

  SET(H3DPhysics_FOUND 1)
  IF(H3DPhysics_LIBRARY)
    SET(H3DPhysics_LIBRARIES ${H3DPhysics_LIBRARIES} optimized ${H3DPhysics_LIBRARY} )
  ELSE(H3DPhysics_LIBRARY)
    SET(H3DPhysics_LIBRARIES ${H3DPhysics_LIBRARIES} optimized ${H3DPhysics_NAME} )
    MESSAGE( STATUS "H3DPhysics release libraries not found. Release build might not work." )
  ENDIF(H3DPhysics_LIBRARY)

  IF(H3DPhysics_DEBUG_LIBRARY)
    SET(H3DPhysics_LIBRARIES ${H3DPhysics_LIBRARIES} debug ${H3DPhysics_DEBUG_LIBRARY} )
  ELSE(H3DPhysics_DEBUG_LIBRARY)
    SET(H3DPhysics_LIBRARIES ${H3DPhysics_LIBRARIES} debug ${H3DPhysics_NAME}_d )
    MESSAGE( STATUS "H3DPhysics debug libraries not found. Debug build might not work." )
  ENDIF(H3DPhysics_DEBUG_LIBRARY)
  
  SET(H3DPhysics_INCLUDE_DIR ${H3DPhysics_INCLUDE_DIR} )
  SET(H3DPhysics_LIBRARIES ${H3DPhysics_LIBRARIES} )
ELSE(H3DPhysics_INCLUDE_DIR AND HAVE_H3DPhysics_LIBRARY)
  SET(H3DPhysics_FOUND 0)
  SET(H3DPhysics_LIBRARIES)
  SET(H3DPhysics_INCLUDE_DIR)
ENDIF(H3DPhysics_INCLUDE_DIR AND HAVE_H3DPhysics_LIBRARY)

# Report the results.
IF(NOT H3DPhysics_FOUND)
  SET(H3DPhysics_DIR_MESSAGE
    "H3DPhysics was not found. Make sure H3DPhysics_LIBRARY ( and/or H3DPhysics_DEBUG_LIBRARY ) and H3DPhysics_INCLUDE_DIR are set.")
  IF(H3DPhysics_FIND_REQUIRED)
    MESSAGE(FATAL_ERROR "${H3DPhysics_DIR_MESSAGE}")
  ELSEIF(NOT H3DPhysics_FIND_QUIETLY)
    MESSAGE(STATUS "${H3DPhysics_DIR_MESSAGE}")
  ENDIF(H3DPhysics_FIND_REQUIRED)
ENDIF(NOT H3DPhysics_FOUND)
