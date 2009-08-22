IF( COMMAND cmake_policy)
  cmake_policy(SET CMP0003 OLD) 
ENDIF( COMMAND cmake_policy)

IF(WIN32)
  cmake_minimum_required(VERSION 2.6.0)
ENDIF(WIN32)
# The name of our project is "H3DViewer".  CMakeLists files in this project can
# refer to the root source directory of the project as ${H3DViewer_SOURCE_DIR} and
# to the root binary directory of the project as ${H3DViewer_BINARY_DIR}.
project (H3DViewer)

# Source files.
SET( H3DViewer_SRCS "${H3DViewer_SOURCE_DIR}/../src/ConsoleDialog.cpp"
                    "${H3DViewer_SOURCE_DIR}/../src/ConsoleDialog.h"
                    "${H3DViewer_SOURCE_DIR}/../src/Envini.h"
                    "${H3DViewer_SOURCE_DIR}/../src/H3DViewerApp.cpp"
                    "${H3DViewer_SOURCE_DIR}/../src/WxFrame.cpp"
                    "${H3DViewer_SOURCE_DIR}/../src/WxFrame.h"
                    "${H3DViewer_SOURCE_DIR}/../src/WxWidgetsWindow.cpp"
                    "${H3DViewer_SOURCE_DIR}/../src/WxWidgetsWindow.h"
                    "${H3DViewer_SOURCE_DIR}/../src/H3DViewerConfig.h"
                    "${H3DViewer_SOURCE_DIR}/../src/H3DViewerFieldValuesDialog.cpp"
                    "${H3DViewer_SOURCE_DIR}/../src/H3DViewerFieldValuesDialog.h"
                    "${H3DViewer_SOURCE_DIR}/../src/H3DViewerTreeViewDialog.cpp"        
                    "${H3DViewer_SOURCE_DIR}/../src/H3DViewerTreeViewDialog.h"
                    "${H3DViewer_SOURCE_DIR}/../src/H3DViewerPluginsDialog.cpp"        
                    "${H3DViewer_SOURCE_DIR}/../src/H3DViewerPluginsDialog.h"        
                    "${H3DViewer_SOURCE_DIR}/../src/H3DViewer.cpp"
                    "${H3DViewer_SOURCE_DIR}/../src/H3DViewer.h" )

# Include directories.
INCLUDE_DIRECTORIES( ../src )

# Add required libs to this variable.
SET(requiredLibs)

# Where to find cmake modules used to find packages.
SET(CMAKE_MODULE_PATH "${H3DViewer_SOURCE_DIR}/../../build/modules/")
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
  SET(wxWidgets_USE_LIBS base core gl adv richtext html)
  FIND_PACKAGE(wxWidgets REQUIRED)
  IF(wxWidgets_FOUND)
    INCLUDE(${wxWidgets_USE_FILE})
    SET(requiredLibs ${requiredLibs} ${wxWidgets_LIBRARIES} )
  ENDIF(wxWidgets_FOUND)
ENDIF(wxWidgets_FOUND)

# Create build files.
IF( GENERATE_H3DVIEWER_CPACK_PROJECT )
  # if generating CPACK project using BUNDLE we should not create the bundle here instead
  # it is built with CPack 
  ADD_EXECUTABLE(H3DViewer WIN32 ${H3DViewer_SRCS})
ELSE( GENERATE_H3DVIEWER_CPACK_PROJECT )
 ADD_EXECUTABLE(H3DViewer WIN32 MACOSX_BUNDLE ${H3DViewer_SRCS})
ENDIF ( GENERATE_H3DVIEWER_CPACK_PROJECT )

# Link against requiredLibs and optionalLibs.
TARGET_LINK_LIBRARIES( H3DViewer ${requiredLibs} ${optionalLibs} )

# Set postfix for debug version of H3DViewer.
SET_TARGET_PROPERTIES( H3DViewer PROPERTIES DEBUG_POSTFIX "_d" )

SET_TARGET_PROPERTIES( H3DViewer
    PROPERTIES
    MACOSX_BUNDLE_BUNDLE_NAME H3DViewer
    MACOSX_BUNDLE_ICON_FILE ${H3DViewer_SOURCE_DIR}/H3DViewer.icns
)

IF(WIN32)

  # Set compile and link properties for projects.
  IF(MSVC70 OR MSVC71)
    SET_TARGET_PROPERTIES( H3DViewer PROPERTIES LINK_FLAGS_DEBUG "/NODEFAULTLIB:msvcrt" )
  ELSEIF(MSVC80)
    # Remove some warnings about deprecated functions. Do not treat wchar_t as built in type
    SET_TARGET_PROPERTIES( H3DViewer PROPERTIES COMPILE_FLAGS "-D_CRT_SECURE_NO_DEPRECATE /Zc:wchar_t-" )
    # Needed to run in debug mode. Removes warning about conflict between libraries.
    SET_TARGET_PROPERTIES( H3DViewer PROPERTIES LINK_FLAGS_DEBUG "/NODEFAULTLIB:msvcrt" )
  ELSEIF(MSVC90)
    # Remove some warnings about deprecated functions. Do not treat wchar_t as built in type. Compile on multiple processors/threads if possible.
    SET_TARGET_PROPERTIES( H3DViewer PROPERTIES COMPILE_FLAGS "-D_CRT_SECURE_NO_DEPRECATE /Zc:wchar_t- /MP" )
    # Needed to run in debug mode. Removes warning about conflict between libraries.
    SET_TARGET_PROPERTIES( H3DViewer PROPERTIES LINK_FLAGS_DEBUG "/NODEFAULTLIB:msvcrt" )
  ENDIF(MSVC70 OR MSVC71)

  # set the install directory to the H3D directory on Windows
  IF( CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT )
    SET( CMAKE_INSTALL_PREFIX ${H3DViewer_SOURCE_DIR}/../../.. CACHE PATH "Install path prefix, prepended onto install directories." FORCE )
  ENDIF( CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT )
ENDIF(WIN32)

# Install in these folders.
INSTALL( TARGETS H3DViewer
         LIBRARY DESTINATION lib
         RUNTIME DESTINATION bin )

IF( APPLE )
  INSTALL( BUNDLE DESTINATION "/Applications" )
ENDIF( APPLE )

# Add a cache variable GENERATE_H3DVIEWER_CPACK_PROJECT to have the choice of generating a project
# for packaging H3DAPI. Default is NO since most people will not use this.
IF( NOT DEFINED GENERATE_H3DVIEWER_CPACK_PROJECT )
  SET( GENERATE_H3DVIEWER_CPACK_PROJECT "NO" CACHE BOOL "Decides if a cpack project should be generated. The project in the first loaded CMakeLists will configure CPack." )
  MARK_AS_ADVANCED(GENERATE_H3DVIEWER_CPACK_PROJECT)
ENDIF( NOT DEFINED GENERATE_H3DVIEWER_CPACK_PROJECT )

IF( GENERATE_H3DVIEWER_CPACK_PROJECT )

  # Set information properties about the project to install.
  SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "H3DViewer. An X3D viewer with extensions for haptics." )
  SET(CPACK_PACKAGE_VENDOR "SenseGraphics AB")
  SET(CPACK_PACKAGE_CONTACT "support@sensegraphics.com" )
  SET(CPACK_RESOURCE_FILE_LICENSE "${H3DViewer_SOURCE_DIR}/../../LICENSE")
  SET(CPACK_INSTALL_CMAKE_PROJECTS "${CMAKE_CURRENT_BINARY_DIR};H3DViewer;ALL;/")
  SET(CPACK_PACKAGE_VERSION_MAJOR "2")
  SET(CPACK_PACKAGE_VERSION_MINOR "1")
  SET(CPACK_PACKAGE_VERSION_PATCH "0")


  IF( APPLE )
    IF( NOT DEFINED H3DVIEWER_CPACK_INCLUDE_LIBRARIES )
      SET( H3DVIEWER_CPACK_INCLUDE_LIBRARIES "NO" CACHE BOOL "Decides if all dependent shared libraries should be included in the bundle or not." )
      MARK_AS_ADVANCED(H3DVIEWER_CPACK_INCLUDE_LIBRARIES)
    ENDIF( NOT DEFINED H3DVIEWER_CPACK_INCLUDE_LIBRARIES )

    SET( CPACK_BUNDLE_NAME "H3DViewer" ) #- provides the bundle name (displayed in the finder underneath the bundle icon). 
    SET( CPACK_BUNDLE_ICON "${H3DViewer_SOURCE_DIR}/H3DViewer.icns" ) # - provides the bundle icon (displayed in the /Applications folder, on the dock, etc). 
    SET( CPACK_BUNDLE_PLIST "${H3DViewer_SOURCE_DIR}/info.plist" ) # - path to a file that will become the bundle plist. 
    SET( CPACK_BUNDLE_STARTUP_COMMAND "${H3DViewer_SOURCE_DIR}/start.sh" ) #- path to a file that will be executed when the user opens the bundle. Could be a shell-script or a binary.

    IF( H3DVIEWER_CPACK_INCLUDE_LIBRARIES )
      #Include all shared libraries in bundle
      INCLUDE("${H3DViewer_SOURCE_DIR}/OSXCPackLibraries.txt" )

      INSTALL( FILES ${OSX_DEPENDENT_LIBRARIES}
               DESTINATION lib )

      INSTALL( FILES ${OSX_PLUGIN_LIBRARIES}
               DESTINATION Plugins )
    ENDIF( H3DVIEWER_CPACK_INCLUDE_LIBRARIES )

  ENDIF( APPLE )

  INCLUDE(CPack)

ENDIF( GENERATE_H3DVIEWER_CPACK_PROJECT )
