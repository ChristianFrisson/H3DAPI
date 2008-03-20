# - Find curl
# Find the native curl headers and libraries.
# If none is found for Windows use the distributed one in External
#
#  H3DCURL_INCLUDE_DIR -  where to find curl.h, etc.
#  H3DCURL_LIBRARIES    - List of libraries when using curl.
#  H3DCURL_FOUND        - True if curlfound.


FIND_PACKAGE(CURL)
IF(CURL_FOUND)
  SET(H3DCURL_FOUND 1)
  SET(H3DCURL_LIBRARIES ${CURL_LIBRARIES})
  SET(H3DCURL_INCLUDE_DIR ${CURL_INCLUDE_DIR})
ELSE(CURL_FOUND)
  SET(H3DCURL_FOUND 0)
  SET(H3DCURL_LIBRARIES)
  SET(H3DCURL_INCLUDE_DIR)
  
  IF(WIN32)
    # Look for the header file.
    FIND_PATH( H3DCURL_INCLUDE_DIRS NAMES curl/curl.h
               PATHS $ENV{H3D_EXTERNAL_ROOT}/include  
                     ../../External/include )
    
    # Look for the library.
    FIND_LIBRARY( H3DCURL_LIBRARY NAMES libcurl 
                  PATHS $ENV{H3D_EXTERNAL_ROOT}/lib
                        ../../External/lib )    
    
    IF(H3DCURL_INCLUDE_DIRS AND H3DCURL_LIBRARY)
      SET(H3DCURL_FOUND 1)
      SET(H3DCURL_LIBRARIES ${H3DCURL_LIBRARY})
      SET(H3DCURL_INCLUDE_DIR ${H3DCURL_INCLUDE_DIRS})
    ENDIF(H3DCURL_INCLUDE_DIRS AND H3DCURL_LIBRARY)
    
  ENDIF(WIN32)
ENDIF(CURL_FOUND)

# Report the results.
IF(NOT H3DCURL_FOUND)
  SET(H3DCURL_DIR_MESSAGE
    "Curl was not found. Make sure CURL_LIBRARY and CURL_INCLUDE_DIRS are set if compressed files support is desired.")
  IF(NOT H3DCURL_FIND_QUIETLY)
    MESSAGE(STATUS "${H3DCURL_DIR_MESSAGE}")
  ELSE(NOT H3DCURL_FIND_QUIETLY)
    IF(H3DCURL_FIND_REQUIRED)
      SET(H3DCURL_DIR_MESSAGE
          "Curl was not found. Make sure CURL_LIBRARY and CURL_INCLUDE_DIRS are set. CURL is required to build.")
      MESSAGE(FATAL_ERROR "${H3DCURL_DIR_MESSAGE}")
    ENDIF(H3DCURL_FIND_REQUIRED)
  ENDIF(NOT H3DCURL_FIND_QUIETLY)
ENDIF(NOT H3DCURL_FOUND)

MARK_AS_ADVANCED(H3DCURL_INCLUDE_DIRS H3DCURL_LIBRARY)
