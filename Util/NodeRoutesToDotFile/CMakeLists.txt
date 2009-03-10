IF(WIN32)
  cmake_minimum_required(VERSION 2.6.0)
ENDIF(WIN32)

# The name of our project is "NodeRoutesToDotFile".  CMakeLists files in this project can
# refer to the root source directory of the project as ${NodeRoutesToDotFile_SOURCE_DIR} and
# to the root binary directory of the project as ${NodeRoutesToDotFile_BINARY_DIR}.
project (NodeRoutesToDotFile)

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

# Optional extra library. Used for sound in H3D API.
FIND_PACKAGE(H3DOpenAL)
IF(OPENAL_FOUND)
  INCLUDE_DIRECTORIES( ${OPENAL_INCLUDE_DIR} )
  SET(optionalLibs ${optionalLibs} ${OPENAL_LIBRARIES} )
ENDIF(OPENAL_FOUND)

#UI
SET(CMAKE_MODULE_PATH "${NodeRoutesToDotFile_SOURCE_DIR}")
FIND_PACKAGE(UI)

IF(UI_FOUND)
  INCLUDE_DIRECTORIES( ${UI_INCLUDE_DIR} ) 
  SET(optionalLibs ${optionalLibs} ${UI_LIBRARIES} )
ENDIF(UI_FOUND)
SET(CMAKE_MODULE_PATH "${NodeRoutesToDotFile_SOURCE_DIR}/../../build/modules/")

# Create build filess.
ADD_EXECUTABLE(NodeRoutesToDotFile  ${NodeRoutesToDotFile_SRCS})
TARGET_LINK_LIBRARIES( NodeRoutesToDotFile ${requiredLibs} ${optionalLibs} )

# Debug version should have _d postfix.
SET_TARGET_PROPERTIES( NodeRoutesToDotFile PROPERTIES DEBUG_POSTFIX "_d" )

# set the install directory to the H3D directory on Windows
IF(WIN32) 
  # Variable used to set compile properties.
  SET( NODEROUTESTODOTFILE_COMPILE_FLAGS "" )

  IF( UI_FOUND )
    SET( NODEROUTESTODOTFILE_COMPILE_FLAGS "${NODEROUTESTODOTFILE_COMPILE_FLAGS} -DHAVE_UI" )
  ENDIF( UI_FOUND )

  # Set compile and link properties for projects.
  IF(MSVC80)
    # Remove some warnings about deprecated functions. Do not treat wchar_t as built in type
    SET( NODEROUTESTODOTFILE_COMPILE_FLAGS "${NODEROUTESTODOTFILE_COMPILE_FLAGS} -D_CRT_SECURE_NO_DEPRECATE /Zc:wchar_t-" )
  ELSEIF(MSVC90)
    # Remove some warnings about deprecated functions. Do not treat wchar_t as built in type. Compile on multiple processors/threads if possible.
    SET( NODEROUTESTODOTFILE_COMPILE_FLAGS "${NODEROUTESTODOTFILE_COMPILE_FLAGS} -D_CRT_SECURE_NO_DEPRECATE /Zc:wchar_t- /MP" )
  ENDIF(MSVC80)

  SET_TARGET_PROPERTIES( NodeRoutesToDotFile PROPERTIES COMPILE_FLAGS "${NODEROUTESTODOTFILE_COMPILE_FLAGS}" )

ENDIF(WIN32)
