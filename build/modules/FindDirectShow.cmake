# - Find DirectShow, windows only
#
#  DIRECTSHOW_INCLUDE_DIR -  where to find streams.h.
#  DIRECTSHOW_LIBRARIES    - List of libraries when using DirectShow.
#  DIRECTSHOW_FOUND        - True if DirectShow is found.

INCLUDE( TestIfVCExpress )
TestIfVCExpress()

include( H3DExternalSearchPath )
GET_FILENAME_COMPONENT( module_file_path ${CMAKE_CURRENT_LIST_FILE} PATH )
get_external_search_paths_h3d( module_include_search_paths module_lib_search_paths ${module_file_path} "DirectShow/BaseClasses" "static" )

IF(CMake_HAVE_MFC)
# Look for the header file.
SET( DIRECTSHOW_EXTRA_DIR )
IF(MSVC70 OR MSVC71)
  SET( DIRECTSHOW_EXTRA_DIR $ENV{VS71COMNTOOLS}../../Vc7/PlatformSDK/Include )
ELSEIF(MSVC80)
  SET( DIRECTSHOW_EXTRA_DIR $ENV{VS80COMNTOOLS}../../VC/PlatformSDK/Include )
ELSEIF(MSVC90)
  SET( DIRECTSHOW_EXTRA_DIR $ENV{VS90COMNTOOLS}../../VC/PlatformSDK/Include )
ELSEIF(MSVC10)
  SET( DIRECTSHOW_EXTRA_DIR $ENV{VS100COMNTOOLS}../../VC/PlatformSDK/Include )
ENDIF(MSVC70 OR MSVC71)

GET_FILENAME_COMPONENT(module_file_path ${CMAKE_CURRENT_LIST_FILE} PATH )

FIND_PATH( DIRECTSHOW_INCLUDE_DIR_STREAMS_H NAMES streams.h
           PATHS ${module_include_search_paths}
           DOC "Path in which the file streams.h is located." )
MARK_AS_ADVANCED(DIRECTSHOW_INCLUDE_DIR_STREAMS_H)

FIND_PATH( DIRECTSHOW_INCLUDE_DIR_DDRAW_H NAMES ddraw.h
           PATHS $ENV{DXSDK_DIR}/include
                 ${DIRECTSHOW_EXTRA_DIR}
		 "C:/Program Files (x86)/Microsoft SDKs/Windows/v7.0A/Include"
		 "C:/Program Files/Microsoft SDKs/Windows/v7.0A/Include"
		 "C:/Program Files (x86)/Microsoft SDKs/Windows/v7.0/Include"
		 "C:/Program Files/Microsoft SDKs/Windows/v7.0/Include"

           DOC "Path in which the file ddraw.h is located." )
MARK_AS_ADVANCED(DIRECTSHOW_INCLUDE_DIR_DDRAW_H)

FIND_PATH( DIRECTSHOW_INCLUDE_DIR_INTSAFE_H NAMES intsafe.h
           PATHS $ENV{DXSDK_DIR}/include
                 ${DIRECTSHOW_EXTRA_DIR}
		 "C:/Program Files (x86)/Microsoft SDKs/Windows/v7.0A/Include"
		 "C:/Program Files/Microsoft SDKs/Windows/v7.0A/Include"
		 "C:/Program Files (x86)/Microsoft SDKs/Windows/v7.0/Include"
		 "C:/Program Files/Microsoft SDKs/Windows/v7.0/Include"
 
            DOC "Path in which the file intsafe.h is located." )
MARK_AS_ADVANCED(DIRECTSHOW_INCLUDE_DIR_INTSAFE_H)

FIND_LIBRARY( DIRECTSHOW_LIBRARY NAMES strmbase
              PATHS ${module_lib_search_paths}
              DOC "Path to strmbase library." )
MARK_AS_ADVANCED(DIRECTSHOW_LIBRARY)

# Copy the results to the output variables.
IF(DIRECTSHOW_INCLUDE_DIR_STREAMS_H AND DIRECTSHOW_INCLUDE_DIR_INTSAFE_H AND DIRECTSHOW_INCLUDE_DIR_DDRAW_H AND DIRECTSHOW_LIBRARY)
  SET(DIRECTSHOW_FOUND 1)
  SET(DIRECTSHOW_LIBRARIES ${DIRECTSHOW_LIBRARY})
  SET(DIRECTSHOW_INCLUDE_DIR ${DIRECTSHOW_INCLUDE_DIR_STREAMS_H} ${DIRECTSHOW_INCLUDE_DIR_DDRAW_H})
ELSE(DIRECTSHOW_INCLUDE_DIR_STREAMS_H AND DIRECTSHOW_INCLUDE_DIR_INTSAFE_H AND DIRECTSHOW_INCLUDE_DIR_DDRAW_H AND DIRECTSHOW_LIBRARY)
  SET(DIRECTSHOW_FOUND 0)
  SET(DIRECTSHOW_LIBRARIES)
  SET(DIRECTSHOW_INCLUDE_DIR)
ENDIF(DIRECTSHOW_INCLUDE_DIR_STREAMS_H AND DIRECTSHOW_INCLUDE_DIR_INTSAFE_H AND DIRECTSHOW_INCLUDE_DIR_DDRAW_H AND DIRECTSHOW_LIBRARY)

ENDIF(CMake_HAVE_MFC)

# Report the results.
IF(NOT DIRECTSHOW_FOUND)
  SET(DIRECTSHOW_DIR_MESSAGE
    "Directshow was not found. Set DIRECTSHOW_INCLUDE_DIR_STREAMS_H, DIRECTSHOW_INCLUDE_DIR_INTSAFE_H, DIRECTSHOW_INCLUDE_DIR_DDRAW_H and DIRECTSHOW_LIBRARY to enable Directshow (DSHOW) support.")
  IF(DirectShow_FIND_REQUIRED)
    MESSAGE(FATAL_ERROR "${DIRECTSHOW_DIR_MESSAGE}")
  ELSEIF(NOT DirectShow_FIND_QUIETLY)
    MESSAGE(STATUS "${DIRECTSHOW_DIR_MESSAGE}")
  ENDIF(DirectShow_FIND_REQUIRED)
ENDIF(NOT DIRECTSHOW_FOUND)
