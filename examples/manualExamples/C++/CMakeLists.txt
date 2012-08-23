IF(WIN32)
  cmake_minimum_required(VERSION 2.6.0)
ENDIF(WIN32)
# The name of our project is "ManualExamples". CMakeLists files in this project can
# refer to the root source directory of the project as ${ManualExamples_SOURCE_DIR} and
# to the root binary directory of the project as ${ManualExamples_BINARY_DIR}.
project (ManualExamples)

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
  IF(PTHREAD_FOUND)
    INCLUDE_DIRECTORIES( ${PTHREAD_INCLUDE_DIR} ) 
  ENDIF(PTHREAD_FOUND)
  INCLUDE_DIRECTORIES( ${EXTERNAL_INCLUDE_DIR} ) 
  INCLUDE_DIRECTORIES( ${H3DUTIL_INCLUDE_DIR} ) 
  INCLUDE_DIRECTORIES( ${HAPI_INCLUDE_DIR} ) 
  INCLUDE_DIRECTORIES( ${H3DAPI_INCLUDE_DIR} ) 
  
  SET( requiredLibs ${requiredLibs} H3DUtil HAPI H3DAPI )
ELSE( H3D_USE_DEPENDENCIES_ONLY )
  #H3DAPI
  FIND_PACKAGE(H3DAPI REQUIRED)

  IF(H3DAPI_FOUND)
    INCLUDE_DIRECTORIES( ${H3DAPI_INCLUDE_DIR} ) 
    SET(requiredLibs ${requiredLibs} ${H3DAPI_LIBRARIES} )
  ENDIF(H3DAPI_FOUND)

  #H3DUtil
  FIND_PACKAGE(H3DUtil REQUIRED)

  IF(H3DUTIL_FOUND)
    INCLUDE_DIRECTORIES( ${H3DUTIL_INCLUDE_DIR} ) 
    SET(requiredLibs ${requiredLibs} ${H3DUTIL_LIBRARIES} )
  ENDIF(H3DUTIL_FOUND)

  #HAPI
  FIND_PACKAGE(HAPI REQUIRED)

  IF(HAPI_FOUND)
    INCLUDE_DIRECTORIES( ${HAPI_INCLUDE_DIR} ) 
    SET(requiredLibs ${requiredLibs} ${HAPI_LIBRARIES} )
  ENDIF(HAPI_FOUND)
ENDIF( H3D_USE_DEPENDENCIES_ONLY )

# Glut.
FIND_PACKAGE(GLUT REQUIRED)

IF( NOT GLUT_FOUND AND WIN32 )
  FIND_PACKAGE(GLUTWin REQUIRED)
ENDIF( NOT GLUT_FOUND AND WIN32 )

IF(GLUT_FOUND)
  INCLUDE_DIRECTORIES( ${GLUT_INCLUDE_DIR} )
  IF( WIN32 )
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
IF(WIN32) 
  IF(MSVC80)
    # Remove some warnings about deprecated functions. Do not treat wchar_t as built in type
    SET_TARGET_PROPERTIES( Sphere_X3D PROPERTIES COMPILE_FLAGS "-D_CRT_SECURE_NO_DEPRECATE /Zc:wchar_t-" )
    SET_TARGET_PROPERTIES( Spheres_X3D PROPERTIES COMPILE_FLAGS "-D_CRT_SECURE_NO_DEPRECATE /Zc:wchar_t-" )
  ELSEIF(MSVC90)
    # Remove some warnings about deprecated functions. Do not treat wchar_t as built in type. Compile on multiple processors/threads if possible.
    SET_TARGET_PROPERTIES( Sphere_X3D PROPERTIES COMPILE_FLAGS "-D_CRT_SECURE_NO_DEPRECATE /Zc:wchar_t- /MP" )
    SET_TARGET_PROPERTIES( Spheres_X3D PROPERTIES COMPILE_FLAGS "-D_CRT_SECURE_NO_DEPRECATE /Zc:wchar_t- /MP" )
  ENDIF(MSVC80)

  IF( CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT AND NOT H3D_USE_DEPENDENCIES_ONLY)
    SET( CMAKE_INSTALL_PREFIX ${ManualExamples_SOURCE_DIR}/../../../.. CACHE PATH "Install path prefix, prepended onto install directories." FORCE )
  ENDIF( CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT AND NOT H3D_USE_DEPENDENCIES_ONLY)
ENDIF(WIN32)

# Install to these directories.
INSTALL( TARGETS Sphere_X3D
         LIBRARY DESTINATION lib
         RUNTIME DESTINATION bin )

INSTALL( TARGETS Spheres_X3D
         LIBRARY DESTINATION lib
         RUNTIME DESTINATION bin )

