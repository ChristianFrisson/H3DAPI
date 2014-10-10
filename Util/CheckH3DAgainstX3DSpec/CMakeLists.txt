IF(WIN32)
  cmake_minimum_required(VERSION 2.6.0)
ENDIF(WIN32)

# The name of our project is "CheckH3DAgainstX3DSpec".  CMakeLists files in this project can
# refer to the root source directory of the project as ${CheckH3DAgainstX3DSpec_SOURCE_DIR} and
# to the root binary directory of the project as ${CheckH3DAgainstX3DSpec_BINARY_DIR}.
project(CheckH3DAgainstX3DSpec)

# Where to find Source files.
SET( CheckH3DAgainstX3DSpec_SRCS "${CheckH3DAgainstX3DSpec_SOURCE_DIR}/CheckH3DAgainstX3DSpec.cpp" )

# Add optional libs to this variable.
SET(optionalLibs)

# Include directories.
INCLUDE_DIRECTORIES( . )

# Add required libs to this variable.
SET(requiredLibs)

# Where to find modules, used when finding packages.
SET(CMAKE_MODULE_PATH "${CheckH3DAgainstX3DSpec_SOURCE_DIR}/../../build/modules/")

IF( H3D_USE_DEPENDENCIES_ONLY )
  # The variables set here must be set by the CMakeLists.txt that sets H3D_USE_DEPENDENCIES_ONLY to true.
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

SET( H3D_MSVC_VERSION 6 )
SET( TEMP_MSVC_VERSION 1299 )
WHILE( ${MSVC_VERSION} GREATER ${TEMP_MSVC_VERSION} )
  MATH( EXPR H3D_MSVC_VERSION "${H3D_MSVC_VERSION} + 1" )
  MATH( EXPR TEMP_MSVC_VERSION "${TEMP_MSVC_VERSION} + 100" )
ENDWHILE( ${MSVC_VERSION} GREATER ${TEMP_MSVC_VERSION} )

SET( DEFAULT_BIN_INSTALL "bin" )

IF( WIN32 )
  SET( DEFAULT_BIN_INSTALL "bin32" )
  IF( CMAKE_SIZEOF_VOID_P EQUAL 8 )
    SET( DEFAULT_BIN_INSTALL "bin64" )
  ENDIF( CMAKE_SIZEOF_VOID_P EQUAL 8 )
ENDIF( WIN32 )

#Geospatial
OPTION( INCLUDE_Geospatial
        "Add Geospatial nodes to list"
        OFF )
IF( INCLUDE_Geospatial )
  IF( TARGET Geospatial )
    SET( Geospatial_SEARCH_PATH "${CMAKE_INSTALL_PREFIX}/bin" )
  ELSE( TARGET Geospatial )
    SET( CMAKE_MODULE_PATH "${CheckH3DAgainstX3DSpec_SOURCE_DIR}" )
    FIND_FILE(Geospatial_DLL "Geospatial" )

    IF(Geospatial_DLL)
      get_filename_component( Geospatial_SEARCH_PATH ${Geospatial_DLL} PATH )
      STRING( REGEX REPLACE "/([^/])*$" "/bin" Geospatial_SEARCH_PATH "${Geospatial_SEARCH_PATH}" )
    ENDIF(Geospatial_DLL)
  ENDIF( TARGET Geospatial )
ENDIF( INCLUDE_Geospatial )

#H3DPhysics
OPTION( INCLUDE_H3DPhysics
        "Add H3DPhysics nodes to list"
        OFF )
IF( INCLUDE_H3DPhysics )
  IF( TARGET H3DPhysics )
    SET( H3DPhysics_SEARCH_PATH "${CMAKE_INSTALL_PREFIX}/${DEFAULT_BIN_INSTALL}" )
  ELSE( TARGET H3DPhysics )
    SET( CMAKE_MODULE_PATH "${CheckH3DAgainstX3DSpec_SOURCE_DIR}/../NodeRoutesToDotFile/" )
    FIND_PACKAGE(H3DPhysics)

    IF(H3DPhysics_FOUND)
      get_filename_component( H3DPhysics_SEARCH_PATH ${H3DPhysics_LIBRARY} PATH )
      STRING( REGEX REPLACE "/([^/])*$" "/${DEFAULT_BIN_INSTALL}" H3DPhysics_SEARCH_PATH "${H3DPhysics_SEARCH_PATH}" )
    ENDIF(H3DPhysics_FOUND)
  ENDIF( TARGET H3DPhysics )
ENDIF( INCLUDE_H3DPhysics )

#MedX3D
OPTION( INCLUDE_MedX3D
        "Add MedX3D nodes to list"
        OFF )
IF( INCLUDE_MedX3D )
  IF( TARGET MedX3D )
    SET( MedX3D_SEARCH_PATH "${CMAKE_INSTALL_PREFIX}/${DEFAULT_BIN_INSTALL}" )
  ELSE( TARGET MedX3D )
    SET( CMAKE_MODULE_PATH "${CheckH3DAgainstX3DSpec_SOURCE_DIR}/../CreateH3DNodesFieldsList/" )
    FIND_PACKAGE(MedX3D)

    IF(MEDX3D_FOUND)
      get_filename_component( MedX3D_SEARCH_PATH ${MEDX3D_LIBRARY} PATH )
      STRING( REGEX REPLACE "/([^/])*$" "/${DEFAULT_BIN_INSTALL}" MedX3D_SEARCH_PATH "${MedX3D_SEARCH_PATH}" )
    ENDIF(MEDX3D_FOUND)
  ENDIF( TARGET MedX3D )
ENDIF( INCLUDE_MedX3D )

CONFIGURE_FILE( ${CheckH3DAgainstX3DSpec_SOURCE_DIR}/CheckH3DAgainstX3DSpec.cmake ${CheckH3DAgainstX3DSpec_SOURCE_DIR}/CheckH3DAgainstX3DSpec.cpp )

SET(CMAKE_MODULE_PATH "${CheckH3DAgainstX3DSpec_SOURCE_DIR}/../../build/modules/")

# Create build filess.
ADD_EXECUTABLE(CheckH3DAgainstX3DSpec  ${CheckH3DAgainstX3DSpec_SRCS})
TARGET_LINK_LIBRARIES( CheckH3DAgainstX3DSpec ${requiredLibs} ${optionalLibs} )

# Debug version should have _d postfix.
SET_TARGET_PROPERTIES( CheckH3DAgainstX3DSpec PROPERTIES DEBUG_POSTFIX "_d" )

# Variable used to set compile properties.
SET( CheckH3DAgainstX3DSpec_COMPILE_FLAGS "" )

OPTION( DONT_LIST_INITIALIZE_TO_INPUT_OUTPUT
        "If true and X3D specification lists a field access type as INITIALIZE_ONLY and H3D lists it as INPUT_OUTPUT then don't show this difference"
        OFF )
IF( DONT_LIST_INITIALIZE_TO_INPUT_OUTPUT )
    SET( CheckH3DAgainstX3DSpec_COMPILE_FLAGS "${CheckH3DAgainstX3DSpec_COMPILE_FLAGS} -DDONT_LIST_INITIALIZE_TO_INPUT_OUTPUT" )
ENDIF( DONT_LIST_INITIALIZE_TO_INPUT_OUTPUT )

# set the install directory to the H3D directory on Windows
IF(WIN32) 
  IF( Geospatial_DLL )
    SET( CheckH3DAgainstX3DSpec_COMPILE_FLAGS "${CheckH3DAgainstX3DSpec_COMPILE_FLAGS} -DHAVE_Geospatial" )
    ADD_DEPENDENCIES( CheckH3DAgainstX3DSpec Geospatial )
  ENDIF( Geospatial_DLL )

  IF( H3DPhysics_FOUND OR H3D_USE_DEPENDENCIES_ONLY )
    SET( CheckH3DAgainstX3DSpec_COMPILE_FLAGS "${CheckH3DAgainstX3DSpec_COMPILE_FLAGS} -DHAVE_H3DPhysics" )
    ADD_DEPENDENCIES( CheckH3DAgainstX3DSpec H3DPhysics )
  ENDIF( H3DPhysics_FOUND OR H3D_USE_DEPENDENCIES_ONLY )

  IF( MEDX3D_FOUND OR H3D_USE_DEPENDENCIES_ONLY )
    SET( CheckH3DAgainstX3DSpec_COMPILE_FLAGS "${CheckH3DAgainstX3DSpec_COMPILE_FLAGS} -DHAVE_MedX3D" )
    ADD_DEPENDENCIES( CheckH3DAgainstX3DSpec MedX3D )
  ENDIF( MEDX3D_FOUND OR H3D_USE_DEPENDENCIES_ONLY )

  # Set compile and link properties for projects.
  IF(MSVC80)
    # Remove some warnings about deprecated functions. Do not treat wchar_t as built in type
    SET( CheckH3DAgainstX3DSpec_COMPILE_FLAGS "${CheckH3DAgainstX3DSpec_COMPILE_FLAGS} -D_CRT_SECURE_NO_DEPRECATE /Zc:wchar_t-" )
  ELSEIF(MSVC90)
    # Remove some warnings about deprecated functions. Do not treat wchar_t as built in type. Compile on multiple processors/threads if possible.
    SET( CheckH3DAgainstX3DSpec_COMPILE_FLAGS "${CheckH3DAgainstX3DSpec_COMPILE_FLAGS} -D_CRT_SECURE_NO_DEPRECATE /Zc:wchar_t- /MP" )
  ENDIF(MSVC80)

  SET_TARGET_PROPERTIES( CheckH3DAgainstX3DSpec PROPERTIES COMPILE_FLAGS "${CheckH3DAgainstX3DSpec_COMPILE_FLAGS}" )

ENDIF(WIN32)
