# - Find AUDIOFILE
# Find the native AUDIOFILE headers and libraries.
#
#  AUDIOFILE_INCLUDE_DIR -  where to find AUDIOFILE.h, etc.
#  AUDIOFILE_LIBRARIES    - List of libraries when using AUDIOFILE.
#  AUDIOFILE_FOUND        - True if AUDIOFILE found.

GET_FILENAME_COMPONENT(module_file_path ${CMAKE_CURRENT_LIST_FILE} PATH )

IF( CMAKE_CL_64 )
  SET( LIB "lib64" )
ELSE( CMAKE_CL_64 )
  SET( LIB "lib32" )
ENDIF( CMAKE_CL_64 )

# Look for the header file.
FIND_PATH( AUDIOFILE_INCLUDE_DIR NAMES audiofile.h
           PATHS /usr/local/include
                 $ENV{H3D_EXTERNAL_ROOT}/include  
                 $ENV{H3D_EXTERNAL_ROOT}/include/libaudiofile
                 $ENV{H3D_ROOT}/../External/include  
                 $ENV{H3D_ROOT}/../External/include/libaudiofile
                 ../../External/include
                 ../../External/include/libaudiofile
                 ${module_file_path}/../../../External/include
                 ${module_file_path}/../../../External/include/libaudiofile
           DOC "Path in which the file audiofile.h is located." )
MARK_AS_ADVANCED(AUDIOFILE_INCLUDE_DIR)

# Look for the library.
# Does this work on UNIX systems? (LINUX)
FIND_LIBRARY( AUDIOFILE_LIBRARY NAMES audiofile
              PATHS $ENV{H3D_EXTERNAL_ROOT}/${LIB}
                    $ENV{H3D_ROOT}/../External/${LIB}
                    ../../External/${LIB}
                    ${module_file_path}/../../../External/${LIB}
              DOC "Path to audiofile library." )
MARK_AS_ADVANCED(AUDIOFILE_LIBRARY)

# Copy the results to the output variables.
IF(AUDIOFILE_INCLUDE_DIR AND AUDIOFILE_LIBRARY)
  SET(AUDIOFILE_FOUND 1)
  SET(AUDIOFILE_LIBRARIES ${AUDIOFILE_LIBRARY})
  SET(AUDIOFILE_INCLUDE_DIR ${AUDIOFILE_INCLUDE_DIR})
ELSE(AUDIOFILE_INCLUDE_DIR AND AUDIOFILE_LIBRARY)
  SET(AUDIOFILE_FOUND 0)
  SET(AUDIOFILE_LIBRARIES)
  SET(AUDIOFILE_INCLUDE_DIR)
ENDIF(AUDIOFILE_INCLUDE_DIR AND AUDIOFILE_LIBRARY)

# Report the results.
IF(NOT AUDIOFILE_FOUND)
  SET(AUDIOFILE_DIR_MESSAGE
    "AUDIOFILE was not found. Make sure AUDIOFILE_LIBRARY and AUDIOFILE_INCLUDE_DIR are set.")
  IF(Audiofile_FIND_REQUIRED)
    MESSAGE(FATAL_ERROR "${AUDIOFILE_DIR_MESSAGE}")
  ELSEIF(NOT Audiofile_FIND_QUIETLY)
    MESSAGE(STATUS "${AUDIOFILE_DIR_MESSAGE}")
  ENDIF(Audiofile_FIND_REQUIRED)
ENDIF(NOT AUDIOFILE_FOUND)
