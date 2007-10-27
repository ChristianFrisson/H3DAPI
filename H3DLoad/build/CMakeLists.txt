# The name of our project is "HELLO".  CMakeLists files in this project can
# refer to the root source directory of the project as ${HELLO_SOURCE_DIR} and
# to the root binary directory of the project as ${HELLO_BINARY_DIR}.
project (H3DLoad)

# Create a library called "Hello" which includes the source file "hello.cxx".
# The extension is already found.  Any number of sources could be listed here.

AUX_SOURCE_DIRECTORY(../src H3DLoad_SRCS)

SET(optionalLibs)

INCLUDE_DIRECTORIES( ../src )

SET(requiredLibs)

SET(CMAKE_MODULE_PATH "../../build/modules/")

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

#H#DAPI
FIND_PACKAGE(H3DAPI REQUIRED)

IF(H3DAPI_FOUND)
  INCLUDE_DIRECTORIES( ${H3DAPI_INCLUDE_DIR} ) 
  SET(requiredLibs ${requiredLibs} ${H3DAPI_LIBRARIES} )
ENDIF(H3DAPI_FOUND)

FIND_PACKAGE(OpenGL REQUIRED)

IF(OPENGL_FOUND)
  INCLUDE_DIRECTORIES( ${OPENGL_INCLUDE_DIR} ) 
  SET(requiredLibs ${requiredLibs} ${OPENGL_LIBRARIES} )
ENDIF(OPENGL_FOUND)

FIND_PACKAGE(GLUT REQUIRED)

IF(GLUT_FOUND)
  INCLUDE_DIRECTORIES( ${GLUT_INCLUDE_DIR} ) 
  SET(requiredLibs ${requiredLibs} ${GLUT_LIBRARIES} )
ENDIF(GLUT_FOUND)


FIND_PACKAGE(XERCES)

IF(XERCES_FOUND)
  SET(HAVE_XERCES 1)
  INCLUDE_DIRECTORIES( ${XERCES_INCLUDE_DIR} ) 
  SET(optionalLibs ${optionalLibs} ${XERCES_LIBRARIES} )
ENDIF(XERCES_FOUND)

ADD_EXECUTABLE(H3DLoad  ${H3DLoad_SRCS})
TARGET_LINK_LIBRARIES( H3DLoad ${requiredLibs} ${optionalLibs} )


INSTALL( TARGETS H3DLoad
	 LIBRARY DESTINATION lib
	 RUNTIME DESTINATION bin )



    

