cmake_minimum_required(VERSION 2.6.0)

# The name of our project is "H3DLoad".  CMakeLists files in this project can
# refer to the root source directory of the project as ${H3DLoad_SOURCE_DIR} and
# to the root binary directory of the project as ${H3DLoad_BINARY_DIR}.
project (H3DLoad)

# Where to find Source files.
AUX_SOURCE_DIRECTORY(../src H3DLoad_SRCS)

# Add optional libs to this variable.
SET(optionalLibs)

# Include directories.
INCLUDE_DIRECTORIES( ../src )

# Add required libs to this variable.
SET(requiredLibs)

# Where to find modules, used when finding packages.
SET(CMAKE_MODULE_PATH "${H3DLoad_SOURCE_DIR}/../../build/modules/")
INCLUDE( StripAndAddLibraryDirectories )

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

#H3DAPI
FIND_PACKAGE(H3DAPI REQUIRED)

IF(H3DAPI_FOUND)
  INCLUDE_DIRECTORIES( ${H3DAPI_INCLUDE_DIR} ) 
  SET(requiredLibs ${requiredLibs} ${H3DAPI_LIBRARIES} )
ENDIF(H3DAPI_FOUND)

# OpenGL
FIND_PACKAGE(OpenGL REQUIRED)

IF(OPENGL_FOUND)
  INCLUDE_DIRECTORIES( ${OPENGL_INCLUDE_DIR} ) 
  SET(requiredLibs ${requiredLibs} ${OPENGL_LIBRARIES} )
ENDIF(OPENGL_FOUND)

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

# Xerces.
FIND_PACKAGE(Xerces)

IF(XERCES_FOUND)
  INCLUDE_DIRECTORIES( ${XERCES_INCLUDE_DIR} ) 
  SET(optionalLibs ${optionalLibs} ${XERCES_LIBRARIES} )
ENDIF(XERCES_FOUND)

# Create build filess.
ADD_EXECUTABLE(H3DLoad  ${H3DLoad_SRCS})
TARGET_LINK_LIBRARIES( H3DLoad ${requiredLibs} ${optionalLibs} )

# Debug version should have _d postfix.
SET_TARGET_PROPERTIES( H3DLoad PROPERTIES DEBUG_POSTFIX "_d" )

# set the install directory to the H3D directory on Windows
IF(WIN32) 
  # Set compile and link properties for projects.
  IF(MSVC80)
    # Remove some warnings about deprecated functions. Do not treat wchar_t as built in type
    SET_TARGET_PROPERTIES( H3DLoad PROPERTIES COMPILE_FLAGS "-D_CRT_SECURE_NO_DEPRECATE /Zc:wchar_t-" )
  ELSEIF(MSVC90)
    # Remove some warnings about deprecated functions. Do not treat wchar_t as built in type. Compile on multiple processors/threads if possible.
    SET_TARGET_PROPERTIES( H3DLoad PROPERTIES COMPILE_FLAGS "-D_CRT_SECURE_NO_DEPRECATE /Zc:wchar_t- /MP" )
  ENDIF(MSVC80)

 SET( CMAKE_INSTALL_PREFIX ${H3DLoad_SOURCE_DIR}/../../..)
ENDIF(WIN32)

# Install to this destination.
INSTALL( TARGETS H3DLoad
         LIBRARY DESTINATION lib
         RUNTIME DESTINATION bin )

