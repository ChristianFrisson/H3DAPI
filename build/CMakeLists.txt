cmake_minimum_required(VERSION 2.6.0)
PROJECT (H3DAPI)

# Add a cache variable GENERATE_CPACK_PROJECT to have the choice of generating a project
# for packaging H3DAPI. Default is NO since most people will not use this.
IF( NOT DEFINED GENERATE_CPACK_PROJECT )
  SET( GENERATE_CPACK_PROJECT "NO" CACHE BOOL "Decides if a cpack project should be generated. The project in the first loaded CMakeLists will configure CPack." )
  MARK_AS_ADVANCED(GENERATE_CPACK_PROJECT)
ENDIF( NOT DEFINED GENERATE_CPACK_PROJECT )

# Include HAPI in the build.
IF( NOT GENERATE_CPACK_PROJECT OR WIN32 )
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
  IF(OPENHAPTICS_INCLUDE_DIR AND HD_LIBRARY AND HL_LIBRARY AND HDU_LIBRARY)
    SET( OPENHAPTICS_FOUND 1 )
  ENDIF(OPENHAPTICS_INCLUDE_DIR AND HD_LIBRARY AND HL_LIBRARY AND HDU_LIBRARY)
ENDIF( NOT GENERATE_CPACK_PROJECT OR WIN32 )

# Version of H3D API.
SET( H3DAPI_MAJOR_VERSION 2 )
SET( H3DAPI_MINOR_VERSION 0 )
SET( H3DAPI_BUILD_VERSION 0 )

# Set here already in case a special NSIS template needs to be used by cpack.
SET(CMAKE_MODULE_PATH ${H3DAPI_SOURCE_DIR}/modules )
INCLUDE( StripAndAddLibraryDirectories )

# If cpack should be configured.
IF( GENERATE_CPACK_PROJECT )
  # Set information properties about the project to install.
  SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "H3DAPI. A cross platform, haptics device independent, X3D based API for 3D graphics and haptics.")
  SET(CPACK_PACKAGE_VENDOR "SenseGraphics AB")
  SET(CPACK_PACKAGE_CONTACT "support@sensegraphics.com" )
  SET(CPACK_PACKAGE_DESCRIPTION_FILE "${H3DAPI_SOURCE_DIR}/../ReadMe.txt")
  SET(CPACK_RESOURCE_FILE_LICENSE "${H3DAPI_SOURCE_DIR}/../LICENSE")

  # Project to install.
  SET(CPACK_INSTALL_CMAKE_PROJECTS "${CMAKE_CURRENT_BINARY_DIR};H3DAPI;ALL;/")

  # Installation directory for package.
  SET(CPACK_PACKAGE_INSTALL_DIRECTORY "H3D" )

  # Our project depends on these debian packages for Linux.
  SET(DEBIAN_PACKAGE_DEPENDS "libcurl3, libfreetype6-dev, ftgl-dev, python2.4-dev, libaudiofile0, libvorbis-dev, libopenal-dev, zlib1g-dev, libxerces27-dev, libfreeimage-dev, hapi(>= 1.0.0), h3dutil(>=1.0.0)" )	      


  # If H3DLoad should be added to the package. TODO: Only for Windows?
  SET( H3DLoad_cmake_install "" CACHE PATH "Needs to be set if H3DLoad is desired to be packed in the install. Set to location of cmake_install.cmake for H3DLoad." )
  MARK_AS_ADVANCED(H3DLoad_cmake_install)
  IF( H3DLoad_cmake_install )
    SET(CPACK_INSTALL_CMAKE_PROJECTS ${CPACK_INSTALL_CMAKE_PROJECTS}
                                     "${H3DLoad_cmake_install};H3DLoad;ALL;/" )
    IF( WIN32 )
      SET( CPACK_ADD_H3DLOAD_DEMOS_LINKS "ON" )
    ENDIF( WIN32 )
  ENDIF( H3DLoad_cmake_install )

  # If H3DViewer should be added to package. TODO: Only for Windows?
  SET( H3DViewer_cmake_install "" CACHE PATH "Needs to be set if H3DViewer is desired to be packed in the install.  Set to location of cmake_install.cmake for H3DViewer." )
  MARK_AS_ADVANCED(H3DViewer_cmake_install)
  IF( H3DViewer_cmake_install )
    SET( CPACK_INSTALL_CMAKE_PROJECTS ${CPACK_INSTALL_CMAKE_PROJECTS}
                                      "${H3DViewer_cmake_install};H3DViewer;ALL;/" )
    IF( WIN32 )
      SET( CPACK_PACKAGE_EXECUTABLES ${CPACK_PACKAGE_EXECUTABLES}
                                     "H3DViewer;H3DViewer" )
      SET(WXWINDOWS_USE_GL 1)
      FIND_PACKAGE(wxWidgetsWin)
      IF(wxWidgets_FOUND)
        SET( CPACK_INSTALLED_DIRECTORIES ${CPACK_INSTALLED_DIRECTORIES}
                                         "${wxWidgets_INCLUDE_DIR}/wx;External/include/wx" )
      ENDIF(wxWidgets_FOUND)
    ENDIF( WIN32 )
  ENDIF( H3DViewer_cmake_install )

  #If particleDemo should be added to package. TODO: Only for Windows?
  SET( ParticleDemo_cmake_install "" CACHE PATH "Needs to be set if ParticleDemo is desired to be packed in the install.  Set to location of cmake_install.cmake for ParticleDemo." )
  MARK_AS_ADVANCED(ParticleDemo_cmake_install)
  IF( ParticleDemo_cmake_install )
    SET( CPACK_INSTALL_CMAKE_PROJECTS ${CPACK_INSTALL_CMAKE_PROJECTS}
                                      "${ParticleDemo_cmake_install};ParticleDemo;ALL;/" )
    IF( WIN32 AND NOT H3DViewer_cmake_install )
      FIND_PACKAGE(wxWidgetsWin)
      IF(wxWidgets_FOUND)
        SET( CPACK_INSTALLED_DIRECTORIES ${CPACK_INSTALLED_DIRECTORIES}
                                         "${wxWidgets_INCLUDE_DIR}/wx;External/include/wx" )
      ENDIF(wxWidgets_FOUND)
    ENDIF( WIN32 AND NOT H3DViewer_cmake_install )
  ELSE( ParticleDemo_cmake_install )
    SET(CPACK_IGNORE_FILES /examples/Particledemo/ )
  ENDIF( ParticleDemo_cmake_install )

  SET(CPACK_PACKAGE_VERSION_MAJOR ${H3DAPI_MAJOR_VERSION})
  SET(CPACK_PACKAGE_VERSION_MINOR ${H3DAPI_MINOR_VERSION})
  SET(CPACK_PACKAGE_VERSION_PATCH ${H3DAPI_BUILD_VERSION})
  
  # TODO: fix different for windows and unix if needed.
  IF(WIN32 AND NOT UNIX)
    # Pattern in folders and files to ignore when adding files to installation package.
    SET(CPACK_IGNORE_FILES ${CPACK_IGNORE_FILES}
                           /\\\\.svn/
                           \\\\.obj$
                           \\\\.ncb$
                           \\\\.log$
                          \\\\.suo$
                          \\\\.zip$
                          \\\\.dir/
                          \\\\.user$
                          \\\\.cv$
                          "/Debug(.)*/"
                          "/debug(.)*/"
                          /Release
                          /release
                          /linux
                          /build/win32/
                          /build/vc8
                          /build/vc7
                          /osx
                          /H3DAPI/Util/
                          /fparser\\\\.lib$
                          /teem\\\\.lib$
                          /wxbase28_net\\\\.lib$
                          /wxbase28_odbc\\\\.lib$
                          /wxbase28_xml\\\\.lib$
                          /wxbase28_net\\\\.lib$
                          /wxbase28_odbc\\\\.lib$
                          /wxbase28_xml\\\\.lib$
                          /wxexpat\\\\.lib$
                          /wxjpeg\\\\.lib$
                          /wxmsw28_aui\\\\.lib$
                          /wxmsw28_dbgrid\\\\.lib$
                          /wxmsw28_html\\\\.lib$
                          /wxmsw28_media\\\\.lib$
                          /wxmsw28_qa\\\\.lib$
                          /wxmsw28_richtext\\\\.lib$
                          /wxmsw28_xrc\\\\.lib$
                          /wxpng\\\\.lib$
                          /wxregex\\\\.lib$
                          /wxtiff\\\\.lib$
                          /wxzlib\\\\.lib$
                          /bz2\\\\.lib$
                          /z\\\\.lib$
                          /png\\\\.lib$
                          /pthreadGC2\\\\.dll$
                          /pthreadGCE2\\\\.dll$
                          /pthreadVSE2\\\\.dll$
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
                          /plane/bobcat_nh\\\\.wrl$
                          /plane/bobcat_orig\\\\.x3d$
                          /HAPI/doc/ )
    
    # Directories to install, and how they should be named. Non-existing folders will be ignored when packing.
    SET(CPACK_INSTALLED_DIRECTORIES "${H3DAPI_SOURCE_DIR}/../../H3DAPI;H3DAPI"
                                    "${H3DAPI_SOURCE_DIR}/../../HAPI;HAPI"
                                    "${H3DAPI_SOURCE_DIR}/../../H3DUtil;H3DUtil"
                                    "${H3DAPI_SOURCE_DIR}/../../doc;doc" )

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
      STRING( REPLACE "/" "\\\\" TEMP_PythonInstallMSI ${PythonInstallMSI} )
      SET( CPACK_NSIS_EXTRA_INSTALL_COMMANDS ${CPACK_NSIS_EXTRA_INSTALL_COMMANDS}
                                             "   Code to install Python\\n   ReadRegStr $0 HKLM SOFTWARE\\\\Python\\\\PythonCore\\\\2.4\\\\InstallPath \\\"\\\"\\n   StrCmp $0 \\\"\\\" 0 +5\\n  file ${TEMP_PythonInstallMSI}\\n  ExecWait '\\\"msiexec\\\" /i \\\"$INSTDIR\\\\python-2.4.4.msi\\\"'\\n  delete $INSTDIR\\\\python-2.4.4.msi\\n   ReadRegStr $0 HKLM SOFTWARE\\\\Python\\\\PythonCore\\\\2.4\\\\InstallPath \\\"\\\"\\n" )
    ENDIF( PythonInstallMSI )
    
    # Install OpenAL.
    SET( OpenAlInstallExe "" CACHE FILEPATH "Needs to be set to add openal installation to the package." )
    MARK_AS_ADVANCED(OpenAlInstallExe)
    IF( OpenAlInstallExe )
      STRING( REPLACE "/" "\\\\" TEMP_OpenAlInstallExe ${OpenAlInstallExe} )
      SET( CPACK_NSIS_EXTRA_INSTALL_COMMANDS ${CPACK_NSIS_EXTRA_INSTALL_COMMANDS}
                                             "   Code to install OPENAL\\n   file ${TEMP_OpenAlInstallExe}\\n  ExecWait \\\"$INSTDIR\\\\OpenALwEAX.exe\\\"\\n  delete $INSTDIR\\\\OpenALwEAX.exe\\n" )
    ENDIF( OpenAlInstallExe )

    # Do not modify path since this is done by the NSIS template.
    SET( CPACK_NSIS_MODIFY_PATH "ON" )
  ENDIF(WIN32 AND NOT UNIX)
ENDIF( GENERATE_CPACK_PROJECT )

# Version of project.
SET( H3DAPI_FULL_VERSION
${H3DAPI_MAJOR_VERSION}.${H3DAPI_MINOR_VERSION}.${H3DAPI_BUILD_VERSION} ) 

# Add all sources
AUX_SOURCE_DIRECTORY(${H3DAPI_SOURCE_DIR}/../src H3DAPI_SRCS)

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
  SET(requiredLibs ${requiredLibs} H3DUtil HAPI)
ELSEIF( MSVC70 OR MSVC71 )
  # On other versions of visual studio dependency setting is enough to link correctly.
  SET(requiredLibs ${requiredLibs} H3DUtil HAPI)

  IF(CHAI3D_FOUND)
    SET(requiredLibs ${requiredLibs} Chai3DRenderer)
  ENDIF(CHAI3D_FOUND)

  IF(OPENHAPTICS_FOUND)
    SET(requiredLibs ${requiredLibs} OpenHapticsRenderer)
  ENDIF(OPENHAPTICS_FOUND)
ENDIF(NOT WIN32)

# Include directories needed.
INCLUDE_DIRECTORIES( ../include
${H3DAPI_SOURCE_DIR}/../../H3DUtil/include
${H3DAPI_SOURCE_DIR}/../../HAPI/include )

# Possible extra include directories.
IF(CHAI3D_FOUND)
  INCLUDE_DIRECTORIES( ${H3DAPI_SOURCE_DIR}/../../HAPI/Chai3DRenderer/include )
ENDIF(CHAI3D_FOUND)

IF(OPENHAPTICS_FOUND)
  INCLUDE_DIRECTORIES( ${H3DAPI_SOURCE_DIR}/../../HAPI/OpenHapticsRenderer/include )
ENDIF(OPENHAPTICS_FOUND)

# Extra libs to link against. TODO: Only for MSVC?
IF(WIN32)
  SET(requiredLibs ${requiredLibs} winmm.lib DelayImp.lib )
ENDIF(WIN32)

# Find pthread library. Required to compile.
FIND_PACKAGE(PTHREAD REQUIRED)
IF(PTHREAD_FOUND)
  INCLUDE_DIRECTORIES( ${PTHREAD_INCLUDE_DIR} ) 
  IF(WIN32)
    STRIP_AND_ADD_LIBRARY_DIRECTORIES( ${PTHREAD_LIBRARIES})
  ELSE(WIN32)
    SET(requiredLibs ${requiredLibs} ${PTHREAD_LIBRARIES} )
  ENDIF(WIN32)
ENDIF(PTHREAD_FOUND)

# Find OpenGL library. Required to compile.
FIND_PACKAGE(OpenGL REQUIRED)
IF(OPENGL_FOUND)
  SET(HAVE_OPENGL 1)
  INCLUDE_DIRECTORIES( ${OPENGL_INCLUDE_DIR} )
  IF(WIN32)
    STRIP_AND_ADD_LIBRARY_DIRECTORIES( ${OPENGL_LIBRARIES} )
  ELSE(WIN32)
    SET(requiredLibs ${requiredLibs} ${OPENGL_LIBRARIES} )
  ENDIF(WIN32)
ENDIF(OPENGL_FOUND)

# Find Glew library. Required to compile.
FIND_PACKAGE(GLEW REQUIRED)
IF(GLEW_FOUND)
  INCLUDE_DIRECTORIES( ${GLEW_INCLUDE_DIR} ) 
  IF(WIN32)
    STRIP_AND_ADD_LIBRARY_DIRECTORIES( ${GLEW_LIBRARIES} )
  ELSE(WIN32)
    SET(requiredLibs ${requiredLibs} ${GLEW_LIBRARIES} )
  ENDIF(WIN32)
ENDIF(GLEW_FOUND)

# Optional extra library. Makes is possible to used GLUT windows handling with H3D API.
FIND_PACKAGE(GLUT)
IF(GLUT_FOUND)
  INCLUDE_DIRECTORIES( ${GLUT_INCLUDE_DIR} ) 
  IF(WIN32)
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
  IF(WIN32)
    STRIP_AND_ADD_LIBRARY_DIRECTORIES( ${XERCES_LIBRARIES} )
  ELSE(WIN32)
    SET(optionalLibs ${optionalLibs} ${XERCES_LIBRARIES} )
  ENDIF(WIN32)
ENDIF(XERCES_FOUND)

# Optional extra library. Used in H3D API to resolved urls.
FIND_PACKAGE(H3DCURL)
IF(CURL_FOUND)
  SET(HAVE_LIBCURL 1)
  INCLUDE_DIRECTORIES( ${CURL_INCLUDE_DIR} ) 
  IF(WIN32)
    STRIP_AND_ADD_LIBRARY_DIRECTORIES( ${CURL_LIBRARIES} )
  ELSE(WIN32)
    SET(optionalLibs ${optionalLibs} ${CURL_LIBRARIES} )
  ENDIF(WIN32)
ENDIF(CURL_FOUND)

# Optional extra library. Needed to support shading using the CG shader language from NVidia.
FIND_PACKAGE(NvidiaCG)
IF(NVIDIACG_FOUND)
  SET(HAVE_CG 1)
  INCLUDE_DIRECTORIES( ${NVIDIACG_INCLUDE_DIR} ) 
  IF(WIN32)
    STRIP_AND_ADD_LIBRARY_DIRECTORIES( ${NVIDIACG_LIBRARIES} )
  ELSE(WIN32)
    SET(optionalLibs ${optionalLibs} ${NVIDIACG_LIBRARIES} )
  ENDIF(WIN32)
ENDIF(NVIDIACG_FOUND)

# Optional extra library. Used to read compressed files.
FIND_PACKAGE(H3DZLIB)
IF(ZLIB_FOUND)
  SET(HAVE_ZLIB 1)
  INCLUDE_DIRECTORIES( ${ZLIB_INCLUDE_DIR} )
  IF(WIN32)
    STRIP_AND_ADD_LIBRARY_DIRECTORIES( ${ZLIB_LIBRARIES})
  ELSE(WIN32)
    SET(optionalLibs ${optionalLibs} ${ZLIB_LIBRARIES} )
  ENDIF(WIN32)
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
  IF(WIN32)
    STRIP_AND_ADD_LIBRARY_DIRECTORIES( ${PYTHON_LIBRARIES})
  ELSE(WIN32)
    SET(optionalLibs ${optionalLibs} ${PYTHON_LIBRARIES} )
  ENDIF(WIN32)
ENDIF(PYTHON_FOUND)

# Optional extra library. Used for sound in H3D API.
FIND_PACKAGE(H3DOpenAL)
IF(OPENAL_FOUND)
  SET(HAVE_OPENAL 1)
  INCLUDE_DIRECTORIES( ${OPENAL_INCLUDE_DIR} )
  IF(WIN32)
    STRIP_AND_ADD_LIBRARY_DIRECTORIES( ${OPENAL_LIBRARIES})
  ELSE(WIN32)
    SET(optionalLibs ${optionalLibs} ${OPENAL_LIBRARIES} )
  ENDIF(WIN32)
ENDIF(OPENAL_FOUND)

# Optional extra library. Used to read sound files.
FIND_PACKAGE(Vorbis)
IF(VORBIS_FOUND)
  SET(HAVE_LIBVORBIS 1)
  INCLUDE_DIRECTORIES( ${VORBIS_INCLUDE_DIR} ) 
  IF(WIN32)
    STRIP_AND_ADD_LIBRARY_DIRECTORIES( ${VORBIS_LIBRARIES})
  ELSE(WIN32)
    SET(optionalLibs ${optionalLibs} ${VORBIS_LIBRARIES} )
  ENDIF(WIN32)
ENDIF(VORBIS_FOUND)

# Optional extra library. Used to read sound files.
FIND_PACKAGE(Audiofile)
IF(AUDIOFILE_FOUND)
  SET(HAVE_LIBAUDIOFILE 1)
  INCLUDE_DIRECTORIES( ${AUDIOFILE_INCLUDE_DIR} ) 
  IF(WIN32)
    STRIP_AND_ADD_LIBRARY_DIRECTORIES( ${AUDIOFILE_LIBRARIES})
  ELSE(WIN32)
    SET(optionalLibs ${optionalLibs} ${AUDIOFILE_LIBRARIES} )
  ENDIF(WIN32)
ENDIF(AUDIOFILE_FOUND)

# Optional extra library. Used to handle fonts in H3D API.
FIND_PACKAGE(FreeType)
IF(FREETYPE_FOUND)
  SET(HAVE_FREETYPE 1)
  INCLUDE_DIRECTORIES( ${FREETYPE_INCLUDE_DIR} ) 
  IF(WIN32)
    STRIP_AND_ADD_LIBRARY_DIRECTORIES( ${FREETYPE_LIBRARIES})
  ELSE(WIN32)
    SET(optionalLibs ${optionalLibs} ${FREETYPE_LIBRARIES} )
  ENDIF(WIN32)
ENDIF(FREETYPE_FOUND)

# Optional extra library. Adds support to use devices by 3dxware. See www.3dconnexion.com.
FIND_PACKAGE(3DXWARE)
IF(3DXWARE_FOUND)
  SET(HAVE_3DXWARE 1)
  INCLUDE_DIRECTORIES( ${3DXWARE_INCLUDE_DIR} ) 
  IF(WIN32)
    STRIP_AND_ADD_LIBRARY_DIRECTORIES( ${3DXWARE_LIBRARIES})
  ELSE(WIN32)
    SET(optionalLibs ${optionalLibs} ${3DXWARE_LIBRARIES} )
  ENDIF(WIN32)
ENDIF(3DXWARE_FOUND)

# Optional extra library. Used to render fonts in OpenGL.
FIND_PACKAGE(FTGL)
IF(FTGL_FOUND)
  SET(HAVE_FTGL 1)
  INCLUDE_DIRECTORIES( ${FTGL_INCLUDE_DIR} ) 
  IF(WIN32)
    STRIP_AND_ADD_LIBRARY_DIRECTORIES( ${FTGL_LIBRARIES})
  ELSE(WIN32)
    SET(optionalLibs ${optionalLibs} ${FTGL_LIBRARIES} )
  ENDIF(WIN32)
ENDIF(FTGL_FOUND)

# Optional extra library. Used to read images.
FIND_PACKAGE(FreeImage)
IF(FREEIMAGE_FOUND)
  SET(HAVE_FREEIMAGE 1)
  INCLUDE_DIRECTORIES( ${FREEIMAGE_INCLUDE_DIR} ) 
  IF(WIN32)
    STRIP_AND_ADD_LIBRARY_DIRECTORIES( ${FREEIMAGE_LIBRARIES})
  ELSE(WIN32)
    SET(optionalLibs ${optionalLibs} ${FREEIMAGE_LIBRARIES} )
  ENDIF(WIN32)
ENDIF(FREEIMAGE_FOUND)

IF(WIN32)
  # Optional extra library. Used to render movies as textures.
  FIND_PACKAGE(DirectShow)
  IF(DIRECTSHOW_FOUND)
    SET(HAVE_DSHOW 1)
    INCLUDE_DIRECTORIES( ${DIRECTSHOW_INCLUDE_DIR} ) 
    STRIP_AND_ADD_LIBRARY_DIRECTORIES( ${DIRECTSHOW_LIBRARIES})
  ENDIF(DIRECTSHOW_FOUND)
ENDIF(WIN32)

IF(NOT WIN32 AND NOT APPLE)
  FIND_PACKAGE(FontConfig)
  IF(FONTCONFIG_FOUND)
    SET(HAVE_FONTCONFIG 1)
    INCLUDE_DIRECTORIES( ${FONTCONFIG_INCLUDE_DIR} ) 
    SET(optionalLibs ${optionalLibs} ${FONTCONFIG_LIBRARIES} )
  ENDIF(FONTCONFIG_FOUND)
ENDIF(NOT WIN32 AND NOT APPLE) 

# If cpack project should be generated and windows is the system on which to generate
# a number of extra directories needs to be added to be installed.
IF( GENERATE_CPACK_PROJECT )
  IF( WIN32 )
    IF( PTHREAD_FOUND )
      SET(CPACK_INSTALLED_DIRECTORIES ${CPACK_INSTALLED_DIRECTORIES}
                                      "${PTHREAD_INCLUDE_DIR};External/include/pthread" )
    ENDIF( PTHREAD_FOUND )

    IF( OPENGL_FOUND AND OPENGL_INCLUDE_DIR )
      SET(CPACK_INSTALLED_DIRECTORIES ${CPACK_INSTALLED_DIRECTORIES}
                                      "${OPENGL_INCLUDE_DIR}/GL;External/include/GL" )
    ENDIF( OPENGL_FOUND AND OPENGL_INCLUDE_DIR )

    IF( GLUT_FOUND )
      SET(CPACK_INSTALLED_DIRECTORIES ${CPACK_INSTALLED_DIRECTORIES}
                                      "${GLUT_INCLUDE_DIR}/GL;External/include/GL" )
    ENDIF( GLUT_FOUND )

    IF( GLEW_FOUND )
      SET(CPACK_INSTALLED_DIRECTORIES ${CPACK_INSTALLED_DIRECTORIES}
                                      "${GLEW_INCLUDE_DIR}/GL;External/include/GL" )
    ENDIF( GLEW_FOUND )

    IF( XERCES_FOUND )
      SET(CPACK_INSTALLED_DIRECTORIES ${CPACK_INSTALLED_DIRECTORIES}
                                      "${XERCES_INCLUDE_DIR}/..;External/include/xerces-c-src_2_7_0" )
    ENDIF( XERCES_FOUND )

    IF( CURL_FOUND )
      SET(CPACK_INSTALLED_DIRECTORIES ${CPACK_INSTALLED_DIRECTORIES}
                                      "${CURL_INCLUDE_DIR}/curl;External/include/curl" )
    ENDIF( CURL_FOUND )

    IF( NVIDIACG_FOUND )
      SET(CPACK_INSTALLED_DIRECTORIES ${CPACK_INSTALLED_DIRECTORIES}
                                      "${NVIDIACG_INCLUDE_DIR};External/include/Cg" )
    ENDIF( NVIDIACG_FOUND )

    IF( ZLIB_FOUND )
      SET(CPACK_INSTALLED_DIRECTORIES ${CPACK_INSTALLED_DIRECTORIES}
                                      "${ZLIB_INCLUDE_DIR};External/include/zlib" )
    ENDIF( ZLIB_FOUND )

    IF( OPENAL_FOUND )
      SET(CPACK_INSTALLED_DIRECTORIES ${CPACK_INSTALLED_DIRECTORIES}
                                      "${OPENAL_INCLUDE_DIR};External/include/OpenAL" )
    ENDIF( OPENAL_FOUND )

    IF( VORBIS_FOUND )
      FOREACH(TEMP_LIST_ITEM ${VORBIS_INCLUDE_DIR})
      SET(CPACK_INSTALLED_DIRECTORIES ${CPACK_INSTALLED_DIRECTORIES}
                                      "${TEMP_LIST_ITEM}/..;External/include/vorbis" )
      ENDFOREACH(TEMP_LIST_ITEM ${ARGV})
    ENDIF( VORBIS_FOUND )

    IF( AUDIOFILE_FOUND )
      SET(CPACK_INSTALLED_DIRECTORIES ${CPACK_INSTALLED_DIRECTORIES}
                                      "${AUDIOFILE_INCLUDE_DIR};External/include/libaudiofile" )
    ENDIF( AUDIOFILE_FOUND )

    IF( FREETYPE_FOUND )
      SET(CPACK_INSTALLED_DIRECTORIES ${CPACK_INSTALLED_DIRECTORIES}
                                      "${FREETYPE_INCLUDE_DIR}/..;External/include/freetype" )
    ENDIF( FREETYPE_FOUND )

    IF( 3DXWARE_FOUND )
      SET(CPACK_INSTALLED_DIRECTORIES ${CPACK_INSTALLED_DIRECTORIES}
                                      "${3DXWARE_INCLUDE_DIR}/..;External/include/3dconnexion" )
    ENDIF( 3DXWARE_FOUND )

    IF( FTGL_FOUND )
      SET(CPACK_INSTALLED_DIRECTORIES ${CPACK_INSTALLED_DIRECTORIES}
                                      "${FTGL_INCLUDE_DIR}/FTGL;External/include/FTGL" )
    ENDIF( FTGL_FOUND )

    IF( FREEIMAGE_FOUND )
      SET(CPACK_INSTALLED_DIRECTORIES ${CPACK_INSTALLED_DIRECTORIES}
                                      "${FREEIMAGE_INCLUDE_DIR}/..;External/include/FreeImage" )
    ENDIF( FREEIMAGE_FOUND )

    IF( DIRECTSHOW_FOUND )
      SET(CPACK_INSTALLED_DIRECTORIES ${CPACK_INSTALLED_DIRECTORIES}
                                      "${DIRECTSHOW_INCLUDE_DIR_STREAMS_H}/..;External/include/DirectShow" )
    ENDIF( DIRECTSHOW_FOUND )
  
    SET(CPACK_INSTALLED_DIRECTORIES ${CPACK_INSTALLED_DIRECTORIES}
                                    ${HAPI_CPACK_INSTALLED_DIRECTORIES} )

    IF( H3DViewer_cmake_install )
      FIND_PACKAGE(wxWidgetsWin)
      IF(wxWidgets_FOUND)
        SET(CPACK_INSTALLED_DIRECTORIES ${CPACK_INSTALLED_DIRECTORIES}
                                        "${wxWidgets_INCLUDE_DIR}/wx;External/include/wx" )
      ENDIF(wxWidgets_FOUND)
    ENDIF( H3DViewer_cmake_install )

    # External/bin and External/lib are needed to run program.
    SET(CPACK_INSTALLED_DIRECTORIES ${CPACK_INSTALLED_DIRECTORIES}
                                    "${H3DAPI_SOURCE_DIR}/../../External/bin;External/bin" )  
    SET(CPACK_INSTALLED_DIRECTORIES ${CPACK_INSTALLED_DIRECTORIES}
                                    "${H3DAPI_SOURCE_DIR}/../../External/lib;External/lib" )
  ENDIF(WIN32)

  INCLUDE(CPack)
  INCLUDE(UseDebian)
  IF(DEBIAN_FOUND)
    ADD_DEBIAN_TARGETS(H3DAPI)
  ENDIF(DEBIAN_FOUND)
ENDIF( GENERATE_CPACK_PROJECT )

# H3DAPI shared library definition
ADD_LIBRARY(H3DAPI SHARED ${H3DAPI_SRCS})

# make sure that the H3DAPI.rc contains the correct svn-version
IF( MSVC )
  EXECUTE_PROCESS( COMMAND ${H3DAPI_SOURCE_DIR}/FindSVNVersion ${H3DAPI_SOURCE_DIR}/../src/.svn/entries 
                   OUTPUT_VARIABLE H3DAPI_SVN_VERSION )
  # autogenerate H3DAPI.rc depending on the version
  CONFIGURE_FILE( H3DAPI.rc.cmake ${CMAKE_CURRENT_BINARY_DIR}/H3DAPI.rc )

  ADD_CUSTOM_COMMAND( TARGET H3DAPI 
                      PRE_BUILD 
                      COMMAND ${H3DAPI_SOURCE_DIR}/UpdateResourceFile 
                      ARGS H3DAPI ${CMAKE_CURRENT_BINARY_DIR}/H3DAPI.rc H3DAPI.rc.cmake
                      ${H3DAPI_MAJOR_VERSION} ${H3DAPI_MINOR_VERSION}
                      ${H3DAPI_BUILD_VERSION}
                      ${H3DAPI_SOURCE_DIR}/../src/.svn/entries )
ENDIF(MSVC)

# set the install directory to the H3D directory on Windows
IF(WIN32) 
 SET( CMAKE_INSTALL_PREFIX ${H3DAPI_SOURCE_DIR}/../..)
ENDIF(WIN32)

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
  SET( H3DAPI_LINK_FLAGS_DEBUG "" )

  # change the name depending on compiler to be able to tell them apart
  # since they are not compatible with each other. 
  IF(MSVC70 OR MSVC71)
    SET_TARGET_PROPERTIES( H3DAPI PROPERTIES OUTPUT_NAME H3DAPI_vc7 )
    SET( TEMP_OUTPUT_POSTFIX _vc7 )
    # H3D API is now to big to link incrementally with vc7.
    SET( H3DAPI_LINK_FLAGS_DEBUG "${H3DAPI_LINK_FLAGS_DEBUG} /NODEFAULTLIB:msvcrt /INCREMENTAL:NO" )
  ELSEIF(MSVC80)
    SET_TARGET_PROPERTIES( H3DAPI PROPERTIES OUTPUT_NAME H3DAPI_vc8 )
    SET( TEMP_OUTPUT_POSTFIX _vc8 )
    # remove some warnings about deprecated functions.
    SET( H3DAPI_COMPILE_FLAGS "${H3DAPI_COMPILE_FLAGS} -D_CRT_SECURE_NO_DEPRECATE /Zc:wchar_t-" )
    SET( H3DAPI_LINK_FLAGS_DEBUG "${H3DAPI_LINK_FLAGS_DEBUG} /NODEFAULTLIB:msvcrt" )
    # Needed to delayload OpenHapticsRenderer. They must have changed the way symbols are imported between versions of visual studio.
    IF(OPENHAPTICS_FOUND)
      SET_SOURCE_FILES_PROPERTIES( ${H3DAPI_SOURCE_DIR}/../src/MagneticSurface.cpp
                                   ${H3DAPI_SOURCE_DIR}/../src/OpenHapticsSurface.cpp
                                   ${H3DAPI_SOURCE_DIR}/../src/X3DGeometryNode.cpp
                                   PROPERTIES COMPILE_FLAGS "/Od" )
    ENDIF(OPENHAPTICS_FOUND)
  ELSEIF(MSVC90)
    SET_TARGET_PROPERTIES( H3DAPI PROPERTIES OUTPUT_NAME H3DAPI_vc9 )
    SET( TEMP_OUTPUT_POSTFIX _vc9 )
    # remove some warnings about deprecated functions.
    SET( H3DAPI_COMPILE_FLAGS "${H3DAPI_COMPILE_FLAGS} -D_CRT_SECURE_NO_DEPRECATE /Zc:wchar_t- /MP" )
    # H3D API is now to big to link incrementally with vc9.
    SET( H3DAPI_LINK_FLAGS_DEBUG "${H3DAPI_LINK_FLAGS_DEBUG} /NODEFAULTLIB:msvcrt /INCREMENTAL:NO" )
    # Needed to delayload OpenHapticsRenderer. They must have changed the way symbols are imported between versions of visual studio.
    IF(OPENHAPTICS_FOUND)
      SET_SOURCE_FILES_PROPERTIES( ${H3DAPI_SOURCE_DIR}/../src/MagneticSurface.cpp
                                   ${H3DAPI_SOURCE_DIR}/../src/OpenHapticsSurface.cpp
                                   ${H3DAPI_SOURCE_DIR}/../src/X3DGeometryNode.cpp
                                   PROPERTIES COMPILE_FLAGS "/Od" )
    ENDIF(OPENHAPTICS_FOUND)
  ENDIF(MSVC70 OR MSVC71)

  # Set delayload properties of H3D API.
  SET_TARGET_PROPERTIES( H3DAPI
                         PROPERTIES
                         LINK_FLAGS "/DELAYLOAD:\"cg.dll\" /DELAYLOAD:\"cgGL.dll\" /DELAYLOAD:\"FreeImage.dll\" /DELAYLOAD:\"ftgl_dynamic_MTD.dll\" /DELAYLOAD:\"OpenAL32.dll\" /DELAYLOAD:\"vorbisfile.dll\"" )
  
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
                         LINK_FLAGS_RELEASE "${H3DAPI_LINK_FLAGS_RELEASE}" )
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
  INSTALL( DIRECTORY ../include/H3D 
           DESTINATION include 
           PATTERN .svn EXCLUDE )
ENDIF( NOT WIN32 )

# H3DAPI is dependent on H3DUtil and HAPI
ADD_DEPENDENCIES( H3DAPI H3DUtil )
ADD_DEPENDENCIES( H3DAPI HAPI )

IF( WIN32 )
  #Only add dependencies if OpenHaptics exists.
  IF(OPENHAPTICS_FOUND)
    ADD_DEPENDENCIES( H3DAPI OpenHapticsRenderer )
  ENDIF(OPENHAPTICS_FOUND)

  #Only add dependencies if Chai3d exists.
  IF(CHAI3D_FOUND)
    ADD_DEPENDENCIES( H3DAPI Chai3DRenderer )
  ENDIF(CHAI3D_FOUND)
ENDIF( WIN32 )
