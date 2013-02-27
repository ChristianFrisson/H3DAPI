IF(WIN32)
  cmake_minimum_required(VERSION 2.6.0)
ENDIF(WIN32)

# The name of our project is "H3DLoad".  CMakeLists files in this project can
# refer to the root source directory of the project as ${H3DLoad_SOURCE_DIR} and
# to the root binary directory of the project as ${H3DLoad_BINARY_DIR}.
project (H3DLoad)

IF( WIN32 AND CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT AND NOT H3D_CMAKE_INSTALL_PREFIX_ALREADY_SET )
  SET( CMAKE_INSTALL_PREFIX ${H3DLoad_SOURCE_DIR}/../../.. CACHE PATH "Install path prefix, prepended onto install directories." FORCE )
  SET( H3D_CMAKE_INSTALL_PREFIX_ALREADY_SET TRUE )
ENDIF( WIN32 AND CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT AND NOT H3D_CMAKE_INSTALL_PREFIX_ALREADY_SET )

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

# Where to find Source files.
SET( H3DLoad_SRCS "${H3DLoad_SOURCE_DIR}/../src/H3DLoad.cpp" )

# Add optional libs to this variable.
SET(optionalLibs)

# Include directories.
INCLUDE_DIRECTORIES( ../src )

# Add required libs to this variable.
SET(requiredLibs)

# Where to find modules, used when finding packages.
SET(CMAKE_MODULE_PATH "${H3DLoad_SOURCE_DIR}/../../build/modules/")
IF( COMMAND cmake_policy )
  IF( POLICY CMP0011 )
    cmake_policy( SET CMP0011 NEW )
  ENDIF( POLICY CMP0011 )
ENDIF( COMMAND cmake_policy )
INCLUDE( StripAndAddLibraryDirectories )

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

# Glut.
IF( WIN32 )
  FIND_PACKAGE(GLUTWin REQUIRED)
ELSE( WIN32 )
  FIND_PACKAGE(GLUT REQUIRED)
ENDIF( WIN32 )

IF(GLUT_FOUND)
  INCLUDE_DIRECTORIES( ${GLUT_INCLUDE_DIR} )
  IF( WIN32 )
    STRIP_AND_ADD_LIBRARY_DIRECTORIES( ${GLUT_LIBRARIES} )
  ELSE( WIN32 )
    SET(requiredLibs ${requiredLibs} ${GLUT_LIBRARIES} )
  ENDIF( WIN32 )
ENDIF(GLUT_FOUND)
  
# OpenGL
FIND_PACKAGE(OpenGL REQUIRED)

IF(OPENGL_FOUND)
  INCLUDE_DIRECTORIES( ${OPENGL_INCLUDE_DIR} ) 
  SET(requiredLibs ${requiredLibs} ${OPENGL_LIBRARIES} )
ENDIF(OPENGL_FOUND)

# Create build filess.
ADD_EXECUTABLE(H3DLoad  ${H3DLoad_SRCS})
TARGET_LINK_LIBRARIES( H3DLoad ${requiredLibs} ${optionalLibs} )

# Debug version should have _d postfix.
SET_TARGET_PROPERTIES( H3DLoad PROPERTIES DEBUG_POSTFIX "_d" )

# set the install directory to the H3D directory on Windows
IF(MSVC) 
  # Set compile and link properties for projects.
  SET( H3DLoad_COMPILE_FLAGS "" )
  
  # Treat wchar_t as built in type for all visual studio versions.
  # This is default for every version above 7 ( so far ) but we still set it for all.
  SET( H3DLoad_COMPILE_FLAGS "${H3DLoad_COMPILE_FLAGS} /Zc:wchar_t")
  
  IF( ${MSVC_VERSION} GREATER 1399 )
    # Remove compiler warnings about deprecation for visual studio versions 8 and above.
    SET( H3DLoad_COMPILE_FLAGS "${H3DLoad_COMPILE_FLAGS} -D_CRT_SECURE_NO_DEPRECATE" )
  ENDIF( ${MSVC_VERSION} GREATER 1399 )
  
  IF( ${MSVC_VERSION} GREATER 1499 )
    # Build using several threads for visual studio versions 9 and above.
    SET( H3DLoad_COMPILE_FLAGS "${H3DLoad_COMPILE_FLAGS} /MP" )
  ENDIF( ${MSVC_VERSION} GREATER 1499 )
  
  SET_TARGET_PROPERTIES( H3DLoad PROPERTIES COMPILE_FLAGS "${H3DLoad_COMPILE_FLAGS}" )
	SET_TARGET_PROPERTIES( H3DLoad PROPERTIES LINK_FLAGS_DEBUG "/INCREMENTAL:NO /NODEFAULTLIB:msvcrt" )
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

# Install to this destination.
INSTALL( TARGETS H3DLoad
         LIBRARY DESTINATION ${DEFAULT_LIB_INSTALL}
         RUNTIME DESTINATION ${DEFAULT_BIN_INSTALL}
         COMPONENT H3DAPI_cpack_runtime )

