cmake_minimum_required(VERSION 2.4.8)
# The name of our project is "ManualExamples". CMakeLists files in this project can
# refer to the root source directory of the project as ${ManualExamples_SOURCE_DIR} and
# to the root binary directory of the project as ${ManualExamples_BINARY_DIR}.
project (ManualExamples)

SET(optionalLibs)

SET(requiredLibs)

SET(CMAKE_MODULE_PATH "${ManualExamples_SOURCE_DIR}/../../../build/modules/")

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

#H3DAPI
FIND_PACKAGE(H3DAPI REQUIRED)

IF(H3DAPI_FOUND)
  INCLUDE_DIRECTORIES( ${H3DAPI_INCLUDE_DIR} ) 
  SET(requiredLibs ${requiredLibs} ${H3DAPI_LIBRARIES} )
ENDIF(H3DAPI_FOUND)

#Xerces
FIND_PACKAGE(Xerces)

IF(XERCES_FOUND)
  INCLUDE_DIRECTORIES( ${XERCES_INCLUDE_DIR} ) 
  SET(optionalLibs ${optionalLibs} ${XERCES_LIBRARIES} )
ENDIF(XERCES_FOUND)

ADD_EXECUTABLE(Sphere_X3D ${ManualExamples_SOURCE_DIR}/Sphere_X3D.cpp )
ADD_EXECUTABLE(Spheres_X3D ${ManualExamples_SOURCE_DIR}/Spheres_X3D.cpp )

TARGET_LINK_LIBRARIES( Sphere_X3D ${requiredLibs} ${optionalLibs} )
TARGET_LINK_LIBRARIES( Spheres_X3D ${requiredLibs} ${optionalLibs} )

SET_TARGET_PROPERTIES( Sphere_X3D PROPERTIES DEBUG_POSTFIX "_d" )
SET_TARGET_PROPERTIES( Spheres_X3D PROPERTIES DEBUG_POSTFIX "_d" )

IF(MSVC80)
  # remove some warnings about deprecated functions.
  SET_TARGET_PROPERTIES( Sphere_X3D PROPERTIES COMPILE_FLAGS "-D_CRT_SECURE_NO_DEPRECATE /Zc:wchar_t-" )
  SET_TARGET_PROPERTIES( Spheres_X3D PROPERTIES COMPILE_FLAGS "-D_CRT_SECURE_NO_DEPRECATE /Zc:wchar_t-" )
ELSEIF(MSVC90)
  # remove some warnings about deprecated functions.
  SET_TARGET_PROPERTIES( Sphere_X3D PROPERTIES COMPILE_FLAGS "-D_CRT_SECURE_NO_DEPRECATE /Zc:wchar_t- /MP" )
  SET_TARGET_PROPERTIES( Spheres_X3D PROPERTIES COMPILE_FLAGS "-D_CRT_SECURE_NO_DEPRECATE /Zc:wchar_t- /MP" )
ENDIF(MSVC80)

# set the install directory to the H3D directory on Windows
IF(WIN32) 
 SET( CMAKE_INSTALL_PREFIX ${ManualExamples_SOURCE_DIR}/../../../..)
ENDIF(WIN32)

INSTALL( TARGETS Sphere_X3D
	 LIBRARY DESTINATION lib
	 RUNTIME DESTINATION bin )

INSTALL( TARGETS Spheres_X3D
	 LIBRARY DESTINATION lib
	 RUNTIME DESTINATION bin )


