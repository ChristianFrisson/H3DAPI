IF( COMMAND cmake_policy )
  IF( POLICY CMP0003 )
    cmake_policy(SET CMP0003 OLD)
  ENDIF( POLICY CMP0003 )
ENDIF( COMMAND cmake_policy )
IF(WIN32)
  cmake_minimum_required(VERSION 2.6.0)
ENDIF(WIN32)
# The name of our project is "H3DViewer".  CMakeLists files in this project can
# refer to the root source directory of the project as ${H3DViewer_SOURCE_DIR} and
# to the root binary directory of the project as ${H3DViewer_BINARY_DIR}.
project (H3DViewer)

# set the install directory to the H3D directory on Windows
IF( WIN32 AND CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT AND NOT H3D_CMAKE_INSTALL_PREFIX_ALREADY_SET )
  SET( CMAKE_INSTALL_PREFIX ${H3DViewer_SOURCE_DIR}/../../.. CACHE PATH "Install path prefix, prepended onto install directories." FORCE )
  SET( H3D_CMAKE_INSTALL_PREFIX_ALREADY_SET TRUE )
ENDIF( WIN32 AND CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT AND NOT H3D_CMAKE_INSTALL_PREFIX_ALREADY_SET )

# use, i.e. don't skip the full RPATH for the build tree
SET(CMAKE_SKIP_BUILD_RPATH FALSE)

# when building, don't use the install RPATH already
# (but later on when installing)
SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE) 

# add the automatically determined parts of the RPATH
# which point to directories outside the build tree to the install RPATH
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

# the RPATH to be used when installing, but only if it's not a system directory
LIST(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${CMAKE_INSTALL_PREFIX}/lib" isSystemDir)
IF("${isSystemDir}" STREQUAL "-1")
   SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
ENDIF("${isSystemDir}" STREQUAL "-1")

SET(H3DViewer_MAJOR_VERSION "2")
SET(H3DViewer_MINOR_VERSION "2")
SET(H3DViewer_BUILD_VERSION "0")
SET( H3DVIEWER_APP_NAME "H3DViewer ${H3DViewer_MAJOR_VERSION}.${H3DViewer_MINOR_VERSION}.${H3DViewer_BUILD_VERSION}" )

# Source files.
SET( H3DViewer_SRCS "${H3DViewer_SOURCE_DIR}/../src/Envini.h"
                    "${H3DViewer_SOURCE_DIR}/../src/H3DViewerApp.cpp"
                    "${H3DViewer_SOURCE_DIR}/../src/WxConsoleDialog.cpp"
                    "${H3DViewer_SOURCE_DIR}/../src/WxConsoleDialog.h"
                    "${H3DViewer_SOURCE_DIR}/../src/WxFrame.cpp"
                    "${H3DViewer_SOURCE_DIR}/../src/WxFrame.h"
                    "${H3DViewer_SOURCE_DIR}/../src/WxWidgetsWindow.cpp"
                    "${H3DViewer_SOURCE_DIR}/../src/WxWidgetsWindow.h"
                    "${H3DViewer_SOURCE_DIR}/../src/H3DViewerConfig.h"
                    "${H3DViewer_SOURCE_DIR}/../src/H3DViewerFieldValuesDialog.cpp"
                    "${H3DViewer_SOURCE_DIR}/../src/H3DViewerFieldValuesDialog.h"
                    "${H3DViewer_SOURCE_DIR}/../src/H3DViewerFieldValuesDialogPropGrid.cpp"  
                    "${H3DViewer_SOURCE_DIR}/../src/H3DViewerFieldValuesDialogPropGrid.h"
                    "${H3DViewer_SOURCE_DIR}/../src/H3DViewerTreeViewDialog.cpp"
                    "${H3DViewer_SOURCE_DIR}/../src/H3DViewerTreeViewDialog.h"
                    "${H3DViewer_SOURCE_DIR}/../src/H3DViewerPluginsDialog.cpp"
                    "${H3DViewer_SOURCE_DIR}/../src/H3DViewerPluginsDialog.h"
                    "${H3DViewer_SOURCE_DIR}/../src/H3DViewerPopupMenus.cpp"
                    "${H3DViewer_SOURCE_DIR}/../src/H3DViewerPopupMenus.h"
                    "${H3DViewer_SOURCE_DIR}/../src/H3DViewer.cpp"
                    "${H3DViewer_SOURCE_DIR}/../src/H3DViewer.h" )

# Include directories.
INCLUDE_DIRECTORIES( ../src )

# Add the H3DViewer.rc resource file if Visual Studio
IF(MSVC)
  SET( H3DViewer_SRCS ${H3DViewer_SRCS} ${CMAKE_CURRENT_BINARY_DIR}/H3DViewer.rc
                                        ${H3DViewer_SOURCE_DIR}/H3DViewer.ico )
ENDIF(MSVC)

# Add required libs to this variable.
SET(requiredLibs)

# Where to find cmake modules used to find packages.
SET(CMAKE_MODULE_PATH "${H3DViewer_SOURCE_DIR}/modules" "${H3DViewer_SOURCE_DIR}/../../build/modules/")
IF( COMMAND cmake_policy )
  IF( POLICY CMP0011 )
    cmake_policy( SET CMP0011 NEW )
  ENDIF( POLICY CMP0011 )
ENDIF( COMMAND cmake_policy )
INCLUDE( StripAndAddLibraryDirectories )

IF( TARGET H3DUtil )
  INCLUDE_DIRECTORIES( ${H3DUTIL_INCLUDE_DIR} ) 
  SET( requiredLibs ${requiredLibs} H3DUtil )
ELSE( TARGET H3DUtil )
  #H3DUtil
  FIND_PACKAGE(H3DUtil REQUIRED)

  IF(H3DUTIL_FOUND)
    INCLUDE_DIRECTORIES( ${H3DUTIL_INCLUDE_DIR} ) 
    SET(requiredLibs ${requiredLibs} ${H3DUTIL_LIBRARIES} )
  ENDIF(H3DUTIL_FOUND)
ENDIF( TARGET H3DUtil )

IF( TARGET HAPI )
  INCLUDE_DIRECTORIES( ${HAPI_INCLUDE_DIR} ) 
  SET( requiredLibs ${requiredLibs} HAPI )
ELSE( TARGET HAPI )
  #HAPI
  FIND_PACKAGE(HAPI REQUIRED)

  IF(HAPI_FOUND)
    INCLUDE_DIRECTORIES( ${HAPI_INCLUDE_DIR} ) 
    SET(requiredLibs ${requiredLibs} ${HAPI_LIBRARIES} )
  ENDIF(HAPI_FOUND)
ENDIF( TARGET HAPI )

IF( TARGET H3DAPI )
  INCLUDE_DIRECTORIES( ${H3DAPI_INCLUDE_DIR} ) 
  SET( requiredLibs ${requiredLibs} H3DAPI )
ELSE( TARGET H3DAPI )
  #H3DAPI
  FIND_PACKAGE(H3DAPI REQUIRED)

  IF(H3DAPI_FOUND)
    INCLUDE_DIRECTORIES( ${H3DAPI_INCLUDE_DIR} ) 
    SET(requiredLibs ${requiredLibs} ${H3DAPI_LIBRARIES} )
  ENDIF(H3DAPI_FOUND)
ENDIF( TARGET H3DAPI )

IF( H3D_USE_DEPENDENCIES_ONLY )
  # The variables set here must be set by the CMakeLists.txt that sets H3D_USE_DEPENDENCIES_ONLY to true.
  IF(PTHREAD_FOUND)
    INCLUDE_DIRECTORIES( ${PTHREAD_INCLUDE_DIR} ) 
  ENDIF(PTHREAD_FOUND)
  INCLUDE_DIRECTORIES( ${EXTERNAL_INCLUDE_DIR} ) 
ENDIF( H3D_USE_DEPENDENCIES_ONLY )

# WxWidgets.
SET(WXWINDOWS_USE_GL 1)
# find wx library propgrid
# The propgrid code does not work properly on Ubuntu 11.10 with the installed CMake and wx (2.8).
# It returns true even though propgrid does not exist, it probably ignores USE_LIBS that does not exist.
# Since there is no wxWidgets_VERSION variable or something like that there is code to check the end of
# include path. I have no idea if this would work on all other UNIX systems as well. NOTE, report
# this to CMAKE unless it is reported already.
SET(wxWidgets_USE_LIBS propgrid)  
SET( wxWidgets_FIND_QUIETLY 1 )
SET(wxWidgets_FOUND 0)
IF(WIN32)
  FIND_PACKAGE(wxWidgetsWin)
  IF(wxWidgets_FOUND)
    INCLUDE_DIRECTORIES( ${wxWidgets_INCLUDE_DIR} ) 
    SET(requiredLibs ${requiredLibs} ${wxWidgets_LIBRARIES} )
  ENDIF(wxWidgets_FOUND)
ENDIF(WIN32)

IF(wxWidgets_FOUND)
ELSE(wxWidgets_FOUND)
  FIND_PACKAGE(wxWidgets)
ENDIF(wxWidgets_FOUND)

IF(wxWidgets_FOUND)
  IF( ${CMAKE_SYSTEM_NAME} STREQUAL "Linux" )
    SET( wxWIDGETS_VERSION29_TMP FALSE )
    FOREACH( TMP_INCLUDE_DIR ${wxWidgets_INCLUDE_DIRS} )
      IF( ${TMP_INCLUDE_DIR} MATCHES "wx\\-2\\.9$" )
        SET( wxWIDGETS_VERSION29_TMP TRUE )
      ENDIF( ${TMP_INCLUDE_DIR} MATCHES "wx\\-2\\.9$" )
    ENDFOREACH( TMP_INCLUDE_DIR ${wxWidgets_INCLUDE_DIRS} )
    IF( NOT wxWIDGETS_VERSION29_TMP )
      SET(wxWidgets_FOUND 0)
    ENDIF( NOT wxWIDGETS_VERSION29_TMP )
  ENDIF( ${CMAKE_SYSTEM_NAME} STREQUAL "Linux" )
ENDIF(wxWidgets_FOUND )

IF(wxWidgets_FOUND)
  SET( HAVE_WXPROPGRID 1 )
  SET(wxWidgets_USE_LIBS base core gl adv richtext html propgrid)  
ELSE(wxWidgets_FOUND )
  MESSAGE( STATUS "wxWidgets propgrid not found. Needed for better tree view and program setting choice in H3DViewer. Part of wxWidgets 2.9." )
  SET(wxWidgets_USE_LIBS base core gl adv richtext html)
ENDIF(wxWidgets_FOUND )

# reset the wxWidgets_FOUND variable before running FIND_PACKAGE again.
SET( wxWidgets_FOUND 0 )

# find wx libraries except propgrid
IF(WIN32)
  FIND_PACKAGE(wxWidgetsWin)
  IF(wxWidgets_FOUND)
    INCLUDE_DIRECTORIES( ${wxWidgets_INCLUDE_DIR} ) 
    SET(requiredLibs ${requiredLibs} ${wxWidgets_LIBRARIES} )
  ENDIF(wxWidgets_FOUND)
ENDIF(WIN32)

IF(wxWidgets_FOUND)
ELSE(wxWidgets_FOUND)
  FIND_PACKAGE(wxWidgets REQUIRED)
  IF(wxWidgets_FOUND)
    INCLUDE(${wxWidgets_USE_FILE})
    SET(requiredLibs ${requiredLibs} ${wxWidgets_LIBRARIES} )
  ENDIF(wxWidgets_FOUND)
ENDIF(wxWidgets_FOUND)

FIND_PACKAGE( DirectX )
IF( DirectX_FOUND )
  SET( HAVE_DX9 1 )
  INCLUDE_DIRECTORIES( ${DirectX_INCLUDE_DIR} )
  SET( requiredLibs ${requiredLibs} ${DirectX_LIBRARIES} )
ENDIF( DirectX_FOUND )

# Create build files.
IF( GENERATE_H3DVIEWER_CPACK_PROJECT )
  # if generating CPACK project using BUNDLE we should not create the bundle here instead
  # it is built with CPack 
  ADD_EXECUTABLE(H3DViewer WIN32 ${H3DViewer_SRCS})
ELSE( GENERATE_H3DVIEWER_CPACK_PROJECT )
  ADD_EXECUTABLE(H3DViewer WIN32 MACOSX_BUNDLE ${H3DViewer_SRCS})
ENDIF ( GENERATE_H3DVIEWER_CPACK_PROJECT )

# make sure that the H3DViewer.rc contains the correct svn-version
IF( MSVC )

  SET( H3DViewer_SVN_VERSION "0" )
  # Find SubWCRev.exe
  FIND_FILE( SubWCRev
             NAMES "SubWCRev.exe"
             DOC   "Set to SubWCRev.exe that comes with TortoiseSVN. Used to find svn revision number." )

  IF( SubWCRev )
    SET( H3DViewer_SVN_VERSION "$WCREV$" )
  ENDIF( SubWCRev )

  # autogenerate H3DViewer.rc depending on the version
  CONFIGURE_FILE( H3DViewer.rc.cmake ${CMAKE_CURRENT_BINARY_DIR}/H3DViewer.rc )

  ADD_CUSTOM_COMMAND( TARGET H3DViewer
                      PRE_BUILD
                      COMMAND ${H3DViewer_SOURCE_DIR}/../../build/UpdateResourceFile 
                      ARGS H3DViewer ${CMAKE_CURRENT_BINARY_DIR}/H3DViewer.rc ${H3DViewer_SOURCE_DIR}/H3DViewer.rc.cmake
                      ${H3DViewer_MAJOR_VERSION} ${H3DViewer_MINOR_VERSION}
                      ${H3DViewer_BUILD_VERSION} "${H3DViewer_SVN_VERSION}"
                      "H3DViewer_SOURCE_DIR"
                      ${H3DViewer_SOURCE_DIR}
                      "H3DViewer_Output_Name"
                      "$(TargetFileName)" )

  IF( SubWCRev )
    # Update SVN revision in file.
    EXECUTE_PROCESS( COMMAND ${SubWCRev} ${H3DViewer_SOURCE_DIR}/../ ${CMAKE_CURRENT_BINARY_DIR}/H3DViewer.rc ${CMAKE_CURRENT_BINARY_DIR}/H3DViewer.rc )
    
    ADD_CUSTOM_COMMAND( TARGET H3DViewer 
                        PRE_BUILD 
                        COMMAND ${SubWCRev} 
                        ARGS ${H3DViewer_SOURCE_DIR}/../ ${CMAKE_CURRENT_BINARY_DIR}/H3DViewer.rc ${CMAKE_CURRENT_BINARY_DIR}/H3DViewer.rc )
  ENDIF( SubWCRev )  
ENDIF(MSVC)

CONFIGURE_FILE( ${H3DViewer_SOURCE_DIR}/../src/H3DViewerConfig.cmake ${H3DViewer_SOURCE_DIR}/../src/H3DViewerConfig.h )

# Link against requiredLibs and optionalLibs.
TARGET_LINK_LIBRARIES( H3DViewer ${requiredLibs} ${optionalLibs} )

# Set postfix for debug version of H3DViewer.
SET_TARGET_PROPERTIES( H3DViewer PROPERTIES DEBUG_POSTFIX "_d" )

SET_TARGET_PROPERTIES( H3DViewer
    PROPERTIES
    MACOSX_BUNDLE_BUNDLE_NAME H3DViewer
    MACOSX_BUNDLE_ICON_FILE ${H3DViewer_SOURCE_DIR}/H3DViewer.icns
)

IF(MSVC)

  SET( H3DViewer_COMPILE_FLAGS "-DWXUSINGDLL" )
  IF( GENERATE_H3DVIEWER_CPACK_PROJECT )
    SET( H3DViewer_COMPILE_FLAGS "${H3DViewer_COMPILE_FLAGS} -DH3DVIEWER_STANDALONE" )
  ENDIF( GENERATE_H3DVIEWER_CPACK_PROJECT )

  # Treat wchar_t as built in type for all visual studio versions.
  # This is default for every version above 7 ( so far ) but we still set it for all.
  SET( H3DViewer_COMPILE_FLAGS "${H3DViewer_COMPILE_FLAGS} /Zc:wchar_t")

  IF( MSVC80 )
    # This might be useful for visual studio 2005 users that often recompile the api.
    IF( NOT DEFINED USE_VC8_MP_FLAG )
      SET( USE_VC8_MP_FLAG "NO" CACHE BOOL "In visual studio 8 the MP flag exists but is not documented. Maybe it is unsafe to use. If you want to use it then set this flag to yes." )
    ENDIF( NOT DEFINED USE_VC8_MP_FLAG )

    IF( USE_VC8_MP_FLAG )
      SET( H3DViewer_COMPILE_FLAGS "${H3DViewer_COMPILE_FLAGS} /MP" )
    ENDIF( USE_VC8_MP_FLAG )
  ENDIF( MSVC80 )

  IF( ${MSVC_VERSION} GREATER 1399 )
    # Remove compiler warnings about deprecation for visual studio versions 8 and above.
    SET( H3DViewer_COMPILE_FLAGS "${H3DViewer_COMPILE_FLAGS} -D_CRT_SECURE_NO_DEPRECATE" )
  ENDIF( ${MSVC_VERSION} GREATER 1399 )

  IF( ${MSVC_VERSION} GREATER 1499 )
    # Build using several threads for visual studio versions 9 and above.
    SET( H3DViewer_COMPILE_FLAGS "${H3DViewer_COMPILE_FLAGS} /MP" )
  ENDIF( ${MSVC_VERSION} GREATER 1499 )

  IF( ${MSVC_VERSION} LESS 1600 )
    # Turn off optimization for visual studio versions 8 and below.
    # Old setting which I (Markus) have no idea why it was added.
    SET( H3DViewer_COMPILE_FLAGS "${H3DViewer_COMPILE_FLAGS} /Od" )
  ENDIF( ${MSVC_VERSION} LESS 1600 )

  # H3D API is now to big to link incrementally with vc. And remove link warning in debug build.
  SET_TARGET_PROPERTIES( H3DViewer PROPERTIES LINK_FLAGS_DEBUG "/INCREMENTAL:NO /NODEFAULTLIB:msvcrt" )
  SET_TARGET_PROPERTIES( H3DViewer PROPERTIES LINK_FLAGS "/INCREMENTAL:NO" )

  SET_TARGET_PROPERTIES( H3DViewer PROPERTIES COMPILE_FLAGS "${H3DViewer_COMPILE_FLAGS}" )
ENDIF(MSVC)

# Install in these folders.
IF( APPLE )
 INSTALL( TARGETS H3DViewer
          LIBRARY DESTINATION lib
          RUNTIME DESTINATION bin
          BUNDLE DESTINATION "/Applications"
          COMPONENT H3DAPI_cpack_runtime )
ELSE( APPLE )
  INSTALL( TARGETS H3DViewer
           LIBRARY DESTINATION lib
           RUNTIME DESTINATION bin COMPONENT H3DViewer_cpack_runtime )
ENDIF( APPLE )

# Add a cache variable GENERATE_H3DVIEWER_CPACK_PROJECT to have the choice of generating a project
# for packaging H3DViewer. Default is NO since most people will not use this.
IF( NOT DEFINED GENERATE_H3DVIEWER_CPACK_PROJECT )
  SET( GENERATE_H3DVIEWER_CPACK_PROJECT "NO" CACHE BOOL "Decides if a cpack project should be generated. The project in the first loaded CMakeLists will configure CPack." )
  MARK_AS_ADVANCED(GENERATE_H3DVIEWER_CPACK_PROJECT)
ENDIF( NOT DEFINED GENERATE_H3DVIEWER_CPACK_PROJECT )

IF( NOT H3DAPI_SOURCE_DIR )
  include( ${H3DViewer_SOURCE_DIR}/H3DViewerCPack.cmake )
ENDIF( NOT H3DAPI_SOURCE_DIR )
