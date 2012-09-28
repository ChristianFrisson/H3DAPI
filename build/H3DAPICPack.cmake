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
      IF( $ENV{H3D_EXTERNAL_ROOT} STREQUAL "" )
        SET( H3DAPI_CPACK_EXTERNAL_ROOT "" CACHE PATH "Set to the External directory used with H3DAPI, needed to pack properly. If not set FIND_modules will be used instead." )
      ELSE( $ENV{H3D_EXTERNAL_ROOT} STREQUAL "" )
        SET( H3DAPI_CPACK_EXTERNAL_ROOT "$ENV{H3D_EXTERNAL_ROOT}" CACHE PATH "Set to the External directory used with H3DAPI, needed to pack properly. If not set FIND_modules will be used instead." )
      ENDIF( $ENV{H3D_EXTERNAL_ROOT} STREQUAL "" )
      MARK_AS_ADVANCED(H3DAPI_CPACK_EXTERNAL_ROOT)
    ENDIF( NOT DEFINED H3DAPI_CPACK_EXTERNAL_ROOT )
  ENDIF( WIN32 )
  include( ${H3DAPI_SOURCE_DIR}/../../HAPI/build/HAPICPack.cmake )

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
    SET( CPACK_PACKAGE_START_MENU_NAME "H3DAPI 2.1" )
    
    # External binary directory to add to path.
    SET( CPACK_EXTERNAL_BIN "bin32" )
    IF( CMAKE_SIZEOF_VOID_P EQUAL 8 )
      SET( CPACK_EXTERNAL_BIN "bin64" )
    ENDIF( CMAKE_SIZEOF_VOID_P EQUAL 8 )

    # Pattern in folders and files to ignore when adding files to installation package.
    SET(CPACK_IGNORE_FILES ${H3DAPI_CPACK_IGNORE_PATTERNS}
                           /fparser\\\\.lib$
                           "/wxbase28_net(_vc8)*\\\\.lib$"
                           "/wxbase28_odbc(_vc8)*\\\\.lib$"
                           "/wxbase28_xml(_vc8)*\\\\.lib$"
                           "/wxexpat(_vc8)*\\\\.lib$"
                           "/wxjpeg(_vc8)*\\\\.lib$"
                           "/wxmsw28_aui(_vc8)*\\\\.lib$"
                           "/wxmsw28_dbgrid(_vc8)*\\\\.lib$"
                           "/wxmsw28_media(_vc8)*\\\\.lib$"
                           "/wxmsw28_qa(_vc8)*\\\\.lib$"
                           "/wxmsw28_xrc(_vc8)*\\\\.lib$"
                           "/wxpng(_vc8)*\\\\.lib$"
                           "/wxregex(_vc8)*\\\\.lib$"
                           "/wxtiff(_vc8)*\\\\.lib$"
                           "/wxzlib(_vc8)*\\\\.lib$"
                           /pthreadGC2\\\\.dll$
                           /pthreadGCE2\\\\.dll$
                           "/pthreadVSE2\\\\.(dll|lib)$"
                           /HAPI/doc/
                           /H3DUtil/doc/
                           /include/fparser
                           /HapticAPI2
                           /xerces-c_2_7\\\\.dll$
                           /xerces-c_2_7D\\\\.dll$
                           /dcmdata\\\\.lib$
                           /dcmimage\\\\.lib$
                           /dcmimgle\\\\.lib$
                           /ofstd\\\\.lib$
                           /Definitions.h$
                           "/DentalSimAPI\\\\.(lib)*(dll)*(h)*$"
                           "/ogg_d\\\\.dll$"
                           "/H3DLoad_d\\\\.exe$"
                           "/H3DViewer_d\\\\.exe$" )

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
    
    SET( EXTERNAL_BIN_PATH "bin32" )
    SET( EXTERNAL_BIN_REPLACE_PATH "bin64" )
    IF( CMAKE_SIZEOF_VOID_P EQUAL 8 )
      SET( EXTERNAL_BIN_PATH "bin64" )
      SET( EXTERNAL_BIN_REPLACE_PATH "bin32" )
    ENDIF( CMAKE_SIZEOF_VOID_P EQUAL 8 )
    
    IF( EXISTS ${H3DAPI_CPACK_EXTERNAL_ROOT} )
      SET( EXTERNAL_INCLUDES ${H3DAPI_CPACK_EXTERNAL_ROOT}/include/xercesc
                             ${H3DAPI_CPACK_EXTERNAL_ROOT}/include/curl
                             ${H3DAPI_CPACK_EXTERNAL_ROOT}/include/Cg
                             ${H3DAPI_CPACK_EXTERNAL_ROOT}/include/AL
                             ${H3DAPI_CPACK_EXTERNAL_ROOT}/include/vorbis
                             ${H3DAPI_CPACK_EXTERNAL_ROOT}/include/ogg
                             ${H3DAPI_CPACK_EXTERNAL_ROOT}/include/libaudiofile
                             ${H3DAPI_CPACK_EXTERNAL_ROOT}/include/freetype
                             ${H3DAPI_CPACK_EXTERNAL_ROOT}/include/3dconnexion
                             ${H3DAPI_CPACK_EXTERNAL_ROOT}/include/FTGL
                             ${H3DAPI_CPACK_EXTERNAL_ROOT}/include/DirectShow
                             ${H3DAPI_CPACK_EXTERNAL_ROOT}/include/js )
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
                             ${H3DAPI_CPACK_EXTERNAL_ROOT}/${EXTERNAL_BIN_PATH}/OpenAL32.dll
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
      # Add the other binary path as external_source since it only needs to be included when
      # a user wants to build H3D or against it.
      STRING( REGEX REPLACE "(/${EXTERNAL_BIN_PATH}/)" "/${EXTERNAL_BIN_REPLACE_PATH}/" other_binary ${binary} )
      IF( EXISTS ${other_binary} )
        INSTALL( FILES ${binary}
                 DESTINATION External/${EXTERNAL_BIN_REPLACE_PATH}
                 COMPONENT H3DAPI_cpack_external_source )
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
      SET( H3DAPI_CPACK_INSTALLED_DIRECTORIES ${H3DAPI_CPACK_INSTALLED_DIRECTORIES}
                                              "${H3DVIEWER_PLUGIN_FOLDER};Plugins" )

      # Create cached variable for getting the UI examples folder.
      SET( UI_EXAMPLES_FOLDER "" CACHE PATH "Path to folder containing UI examples." )
      MARK_AS_ADVANCED(UI_EXAMPLES_FOLDER)
      IF( UI_EXAMPLES_FOLDER )
        SET( H3DAPI_CPACK_INSTALLED_DIRECTORIES ${H3DAPI_CPACK_INSTALLED_DIRECTORIES}
                                                "${UI_EXAMPLES_FOLDER};H3DAPI/examples/UI" )
        SET( CPACK_ADD_UI_DEMOS_LINKS "ON" )
      ELSE( UI_EXAMPLES_FOLDER )
        SET( CPACK_ADD_UI_DEMOS_LINKS "OFF" )
      ENDIF( UI_EXAMPLES_FOLDER )

      # Create cached variable for getting the MedX3D examples folder.
      SET( MedX3D_EXAMPLES_FOLDER "" CACHE PATH "Path to folder containing MedX3D examples." )
      MARK_AS_ADVANCED(MedX3D_EXAMPLES_FOLDER)
      IF( MedX3D_EXAMPLES_FOLDER )
        SET( H3DAPI_CPACK_INSTALLED_DIRECTORIES ${H3DAPI_CPACK_INSTALLED_DIRECTORIES}
                                                "${MedX3D_EXAMPLES_FOLDER};H3DAPI/examples/MedX3D" )
        SET( CPACK_ADD_MedX3D_DEMOS_LINKS "ON" )
      ELSE( MedX3D_EXAMPLES_FOLDER )
        SET( CPACK_ADD_MedX3D_DEMOS_LINKS "OFF" )
      ENDIF( MedX3D_EXAMPLES_FOLDER )

      # Create cached variable for getting the RigidBodyPhysics examples folder.
      SET( RigidBodyPhysics_EXAMPLES_FOLDER "" CACHE PATH "Path to folder containing RigidBodyPhysics examples." )
      MARK_AS_ADVANCED(RigidBodyPhysics_EXAMPLES_FOLDER)
      IF( RigidBodyPhysics_EXAMPLES_FOLDER )
        SET( H3DAPI_CPACK_INSTALLED_DIRECTORIES ${H3DAPI_CPACK_INSTALLED_DIRECTORIES}
                                                "${RigidBodyPhysics_EXAMPLES_FOLDER};H3DAPI/examples/RigidBodyPhysics" )
        SET( CPACK_ADD_RigidBodyPhysics_DEMOS_LINKS "ON" )
      ELSE( RigidBodyPhysics_EXAMPLES_FOLDER )
        SET( CPACK_ADD_RigidBodyPhysics_DEMOS_LINKS "OFF" )
      ENDIF( RigidBodyPhysics_EXAMPLES_FOLDER )

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

    # Directories to install, and how they should be named. Non-existing folders will be ignored when packing.
    #SET(CPACK_INSTALLED_DIRECTORIES ${H3DAPI_CPACK_INSTALLED_DIRECTORIES}
    #                                "${H3DAPI_SOURCE_DIR}/../../H3DAPI;H3DAPI"
    #                                "${H3DAPI_SOURCE_DIR}/../../HAPI;HAPI"
    #                                "${H3DAPI_SOURCE_DIR}/../../H3DUtil;H3DUtil"
    #                                "${H3DAPI_SOURCE_DIR}/../../doc;doc"
    #                                "${H3DAPI_SOURCE_DIR}/../../bin;bin"
    #                                "${H3DAPI_SOURCE_DIR}/../../lib;lib" )

    #Extra links to start menu if values are "ON"
    SET( CPACK_ADD_H3DDOC_LINKS "ON" )
    SET( CPACK_ADD_H3DSETTINGS_LINKS "ON" )
    
    # Extra install commands will be set to install python and OpenAL
    SET( CPACK_NSIS_EXTRA_INSTALL_COMMANDS "\\n" )
    SET( CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS "\\n" )
    
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
                                             " Delete install file\\n  Delete \\\"$INSTDIR\\\\${OpenAL_FILE_NAME}\\\"\\n\\n" )
    ENDIF( OpenAlInstallExe )
    
    # Add cache variable vc8(9)_redist which should be set to the install file
    # for microsoft visual studio redistributables, they can be found in the
    # installation folder for each visual studio installation.
    IF( NOT DEFINED vc8_redist )
      SET( vc8_redist CACHE FILEPATH "Set this to the exe installing microsoft visual studio redistributable for visual studio 8" )
      MARK_AS_ADVANCED(vc8_redist)
    ENDIF( NOT DEFINED vc8_redist )
    
    IF( NOT DEFINED vc9_redist )
      SET( vc9_redist CACHE FILEPATH "Set this to the exe installing microsoft visual studio redistributable for visual studio 9." )
      MARK_AS_ADVANCED(vc9_redist)
    ENDIF( NOT DEFINED vc9_redist )
    
    IF( NOT DEFINED vc10_redist )
      SET( vc10_redist CACHE FILEPATH "Set this to the exe installing microsoft visual studio redistributable for visual studio 10." )
      MARK_AS_ADVANCED(vc10_redist)
    ENDIF( NOT DEFINED vc10_redist )

    IF( vc8_redist )
      STRING( REPLACE "/" "\\\\" Temp_vc8_redist ${vc8_redist} )
      GET_FILENAME_COMPONENT( VC8_FILE_NAME ${vc8_redist} NAME )
      SET( CPACK_NSIS_EXTRA_INSTALL_COMMANDS ${CPACK_NSIS_EXTRA_INSTALL_COMMANDS}
                                             " Set output Path\\n  SetOutPath \\\"$INSTDIR\\\\vc8\\\"\\n"
                                             " Code to install Visual studio redistributable\\n  File \\\"${Temp_vc8_redist}\\\"\\n"
                                             " Execute silent and wait\\n  ExecWait '\\\"$INSTDIR\\\\vc8\\\\${VC8_FILE_NAME}\\\"/q:a /c:\\\"msiexec /i vcredist.msi /qn\\\"' $0\\n"
                                             " Wait a bit for system to unlock file.\\n  Sleep 1000\\n"
                                             " Delete file\\n  Delete \\\"$INSTDIR\\\\vc8\\\\${VC8_FILE_NAME}\\\"\\n"
                                             " Reset output Path\\n  SetOutPath \\\"$INSTDIR\\\"\\n"
                                             " Remove folder\\n  RMDir /r \\\"$INSTDIR\\\\vc8\\\"\\n\\n" )
    ENDIF( vc8_redist )
    
    IF( vc9_redist )
      STRING( REPLACE "/" "\\\\" Temp_vc9_redist ${vc9_redist} )
      GET_FILENAME_COMPONENT( VC9_FILE_NAME ${vc9_redist} NAME )
      SET( CPACK_NSIS_EXTRA_INSTALL_COMMANDS ${CPACK_NSIS_EXTRA_INSTALL_COMMANDS}
                                             " Set output Path\\n  SetOutPath \\\"$INSTDIR\\\\vc9\\\"\\n"
                                             " Code to install Visual studio redistributable\\n  File \\\"${Temp_vc9_redist}\\\"\\n"
                                             " Execute silent and wait\\n  ExecWait '\\\"$INSTDIR\\\\vc9\\\\${VC9_FILE_NAME}\\\"/q:a /c:\\\"msiexec /i vcredist.msi /qn /l*v %temp%\\\\vcredist_x86.log\\\"' $0\\n"
                                             " Wait a bit for system to unlock file.\\n  Sleep 1000\\n"
                                             " Delete file\\n  Delete \\\"$INSTDIR\\\\vc9\\\\${VC9_FILE_NAME}\\\"\\n"
                                             " Reset output Path\\n  SetOutPath \\\"$INSTDIR\\\"\\n"
                                             " Remove folder\\n  RMDir /r \\\"$INSTDIR\\\\vc9\\\"\\n\\n" )
    ENDIF( vc9_redist )
    
    IF( vc10_redist )
      STRING( REPLACE "/" "\\\\" Temp_vc10_redist ${vc10_redist} )
      GET_FILENAME_COMPONENT( VC10_FILE_NAME ${vc10_redist} NAME )
      SET( CPACK_NSIS_EXTRA_INSTALL_COMMANDS ${CPACK_NSIS_EXTRA_INSTALL_COMMANDS}
                                             " Set output Path\\n  SetOutPath \\\"$INSTDIR\\\\vc10\\\"\\n"
                                             " Code to install Visual studio redistributable\\n  File \\\"${Temp_vc10_redist}\\\"\\n"
                                             " Execute silent and wait\\n  ExecWait '\\\"$INSTDIR\\\\vc10\\\\${VC10_FILE_NAME}\\\"/q:a /c:\\\"msiexec /i vcredist.msi /qn /l*v %temp%\\\\vcredist_x86.log\\\"' $0\\n"
                                             " Wait a bit for system to unlock file.\\n  Sleep 1000\\n"
                                             " Delete file\\n  Delete \\\"$INSTDIR\\\\vc10\\\\${VC10_FILE_NAME}\\\"\\n"
                                             " Reset output Path\\n  SetOutPath \\\"$INSTDIR\\\"\\n"
                                             " Remove folder\\n  RMDir /r \\\"$INSTDIR\\\\vc10\\\"\\n\\n" )
    ENDIF( vc10_redist )

    # Modify path in the the NSIS template.
    SET( CPACK_NSIS_MODIFY_PATH "ON" )
  ELSE(WIN32 AND NOT UNIX)
    SET(H3DAPI_CPACK_IGNORE_PATTERNS ${H3DAPI_CPACK_IGNORE_PATTERNS} /examples/Particledemo/
                                                                     "~$" )
    SET(CPACK_SOURCE_IGNORE_FILES ${H3DAPI_CPACK_IGNORE_PATTERNS} )
    SET(CPACK_SOURCE_INSTALLED_DIRECTORIES "${H3DAPI_SOURCE_DIR}/../../H3DAPI;H3DAPI" )
  ENDIF(WIN32 AND NOT UNIX)
  
  # Install header files
  INSTALL( FILES ${H3DAPI_HEADERS}
           DESTINATION H3DAPI/include/H3DAPI
           COMPONENT H3DAPI_cpack_headers )
  
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
                 ${H3DAPI_SOURCE_DIR}/modules/FindDCMTK.cmake
                 ${H3DAPI_SOURCE_DIR}/modules/FindDirectShow.cmake
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
                 ${H3DAPI_SOURCE_DIR}/modules/FindTEEM.cmake
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
           REGEX "(/.svn)|(/CVS)" EXCLUDE )

  INSTALL( DIRECTORY ${H3DAPI_SOURCE_DIR}/../lib
           DESTINATION H3DAPI
           COMPONENT H3DAPI_cpack_sources
           REGEX "(/.svn)|(/CVS)" EXCLUDE )

  INSTALL( DIRECTORY ${H3DAPI_SOURCE_DIR}/../settings
           DESTINATION H3DAPI
           COMPONENT H3DAPI_cpack_sources
           REGEX "(/.svn)|(/CVS)" EXCLUDE )

  # Add a cache variable H3DAPI_DOCS_DIRECTORY used to indicate where the H3DAPI docs are.
  IF( NOT DEFINED H3DAPI_DOCS_DIRECTORY )
    SET( H3DAPI_DOCS_DIRECTORY "" CACHE PATH "Set this to the directory containing the documentation of H3DAPI." )
    MARK_AS_ADVANCED(H3DAPI_DOCS_DIRECTORY)
  ENDIF( NOT DEFINED H3DAPI_DOCS_DIRECTORY )
  
  IF( EXISTS ${H3DAPI_DOCS_DIRECTORY} )
    # The trailing / is there in order to copy the contents of the doc without the actual document name, since the content
    # should be put in a doc directory.
    INSTALL( DIRECTORY ${H3DAPI_DOCS_DIRECTORY}/
             DESTINATION doc
             COMPONENT H3DAPI_cpack_headers
             REGEX "(/.svn)|(/CVS)" EXCLUDE )
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
  set(CPACK_COMPONENT_GROUP_HAPI_CPACK_GROUP_PARENT_GROUP "H3DAPI_cpack_group")  

  # Add a cache variable H3D_cmake_runtime_path to point to cmake binary.
  IF( NOT DEFINED H3D_cmake_runtime_path )
    IF( WIN32 AND NOT UNIX )
      SET( H3D_cmake_runtime_path "C:/Program Files (x86)/CMake 2.8/bin/cmake.exe" CACHE FILEPATH "The path to the cmake runtime." )
    ELSE( WIN32 AND NOT UNIX )
      SET( H3D_cmake_runtime_path "cmake" CACHE FILEPATH "The path to the cmake runtime." )
    ENDIF( WIN32 AND NOT UNIX )
    MARK_AS_ADVANCED(H3D_cmake_runtime_path)
  ENDIF( NOT DEFINED H3D_cmake_runtime_path )

  SET( INSTALL_RUNTIME_AND_LIBRARIES_ONLY_POST_BUILD ${INSTALL_RUNTIME_AND_LIBRARIES_ONLY_POST_BUILD}
                                                      )
  
  ADD_CUSTOM_COMMAND( OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/DummyFile
                      COMMAND echo )
  ADD_CUSTOM_TARGET( INSTALL_RUNTIME_AND_LIBRARIES_ONLY
                     DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/DummyFile )

  ADD_CUSTOM_COMMAND( TARGET INSTALL_RUNTIME_AND_LIBRARIES_ONLY
                      POST_BUILD
                      ${INSTALL_RUNTIME_AND_LIBRARIES_ONLY_POST_BUILD}
                      COMMAND ${H3D_cmake_runtime_path} 
                      ARGS -DBUILD_TYPE=$(Configuration) -DCOMPONENT=H3DAPI_cpack_runtime -P cmake_install.cmake 
                      COMMAND ${H3D_cmake_runtime_path} 
                      ARGS -DBUILD_TYPE=$(Configuration) -DCOMPONENT=H3DAPI_cpack_libraries -P cmake_install.cmake
                      COMMAND ${H3D_cmake_runtime_path} 
                      ARGS -DBUILD_TYPE=$(Configuration) -DCOMPONENT=H3DAPI_cpack_examples_runtime -P cmake_install.cmake )
  ADD_DEPENDENCIES( INSTALL_RUNTIME_AND_LIBRARIES_ONLY H3DAPI ${INSTALL_RUNTIME_AND_LIBRARIES_ONLY_DEPENDENCIES} )

  IF( WIN32 )
    #SET(CPACK_INSTALLED_DIRECTORIES ${CPACK_INSTALLED_DIRECTORIES}
    #                                ${HAPI_CPACK_INSTALLED_DIRECTORIES} )
  ENDIF(WIN32)

  INCLUDE(CPack)
  INCLUDE(UseDebian)
  IF(DEBIAN_FOUND)
    ADD_DEBIAN_TARGETS(H3DAPI)
  ENDIF(DEBIAN_FOUND)
ENDIF( GENERATE_CPACK_PROJECT )
