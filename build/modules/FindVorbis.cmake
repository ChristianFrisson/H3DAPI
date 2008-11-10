# - Find VORBIS
# Find the native VORBIS headers and libraries.
#
#  VORBIS_INCLUDE_DIR -  where to find VORBIS.h, etc.
#  VORBIS_LIBRARIES    - List of libraries when using VORBIS.
#  VORBIS_FOUND        - True if VORBIS found.

GET_FILENAME_COMPONENT(module_file_path ${CMAKE_CURRENT_LIST_FILE} PATH )

# Look for the header file.
FIND_PATH(VORBIS_INCLUDE_DIR NAMES vorbis/vorbisfile.h
                             PATHS $ENV{H3D_EXTERNAL_ROOT}/include
                                   $ENV{H3D_ROOT}/../External/include
                                   ../../External/include
                                   ${module_file_path}/../../../External/include )
MARK_AS_ADVANCED(VORBIS_INCLUDE_DIR)

FIND_PATH(OGG_INCLUDE_DIR NAMES ogg/ogg.h
                          PATHS $ENV{H3D_EXTERNAL_ROOT}/include
                                $ENV{H3D_ROOT}/../External/include
                                ../../External/include
                                ${module_file_path}/../../../External/include )
MARK_AS_ADVANCED(OGG_INCLUDE_DIR)

# Look for the library.
FIND_LIBRARY(VORBISFILE_LIBRARY NAMES vorbisfile
                            PATHS $ENV{H3D_EXTERNAL_ROOT}/lib
                                  ../../External/lib
                                  ${module_file_path}/../../../External/lib )
MARK_AS_ADVANCED(VORBISFILE_LIBRARY)

FIND_LIBRARY(VORBIS_LIBRARY NAMES vorbis
                            PATHS $ENV{H3D_EXTERNAL_ROOT}/lib
                                  $ENV{H3D_ROOT}/../External/lib
                                  ../../External/lib
                                  ${module_file_path}/../../../External/lib)
MARK_AS_ADVANCED(VORBIS_LIBRARY)

FIND_LIBRARY(OGG_LIBRARY NAMES ogg 
                         PATHS $ENV{H3D_EXTERNAL_ROOT}/lib
                               $ENV{H3D_ROOT}/../External/lib
                               ../../External/lib
                               ${module_file_path}/../../../External/lib)
MARK_AS_ADVANCED(OGG_LIBRARY)

IF( WIN32 AND PREFER_STATIC_LIBRARIES )
  FIND_LIBRARY( VORBISFILE_STATIC_LIBRARY NAMES vorbisfile_static
                                         PATHS $ENV{H3D_EXTERNAL_ROOT}/lib
                                         $ENV{H3D_ROOT}/../External/lib
                                         ../../External/lib
                                         ${module_file_path}/../../../External/lib )
  MARK_AS_ADVANCED(VORBISFILE_STATIC_LIBRARY)
  
  FIND_LIBRARY( OGG_STATIC_LIBRARY NAMES ogg_static
                                         PATHS $ENV{H3D_EXTERNAL_ROOT}/lib
                                         $ENV{H3D_ROOT}/../External/lib
                                         ../../External/lib
                                         ${module_file_path}/../../../External/lib )
  MARK_AS_ADVANCED(OGG_STATIC_LIBRARY)
ENDIF( WIN32 AND PREFER_STATIC_LIBRARIES )

IF( VORBIS_LIBRARY AND OGG_LIBRARY AND VORBISFILE_LIBRARY )
  SET( VORBIS_LIBRARIES_FOUND 1 )
ENDIF( VORBIS_LIBRARY AND OGG_LIBRARY AND VORBISFILE_LIBRARY )

IF( VORBISFILE_STATIC_LIBRARY AND OGG_STATIC_LIBRARY )
  SET( VORBIS_LIBRARIES_FOUND 1 )
  SET( VORBIS_STATIC_LIBRARIES_FOUND 1 )
ENDIF( VORBISFILE_STATIC_LIBRARY AND OGG_STATIC_LIBRARY )

# Copy the results to the output variables.
IF(VORBIS_INCLUDE_DIR AND OGG_INCLUDE_DIR AND VORBIS_LIBRARIES_FOUND)
  SET(VORBIS_FOUND 1)
  IF( VORBIS_STATIC_LIBRARIES_FOUND )
    SET(VORBIS_LIBRARIES ${VORBISFILE_STATIC_LIBRARY} ${OGG_STATIC_LIBRARY} )
  ELSE( VORBIS_STATIC_LIBRARIES_FOUND )
    SET(VORBIS_LIBRARIES ${VORBISFILE_LIBRARY} ${VORBIS_LIBRARY} ${OGG_LIBRARY} )
  ENDIF( VORBIS_STATIC_LIBRARIES_FOUND )
  SET(VORBIS_INCLUDE_DIR ${VORBIS_INCLUDE_DIR} ${OGG_INCLUDE_DIR})
ELSE(VORBIS_INCLUDE_DIR AND OGG_INCLUDE_DIR AND VORBIS_LIBRARIES_FOUND)
  SET(VORBIS_FOUND 0)
  SET(VORBIS_LIBRARIES)
  SET(VORBIS_INCLUDE_DIR)
ENDIF(VORBIS_INCLUDE_DIR AND OGG_INCLUDE_DIR AND VORBIS_LIBRARIES_FOUND)

# Report the results.
IF(NOT VORBIS_FOUND)
  SET(VORBIS_DIR_MESSAGE
    "VORBIS was not found. Make sure VORBIS_LIBRARY and
    VORBIS_INCLUDE_DIR are set to the directories containing the include and lib files for ogg/vorbis. If you do not have the library you will not be able to use ogg files as sound.")
  IF(Vorbis_FIND_REQUIRED)
    MESSAGE(FATAL_ERROR "${VORBIS_DIR_MESSAGE}")
  ELSEIF(NOT Vorbis_FIND_QUIETLY)
    MESSAGE(STATUS "${VORBIS_DIR_MESSAGE}")
  ENDIF(Vorbis_FIND_REQUIRED)
ENDIF(NOT VORBIS_FOUND)
