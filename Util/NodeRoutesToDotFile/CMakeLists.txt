IF(WIN32)
  cmake_minimum_required(VERSION 2.6.0)
ENDIF(WIN32)

# The name of our project is "NodeRoutesToDotFile".  CMakeLists files in this project can
# refer to the root source directory of the project as ${NodeRoutesToDotFile_SOURCE_DIR} and
# to the root binary directory of the project as ${NodeRoutesToDotFile_BINARY_DIR}.
project (NodeRoutesToDotFile)

# Add a cache variable NodeRoutesToDotFile_EXTRA_RELEASE_LIBRARIES used
# to give extra release libraries which have a database which
# dot-files should be generated for.
IF( NOT DEFINED NodeRoutesToDotFile_EXTRA_RELEASE_LIBRARIES )
  SET( NodeRoutesToDotFile_EXTRA_RELEASE_LIBRARIES "" CACHE STRING "Add paths to extra release libraries that contain additional H3D nodes for which to generate dot-files. Each library path must be enclosed in \" and separated by semi colon(;)" )
  MARK_AS_ADVANCED(NodeRoutesToDotFile_EXTRA_RELEASE_LIBRARIES)
ENDIF( NOT DEFINED NodeRoutesToDotFile_EXTRA_RELEASE_LIBRARIES )

# Add a cache variable NodeRoutesToDotFile_EXTRA_DEBUG_LIBRARIES used
# to give extra debug libraries which have a database which
# dot-files should be generated for.
IF( NOT DEFINED NodeRoutesToDotFile_EXTRA_DEBUG_LIBRARIES )
  SET( NodeRoutesToDotFile_EXTRA_DEBUG_LIBRARIES "" CACHE STRING "Add paths to extra debug libraries that contain additional H3D nodes for which to generate dot-files. Each library path must be enclosed in \" and separated by semi colon(;)" )
  MARK_AS_ADVANCED(NodeRoutesToDotFile_EXTRA_DEBUG_LIBRARIES)
ENDIF( NOT DEFINED NodeRoutesToDotFile_EXTRA_DEBUG_LIBRARIES )

SET( NodeRoutesToDotFile_EXTRA_RELEASE_LIBRARIES_CODE )
FOREACH( NodeRoutesToDotFile_EXTRA_LIBRARY ${NodeRoutesToDotFile_EXTRA_RELEASE_LIBRARIES} )
  FILE( TO_CMAKE_PATH ${NodeRoutesToDotFile_EXTRA_LIBRARY} NodeRoutesToDotFile_EXTRA_LIBRARY_CMAKE_PATH )
  SET( NodeRoutesToDotFile_EXTRA_RELEASE_LIBRARIES_CODE "${NodeRoutesToDotFile_EXTRA_RELEASE_LIBRARIES_CODE}\n  extra_libraries.push_back( ${NodeRoutesToDotFile_EXTRA_LIBRARY_CMAKE_PATH} );" )
ENDFOREACH( NodeRoutesToDotFile_EXTRA_LIBRARY ${NodeRoutesToDotFile_EXTRA_RELEASE_LIBRARIES} )

SET( NodeRoutesToDotFile_EXTRA_DEBUG_LIBRARIES_CODE )
FOREACH( NodeRoutesToDotFile_EXTRA_LIBRARY ${NodeRoutesToDotFile_EXTRA_DEBUG_LIBRARIES} )
  FILE( TO_CMAKE_PATH ${NodeRoutesToDotFile_EXTRA_LIBRARY} NodeRoutesToDotFile_EXTRA_LIBRARY_CMAKE_PATH )
  SET( NodeRoutesToDotFile_EXTRA_DEBUG_LIBRARIES_CODE "${NodeRoutesToDotFile_EXTRA_DEBUG_LIBRARIES_CODE}\n extra_libraries.push_back( ${NodeRoutesToDotFile_EXTRA_LIBRARY_CMAKE_PATH} );" )
ENDFOREACH( NodeRoutesToDotFile_EXTRA_LIBRARY ${NodeRoutesToDotFile_EXTRA_DEBUG_LIBRARIES} )


# Where to find Source files.
SET( NodeRoutesToDotFile_SRCS "${NodeRoutesToDotFile_SOURCE_DIR}/NodeRoutesToDotFile.cpp" )

# Add optional libs to this variable.
SET(optionalLibs)

# Include directories.
INCLUDE_DIRECTORIES( . )

# Add required libs to this variable.
SET(requiredLibs)

# Where to find modules, used when finding packages.
SET(CMAKE_MODULE_PATH "${NodeRoutesToDotFile_SOURCE_DIR}/../../build/modules/")

IF( H3D_USE_DEPENDENCIES_ONLY )
  # The variables set here must be set by the CMakeLists.txt that sets H3D_USE_DEPENDENCIES_ONLY to true.
  INCLUDE_DIRECTORIES( ${EXTERNAL_INCLUDE_DIR} ) 
  INCLUDE_DIRECTORIES( ${H3DUTIL_INCLUDE_DIR} ) 
  INCLUDE_DIRECTORIES( ${HAPI_INCLUDE_DIR} ) 
  INCLUDE_DIRECTORIES( ${H3DAPI_INCLUDE_DIR} ) 
  INCLUDE_DIRECTORIES( ${UI_INCLUDE_DIR} )
  INCLUDE_DIRECTORIES( ${H3DPhysics_INCLUDE_DIR} )
  
  SET( requiredLibs ${requiredLibs} H3DUtil HAPI H3DAPI UI H3DPhysics )
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

  #UI
  SET(CMAKE_MODULE_PATH "${NodeRoutesToDotFile_SOURCE_DIR}")
  FIND_PACKAGE(UI)

  IF(UI_FOUND)
    INCLUDE_DIRECTORIES( ${UI_INCLUDE_DIR} ) 
    SET(optionalLibs ${optionalLibs} ${UI_LIBRARIES} )
  ENDIF(UI_FOUND)

  #H3DPhysics
  FIND_PACKAGE(H3DPhysics)

  IF(H3DPhysics_FOUND)
    INCLUDE_DIRECTORIES( ${H3DPhysics_INCLUDE_DIR} ) 
    SET(optionalLibs ${optionalLibs} ${H3DPhysics_LIBRARIES} )
  ENDIF(H3DPhysics_FOUND)
ENDIF( H3D_USE_DEPENDENCIES_ONLY )

SET(CMAKE_MODULE_PATH "${NodeRoutesToDotFile_SOURCE_DIR}/../../build/modules/")

# Optional extra library. Used for sound in H3D API.
FIND_PACKAGE(H3DOpenAL)
IF(OPENAL_FOUND)
  INCLUDE_DIRECTORIES( ${OPENAL_INCLUDE_DIR} )
  SET(optionalLibs ${optionalLibs} ${OPENAL_LIBRARIES} )
ENDIF(OPENAL_FOUND)

CONFIGURE_FILE( ${NodeRoutesToDotFile_SOURCE_DIR}/NodeRoutesToDotFile.cmake ${NodeRoutesToDotFile_SOURCE_DIR}/NodeRoutesToDotFile.cpp )

# Create build filess.
ADD_EXECUTABLE(NodeRoutesToDotFile  ${NodeRoutesToDotFile_SRCS})
TARGET_LINK_LIBRARIES( NodeRoutesToDotFile ${requiredLibs} ${optionalLibs} )

# Debug version should have _d postfix.
SET_TARGET_PROPERTIES( NodeRoutesToDotFile PROPERTIES DEBUG_POSTFIX "_d" )

# Variable used to set compile properties.
SET( NODEROUTESTODOTFILE_COMPILE_FLAGS "-DH3D_GENERATE_DOTROUTE_FILES" )

SET( DEFAULT_BIN_INSTALL "bin" )
SET( DEFAULT_LIB_INSTALL "lib" )

IF( WIN32 )
  SET( DEFAULT_BIN_INSTALL "bin32" )
  SET( DEFAULT_LIB_INSTALL "lib32" )
  IF( CMAKE_SIZEOF_VOID_P EQUAL 8 )
    SET( DEFAULT_BIN_INSTALL "bin64" )
    SET( DEFAULT_LIB_INSTALL "lib64" )
  ENDIF( CMAKE_SIZEOF_VOID_P EQUAL 8 )
  
  IF( UI_FOUND OR H3D_USE_DEPENDENCIES_ONLY )
    SET( NODEROUTESTODOTFILE_COMPILE_FLAGS "${NODEROUTESTODOTFILE_COMPILE_FLAGS} -DHAVE_UI" )
  ENDIF( UI_FOUND OR H3D_USE_DEPENDENCIES_ONLY )

  IF( H3DPhysics_FOUND OR H3D_USE_DEPENDENCIES_ONLY )
    SET( NODEROUTESTODOTFILE_COMPILE_FLAGS "${NODEROUTESTODOTFILE_COMPILE_FLAGS} -DHAVE_H3DPhysics" )
  ENDIF( H3DPhysics_FOUND OR H3D_USE_DEPENDENCIES_ONLY )
  
  # Treat wchar_t as built in type for all visual studio versions.
  # This is default for every version above 7 ( so far ) but we still set it for all.
  SET( NODEROUTESTODOTFILE_COMPILE_FLAGS "${NODEROUTESTODOTFILE_COMPILE_FLAGS} /Zc:wchar_t")

  IF( ${MSVC_VERSION} GREATER 1399 )
    # Remove compiler warnings about deprecation for visual studio versions 8 and above.
    SET( NODEROUTESTODOTFILE_COMPILE_FLAGS "${NODEROUTESTODOTFILE_COMPILE_FLAGS} -D_CRT_SECURE_NO_DEPRECATE" )
  ENDIF( ${MSVC_VERSION} GREATER 1399 )
  
  IF( ${MSVC_VERSION} GREATER 1499 )
    # Build using several threads for visual studio versions 9 and above.
    SET( NODEROUTESTODOTFILE_COMPILE_FLAGS "${NODEROUTESTODOTFILE_COMPILE_FLAGS} /MP" )
  ENDIF( ${MSVC_VERSION} GREATER 1499 )

ENDIF(WIN32)

SET_TARGET_PROPERTIES( NodeRoutesToDotFile PROPERTIES COMPILE_FLAGS "${NODEROUTESTODOTFILE_COMPILE_FLAGS}" )

IF( H3D_USE_DEPENDENCIES_ONLY )
  INSTALL( TARGETS NodeRoutesToDotFile
           LIBRARY DESTINATION ${DEFAULT_LIB_INSTALL}
           RUNTIME DESTINATION ${DEFAULT_BIN_INSTALL} )
ENDIF( H3D_USE_DEPENDENCIES_ONLY )