IF(WIN32)
  cmake_minimum_required(VERSION 2.6.0)
ENDIF(WIN32)

IF( COMMAND cmake_policy )
  IF( POLICY CMP0003 )
    cmake_policy( SET CMP0003 NEW )
  ENDIF( POLICY CMP0003 )
ENDIF( COMMAND cmake_policy )

# The name of our project is "ParticleDemo".  CMakeLists files in this project can
# refer to the root source directory of the project as ${ParticleDemo_SOURCE_DIR} and
# to the root binary directory of the project as ${ParticleDemo_BINARY_DIR}.
project (ParticleDemo)

# set the install directory to the H3D directory on Windows
IF( CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT AND NOT H3D_CMAKE_INSTALL_PREFIX_ALREADY_SET )
  SET( CMAKE_INSTALL_PREFIX ${ParticleDemo_SOURCE_DIR}/../../../.. CACHE PATH "Install path prefix, prepended onto install directories." FORCE )
  SET( H3D_CMAKE_INSTALL_PREFIX_ALREADY_SET TRUE )
ENDIF( CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT AND NOT H3D_CMAKE_INSTALL_PREFIX_ALREADY_SET )

# use, i.e. don't skip the full RPATH for the build tree
SET(CMAKE_SKIP_BUILD_RPATH FALSE)

# when building, don't use the install RPATH already
# (but later on when installing)
SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE) 

# add the automatically determined parts of the RPATH
# which point to directories outside the build tree to the install RPATH
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

# the RPATH to be used when installing, but only if it's not a system directory
LIST(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${CMAKE_INSTALL_PREFIX}/lib" isSystemDir)
IF("${isSystemDir}" STREQUAL "-1")
   SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
ENDIF("${isSystemDir}" STREQUAL "-1")

# Source files.
SET( ParticleDemo_SRCS
       "${ParticleDemo_SOURCE_DIR}/../src/ConsoleDialog.cpp"
       "${ParticleDemo_SOURCE_DIR}/../src/ConsoleDialog.h"
       "${ParticleDemo_SOURCE_DIR}/../src/EmitterSettings.cpp"
       "${ParticleDemo_SOURCE_DIR}/../src/EmitterSettings.h"
       "${ParticleDemo_SOURCE_DIR}/../src/Envini.h"
       "${ParticleDemo_SOURCE_DIR}/../src/ParticleDemo.cpp"
       "${ParticleDemo_SOURCE_DIR}/../src/PhysicsModels.cpp"
       "${ParticleDemo_SOURCE_DIR}/../src/PhysicsModels.h"
       "${ParticleDemo_SOURCE_DIR}/../src/WxFrame.cpp"
       "${ParticleDemo_SOURCE_DIR}/../src/WxFrame.h"
       "${ParticleDemo_SOURCE_DIR}/../src/WxWidgetsWindow.cpp"
       "${ParticleDemo_SOURCE_DIR}/../src/WxWidgetsWindow.h" )

# Include directories.
INCLUDE_DIRECTORIES( ../src )

# Add required libraries to this variable.
SET(requiredLibs)

# Where to find cmake modules used to find packages.
SET(CMAKE_MODULE_PATH "${ParticleDemo_SOURCE_DIR}/../../../build/modules/")

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

# WxWidgets.
SET(WXWINDOWS_USE_GL 1)
IF(WIN32)
  FIND_PACKAGE(wxWidgetsWin)
  IF(wxWidgets_FOUND)
    INCLUDE_DIRECTORIES( ${wxWidgets_INCLUDE_DIR} ) 
    SET(requiredLibs ${requiredLibs} ${wxWidgets_LIBRARIES} )
  ENDIF(wxWidgets_FOUND)
ENDIF(WIN32)

IF(wxWidgets_FOUND)
ELSE(wxWidgets_FOUND)
  SET(wxWidgets_USE_LIBS base core gl adv)
  FIND_PACKAGE(wxWidgets REQUIRED)
  IF(wxWidgets_FOUND)
    INCLUDE(${wxWidgets_USE_FILE})
    SET(requiredLibs ${requiredLibs} ${wxWidgets_LIBRARIES} )
  ENDIF(wxWidgets_FOUND)
ENDIF(wxWidgets_FOUND)

# Create build files.
ADD_EXECUTABLE(ParticleDemo WIN32 MACOSX_BUNDLE ${ParticleDemo_SRCS})

# Link against requiredLibs and optionalLibs.
TARGET_LINK_LIBRARIES( ParticleDemo ${requiredLibs} ${optionalLibs} )

# Set postfix for debug version of H3DViewer.
SET_TARGET_PROPERTIES( ParticleDemo PROPERTIES DEBUG_POSTFIX "_d" )

IF(WIN32)

  # Set compile and link properties for projects.
  IF(MSVC70 OR MSVC71)
    SET_TARGET_PROPERTIES( ParticleDemo PROPERTIES LINK_FLAGS_DEBUG "/NODEFAULTLIB:msvcrt" )
    SET_TARGET_PROPERTIES( ParticleDemo PROPERTIES COMPILE_FLAGS "-DWXUSINGDLL /Zc:wchar_t /Od" )
  ELSEIF(MSVC80)
    SET( ParticleDemo_COMPILE_FLAGS "-DWXUSINGDLL -D_CRT_SECURE_NO_DEPRECATE /Od" )
    
    # This might be useful for visual studio 2005 users that often recompile the api.
    IF( NOT DEFINED USE_VC8_MP_FLAG )
      SET( USE_VC8_MP_FLAG "NO" CACHE BOOL "In visual studio 8 the MP flag exists but is not documented. Maybe it is unsafe to use. If you want to use it then set this flag to yes." )
    ENDIF( NOT DEFINED USE_VC8_MP_FLAG )
    
    IF( USE_VC8_MP_FLAG )
      SET( ParticleDemo_COMPILE_FLAGS "${ParticleDemo_COMPILE_FLAGS} /MP" )
    ENDIF( USE_VC8_MP_FLAG )
    # Remove some warnings about deprecated functions.
    SET_TARGET_PROPERTIES( ParticleDemo PROPERTIES COMPILE_FLAGS "${ParticleDemo_COMPILE_FLAGS}" )
    # Needed to run in debug mode. Removes warning about conflict between libraries.
    SET_TARGET_PROPERTIES( ParticleDemo PROPERTIES LINK_FLAGS_DEBUG "/NODEFAULTLIB:msvcrt" )
  ELSEIF(MSVC90)
    # Remove some warnings about deprecated functions. Compile on multiple processors/threads if possible.
    SET_TARGET_PROPERTIES( ParticleDemo PROPERTIES COMPILE_FLAGS "-DWXUSINGDLL -D_CRT_SECURE_NO_DEPRECATE /MP" )
    # Needed to run in debug mode. Removes warning about conflict between libraries.
    SET_TARGET_PROPERTIES( ParticleDemo PROPERTIES LINK_FLAGS_DEBUG "/NODEFAULTLIB:msvcrt" )
  ELSEIF(MSVC10)
    # Remove some warnings about deprecated functions. Compile on multiple processors/threads if possible.
    SET_TARGET_PROPERTIES( ParticleDemo PROPERTIES COMPILE_FLAGS "-DWXUSINGDLL -D_CRT_SECURE_NO_DEPRECATE /MP" )
    # Needed to run in debug mode. Removes warning about conflict between libraries.
    SET_TARGET_PROPERTIES( ParticleDemo PROPERTIES LINK_FLAGS_DEBUG "/NODEFAULTLIB:msvcrt" )
  ENDIF(MSVC70 OR MSVC71)
ENDIF(WIN32)

INSTALL( TARGETS ParticleDemo
         LIBRARY DESTINATION lib
         RUNTIME DESTINATION bin
         COMPONENT H3DAPI_cpack_examples_runtime )
