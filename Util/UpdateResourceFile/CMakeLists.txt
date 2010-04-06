IF(WIN32)
  cmake_minimum_required(VERSION 2.6.0)
ENDIF(WIN32)

# The name of our project is "UpdateResourceFile".  CMakeLists files in this project can
# refer to the root source directory of the project as ${UpdateResourceFile_SOURCE_DIR} and
# to the root binary directory of the project as ${UpdateResourceFile_BINARY_DIR}.
project (UpdateResourceFile)

# Where to find Source files.
SET( UpdateResourceFile_SRCS "${UpdateResourceFile_SOURCE_DIR}/UpdateResourceFile.cpp" )

# Add optional libs to this variable.
SET(optionalLibs)

# Include directories.
INCLUDE_DIRECTORIES( . )

# Add required libs to this variable.
SET(requiredLibs)

# Where to find modules, used when finding packages.
SET(CMAKE_MODULE_PATH "${UpdateResourceFile_SOURCE_DIR}/../../build/modules/")

# Create build filess.
ADD_EXECUTABLE(UpdateResourceFile  ${UpdateResourceFile_SRCS})
TARGET_LINK_LIBRARIES( UpdateResourceFile ${requiredLibs} ${optionalLibs} )

# Debug version should have _d postfix.
SET_TARGET_PROPERTIES( UpdateResourceFile PROPERTIES DEBUG_POSTFIX "_d" )

# set the install directory to the H3D directory on Windows
IF(WIN32) 
  # Variable used to set compile properties.
  SET( UPDATERESOURCEFILE_COMPILE_FLAGS "" )
  # Set compile and link properties for projects.
  IF(MSVC80)
    # Remove some warnings about deprecated functions. Do not treat wchar_t as built in type
    SET( UPDATERESOURCEFILE_COMPILE_FLAGS "${UPDATERESOURCEFILE_COMPILE_FLAGS} -D_CRT_SECURE_NO_DEPRECATE /Zc:wchar_t-" )
  ELSEIF(MSVC90)
    # Remove some warnings about deprecated functions. Do not treat wchar_t as built in type. Compile on multiple processors/threads if possible.
    SET( UPDATERESOURCEFILE_COMPILE_FLAGS "${UPDATERESOURCEFILE_COMPILE_FLAGS} -D_CRT_SECURE_NO_DEPRECATE /Zc:wchar_t- /MP" )
  ENDIF(MSVC80)

  SET_TARGET_PROPERTIES( UpdateResourceFile PROPERTIES COMPILE_FLAGS "${UPDATERESOURCEFILE_COMPILE_FLAGS}" )

ENDIF(WIN32)
