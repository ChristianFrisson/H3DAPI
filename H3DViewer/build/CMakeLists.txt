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

SET(H3DViewer_MAJOR_VERSION "2")
SET(H3DViewer_MINOR_VERSION "1")
SET(H3DViewer_BUILD_VERSION "1")
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
                    "${H3DViewer_SOURCE_DIR}/../src/H3DViewerTreeViewDialog.cpp"
                    "${H3DViewer_SOURCE_DIR}/../src/H3DViewerTreeViewDialog.h"
                    "${H3DViewer_SOURCE_DIR}/../src/H3DViewerPluginsDialog.cpp"
                    "${H3DViewer_SOURCE_DIR}/../src/H3DViewerPluginsDialog.h"
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

# make sure that the H3DViewer.rc contains the correct svn-version
IF( MSVC )
  EXECUTE_PROCESS( COMMAND ${H3DViewer_SOURCE_DIR}/../../build/FindSVNVersion ${H3DViewer_SOURCE_DIR}/../src/.svn/entries 
                   OUTPUT_VARIABLE H3DViewer_SVN_VERSION )
  # autogenerate H3DViewer.rc depending on the version
  CONFIGURE_FILE( H3DViewer.rc.cmake ${CMAKE_CURRENT_BINARY_DIR}/H3DViewer.rc )

  # Add option to disable UpdateResourceFile call, ON by default if this is an svn checkout.
  # OFF if this is not an svn checkout.
  IF( H3DViewer_SVN_VERSION EQUAL 0 )
    OPTION( CallUpdateResourceFile
            "If ON then a pre build command is added to call UpdateResourceFile. This is ON if H3DViewer is a subversion checkout, OFF if it is not."
            OFF )
  ELSE( H3DViewer_SVN_VERSION EQUAL 0 )
    OPTION( CallUpdateResourceFile
            "If ON then a pre build command is added to call UpdateResourceFile. This is ON if H3DViewer is a subversion checkout, OFF if it is not."
            ON )
  ENDIF( H3DViewer_SVN_VERSION EQUAL 0 )

  IF( CallUpdateResourceFile )
    ADD_CUSTOM_COMMAND( TARGET H3DViewer 
                        PRE_BUILD 
                        COMMAND ${H3DViewer_SOURCE_DIR}/../../build/UpdateResourceFile 
                        ARGS H3DViewer ${CMAKE_CURRENT_BINARY_DIR}/H3DViewer.rc ${H3DViewer_SOURCE_DIR}/H3DViewer.rc.cmake
                        ${H3DViewer_MAJOR_VERSION} ${H3DViewer_MINOR_VERSION}
                        ${H3DViewer_BUILD_VERSION}
                        ${H3DViewer_SOURCE_DIR}/../src/.svn/entries
                        "H3DViewer_SOURCE_DIR"
                        ${H3DViewer_SOURCE_DIR} )
  ENDIF( CallUpdateResourceFile )
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

IF(WIN32)

  SET( H3DViewer_COMPILE_FLAGS "-DWXUSINGDLL" )
  IF( GENERATE_H3DVIEWER_CPACK_PROJECT )
    SET( H3DViewer_COMPILE_FLAGS "-DH3DVIEWER_STANDALONE" )
  ENDIF( GENERATE_H3DVIEWER_CPACK_PROJECT )

  # Set compile and link properties for projects.
  IF(MSVC70 OR MSVC71)
    SET_TARGET_PROPERTIES( H3DViewer PROPERTIES LINK_FLAGS_DEBUG "/NODEFAULTLIB:msvcrt /Zc:wchar_t" )
  ELSEIF(MSVC80)
    # Remove some warnings about deprecated functions. Compile on multiple processors/threads if possible.
    SET( H3DViewer_COMPILE_FLAGS "${H3DViewer_COMPILE_FLAGS} -D_CRT_SECURE_NO_DEPRECATE" )
    
    # This might be useful for visual studio 2005 users that often recompile the api.
    IF( NOT DEFINED USE_VC8_MP_FLAG )
      SET( USE_VC8_MP_FLAG "NO" CACHE BOOL "In visual studio 8 the MP flag exists but is not documented. Maybe it is unsafe to use. If you want to use it then set this flag to yes." )
    ENDIF( NOT DEFINED USE_VC8_MP_FLAG )
    
    IF( USE_VC8_MP_FLAG )
      SET( H3DViewer_COMPILE_FLAGS "${H3DViewer_COMPILE_FLAGS} /MP" )
    ENDIF( USE_VC8_MP_FLAG )

    # Needed to run in debug mode. Removes warning about conflict between libraries.
    SET_TARGET_PROPERTIES( H3DViewer PROPERTIES LINK_FLAGS_DEBUG "/NODEFAULTLIB:msvcrt" )
  ELSEIF(MSVC90 OR MSVC10)
    # Remove some warnings about deprecated functions. Compile on multiple processors/threads if possible.
    SET( H3DViewer_COMPILE_FLAGS "${H3DViewer_COMPILE_FLAGS} -D_CRT_SECURE_NO_DEPRECATE /MP" )
    # Needed to run in debug mode. Removes warning about conflict between libraries.
    SET_TARGET_PROPERTIES( H3DViewer PROPERTIES LINK_FLAGS_DEBUG "/NODEFAULTLIB:msvcrt" )
  ENDIF(MSVC70 OR MSVC71)

  SET_TARGET_PROPERTIES( H3DViewer PROPERTIES COMPILE_FLAGS "${H3DViewer_COMPILE_FLAGS}" )

  # set the install directory to the H3D directory on Windows
  IF( CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT )
    SET( CMAKE_INSTALL_PREFIX ${H3DViewer_SOURCE_DIR}/../../.. CACHE PATH "Install path prefix, prepended onto install directories." FORCE )
  ENDIF( CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT )
ENDIF(WIN32)

# Install in these folders.
IF( APPLE )
 INSTALL( TARGETS H3DViewer
          LIBRARY DESTINATION lib
          RUNTIME DESTINATION bin
          BUNDLE DESTINATION "/Applications" )
ELSE( APPLE )
  INSTALL( TARGETS H3DViewer
           LIBRARY DESTINATION lib
           RUNTIME DESTINATION bin )
ENDIF( APPLE )

# Add a cache variable GENERATE_H3DVIEWER_CPACK_PROJECT to have the choice of generating a project
# for packaging H3DViewer. Default is NO since most people will not use this.
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
  SET(CPACK_PACKAGE_VERSION_MAJOR ${H3DViewer_MAJOR_VERSION})
  SET(CPACK_PACKAGE_VERSION_MINOR ${H3DViewer_MINOR_VERSION})
  SET(CPACK_PACKAGE_VERSION_PATCH ${H3DViewer_BUILD_VERSION})
  SET(CPACK_PACKAGE_INSTALL_REGISTRY_KEY "H3DViewer ${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}" )


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
  ELSEIF( WIN32 )

    SET( H3DVIEWER_PLUGIN_DIRECTORY CACHE PATH "Set this to the directory containing all plugins that should be distributed with H3DViewer" )

    SET( H3D_VIEWER_CPACK_IGNORE_FILES "" )
    IF(MSVC70 OR MSVC71)
      SET( H3D_VIEWER_CPACK_IGNORE_FILES _vc8\\\\.dll$
                                         _vc9\\\\.dll$ )
    ELSEIF(MSVC80)
      SET( H3D_VIEWER_CPACK_IGNORE_FILES _vc7\\\\.dll$
                                         _vc9\\\\.dll$ )
    ELSEIF(MSVC90)
      SET( H3D_VIEWER_CPACK_IGNORE_FILES _vc7\\\\.dll$
                                         _vc8\\\\.dll$ )
    ENDIF(MSVC70 OR MSVC71)

    # File patterns to ignore.
    SET(CPACK_IGNORE_FILES ${H3D_VIEWER_CPACK_IGNORE_FILES}
                           /\\\\.svn/
                           \\\\.obj$
                           \\\\.ncb$
                           \\\\.log$
                           \\\\.suo$
                           \\\\.dir/
                           \\\\.user$
                           \\\\.cv$
                           /pthreadGC2\\\\.dll$
                           /pthreadGCE2\\\\.dll$
                           /pthreadVSE2\\\\.dll$
                           "/HapticAPI2\\\\.(lib)*(dll)*(h)*$"
                           /xerces-c_2_7\\\\.dll$
                           /xerces-c_2_7D\\\\.dll$
                           "/DentalSimAPI\\\\.(lib)*(dll)*(h)*$"
                           "/ogg_d\\\\.dll$"
                           "_d\\\\.(lib)*(dll)*(exe)*$"
                           "H3DLoad\\\\.exe$" )

    SET( H3DVIEWER_INSTALLED_DIRECTORIES "${H3DViewer_SOURCE_DIR}/../../../bin;bin"
                                         "${H3DViewer_SOURCE_DIR}/../../../External/bin;bin"
                                         "${H3DViewer_SOURCE_DIR}/../../Util/H3DViewerPackageExtraFiles;." )

    IF( H3DVIEWER_PLUGIN_DIRECTORY )
      SET( H3DVIEWER_INSTALLED_DIRECTORIES ${H3DVIEWER_INSTALLED_DIRECTORIES}
                                           "${H3DVIEWER_PLUGIN_DIRECTORY};Plugins" )
    ENDIF( H3DVIEWER_PLUGIN_DIRECTORY )
    
    # Add a cache variable vc_redist_folder which should be set to a folder containing all needed
    # visual studio redistributables.
    IF( NOT DEFINED vc_redist_folder )
      SET( vc_redist_folder CACHE PATH "Set this to a folder containing all the needed visual studio redistributable dlls." )
      MARK_AS_ADVANCED(vc_redist_folder)
    ENDIF( NOT DEFINED vc_redist_folder )

    IF( vc_redist_folder )
      SET( H3DVIEWER_INSTALLED_DIRECTORIES ${H3DVIEWER_INSTALLED_DIRECTORIES}
                                           "${vc_redist_folder};bin" )
    ENDIF( vc_redist_folder )

    # Directories to install, and how they should be named. Non-existing folders will be ignored when packing.
    SET(CPACK_INSTALLED_DIRECTORIES ${H3DVIEWER_INSTALLED_DIRECTORIES} )

    # Start menu shortcut.
    SET( CPACK_PACKAGE_EXECUTABLES ${CPACK_PACKAGE_EXECUTABLES}
                                   "H3DViewer;H3DViewer" )

    SET( CPACK_NSIS_EXTRA_INSTALL_COMMANDS "\\n" )

    # Install python if not already installed
    SET( PythonInstallMSI "" CACHE FILEPATH "Needs to be set to add python installation to the package." )
    MARK_AS_ADVANCED(PythonInstallMSI)
    IF( PythonInstallMSI )
      STRING( REGEX MATCH 2\\.[456789] CPACK_PYTHON_VERSION ${PythonInstallMSI} )
      GET_FILENAME_COMPONENT( PYTHON_FILE_NAME ${PythonInstallMSI} NAME )
      STRING( REPLACE "/" "\\\\" TEMP_PythonInstallMSI ${PythonInstallMSI} )
      SET( CPACK_NSIS_EXTRA_INSTALL_COMMANDS ${CPACK_NSIS_EXTRA_INSTALL_COMMANDS}
                                             " Code to install Python\\n  ReadRegStr $0 HKLM SOFTWARE\\\\Python\\\\PythonCore\\\\${CPACK_PYTHON_VERSION}\\\\InstallPath \\\"\\\"\\n"
                                             " Check if python is installed\\n  StrCmp $0 \\\"\\\" 0 +5\\n"
                                             " Extract python installer\\n  File \\\"${TEMP_PythonInstallMSI}\\\"\\n"
                                             " Execute python installer, wait for completion\\n  ExecWait '\\\"msiexec\\\" /i \\\"$INSTDIR\\\\${PYTHON_FILE_NAME}\\\"'  $0\\n"
                                             " Wait a bit for system to unlock file.\\n  Sleep 1000\\n"
                                             " Delete python installer\\n  Delete \\\"$INSTDIR\\\\${PYTHON_FILE_NAME}\\\"\\n\\n" )
    ENDIF( PythonInstallMSI )
    
    # Install OpenAL.
    SET( OpenAlInstallExe "" CACHE FILEPATH "Needs to be set to add openal installation to the package." )
    MARK_AS_ADVANCED(OpenAlInstallExe)
    IF( OpenAlInstallExe )
      GET_FILENAME_COMPONENT( OpenAL_FILE_NAME ${OpenAlInstallExe} NAME )
      STRING( REPLACE "/" "\\\\" TEMP_OpenAlInstallExe ${OpenAlInstallExe} )
      SET( CPACK_NSIS_EXTRA_INSTALL_COMMANDS ${CPACK_NSIS_EXTRA_INSTALL_COMMANDS}
                                             " Code to install OPENAL\\n  File \\\"${TEMP_OpenAlInstallExe}\\\"\\n"
                                             " Execute install file\\n  ExecWait \\\"$INSTDIR\\\\${OpenAL_FILE_NAME}\\\" $0\\n"
                                             " Wait a bit for system to unlock file.\\n  Sleep 1000\\n"
                                             " Delete install file\\n  Delete \\\"$INSTDIR\\\\${OpenAL_FILE_NAME}\\\"\\n" )
    ENDIF( OpenAlInstallExe )

    SET( CPACK_NSIS_MODIFY_PATH "OFF" )
  ENDIF( APPLE )

  INCLUDE(CPack)

ENDIF( GENERATE_H3DVIEWER_CPACK_PROJECT )
