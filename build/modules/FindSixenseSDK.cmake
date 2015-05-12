# - Find SIXENSE
# Find the native SIXENSE headers and libraries.
#
#  SIXENSE_INCLUDE_DIR -  where to find SIXENSE.h, etc.
#  SIXENSE_LIBRARIES    - List of libraries when using SIXENSE.
#  SIXENSE_FOUND        - True if SIXENSE found.

include( H3DExternalSearchPath )
GET_FILENAME_COMPONENT( module_file_path ${CMAKE_CURRENT_LIST_FILE} PATH )
get_external_search_paths_h3d( module_include_search_paths module_lib_search_paths ${module_file_path} )

IF( CMAKE_CL_64 )
  SET( LIB "win64" )
  SET( SDK_LIB "x64" )
  SET( PROGRAMFILES_X86 "ProgramFiles(x86)" ) # Needed because ( and ) are disallowed characters in cmake 3.1.0, see policy cmp0053
  SET( steam_path "$ENV{${PROGRAMFILES_X86}}/Steam/SteamApps/common/Sixense SDK/SixenseSDK" )
ELSE( CMAKE_CL_64 )
  SET( LIB "lib32" )
  SET( SDK_LIB "win32" )
  SET( steam_path "$ENV{ProgramFiles}/Steam/SteamApps/common/Sixense SDK/SixenseSDK" )
ENDIF( CMAKE_CL_64 )

SET(SIXENSE_INSTALL_DIR "" CACHE PATH "Path to external Sixense SDK installation" )

# Look for the header file.
FIND_PATH(SIXENSE_INCLUDE_DIR NAMES sixense.h
                           PATHS ${SIXENSE_INSTALL_DIR}/include
                                 ${module_include_search_paths}
         ${steam_path}/include
         $ENV{SIXENSE_SDK_PATH}/include 
                           DOC "Path in which the file sixense.h is located." )
MARK_AS_ADVANCED(SIXENSE_INCLUDE_DIR)

# Look for the library.
FIND_LIBRARY(SIXENSE_LIBRARY NAMES sixense sixense_${SDK_LIB}
                                PATHS ${SIXENSE_INSTALL_DIR}/lib/${SDK_LIB}/release_dll
                                      ${module_lib_search_paths}
                                      ${steam_path}/lib/${SDK_LIB}/release_dll
                                      $ENV{SIXENSE_SDK_PATH}/lib/${SDK_LIB}/release_dll
                                DOC "Path to sixense library." )
MARK_AS_ADVANCED(SIXENSE_LIBRARY)

# Copy the results to the output variables.
IF(SIXENSE_INCLUDE_DIR AND SIXENSE_LIBRARY)
  SET(SIXENSE_FOUND 1)
  SET(SIXENSE_LIBRARIES ${SIXENSE_LIBRARY})
  SET(SIXENSE_INCLUDE_DIR ${SIXENSE_INCLUDE_DIR})
ELSE(SIXENSE_INCLUDE_DIR AND SIXENSE_LIBRARY)
  SET(SIXENSE_FOUND 0)
  SET(SIXENSE_LIBRARIES)
  SET(SIXENSE_INCLUDE_DIR)
ENDIF(SIXENSE_INCLUDE_DIR AND SIXENSE_LIBRARY)

# Report the results.
IF(NOT SIXENSE_FOUND)
  SET(SIXENSE_DIR_MESSAGE
    "SIXENSE was not found. Make sure SIXENSE_LIBRARY and SIXENSE_INCLUDE_DIR are set to where you have your sixense sdk header and lib files. If you do not have the library you will not be able to use the HydraSensor.")
  IF(SIXENSE_FIND_REQUIRED)
      MESSAGE(FATAL_ERROR "${SIXENSE_DIR_MESSAGE}")
  ELSEIF(NOT SIXENSE_FIND_QUIETLY)
    MESSAGE(STATUS "${SIXENSE_DIR_MESSAGE}")
  ENDIF(SIXENSE_FIND_REQUIRED)
ENDIF(NOT SIXENSE_FOUND)
