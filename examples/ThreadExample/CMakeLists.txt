IF(WIN32)
  cmake_minimum_required(VERSION 2.6.0)
ENDIF(WIN32)
# The name of our project is "ThreadExample". CMakeLists files in this project can
# refer to the root source directory of the project as ${ThreadExample_SOURCE_DIR} and
# to the root binary directory of the project as ${ThreadExample_BINARY_DIR}.
project(ThreadExample)

# set the install directory to the H3D directory on Windows
IF( WIN32 AND CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT AND NOT H3D_CMAKE_INSTALL_PREFIX_ALREADY_SET )
  SET( CMAKE_INSTALL_PREFIX ${ThreadExample_SOURCE_DIR}/../../.. CACHE PATH "Install path prefix, prepended onto install directories." FORCE )
  SET( H3D_CMAKE_INSTALL_PREFIX_ALREADY_SET TRUE )
ENDIF( WIN32 AND CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT AND NOT H3D_CMAKE_INSTALL_PREFIX_ALREADY_SET )

# Optional libraries to link against are added to this variable.
SET(optionalLibs)

# Required libraries to link against are added to this variable.
SET(requiredLibs)

# Where to find modules used to find libraries.
SET(CMAKE_MODULE_PATH "${ThreadExample_SOURCE_DIR}/../../build/modules/")

IF( TARGET H3DUtil )
  INCLUDE_DIRECTORIES( ${H3DUTIL_INCLUDE_DIR} ) 
  SET( requiredLibs ${requiredLibs} H3DUtil )
ELSE( TARGET H3DUtil )
  #H3DUtil
  FIND_PACKAGE(H3DUtil REQUIRED)

  IF(H3DUTIL_FOUND)
    INCLUDE_DIRECTORIES( ${H3DUTIL_INCLUDE_DIR} ) 
    SET(requiredLibs ${requiredLibs} ${H3DUTIL_LIBRARIES} )
  ENDIF(H3DUTIL_FOUND)
ENDIF( TARGET H3DUtil )

IF( TARGET HAPI )
  INCLUDE_DIRECTORIES( ${HAPI_INCLUDE_DIR} ) 
  SET( requiredLibs ${requiredLibs} HAPI )
ELSE( TARGET HAPI )
  #HAPI
  FIND_PACKAGE(HAPI REQUIRED)

  IF(HAPI_FOUND)
    INCLUDE_DIRECTORIES( ${HAPI_INCLUDE_DIR} ) 
    SET(requiredLibs ${requiredLibs} ${HAPI_LIBRARIES} )
  ENDIF(HAPI_FOUND)
ENDIF( TARGET HAPI )

IF( TARGET H3DAPI )
  INCLUDE_DIRECTORIES( ${H3DAPI_INCLUDE_DIR} ) 
  SET( requiredLibs ${requiredLibs} H3DAPI )
ELSE( TARGET H3DAPI )
  #H3DAPI
  FIND_PACKAGE(H3DAPI REQUIRED)

  IF(H3DAPI_FOUND)
    INCLUDE_DIRECTORIES( ${H3DAPI_INCLUDE_DIR} ) 
    SET(requiredLibs ${requiredLibs} ${H3DAPI_LIBRARIES} )
  ENDIF(H3DAPI_FOUND)
ENDIF( TARGET H3DAPI )

IF( H3D_USE_DEPENDENCIES_ONLY )
  # The variables set here must be set by the CMakeLists.txt that sets H3D_USE_DEPENDENCIES_ONLY to true.
  IF(PTHREAD_FOUND)
    INCLUDE_DIRECTORIES( ${PTHREAD_INCLUDE_DIR} ) 
  ENDIF(PTHREAD_FOUND)
  INCLUDE_DIRECTORIES( ${EXTERNAL_INCLUDE_DIR} ) 
ENDIF( H3D_USE_DEPENDENCIES_ONLY )

# Build files created.
ADD_LIBRARY(ThreadExample SHARED ${ThreadExample_SOURCE_DIR}/ThreadExample.h
                                 ${ThreadExample_SOURCE_DIR}/ThreadExample.cpp )

# Link against libraries.
TARGET_LINK_LIBRARIES( ThreadExample ${requiredLibs} ${optionalLibs} )

# Debug versions have _d postfix.
SET_TARGET_PROPERTIES( ThreadExample PROPERTIES DEBUG_POSTFIX "_d" )

IF(WIN32) 
  IF(MSVC80)
    # Remove some warnings about deprecated functions. Do not treat wchar_t as built in type
    SET_TARGET_PROPERTIES( ThreadExample PROPERTIES COMPILE_FLAGS "-D_CRT_SECURE_NO_DEPRECATE /Zc:wchar_t-" )
  ELSEIF(MSVC90)
    # Remove some warnings about deprecated functions. Do not treat wchar_t as built in type. Compile on multiple processors/threads if possible.
    SET_TARGET_PROPERTIES( ThreadExample PROPERTIES COMPILE_FLAGS "-D_CRT_SECURE_NO_DEPRECATE /Zc:wchar_t- /MP" )
  ENDIF(MSVC80)
ENDIF(WIN32)

# Install to these directories.
INSTALL( TARGETS ThreadExample
         LIBRARY DESTINATION lib
         RUNTIME DESTINATION bin )

