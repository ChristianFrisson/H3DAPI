# - Find UI
# Find the native UI headers and libraries.
#
#  UI_INCLUDE_DIR -  where to find UI.h, etc.
#  UI_LIBRARIES    - List of libraries when using UI.
#  UI_FOUND        - True if UI found.

GET_FILENAME_COMPONENT(module_file_path ${CMAKE_CURRENT_LIST_FILE} PATH )

# Look for the header file.
FIND_PATH( UI_INCLUDE_DIR NAMES H3D/UI/UI.h
           PATHS $ENV{H3D_ROOT}/../UI/include
                 ../../../UI/include
                 ${module_file_path}/../../../UI/include )
MARK_AS_ADVANCED(UI_INCLUDE_DIR)

# Look for the library.
IF(MSVC70 OR MSVC71)
  SET( UI_NAME UI_vc7 )
ELSEIF(MSVC80)
  SET( UI_NAME UI_vc8 )
ELSEIF(MSVC90)
  SET( UI_NAME UI_vc9 )
ELSE(MSVC70 OR MSVC71)
  SET( UI_NAME UI )
ENDIF(MSVC70 OR MSVC71)

FIND_LIBRARY( UI_LIBRARY NAMES ${UI_NAME}
              PATHS $ENV{H3D_ROOT}/../UI/lib
                    ../../../lib
                    ${CMAKE_MODULE_PATH}/../../../UI/lib )

FIND_LIBRARY( UI_DEBUG_LIBRARY NAMES ${UI_NAME}_d
              PATHS $ENV{H3D_ROOT}/../UI/lib
                    ../../../lib
                    ${CMAKE_MODULE_PATH}/../../../UI/lib )
MARK_AS_ADVANCED(UI_LIBRARY)
MARK_AS_ADVANCED(UI_DEBUG_LIBRARY)

IF(UI_LIBRARY OR UI_DEBUG_LIBRARY)
  SET( HAVE_UI_LIBRARY 1 )
ELSE(UI_LIBRARY OR UI_DEBUG_LIBRARY)
  SET( HAVE_UI_LIBRARY 0 )
ENDIF(UI_LIBRARY OR UI_DEBUG_LIBRARY)

# Copy the results to the output variables.
IF(UI_INCLUDE_DIR AND HAVE_UI_LIBRARY)

  SET(UI_FOUND 1)
  IF(UI_LIBRARY)
    SET(UI_LIBRARIES ${UI_LIBRARIES} optimized ${UI_LIBRARY} )
  ELSE(UI_LIBRARY)
    SET(UI_LIBRARIES ${UI_LIBRARIES} optimized ${UI_NAME} )
    MESSAGE( STATUS "UI release libraries not found. Release build might not work." )
  ENDIF(UI_LIBRARY)

  IF(UI_DEBUG_LIBRARY)
    SET(UI_LIBRARIES ${UI_LIBRARIES} debug ${UI_DEBUG_LIBRARY} )
  ELSE(UI_DEBUG_LIBRARY)
    SET(UI_LIBRARIES ${UI_LIBRARIES} debug ${UI_NAME}_d )
    MESSAGE( STATUS "UI debug libraries not found. Debug build might not work." )
  ENDIF(UI_DEBUG_LIBRARY)
  
  SET(UI_INCLUDE_DIR ${UI_INCLUDE_DIR} )
  SET(UI_LIBRARIES ${UI_LIBRARIES} )
ELSE(UI_INCLUDE_DIR AND HAVE_UI_LIBRARY)
  SET(UI_FOUND 0)
  SET(UI_LIBRARIES)
  SET(UI_INCLUDE_DIR)
ENDIF(UI_INCLUDE_DIR AND HAVE_UI_LIBRARY)

# Report the results.
IF(NOT UI_FOUND)
  SET(UI_DIR_MESSAGE
    "UI was not found. Make sure UI_LIBRARY ( and/or UI_DEBUG_LIBRARY ) and UI_INCLUDE_DIR are set.")
  IF(UI_FIND_REQUIRED)
    MESSAGE(FATAL_ERROR "${UI_DIR_MESSAGE}")
  ELSEIF(NOT UI_FIND_QUIETLY)
    MESSAGE(STATUS "${UI_DIR_MESSAGE}")
  ENDIF(UI_FIND_REQUIRED)
ENDIF(NOT UI_FOUND)
