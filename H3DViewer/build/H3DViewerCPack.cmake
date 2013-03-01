IF( NOT TARGET H3DViewer )
  MESSAGE( FATAL_ERROR "Include file H3DViewerCPack.cmake require the target H3DViewer to exist. Please add H3DAPI/H3DViewer/build/CMakeLists.txt as subdirectory first." )
ENDIF( NOT TARGET H3DViewer )

# Add all sources, they are added to a variable called H3DViewer_SRCS defined
# in the included file. All header files are added to a variable called
# H3DViewer_HEADERS.
INCLUDE( ${H3DViewer_SOURCE_DIR}/H3DViewerSourceFiles.txt )

# To allow other projects that use H3DVIEWER as a subproject to add extra include directories
# when packaging.
IF( GENERATE_H3DVIEWER_CPACK_PROJECT )
  IF( WIN32 )
    # Add a cache variable which indicates where the Externals directory used for packaging
    # H3DViewer is located. If not set then FIND modules will be used instead.
    IF( NOT DEFINED H3DViewer_CPACK_EXTERNAL_ROOT )
      SET( H3DViewer_CPACK_EXTERNAL_ROOT_DEFAULT "" )
      IF( H3D_USE_DEPENDENCIES_ONLY )
        foreach( EXTERNAL_INCLUDE_DIR_TMP ${EXTERNAL_INCLUDE_DIR} )
          IF( EXISTS ${EXTERNAL_INCLUDE_DIR_TMP}/../include/pthread )
            SET( H3DViewer_CPACK_EXTERNAL_ROOT_DEFAULT "${EXTERNAL_INCLUDE_DIR_TMP}/.." )
          ENDIF( EXISTS ${EXTERNAL_INCLUDE_DIR_TMP}/../include/pthread )
        endforeach( EXTERNAL_INCLUDE_DIR_TMP ${EXTERNAL_INCLUDE_DIR} )
      ELSE( H3D_USE_DEPENDENCIES_ONLY )
        SET( H3DViewer_CPACK_EXTERNAL_ROOT_DEFAULT "$ENV{H3D_EXTERNAL_ROOT}" )
      ENDIF( H3D_USE_DEPENDENCIES_ONLY )
      SET( H3DViewer_CPACK_EXTERNAL_ROOT "${H3DViewer_CPACK_EXTERNAL_ROOT_DEFAULT}" CACHE PATH "Set to the External directory used with H3DViewer, needed to pack properly. If not set FIND_modules will be used instead." )
      MARK_AS_ADVANCED(H3DViewer_CPACK_EXTERNAL_ROOT)
    ENDIF( NOT DEFINED H3DViewer_CPACK_EXTERNAL_ROOT )
  ENDIF( WIN32 )
  
  # Set information properties about the project to install.
  SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "H3DViewer. An X3D viewer with extensions for haptics." )
  SET(CPACK_PACKAGE_VENDOR "SenseGraphics AB")
  SET(CPACK_PACKAGE_CONTACT "support@sensegraphics.com" )
  SET(CPACK_RESOURCE_FILE_LICENSE "${H3DViewer_SOURCE_DIR}/../../LICENSE")
  SET(CPACK_INSTALL_CMAKE_PROJECTS "${CMAKE_CURRENT_BINARY_DIR};H3DViewer;ALL;/")
  
  # Installation directory for package.
  SET(CPACK_PACKAGE_INSTALL_DIRECTORY "SenseGraphics" )
  
  SET(CPACK_PACKAGE_VERSION_MAJOR ${H3DViewer_MAJOR_VERSION})
  SET(CPACK_PACKAGE_VERSION_MINOR ${H3DViewer_MINOR_VERSION})
  SET(CPACK_PACKAGE_VERSION_PATCH ${H3DViewer_BUILD_VERSION})
  SET(CPACK_PACKAGE_INSTALL_REGISTRY_KEY "H3DViewer ${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}" )
  SET(CPACK_NSIS_PACKAGE_NAME "H3DViewer ${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}" )
  SET(CPACK_NSIS_UNINSTALL_NAME "H3DViewer-${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}" )
  
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

      INSTALL( FILES ${OSX_PLUGIN_LIBRARIES}
               DESTINATION Plugins )

      INSTALL( CODE "EXECUTE_PROCESS( COMMAND \"python\" ${H3DViewer_SOURCE_DIR}/osx_bundle.py   
                                      WORKING_DIRECTORY \${CMAKE_INSTALL_PREFIX})" )
    ENDIF( H3DVIEWER_CPACK_INCLUDE_LIBRARIES )
  ENDIF( APPLE )
  
  IF( WIN32 )
    SET(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL "ON" )
    SET(CPACK_MONOLITHIC_INSTALL "TRUE" )
    SET( CPACK_PACKAGE_START_MENU_NAME "H3DViewer ${H3DViewer_MAJOR_VERSION}.${H3DViewer_MINOR_VERSION}" )
                           
    SET(CPACK_PACKAGE_VERSION_MAJOR ${H3DViewer_MAJOR_VERSION})
    SET(CPACK_PACKAGE_VERSION_MINOR ${H3DViewer_MINOR_VERSION})
    SET(CPACK_PACKAGE_VERSION_PATCH ${H3DViewer_BUILD_VERSION})
    
    SET( EXTERNAL_BIN_PATH "bin32" )
    SET( EXTERNAL_BIN_REPLACE_PATH "bin64" )
    SET( CPACK_PACKAGE_NAME "H3DViewer" )
    # CPACK_NSIS_INSTALL_ROOT must be set properly because cmake does not set it correctly
    # for a 64 bit build.
    SET( CPACK_NSIS_INSTALL_ROOT "$PROGRAMFILES32" )
    SET( CPACK_NSIS_DISPLAY_NAME_POSTFIX "(x86)" )
    SET( CPACK_H3D_64_BIT "FALSE" )
    SET( CPACK_NSIS_EXECUTABLES_DIRECTORY bin32 )
    IF( CMAKE_SIZEOF_VOID_P EQUAL 8 ) # check if the system is 64 bit
      SET( EXTERNAL_BIN_PATH "bin64" )
      SET( CPACK_NSIS_EXECUTABLES_DIRECTORY bin64 )
      SET( EXTERNAL_BIN_REPLACE_PATH "bin32" )
      SET( CPACK_NSIS_INSTALL_ROOT "$PROGRAMFILES64" )
      SET( CPACK_H3D_64_BIT "TRUE" )
      SET( CPACK_NSIS_DISPLAY_NAME_POSTFIX "" )
    ENDIF( CMAKE_SIZEOF_VOID_P EQUAL 8 )
    SET(CPACK_NSIS_DISPLAY_NAME "H3DViewer${CPACK_NSIS_DISPLAY_NAME_POSTFIX} ${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}" )

    SET( CPACK_NSIS_EXTRA_INSTALL_COMMANDS "\\n" )
    SET( CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS "\\n" )
    
    SET( redist_versions 8 9 10 )
    foreach( redist_version ${redist_versions} )
      # Add cache variable vc${redist_version}_redist which should be set to the install file
      # for microsoft visual studio redistributables, they can be found in the
      # installation folder for each visual studio installation.
      IF( NOT DEFINED vc${redist_version}_redist )
        SET( vc${redist_version}_redist CACHE FILEPATH "Set this to the exe installing microsoft visual studio redistributable for visual studio ${redist_version}" )
        MARK_AS_ADVANCED(vc${redist_version})
      ENDIF( NOT DEFINED vc${redist_version}_redist )
      IF( vc${redist_version}_redist )
        STRING( REPLACE "/" "\\\\" Temp_vc${redist_version}_redist ${vc${redist_version}_redist} )
        GET_FILENAME_COMPONENT( VC${redist_version}_FILE_NAME ${vc${redist_version}_redist} NAME )
        SET( MS_REDIST_INSTALL_COMMAND_1 " Set output Path\\n  SetOutPath \\\"$INSTDIR\\\\vc${redist_version}\\\"\\n"
                                         " Code to install Visual studio redistributable\\n  File \\\"${Temp_vc${redist_version}_redist}\\\"\\n" )
        SET( CPACK_NSIS_EXTRA_INSTALL_COMMANDS ${CPACK_NSIS_EXTRA_INSTALL_COMMANDS}
                                               ${MS_REDIST_INSTALL_COMMAND_1} )
        SET( CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS ${CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS}
                                                 " Check if uninstall vc redist \\n  MessageBox MB_YESNO \\\"Do you want to uninstall Visual studio ${redist_version} redistributable? It is recommended if no other applications use it.\\\" IDYES uninstall_vcredist_yes IDNO uninstall_vcredist_no\\n"
                                                 " A comment \\n  uninstall_vcredist_yes:\\n"
                                                 ${MS_REDIST_INSTALL_COMMAND_1} )
        IF( ${redist_version} LESS 9 )
          SET( CPACK_NSIS_EXTRA_INSTALL_COMMANDS ${CPACK_NSIS_EXTRA_INSTALL_COMMANDS}
                                                 " Execute silent and wait\\n  ExecWait '\\\"$INSTDIR\\\\vc${redist_version}\\\\${VC${redist_version}_FILE_NAME}\\\" /q:a /norestart /c:\\\"msiexec /i vcredist.msi /qn\\\"'\\n" )
          SET( CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS ${CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS}
                                                 " Execute silent and wait\\n  ExecWait '\\\"$INSTDIR\\\\vc${redist_version}\\\\${VC${redist_version}_FILE_NAME}\\\" /q:a /norestart /c:\\\"msiexec /x vcredist.msi /qn\\\"'\\n" )
        ELSE( )
          SET( CPACK_NSIS_EXTRA_INSTALL_COMMANDS ${CPACK_NSIS_EXTRA_INSTALL_COMMANDS}
                                                 " Execute silent and wait\\n  ExecWait '\\\"$INSTDIR\\\\vc${redist_version}\\\\${VC${redist_version}_FILE_NAME}\\\" /q /norestart \\\"'\\n" )
          SET( CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS ${CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS}
                                                 " Execute silent and wait\\n  ExecWait '\\\"$INSTDIR\\\\vc${redist_version}\\\\${VC${redist_version}_FILE_NAME}\\\" /q /uninstall \\\"'\\n" )
        ENDIF( ${redist_version} LESS 9 )
        SET( MS_REDIST_INSTALL_COMMAND_2 " Wait a bit for system to unlock file.\\n  Sleep 1000\\n"
                                         " Delete file\\n  Delete \\\"$INSTDIR\\\\vc${redist_version}\\\\${VC${redist_version}_FILE_NAME}\\\"\\n"
                                         " Reset output Path\\n  SetOutPath \\\"$INSTDIR\\\"\\n"
                                         " Remove folder\\n  RMDir /r \\\"$INSTDIR\\\\vc${redist_version}\\\"\\n\\n" )
        SET( CPACK_NSIS_EXTRA_INSTALL_COMMANDS ${CPACK_NSIS_EXTRA_INSTALL_COMMANDS}
                                               ${MS_REDIST_INSTALL_COMMAND_2} )
        SET( CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS ${CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS}
                                                 ${MS_REDIST_INSTALL_COMMAND_2}
                                                 " A comment \\n  uninstall_vcredist_no:\\n\\n" )
      ENDIF( vc${redist_version}_redist )
    endforeach( redist_version ${redist_versions} )
    
    # Install python if not already installed
    SET( PythonInstallMSI "" CACHE FILEPATH "Needs to be set to add python installation to the package." )
    MARK_AS_ADVANCED(PythonInstallMSI)
    IF( PythonInstallMSI )
      STRING( REGEX MATCH 2\\.[456789] CPACK_PYTHON_VERSION ${PythonInstallMSI} )
      STRING( REGEX REPLACE \\. "" CPACK_PYTHON_VERSION_NO_DOT ${CPACK_PYTHON_VERSION} )
      GET_FILENAME_COMPONENT( PYTHON_FILE_NAME ${PythonInstallMSI} NAME )
      STRING( REPLACE "/" "\\\\" TEMP_PythonInstallMSI ${PythonInstallMSI} )
      SET( PYTHON_INSTALL_COMMAND_1 " Code to install Python\\n  ReadRegStr $0 HKLM SOFTWARE\\\\Python\\\\PythonCore\\\\${CPACK_PYTHON_VERSION}\\\\InstallPath \\\"\\\"\\n" )
      SET( PYTHON_INSTALL_COMMAND_2 " Extract python installer\\n  File \\\"${TEMP_PythonInstallMSI}\\\"\\n" )
      SET( PYTHON_INSTALL_COMMAND_3 " Wait a bit for system to unlock file.\\n  Sleep 1000\\n"
                                    " Delete python installer\\n  Delete \\\"$INSTDIR\\\\${PYTHON_FILE_NAME}\\\"\\n\\n" )
      
      SET( CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS ${CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS}
                                               ${PYTHON_INSTALL_COMMAND_1}
                                               " Check if python is installed\\n  StrCmp $0 \\\"\\\" uninstall_python_no 0\\n"
                                               " Check if uninstall python \\n  MessageBox MB_YESNO \\\"Do you want to uninstall python? It is recommended if no other applications use python ${CPACK_PYTHON_VERSION}.\\\" IDYES uninstall_python_yes IDNO uninstall_python_no\\n"
                                               " A comment \\n  uninstall_python_yes:\\n"
                                               ${PYTHON_INSTALL_COMMAND_2}
                                               " Execute python installer, wait for completion\\n  ExecWait '\\\"msiexec\\\" /x \\\"$INSTDIR\\\\${PYTHON_FILE_NAME}\\\" /qn'\\n"
                                               ${PYTHON_INSTALL_COMMAND_3}
                                               " A comment \\n  uninstall_python_no:\\n" )
      SET( CPACK_NSIS_EXTRA_INSTALL_COMMANDS ${CPACK_NSIS_EXTRA_INSTALL_COMMANDS}
                                             ${PYTHON_INSTALL_COMMAND_1}
                                             " Check if python is installed\\n  StrCmp $0 \\\"\\\" 0 install_python_no\\n"
                                             ${PYTHON_INSTALL_COMMAND_2}
                                             "A comment \\n  ClearErrors\\n"
                                             "Check if python install path is free \\n  GetFullPathName $0 C:\\\\Python${CPACK_PYTHON_VERSION_NO_DOT}\\n"
                                             "If errors then path was not found, i.e. empty\\n  IfErrors 0 python_install_not_hidden \\n"
                                             "A comment \\n    ClearErrors\\n"
                                             " Execute python installer silent, wait for completion\\n  ExecWait '\\\"msiexec\\\" /i \\\"$INSTDIR\\\\${PYTHON_FILE_NAME}\\\" /qn ALLUSERS=1'\\n"
                                             "A comment \\n Goto python_end_install\\n"
                                             "A comment \\n python_install_not_hidden:\\n"
                                             " Execute python installer, wait for completion\\n  ExecWait '\\\"msiexec\\\" /i \\\"$INSTDIR\\\\${PYTHON_FILE_NAME}\\\"'\\n"
                                             " A comment \\n  python_end_install:\\n"
                                             ${PYTHON_INSTALL_COMMAND_3}
                                             " A comment \\n  install_python_no:\\n" )
    ENDIF( PythonInstallMSI )
    
    # Install OpenAL.
    SET( OpenAlInstallExe "" CACHE FILEPATH "Needs to be set to add openal installation to the package." )
    MARK_AS_ADVANCED(OpenAlInstallExe)
    IF( OpenAlInstallExe )
      GET_FILENAME_COMPONENT( OpenAL_FILE_NAME ${OpenAlInstallExe} NAME )
      STRING( REPLACE "/" "\\\\" TEMP_OpenAlInstallExe ${OpenAlInstallExe} )
      SET( OPENAL_INSTALL_COMMAND_1 " Code to install OPENAL\\n  File \\\"${TEMP_OpenAlInstallExe}\\\"\\n" )
      SET( OPENAL_INSTALL_COMMAND_2 " Wait a bit for system to unlock file.\\n  Sleep 1000\\n"
                                    " Delete install file\\n  Delete \\\"$INSTDIR\\\\${OpenAL_FILE_NAME}\\\"\\n" )
      SET( CPACK_NSIS_EXTRA_INSTALL_COMMANDS ${CPACK_NSIS_EXTRA_INSTALL_COMMANDS}
                                             ${OPENAL_INSTALL_COMMAND_1}
                                             " Execute install file\\n  ExecWait '\\\"$INSTDIR\\\\${OpenAL_FILE_NAME}\\\" /s'\\n"
                                             ${OPENAL_INSTALL_COMMAND_2} )
      IF( CMAKE_SIZEOF_VOID_P EQUAL 8 ) # check if the system is 64 bit
        SET( CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS ${CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS}
                                                 "A comment\\n \\\${If} \\\${RunningX64}\\n"
                                                 "A comment\\n   SetRegView 32\\n"
                                                 "A comment\\n \\\${EndIf}\\n" )
      ENDIF( CMAKE_SIZEOF_VOID_P EQUAL 8 )
      SET( CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS ${CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS}
                                               " Get OpenAL uninstall registry string\\n  ReadRegStr $0 HKLM SOFTWARE\\\\Microsoft\\\\Windows\\\\CurrentVersion\\\\Uninstall\\\\OpenAL \\\"UninstallString\\\"\\n"
                                               " Check if OpenAL is installed\\n  StrCmp $0 \\\"\\\" uninstall_openal_no 0\\n"
                                               " Check if uninstall OpenAL \\n  MessageBox MB_YESNO \\\"Do you want to uninstall OpenAL? It is recommended if no other applications use it.\\\" IDYES uninstall_openal_yes IDNO uninstall_openal_no\\n"
                                               " A comment \\n  uninstall_openal_yes:\\n"
                                               ${OPENAL_INSTALL_COMMAND_1}
                                               " Execute install file\\n  ExecWait '\\\"$INSTDIR\\\\${OpenAL_FILE_NAME}\\\" /u /s'\\n"
                                               ${OPENAL_INSTALL_COMMAND_2}
                                               " A comment \\n  uninstall_openal_no:\\n\\n" )
      IF( CMAKE_SIZEOF_VOID_P EQUAL 8 ) # check if the system is 64 bit
        SET( CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS ${CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS}
                                                 "A comment\\n \\\${If} \\\${RunningX64}\\n"
                                                 "A comment\\n   SetRegView 64\\n"
                                                 "A comment\\n \\\${EndIf}\\n" )
      ENDIF( CMAKE_SIZEOF_VOID_P EQUAL 8 )
    ENDIF( OpenAlInstallExe )

    # Modify path since in the NSIS template.
    SET( CPACK_NSIS_MODIFY_PATH "ON" )
  
    IF( EXISTS ${H3DViewer_CPACK_EXTERNAL_ROOT} )
      SET( EXTERNAL_BINARIES ${EXTERNAL_BINARIES}
                             ${H3DViewer_CPACK_EXTERNAL_ROOT}/${EXTERNAL_BIN_PATH}/audiofile.dll
                             ${H3DViewer_CPACK_EXTERNAL_ROOT}/${EXTERNAL_BIN_PATH}/cg.dll
                             ${H3DViewer_CPACK_EXTERNAL_ROOT}/${EXTERNAL_BIN_PATH}/cgGL.dll
                             ${H3DViewer_CPACK_EXTERNAL_ROOT}/${EXTERNAL_BIN_PATH}/EntactAPI.dll
                             ${H3DViewer_CPACK_EXTERNAL_ROOT}/${EXTERNAL_BIN_PATH}/freeglut.dll
                             ${H3DViewer_CPACK_EXTERNAL_ROOT}/${EXTERNAL_BIN_PATH}/FreeImage.dll
                             ${H3DViewer_CPACK_EXTERNAL_ROOT}/${EXTERNAL_BIN_PATH}/ftgl.dll
                             ${H3DViewer_CPACK_EXTERNAL_ROOT}/${EXTERNAL_BIN_PATH}/glew32.dll
                             ${H3DViewer_CPACK_EXTERNAL_ROOT}/${EXTERNAL_BIN_PATH}/js32.dll
                             ${H3DViewer_CPACK_EXTERNAL_ROOT}/${EXTERNAL_BIN_PATH}/libcurl.dll
                             ${H3DViewer_CPACK_EXTERNAL_ROOT}/${EXTERNAL_BIN_PATH}/libogg.dll
                             ${H3DViewer_CPACK_EXTERNAL_ROOT}/${EXTERNAL_BIN_PATH}/libvorbis.dll
                             ${H3DViewer_CPACK_EXTERNAL_ROOT}/${EXTERNAL_BIN_PATH}/libvorbisfile.dll
                             ${H3DViewer_CPACK_EXTERNAL_ROOT}/${EXTERNAL_BIN_PATH}/pthreadVC2.dll
                             ${H3DViewer_CPACK_EXTERNAL_ROOT}/${EXTERNAL_BIN_PATH}/SimballMedicalHID.dll
                             ${H3DViewer_CPACK_EXTERNAL_ROOT}/${EXTERNAL_BIN_PATH}/teem.dll
                             ${H3DViewer_CPACK_EXTERNAL_ROOT}/${EXTERNAL_BIN_PATH}/wxbase293u_vc_custom.dll
                             ${H3DViewer_CPACK_EXTERNAL_ROOT}/${EXTERNAL_BIN_PATH}/wxbase293u_xml_vc_custom.dll
                             ${H3DViewer_CPACK_EXTERNAL_ROOT}/${EXTERNAL_BIN_PATH}/wxmsw293u_adv_vc_custom.dll
                             ${H3DViewer_CPACK_EXTERNAL_ROOT}/${EXTERNAL_BIN_PATH}/wxmsw293u_core_vc_custom.dll
                             ${H3DViewer_CPACK_EXTERNAL_ROOT}/${EXTERNAL_BIN_PATH}/wxmsw293u_gl_vc_custom.dll
                             ${H3DViewer_CPACK_EXTERNAL_ROOT}/${EXTERNAL_BIN_PATH}/wxmsw293u_html_vc_custom.dll
                             ${H3DViewer_CPACK_EXTERNAL_ROOT}/${EXTERNAL_BIN_PATH}/wxmsw293u_propgrid_vc_custom.dll
                             ${H3DViewer_CPACK_EXTERNAL_ROOT}/${EXTERNAL_BIN_PATH}/wxmsw293u_richtext_vc_custom.dll
                             ${H3DViewer_CPACK_EXTERNAL_ROOT}/${EXTERNAL_BIN_PATH}/xerces-c_3_1.dll
                             ${H3DViewer_CPACK_EXTERNAL_ROOT}/${EXTERNAL_BIN_PATH}/zlib1.dll
                             ${H3DViewer_CPACK_EXTERNAL_ROOT}/${EXTERNAL_BIN_PATH}/ode_double.dll)
    ENDIF( EXISTS ${H3DViewer_CPACK_EXTERNAL_ROOT} )

    foreach( binary ${EXTERNAL_BINARIES} )
      IF( EXISTS ${binary} )
        INSTALL( FILES ${binary}
                 DESTINATION H3DViewer/${DEFAULT_BIN_INSTALL} )
      ENDIF( EXISTS ${binary} )
    endforeach( binary )
    
    SET( H3D_MSVC_VERSION 6 )
    SET( TEMP_MSVC_VERSION 1299 )
    WHILE( ${MSVC_VERSION} GREATER ${TEMP_MSVC_VERSION} )
      MATH( EXPR H3D_MSVC_VERSION "${H3D_MSVC_VERSION} + 1" )
      MATH( EXPR TEMP_MSVC_VERSION "${TEMP_MSVC_VERSION} + 100" )
    ENDWHILE( ${MSVC_VERSION} GREATER ${TEMP_MSVC_VERSION} )

    IF( H3D_USE_DEPENDENCIES_ONLY )
      INSTALL( FILES ${H3DViewer_BINARY_DIR}/../HAPI/H3DUtil/Release/H3DUtil_vc${H3D_MSVC_VERSION}.dll
                     ${H3DViewer_BINARY_DIR}/../HAPI/Release/HAPI_vc${H3D_MSVC_VERSION}.dll
                     ${H3DViewer_BINARY_DIR}/../Release/H3DAPI_vc${H3D_MSVC_VERSION}.dll
                     CONFIGURATIONS Release
                     DESTINATION H3DViewer/${DEFAULT_BIN_INSTALL} )
      IF( TARGET OpenHapticsRenderer )
        INSTALL( FILES ${H3DViewer_BINARY_DIR}/../HAPI/Release/OpenHapticsRenderer_vc${H3D_MSVC_VERSION}.dll
                       CONFIGURATIONS Release
                       DESTINATION H3DViewer/${DEFAULT_BIN_INSTALL} )
      ENDIF( TARGET OpenHapticsRenderer )
      IF( TARGET Chai3DRenderer )
        INSTALL( FILES ${H3DViewer_BINARY_DIR}/../HAPI/Release/Chai3DRenderer_vc${H3D_MSVC_VERSION}.dll
                       CONFIGURATIONS Release
                       DESTINATION H3DViewer/${DEFAULT_BIN_INSTALL} )
      ENDIF( TARGET Chai3DRenderer )
      
      # these part are added separately so that these plug in can be automatically added to H3DViewer
      INSTALL( FILES ${H3DViewer_BINARY_DIR}/../../H3DPhysics/Release/H3DPhysics_vc${H3D_MSVC_VERSION}.dll
                     ${H3DViewer_BINARY_DIR}/../../MedX3D/Release/MedX3D_vc${H3D_MSVC_VERSION}.dll
                     ${H3DViewer_BINARY_DIR}/../../UI/Release/UI_vc${H3D_MSVC_VERSION}.dll
                     CONFIGURATIONS Release
                     DESTINATION H3DViewer/plugins )
    ELSE( H3D_USE_DEPENDENCIES_ONLY )
      INSTALL( FILES ${H3DViewer_SOURCE_DIR}/../../../${DEFAULT_BIN_INSTALL}/H3DUtil_vc${H3D_MSVC_VERSION}.dll
                     ${H3DViewer_SOURCE_DIR}/../../../${DEFAULT_BIN_INSTALL}/HAPI_vc${H3D_MSVC_VERSION}.dll
                     ${H3DViewer_SOURCE_DIR}/../../../${DEFAULT_BIN_INSTALL}/H3DAPI_vc${H3D_MSVC_VERSION}.dll
                     CONFIGURATIONS Release
                     DESTINATION H3DViewer/${DEFAULT_BIN_INSTALL} )
      IF( TARGET OpenHapticsRenderer )
        INSTALL( FILES ${H3DViewer_SOURCE_DIR}/../../../${DEFAULT_BIN_INSTALL}/OpenHapticsRenderer_vc${H3D_MSVC_VERSION}.dll
                       CONFIGURATIONS Release
                       DESTINATION H3DViewer/${DEFAULT_BIN_INSTALL} )
      ENDIF( TARGET OpenHapticsRenderer )
      IF( TARGET Chai3DRenderer )
        INSTALL( FILES ${H3DViewer_SOURCE_DIR}/../../../${DEFAULT_BIN_INSTALL}/Chai3DRenderer_vc${H3D_MSVC_VERSION}.dll
                       CONFIGURATIONS Release
                       DESTINATION H3DViewer/${DEFAULT_BIN_INSTALL} )
      ENDIF( TARGET Chai3DRenderer )
      # these part are added separately so that these plug in can be automatically added to H3DViewer
      INSTALL( FILES ${H3DViewer_SOURCE_DIR}/../../../${DEFAULT_BIN_INSTALL}/H3DPhysics_vc${H3D_MSVC_VERSION}.dll
                     ${H3DViewer_SOURCE_DIR}/../../../${DEFAULT_BIN_INSTALL}/MedX3D_vc${H3D_MSVC_VERSION}.dll
                     ${H3DViewer_SOURCE_DIR}/../../../${DEFAULT_BIN_INSTALL}/UI_vc${H3D_MSVC_VERSION}.dll
                     CONFIGURATIONS Release
                     DESTINATION H3DViewer/plugins )
    ENDIF( H3D_USE_DEPENDENCIES_ONLY )
    
    IF( EXISTS ${H3DViewer_SOURCE_DIR}/../../Util/H3DViewerPackageExtraFiles )
      INSTALL( FILES ${H3DViewer_SOURCE_DIR}/../../Util/H3DViewerPackageExtraFiles/ACKNOWLEDGEMENTS
                     ${H3DViewer_SOURCE_DIR}/../../Util/H3DViewerPackageExtraFiles/NOTICE
                     ${H3DViewer_SOURCE_DIR}/../../Util/H3DViewerPackageExtraFiles/ReadMe.txt
             DESTINATION H3DViewer )
    ENDIF( EXISTS ${H3DViewer_SOURCE_DIR}/../../Util/H3DViewerPackageExtraFiles )
  ENDIF( WIN32 )  
  
  INCLUDE(CPack)

ENDIF( GENERATE_H3DVIEWER_CPACK_PROJECT )