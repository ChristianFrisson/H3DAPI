IF( COMMAND cmake_policy )
  IF( POLICY CMP0003 )
    cmake_policy(SET CMP0003 OLD)
  ENDIF( POLICY CMP0003 )
ENDIF( COMMAND cmake_policy )

IF(WIN32)
  cmake_minimum_required(VERSION 2.6.0)
ENDIF(WIN32)

PROJECT (H3DAPI)

# set the install directory to the H3D directory on Windows
IF(WIN32 AND CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT AND NOT H3D_CMAKE_INSTALL_PREFIX_ALREADY_SET )
  SET( CMAKE_INSTALL_PREFIX ${H3DAPI_SOURCE_DIR}/../.. CACHE PATH "Install path prefix, prepended onto install directories." FORCE )
  SET( H3D_CMAKE_INSTALL_PREFIX_ALREADY_SET TRUE )
ENDIF(WIN32 AND CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT AND NOT H3D_CMAKE_INSTALL_PREFIX_ALREADY_SET )

# Set these variables so subdirectories ( added by ADD_SUBDIRECTORY ) can figure out where include files are.
SET( H3DAPI_INCLUDE_DIR "${H3DAPI_SOURCE_DIR}/../include" )

# Make include files in paths like /usr/local/include have preference before
# files in OSX frameworks. 
SET(CMAKE_FIND_FRAMEWORK "LAST")

# make sure symbols are exported.
SET( H3DAPI_COMPILE_FLAGS "-DH3DAPI_EXPORTS" )
SET( H3DAPI_LINK_FLAGS "" )
SET( H3DAPI_LINK_FLAGS_RELEASE "" )
SET( H3DAPI_LINK_FLAGS_DEBUG "" )

# Add a cache variable GENERATE_CPACK_PROJECT to have the choice of generating a project
# for packaging H3DAPI. Default is NO since most people will not use this.
IF( NOT DEFINED GENERATE_CPACK_PROJECT )
  SET( GENERATE_CPACK_PROJECT "NO" CACHE BOOL "Decides if a cpack project should be generated. The project in the first loaded CMakeLists will configure CPack." )
  MARK_AS_ADVANCED(GENERATE_CPACK_PROJECT)
ENDIF( NOT DEFINED GENERATE_CPACK_PROJECT )

# Add a cache variable GENERATE_UNITY_BUILD to have the choice of selecting
# a unity build project. Default is NO.
IF( NOT DEFINED GENERATE_UNITY_BUILD )
  SET( GENERATE_UNITY_BUILD "NO" CACHE BOOL "Decides if a the generated project files should build through a unity build instead of a normal build. A unity builds packs all .cpp files into a UnityBuild.cpp file and then only include this in the project. This greatly reduces build times." )
ENDIF( NOT DEFINED GENERATE_UNITY_BUILD )

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

# Add a cache variable GENERATE_NODEROUTESTODOTFILE_BUILD that should be
# set to yes if NodeRoutesToDotFile project should be run against this
# build of H3DAPI.
IF( NOT DEFINED GENERATE_NODEROUTESTODOTFILE_BUILD )
  SET( GENERATE_NODEROUTESTODOTFILE_BUILD "NO" CACHE BOOL "Breaks H3DAPI for normal use but this flag must be set to yes when using the NodeRoutesToDotFile project, located in ${H3DAPI_SOURCE_DIR}/../Util/NodeRoutesToDotFile." )
  MARK_AS_ADVANCED(GENERATE_NODEROUTESTODOTFILE_BUILD)
ENDIF( NOT DEFINED GENERATE_NODEROUTESTODOTFILE_BUILD )

# Include HAPI in the build.
IF( WIN32 )
  MESSAGE( STATUS "Including HAPI" )
  ADD_SUBDIRECTORY( ${H3DAPI_SOURCE_DIR}/../../HAPI/build
                    ${CMAKE_CURRENT_BINARY_DIR}/HAPI )

  GET_DIRECTORY_PROPERTY( HAPI_INCLUDE_DIR DIRECTORY ${H3DAPI_SOURCE_DIR}/../../HAPI/build INCLUDE_DIRECTORIES )

  # Check if HAPTIK was found.
  IF(HAPTIK_INCLUDE_DIR AND HAPTIK_LIBRARY)
    SET( HAPTIK_FOUND 1 )
  ENDIF(HAPTIK_INCLUDE_DIR AND HAPTIK_LIBRARY)
ENDIF( WIN32 )

# Include H3D header dir.
INCLUDE_DIRECTORIES( ../include )

# Version of H3D API.
SET( H3DAPI_MAJOR_VERSION 2 )
SET( H3DAPI_MINOR_VERSION 1 )
SET( H3DAPI_BUILD_VERSION 1 )

# Set here already in case a special NSIS template needs to be used by cpack.
SET(CMAKE_MODULE_PATH ${H3DAPI_SOURCE_DIR}/modules )
IF( COMMAND cmake_policy )
  IF( POLICY CMP0011 )
    cmake_policy( SET CMP0011 NEW )
  ENDIF( POLICY CMP0011 )
ENDIF( COMMAND cmake_policy )
INCLUDE( StripAndAddLibraryDirectories )

# Version of project.
SET( H3DAPI_FULL_VERSION
${H3DAPI_MAJOR_VERSION}.${H3DAPI_MINOR_VERSION}.${H3DAPI_BUILD_VERSION} ) 

# Add all sources, they are added to a variable called H3DAPI_SRCS
# defined in the included file. All header files are added to a variable called
# H3DAPI_HEADERS.
INCLUDE( ${H3DAPI_SOURCE_DIR}/H3DAPISourceFiles.txt )

IF( GENERATE_UNITY_BUILD )
# Generate a unity build, by creating the UnityBuild.cpp and only including the required 
# source files.
SET( H3DAPI_SRCS ${H3DAPI_SRCS_UNITYBUILD} )
SET( UNITYBUILD_INCLUDES "" )

foreach(filename ${NON_UNITYBUILD_FILES})
  SET( UNITYBUILD_INCLUDES "${UNITYBUILD_INCLUDES}\n #include \"${filename}\"" )
endforeach(filename)

CONFIGURE_FILE( ${H3DAPI_SOURCE_DIR}/UnityBuild.cmake ${H3DAPI_SOURCE_DIR}/../src/UnityBuild.cpp )

IF( MSVC )
  SET( H3DAPI_COMPILE_FLAGS "${H3DAPI_COMPILE_FLAGS} /bigobj")
ENDIF( MSVC )

ENDIF( GENERATE_UNITY_BUILD )

# Add the H3DAPI.rc resource file if Visual Studio
IF(MSVC)
  SET( H3DAPI_SRCS ${H3DAPI_SRCS} ${CMAKE_CURRENT_BINARY_DIR}/H3DAPI.rc )
ENDIF(MSVC)

# Add all optional libraries to this variable
SET(optionalLibs)

# Add all required libraries to this variable
SET(requiredLibs)

IF( TARGET H3DUtil )
  # Target was created by ADD_SUBDIRECTORY command. There might not be anything in
  # H3DUTIL_INCLUDE_DIR but HAPI_INCLUDE_DIR covers that in this case.
  INCLUDE_DIRECTORIES( ${H3DUTIL_INCLUDE_DIR} ) 
  SET( requiredLibs ${requiredLibs} H3DUtil )
  SET( INSTALL_RUNTIME_AND_LIBRARIES_ONLY_DEPENDENCIES ${INSTALL_RUNTIME_AND_LIBRARIES_ONLY_DEPENDENCIES} H3DUtil )
ELSE( TARGET H3DUtil )
  #H3DUtil
  FIND_PACKAGE(H3DUtil REQUIRED)

  IF(H3DUTIL_FOUND)
    INCLUDE_DIRECTORIES( ${H3DUTIL_INCLUDE_DIR} ) 
    SET(requiredLibs ${requiredLibs} ${H3DUTIL_LIBRARIES} )
  ENDIF(H3DUTIL_FOUND)
ENDIF( TARGET H3DUtil )

IF( TARGET HAPI )
  # Target was created by ADD_SUBDIRECTORY command.
  INCLUDE_DIRECTORIES( ${HAPI_INCLUDE_DIR} ) 
  SET( requiredLibs ${requiredLibs} HAPI )
  SET( INSTALL_RUNTIME_AND_LIBRARIES_ONLY_DEPENDENCIES ${INSTALL_RUNTIME_AND_LIBRARIES_ONLY_DEPENDENCIES} HAPI )
ELSE( TARGET HAPI )
  #HAPI
  FIND_PACKAGE(HAPI REQUIRED)

  IF(HAPI_FOUND)
    INCLUDE_DIRECTORIES( ${HAPI_INCLUDE_DIR} ) 
    SET(requiredLibs ${requiredLibs} ${HAPI_LIBRARIES} )
  ENDIF(HAPI_FOUND)
ENDIF( TARGET HAPI )

IF( TARGET Chai3DRenderer)
  SET(requiredLibs ${requiredLibs} Chai3DRenderer)
  IF( MSVC80)
    SET(requiredLibs ${requiredLibs} optimized atls.lib debug atlsd.lib)
  ENDIF( MSVC80)
  SET( INSTALL_RUNTIME_AND_LIBRARIES_ONLY_DEPENDENCIES ${INSTALL_RUNTIME_AND_LIBRARIES_ONLY_DEPENDENCIES} Chai3DRenderer )
ENDIF( TARGET Chai3DRenderer)
  
IF( TARGET OpenHapticsRenderer)
  SET(requiredLibs ${requiredLibs} OpenHapticsRenderer)
  SET( INSTALL_RUNTIME_AND_LIBRARIES_ONLY_DEPENDENCIES ${INSTALL_RUNTIME_AND_LIBRARIES_ONLY_DEPENDENCIES} OpenHapticsRenderer )
ENDIF( TARGET OpenHapticsRenderer)

# Extra libs to link against.
IF(WIN32)
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

# Optional extra library. Makes is possible to use GLUT windows handling with H3D API.
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

IF(NOT WIN32)
  # Linux specific libraries

  FIND_PACKAGE(FontConfig)
  IF(FONTCONFIG_FOUND)
    SET(HAVE_FONTCONFIG 1)
    INCLUDE_DIRECTORIES( ${FONTCONFIG_INCLUDE_DIR} ) 
    SET(optionalLibs ${optionalLibs} ${FONTCONFIG_LIBRARIES} )
  ENDIF(FONTCONFIG_FOUND)

  IF(NOT APPLE)
    # Optional extra library. Used to render movies as textures.
    FIND_PACKAGE(FFmpeg)
    IF(FFMPEG_FOUND)
      SET(HAVE_FFMPEG 1)
      INCLUDE_DIRECTORIES( ${FFMPEG_INCLUDE_DIRS} )  
      LIST( APPEND optionalLibs ${FFMPEG_LIBRARIES} )
    ENDIF(FFMPEG_FOUND)  
  ENDIF(NOT APPLE)
ENDIF(NOT WIN32)

FIND_PACKAGE(DCMTK)
IF(DCMTK_FOUND)
  INCLUDE_DIRECTORIES( ${DCMTK_INCLUDE_DIR} ) 
  SET(requiredLibs ${requiredLibs} ${DCMTK_LIBRARIES} )
  IF(NOT WIN32 AND NOT APPLE)
    # Linux specific libraries. The debian package for dcmtk
    # seems to not link to libtiff which it is dependent on. In 
    # order to compile out of the box we include that dependency here 
    # instead.
    FIND_PACKAGE(TIFF)
    IF(TIFF_FOUND)
      SET(requiredLibs ${requiredLibs} ${TIFF_LIBRARIES} )
    ELSE(TIFF_FOUND)
      MESSAGE( STATUS "If you are using dcmtk debian package libtiff might be required since dcmtk depends on it and the packaged version does not itself link with it properly." )
    ENDIF(TIFF_FOUND)
  ENDIF(NOT WIN32 AND NOT APPLE)
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

# H3DAPI shared library definition
ADD_LIBRARY(H3DAPI SHARED ${H3DAPI_SRCS} ${H3DAPI_HEADERS})

# make sure that the H3DAPI.rc contains the correct svn-version
IF( MSVC )
  SET( H3DAPI_SVN_VERSION "0" )
  # Find SubWCRev.exe
  FIND_FILE( SubWCRev
             NAMES "SubWCRev.exe"
             DOC   "Set to SubWCRev.exe that comes with TortoiseSVN. Used to find svn revision number." )

  IF( SubWCRev )
    SET( H3DAPI_SVN_VERSION "$WCREV$" )
  ENDIF( SubWCRev )

  # autogenerate H3DAPI.rc depending on the version
  CONFIGURE_FILE( H3DAPI.rc.cmake ${CMAKE_CURRENT_BINARY_DIR}/H3DAPI.rc )
  
  ADD_CUSTOM_COMMAND( TARGET H3DAPI
                      PRE_BUILD
                      COMMAND ${H3DAPI_SOURCE_DIR}/UpdateResourceFile 
                      ARGS H3DAPI ${CMAKE_CURRENT_BINARY_DIR}/H3DAPI.rc ${H3DAPI_SOURCE_DIR}/H3DAPI.rc.cmake
                      ${H3DAPI_MAJOR_VERSION} ${H3DAPI_MINOR_VERSION}
                      ${H3DAPI_BUILD_VERSION} "${H3DAPI_SVN_VERSION}"
                      "H3DAPI_Output_Name"
                      "$(TargetFileName)" )

  IF( SubWCRev )
    # Update SVN revision in file.
    EXECUTE_PROCESS( COMMAND ${SubWCRev} ${H3DAPI_SOURCE_DIR}/../ ${CMAKE_CURRENT_BINARY_DIR}/H3DAPI.rc ${CMAKE_CURRENT_BINARY_DIR}/H3DAPI.rc )

    ADD_CUSTOM_COMMAND( TARGET H3DAPI 
                        PRE_BUILD 
                        COMMAND ${SubWCRev} 
                        ARGS ${H3DAPI_SOURCE_DIR}/../ ${CMAKE_CURRENT_BINARY_DIR}/H3DAPI.rc ${CMAKE_CURRENT_BINARY_DIR}/H3DAPI.rc )
  ENDIF( SubWCRev )
ENDIF(MSVC)

# add the libraries needed for linking
TARGET_LINK_LIBRARIES( H3DAPI ${requiredLibs} ${optionalLibs} )

# make the name of debug libraries end in _d.
SET_TARGET_PROPERTIES( H3DAPI PROPERTIES DEBUG_POSTFIX "_d" )

# set the version of the library
SET_TARGET_PROPERTIES( H3DAPI PROPERTIES VERSION ${H3DAPI_FULL_VERSION} )

IF( WIN32 )
  # Temporary variables used later to set LINK_FLAGS.
  SET( TEMP_OUTPUT_POSTFIX "" )

  # change the name depending on compiler to be able to tell them apart
  # since they are not compatible with each other. 
  IF(MSVC70 OR MSVC71)
    SET_TARGET_PROPERTIES( H3DAPI PROPERTIES OUTPUT_NAME H3DAPI_vc7 )
    SET( TEMP_OUTPUT_POSTFIX _vc7 )
    # H3D API is now to big to link incrementally with vc7.
    SET( H3DAPI_LINK_FLAGS "${H3DAPI_LINK_FLAGS} /INCREMENTAL:NO" )
    SET( H3DAPI_LINK_FLAGS_DEBUG "${H3DAPI_LINK_FLAGS_DEBUG} /NODEFAULTLIB:msvcrt" )
    SET( H3DAPI_COMPILE_FLAGS "${H3DAPI_COMPILE_FLAGS} /Zc:wchar_t")
  ELSEIF(MSVC80)
    SET_TARGET_PROPERTIES( H3DAPI PROPERTIES OUTPUT_NAME H3DAPI_vc8 )
    SET( TEMP_OUTPUT_POSTFIX _vc8 )
    # remove some warnings about deprecated functions.
    SET( H3DAPI_COMPILE_FLAGS "${H3DAPI_COMPILE_FLAGS} -D_CRT_SECURE_NO_DEPRECATE" )
    
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
    SET( H3DAPI_LINK_FLAGS "${H3DAPI_LINK_FLAGS} /INCREMENTAL:NO" )
    SET( H3DAPI_LINK_FLAGS_DEBUG "${H3DAPI_LINK_FLAGS_DEBUG}" )
  ELSEIF(MSVC90)
    SET_TARGET_PROPERTIES( H3DAPI PROPERTIES OUTPUT_NAME H3DAPI_vc9 )
    SET( TEMP_OUTPUT_POSTFIX _vc9 )
    # remove some warnings about deprecated functions.
    SET( H3DAPI_COMPILE_FLAGS "${H3DAPI_COMPILE_FLAGS} -D_CRT_SECURE_NO_DEPRECATE /MP" )
    # if we are using 64 bit compiler some object files get too big so we need
    # to add the /bigobj flag
    IF( CMAKE_CL_64 )
      SET( H3DAPI_COMPILE_FLAGS "${H3DAPI_COMPILE_FLAGS} /bigobj" )
    ENDIF( CMAKE_CL_64 )
    # H3D API is now too big to link incrementally with vc9.
    SET( H3DAPI_LINK_FLAGS "${H3DAPI_LINK_FLAGS} /INCREMENTAL:NO" )
    SET( H3DAPI_LINK_FLAGS_DEBUG "${H3DAPI_LINK_FLAGS_DEBUG} /NODEFAULTLIB:msvcrt" )

  ELSEIF(MSVC10)
    SET_TARGET_PROPERTIES( H3DAPI PROPERTIES OUTPUT_NAME H3DAPI_vc10 )
    SET( TEMP_OUTPUT_POSTFIX _vc10 )
    # remove some warnings about deprecated functions.
    SET( H3DAPI_COMPILE_FLAGS "${H3DAPI_COMPILE_FLAGS} -D_CRT_SECURE_NO_DEPRECATE /MP" )
    # if we are using 64 bit compiler some object files get too big so we need
    # to add the /bigobj flag
    IF( CMAKE_CL_64 )
      SET( H3DAPI_COMPILE_FLAGS "${H3DAPI_COMPILE_FLAGS} /bigobj" )
    ENDIF( CMAKE_CL_64 )
    # H3D API is now to big to link incrementally with vc10.
    SET( H3DAPI_LINK_FLAGS "${H3DAPI_LINK_FLAGS} /INCREMENTAL:NO" )
    SET( H3DAPI_LINK_FLAGS_DEBUG "${H3DAPI_LINK_FLAGS_DEBUG} /NODEFAULTLIB:msvcrt" )
  ENDIF(MSVC70 OR MSVC71)

  # Set delayload properties of H3D API.
  SET( H3DAPI_LINK_FLAGS "${H3DAPI_LINK_FLAGS} /DELAYLOAD:\"cg.dll\" /DELAYLOAD:\"cgGL.dll\" /DELAYLOAD:\"FreeImage.dll\" /DELAYLOAD:\"OpenAL32.dll\" /DELAYLOAD:\"vorbisfile.dll\" /DELAYLOAD:\"VirtualHandDevice.dll\" /DELAYLOAD:\"VirtualHandCore.dll\"")

  IF(HAPTIK_FOUND)
    SET( H3DAPI_LINK_FLAGS "${H3DAPI_LINK_FLAGS} /DELAYLOAD:\"Haptik.Library.dll\"" )
  ENDIF(HAPTIK_FOUND)

  IF( TARGET Chai3DRenderer)
    SET( H3DAPI_LINK_FLAGS_DEBUG "${H3DAPI_LINK_FLAGS_DEBUG} /DELAYLOAD:\"Chai3DRenderer${TEMP_OUTPUT_POSTFIX}_d.dll\"" )
    SET( H3DAPI_LINK_FLAGS_RELEASE "${H3DAPI_LINK_FLAGS_RELEASE} /DELAYLOAD:\"Chai3DRenderer${TEMP_OUTPUT_POSTFIX}.dll\"" )
  ENDIF( TARGET Chai3DRenderer)
  
  IF( TARGET OpenHapticsRenderer)
    SET( H3DAPI_LINK_FLAGS_DEBUG "${H3DAPI_LINK_FLAGS_DEBUG} /DELAYLOAD:\"OpenHapticsRenderer${TEMP_OUTPUT_POSTFIX}_d.dll\"" )
    SET( H3DAPI_LINK_FLAGS_RELEASE "${H3DAPI_LINK_FLAGS_RELEASE} /DELAYLOAD:\"OpenHapticsRenderer${TEMP_OUTPUT_POSTFIX}.dll\"" )
  ENDIF( TARGET OpenHapticsRenderer)


ENDIF( WIN32 )

IF( GENERATE_NODEROUTESTODOTFILE_BUILD )
  SET( H3DAPI_COMPILE_FLAGS "${H3DAPI_COMPILE_FLAGS} -DH3D_GENERATE_DOTROUTE_FILES" )
ENDIF( GENERATE_NODEROUTESTODOTFILE_BUILD )

# Set compile and linker flags for H3DAPI.
  SET_TARGET_PROPERTIES( H3DAPI
                         PROPERTIES  
                         COMPILE_FLAGS "${H3DAPI_COMPILE_FLAGS}"
                         LINK_FLAGS "${H3DAPI_LINK_FLAGS}"
                         LINK_FLAGS_DEBUG "${H3DAPI_LINK_FLAGS_DEBUG}"
                         LINK_FLAGS_RELEASE "${H3DAPI_LINK_FLAGS_RELEASE}"
                         LINK_FLAGS_RELWITHDEBINFO "${H3DAPI_LINK_FLAGS_RELEASE}"
                         LINK_FLAGS_MINSIZEREL "${H3DAPI_LINK_FLAGS_RELEASE}" )


# autogenerate H3DAPI.h depending on the libraries available.
CONFIGURE_FILE( ${H3DAPI_SOURCE_DIR}/../include/H3D/H3DApi.cmake ${H3DAPI_SOURCE_DIR}/../include/H3D/H3DApi.h )

# Intall directories.
INSTALL( TARGETS H3DAPI
         LIBRARY DESTINATION lib COMPONENT H3DAPI_cpack_runtime
         RUNTIME DESTINATION bin COMPONENT H3DAPI_cpack_runtime
         ARCHIVE DESTINATION lib COMPONENT H3DAPI_cpack_libraries )

IF( NOT ( WIN32 OR GENERATE_CPACK_PROJECT ) )
  # Install include files.
  INSTALL( FILES ${H3DAPI_HEADERS}
           DESTINATION include/H3D )
ENDIF( NOT ( WIN32 OR GENERATE_CPACK_PROJECT ) )

IF( NOT DEFINED H3DAPI_LOADER_PROJECTS )
  SET( H3DAPI_LOADER_PROJECTS_DEFAULT "NO" )
  IF( H3D_USE_DEPENDENCIES_ONLY )
    SET( H3DAPI_LOADER_PROJECTS_DEFAULT "YES" )
  ENDIF( H3D_USE_DEPENDENCIES_ONLY )
  SET( H3DAPI_LOADER_PROJECTS "${H3DAPI_LOADER_PROJECTS_DEFAULT}" CACHE BOOL "If set to YES H3DLoad and H3DViewer will be included in the build." )
ENDIF( NOT DEFINED H3DAPI_LOADER_PROJECTS )

IF( H3DAPI_LOADER_PROJECTS )
  MESSAGE( STATUS "Including H3DLoad" )
  ADD_SUBDIRECTORY( ${H3DAPI_SOURCE_DIR}/../H3DLoad/build
                    ${CMAKE_CURRENT_BINARY_DIR}/H3DLoad )


  MESSAGE( STATUS "Including H3DViewer" )
  ADD_SUBDIRECTORY( ${H3DAPI_SOURCE_DIR}/../H3DViewer/build
                    ${CMAKE_CURRENT_BINARY_DIR}/H3DViewer )
  SET( INSTALL_RUNTIME_AND_LIBRARIES_ONLY_DEPENDENCIES ${INSTALL_RUNTIME_AND_LIBRARIES_ONLY_DEPENDENCIES} H3DLoad H3DViewer )
ENDIF( H3DAPI_LOADER_PROJECTS )

IF( NOT DEFINED H3DAPI_EXAMPLE_PROJECTS )
  SET( H3DAPI_EXAMPLE_PROJECTS "NO" CACHE BOOL "If set to YES various example projects build on H3DAPI will be included in the build." )
ENDIF( NOT DEFINED H3DAPI_EXAMPLE_PROJECTS )

IF( H3DAPI_EXAMPLE_PROJECTS )
  MESSAGE( STATUS "Including H3DAPI manual Examples" )
  ADD_SUBDIRECTORY( ${H3DAPI_SOURCE_DIR}/../examples/manualExamples/C++
                    ${CMAKE_CURRENT_BINARY_DIR}/examples/manualExamples )

	IF( EXISTS ${H3DAPI_SOURCE_DIR}/../examples/Particledemo )
	  MESSAGE( STATUS "Including ParticleDemo" )
  	ADD_SUBDIRECTORY( ${H3DAPI_SOURCE_DIR}/../examples/Particledemo/build
  	                  ${CMAKE_CURRENT_BINARY_DIR}/examples/Particledemo )
	ENDIF( EXISTS ${H3DAPI_SOURCE_DIR}/../examples/Particledemo )

  MESSAGE( STATUS "Including H3DAPI ThreadExample" )
  ADD_SUBDIRECTORY( ${H3DAPI_SOURCE_DIR}/../examples/ThreadExample
                    ${CMAKE_CURRENT_BINARY_DIR}/examples/ThreadExample )
  SET( INSTALL_RUNTIME_AND_LIBRARIES_ONLY_DEPENDENCIES ${INSTALL_RUNTIME_AND_LIBRARIES_ONLY_DEPENDENCIES} ParticleDemo ThreadExample Sphere_X3D Spheres_X3D )
ENDIF( H3DAPI_EXAMPLE_PROJECTS )

SET( INSTALL_RUNTIME_AND_LIBRARIES_ONLY_DEPENDENCIES ${INSTALL_RUNTIME_AND_LIBRARIES_ONLY_DEPENDENCIES} PARENT_SCOPE )

IF( NOT H3D_USE_DEPENDENCIES_ONLY)
  INCLUDE( ${H3DAPI_SOURCE_DIR}/H3DAPICPack.cmake )
ENDIF( NOT H3D_USE_DEPENDENCIES_ONLY)
