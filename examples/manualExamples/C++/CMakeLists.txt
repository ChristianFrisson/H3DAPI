IF(WIN32)
  cmake_minimum_required(VERSION 2.6.0)
ENDIF(WIN32)
# The name of our project is "ManualExamples". CMakeLists files in this project can
# refer to the root source directory of the project as ${ManualExamples_SOURCE_DIR} and
# to the root binary directory of the project as ${ManualExamples_BINARY_DIR}.
project (ManualExamples)

IF( WIN32 AND CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT AND NOT H3D_CMAKE_INSTALL_PREFIX_ALREADY_SET )
  SET( CMAKE_INSTALL_PREFIX ${ManualExamples_SOURCE_DIR}/../../../.. CACHE PATH "Install path prefix, prepended onto install directories." FORCE )
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

# Optional libraries to link against are added to this variable.
SET(optionalLibs)

# Required libraries to link against are added to this variable.
SET(requiredLibs)

# Where to find modules used to find libraries.
SET(CMAKE_MODULE_PATH "${ManualExamples_SOURCE_DIR}/../../../build/modules/")
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

# Optional extra library. Makes is possible to used GLUT windows handling with H3D API.
IF(WIN32)
  FIND_PACKAGE(GLUTWin REQUIRED)
ELSE(WIN32)
  FIND_PACKAGE(GLUT REQUIRED)
ENDIF(WIN32)

IF(GLUT_FOUND)
  INCLUDE_DIRECTORIES( ${GLUT_INCLUDE_DIR} )
  IF( WIN32 )
    # This is used in order for the compiler to find freeglut.lib.
    # If freeglut.lib is in any other place than glut32 on your system
    # either this CMakeLists.txt or FindGLUTWin needs to be changed
    # to allow for choice of extra library directory.
    STRIP_AND_ADD_LIBRARY_DIRECTORIES( ${GLUT_LIBRARIES} )
  ELSE( WIN32 )
    SET(requiredLibs ${requiredLibs} ${GLUT_LIBRARIES} )
  ENDIF( WIN32 )
ENDIF(GLUT_FOUND)

# Build files created. Two executables. Sphere_X3D and Spheres_X3D.
ADD_EXECUTABLE(Sphere_X3D ${ManualExamples_SOURCE_DIR}/Sphere_X3D.cpp )
ADD_EXECUTABLE(Spheres_X3D ${ManualExamples_SOURCE_DIR}/Spheres_X3D.cpp )

# Link against libraries.
TARGET_LINK_LIBRARIES( Sphere_X3D ${requiredLibs} ${optionalLibs} )
TARGET_LINK_LIBRARIES( Spheres_X3D ${requiredLibs} ${optionalLibs} )

# Debug versions have _d postfix.
SET_TARGET_PROPERTIES( Sphere_X3D PROPERTIES DEBUG_POSTFIX "_d" )
SET_TARGET_PROPERTIES( Spheres_X3D PROPERTIES DEBUG_POSTFIX "_d" )

# set the install directory to the H3D directory on Windows
SET( DEFAULT_BIN_INSTALL "bin" )
SET( DEFAULT_LIB_INSTALL "lib" )

IF( WIN32 )
  SET( DEFAULT_BIN_INSTALL "bin32" )
  SET( DEFAULT_LIB_INSTALL "lib32" )
  IF( CMAKE_SIZEOF_VOID_P EQUAL 8 )
    SET( DEFAULT_BIN_INSTALL "bin64" )
    SET( DEFAULT_LIB_INSTALL "lib64" )
  ENDIF( CMAKE_SIZEOF_VOID_P EQUAL 8 )
  
  # Set compile and link properties for projects.
  SET( ManualExamples_COMPILE_FLAGS "" )
  
  # Treat wchar_t as built in type for all visual studio versions.
  # This is default for every version above 7 ( so far ) but we still set it for all.
  SET( ManualExamples_COMPILE_FLAGS "${ManualExamples_COMPILE_FLAGS} /Zc:wchar_t")
  
  IF( ${MSVC_VERSION} GREATER 1399 )
    # Remove compiler warnings about deprecation for visual studio versions 8 and above.
    SET( ManualExamples_COMPILE_FLAGS "${ManualExamples_COMPILE_FLAGS} -D_CRT_SECURE_NO_DEPRECATE" )
  ENDIF( ${MSVC_VERSION} GREATER 1399 )
  
  IF( ${MSVC_VERSION} GREATER 1499 )
    # Build using several threads for visual studio versions 9 and above.
    SET( ManualExamples_COMPILE_FLAGS "${ManualExamples_COMPILE_FLAGS} /MP" )
  ENDIF( ${MSVC_VERSION} GREATER 1499 )
  
  SET_TARGET_PROPERTIES( Sphere_X3D PROPERTIES COMPILE_FLAGS "${ManualExamples_COMPILE_FLAGS}" )
  SET_TARGET_PROPERTIES( Spheres_X3D PROPERTIES COMPILE_FLAGS "${ManualExamples_COMPILE_FLAGS}" )
ENDIF(WIN32)

# Install to these directories.
INSTALL( TARGETS Sphere_X3D
         LIBRARY DESTINATION ${DEFAULT_LIB_INSTALL}
         RUNTIME DESTINATION ${DEFAULT_BIN_INSTALL}
         COMPONENT H3DAPI_cpack_examples_runtime )

INSTALL( TARGETS Spheres_X3D
         LIBRARY DESTINATION ${DEFAULT_LIB_INSTALL}
         RUNTIME DESTINATION ${DEFAULT_BIN_INSTALL}
         COMPONENT H3DAPI_cpack_examples_runtime )

