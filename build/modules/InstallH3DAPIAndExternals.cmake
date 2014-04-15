# This file detects H3DAPI libraries/include/binaries and the
# external libraries/includes/binaries a particular
# H3DAPI source is built with and installs that.
# It requires that H3DUTIL_INCLUDE_DIR is set to H3DAPI/include of
# the build that should be checked.
# H3DAPI_CMAKE_INSTALL_PREFIX must be set to the CMAKE_INSTALL_PREFIX
# used when installing H3DAPI (basically the directory above the directory
# in which H3DAPI binaries and libraries are installed.
# EXTERNAL_ROOT should be set to the External directory that comes with
# H3D.
# FEATURES_TO_INSTALL should be set to a list of pairs.  The first
# item in each pair should be a specific word. The second item in the
# pair should be the directory to install into. The predefined words are:
# "include" - Include directories should be installed.
# "lib" - Release libraries should be installed.
# "bin" - Debug binaries should be installed.
# Output variables are:
# H3DAPI_INCLUDE_FILES_INSTALL - Contains a list of include files that
# was used when the checked H3DAPI version was built.
# H3DAPI_INCLUDE_DIRECTORIES_INSTALL - Contains a list of directories that
# contains files used when the checked H3DAPI version was built.
# H3DAPI_LIBRARIES_INSTALL - Contains a list of libraries that
# was used when the checked H3DAPI version was built.
# H3DAPI_BINARIES_INSTALL - Contains a list of binaries that
# the built H3DAPI version needs.
# TODO, IMPLEMENT FOR OTHER THAN WINDOWS IF IT MAKES SENSE TO DO THAT.
# IMPLEMENT for other than MSVC10.
# GET INCLUDE_DIR AND LIBS FROM FIND_MODULES used by H3DAPI?
# IMPLEMENT to HANDLE debug libs/bins and configure to include them or not.

INCLUDE( InstallHAPIAndExternals )
IF( NOT h3d_release_only_warning )
  SET( h3d_release_only_warning TRUE )
  MESSAGE( "NOTE: Packing will only be done properly in Release build, not RelWithDebInfo, MinSizeRel or Debug" )
ENDIF( NOT h3d_release_only_warning )

IF( NOT DEFINED H3DAPI_INCLUDE_DIR )
  SET( H3DAPI_INCLUDE_DIR "" CACHE BOOL "Path to H3DAPI/include. Used to detect externals to install for H3DAPI." )
ENDIF( NOT DEFINED H3DAPI_INCLUDE_DIR )

IF( WIN32 )
  IF( NOT DEFINED EXTERNAL_ROOT )
    SET( EXTERNAL_ROOT_DEFAULT "" )
    IF( NOT "$ENV{H3D_EXTERNAL_ROOT}" STREQUAL  "" )
      SET( EXTERNAL_ROOT_DEFAULT "$ENV{H3D_EXTERNAL_ROOT}" )
    ENDIF( NOT "$ENV{H3D_EXTERNAL_ROOT}" STREQUAL  "" )
    SET( EXTERNAL_ROOT "${EXTERNAL_ROOT_DEFAULT}" CACHE PATH "Path to External directory that comes with H3D." )
  ENDIF( NOT DEFINED EXTERNAL_ROOT )
ENDIF( WIN32 )

IF( NOT DEFINED H3DAPI_CMAKE_INSTALL_PREFIX )
  SET( H3DAPI_CMAKE_INSTALL_PREFIX_DEFAULT "" )
  IF( TARGET H3DAPI )
    SET( H3DAPI_CMAKE_INSTALL_PREFIX_DEFAULT ${CMAKE_INSTALL_PREFIX} )
  ELSEIF( NOT "${H3D_ROOT_CMAKE_PATH}" STREQUAL  "" )
    SET( H3DAPI_CMAKE_INSTALL_PREFIX_DEFAULT "${H3D_ROOT_CMAKE_PATH}/.." )
  ENDIF( TARGET H3DAPI )
  SET( H3DAPI_CMAKE_INSTALL_PREFIX ${H3DAPI_CMAKE_INSTALL_PREFIX_DEFAULT} CACHE PATH "Set this to the CMAKE_INSTALL_PREFIX directory used when installing H3DUtil. It is assumed that H3DUtil is installed in bin32/bin64 and lib32/lib64." )
  MARK_AS_ADVANCED(H3DAPI_CMAKE_INSTALL_PREFIX)
ENDIF( NOT DEFINED H3DAPI_CMAKE_INSTALL_PREFIX )

SET( H3DAPI_INCLUDE_FILES_INSTALL "" CACHE INTERNAL "List of External include files used by this compiled version of H3DAPI." )
SET( H3DAPI_INCLUDE_DIRECTORIES_INSTALL "" CACHE INTERNAL "List of External include directories used by this compiled version of H3DAPI." )
SET( H3DAPI_LIBRARIES_INSTALL "" CACHE INTERNAL "List of External libraries used by this compiled version of H3DAPI." )
SET( H3DAPI_BINARIES_INSTALL "" CACHE INTERNAL "List of External binaries used by this compiled version of H3DAPI." )
SET( H3DAPI_NSIS_EXTRA_INSTALL_COMMANDS "\\n" CACHE INTERNAL "Extra install commands for installing with nsis." )
SET( H3DAPI_NSIS_EXTRA_UNINSTALL_COMMANDS "\\n" CACHE INTERNAL "Extra uninstall commands for installing with nsi." )


SET( H3DAPI_bin "bin32" )
SET( H3DAPI_lib "lib32" )
IF( CMAKE_SIZEOF_VOID_P EQUAL 8 )
  SET( H3DAPI_bin "bin64" )
  SET( H3DAPI_libib "lib64" )
ENDIF( CMAKE_SIZEOF_VOID_P EQUAL 8 )
SET( H3DAPI_EXTERNAL_BIN "${EXTERNAL_ROOT}/${H3DAPI_bin}" )
SET( H3DAPI_EXTERNAL_LIB "${EXTERNAL_ROOT}/${H3DAPI_lib}" )

IF( H3DAPI_INCLUDE_DIR AND EXTERNAL_ROOT)
  set( externals_to_look_for "" )
  IF( MSVC )
    SET( H3D_MSVC_VERSION 6 )
    SET( TEMP_MSVC_VERSION 1299 )
    WHILE( ${MSVC_VERSION} GREATER ${TEMP_MSVC_VERSION} )
      MATH( EXPR H3D_MSVC_VERSION "${H3D_MSVC_VERSION} + 1" )
      MATH( EXPR TEMP_MSVC_VERSION "${TEMP_MSVC_VERSION} + 100" )
    ENDWHILE( ${MSVC_VERSION} GREATER ${TEMP_MSVC_VERSION} )
    
    # Install OpenAL.
    SET( OpenAlInstallExe "" CACHE FILEPATH "Needs to be set to add openal installation to the package." )
    MARK_AS_ADVANCED(OpenAlInstallExe)
    IF( OpenAlInstallExe )
      GET_FILENAME_COMPONENT( OpenAL_FILE_NAME ${OpenAlInstallExe} NAME )
      STRING( REPLACE "/" "\\\\" TEMP_OpenAlInstallExe ${OpenAlInstallExe} )
      SET( OPENAL_INSTALL_COMMAND_1 " Code to install OPENAL\\n  File \\\"${TEMP_OpenAlInstallExe}\\\"\\n" )
      SET( OPENAL_INSTALL_COMMAND_2 " Wait a bit for system to unlock file.\\n  Sleep 1000\\n"
                                    " Delete install file\\n  Delete \\\"$INSTDIR\\\\${OpenAL_FILE_NAME}\\\"\\n" )
      SET( H3DOAL_NSIS_EXTRA_INSTALL_COMMANDS ${OPENAL_INSTALL_COMMAND_1}
                                              " Execute install file\\n  ExecWait '\\\"$INSTDIR\\\\${OpenAL_FILE_NAME}\\\" /s'\\n"
                                              ${OPENAL_INSTALL_COMMAND_2} )
      IF( CMAKE_SIZEOF_VOID_P EQUAL 8 ) # check if the system is 64 bit
        SET( H3DOAL_NSIS_EXTRA_UNINSTALL_COMMANDS "A comment\\n \\\${If} \\\${RunningX64}\\n"
                                                  "A comment\\n   SetRegView 32\\n"
                                                  "A comment\\n \\\${EndIf}\\n" )
      ENDIF( CMAKE_SIZEOF_VOID_P EQUAL 8 )
      SET( H3DOAL_NSIS_EXTRA_UNINSTALL_COMMANDS ${H3DOAL_NSIS_EXTRA_UNINSTALL_COMMANDS}
                                                " Get OpenAL uninstall registry string\\n  ReadRegStr $0 HKLM SOFTWARE\\\\Microsoft\\\\Windows\\\\CurrentVersion\\\\Uninstall\\\\OpenAL \\\"UninstallString\\\"\\n"
                                                " Check if OpenAL is installed\\n  StrCmp $0 \\\"\\\" uninstall_openal_no 0\\n"
                                                " Check if uninstall OpenAL \\n  MessageBox MB_YESNO \\\"Do you want to uninstall OpenAL? It is recommended if no other applications use it.\\\" IDYES uninstall_openal_yes IDNO uninstall_openal_no\\n"
                                                " A comment \\n  uninstall_openal_yes:\\n"
                                                ${OPENAL_INSTALL_COMMAND_1}
                                                " Execute install file\\n  ExecWait '\\\"$INSTDIR\\\\${OpenAL_FILE_NAME}\\\" /u /s'\\n"
                                                ${OPENAL_INSTALL_COMMAND_2}
                                                " A comment \\n  uninstall_openal_no:\\n\\n" )
      IF( CMAKE_SIZEOF_VOID_P EQUAL 8 ) # check if the system is 64 bit
        SET( H3DOAL_NSIS_EXTRA_UNINSTALL_COMMANDS ${H3DOAL_NSIS_EXTRA_UNINSTALL_COMMANDS}
                                                  "A comment\\n \\\${If} \\\${RunningX64}\\n"
                                                  "A comment\\n   SetRegView 64\\n"
                                                  "A comment\\n \\\${EndIf}\\n" )
      ENDIF( CMAKE_SIZEOF_VOID_P EQUAL 8 )
    ENDIF( OpenAlInstallExe )
    
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
      
      SET( H3DPYTHON_NSIS_EXTRA_UNINSTALL_COMMANDS ${PYTHON_INSTALL_COMMAND_1}
                                                   " Check if python is installed\\n  StrCmp $0 \\\"\\\" uninstall_python_no 0\\n"
                                                   " Check if uninstall python \\n  MessageBox MB_YESNO \\\"Do you want to uninstall python? It is recommended if no other applications use python ${CPACK_PYTHON_VERSION}.\\\" IDYES uninstall_python_yes IDNO uninstall_python_no\\n"
                                                   " A comment \\n  uninstall_python_yes:\\n"
                                                   ${PYTHON_INSTALL_COMMAND_2}
                                                   " Execute python installer, wait for completion\\n  ExecWait '\\\"msiexec\\\" /x \\\"$INSTDIR\\\\${PYTHON_FILE_NAME}\\\" /qn'\\n"
                                                   ${PYTHON_INSTALL_COMMAND_3}
                                                   " A comment \\n  uninstall_python_no:\\n" )
      SET( H3DPYTHON_NSIS_EXTRA_INSTALL_COMMANDS ${PYTHON_INSTALL_COMMAND_1}
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
                                                 "A comment \\n  install_python_no:\\n" )
    ENDIF( PythonInstallMSI )
    
    # Extra install commands will be set to install python and OpenAL
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
        SET( H3DVS_NSIS_EXTRA_INSTALL_COMMANDS ${MS_REDIST_INSTALL_COMMAND_1} )
        SET( H3DVS_NSIS_EXTRA_UNINSTALL_COMMANDS " Check if uninstall vc redist \\n  MessageBox MB_YESNO \\\"Do you want to uninstall Visual studio ${redist_version} redistributable? It is recommended if no other applications use it.\\\" IDYES uninstall_vcredist_yes IDNO uninstall_vcredist_no\\n"
                                                 " A comment \\n  uninstall_vcredist_yes:\\n"
                                                 ${MS_REDIST_INSTALL_COMMAND_1} )
        IF( ${redist_version} LESS 9 )
          SET( H3DVS_NSIS_EXTRA_INSTALL_COMMANDS ${H3DVS_NSIS_EXTRA_INSTALL_COMMANDS}
                                                 " Execute silent and wait\\n  ExecWait '\\\"$INSTDIR\\\\vc${redist_version}\\\\${VC${redist_version}_FILE_NAME}\\\" /q:a /norestart /c:\\\"msiexec /i vcredist.msi /qn\\\"'\\n" )
          SET( H3DVS_NSIS_EXTRA_UNINSTALL_COMMANDS ${H3DVS_NSIS_EXTRA_UNINSTALL_COMMANDS}
                                                 " Execute silent and wait\\n  ExecWait '\\\"$INSTDIR\\\\vc${redist_version}\\\\${VC${redist_version}_FILE_NAME}\\\" /q:a /norestart /c:\\\"msiexec /x vcredist.msi /qn\\\"'\\n" )
        ELSE( )
          SET( H3DVS_NSIS_EXTRA_INSTALL_COMMANDS ${H3DVS_NSIS_EXTRA_INSTALL_COMMANDS}
                                                 " Execute silent and wait\\n  ExecWait '\\\"$INSTDIR\\\\vc${redist_version}\\\\${VC${redist_version}_FILE_NAME}\\\" /q /norestart \\\"'\\n" )
          SET( H3DVS_NSIS_EXTRA_UNINSTALL_COMMANDS ${H3DVS_NSIS_EXTRA_UNINSTALL_COMMANDS}
                                                 " Execute silent and wait\\n  ExecWait '\\\"$INSTDIR\\\\vc${redist_version}\\\\${VC${redist_version}_FILE_NAME}\\\" /q /uninstall \\\"'\\n" )
        ENDIF( ${redist_version} LESS 9 )
        SET( MS_REDIST_INSTALL_COMMAND_2 " Wait a bit for system to unlock file.\\n  Sleep 1000\\n"
                                         " Delete file\\n  Delete \\\"$INSTDIR\\\\vc${redist_version}\\\\${VC${redist_version}_FILE_NAME}\\\"\\n"
                                         " Reset output Path\\n  SetOutPath \\\"$INSTDIR\\\"\\n"
                                         " Remove folder\\n  RMDir /r \\\"$INSTDIR\\\\vc${redist_version}\\\"\\n\\n" )
        SET( H3DVS_NSIS_EXTRA_INSTALL_COMMANDS ${H3DVS_NSIS_EXTRA_INSTALL_COMMANDS}
                                               ${MS_REDIST_INSTALL_COMMAND_2} )
        SET( H3DVS_NSIS_EXTRA_UNINSTALL_COMMANDS ${H3DVS_NSIS_EXTRA_UNINSTALL_COMMANDS}
                                                 ${MS_REDIST_INSTALL_COMMAND_2}
                                                 " A comment \\n  uninstall_vcredist_no:\\n\\n" )
      ENDIF( vc${redist_version}_redist )
    endforeach( redist_version ${redist_versions} )
  
    # When the first item for an external entry is only "#define" then it will always be included.
    set( externals_to_look_for "#define HAVE_XERCES"
                               "include" "xercesc"
                               "lib" "xerces-c_3"
                               "bin" "xerces-c_3_1"
                               
                               "#define HAVE_OPENAL"
                               "include" "AL"
                               "lib" "OpenAL32"
                               "nsisextrainstall" ${H3DOAL_NSIS_EXTRA_INSTALL_COMMANDS}
                               "nsisextrauninstall" ${H3DOAL_NSIS_EXTRA_UNINSTALL_COMMANDS}
                                                              
                               "#define HAVE_LIBVORBIS"
                               "include" "vorbis" "ogg"
                               "lib" "libvorbis" "libogg" "libvorbisfile"
                               "bin" "libvorbis" "libogg" "libvorbisfile"
                               
                               "#define HAVE_LIBAUDIOFILE"
                               "include" "libaudiofile"
                               "lib" "audiofile"
                               "bin" "audiofile"
                               
                               "#define HAVE_CG"
                               "include" "Cg"
                               "lib" "cgGL" "cg"
                               "bin" "cg" "cgGL"
                               
                               "#define HAVE_FTGL"
                               "include" "FTGL"
                               "lib" "ftgl"
                               "bin" "ftgl"
                               
                               "#define HAVE_FTGL"
                               "include" "FTGL"
                               "lib" "ftgl"
                               "bin" "ftgl"
                               
                               "#define HAVE_FREETYPE"
                               "include" "freetype"
                               "lib" "freetype2312"
                               
                               "#define HAVE_FONTCONFIG"
                               "warning" "NOTE: H3DAPI compiled with font config support. Make sure font config features are included."
                               
                               "#define HAVE_3DXWARE"
                               "warning" "NOTE: H3DAPI compiled with 3DXWare support. Test that application starts on system without 3DConnection drivers installed before distributing package if you do not distribute it yourself."
                               
                               "#define HAVE_PYTHON"
                               "nsisextrainstall" ${H3DPYTHON_NSIS_EXTRA_INSTALL_COMMANDS}
                               "nsisextrauninstall" ${H3DPYTHON_NSIS_EXTRA_UNINSTALL_COMMANDS}
                               
                               "#define HAVE_LIBCURL"
                               "include" "curl"
                               "lib" "libcurl"
                               "bin" "libcurl"
                               
                               "#define HAVE_SPIDERMONKEY"
                               "include" "js"
                               "lib" "js32"
                               "bin" "js32"
                               
                               "#define HAVE_DSHOW"
                               "include" "DirectShow"
                               "lib" "strmbase"
                               
                               "#define HAVE_FFMPEG"
                               "warning" "NOTE: H3DAPI compiled with ffmpeg support. Make sure fmpeg features are included."
                               
                               "#define HAVE_VIRTUAL_HAND_SDK"
                               "warning" "NOTE: H3DAPI compiled with Virtual Hand support. Test that application starts on system without Virtual hand installed before distributing package if you do not distribute it yourself."
                               
                               "#define"
                               "include" "GL/freeglut" "GL/freeglut" "GL/freeglut_ext" "GL/freeglut_std" "GL/glew" "GL/glext" "GL/glut" "GL/wglew" "H3D"
                               "lib" "glew32" "freeglut" "H3DAPI"
                               "bin" "glew32" "freeglut" "H3DAPI"
                               "nsisextrainstall" ${H3DVS_NSIS_EXTRA_INSTALL_COMMANDS}
                               "nsisextrauninstall" ${H3DVS_NSIS_EXTRA_UNINSTALL_COMMANDS}
                               )
  ENDIF( MSVC )
  
  list( LENGTH FEATURES_TO_INSTALL FEATURES_TO_INSTALL_LENGTH )
  math( EXPR FEATURES_TO_INSTALL_LENGTH "${FEATURES_TO_INSTALL_LENGTH} - 1" )
  set( FEATURES_TO_INSTALL_TRUNCATED "")
  foreach( loop_var RANGE 0 ${FEATURES_TO_INSTALL_LENGTH} 2 )
    list( GET FEATURES_TO_INSTALL ${loop_var} one_feature )
    list( APPEND FEATURES_TO_INSTALL_TRUNCATED ${one_feature} )
  endforeach(loop_var)
  
  SET( INCLUDE_DIRS_TO_CHECK ${EXTERNAL_ROOT}/include ${H3DAPI_INCLUDE_DIR} )
  SET( ADD_EXTERNAL FALSE )
  SET( CURRENT_CHECKED_FEATURE "" )
  foreach( H3DAPI_INCLUDE_DIR_TMP ${H3DAPI_INCLUDE_DIR} )
    IF( EXISTS ${H3DAPI_INCLUDE_DIR_TMP}/H3D/H3DApi.h )
      foreach( feature_to_look_for ${externals_to_look_for} )
        IF( feature_to_look_for MATCHES "#define" )
          SET( ADD_EXTERNAL FALSE )
          SET( regex_to_find ${feature_to_look_for} )
          FILE( STRINGS ${H3DAPI_INCLUDE_DIR_TMP}/H3D/H3DApi.h list_of_defines REGEX ${regex_to_find} )
          LIST( LENGTH list_of_defines list_of_defines_length )
          IF( list_of_defines_length )
            SET( ADD_EXTERNAL TRUE )
          ENDIF( list_of_defines_length )
        ELSEIF( ADD_EXTERNAL )
          IF( feature_to_look_for STREQUAL "include" OR
              feature_to_look_for STREQUAL "lib" OR
              feature_to_look_for STREQUAL "bin" OR
              feature_to_look_for STREQUAL "nsisextrainstall" OR
              feature_to_look_for STREQUAL "nsisextrauninstall" )
            SET( external_to_look_for_tmp ${feature_to_look_for} )
            IF( feature_to_look_for STREQUAL "nsisextrainstall" OR
                feature_to_look_for STREQUAL "nsisextrauninstall" )
              SET( external_to_look_for_tmp "bin" )
            ENDIF()
            LIST( FIND FEATURES_TO_INSTALL_TRUNCATED ${external_to_look_for_tmp} feature_found )
            IF( ${feature_found} EQUAL -1 )
              SET( CURRENT_CHECKED_FEATURE "feature_not_desired" )
            ELSE( ${feature_found} EQUAL -1 )
              SET( CURRENT_CHECKED_FEATURE ${feature_to_look_for} )
            ENDIF( ${feature_found} EQUAL -1 )
          ELSEIF( feature_to_look_for STREQUAL "warning" )
            SET( CURRENT_CHECKED_FEATURE ${feature_to_look_for} )
          ELSEIF( CURRENT_CHECKED_FEATURE STREQUAL "include" )
            SET( FOUND_INCLUDE_PATH FALSE )
            foreach( include_dir_to_check ${INCLUDE_DIRS_TO_CHECK} )
              IF( EXISTS ${include_dir_to_check}/${feature_to_look_for} )
                SET( FOUND_INCLUDE_PATH TRUE )
                SET( H3DAPI_INCLUDE_DIRECTORIES_INSTALL ${H3DAPI_INCLUDE_DIRECTORIES_INSTALL} ${include_dir_to_check}/${feature_to_look_for} )
              ELSEIF( EXISTS ${include_dir_to_check}/${feature_to_look_for}.h )
                SET( FOUND_INCLUDE_PATH TRUE )
                SET( H3DAPI_INCLUDE_FILES_INSTALL ${H3DAPI_INCLUDE_FILES_INSTALL} ${include_dir_to_check}/${feature_to_look_for}.h )
              ENDIF( EXISTS ${include_dir_to_check}/${feature_to_look_for} )
            endforeach( include_dir_to_check )
            IF( NOT FOUND_INCLUDE_PATH )
              MESSAGE( "Include directory or file ${feature_to_look_for} not found. Searched using CMake variable EXTERNAL_ROOT and H3DAPI_INCLUDE_DIR." )
            ENDIF( NOT FOUND_INCLUDE_PATH )
          ELSEIF( CURRENT_CHECKED_FEATURE STREQUAL "lib" )
            IF( EXISTS ${H3DAPI_EXTERNAL_LIB}/${feature_to_look_for}.lib )
              SET( H3DAPI_LIBRARIES_INSTALL ${H3DAPI_LIBRARIES_INSTALL} ${H3DAPI_EXTERNAL_LIB}/${feature_to_look_for}.lib )
            ELSEIF( EXISTS ${H3DAPI_EXTERNAL_LIB}/static/${feature_to_look_for}.lib )
              SET( H3DAPI_LIBRARIES_INSTALL ${H3DAPI_LIBRARIES_INSTALL} ${H3DAPI_EXTERNAL_LIB}/static/${feature_to_look_for}.lib )
            ELSEIF( TARGET ${feature_to_look_for} )
              get_target_property( h3dapi_release_filename_path ${feature_to_look_for} LOCATION_RELEASE )
              get_filename_component( h3dapi_release_filename_path ${h3dapi_release_filename_path} PATH )
              SET( H3DAPI_LIBRARIES_INSTALL ${H3DAPI_LIBRARIES_INSTALL} ${h3dapi_release_filename_path}/${feature_to_look_for}_vc${H3D_MSVC_VERSION}.lib )
            ELSEIF( H3DAPI_CMAKE_INSTALL_PREFIX )
              set( dirs_to_test ${H3DAPI_CMAKE_INSTALL_PREFIX}/${H3DAPI_lib}
                                ${H3DAPI_CMAKE_INSTALL_PREFIX}/lib )
              IF( dirs_to_test )
                foreach( dir_to_test ${dirs_to_test} )
                  IF( EXISTS ${dir_to_test}/${feature_to_look_for}_vc${H3D_MSVC_VERSION}.lib )
                    SET( H3DAPI_LIBRARIES_INSTALL ${H3DAPI_LIBRARIES_INSTALL} ${dir_to_test}/${feature_to_look_for}_vc${H3D_MSVC_VERSION}.lib )
                    break()
                  ENDIF( EXISTS ${dir_to_test}/${feature_to_look_for}_vc${H3D_MSVC_VERSION}.lib )
                endforeach( dir_to_test )
              ENDIF( dirs_to_test )
            ELSE( EXISTS ${H3DAPI_EXTERNAL_LIB}/${feature_to_look_for}.lib )
              MESSAGE( "Library file ${feature_to_look_for}.lib not found. Searched using CMake variable EXTERNAL_ROOT and H3DAPI_CMAKE_INSTALL_PREFIX." )
            ENDIF( EXISTS ${H3DAPI_EXTERNAL_LIB}/${feature_to_look_for}.lib )
          ELSEIF( CURRENT_CHECKED_FEATURE STREQUAL "bin" )
            IF( EXISTS ${H3DAPI_EXTERNAL_BIN}/${feature_to_look_for}.dll )
              SET( H3DAPI_BINARIES_INSTALL ${H3DAPI_BINARIES_INSTALL} ${H3DAPI_EXTERNAL_BIN}/${feature_to_look_for}.dll )
            ELSEIF( TARGET ${feature_to_look_for} )
              get_target_property( h3dapi_release_filename ${feature_to_look_for} LOCATION_RELEASE )
              SET( H3DAPI_BINARIES_INSTALL ${H3DAPI_BINARIES_INSTALL} ${h3dapi_release_filename} )
            ELSEIF( H3DAPI_CMAKE_INSTALL_PREFIX )
              set( dirs_to_test ${H3DAPI_CMAKE_INSTALL_PREFIX}/${H3DAPI_bin}
                                ${H3DAPI_CMAKE_INSTALL_PREFIX}/bin )
              foreach( dir_to_test ${dirs_to_test} )
                IF( EXISTS ${dir_to_test}/${feature_to_look_for}_vc${H3D_MSVC_VERSION}.dll )
                  SET( H3DAPI_BINARIES_INSTALL ${H3DAPI_BINARIES_INSTALL} ${dir_to_test}/${feature_to_look_for}_vc${H3D_MSVC_VERSION}.dll )
                  break()
                ENDIF( EXISTS ${dir_to_test}/${feature_to_look_for}_vc${H3D_MSVC_VERSION}.dll )
              endforeach( dir_to_test )
            ELSE( EXISTS ${H3DAPI_EXTERNAL_BIN}/${feature_to_look_for}.dll )
              MESSAGE( "Binary file ${feature_to_look_for}.dll not found. Searched using CMake variable EXTERNAL_ROOT and H3DAPI_CMAKE_INSTALL_PREFIX." )
            ENDIF( EXISTS ${H3DAPI_EXTERNAL_BIN}/${feature_to_look_for}.dll )
          ELSEIF( CURRENT_CHECKED_FEATURE STREQUAL "warning" )
            MESSAGE( ${feature_to_look_for} )
          ELSEIF( CURRENT_CHECKED_FEATURE STREQUAL "nsisextrainstall" )
            SET( H3DAPI_NSIS_EXTRA_INSTALL_COMMANDS ${H3DAPI_NSIS_EXTRA_INSTALL_COMMANDS} ${feature_to_look_for} )
          ELSEIF( CURRENT_CHECKED_FEATURE STREQUAL "nsisextrauninstall" )
            SET( H3DAPI_NSIS_EXTRA_UNINSTALL_COMMANDS ${H3DAPI_NSIS_EXTRA_UNINSTALL_COMMANDS} ${feature_to_look_for} )
          ENDIF() 
        ENDIF( feature_to_look_for MATCHES "#define" )
      endforeach( feature_to_look_for)
      break()
    ENDIF( EXISTS ${H3DAPI_INCLUDE_DIR_TMP}/H3D/H3DApi.h )
  endforeach( H3DAPI_INCLUDE_DIR_TMP )

  SET( INSTALL_INCLUDE FALSE )
  SET( INSTALL_LIB FALSE )
  SET( INSTALL_BIN FALSE )
  foreach( feature_to_install ${FEATURES_TO_INSTALL} )
    IF( NOT INSTALL_INCLUDE AND feature_to_install STREQUAL "include" )
      SET( INSTALL_INCLUDE TRUE )
    ELSEIF( NOT INSTALL_LIB AND feature_to_install STREQUAL "lib" )
      SET( INSTALL_LIB TRUE )
    ELSEIF( NOT INSTALL_BIN AND feature_to_install STREQUAL "bin" )
      SET( INSTALL_BIN TRUE )
    ELSEIF( INSTALL_INCLUDE )
      SET( INSTALL_INCLUDE FALSE )
      IF( H3DAPI_INCLUDE_DIRECTORIES_INSTALL )
        foreach( ext_dir ${H3DAPI_INCLUDE_DIRECTORIES_INSTALL} )
          SET( include_file_path_last_part "" )
          foreach( include_dir_to_check ${INCLUDE_DIRS_TO_CHECK} )
            get_filename_component( include_file_path ${ext_dir} PATH )
            string( LENGTH ${include_file_path} include_length )
            string( LENGTH ${include_dir_to_check} include_dir_to_check_length )
            IF( ${include_length} GREATER ${include_dir_to_check_length} )
              string( SUBSTRING ${include_file_path} 0 ${include_dir_to_check_length} include_file_path_first_part )
              IF( ${include_file_path_first_part} STREQUAL ${include_dir_to_check} )
                string( SUBSTRING ${include_file_path} ${include_dir_to_check_length} -1 include_file_path_last_part )
                break()
              ENDIF( ${include_file_path_first_part} STREQUAL ${include_dir_to_check} )
            ENDIF( ${include_length} GREATER ${include_dir_to_check_length} )
          endforeach( include_dir_to_check )
          INSTALL( DIRECTORY ${ext_dir}
                   DESTINATION ${feature_to_install}
                   REGEX "(/.svn)|(/CVS)" EXCLUDE )
        endforeach( ext_dir )
      ENDIF( H3DAPI_INCLUDE_DIRECTORIES_INSTALL )
      
      IF( H3DAPI_INCLUDE_FILES_INSTALL )
        foreach( ext_include_file ${H3DAPI_INCLUDE_FILES_INSTALL} )
          SET( include_file_path_last_part "" )
          foreach( include_dir_to_check ${INCLUDE_DIRS_TO_CHECK} )
            get_filename_component( include_file_path ${ext_include_file} PATH )
            string( LENGTH ${include_file_path} include_length )
            string( LENGTH ${include_dir_to_check} include_dir_to_check_length )
            IF( ${include_length} GREATER ${include_dir_to_check_length} )
              string( SUBSTRING ${include_file_path} 0 ${include_dir_to_check_length} include_file_path_first_part )
              IF( ${include_file_path_first_part} STREQUAL ${include_dir_to_check} )
                string( SUBSTRING ${include_file_path} ${include_dir_to_check_length} -1 include_file_path_last_part )
                break()
              ENDIF( ${include_file_path_first_part} STREQUAL ${include_dir_to_check} )
            ENDIF( ${include_length} GREATER ${include_dir_to_check_length} )
          endforeach( include_dir_to_check )
          INSTALL( FILES ${ext_include_file}
                   DESTINATION ${feature_to_install}${include_file_path_last_part} )
        endforeach( ext_include_file )
      ENDIF( H3DAPI_INCLUDE_FILES_INSTALL )
    ELSEIF( INSTALL_LIB )
      SET( INSTALL_LIB FALSE )
      IF( H3DAPI_LIBRARIES_INSTALL )
        foreach( ext_lib ${H3DAPI_LIBRARIES_INSTALL} )
          INSTALL( FILES ${ext_lib}
                   DESTINATION ${feature_to_install} )
        endforeach( ext_lib )
      ENDIF( H3DAPI_LIBRARIES_INSTALL )
    ELSEIF( INSTALL_BIN )
      SET( INSTALL_BIN FALSE )
      IF( H3DAPI_BINARIES_INSTALL )
        foreach( ext_bin ${H3DAPI_BINARIES_INSTALL} )
          INSTALL( FILES ${ext_bin}
                   DESTINATION ${feature_to_install} )
        endforeach( ext_bin )
      ENDIF( H3DAPI_BINARIES_INSTALL )
      IF( H3DAPI_NSIS_EXTRA_INSTALL_COMMANDS )
        SET( CPACK_NSIS_EXTRA_INSTALL_COMMANDS ${CPACK_NSIS_EXTRA_INSTALL_COMMANDS} ${H3DAPI_NSIS_EXTRA_INSTALL_COMMANDS} )
      ENDIF( H3DAPI_NSIS_EXTRA_INSTALL_COMMANDS )
      IF( H3DAPI_NSIS_EXTRA_UNINSTALL_COMMANDS )
        SET( CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS ${CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS} ${H3DAPI_NSIS_EXTRA_UNINSTALL_COMMANDS} )
      ENDIF( H3DAPI_NSIS_EXTRA_UNINSTALL_COMMANDS )
    ENDIF( NOT INSTALL_INCLUDE AND feature_to_install STREQUAL "include" )
  endforeach( feature_to_install )
ENDIF( H3DAPI_INCLUDE_DIR AND EXTERNAL_ROOT)