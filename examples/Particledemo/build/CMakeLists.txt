IF(WIN32)
  cmake_minimum_required(VERSION 2.6.0)
ENDIF(WIN32)
# The name of our project is "ParticleDemo".  CMakeLists files in this project can
# refer to the root source directory of the project as ${ParticleDemo_SOURCE_DIR} and
# to the root binary directory of the project as ${ParticleDemo_BINARY_DIR}.
project (ParticleDemo)

# Source files.
AUX_SOURCE_DIRECTORY(../src ParticleDemo_SRCS)

# Include directories.
INCLUDE_DIRECTORIES( ../src )

# Add required libraries to this variable.
SET(requiredLibs)

# Where to find cmake modules used to find packages.
SET(CMAKE_MODULE_PATH "${ParticleDemo_SOURCE_DIR}/../../../build/modules/")
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
  IF( WIN32 )
    STRIP_AND_ADD_LIBRARY_DIRECTORIES( ${H3DAPI_LIBRARIES} )
    SET(requiredLibs ${requiredLibs} ${H3DAPI_LIBRARIES} )
  ELSE( WIN32 )
    SET(requiredLibs ${requiredLibs} ${H3DAPI_LIBRARIES} )
  ENDIF( WIN32 )
ENDIF(H3DAPI_FOUND)

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

# Xerces.
FIND_PACKAGE(Xerces)

IF(XERCES_FOUND)
  INCLUDE_DIRECTORIES( ${XERCES_INCLUDE_DIR} ) 
  SET(optionalLibs ${optionalLibs} ${XERCES_LIBRARIES} )
ENDIF(XERCES_FOUND)

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
  ELSEIF(MSVC80)
    # Remove some warnings about deprecated functions. Do not treat wchar_t as built in type
    SET_TARGET_PROPERTIES( ParticleDemo PROPERTIES COMPILE_FLAGS "-D_CRT_SECURE_NO_DEPRECATE /Zc:wchar_t-" )
    # Needed to run in debug mode. Removes warning about conflict between libraries.
    SET_TARGET_PROPERTIES( ParticleDemo PROPERTIES LINK_FLAGS_DEBUG "/NODEFAULTLIB:msvcrt" )
  ELSEIF(MSVC90)
    # Remove some warnings about deprecated functions. Do not treat wchar_t as built in type. Compile on multiple processors/threads if possible.
    SET_TARGET_PROPERTIES( ParticleDemo PROPERTIES COMPILE_FLAGS "-D_CRT_SECURE_NO_DEPRECATE /Zc:wchar_t- /MP" )
    # Needed to run in debug mode. Removes warning about conflict between libraries.
    SET_TARGET_PROPERTIES( ParticleDemo PROPERTIES LINK_FLAGS_DEBUG "/NODEFAULTLIB:msvcrt" )
  ENDIF(MSVC70 OR MSVC71)

  # set the install directory to the H3D directory on Windows
  SET( CMAKE_INSTALL_PREFIX ${ParticleDemo_SOURCE_DIR}/../../../..)
ENDIF(WIN32)

INSTALL( TARGETS ParticleDemo
         LIBRARY DESTINATION lib
         RUNTIME DESTINATION bin )
