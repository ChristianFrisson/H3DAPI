IF( COMMAND cmake_policy )
  IF( POLICY CMP0003 )
    cmake_policy(SET CMP0003 OLD)
  ENDIF( POLICY CMP0003 )
ENDIF( COMMAND cmake_policy )

IF(WIN32)
  cmake_minimum_required(VERSION 2.6.0)
ENDIF(WIN32)

PROJECT (H3DAPI)

# Make include files in paths like /usr/local/include have preference before
# files in OSX frameworks. 
SET(CMAKE_FIND_FRAMEWORK "LAST")

# Add a cache variable GENERATE_CPACK_PROJECT to have the choice of generating a project
# for packaging H3DAPI. Default is NO since most people will not use this.
IF( NOT DEFINED GENERATE_CPACK_PROJECT )
  SET( GENERATE_CPACK_PROJECT "NO" CACHE BOOL "Decides if a cpack project should be generated. The project in the first loaded CMakeLists will configure CPack." )
  MARK_AS_ADVANCED(GENERATE_CPACK_PROJECT)
ENDIF( NOT DEFINED GENERATE_CPACK_PROJECT )

# Add a cache variable PREFER_STATIC_LIBRARIES to have the choice of generating a project
# linking against static libraries if they exist. Default is NO since most people will not use this.
IF( NOT DEFINED PREFER_STATIC_LIBRARIES )
  SET( PREFER_STATIC_LIBRARIES "NO" CACHE BOOL "Decides if CMake should prefer static libraries to dynamic libraries when both exist. To link FreeGLUT static see the PREFER_FREEGLUT_STATIC_LIBRARIES option." )
  MARK_AS_ADVANCED(PREFER_STATIC_LIBRARIES)
ENDIF( NOT DEFINED PREFER_STATIC_LIBRARIES )

IF( WIN32 AND NOT DEFINED PREFER_FREEGLUT_STATIC_LIBRARIES )
  # Add a cache variable PREFER_FREEGLUT_STATIC_LIBRARIES to have the choice of generating a project
  # linking statically against glut. Default is NO since most people will not use this.
  SET( PREFER_FREEGLUT_STATIC_LIBRARIES "NO" CACHE BOOL "Decides if CMake should prefer static libraries to dynamic libraries for FreeGLUT when both exist." )
  MARK_AS_ADVANCED(PREFER_FREEGLUT_STATIC_LIBRARIES)
ENDIF( WIN32 AND NOT DEFINED PREFER_FREEGLUT_STATIC_LIBRARIES )


IF( PREFER_STATIC_LIBRARIES )
  SET( CMAKE_FIND_LIBRARY_SUFFIXES .a;${CMAKE_FIND_LIBRARY_SUFFIXES} )
ENDIF( PREFER_STATIC_LIBRARIES )


# Include HAPI in the build.
IF( WIN32 )
  # Should this part only be done for Windows? It is used to avoid placing everything directly in "build" catalogue if some other catalogue is chosen.
  STRING( REPLACE ${H3DAPI_SOURCE_DIR} ${H3DAPI_SOURCE_DIR}/../../HAPI/build BINARY_DIR_FOR_HAPI ${CMAKE_CURRENT_BINARY_DIR} )
  STRING( COMPARE EQUAL ${BINARY_DIR_FOR_HAPI} ${CMAKE_CURRENT_BINARY_DIR} SPECIAL_BINARY_DIR )
  # If the string was not found when doing replace the default directory should be used for build files.
  IF( SPECIAL_BINARY_DIR )
    SET( BINARY_DIR_FOR_HAPI ${H3DAPI_SOURCE_DIR}/../../HAPI/build )
  ENDIF( SPECIAL_BINARY_DIR )
  ADD_SUBDIRECTORY( ${H3DAPI_SOURCE_DIR}/../../HAPI/build
                  ${BINARY_DIR_FOR_HAPI} )

  GET_DIRECTORY_PROPERTY( HAPI_INCLUDES DIRECTORY ${H3DAPI_SOURCE_DIR}/../../HAPI/build INCLUDE_DIRECTORIES )
  INCLUDE_DIRECTORIES( ${HAPI_INCLUDES} )

  # Check if Chai3D was found.
  IF(CHAI3D_INCLUDE_DIR AND CHAI3D_LIBRARY)
    SET( CHAI3D_FOUND 1 )
  ENDIF(CHAI3D_INCLUDE_DIR AND CHAI3D_LIBRARY)

  # Check if OpenHaptics was found.
  IF(OPENHAPTICS_INCLUDE_DIR AND OPENHAPTICS_HD_LIBRARY AND OPENHAPTICS_HL_LIBRARY AND OPENHAPTICS_HDU_LIBRARY)
    SET( OPENHAPTICS_FOUND 1 )
  ENDIF(OPENHAPTICS_INCLUDE_DIR AND OPENHAPTICS_HD_LIBRARY AND OPENHAPTICS_HL_LIBRARY AND OPENHAPTICS_HDU_LIBRARY)

  # Check if HAPTIK was found.
  IF(HAPTIK_INCLUDE_DIR AND HAPTIK_LIBRARY)
    SET( HAPTIK_FOUND 1 )
  ENDIF(HAPTIK_INCLUDE_DIR AND HAPTIK_LIBRARY)
ENDIF( WIN32 )

# Version of H3D API.
SET( H3DAPI_MAJOR_VERSION 2 )
SET( H3DAPI_MINOR_VERSION 1 )
SET( H3DAPI_BUILD_VERSION 0 )

# Set here already in case a special NSIS template needs to be used by cpack.
SET(CMAKE_MODULE_PATH ${H3DAPI_SOURCE_DIR}/modules )
IF( COMMAND cmake_policy )
  IF( POLICY CMP0011 )
    cmake_policy( SET CMP0011 NEW )
  ENDIF( POLICY CMP0011 )
ENDIF( COMMAND cmake_policy )
INCLUDE( StripAndAddLibraryDirectories )

# If cpack should be configured.
IF( GENERATE_CPACK_PROJECT )
  # Set information properties about the project to install.
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
    SET( CPACK_PACKAGE_START_MENU_NAME "H3DAPI 2.1" )
    # If H3DLoad should be added to the package.
    SET( H3DLoad_cmake_install "" CACHE PATH "Needs to be set if H3DLoad is desired to be packed in the install. Set to location of cmake_install.cmake for H3DLoad." )
    MARK_AS_ADVANCED(H3DLoad_cmake_install)
    IF( H3DLoad_cmake_install )
      SET(CPACK_INSTALL_CMAKE_PROJECTS ${CPACK_INSTALL_CMAKE_PROJECTS}
                                       "${H3DLoad_cmake_install};H3DLoad;ALL;/" )
    ENDIF( H3DLoad_cmake_install )

    IF( H3DLoad_cmake_install OR H3DAPI_LOADER_PROJECTS )
      SET( CPACK_ADD_H3DLOAD_DEMOS_LINKS "ON" )
    ENDIF( H3DLoad_cmake_install OR H3DAPI_LOADER_PROJECTS )

    # If H3DViewer should be added to package.
    SET( H3DViewer_cmake_install "" CACHE PATH "Needs to be set if H3DViewer is desired to be packed in the install.  Set to location of cmake_install.cmake for H3DViewer." )
    MARK_AS_ADVANCED(H3DViewer_cmake_install)
    IF( H3DViewer_cmake_install )
      SET( CPACK_INSTALL_CMAKE_PROJECTS ${CPACK_INSTALL_CMAKE_PROJECTS}
                                        "${H3DViewer_cmake_install};H3DViewer;ALL;/" )
      SET(WXWINDOWS_USE_GL 1)
      FIND_PACKAGE(wxWidgetsWin)
    ENDIF( H3DViewer_cmake_install )
    
    IF( H3DViewer_cmake_install OR H3DAPI_LOADER_PROJECTS )
      SET( CPACK_PACKAGE_EXECUTABLES ${CPACK_PACKAGE_EXECUTABLES}
                                     "H3DViewer;H3DViewer" )
    ENDIF( H3DViewer_cmake_install OR H3DAPI_LOADER_PROJECTS )

    #If particleDemo should be added to package.
    SET( ParticleDemo_cmake_install "" CACHE PATH "Needs to be set if ParticleDemo is desired to be packed in the install.  Set to location of cmake_install.cmake for ParticleDemo." )
    MARK_AS_ADVANCED(ParticleDemo_cmake_install)
    IF( ParticleDemo_cmake_install )
      SET( CPACK_INSTALL_CMAKE_PROJECTS ${CPACK_INSTALL_CMAKE_PROJECTS}
                                        "${ParticleDemo_cmake_install};ParticleDemo;ALL;/" )
    ELSE( ParticleDemo_cmake_install )
      SET(H3DAPI_CPACK_IGNORE_PATTERNS ${H3DAPI_CPACK_IGNORE_PATTERNS} /examples/Particledemo/ )
    ENDIF( ParticleDemo_cmake_install )

    IF( NOT PREFER_STATIC_LIBRARIES )
      # Ignore static libraries.
      SET( H3DAPI_CPACK_IGNORE_PATTERNS ${H3DAPI_CPACK_IGNORE_PATTERNS}
                                        /freeglut_static\\\\.lib$
                                        /glew32_static\\\\.lib$
                                        "/FreeImage_static(_vc8)*(_vc9)*(_d)*\\\\.lib$"
                                        "/ftgl_static_MTD(_vc8)*(_vc9)*(_d)*\\\\.lib$"
                                        /libcurl_static\\\\.lib$
                                        /ogg_static\\\\.lib$
                                        /vorbis_static\\\\.lib$
                                        /vorbisfile_static\\\\.lib$
                                        "/(X|x)erces-c_static_2(_vc8)*(_vc9)*(_d)*\\\\.lib$" )
    ENDIF( NOT PREFER_STATIC_LIBRARIES )

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

    SET( H3DAPI_CPACK_INSTALLED_DIRECTORIES "" )

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
    SET(CPACK_INSTALLED_DIRECTORIES ${H3DAPI_CPACK_INSTALLED_DIRECTORIES}
                                    "${H3DAPI_SOURCE_DIR}/../../H3DAPI;H3DAPI"
                                    "${H3DAPI_SOURCE_DIR}/../../HAPI;HAPI"
                                    "${H3DAPI_SOURCE_DIR}/../../H3DUtil;H3DUtil"
                                    "${H3DAPI_SOURCE_DIR}/../../doc;doc"
                                    "${H3DAPI_SOURCE_DIR}/../../bin;bin"
                                    "${H3DAPI_SOURCE_DIR}/../../lib;lib" )

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

    # Modify path in the the NSIS template.
    SET( CPACK_NSIS_MODIFY_PATH "ON" )
  ELSE(WIN32 AND NOT UNIX)
    SET(H3DAPI_CPACK_IGNORE_PATTERNS ${H3DAPI_CPACK_IGNORE_PATTERNS} /examples/Particledemo/
                                                                     "~$" )
    SET(CPACK_SOURCE_IGNORE_FILES ${H3DAPI_CPACK_IGNORE_PATTERNS} )
    SET(CPACK_SOURCE_INSTALLED_DIRECTORIES "${H3DAPI_SOURCE_DIR}/../../H3DAPI;H3DAPI" )
  ENDIF(WIN32 AND NOT UNIX)
ENDIF( GENERATE_CPACK_PROJECT )

# Version of project.
SET( H3DAPI_FULL_VERSION
${H3DAPI_MAJOR_VERSION}.${H3DAPI_MINOR_VERSION}.${H3DAPI_BUILD_VERSION} ) 

# Add all sources, they are added to a variable called H3DAPI_SRCS
# defined in the included file. All header files are added to a variable called
# H3DAPI_HEADERS.
INCLUDE( ${H3DAPI_SOURCE_DIR}/H3DAPISourceFiles.txt )

# Add the H3DAPI.rc resource file if Visual Studio
IF(MSVC)
  SET( H3DAPI_SRCS ${H3DAPI_SRCS} ${CMAKE_CURRENT_BINARY_DIR}/H3DAPI.rc )
ENDIF(MSVC)

# Add all optional libraries to this variable
SET(optionalLibs)

# Add all required libraries to this variable
SET(requiredLibs)

# Add projects to link against. The projects where created when adding subdirectory in the beginning of this file.
IF(NOT WIN32)
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
ELSEIF( MSVC70 OR MSVC71 OR ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION} GREATER 2.7)
  # On other versions than vc7 visual studio dependency setting is enough to 
  # link correctly( except from cmake v2.8 and forward)
  SET(requiredLibs ${requiredLibs} H3DUtil HAPI)

  IF(CHAI3D_FOUND)
    SET(requiredLibs ${requiredLibs} Chai3DRenderer)
  ENDIF(CHAI3D_FOUND)

  IF(OPENHAPTICS_FOUND)
    SET(requiredLibs ${requiredLibs} OpenHapticsRenderer)
  ENDIF(OPENHAPTICS_FOUND)
ELSEIF( MSVC80 )
  IF(CHAI3D_FOUND)
    SET(requiredLibs ${requiredLibs} atls.lib)
  ENDIF(CHAI3D_FOUND)
ENDIF(NOT WIN32)

# Include directories needed.
INCLUDE_DIRECTORIES( ../include )

# Extra libs to link against.
IF(WIN32)
  INCLUDE_DIRECTORIES( ${H3DAPI_SOURCE_DIR}/../../H3DUtil/include
                       ${H3DAPI_SOURCE_DIR}/../../HAPI/include )
  # TODO: Only for MSVC?
  SET(requiredLibs ${requiredLibs} winmm.lib DelayImp.lib )
ENDIF(WIN32)

# Find pthread library. Required to compile.
FIND_PACKAGE(PTHREAD REQUIRED)
IF(PTHREAD_FOUND)
  INCLUDE_DIRECTORIES( ${PTHREAD_INCLUDE_DIR} ) 
  SET(requiredLibs ${requiredLibs} ${PTHREAD_LIBRARIES} )
ENDIF(PTHREAD_FOUND)

# Find OpenGL library. Required to compile.
FIND_PACKAGE(OpenGL REQUIRED)
IF(OPENGL_FOUND)
  SET(HAVE_OPENGL 1)
  INCLUDE_DIRECTORIES( ${OPENGL_INCLUDE_DIR} )
  SET(requiredLibs ${requiredLibs} ${OPENGL_LIBRARIES} )
ENDIF(OPENGL_FOUND)

# Find Glew library. Required to compile.
FIND_PACKAGE(GLEW REQUIRED)
IF(GLEW_FOUND)
  INCLUDE_DIRECTORIES( ${GLEW_INCLUDE_DIR} ) 
  SET(requiredLibs ${requiredLibs} ${GLEW_LIBRARIES} )
ENDIF(GLEW_FOUND)

# Optional extra library. Makes is possible to used GLUT windows handling with H3D API.
IF(WIN32)
  FIND_PACKAGE(GLUTWin)
ELSE(WIN32)
  FIND_PACKAGE(GLUT)
ENDIF(WIN32)

IF(GLUT_FOUND)
  INCLUDE_DIRECTORIES( ${GLUT_INCLUDE_DIR} )
  IF(WIN32)
    # This is used in order for the compiler to find freeglut.lib.
    # If freeglut.lib is in any other place than glut32 on your system
    # either this CMakeLists.txt or FindGLUTWin needs to be changed
    # to allow for choice of extra library directory.
    STRIP_AND_ADD_LIBRARY_DIRECTORIES( ${GLUT_LIBRARIES} )
  ELSE(WIN32)
    SET(requiredLibs ${requiredLibs} ${GLUT_LIBRARIES} )
  ENDIF(WIN32)
ENDIF(GLUT_FOUND)

# Optional extra library. Without this H3D API can not load scenes defined with X3D syntax.
FIND_PACKAGE(Xerces)
IF(XERCES_FOUND)
  SET(HAVE_XERCES 1)
  INCLUDE_DIRECTORIES( ${XERCES_INCLUDE_DIR} ) 
  SET(optionalLibs ${optionalLibs} ${XERCES_LIBRARIES} )
ENDIF(XERCES_FOUND)

# Optional extra library. Used in H3D API to resolved urls.
FIND_PACKAGE(H3DCURL)
IF(CURL_FOUND)
  SET(HAVE_LIBCURL 1)
  INCLUDE_DIRECTORIES( ${CURL_INCLUDE_DIR} ) 
  SET(optionalLibs ${optionalLibs} ${CURL_LIBRARIES} )
ENDIF(CURL_FOUND)

# Optional extra library. Needed to support shading using the CG shader language from NVidia.
FIND_PACKAGE(NvidiaCG)
IF(NVIDIACG_FOUND)
  SET(HAVE_CG 1)
  INCLUDE_DIRECTORIES( ${NVIDIACG_INCLUDE_DIR} ) 
  SET(optionalLibs ${optionalLibs} ${NVIDIACG_LIBRARIES} )
ENDIF(NVIDIACG_FOUND)

# Optional extra library. Used to read compressed files.
FIND_PACKAGE(H3DZLIB)
IF(ZLIB_FOUND)
  INCLUDE_DIRECTORIES( ${ZLIB_INCLUDE_DIR} )
  SET(optionalLibs ${optionalLibs} ${ZLIB_LIBRARIES} )
ENDIF(ZLIB_FOUND)

# Optional extra library. Add python bindings to H3D API, allow for extended prototyping without using C++.
FIND_PACKAGE(PythonLibs)
IF( PYTHON_INCLUDE_PATH AND PYTHON_LIBRARIES)
  SET(PYTHON_FOUND 1)
ELSE(PYTHON_INCLUDE_PATH AND PYTHON_LIBRARIES)
  SET(PYTHON_FOUND 0)
ENDIF(PYTHON_INCLUDE_PATH AND PYTHON_LIBRARIES)

# Report the results.
IF(NOT PYTHON_FOUND)
  SET(PYTHON_DIR_MESSAGE
    "PYTHON was not found. Make sure PYTHON_LIBRARY and PYTHON_INCLUDE_PATH are set.")
  IF(NOT PYTHON_FIND_QUIETLY)
    MESSAGE(STATUS "${PYTHON_DIR_MESSAGE}")
  ELSE(NOT PYTHON_FIND_QUIETLY)
    IF(PYTHON_FIND_REQUIRED) 
      MESSAGE(FATAL_ERROR "${PYTHON_DIR_MESSAGE}")
    ENDIF(PYTHON_FIND_REQUIRED)
  ENDIF(NOT PYTHON_FIND_QUIETLY)
ENDIF(NOT PYTHON_FOUND)

IF(PYTHON_FOUND)
  SET(HAVE_PYTHON 1)
  INCLUDE_DIRECTORIES( ${PYTHON_INCLUDE_PATH} ) 
  SET(optionalLibs ${optionalLibs} ${PYTHON_LIBRARIES} )
ENDIF(PYTHON_FOUND)

# Optional extra library. Used for sound in H3D API.
FIND_PACKAGE(H3DOpenAL)
IF(OPENAL_FOUND)
  SET(HAVE_OPENAL 1)
  INCLUDE_DIRECTORIES( ${OPENAL_INCLUDE_DIR} )
  SET(optionalLibs ${optionalLibs} ${OPENAL_LIBRARIES} )
ENDIF(OPENAL_FOUND)

# Optional extra library. Used to read sound files.
FIND_PACKAGE(Vorbis)
IF(VORBIS_FOUND)
  SET(HAVE_LIBVORBIS 1)
  INCLUDE_DIRECTORIES( ${VORBIS_INCLUDE_DIR} ) 
  SET(optionalLibs ${optionalLibs} ${VORBIS_LIBRARIES} )
ENDIF(VORBIS_FOUND)

# Optional extra library. Used to read sound files.
FIND_PACKAGE(Audiofile)
IF(AUDIOFILE_FOUND)
  SET(HAVE_LIBAUDIOFILE 1)
  INCLUDE_DIRECTORIES( ${AUDIOFILE_INCLUDE_DIR} ) 
  SET(optionalLibs ${optionalLibs} ${AUDIOFILE_LIBRARIES} )
ENDIF(AUDIOFILE_FOUND)

# Optional extra library. Used to handle fonts in H3D API.
FIND_PACKAGE(FreeType)
IF(FREETYPE_FOUND)
  SET(HAVE_FREETYPE 1)
  INCLUDE_DIRECTORIES( ${FREETYPE_INCLUDE_DIR} ) 
  SET(optionalLibs ${optionalLibs} ${FREETYPE_LIBRARIES} )
ENDIF(FREETYPE_FOUND)

# Optional extra library. Adds support to use devices by 3dxware. See www.3dconnexion.com.
FIND_PACKAGE(3DXWARE)
IF(3DXWARE_FOUND)
  SET(HAVE_3DXWARE 1)
  INCLUDE_DIRECTORIES( ${3DXWARE_INCLUDE_DIR} ) 
  SET(optionalLibs ${optionalLibs} ${3DXWARE_LIBRARIES} )
ENDIF(3DXWARE_FOUND)

# Optional extra library. Used to render fonts in OpenGL.
FIND_PACKAGE(FTGL)
IF(FTGL_FOUND)
  SET(HAVE_FTGL 1)
  SET(FTGL_INCLUDE_IS_UPPER ${FTGL_INCLUDE_IS_UPPER})
  INCLUDE_DIRECTORIES( ${FTGL_INCLUDE_DIR} ) 
  SET(optionalLibs ${optionalLibs} ${FTGL_LIBRARIES} )
ENDIF(FTGL_FOUND)

# Optional extra library. Used to read images.
FIND_PACKAGE(FreeImage)
IF(FREEIMAGE_FOUND)
  INCLUDE_DIRECTORIES( ${FREEIMAGE_INCLUDE_DIR} ) 
  SET(optionalLibs ${optionalLibs} ${FREEIMAGE_LIBRARIES} )
ENDIF(FREEIMAGE_FOUND)

IF(HAPTIK_FOUND)
  SET(optionalLibs ${optionalLibs} ${HAPTIK_LIBRARY} )
ENDIF(HAPTIK_FOUND)

IF(WIN32)
  # Windows specific libraries

  # Optional extra library. Used to render movies as textures.
  FIND_PACKAGE(DirectShow)
  IF(DIRECTSHOW_FOUND)
    SET(HAVE_DSHOW 1)
    INCLUDE_DIRECTORIES( ${DIRECTSHOW_INCLUDE_DIR} ) 
    SET(optionalLibs ${optionalLibs} ${DIRECTSHOW_LIBRARIES} )
  ENDIF(DIRECTSHOW_FOUND)
ENDIF(WIN32)

IF(NOT WIN32 AND NOT APPLE)
  # Linux specific libraries

  FIND_PACKAGE(FontConfig)
  IF(FONTCONFIG_FOUND)
    SET(HAVE_FONTCONFIG 1)
    INCLUDE_DIRECTORIES( ${FONTCONFIG_INCLUDE_DIR} ) 
    SET(optionalLibs ${optionalLibs} ${FONTCONFIG_LIBRARIES} )
  ENDIF(FONTCONFIG_FOUND)

  # Optional extra library. Used to render movies as textures.
  FIND_PACKAGE(FFmpeg)
  IF(FFMPEG_FOUND)
    SET(HAVE_FFMPEG 1)
    INCLUDE_DIRECTORIES( ${FFMPEG_INCLUDE_DIRS} )  
    LIST( APPEND optionalLibs ${FFMPEG_LIBRARIES} )
  ENDIF(FFMPEG_FOUND)  
ENDIF(NOT WIN32 AND NOT APPLE)

FIND_PACKAGE(DCMTK)
IF(DCMTK_FOUND)
  INCLUDE_DIRECTORIES( ${DCMTK_INCLUDE_DIR} ) 
  SET(requiredLibs ${requiredLibs} ${DCMTK_LIBRARIES} )
ENDIF(DCMTK_FOUND)

FIND_PACKAGE(TEEM)
IF(TEEM_FOUND)
  INCLUDE_DIRECTORIES( ${TEEM_INCLUDE_DIR} ) 
  SET(requiredLibs ${requiredLibs} ${TEEM_LIBRARIES} )
ENDIF(TEEM_FOUND)

FIND_PACKAGE(V8)
IF(V8_FOUND)
  SET( HAVE_V8 1 )
  INCLUDE_DIRECTORIES( ${V8_INCLUDE_DIR} ) 
  SET(requiredLibs ${requiredLibs} ${V8_LIBRARIES} )
ENDIF(V8_FOUND)

FIND_PACKAGE(SpiderMonkey)
IF(SPIDERMONKEY_FOUND)
  SET( HAVE_SPIDERMONKEY 1 )
  INCLUDE_DIRECTORIES( ${SPIDERMONKEY_INCLUDE_DIR} ) 
  SET(requiredLibs ${requiredLibs} ${SPIDERMONKEY_LIBRARIES} )
ENDIF(SPIDERMONKEY_FOUND)

FIND_PACKAGE(VirtualHand)
IF(VirtualHand_FOUND)
  SET( HAVE_VIRTUAL_HAND_SDK 1 )
  INCLUDE_DIRECTORIES( ${VirtualHand_INCLUDE_DIR} ) 
  SET(requiredLibs ${requiredLibs} ${VirtualHand_LIBRARIES} )
ENDIF(VirtualHand_FOUND)


# If cpack project should be generated and windows is the system on which to generate
# a number of extra directories needs to be added to be installed.
IF( GENERATE_CPACK_PROJECT )
  IF( WIN32 )
    SET(CPACK_INSTALLED_DIRECTORIES ${CPACK_INSTALLED_DIRECTORIES}
                                    ${HAPI_CPACK_INSTALLED_DIRECTORIES} )

  ENDIF(WIN32)

  INCLUDE(CPack)
  INCLUDE(UseDebian)
  IF(DEBIAN_FOUND)
    ADD_DEBIAN_TARGETS(H3DAPI)
  ENDIF(DEBIAN_FOUND)
ENDIF( GENERATE_CPACK_PROJECT )

# H3DAPI shared library definition
ADD_LIBRARY(H3DAPI SHARED ${H3DAPI_SRCS} ${H3DAPI_HEADERS})

# make sure that the H3DAPI.rc contains the correct svn-version
IF( MSVC )
  EXECUTE_PROCESS( COMMAND ${H3DAPI_SOURCE_DIR}/FindSVNVersion ${H3DAPI_SOURCE_DIR}/../src/.svn/entries 
                   OUTPUT_VARIABLE H3DAPI_SVN_VERSION )
  # autogenerate H3DAPI.rc depending on the version
  CONFIGURE_FILE( H3DAPI.rc.cmake ${CMAKE_CURRENT_BINARY_DIR}/H3DAPI.rc )

  ADD_CUSTOM_COMMAND( TARGET H3DAPI 
                      PRE_BUILD 
                      COMMAND ${H3DAPI_SOURCE_DIR}/UpdateResourceFile 
                      ARGS H3DAPI ${CMAKE_CURRENT_BINARY_DIR}/H3DAPI.rc ${H3DAPI_SOURCE_DIR}/H3DAPI.rc.cmake
                      ${H3DAPI_MAJOR_VERSION} ${H3DAPI_MINOR_VERSION}
                      ${H3DAPI_BUILD_VERSION}
                      ${H3DAPI_SOURCE_DIR}/../src/.svn/entries )
ENDIF(MSVC)

# set the install directory to the H3D directory on Windows
IF(WIN32 AND CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT )
  SET( CMAKE_INSTALL_PREFIX ${H3DAPI_SOURCE_DIR}/../.. CACHE PATH "Install path prefix, prepended onto install directories." FORCE )
ENDIF(WIN32 AND CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT )

# make sure symbols are exported.
SET( H3DAPI_COMPILE_FLAGS "-DH3DAPI_EXPORTS" )

# add the libraries needed for linking
TARGET_LINK_LIBRARIES( H3DAPI ${requiredLibs} ${optionalLibs} )

# make the name of debug libraries end in _d.
SET_TARGET_PROPERTIES( H3DAPI PROPERTIES DEBUG_POSTFIX "_d" )

# set the version of the library
SET_TARGET_PROPERTIES( H3DAPI PROPERTIES VERSION ${H3DAPI_FULL_VERSION} )

IF( WIN32 )
  # Temporary variables used later to set LINK_FLAGS.
  SET( TEMP_OUTPUT_POSTFIX "" )
  SET( H3DAPI_LINK_FLAGS "" )
  SET( H3DAPI_LINK_FLAGS_RELEASE "" )
  SET( H3DAPI_LINK_FLAGS_DEBUG "" )

  # change the name depending on compiler to be able to tell them apart
  # since they are not compatible with each other. 
  IF(MSVC70 OR MSVC71)
    SET_TARGET_PROPERTIES( H3DAPI PROPERTIES OUTPUT_NAME H3DAPI_vc7 )
    SET( TEMP_OUTPUT_POSTFIX _vc7 )
    # H3D API is now to big to link incrementally with vc7.
    SET( H3DAPI_LINK_FLAGS "${H3DAPI_LINK_FLAGS} /INCREMENTAL:NO" )
    SET( H3DAPI_LINK_FLAGS_DEBUG "${H3DAPI_LINK_FLAGS_DEBUG} /NODEFAULTLIB:msvcrt"  )
  ELSEIF(MSVC80)
    SET_TARGET_PROPERTIES( H3DAPI PROPERTIES OUTPUT_NAME H3DAPI_vc8 )
    SET( TEMP_OUTPUT_POSTFIX _vc8 )
    # remove some warnings about deprecated functions.
    SET( H3DAPI_COMPILE_FLAGS "${H3DAPI_COMPILE_FLAGS} -D_CRT_SECURE_NO_DEPRECATE /Zc:wchar_t-" )
    
    # This might be useful for visual studio 2005 users that often recompile the api.
    IF( NOT DEFINED USE_VC8_MP_FLAG )
      SET( USE_VC8_MP_FLAG "NO" CACHE BOOL "In visual studio 8 the MP flag exists but is not documented. Maybe it is unsafe to use. If you want to use it then set this flag to yes." )
    ENDIF( NOT DEFINED USE_VC8_MP_FLAG )
    
    IF( USE_VC8_MP_FLAG )
      SET( H3DAPI_COMPILE_FLAGS "${H3DAPI_COMPILE_FLAGS} /MP" )
    ENDIF( USE_VC8_MP_FLAG )
    
    SET( H3DAPI_LINK_FLAGS_DEBUG "${H3DAPI_LINK_FLAGS_DEBUG} /NODEFAULTLIB:msvcrt" )
    # if we are using 64 bit compiler some object files get too big so we need
    # to add the /bigobj flag
    IF( CMAKE_CL_64 )
      SET( H3DAPI_COMPILE_FLAGS "${H3DAPI_COMPILE_FLAGS} /bigobj" )
    ENDIF( CMAKE_CL_64 )
    # H3D API is now to big to link incrementally with vc8.
    SET( H3DAPI_LINK_FLAGS_DEBUG "${H3DAPI_LINK_FLAGS_DEBUG} /INCREMENTAL:NO" )
    # Needed to delayload OpenHapticsRenderer. They must have changed the way symbols are imported between versions of visual studio.
    IF(OPENHAPTICS_FOUND)
      SET_SOURCE_FILES_PROPERTIES( "${H3DAPI_SOURCE_DIR}/../src/MagneticSurface.cpp"
                                   "${H3DAPI_SOURCE_DIR}/../src/OpenHapticsSurface.cpp"
                                   "${H3DAPI_SOURCE_DIR}/../src/X3DGeometryNode.cpp"
                                   PROPERTIES COMPILE_FLAGS "/Od" )
    ENDIF(OPENHAPTICS_FOUND)
  ELSEIF(MSVC90)
    SET_TARGET_PROPERTIES( H3DAPI PROPERTIES OUTPUT_NAME H3DAPI_vc9 )
    SET( TEMP_OUTPUT_POSTFIX _vc9 )
    # remove some warnings about deprecated functions.
    SET( H3DAPI_COMPILE_FLAGS "${H3DAPI_COMPILE_FLAGS} -D_CRT_SECURE_NO_DEPRECATE /Zc:wchar_t- /MP" )
    # if we are using 64 bit compiler some object files get too big so we need
    # to add the /bigobj flag
    IF( CMAKE_CL_64 )
      SET( H3DAPI_COMPILE_FLAGS "${H3DAPI_COMPILE_FLAGS} /bigobj" )
    ENDIF( CMAKE_CL_64 )
    # H3D API is now too big to link incrementally with vc9.
    SET( H3DAPI_LINK_FLAGS_DEBUG "${H3DAPI_LINK_FLAGS_DEBUG} /NODEFAULTLIB:msvcrt /INCREMENTAL:NO" )
    # Needed to delayload OpenHapticsRenderer. They must have changed the way symbols are imported between versions of visual studio.
    IF(OPENHAPTICS_FOUND)
      SET_SOURCE_FILES_PROPERTIES( "${H3DAPI_SOURCE_DIR}/../src/MagneticSurface.cpp"
                                   "${H3DAPI_SOURCE_DIR}/../src/OpenHapticsSurface.cpp"
                                   "${H3DAPI_SOURCE_DIR}/../src/X3DGeometryNode.cpp"
                                   PROPERTIES COMPILE_FLAGS "/Od" )
    ENDIF(OPENHAPTICS_FOUND)

  ELSEIF(MSVC10)
    SET_TARGET_PROPERTIES( H3DAPI PROPERTIES OUTPUT_NAME H3DAPI_vc10 )
    SET( TEMP_OUTPUT_POSTFIX _vc10 )
    # remove some warnings about deprecated functions.
    SET( H3DAPI_COMPILE_FLAGS "${H3DAPI_COMPILE_FLAGS} -D_CRT_SECURE_NO_DEPRECATE /Zc:wchar_t- /MP" )
    # if we are using 64 bit compiler some object files get too big so we need
    # to add the /bigobj flag
    IF( CMAKE_CL_64 )
      SET( H3DAPI_COMPILE_FLAGS "${H3DAPI_COMPILE_FLAGS} /bigobj" )
    ENDIF( CMAKE_CL_64 )
    # H3D API is now to big to link incrementally with vc10.
    SET( H3DAPI_LINK_FLAGS_DEBUG "${H3DAPI_LINK_FLAGS_DEBUG} /NODEFAULTLIB:msvcrt /INCREMENTAL:NO" )
    # Needed to delayload OpenHapticsRenderer. They must have changed the way symbols are imported between versions of visual studio.
    IF(OPENHAPTICS_FOUND)
      SET_SOURCE_FILES_PROPERTIES( "${H3DAPI_SOURCE_DIR}/../src/MagneticSurface.cpp"
                                   "${H3DAPI_SOURCE_DIR}/../src/OpenHapticsSurface.cpp"
                                   "${H3DAPI_SOURCE_DIR}/../src/X3DGeometryNode.cpp"
                                   PROPERTIES COMPILE_FLAGS "/Od" )
    ENDIF(OPENHAPTICS_FOUND)
  ENDIF(MSVC70 OR MSVC71)

  # Set delayload properties of H3D API.
  SET( H3DAPI_LINK_FLAGS "${H3DAPI_LINK_FLAGS} /DELAYLOAD:\"cg.dll\" /DELAYLOAD:\"cgGL.dll\" /DELAYLOAD:\"FreeImage.dll\" /DELAYLOAD:\"OpenAL32.dll\" /DELAYLOAD:\"vorbisfile.dll\" /DELAYLOAD:\"VirtualHandDevice.dll\" /DELAYLOAD:\"VirtualHandCore.dll\"")

  IF(HAPTIK_FOUND)
    SET( H3DAPI_LINK_FLAGS "${H3DAPI_LINK_FLAGS} /DELAYLOAD:\"Haptik.Library.dll\"" )
  ENDIF(HAPTIK_FOUND)

  SET_TARGET_PROPERTIES( H3DAPI
                         PROPERTIES
                         LINK_FLAGS "${H3DAPI_LINK_FLAGS}" )
  
  IF(CHAI3D_FOUND)
    SET( H3DAPI_LINK_FLAGS_DEBUG "${H3DAPI_LINK_FLAGS_DEBUG} /DELAYLOAD:\"Chai3DRenderer${TEMP_OUTPUT_POSTFIX}_d.dll\"" )
    SET( H3DAPI_LINK_FLAGS_RELEASE "${H3DAPI_LINK_FLAGS_RELEASE} /DELAYLOAD:\"Chai3DRenderer${TEMP_OUTPUT_POSTFIX}.dll\"" )
  ENDIF(CHAI3D_FOUND)
  
  IF(OPENHAPTICS_FOUND)
    SET( H3DAPI_LINK_FLAGS_DEBUG "${H3DAPI_LINK_FLAGS_DEBUG} /DELAYLOAD:\"OpenHapticsRenderer${TEMP_OUTPUT_POSTFIX}_d.dll\"" )
    SET( H3DAPI_LINK_FLAGS_RELEASE "${H3DAPI_LINK_FLAGS_RELEASE} /DELAYLOAD:\"OpenHapticsRenderer${TEMP_OUTPUT_POSTFIX}.dll\"" )
  ENDIF(OPENHAPTICS_FOUND)

  SET_TARGET_PROPERTIES( H3DAPI
                         PROPERTIES  
                         LINK_FLAGS_DEBUG "${H3DAPI_LINK_FLAGS_DEBUG}"
                         LINK_FLAGS_RELEASE "${H3DAPI_LINK_FLAGS_RELEASE}"
                         LINK_FLAGS_RELWITHDEBINFO "${H3DAPI_LINK_FLAGS_RELEASE}"
                         LINK_FLAGS_MINSIZEREL "${H3DAPI_LINK_FLAGS_RELEASE}" )
ENDIF( WIN32 )

# Set compile properties for H3DAPI.
SET_TARGET_PROPERTIES( H3DAPI PROPERTIES COMPILE_FLAGS "${H3DAPI_COMPILE_FLAGS}" )

# autogenerate H3DAPI.h depending on the libraries available.
CONFIGURE_FILE( ${H3DAPI_SOURCE_DIR}/../include/H3D/H3DApi.cmake ${H3DAPI_SOURCE_DIR}/../include/H3D/H3DApi.h )

# Intall directories.
INSTALL( TARGETS H3DAPI
         LIBRARY DESTINATION lib
         RUNTIME DESTINATION bin
         ARCHIVE DESTINATION lib )

IF( NOT WIN32 )
  # Install include files.
  INSTALL( FILES ${H3DAPI_HEADERS}
           DESTINATION include/H3D )
ENDIF( NOT WIN32 )

IF( WIN32 )
  # H3DAPI is dependent on H3DUtil and HAPI
  ADD_DEPENDENCIES( H3DAPI H3DUtil )
  ADD_DEPENDENCIES( H3DAPI HAPI )

  #Only add dependencies if OpenHaptics exists.
  IF(OPENHAPTICS_FOUND)
    ADD_DEPENDENCIES( H3DAPI OpenHapticsRenderer )
  ENDIF(OPENHAPTICS_FOUND)

  #Only add dependencies if Chai3d exists.
  IF(CHAI3D_FOUND)
    ADD_DEPENDENCIES( H3DAPI Chai3DRenderer )
  ENDIF(CHAI3D_FOUND)
ENDIF( WIN32 )

IF( NOT DEFINED H3DAPI_LOADER_PROJECTS )
  SET( H3DAPI_LOADER_PROJECTS "NO" CACHE BOOL "If set to YES H3DLoad and H3DViewer will be included in the build." )
ENDIF( NOT DEFINED H3DAPI_LOADER_PROJECTS )

IF( H3DAPI_LOADER_PROJECTS )
  # This is done to avoid placing everything directly in "build" catalogue if some other catalogue is chosen.
  STRING( REPLACE ${H3DAPI_SOURCE_DIR} ${H3DAPI_SOURCE_DIR}/../H3DLoad/build H3DAPI_LOADER_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR} )
  ADD_SUBDIRECTORY( ${H3DAPI_SOURCE_DIR}/../H3DLoad/build
                    ${H3DAPI_LOADER_BINARY_DIR} )
  ADD_DEPENDENCIES( H3DLoad H3DAPI )
  
  STRING( REPLACE ${H3DAPI_SOURCE_DIR} ${H3DAPI_SOURCE_DIR}/../H3DViewer/build H3DAPI_LOADER_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR} )
  ADD_SUBDIRECTORY( ${H3DAPI_SOURCE_DIR}/../H3DViewer/build
                    ${H3DAPI_LOADER_BINARY_DIR} )
  ADD_DEPENDENCIES( H3DViewer H3DAPI )  
ENDIF( H3DAPI_LOADER_PROJECTS )

IF( NOT DEFINED H3DAPI_EXAMPLE_PROJECTS )
  SET( H3DAPI_EXAMPLE_PROJECTS "NO" CACHE BOOL "If set to YES various example projects build on H3DAPI will be included in the build." )
ENDIF( NOT DEFINED H3DAPI_EXAMPLE_PROJECTS )

IF( H3DAPI_EXAMPLE_PROJECTS )
  # This is done to avoid placing everything directly in "build" catalogue if some other catalogue is chosen.
  STRING( REPLACE ${H3DAPI_SOURCE_DIR} ${H3DAPI_SOURCE_DIR}/../examples/manualExamples/C++ H3DAPI_EXAMPLES_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR} )
  ADD_SUBDIRECTORY( ${H3DAPI_SOURCE_DIR}/../examples/manualExamples/C++
                    ${H3DAPI_EXAMPLES_BINARY_DIR} )
  ADD_DEPENDENCIES( Sphere_X3D H3DAPI )
  ADD_DEPENDENCIES( Spheres_X3D H3DAPI )
  
  STRING( REPLACE ${H3DAPI_SOURCE_DIR} ${H3DAPI_SOURCE_DIR}/../examples/Particledemo/build H3DAPI_EXAMPLES_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR} )
  ADD_SUBDIRECTORY( ${H3DAPI_SOURCE_DIR}/../examples/Particledemo/build
                    ${H3DAPI_EXAMPLES_BINARY_DIR} )

  STRING( REPLACE ${H3DAPI_SOURCE_DIR} ${H3DAPI_SOURCE_DIR}/../examples/ThreadExample H3DAPI_EXAMPLES_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR} )
  ADD_SUBDIRECTORY( ${H3DAPI_SOURCE_DIR}/../examples/ThreadExample
                    ${H3DAPI_EXAMPLES_BINARY_DIR} )
  IF( DEFINED ParticleDemo_SOURCE_DIR )
    ADD_DEPENDENCIES( ParticleDemo H3DAPI )
  ENDIF( DEFINED ParticleDemo_SOURCE_DIR )
  IF( DEFINED ThreadExample_SOURCE_DIR )
    ADD_DEPENDENCIES( ThreadExample H3DAPI )
  ENDIF( DEFINED ThreadExample_SOURCE_DIR )
ENDIF( H3DAPI_EXAMPLE_PROJECTS )

