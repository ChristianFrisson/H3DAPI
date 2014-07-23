# - Find OpenEXR
# Finds OpenEXR libraries from ILM for handling HRD / float image formats
#
#  OPENEXR_INCLUDE_DIR -  where to find OpenEXR headers
#  OPENEXR_LIBRARIES    - List of libraries when using OpenEXR.
#  OPENEXR_FOUND        - True if OpenEXR found.


IF( CMAKE_CL_64 )
  SET( LIB "lib64" )
ELSE( CMAKE_CL_64 )
  SET( LIB "lib32" )
ENDIF( CMAKE_CL_64 )
GET_FILENAME_COMPONENT(module_file_path ${CMAKE_CURRENT_LIST_FILE} PATH )
# Look for the header file.
FIND_PATH( OPENEXR_INCLUDE_DIR NAMES OpenEXR/Iex.h
           PATHS $ENV{H3D_EXTERNAL_ROOT}/include
                 $ENV{H3D_ROOT}/../External/include
                 ../../External/include
                 ${module_file_path}/../../../External/include
           DOC "Path in which the file OpenEXR/Iex.h is located." )

# Look for the library.
FIND_LIBRARY( OPENEXR_LIBRARY_IMF NAMES IlmImf 
              PATHS $ENV{H3D_EXTERNAL_ROOT}/${LIB}
                    $ENV{H3D_ROOT}/../External/${LIB}
                    ../../External/${LIB}
                    ${module_file_path}/../../../External/${LIB}
              DOC "Path to IlmImf library." )

FIND_LIBRARY( OPENEXR_LIBRARY_ZLIB NAMES zlibwapi 
              PATHS $ENV{H3D_EXTERNAL_ROOT}/${LIB}
                    $ENV{H3D_ROOT}/../External/${LIB}
                    ../../External/${LIB}
                    ${module_file_path}/../../../External/${LIB}
              DOC "Path to zlibwapi library." )

FIND_LIBRARY( OPENEXR_LIBRARY_THREAD NAMES IlmThread 
              PATHS $ENV{H3D_EXTERNAL_ROOT}/${LIB}
                    $ENV{H3D_ROOT}/../External/${LIB}
                    ../../External/${LIB}
                    ${module_file_path}/../../../External/${LIB}
              DOC "Path to IlmThread library." )

FIND_LIBRARY( OPENEXR_LIBRARY_MATH NAMES Imath 
              PATHS $ENV{H3D_EXTERNAL_ROOT}/${LIB}
                    $ENV{H3D_ROOT}/../External/${LIB}
                    ../../External/${LIB}
                    ${module_file_path}/../../../External/${LIB}
              DOC "Path to Imath library." )

FIND_LIBRARY( OPENEXR_LIBRARY_HALF NAMES Half 
              PATHS $ENV{H3D_EXTERNAL_ROOT}/${LIB}
                    $ENV{H3D_ROOT}/../External/${LIB}
                    ../../External/${LIB}
                    ${module_file_path}/../../../External/${LIB}
              DOC "Path to Half library." )

FIND_LIBRARY( OPENEXR_LIBRARY_EX NAMES Iex 
              PATHS $ENV{H3D_EXTERNAL_ROOT}/${LIB}
                    $ENV{H3D_ROOT}/../External/${LIB}
                    ../../External/${LIB}
                    ${module_file_path}/../../../External/${LIB}
              DOC "Path to Iex library." )

IF(OPENEXR_INCLUDE_DIR AND OPENEXR_LIBRARY_IMF AND OPENEXR_LIBRARY_ZLIB AND OPENEXR_LIBRARY_THREAD AND OPENEXR_LIBRARY_MATH AND OPENEXR_LIBRARY_HALF)
  SET(OPENEXR_FOUND 1)
  SET(OPENEXR_LIBRARIES ${OPENEXR_LIBRARY_IMF} ${OPENEXR_LIBRARY_ZLIB} ${OPENEXR_LIBRARY_THREAD} ${OPENEXR_LIBRARY_MATH} ${OPENEXR_LIBRARY_HALF} ${OPENEXR_LIBRARY_EX})
ENDIF()

# Report the results.
IF(NOT OPENEXR_FOUND)
  MESSAGE(STATUS "OPENEXR was not found. Handling OpenEXR images will not be supported.")
ENDIF()

MARK_AS_ADVANCED(OPENEXR_INCLUDE_DIR OPENEXR_LIBRARY)
