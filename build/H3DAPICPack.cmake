IF( NOT TARGET H3DAPI )
  MESSAGE( FATAL_ERROR "Include file H3DAPICPack.cmake require the target H3DAPI to exist. Please add H3DAPI/build/CMakeLists.txt as subdirectory first." )
ENDIF( NOT TARGET H3DAPI )

# Add all sources, they are added to a variable called H3DAPI_SRCS defined
# in the included file. All header files are added to a variable called
# H3DAPI_HEADERS.
INCLUDE( ${H3DAPI_SOURCE_DIR}/H3DAPISourceFiles.txt )

# If cpack should be configured.
IF( GENERATE_CPACK_PROJECT )
  IF( WIN32 )
    # Add a cache variable which indicates where the Externals directory used for packaging
    # HAPI is located. If not set then FIND modules will be used instead.
    IF( NOT DEFINED H3DAPI_CPACK_EXTERNAL_ROOT )
      SET( H3DAPI_CPACK_EXTERNAL_ROOT_DEFAULT "" )
      IF( H3D_USE_DEPENDENCIES_ONLY )
        foreach( EXTERNAL_INCLUDE_DIR_TMP ${EXTERNAL_INCLUDE_DIR} )
          IF( EXISTS ${EXTERNAL_INCLUDE_DIR_TMP}/../include/pthread )
            SET( H3DAPI_CPACK_EXTERNAL_ROOT_DEFAULT "${EXTERNAL_INCLUDE_DIR_TMP}/.." )
          ENDIF( EXISTS ${EXTERNAL_INCLUDE_DIR_TMP}/../include/pthread )
        endforeach( EXTERNAL_INCLUDE_DIR_TMP ${EXTERNAL_INCLUDE_DIR} )
      ELSE( H3D_USE_DEPENDENCIES_ONLY )
        SET( H3DAPI_CPACK_EXTERNAL_ROOT_DEFAULT "$ENV{H3D_EXTERNAL_ROOT}" )
      ENDIF( H3D_USE_DEPENDENCIES_ONLY )
      SET( H3DAPI_CPACK_EXTERNAL_ROOT "${H3DAPI_CPACK_EXTERNAL_ROOT_DEFAULT}" CACHE PATH "Set to the External directory used with H3DAPI, needed to pack properly. If not set FIND_modules will be used instead." )
      MARK_AS_ADVANCED(H3DAPI_CPACK_EXTERNAL_ROOT)
    ENDIF( NOT DEFINED H3DAPI_CPACK_EXTERNAL_ROOT )

    IF( TARGET OpenHapticsRenderer )
      SET( OPENHAPTICS_FOUND TRUE )
    ENDIF( TARGET OpenHapticsRenderer )
    IF( TARGET Chai3DRenderer )
      SET( CHAI3D_FOUND TRUE )
    ENDIF( TARGET Chai3DRenderer )
    include( ${H3DAPI_SOURCE_DIR}/../../HAPI/build/HAPICPack.cmake )
  ENDIF( WIN32 )


  # Set information properties about the project to install.
  set( CPACK_ALL_INSTALL_TYPES Full Developer )
  SET(CMAKE_MODULE_PATH ${H3DAPI_SOURCE_DIR}/modules )
  SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "H3DAPI. A cross platform, haptics device independent, X3D based API for 3D graphics and haptics.")
  SET(CPACK_PACKAGE_VENDOR "SenseGraphics AB")
  SET(CPACK_PACKAGE_CONTACT "support@sensegraphics.com" )
  SET(CPACK_PACKAGE_DESCRIPTION_FILE "${H3DAPI_SOURCE_DIR}/../ReadMe")
  SET(CPACK_RESOURCE_FILE_LICENSE "${H3DAPI_SOURCE_DIR}/../LICENSE")

  # Project to install.
  SET(CPACK_INSTALL_CMAKE_PROJECTS "${CMAKE_CURRENT_BINARY_DIR};H3DAPI;ALL;/")

  # Installation directory for package.
  SET(CPACK_PACKAGE_INSTALL_DIRECTORY "H3D" )

  # Our project depends on these debian packages for Linux.
  SET(DEBIAN_PACKAGE_DEPENDS "libcurl3, libfreetype6-dev, ftgl-dev, python2.4-dev, libaudiofile0, libvorbis-dev, libopenal-dev, zlib1g-dev, libxerces27-dev, libfreeimage-dev, hapi(>= 1.0.0), h3dutil(>=1.0.0)" )
  
  # File patterns to ignore, common for all operating systems.
  SET( H3DAPI_CPACK_IGNORE_PATTERNS /\\\\.svn/
									\\\\.obj$
									\\\\.ncb$
									\\\\.log$
									\\\\.suo$
									\\\\.dir/
									\\\\.user$
									\\\\.cv$
									"/Debug(.)*/"
									"/debug(.)*/"
									/Release
									/release
									/linux
									/build/win32/
									"/((C\\\\+\\\\+|ThreadExample)|(build|examples))/vc(7|(8|9))"
									/osx
									/H3DAPI/Util/
									/berk/berk\\\\.wrl$
									/berk/berk_orig\\\\.x3d$
									/fish/Kumanomi\\\\.wrl$
									/fish/Kumanomi_orig\\\\.x3d$
									/humvee/humvee\\\\.WRL$
									/humvee/humvee_orig\\\\.x3d$
									/manikin/manikin\\\\.wrl$
									/manikin/manikin_orig\\\\.x3d$
									/moondial/moondial_orig\\\\.x3d$
									/moondial/themoondial\\\\.wrl$
									/plane/bobcat2\\\\.x3d$
									/plane/bobcat_nh\\\\.x3d$
									/plane/bobcat_orig\\\\.x3d$ )

  SET(CPACK_PACKAGE_VERSION_MAJOR ${H3DAPI_MAJOR_VERSION})
  SET(CPACK_PACKAGE_VERSION_MINOR ${H3DAPI_MINOR_VERSION})
  SET(CPACK_PACKAGE_VERSION_PATCH ${H3DAPI_BUILD_VERSION})
  
  # TODO: fix different for windows and unix if needed.
  IF(WIN32 AND NOT UNIX)
	SET(CPACK_NSIS_INSTALL_ROOT "C:" )
	SET(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL "ON" )
	SET( CPACK_PACKAGE_START_MENU_NAME "H3DAPI ${H3DAPI_MAJOR_VERSION}.${H3DAPI_MINOR_VERSION}" )
    
    # External binary directory to add to path.
    SET( CPACK_EXTERNAL_BIN "bin32" )
    SET( CPACK_H3D_64_BIT "FALSE" )
    SET( EXTERNAL_BIN_REPLACE_PATH "bin64" )
    SET( CPACK_NSIS_EXECUTABLES_DIRECTORY bin32 )
    IF( CMAKE_SIZEOF_VOID_P EQUAL 8 )
      SET( CPACK_EXTERNAL_BIN "bin64" )
      SET( EXTERNAL_BIN_REPLACE_PATH "bin32" )
      SET( CPACK_H3D_64_BIT "TRUE" )
      SET( CPACK_NSIS_EXECUTABLES_DIRECTORY bin64 )
    ENDIF( CMAKE_SIZEOF_VOID_P EQUAL 8 )
    SET( EXTERNAL_BIN_PATH "${CPACK_EXTERNAL_BIN}" )

    # EXTERNAL_INCLUDES and EXTERNAL_INCLUDE_INSTALL_PATHS must be of equal lengths.
    # The reason for defining these variables here is in case we want to add functionality
    # to configure installation in some other way (using FIND-modules for example).
    SET( EXTERNAL_INCLUDES "" )
    SET( EXTERNAL_INCLUDE_INSTALL_PATHS "" )
    # The EXTERNAL_INCLUDES_FILES are installed directly in External/include
    SET( EXTERNAL_INCLUDES_FILES "" )
    SET( EXTERNAL_LIBRARIES "" )
    SET( EXTERNAL_STATIC_LIBRARIES "" )
    SET( EXTERNAL_BINARIES "" )
    
    IF( EXISTS ${H3DAPI_CPACK_EXTERNAL_ROOT} )
      SET( EXTERNAL_INCLUDES ${H3DAPI_CPACK_EXTERNAL_ROOT}/include/xercesc/
                             ${H3DAPI_CPACK_EXTERNAL_ROOT}/include/curl/
                             ${H3DAPI_CPACK_EXTERNAL_ROOT}/include/Cg/
                             ${H3DAPI_CPACK_EXTERNAL_ROOT}/include/AL/
                             ${H3DAPI_CPACK_EXTERNAL_ROOT}/include/vorbis/
                             ${H3DAPI_CPACK_EXTERNAL_ROOT}/include/ogg/
                             ${H3DAPI_CPACK_EXTERNAL_ROOT}/include/libaudiofile/
                             ${H3DAPI_CPACK_EXTERNAL_ROOT}/include/freetype/
                             ${H3DAPI_CPACK_EXTERNAL_ROOT}/include/3dconnexion/
                             ${H3DAPI_CPACK_EXTERNAL_ROOT}/include/FTGL/
                             ${H3DAPI_CPACK_EXTERNAL_ROOT}/include/DirectShow/
                             ${H3DAPI_CPACK_EXTERNAL_ROOT}/include/js/ )
      SET( EXTERNAL_INCLUDE_INSTALL_PATHS External/include/xercesc
                                          External/include/curl
                                          External/include/Cg
                                          External/include/AL
                                          External/include/vorbis
                                          External/include/ogg
                                          External/include/libaudiofile
                                          External/include/freetype
                                          External/include/3dconnexion
                                          External/include/FTGL
                                          External/include/DirectShow
                                          External/include/js )

      SET( EXTERNAL_INCLUDES_FILES ${H3DAPI_CPACK_EXTERNAL_ROOT}/include/.h )

      SET( EXTERNAL_LIBRARIES ${H3DAPI_CPACK_EXTERNAL_ROOT}/lib32/glew32.lib
                              ${H3DAPI_CPACK_EXTERNAL_ROOT}/lib32/xerces-c_3.lib
                              ${H3DAPI_CPACK_EXTERNAL_ROOT}/lib32/xerces-c_3D.lib
                              ${H3DAPI_CPACK_EXTERNAL_ROOT}/lib32/libcurl.lib
                              ${H3DAPI_CPACK_EXTERNAL_ROOT}/lib32/cg.lib
                              ${H3DAPI_CPACK_EXTERNAL_ROOT}/lib32/cgGL.lib
                              ${H3DAPI_CPACK_EXTERNAL_ROOT}/lib32/OpenAL32.lib
                              ${H3DAPI_CPACK_EXTERNAL_ROOT}/lib32/libvorbisfile.lib
                              ${H3DAPI_CPACK_EXTERNAL_ROOT}/lib32/libvorbis.lib
                              ${H3DAPI_CPACK_EXTERNAL_ROOT}/lib32/libogg.lib
                              ${H3DAPI_CPACK_EXTERNAL_ROOT}/lib32/audiofile.lib
                              ${H3DAPI_CPACK_EXTERNAL_ROOT}/lib32/siapp.lib
                              ${H3DAPI_CPACK_EXTERNAL_ROOT}/lib32/spwmath.lib
                              ${H3DAPI_CPACK_EXTERNAL_ROOT}/lib32/ftgl.lib
                              ${H3DAPI_CPACK_EXTERNAL_ROOT}/lib32/js32.lib )      

      SET( EXTERNAL_STATIC_LIBRARIES ${EXTERNAL_STATIC_LIBRARIES}
                                     #${H3DAPI_CPACK_EXTERNAL_ROOT}/lib32/static/glew32s.lib
                                     #${H3DAPI_CPACK_EXTERNAL_ROOT}/lib32/static/libcurl.lib
                                     #${H3DAPI_CPACK_EXTERNAL_ROOT}/lib32/static/libvorbisfile_static.lib
                                     #${H3DAPI_CPACK_EXTERNAL_ROOT}/lib32/static/libvorbis_static.lib
                                     #${H3DAPI_CPACK_EXTERNAL_ROOT}/lib32/static/libogg_static.lib
                                     ${H3DAPI_CPACK_EXTERNAL_ROOT}/lib32/static/freetype2312.lib
                                     ${H3DAPI_CPACK_EXTERNAL_ROOT}/lib32/static/freetype2312MT.lib
                                     #${H3DAPI_CPACK_EXTERNAL_ROOT}/lib32/static/ftgl_static.lib
                                     ${H3DAPI_CPACK_EXTERNAL_ROOT}/lib32/static/strmbase.lib )
     
      SET( EXTERNAL_BINARIES ${EXTERNAL_BINARIES}
                             ${H3DAPI_CPACK_EXTERNAL_ROOT}/${EXTERNAL_BIN_PATH}/glew32.dll
                             ${H3DAPI_CPACK_EXTERNAL_ROOT}/${EXTERNAL_BIN_PATH}/xerces-c_3_1.dll
                             ${H3DAPI_CPACK_EXTERNAL_ROOT}/${EXTERNAL_BIN_PATH}/xerces-c_3_1D.dll 
                             ${H3DAPI_CPACK_EXTERNAL_ROOT}/${EXTERNAL_BIN_PATH}/libcurl.dll
                             ${H3DAPI_CPACK_EXTERNAL_ROOT}/${EXTERNAL_BIN_PATH}/cg.dll
                             ${H3DAPI_CPACK_EXTERNAL_ROOT}/${EXTERNAL_BIN_PATH}/cgGL.dll
                             ${H3DAPI_CPACK_EXTERNAL_ROOT}/${EXTERNAL_BIN_PATH}/libvorbisfile.dll
                             ${H3DAPI_CPACK_EXTERNAL_ROOT}/${EXTERNAL_BIN_PATH}/libvorbis.dll
                             ${H3DAPI_CPACK_EXTERNAL_ROOT}/${EXTERNAL_BIN_PATH}/libogg.dll
                             ${H3DAPI_CPACK_EXTERNAL_ROOT}/${EXTERNAL_BIN_PATH}/audiofile.dll
                             ${H3DAPI_CPACK_EXTERNAL_ROOT}/${EXTERNAL_BIN_PATH}/ftgl.dll
                             ${H3DAPI_CPACK_EXTERNAL_ROOT}/${EXTERNAL_BIN_PATH}/js32.dll )

    ELSE( EXISTS ${H3DAPI_CPACK_EXTERNAL_ROOT} )
      MESSAGE( WARNING "H3DAPI_CPACK_EXTERNAL_ROOT must be set to the External directory used by H3DAPI in order to package properly." )
    ENDIF( EXISTS ${H3DAPI_CPACK_EXTERNAL_ROOT} )
    
    IF( EXTERNAL_INCLUDES )
      list( LENGTH EXTERNAL_INCLUDES EXTERNAL_INCLUDES_LENGTH )
      math( EXPR EXTERNAL_INCLUDES_LENGTH "${EXTERNAL_INCLUDES_LENGTH} - 1" )
      foreach( val RANGE ${EXTERNAL_INCLUDES_LENGTH} )
        list(GET EXTERNAL_INCLUDES ${val} val1)
        list(GET EXTERNAL_INCLUDE_INSTALL_PATHS ${val} val2)
        INSTALL( DIRECTORY ${val1}
                 DESTINATION ${val2}
                 COMPONENT H3DAPI_cpack_external_source
                 REGEX "(/.svn)|(/CVS)" EXCLUDE )
      endforeach( val )
    ENDIF( EXTERNAL_INCLUDES )
    
    foreach( include_file ${EXTERNAL_INCLUDES_FILES} )
      IF( EXISTS ${include_file} )
        INSTALL( FILES ${include_file}
                 DESTINATION External/include
                 COMPONENT H3DAPI_cpack_external_source )
      ENDIF( EXISTS ${include_file} )
    endforeach( include_file )
    
    foreach( library ${EXTERNAL_LIBRARIES} )
      IF( EXISTS ${library} )
        INSTALL( FILES ${library}
                 DESTINATION External/lib32
                 COMPONENT H3DAPI_cpack_external_source )
      ENDIF( EXISTS ${library} )
      # Add the other library path as well
      STRING( REGEX REPLACE "(/lib32/)" "/lib64/" other_library ${library} )
      IF( EXISTS ${other_library} )
        INSTALL( FILES ${other_library}
                 DESTINATION External/lib64
                 COMPONENT H3DAPI_cpack_external_source )
      ENDIF( EXISTS ${other_library} )
    endforeach( library )
    
    foreach( library ${EXTERNAL_STATIC_LIBRARIES} )
      IF( EXISTS ${library} )
        INSTALL( FILES ${library}
                 DESTINATION External/lib32/static
                 COMPONENT H3DAPI_cpack_external_source )
      ENDIF( EXISTS ${library} )
      # Add the other library path as well
      STRING( REGEX REPLACE "(/lib32/)" "/lib64/" other_library ${library} )
      IF( EXISTS ${other_library} )
        INSTALL( FILES ${other_library}
                 DESTINATION External/lib64/static
                 COMPONENT H3DAPI_cpack_external_source )
      ENDIF( EXISTS ${other_library} )
    endforeach( library )
    
    foreach( binary ${EXTERNAL_BINARIES} )
      IF( EXISTS ${binary} )
        INSTALL( FILES ${binary}
                 DESTINATION External/${EXTERNAL_BIN_PATH}
                 COMPONENT H3DAPI_cpack_external_runtime )
      ENDIF( EXISTS ${binary} )

      STRING( REGEX REPLACE "(/${EXTERNAL_BIN_PATH}/)" "/${EXTERNAL_BIN_REPLACE_PATH}/" other_binary ${binary} )
      IF( EXISTS ${other_binary} )
        INSTALL( FILES ${other_binary}
                 DESTINATION External/${EXTERNAL_BIN_REPLACE_PATH}
                 COMPONENT H3DAPI_cpack_external_runtime )
      ENDIF( EXISTS ${other_binary} )
    endforeach( binary )    
    
    # setting names and dependencies between components and also grouping them.
    set(CPACK_COMPONENT_H3DAPI_CPACK_EXTERNAL_RUNTIME_DISPLAY_NAME "External runtime")
    set(CPACK_COMPONENT_H3DAPI_CPACK_EXTERNAL_RUNTIME_DESCRIPTION "External runtime binaries needed by H3DAPI.")
    set(CPACK_COMPONENT_H3DAPI_CPACK_EXTERNAL_RUNTIME_DEPENDS HAPI_cpack_external_runtime )
    set(CPACK_COMPONENT_H3DAPI_CPACK_EXTERNAL_RUNTIME_GROUP "H3DAPI_cpack_group")
    set(CPACK_COMPONENT_H3DAPI_CPACK_EXTERNAL_RUNTIME_INSTALL_TYPES Developer Full)
    
    set(CPACK_COMPONENT_H3DAPI_CPACK_EXTERNAL_SOURCE_DISPLAY_NAME "External header/libraries")
    set(CPACK_COMPONENT_H3DAPI_CPACK_EXTERNAL_SOURCE_DESCRIPTION "External headers and libraries needed by H3DAPI.")
    set(CPACK_COMPONENT_H3DAPI_CPACK_EXTERNAL_SOURCE_DEPENDS HAPI_cpack_external_source H3DAPI_cpack_external_runtime )
    set(CPACK_COMPONENT_H3DAPI_CPACK_EXTERNAL_SOURCE_GROUP "H3DAPI_cpack_group")
    set(CPACK_COMPONENT_H3DAPI_CPACK_EXTERNAL_SOURCE_INSTALL_TYPES Developer Full)

    SET( H3DAPI_CPACK_INSTALLED_DIRECTORIES "" )
    
    IF( H3DAPI_LOADER_PROJECTS )
      SET( CPACK_ADD_H3DLOAD_DEMOS_LINKS "ON" )
    ENDIF( H3DAPI_LOADER_PROJECTS )

    # Create cached variable for getting the plugin folder.
    SET( H3DVIEWER_PLUGIN_FOLDER "" CACHE PATH "Path to folder containing plugins for H3DViewer." )
    MARK_AS_ADVANCED(H3DVIEWER_PLUGIN_FOLDER)
    IF( H3DVIEWER_PLUGIN_FOLDER )
      # Create cached variable for getting the VHTK examples folder.
      SET( VHTK_EXAMPLES_FOLDER "" CACHE PATH "Path to folder containing VHTK examples." )
      MARK_AS_ADVANCED(VHTK_EXAMPLES_FOLDER)
      IF( VHTK_EXAMPLES_FOLDER )
        SET( H3DAPI_CPACK_INSTALLED_DIRECTORIES ${H3DAPI_CPACK_INSTALLED_DIRECTORIES}
                                                "${VHTK_EXAMPLES_FOLDER};H3DAPI/examples/VHTK" )
        SET( CPACK_ADD_VHTK_DEMOS_LINKS "ON" )
      ELSE( VHTK_EXAMPLES_FOLDER )
        SET( CPACK_ADD_VHTK_DEMOS_LINKS "OFF" )
      ENDIF( VHTK_EXAMPLES_FOLDER )
    ENDIF( H3DVIEWER_PLUGIN_FOLDER )

    #Extra links to start menu if values are "ON"
    SET( CPACK_ADD_H3DDOC_LINKS "ON" )
    SET( CPACK_ADD_H3DSETTINGS_LINKS "ON" )
    
    # Extra install commands will be set to install python and OpenAL
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
                                              " Check if H3DAPI selected for installation\\n IntOp $0 $H3DAPI_cpack_sources_selected | $H3DAPI_cpack_runtime_selected\\n"
                                              " Check if H3DAPI selected for installation\\n \\\${If} $0 > 0\\n"
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
                                             "A comment \\n  install_python_no:\\n"
                                             "A comment \\n \\\${EndIf}\\n" )
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
	
	SET( CMakeInstallExe "" CACHE FILEPATH "Needs to be set to add CMake installation to the package." )
    MARK_AS_ADVANCED(CMakeInstallExe)
    IF( CMakeInstallExe )
      SET( CMAKE_VERSION_TO_CHECK "" )
      STRING( REGEX MATCH [0123456789]+\\.[0123456789]+\\.[0123456789]+\\.[0123456789]+ CMAKE_VERSION_TO_CHECK ${CMakeInstallExe} )
      GET_FILENAME_COMPONENT( CMAKE_INSTALL_EXE_FILE_NAME ${CMakeInstallExe} NAME )
      STRING( REPLACE "/" "\\\\" TEMP_CMakeInstallExe ${CMakeInstallExe} )
      SET( CMAKE_INSTALL_EXE_INSTALL_COMMAND_2 " Extract CMake installer\\n  File \\\"${TEMP_CMakeInstallExe}\\\"\\n" )
      SET( CMAKE_INSTALL_EXE_INSTALL_COMMAND_3 " Wait a bit for system to unlock file.\\n  Sleep 1000\\n"
                                       " Delete CMake installer\\n  Delete \\\"$INSTDIR\\\\${CMAKE_INSTALL_EXE_FILE_NAME}\\\"\\n\\n" )
      
      # could not find a way to uninstall using the exe command line, no idea how to do that yet.
      #SET( CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS ${CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS}
      #                                         " Code to install CMake\\n  ReadRegStr $0 HKLM \\\"SOFTWARE\\\\Kitware\\\\CMake ${CMAKE_VERSION_TO_CHECK}\\\" \\\"\\\"\\n"
      #                                         " Check if CMake is installed\\n  StrCmp $0 \\\"\\\" uninstall_cmake_no 0\\n"
      #                                         " Check if uninstall CMake \\n  MessageBox MB_YESNO \\\"Do you want to uninstall CMake? It is recommended if no other builds on your system use CMake for configuration.\\\" IDYES uninstall_cmake_yes IDNO uninstall_cmake_no\\n"
      #                                         " A comment \\n  uninstall_cmake_yes:\\n"
      #                                         ${CMAKE_INSTALL_EXE_INSTALL_COMMAND_2}
      #                                         " Execute CMake installer, wait for completion\\n  ExecWait '\\\"$INSTDIR\\\\${CMAKE_INSTALL_EXE_FILE_NAME}\\\" /S'\\n"
      #                                         ${CMAKE_INSTALL_EXE_INSTALL_COMMAND_3}
      #                                         " A comment \\n  uninstall_cmake_no:\\n" )
      SET( CPACK_NSIS_EXTRA_INSTALL_COMMANDS ${CPACK_NSIS_EXTRA_INSTALL_COMMANDS}
                                             " Check if any sources selected for installation\\n IntOp $0 $H3DUtil_cpack_sources_selected | $HAPI_cpack_sources_selected\\n"
                                             " Because of dependencies check only H3DUtil and HAPI sources\\n \\\${If} $0 > 0\\n"
                                             " CMake major version\\n  StrCpy \\\$R0 \\\"-1\\\"\\n"
                                             " A comment\\n  trypreviouscmakeversion:\\n"
                                             " CMake reg key\\n  IntOp \\\$R0 \\\$R0 + 1\\n"
                                             " A comment\\n  ClearErrors\\n"
                                             " Read registry\\n  EnumRegKey $R1 SHCTX \\\"Software\\\\Kitware\\\" \\\"\\\$R0\\\"\\n"
                                             " Check if string found\\n  IfErrors install_cmake 0\\n"
                                             " Check if string empty\\n  StrCmp $R1 \\\"\\\" install_cmake 0\\n"
                                             " Copy first five\\n  StrCpy $R1 $R1 5\\n"
                                             " Check if string starts with CMake\\n  StrCmp $R1 \\\"CMake\\\" install_cmake_no trypreviouscmakeversion\\n"
                                             " A comment \\n install_cmake:\\n"
                                             ${CMAKE_INSTALL_EXE_INSTALL_COMMAND_2}
                                             "A comment \\n    ClearErrors\\n"
                                             " Execute cmake installer silent, wait for completion\\n  ExecWait '\\\"$INSTDIR\\\\${CMAKE_INSTALL_EXE_FILE_NAME}\\\" /S'\\n"
                                             "A comment \\n Goto cmake_end_install\\n"
                                             "A comment \\n cmake_install_not_hidden:\\n"
                                             " Execute cmake installer, wait for completion\\n  ExecWait '\\\"$INSTDIR\\\\${CMAKE_INSTALL_EXE_FILE_NAME}\\\"'\\n"
                                             " A comment \\n  cmake_end_install:\\n"
                                             ${CMAKE_INSTALL_EXE_INSTALL_COMMAND_3}
                                             "A comment \\n  install_cmake_no:\\n"
                                             "A comment \\n \\\${EndIf}\\n" )
    ENDIF( CMakeInstallExe )

    # Modify path in the the NSIS template.
    SET( CPACK_NSIS_MODIFY_PATH "ON" )
  ELSE(WIN32 AND NOT UNIX)
    SET(H3DAPI_CPACK_IGNORE_PATTERNS ${H3DAPI_CPACK_IGNORE_PATTERNS} /examples/Particledemo/
                                                                     "~$" )
    SET( H3DAPI_CPACK_IGNORE_PATTERNS ${H3DAPI_CPACK_IGNORE_PATTERNS}
				    "/CVS/;/.svn/;/.bzr/;/.hg/;/.git.*/;.swp$;.#;/#;~$")
    SET(CPACK_SOURCE_GENERATOR TGZ ZIP ) 
    SET(CPACK_SOURCE_PACKAGE_FILE_NAME "h3dapi-${H3DAPI_MAJOR_VERSION}.${H3DAPI_MINOR_VERSION}.${H3DAPI_BUILD_VERSION}") 
    SET(CPACK_SOURCE_IGNORE_FILES ${H3DAPI_CPACK_IGNORE_PATTERNS} )
    SET(CPACK_SOURCE_INSTALLED_DIRECTORIES "${H3DAPI_SOURCE_DIR}/..;/" )
  ENDIF(WIN32 AND NOT UNIX)
  
  # Install header files
  INSTALL( FILES ${H3DAPI_HEADERS} ${H3DAPI_SOURCE_DIR}/../include/H3D/H3DApi.cmake
           DESTINATION H3DAPI/include/H3D
           COMPONENT H3DAPI_cpack_headers )

  # H3DApi.cmake that goes to headers is not needed unless sources is required.
  INSTALL( FILES ${H3DAPI_SOURCE_DIR}/../include/H3D/H3DApi.cmake
			DESTINATION H3DAPI/include/H3D
			COMPONENT H3DAPI_cpack_sources )
  
  # Install src files.
  INSTALL( FILES ${H3DAPI_SRCS}
           DESTINATION H3DAPI/src
           COMPONENT H3DAPI_cpack_sources )

  INSTALL( FILES ${H3DAPI_SOURCE_DIR}/../changelog
                 ${H3DAPI_SOURCE_DIR}/../LICENSE
                 ${H3DAPI_SOURCE_DIR}/../ReadMe
           DESTINATION H3DAPI
           COMPONENT H3DAPI_cpack_sources )

  INSTALL( FILES ${H3DAPI_SOURCE_DIR}/CMakeLists.txt
                 ${H3DAPI_SOURCE_DIR}/H3DAPI.rc.cmake
                 ${H3DAPI_SOURCE_DIR}/H3DAPICPack.cmake
                 ${H3DAPI_SOURCE_DIR}/H3DAPISourceFiles.txt
                 ${H3DAPI_SOURCE_DIR}/UnityBuild.cmake
                 ${H3DAPI_SOURCE_DIR}/UpdateResourceFile.exe
           DESTINATION H3DAPI/build
           COMPONENT H3DAPI_cpack_sources )

  INSTALL( FILES ${H3DAPI_SOURCE_DIR}/modules/Find3DXWARE.cmake
                 ${H3DAPI_SOURCE_DIR}/modules/FindAudiofile.cmake
                 ${H3DAPI_SOURCE_DIR}/modules/FindChai3D.cmake
                 ${H3DAPI_SOURCE_DIR}/modules/FindDCMTK.cmake
                 ${H3DAPI_SOURCE_DIR}/modules/FindDirectShow.cmake
                 ${H3DAPI_SOURCE_DIR}/modules/FindDirectX.cmake
                 ${H3DAPI_SOURCE_DIR}/modules/FindFFmpeg.cmake
                 ${H3DAPI_SOURCE_DIR}/modules/FindFontConfig.cmake
                 ${H3DAPI_SOURCE_DIR}/modules/FindFreeImage.cmake
                 ${H3DAPI_SOURCE_DIR}/modules/FindFreeType.cmake
                 ${H3DAPI_SOURCE_DIR}/modules/FindFTGL.cmake
                 ${H3DAPI_SOURCE_DIR}/modules/FindGLEW.cmake
                 ${H3DAPI_SOURCE_DIR}/modules/FindGLUTWin.cmake
                 ${H3DAPI_SOURCE_DIR}/modules/FindH3DAPI.cmake
                 ${H3DAPI_SOURCE_DIR}/modules/FindH3DCURL.cmake
                 ${H3DAPI_SOURCE_DIR}/modules/FindH3DOpenAL.cmake
                 ${H3DAPI_SOURCE_DIR}/modules/FindH3DUtil.cmake
                 ${H3DAPI_SOURCE_DIR}/modules/FindH3DZLIB.cmake
                 ${H3DAPI_SOURCE_DIR}/modules/FindHAPI.cmake
                 ${H3DAPI_SOURCE_DIR}/modules/FindMd5sum.cmake
                 ${H3DAPI_SOURCE_DIR}/modules/FindNvidiaCG.cmake
                 ${H3DAPI_SOURCE_DIR}/modules/FindOpenHaptics.cmake
                 ${H3DAPI_SOURCE_DIR}/modules/FindPTHREAD.cmake
                 ${H3DAPI_SOURCE_DIR}/modules/FindSpiderMonkey.cmake
                 ${H3DAPI_SOURCE_DIR}/modules/FindH3DTEEM.cmake
                 ${H3DAPI_SOURCE_DIR}/modules/FindV8.cmake
                 ${H3DAPI_SOURCE_DIR}/modules/FindVirtualHand.cmake
                 ${H3DAPI_SOURCE_DIR}/modules/FindVorbis.cmake
                 ${H3DAPI_SOURCE_DIR}/modules/FindWxWidgetsWin.cmake
                 ${H3DAPI_SOURCE_DIR}/modules/FindXerces.cmake
                 ${H3DAPI_SOURCE_DIR}/modules/NSIS.InstallOptions.ini.in
                 ${H3DAPI_SOURCE_DIR}/modules/NSIS.template.in
                 ${H3DAPI_SOURCE_DIR}/modules/StripAndAddLibraryDirectories.cmake
                 ${H3DAPI_SOURCE_DIR}/modules/TestIfVCExpress.cmake
                 ${H3DAPI_SOURCE_DIR}/modules/UseDebian.cmake
           DESTINATION H3DAPI/build/modules
           COMPONENT H3DAPI_cpack_sources )

  INSTALL( DIRECTORY ${H3DAPI_SOURCE_DIR}/../Conformance
           DESTINATION H3DAPI
           COMPONENT H3DAPI_cpack_sources
           REGEX "(/.svn)|(/CVS)" EXCLUDE )

  INSTALL( DIRECTORY ${H3DAPI_SOURCE_DIR}/../examples
           DESTINATION H3DAPI
           COMPONENT H3DAPI_cpack_sources
           REGEX "(/.svn)|(/CVS)|(/Particledemo)" EXCLUDE
           PATTERN "berk.wrl" EXCLUDE
					 PATTERN "berk_orig.x3d" EXCLUDE
					 PATTERN "Kumanomi.wrl" EXCLUDE
					 PATTERN "Kumanomi_orig.x3d" EXCLUDE
					 PATTERN "humvee.WRL" EXCLUDE
					 PATTERN "humvee_orig.x3d" EXCLUDE
					 PATTERN "manikin.wrl" EXCLUDE
					 PATTERN "manikin_orig.x3d" EXCLUDE
					 PATTERN "moondial_orig.x3d" EXCLUDE
					 PATTERN "themoondial.wrl" EXCLUDE
					 PATTERN "bobcat2.x3d" EXCLUDE
					 PATTERN "bobcat_nh.x3d" EXCLUDE
					 PATTERN "bobcat_orig.x3d" EXCLUDE )

  INSTALL( DIRECTORY ${H3DAPI_SOURCE_DIR}/../H3DLoad
           DESTINATION H3DAPI
           COMPONENT H3DAPI_cpack_sources
           REGEX "(/.svn)|(/CVS)" EXCLUDE )

  INSTALL( DIRECTORY ${H3DAPI_SOURCE_DIR}/../H3DViewer
           DESTINATION H3DAPI
           COMPONENT H3DAPI_cpack_sources
           REGEX "(/.svn)|(/CVS)" EXCLUDE )

  IF( EXISTS ${H3DAPI_SOURCE_DIR}/../lib/doc )
    INSTALL( DIRECTORY ${H3DAPI_SOURCE_DIR}/../lib/doc
           DESTINATION H3DAPI/lib
           COMPONENT H3DAPI_cpack_headers
           REGEX "(/.svn)|(/CVS)" EXCLUDE )
  ENDIF( EXISTS ${H3DAPI_SOURCE_DIR}/../lib/doc )

  INSTALL( FILES ${H3DAPI_SOURCE_DIR}/../lib/H3DInterface.py
                 ${H3DAPI_SOURCE_DIR}/../lib/H3DUtils.py
           DESTINATION H3DAPI/lib
           COMPONENT H3DAPI_cpack_sources )

  INSTALL( DIRECTORY ${H3DAPI_SOURCE_DIR}/../settings/common
           DESTINATION H3DAPI/settings
           COMPONENT H3DAPI_cpack_runtime
           REGEX "(/.svn)|(/CVS)" EXCLUDE )

  INSTALL( DIRECTORY ${H3DAPI_SOURCE_DIR}/../settings/current
           DESTINATION H3DAPI/settings
           COMPONENT H3DAPI_cpack_runtime
           REGEX "(/.svn)|(/CVS)" EXCLUDE )

  INSTALL( DIRECTORY ${H3DAPI_SOURCE_DIR}/../settings/display
           DESTINATION H3DAPI/settings
           COMPONENT H3DAPI_cpack_runtime
           REGEX "(/.svn)|(/CVS)" EXCLUDE )

  INSTALL( FILES ${H3DAPI_SOURCE_DIR}/../settings/h3dload.ini
           DESTINATION H3DAPI/settings
           COMPONENT H3DAPI_cpack_runtime )

  INSTALL( FILES ${H3DAPI_SOURCE_DIR}/../settings/dist.py
                 ${H3DAPI_SOURCE_DIR}/../settings/SettingsGUI.py
           DESTINATION H3DAPI/settings
           COMPONENT H3DAPI_cpack_sources )

  IF( EXISTS ${H3DAPI_SOURCE_DIR}/../settings/dist )
    INSTALL( DIRECTORY ${H3DAPI_SOURCE_DIR}/../settings/dist/
           DESTINATION H3DAPI/settings
           COMPONENT H3DAPI_cpack_runtime
           REGEX "(/.svn)|(/CVS)" EXCLUDE )
  ENDIF( EXISTS ${H3DAPI_SOURCE_DIR}/../settings/dist )
  
  IF( EXISTS ${H3DAPI_SOURCE_DIR}/../settings/icons )
    INSTALL( DIRECTORY ${H3DAPI_SOURCE_DIR}/../settings/icons
           DESTINATION H3DAPI/settings
           COMPONENT H3DAPI_cpack_runtime
           REGEX "(/.svn)|(/CVS)" EXCLUDE )
  ENDIF( EXISTS ${H3DAPI_SOURCE_DIR}/../settings/icons )

  # Add a cache variable H3DAPI_DOCS_DIRECTORY used to indicate where the H3DAPI docs are.
  IF( NOT DEFINED H3DAPI_DOCS_DIRECTORY )
    SET( H3DAPI_DOCS_DIRECTORY_DEFAULT "" )
    IF( H3D_USE_DEPENDENCIES_ONLY )
      SET( H3DAPI_DOCS_DIRECTORY_DEFAULT "${H3DAPI_SOURCE_DIR}/../../doc" )
    ENDIF( H3D_USE_DEPENDENCIES_ONLY )
    SET( H3DAPI_DOCS_DIRECTORY "${H3DAPI_DOCS_DIRECTORY_DEFAULT}" CACHE PATH "Set this to the directory containing the documentation of H3DAPI." )
    MARK_AS_ADVANCED(H3DAPI_DOCS_DIRECTORY)
  ENDIF( NOT DEFINED H3DAPI_DOCS_DIRECTORY )
  
  IF( EXISTS ${H3DAPI_DOCS_DIRECTORY} )
    INSTALL( DIRECTORY ${H3DAPI_DOCS_DIRECTORY}/H3DAPI
             DESTINATION doc
             COMPONENT H3DAPI_cpack_headers
             REGEX "(/.svn)|(/CVS)" EXCLUDE )
    INSTALL( FILES "${H3DAPI_DOCS_DIRECTORY}/H3D API Manual.pdf"
             DESTINATION doc
             COMPONENT H3DAPI_cpack_headers )
  ENDIF( EXISTS ${H3DAPI_DOCS_DIRECTORY} )
  
  # setting names and dependencies between components and also grouping them.
  set(CPACK_COMPONENT_H3DAPI_CPACK_RUNTIME_DISPLAY_NAME "Runtime")
  set(CPACK_COMPONENT_H3DAPI_CPACK_RUNTIME_DESCRIPTION "The runtime libraries (dlls) for H3DAPI.")
  set(CPACK_COMPONENT_H3DAPI_CPACK_RUNTIME_DEPENDS HAPI_cpack_runtime H3DAPI_cpack_external_runtime )
  set(CPACK_COMPONENT_H3DAPI_CPACK_RUNTIME_GROUP "H3DAPI_cpack_group")
  set(CPACK_COMPONENT_H3DAPI_CPACK_RUNTIME_INSTALL_TYPES Developer Full)
    
  set(CPACK_COMPONENT_H3DAPI_CPACK_LIBRARIES_DISPLAY_NAME "Libraries")
  set(CPACK_COMPONENT_H3DAPI_CPACK_LIBRARIES_DESCRIPTION "H3DAPI libraries, needed for building against H3DAPI.")
  set(CPACK_COMPONENT_H3DAPI_CPACK_LIBRARIES_DEPENDS HAPI_cpack_libraries H3DAPI_cpack_external_source H3DAPI_cpack_headers )
  set(CPACK_COMPONENT_H3DAPI_CPACK_LIBRARIES_GROUP "H3DAPI_cpack_group")
  set(CPACK_COMPONENT_H3DAPI_CPACK_LIBRARIES_INSTALL_TYPES Developer Full)
    
  set(CPACK_COMPONENT_H3DAPI_CPACK_HEADERS_DISPLAY_NAME "C++ Headers")
  set(CPACK_COMPONENT_H3DAPI_CPACK_HEADERS_DESCRIPTION "H3DAPI C++ headers, needed for building against H3DAPI.")
  set(CPACK_COMPONENT_H3DAPI_CPACK_HEADERS_DEPENDS HAPI_cpack_headers H3DAPI_cpack_external_source H3DAPI_cpack_libraries )
  set(CPACK_COMPONENT_H3DAPI_CPACK_HEADERS_GROUP "H3DAPI_cpack_group")
  set(CPACK_COMPONENT_H3DAPI_CPACK_HEADERS_INSTALL_TYPES Developer Full)
    
  set(CPACK_COMPONENT_H3DAPI_CPACK_SOURCES_DISPLAY_NAME "C++ Source")
  set(CPACK_COMPONENT_H3DAPI_CPACK_SOURCES_DESCRIPTION "Everything needed to build H3DAPI.")
  set(CPACK_COMPONENT_H3DAPI_CPACK_SOURCES_DEPENDS H3DAPI_cpack_headers HAPI_cpack_sources )
  set(CPACK_COMPONENT_H3DAPI_CPACK_SOURCES_GROUP "H3DAPI_cpack_group")
  set(CPACK_COMPONENT_H3DAPI_CPACK_SOURCES_INSTALL_TYPES Full)
  
  set(CPACK_COMPONENT_H3DAPI_CPACK_EXAMPLES_RUNTIME_DISPLAY_NAME "Example applications")
  set(CPACK_COMPONENT_H3DAPI_CPACK_EXAMPLES_RUNTIME_DESCRIPTION "The example applications for H3DAPI.")
  set(CPACK_COMPONENT_H3DAPI_CPACK_EXAMPLES_RUNTIME_DEPENDS H3DAPI_cpack_runtime )
  set(CPACK_COMPONENT_H3DAPI_CPACK_EXAMPLES_RUNTIME_GROUP "H3DAPI_cpack_group")
  set(CPACK_COMPONENT_H3DAPI_CPACK_EXAMPLES_RUNTIME_INSTALL_TYPES Developer Full)
  
  set(CPACK_COMPONENT_GROUP_H3DAPI_CPACK_GROUP_DISPLAY_NAME "H3DAPI")
  set(CPACK_COMPONENT_GROUP_H3DAPI_CPACK_GROUP_DESCRIPTION "H3DAPI is an open source, cross platform, scene graph API. Build X3D scenes by using the nodes written in H3DAPI. Load scenes using H3DViewer or H3DLoad that comes with this package.")
  set(CPACK_COMPONENT_GROUP_HAPI_CPACK_GROUP_PARENT_GROUP "H3DAPI_cpack_group")  

  # Add a cache variable H3D_cmake_runtime_path to point to cmake binary.
  SET (H3D_cmake_runtime_path_default "")
  IF( NOT DEFINED H3D_cmake_runtime_path )
    IF( WIN32 AND NOT UNIX )
      SET (VERSION_CMAKES "4.0" "3.9" "3.8" "3.7" "3.6" "3.5" "3.4" "3.3" "3.2" "3.1" "3.0" "2.9" "2.8" "2.7" "2.6")
      foreach (version_cmake ${VERSION_CMAKES} )
        IF (EXISTS "C:/Program Files/CMake ${version_cmake}/bin/cmake.exe")
          SET( H3D_cmake_runtime_path_default "C:/Program Files/CMake ${version_cmake}/bin/cmake.exe" )
          break()
        ENDIF (EXISTS "C:/Program Files/CMake ${version_cmake}/bin/cmake.exe")
        
        IF (EXISTS "C:/Program Files (x86)/CMake ${version_cmake}/bin/cmake.exe")
          SET( H3D_cmake_runtime_path_default "C:/Program Files (x86)/CMake ${version_cmake}/bin/cmake.exe" )
          break()
        ENDIF (EXISTS "C:/Program Files (x86)/CMake ${version_cmake}/bin/cmake.exe")
        
        IF ( EXISTS "C:/Program/CMake ${version_cmake}/bin/cmake.exe")
          SET( H3D_cmake_runtime_path_default "C:/Program/CMake ${version_cmake}/bin/cmake.exe" )
          break()
        ENDIF ( EXISTS "C:/Program/CMake ${version_cmake}/bin/cmake.exe")
      endforeach (version_cmake )
    ELSE( WIN32 AND NOT UNIX )
      SET( H3D_cmake_runtime_path_default "cmake" )
    ENDIF( WIN32 AND NOT UNIX )
    SET( H3D_cmake_runtime_path ${H3D_cmake_runtime_path_default} CACHE FILEPATH "The path to the cmake runtime." )
    MARK_AS_ADVANCED(H3D_cmake_runtime_path)
  ENDIF( NOT DEFINED H3D_cmake_runtime_path )

  IF( H3D_cmake_runtime_path )
    SET( INSTALL_RUNTIME_AND_LIBRARIES_ONLY_POST_BUILD ${INSTALL_RUNTIME_AND_LIBRARIES_ONLY_POST_BUILD}
                                                       COMMAND ${H3D_cmake_runtime_path} 
                                                       ARGS -DBUILD_TYPE=$(Configuration) -DCOMPONENT=H3DAPI_cpack_runtime -P cmake_install.cmake 
                                                       COMMAND ${H3D_cmake_runtime_path} 
                                                       ARGS -DBUILD_TYPE=$(Configuration) -DCOMPONENT=H3DAPI_cpack_libraries -P cmake_install.cmake
                                                       COMMAND ${H3D_cmake_runtime_path} 
                                                       ARGS -DBUILD_TYPE=$(Configuration) -DCOMPONENT=H3DAPI_cpack_examples_runtime -P cmake_install.cmake)
    
    IF (NOT H3D_USE_DEPENDENCIES_ONLY)
      ADD_CUSTOM_COMMAND( OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/DummyFile
                          COMMAND echo )
      ADD_CUSTOM_TARGET( INSTALL_RUNTIME_AND_LIBRARIES_ONLY
                         DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/DummyFile )
                         
      ADD_CUSTOM_COMMAND( TARGET INSTALL_RUNTIME_AND_LIBRARIES_ONLY
                          POST_BUILD
                          ${INSTALL_RUNTIME_AND_LIBRARIES_ONLY_POST_BUILD} )
      ADD_DEPENDENCIES( INSTALL_RUNTIME_AND_LIBRARIES_ONLY H3DAPI ${INSTALL_RUNTIME_AND_LIBRARIES_ONLY_DEPENDENCIES} )
    ENDIF (NOT H3D_USE_DEPENDENCIES_ONLY)
  ELSE( H3D_cmake_runtime_path )
    MESSAGE (STATUS "H3D_cmake_runtime_path is not set, please set it to continue")
  ENDIF( H3D_cmake_runtime_path )

  IF( NOT H3D_USE_DEPENDENCIES_ONLY)
	INCLUDE(CPack)
    INCLUDE(UseDebian)
    IF(DEBIAN_FOUND)
      ADD_DEBIAN_TARGETS(H3DAPI)
    ENDIF(DEBIAN_FOUND)
  ENDIF( NOT H3D_USE_DEPENDENCIES_ONLY)
ENDIF( GENERATE_CPACK_PROJECT )
