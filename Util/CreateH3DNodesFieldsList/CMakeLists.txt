IF(WIN32)
  cmake_minimum_required(VERSION 2.6.0)
ENDIF(WIN32)

# The name of our project is "CreateH3DNodesFieldsList".  CMakeLists files in this project can
# refer to the root source directory of the project as ${CreateH3DNodesFieldsList_SOURCE_DIR} and
# to the root binary directory of the project as ${CreateH3DNodesFieldsList_BINARY_DIR}.
project(CreateH3DNodesFieldsList)

# Where to find Source files.
SET( CreateH3DNodesFieldsList_SRCS "${CreateH3DNodesFieldsList_SOURCE_DIR}/CreateH3DNodesFieldsList.cpp" )

# Add optional libs to this variable.
SET(optionalLibs)

# Include directories.
INCLUDE_DIRECTORIES( . )

# Add required libs to this variable.
SET(requiredLibs)

# Where to find modules, used when finding packages.
SET(CMAKE_MODULE_PATH "${CreateH3DNodesFieldsList_SOURCE_DIR}/../../build/modules/")

IF( H3D_USE_DEPENDENCIES_ONLY )
  # The variables set here must be set by the CMakeLists.txt that sets H3D_USE_DEPENDENCIES_ONLY to true.
  IF(PTHREAD_FOUND)
    INCLUDE_DIRECTORIES( ${PTHREAD_INCLUDE_DIR} ) 
  ENDIF(PTHREAD_FOUND)
  INCLUDE_DIRECTORIES( ${EXTERNAL_INCLUDE_DIR} ) 
  INCLUDE_DIRECTORIES( ${H3DUTIL_INCLUDE_DIR} ) 
  INCLUDE_DIRECTORIES( ${HAPI_INCLUDE_DIR} ) 
  INCLUDE_DIRECTORIES( ${H3DAPI_INCLUDE_DIR} ) 
  INCLUDE_DIRECTORIES( ${UI_INCLUDE_DIR} )
  INCLUDE_DIRECTORIES( ${H3DPhysics_INCLUDE_DIR} )
  INCLUDE_DIRECTORIES( ${MedX3D_INCLUDE_DIR} )
  
  SET( requiredLibs ${requiredLibs} H3DUtil HAPI H3DAPI UI H3DPhysics MedX3D)
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

#UI
OPTION( INCLUDE_UI
        "Add UI nodes to list"
        OFF )
IF( INCLUDE_UI )
  SET( CMAKE_MODULE_PATH "${CreateH3DNodesFieldsList_SOURCE_DIR}/../NodeRoutesToDotFile/" )
  FIND_PACKAGE(UI)

  IF(UI_FOUND)
    INCLUDE_DIRECTORIES( ${UI_INCLUDE_DIR} ) 
    SET(optionalLibs ${optionalLibs} ${UI_LIBRARIES} )
  ENDIF(UI_FOUND)
ENDIF( INCLUDE_UI )

#H3DPhysics
OPTION( INCLUDE_H3DPhysics
        "Add H3DPhysics nodes to list"
        OFF )
IF( INCLUDE_H3DPhysics )
  FIND_PACKAGE(H3DPhysics)

  IF(H3DPhysics_FOUND)
    INCLUDE_DIRECTORIES( ${H3DPhysics_INCLUDE_DIR} ) 
    SET(optionalLibs ${optionalLibs} ${H3DPhysics_LIBRARIES} )
  ENDIF(H3DPhysics_FOUND)
ENDIF( INCLUDE_H3DPhysics )

#MedX3D
OPTION( INCLUDE_MedX3D
        "Add MedX3D nodes to list"
        OFF )
IF( INCLUDE_MedX3D )
  SET( CMAKE_MODULE_PATH "${CreateH3DNodesFieldsList_SOURCE_DIR}" )
  FIND_PACKAGE(MedX3D)

  IF(MedX3D_FOUND)
    INCLUDE_DIRECTORIES( ${MedX3D_INCLUDE_DIR} ) 
    SET(optionalLibs ${optionalLibs} ${MedX3D_LIBRARIES} )
  ENDIF(MedX3D_FOUND)
ENDIF( INCLUDE_MedX3D )

# Optional extra library. Used for sound in H3D API.
FIND_PACKAGE(H3DOpenAL)
IF(OPENAL_FOUND)
  INCLUDE_DIRECTORIES( ${OPENAL_INCLUDE_DIR} )
  SET(optionalLibs ${optionalLibs} ${OPENAL_LIBRARIES} )
ENDIF(OPENAL_FOUND)

SET(CMAKE_MODULE_PATH "${CreateH3DNodesFieldsList_SOURCE_DIR}/../../build/modules/")

# Create build filess.
ADD_EXECUTABLE(CreateH3DNodesFieldsList  ${CreateH3DNodesFieldsList_SRCS})
TARGET_LINK_LIBRARIES( CreateH3DNodesFieldsList ${requiredLibs} ${optionalLibs} )

# Debug version should have _d postfix.
SET_TARGET_PROPERTIES( CreateH3DNodesFieldsList PROPERTIES DEBUG_POSTFIX "_d" )

# set the install directory to the H3D directory on Windows
IF(WIN32) 
  # Variable used to set compile properties.
  SET( CreateH3DNodesFieldsList_COMPILE_FLAGS "" )

  IF( UI_FOUND )
    SET( CreateH3DNodesFieldsList_COMPILE_FLAGS "${CreateH3DNodesFieldsList_COMPILE_FLAGS} -DHAVE_UI" )
  ENDIF( UI_FOUND )

  IF( H3DPhysics_FOUND )
    SET( CreateH3DNodesFieldsList_COMPILE_FLAGS "${CreateH3DNodesFieldsList_COMPILE_FLAGS} -DHAVE_H3DPhysics" )
  ENDIF( H3DPhysics_FOUND )

  IF( MedX3D_FOUND )
    SET( CreateH3DNodesFieldsList_COMPILE_FLAGS "${CreateH3DNodesFieldsList_COMPILE_FLAGS} -DHAVE_MedX3D" )
  ENDIF( MedX3D_FOUND )

  # Set compile and link properties for projects.
  IF(MSVC80)
    # Remove some warnings about deprecated functions. Do not treat wchar_t as built in type
    SET( CreateH3DNodesFieldsList_COMPILE_FLAGS "${CreateH3DNodesFieldsList_COMPILE_FLAGS} -D_CRT_SECURE_NO_DEPRECATE /Zc:wchar_t-" )
  ELSEIF(MSVC90)
    # Remove some warnings about deprecated functions. Do not treat wchar_t as built in type. Compile on multiple processors/threads if possible.
    SET( CreateH3DNodesFieldsList_COMPILE_FLAGS "${CreateH3DNodesFieldsList_COMPILE_FLAGS} -D_CRT_SECURE_NO_DEPRECATE /Zc:wchar_t- /MP" )
  ENDIF(MSVC80)

  SET_TARGET_PROPERTIES( CreateH3DNodesFieldsList PROPERTIES COMPILE_FLAGS "${CreateH3DNodesFieldsList_COMPILE_FLAGS}" )

ENDIF(WIN32)
