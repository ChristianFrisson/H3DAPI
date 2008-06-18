# - Find XERCES
# Find the native XERCES headers and libraries.
#
#  XERCES_INCLUDE_DIR -  where to find XERCES.h, etc.
#  XERCES_LIBRARIES    - List of libraries when using XERCES.
#  XERCES_FOUND        - True if XERCES found.


# Look for the header file.
FIND_PATH(XERCES_INCLUDE_DIR NAMES xercesc/sax2/Attributes.hpp
                             PATHS $ENV{H3D_EXTERNAL_ROOT}/include
                                   $ENV{H3D_ROOT}/../External/include
                                   ../../External/include
                                   ${CMAKE_MODULE_PATH}/../../../External/include )
MARK_AS_ADVANCED(XERCES_INCLUDE_DIR)

# Look for the library.
FIND_LIBRARY(XERCES_LIBRARY NAMES xerces-c xerces-c_2  
                            PATHS $ENV{H3D_EXTERNAL_ROOT}/lib
                                  $ENV{H3D_ROOT}/../External/lib
                                  ../../External/lib
                                  ${CMAKE_MODULE_PATH}/../../../External/lib )
MARK_AS_ADVANCED(XERCES_LIBRARY)

# Copy the results to the output variables.
IF(XERCES_INCLUDE_DIR AND XERCES_LIBRARY)
  SET(XERCES_FOUND 1)
  SET(XERCES_LIBRARIES ${XERCES_LIBRARY})
  SET(XERCES_INCLUDE_DIR ${XERCES_INCLUDE_DIR})
ELSE(XERCES_INCLUDE_DIR AND XERCES_LIBRARY)
  SET(XERCES_FOUND 0)
  SET(XERCES_LIBRARIES)
  SET(XERCES_INCLUDE_DIR)
ENDIF(XERCES_INCLUDE_DIR AND XERCES_LIBRARY)

# Report the results.
IF(NOT XERCES_FOUND)
  SET(XERCES_DIR_MESSAGE
    "Xerces-c was not found. Make sure XERCES_LIBRARY and
    XERCES_INCLUDE_DIR are set to the directory of your xerces lib and
    include files. If you do not have Xerces x3d/xml files cannot be parsed.")
  IF(XERCES_FIND_REQUIRED)
    MESSAGE(FATAL_ERROR "${XERCES_DIR_MESSAGE}")
  ELSEIF(NOT XERCES_FIND_QUIETLY)
    MESSAGE(STATUS "${XERCES_DIR_MESSAGE}")
  ENDIF(XERCES_FIND_REQUIRED)
ENDIF(NOT XERCES_FOUND)
