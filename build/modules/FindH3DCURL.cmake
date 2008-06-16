# - Find curl
# Find the native curl headers and libraries.
# If none is found for Windows use the distributed one in External
#
#  CURL_INCLUDE_DIR -  where to find curl.h, etc.
#  CURL_LIBRARIES    - List of libraries when using curl.
#  CURL_FOUND        - True if curlfound.


FIND_PACKAGE(CURL)
IF(NOT CURL_FOUND AND WIN32)
  # Look for the header file.
  FIND_PATH( CURL_INCLUDE_DIR NAMES curl/curl.h
             PATHS $ENV{H3D_EXTERNAL_ROOT}/include  
                   ../../External/include )
  
  # Look for the library.
  FIND_LIBRARY( CURL_LIBRARY NAMES libcurl 
                PATHS $ENV{H3D_EXTERNAL_ROOT}/lib
                      ../../External/lib )    
  
  IF(CURL_INCLUDE_DIR AND CURL_LIBRARY)
    SET(CURL_FOUND 1)
    SET(CURL_LIBRARIES ${CURL_LIBRARY})
    SET(CURL_INCLUDE_DIR ${CURL_INCLUDE_DIR})
  ENDIF(CURL_INCLUDE_DIR AND CURL_LIBRARY)
ENDIF(NOT CURL_FOUND AND WIN32)

# Report the results.
IF(NOT CURL_FOUND)
  SET(CURL_DIR_MESSAGE
    "Curl was not found. Make sure CURL_LIBRARY and CURL_INCLUDE_DIR are set if compressed files support is desired.")
  IF(CURL_FIND_REQUIRED)
    SET(CURL_DIR_MESSAGE
        "Curl was not found. Make sure CURL_LIBRARY and CURL_INCLUDE_DIR are set. CURL is required to build.")
    MESSAGE(FATAL_ERROR "${CURL_DIR_MESSAGE}")
  ELSEIF(NOT CURL_FIND_QUIETLY)
    MESSAGE(STATUS "${CURL_DIR_MESSAGE}")
  ENDIF(CURL_FIND_REQUIRED)
ENDIF(NOT CURL_FOUND)

MARK_AS_ADVANCED(CURL_INCLUDE_DIR CURL_LIBRARY)
