# The name of our project is "HELLO".  CMakeLists files in this project can
# refer to the root source directory of the project as ${HELLO_SOURCE_DIR} and
# to the root binary directory of the project as ${HELLO_BINARY_DIR}.
project (H3DViewer)

# Create a library called "Hello" which includes the source file "hello.cxx".
# The extension is already found.  Any number of sources could be listed here.

AUX_SOURCE_DIRECTORY(../src H3DViewer_SRCS)

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

#H3DAPI
FIND_PACKAGE(H3DAPI REQUIRED)

IF(H3DAPI_FOUND)
  INCLUDE_DIRECTORIES( ${H3DAPI_INCLUDE_DIR} ) 
  SET(requiredLibs ${requiredLibs} ${H3DAPI_LIBRARIES} )
ENDIF(H3DAPI_FOUND)

SET(WXWINDOWS_USE_GL 1)	
SET(wxWidgets_USE_LIBS base core gl adv)
FIND_PACKAGE(wxWidgets REQUIRED)
IF(wxWidgets_FOUND)
  INCLUDE(${wxWidgets_USE_FILE})
#  MESSAGE(FATAL_ERROR, ${wxWidgets_USE_FILE} )
#  MESSAGE(FATAL_ERROR, ${wxWidgets_CXX_FLAGS} )
#  MESSAGE(FATAL_ERROR, ${wxWidgets_DEFININTIONS} )
#  MESSAGE(FATAL_ERROR, ${wxWidgets_LIBRARIES} )
  SET(requiredLibs ${requiredLibs} ${wxWidgets_LIBRARIES} )
ENDIF(wxWidgets_FOUND)

ADD_EXECUTABLE(H3DViewer MACOSX_BUNDLE ${H3DViewer_SRCS})

TARGET_LINK_LIBRARIES( H3DViewer ${requiredLibs} ${optionalLibs} )

#INSTALL( TARGETS H3DViewer
#	 LIBRARY DESTINATION lib
#	 RUNTIME DESTINATION bin )

