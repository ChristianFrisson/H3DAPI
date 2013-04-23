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
SET(H3DViewer_BUILD_VERSION "1")
SET( H3DVIEWER_APP_NAME "H3DViewer ${H3DViewer_MAJOR_VERSION}.${H3DViewer_MINOR_VERSION}.${H3DViewer_BUILD_VERSION}" )

# Include directories.
INCLUDE_DIRECTORIES( ../src )

INCLUDE( "${H3DViewer_SOURCE_DIR}/H3DViewerSourceFiles.txt" )

# Add the H3DViewer.rc resource file if Visual Studio
IF(MSVC)
  SET( H3DViewer_SRCS ${H3DViewer_SRCS} ${CMAKE_CURRENT_BINARY_DIR}/H3DViewer.rc
                                        ${H3DViewer_SOURCE_DIR}/H3DViewer.ico )
ENDIF(MSVC)

# Add the directory to INCLUDE_DIRECTORIES before any other statement to make sure that this header file is found first.first.
# This assumes that we always use "#include <path/include_header.h>" in our code even if the file is actually found in
# the same directory as the file that contains the include statement.
INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_BINARY_DIR}/include )

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

IF( H3D_USE_DEPENDENCIES_ONLY )
  # The variables set here must be set by the CMakeLists.txt that sets H3D_USE_DEPENDENCIES_ONLY to true.
  INCLUDE_DIRECTORIES( ${EXTERNAL_INCLUDE_DIR} ) 
ENDIF( H3D_USE_DEPENDENCIES_ONLY )

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
  ADD_EXECUTABLE(H3DViewer WIN32 MACOSX_BUNDLE ${H3DViewer_SRCS} ${H3DViewer_SOURCE_DIR}/H3DViewer.icns )
ENDIF ( GENERATE_H3DVIEWER_CPACK_PROJECT )

# make sure that the H3DViewer.rc contains the correct svn-version
IF( MSVC )

  SET( H3DViewer_SVN_VERSION "0" )
  # Find SubWCRev.exe
  FIND_FILE( SubWCRev
             NAMES "SubWCRev.exe"
             DOC   "Set to SubWCRev.exe that comes with TortoiseSVN. Used to find svn revision number." )
  MARK_AS_ADVANCED( SubWCRev )

  SET( H3DViewer_is_working_copy 10 )
  IF( SubWCRev )
    EXECUTE_PROCESS( COMMAND ${SubWCRev} ${H3DViewer_SOURCE_DIR}/../
                     RESULT_VARIABLE H3DViewer_is_working_copy )
    IF( ${H3DViewer_is_working_copy} EQUAL 0 )
      SET( H3DViewer_SVN_VERSION "$WCREV$" )
    ENDIF( ${H3DViewer_is_working_copy} EQUAL 0 )
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

  IF( SubWCRev AND ${H3DViewer_is_working_copy} EQUAL 0 )
    # Update SVN revision in file.
    EXECUTE_PROCESS( COMMAND ${SubWCRev} ${H3DViewer_SOURCE_DIR}/../ ${CMAKE_CURRENT_BINARY_DIR}/H3DViewer.rc ${CMAKE_CURRENT_BINARY_DIR}/H3DViewer.rc )
    
    ADD_CUSTOM_COMMAND( TARGET H3DViewer 
                        PRE_BUILD 
                        COMMAND ${SubWCRev} 
                        ARGS ${H3DViewer_SOURCE_DIR}/../ ${CMAKE_CURRENT_BINARY_DIR}/H3DViewer.rc ${CMAKE_CURRENT_BINARY_DIR}/H3DViewer.rc )
  ENDIF( SubWCRev AND ${H3DViewer_is_working_copy} EQUAL 0 )
ENDIF(MSVC)


IF( EXISTS ${H3DViewer_SOURCE_DIR}/../src/H3DViewerConfig.h )
  FILE( REMOVE ${H3DViewer_SOURCE_DIR}/../src/H3DViewerConfig.h )
ENDIF( EXISTS ${H3DViewer_SOURCE_DIR}/../src/H3DViewerConfig.h )
CONFIGURE_FILE( ${H3DViewer_SOURCE_DIR}/../src/H3DViewerConfig.cmake ${CMAKE_CURRENT_BINARY_DIR}/include/H3DViewerConfig.h )

IF( NOT GENERATE_CPACK_PROJECT )
  # Install header files on non-windows system (e.g. Unix).
  INSTALL( FILES ${CMAKE_CURRENT_BINARY_DIR}/include/H3DViewerConfig.h
           DESTINATION ${H3DViewer_SOURCE_DIR}/../src )
ENDIF( NOT GENERATE_CPACK_PROJECT )

# Link against requiredLibs.
TARGET_LINK_LIBRARIES( H3DViewer ${requiredLibs} )

# Set postfix for debug version of H3DViewer.
SET_TARGET_PROPERTIES( H3DViewer PROPERTIES DEBUG_POSTFIX "_d" )

SET_TARGET_PROPERTIES( H3DViewer
    PROPERTIES
    MACOSX_BUNDLE_BUNDLE_NAME H3DViewer
    MACOSX_BUNDLE_ICON_FILE H3DViewer.icns
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

SET( DEFAULT_BIN_INSTALL "bin" )
SET( DEFAULT_LIB_INSTALL "lib" )

IF( WIN32 )
  SET( DEFAULT_BIN_INSTALL "bin32" )
  SET( DEFAULT_LIB_INSTALL "lib32" )
  IF( CMAKE_SIZEOF_VOID_P EQUAL 8 )
    SET( DEFAULT_BIN_INSTALL "bin64" )
    SET( DEFAULT_LIB_INSTALL "lib64" )
  ENDIF( CMAKE_SIZEOF_VOID_P EQUAL 8 )
ENDIF( WIN32 )

# Install in these folders.
IF( APPLE )
  IF( NOT DEFINED OSX_BUNDLE_INSTALL_DESTINATION )
    SET( OSX_BUNDLE_INSTALL_DESTINATION /Applications CACHE PATH "Directory into which to install the H3DViewer.app bundle." )
  ENDIF( NOT DEFINED OSX_BUNDLE_INSTALL_DESTINATION )


  SET_SOURCE_FILES_PROPERTIES(
	${H3DViewer_SOURCE_DIR}/H3DViewer.icns
   	PROPERTIES
	MACOSX_PACKAGE_LOCATION Resources
   )

  INSTALL( TARGETS H3DViewer
           LIBRARY DESTINATION ${DEFAULT_LIB_INSTALL}
           RUNTIME DESTINATION ${DEFAULT_BIN_INSTALL}
           BUNDLE DESTINATION ${OSX_BUNDLE_INSTALL_DESTINATION}
           COMPONENT H3DAPI_cpack_runtime )
ELSE(APPLE)
  SET( H3DViewer_package_install_dir "" )
  IF( WIN32 AND GENERATE_H3DVIEWER_CPACK_PROJECT )
    SET( H3DViewer_package_install_dir "H3DViewer/" )
  ENDIF( WIN32 AND GENERATE_H3DVIEWER_CPACK_PROJECT )

	SET( H3DVIEWER_INSTALL_COMPONENT_NAME "H3DViewer_cpack_runtime" )
	IF( GENERATE_CPACK_PROJECT )
	  SET( H3DVIEWER_INSTALL_COMPONENT_NAME "H3DAPI_cpack_runtime" )
  ENDIF( GENERATE_CPACK_PROJECT )

  INSTALL( TARGETS H3DViewer
					 LIBRARY DESTINATION ${DEFAULT_LIB_INSTALL}
					 RUNTIME DESTINATION ${H3DViewer_package_install_dir}${DEFAULT_BIN_INSTALL} COMPONENT ${H3DVIEWER_INSTALL_COMPONENT_NAME} )
ENDIF( APPLE )

# Add a cache variable GENERATE_H3DVIEWER_CPACK_PROJECT to have the choice of generating a project
# for packaging H3DViewer. Default is NO since most people will not use this.
IF( NOT DEFINED GENERATE_H3DVIEWER_CPACK_PROJECT )
  SET( GENERATE_H3DVIEWER_CPACK_PROJECT "NO" CACHE BOOL "Decides if a cpack project should be generated. The project in the first loaded CMakeLists will configure CPack." )
  MARK_AS_ADVANCED(GENERATE_H3DVIEWER_CPACK_PROJECT)
ENDIF( NOT DEFINED GENERATE_H3DVIEWER_CPACK_PROJECT )

IF( GENERATE_H3DVIEWER_CPACK_PROJECT )
  include( ${H3DViewer_SOURCE_DIR}/H3DViewerCPack.cmake )
ENDIF( GENERATE_H3DVIEWER_CPACK_PROJECT )
