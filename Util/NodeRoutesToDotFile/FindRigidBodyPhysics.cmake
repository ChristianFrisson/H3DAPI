# - Find RigidBodyPhysics
# Find the native RigidBodyPhysics headers and libraries.
#
#  RigidBodyPhysics_INCLUDE_DIR -  where to find RigidBodyPhysics.h, etc.
#  RigidBodyPhysics_LIBRARIES    - List of libraries when using RigidBodyPhysics.
#  RigidBodyPhysics_FOUND        - True if RigidBodyPhysics found.

GET_FILENAME_COMPONENT(module_file_path ${CMAKE_CURRENT_LIST_FILE} PATH )

# Look for the header file.
FIND_PATH( RigidBodyPhysics_INCLUDE_DIR NAMES H3D/RigidBodyPhysics/PhysicsEngineThread.h
           PATHS $ENV{H3D_ROOT}/../RigidBodyPhysics/include
                 ../../../RigidBodyPhysics/include
                 ${module_file_path}/../../../RigidBodyPhysics/include )
MARK_AS_ADVANCED(RigidBodyPhysics_INCLUDE_DIR)

# Look for the library.
IF(MSVC70 OR MSVC71)
  SET( RigidBodyPhysics_NAME RigidBodyPhysics_vc7 )
ELSEIF(MSVC80)
  SET( RigidBodyPhysics_NAME RigidBodyPhysics_vc8 )
ELSEIF(MSVC90)
  SET( RigidBodyPhysics_NAME RigidBodyPhysics_vc9 )
ELSEIF(MSVC10)
  SET( RigidBodyPhysics_NAME RigidBodyPhysics_vc10 )
ELSE(MSVC70 OR MSVC71)
  SET( RigidBodyPhysics_NAME RigidBodyPhysics )
ENDIF(MSVC70 OR MSVC71)

FIND_LIBRARY( RigidBodyPhysics_LIBRARY NAMES ${RigidBodyPhysics_NAME}
              PATHS $ENV{H3D_ROOT}/../RigidBodyPhysics/lib
                    ../../../lib
                    ${CMAKE_MODULE_PATH}/../../../RigidBodyPhysics/lib )

FIND_LIBRARY( RigidBodyPhysics_DEBUG_LIBRARY NAMES ${RigidBodyPhysics_NAME}_d
              PATHS $ENV{H3D_ROOT}/../RigidBodyPhysics/lib
                    ../../../lib
                    ${CMAKE_MODULE_PATH}/../../../RigidBodyPhysics/lib )
MARK_AS_ADVANCED(RigidBodyPhysics_LIBRARY)
MARK_AS_ADVANCED(RigidBodyPhysics_DEBUG_LIBRARY)

IF(RigidBodyPhysics_LIBRARY OR RigidBodyPhysics_DEBUG_LIBRARY)
  SET( HAVE_RigidBodyPhysics_LIBRARY 1 )
ELSE(RigidBodyPhysics_LIBRARY OR RigidBodyPhysics_DEBUG_LIBRARY)
  SET( HAVE_RigidBodyPhysics_LIBRARY 0 )
ENDIF(RigidBodyPhysics_LIBRARY OR RigidBodyPhysics_DEBUG_LIBRARY)

# Copy the results to the output variables.
IF(RigidBodyPhysics_INCLUDE_DIR AND HAVE_RigidBodyPhysics_LIBRARY)

  SET(RigidBodyPhysics_FOUND 1)
  IF(RigidBodyPhysics_LIBRARY)
    SET(RigidBodyPhysics_LIBRARIES ${RigidBodyPhysics_LIBRARIES} optimized ${RigidBodyPhysics_LIBRARY} )
  ELSE(RigidBodyPhysics_LIBRARY)
    SET(RigidBodyPhysics_LIBRARIES ${RigidBodyPhysics_LIBRARIES} optimized ${RigidBodyPhysics_NAME} )
    MESSAGE( STATUS "RigidBodyPhysics release libraries not found. Release build might not work." )
  ENDIF(RigidBodyPhysics_LIBRARY)

  IF(RigidBodyPhysics_DEBUG_LIBRARY)
    SET(RigidBodyPhysics_LIBRARIES ${RigidBodyPhysics_LIBRARIES} debug ${RigidBodyPhysics_DEBUG_LIBRARY} )
  ELSE(RigidBodyPhysics_DEBUG_LIBRARY)
    SET(RigidBodyPhysics_LIBRARIES ${RigidBodyPhysics_LIBRARIES} debug ${RigidBodyPhysics_NAME}_d )
    MESSAGE( STATUS "RigidBodyPhysics debug libraries not found. Debug build might not work." )
  ENDIF(RigidBodyPhysics_DEBUG_LIBRARY)
  
  SET(RigidBodyPhysics_INCLUDE_DIR ${RigidBodyPhysics_INCLUDE_DIR} )
  SET(RigidBodyPhysics_LIBRARIES ${RigidBodyPhysics_LIBRARIES} )
ELSE(RigidBodyPhysics_INCLUDE_DIR AND HAVE_RigidBodyPhysics_LIBRARY)
  SET(RigidBodyPhysics_FOUND 0)
  SET(RigidBodyPhysics_LIBRARIES)
  SET(RigidBodyPhysics_INCLUDE_DIR)
ENDIF(RigidBodyPhysics_INCLUDE_DIR AND HAVE_RigidBodyPhysics_LIBRARY)

# Report the results.
IF(NOT RigidBodyPhysics_FOUND)
  SET(RigidBodyPhysics_DIR_MESSAGE
    "RigidBodyPhysics was not found. Make sure RigidBodyPhysics_LIBRARY ( and/or RigidBodyPhysics_DEBUG_LIBRARY ) and RigidBodyPhysics_INCLUDE_DIR are set.")
  IF(RigidBodyPhysics_FIND_REQUIRED)
    MESSAGE(FATAL_ERROR "${RigidBodyPhysics_DIR_MESSAGE}")
  ELSEIF(NOT RigidBodyPhysics_FIND_QUIETLY)
    MESSAGE(STATUS "${RigidBodyPhysics_DIR_MESSAGE}")
  ENDIF(RigidBodyPhysics_FIND_REQUIRED)
ENDIF(NOT RigidBodyPhysics_FOUND)
