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

IF( H3D_USE_DEPENDENCIES_ONLY )
  # The variables set here must be set by the CMakeLists.txt that sets H3D_USE_DEPENDENCIES_ONLY to true.
  INCLUDE_DIRECTORIES( ${EXTERNAL_INCLUDE_DIR} ) 
ENDIF( H3D_USE_DEPENDENCIES_ONLY )

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

# WxWidgets.
SET( TMP_wxWidgets_FOUND FALSE )
SET(WXWINDOWS_USE_GL 1)
IF(WIN32)
  FIND_PACKAGE(wxWidgetsWin)
  IF(wxWidgets_FOUND)
    INCLUDE_DIRECTORIES( ${wxWidgets_INCLUDE_DIR} ) 
    SET(requiredLibs ${requiredLibs} ${wxWidgets_LIBRARIES} )
    SET( TMP_wxWidgets_FOUND TRUE )
  ENDIF(wxWidgets_FOUND)
ENDIF(WIN32)

IF(TMP_wxWidgets_FOUND)
ELSE(TMP_wxWidgets_FOUND)
  SET(wxWidgets_USE_LIBS base core gl adv)
  FIND_PACKAGE(wxWidgets REQUIRED)
  IF(wxWidgets_FOUND)
    INCLUDE(${wxWidgets_USE_FILE})
    SET(requiredLibs ${requiredLibs} ${wxWidgets_LIBRARIES} )
  ENDIF(wxWidgets_FOUND)
ENDIF(TMP_wxWidgets_FOUND)

# Create build files.
ADD_EXECUTABLE(ParticleDemo WIN32 MACOSX_BUNDLE ${ParticleDemo_SRCS})

# Link against requiredLibs and optionalLibs.
TARGET_LINK_LIBRARIES( ParticleDemo ${requiredLibs} ${optionalLibs} )

# Set postfix for debug version of H3DViewer.
SET_TARGET_PROPERTIES( ParticleDemo PROPERTIES DEBUG_POSTFIX "_d" )

IF(MSVC)
  # Set compile and link properties for projects.
  SET( ParticleDemo_COMPILE_FLAGS "-DWXUSINGDLL" )
  
  # Treat wchar_t as built in type for all visual studio versions.
  # This is default for every version above 7 ( so far ) but we still set it for all.
  SET( ParticleDemo_COMPILE_FLAGS "${ParticleDemo_COMPILE_FLAGS} /Zc:wchar_t")
  
  IF( ${MSVC_VERSION} GREATER 1399 )
    # Remove compiler warnings about deprecation for visual studio versions 8 and above.
    SET( ParticleDemo_COMPILE_FLAGS "${ParticleDemo_COMPILE_FLAGS} -D_CRT_SECURE_NO_DEPRECATE" )
  ENDIF( ${MSVC_VERSION} GREATER 1399 )
  
  IF( ${MSVC_VERSION} GREATER 1499 )
    # Build using several threads for visual studio versions 9 and above.
    SET( ParticleDemo_COMPILE_FLAGS "${ParticleDemo_COMPILE_FLAGS} /MP" )
  ENDIF( ${MSVC_VERSION} GREATER 1499 )
  
  IF( ${MSVC_VERSION} LESS 1500 )
    # Turn off optimization for visual studio versions 8 and below.
    # Old setting which I (Markus) have no idea why it was added.
    SET( ParticleDemo_COMPILE_FLAGS "${ParticleDemo_COMPILE_FLAGS} /Od" )
  ENDIF( ${MSVC_VERSION} LESS 1500 )

  SET_TARGET_PROPERTIES( ParticleDemo PROPERTIES COMPILE_FLAGS "${ParticleDemo_COMPILE_FLAGS}" )
  # Remove link warning in debug build.
  SET_TARGET_PROPERTIES( ParticleDemo PROPERTIES LINK_FLAGS_DEBUG "/NODEFAULTLIB:msvcrt" )
ENDIF(MSVC)

SET( DEFAULT_BIN_INSTALL "bin" )
SET( DEFAULT_LIB_INSTALL "lib" )

IF( WIN32 )
  SET( DEFAULT_BIN_INSTALL "bin32" )
  SET( DEFAULT_LIB_INSTALL "lib32" )
  IF( CMAKE_SIZEOF_VOID_P EQUAL 8 )
    SET( DEFAULT_BIN_INSTALL "bin64" )
    SET( DEFAULT_LIB_INSTALL "lib64" )
  ENDIF( CMAKE_SIZEOF_VOID_P EQUAL 8 )
ENDIF( WIN32 )

INSTALL( TARGETS ParticleDemo
         LIBRARY DESTINATION ${DEFAULT_LIB_INSTALL}
         RUNTIME DESTINATION ${DEFAULT_BIN_INSTALL}
         COMPONENT H3DAPI_cpack_examples_runtime )
