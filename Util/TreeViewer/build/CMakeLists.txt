IF(WIN32)
  cmake_minimum_required(VERSION 2.6.0)
ENDIF(WIN32)
# The name of our project is "TreeViewer".  CMakeLists files in this project can
# refer to the root source directory of the project as ${TreeViewer_SOURCE_DIR} and
# to the root binary directory of the project as ${TreeViewer_BINARY_DIR}.
project (TreeViewer)

# Source files.
SET( TreeViewer_SRCS "${TreeViewer_SOURCE_DIR}/../src/ConsoleDialog.cpp"
                     "${TreeViewer_SOURCE_DIR}/../src/ConsoleDialog.h"
                     "${TreeViewer_SOURCE_DIR}/../src/TreeViewer.cpp"
                     "${TreeViewer_SOURCE_DIR}/../src/TreeViewer.h"
                     "${TreeViewer_SOURCE_DIR}/../src/TreeViewerApp.cpp"
                     "${TreeViewer_SOURCE_DIR}/../src/TreeViewerApp.h"
                     "${TreeViewer_SOURCE_DIR}/../src/TreeViewerFieldValuesDialog.cpp"
                     "${TreeViewer_SOURCE_DIR}/../src/TreeViewerFieldValuesDialog.h"
                     "${TreeViewer_SOURCE_DIR}/../src/TreeViewerMainFrame.cpp"
                     "${TreeViewer_SOURCE_DIR}/../src/TreeViewerMainFrame.h"
                     "${TreeViewer_SOURCE_DIR}/../src/TreeViewerTreeViewDialog.cpp"
                     "${TreeViewer_SOURCE_DIR}/../src/TreeViewerTreeViewDialog.h"
                     "${TreeViewer_SOURCE_DIR}/../src/WxWidgetsWindow.cpp"
                     "${TreeViewer_SOURCE_DIR}/../src/WxWidgetsWindow.h" )

# Include directories.
INCLUDE_DIRECTORIES( ../src )

# Add required libs to this variable.
SET(requiredLibs)

# Where to find cmake modules used to find packages.
SET(CMAKE_MODULE_PATH $ENV{H3D_ROOT}/build/modules ${TreeViewer_SOURCE_DIR}/../../../build/modules )
INCLUDE( StripAndAddLibraryDirectories )

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

# Create build files.
ADD_EXECUTABLE(TreeViewer WIN32 MACOSX_BUNDLE ${TreeViewer_SRCS})

# Link against requiredLibs and optionalLibs.
TARGET_LINK_LIBRARIES( TreeViewer ${requiredLibs} ${optionalLibs} )

# Set postfix for debug version of TreeViewer.
SET_TARGET_PROPERTIES( TreeViewer PROPERTIES DEBUG_POSTFIX "_d" )

IF(WIN32)

  # Set compile and link properties for projects.
  IF(MSVC70 OR MSVC71)
    SET_TARGET_PROPERTIES( TreeViewer PROPERTIES LINK_FLAGS_DEBUG "/NODEFAULTLIB:msvcrt" )
  ELSEIF(MSVC80)
    # Remove some warnings about deprecated functions. Do not treat wchar_t as built in type
    SET_TARGET_PROPERTIES( TreeViewer PROPERTIES COMPILE_FLAGS "-D_CRT_SECURE_NO_DEPRECATE /Zc:wchar_t-" )
    # Needed to run in debug mode. Removes warning about conflict between libraries.
    SET_TARGET_PROPERTIES( TreeViewer PROPERTIES LINK_FLAGS_DEBUG "/NODEFAULTLIB:msvcrt" )
  ELSEIF(MSVC90)
    # Remove some warnings about deprecated functions. Do not treat wchar_t as built in type. Compile on multiple processors/threads if possible.
    SET_TARGET_PROPERTIES( TreeViewer PROPERTIES COMPILE_FLAGS "-D_CRT_SECURE_NO_DEPRECATE /Zc:wchar_t- /MP" )
    # Needed to run in debug mode. Removes warning about conflict between libraries.
    SET_TARGET_PROPERTIES( TreeViewer PROPERTIES LINK_FLAGS_DEBUG "/NODEFAULTLIB:msvcrt" )
  ENDIF(MSVC70 OR MSVC71)

  # set the install directory to the H3D directory on Windows
  IF( CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT )
    SET( CMAKE_INSTALL_PREFIX ${TreeViewer_SOURCE_DIR}/../../.. CACHE PATH "Install path prefix, prepended onto install directories." FORCE )
  ENDIF( CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT )
ENDIF(WIN32)

# Install in these folders.
INSTALL( TARGETS TreeViewer
         LIBRARY DESTINATION lib
         RUNTIME DESTINATION bin )
