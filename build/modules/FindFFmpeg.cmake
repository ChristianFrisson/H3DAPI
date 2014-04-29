# - Find FFmpeg
# Find the native FFmpeg headers and libraries.
#
#  FFMPEG_INCLUDE_DIR -  where to find avcodec.h, etc.
#  FFMPEG_LIBRARIES    - List of libraries when using FFmpeg.
#  FFMPEG_FOUND        - True if FFmpeg found.


IF(WIN32 OR APPLE)
  MESSAGE(FATAL_ERROR "FindFFmpeg not yet ready for Windows and Mac! Please contribute")
ENDIF(WIN32 OR APPLE)


# Try to use pkgconfig
#INCLUDE(FindPkgConfig)
# pkg-config is disabled for now since it does not seem to find
# the directories we want always. 
# explicitly set the PKG_CONFIG_FOUND to be False as it can be set to be
# True by other module before FindFFmpeg module
SET( PKG_CONFIG_FOUND False )
IF( PKG_CONFIG_FOUND )
  PKG_CHECK_MODULES(FFMPEGMODULES libavcodec libavformat libswscale)
ELSE( PKG_CONFIG_FOUND )

  # Look for the header file.
  FIND_PATH(FFMPEG_AVCODEC_INCLUDE_DIR NAMES avcodec.h
                               PATHS /usr/local/include/libavcodec
                                     /usr/include/libavcodec
                                     /usr/local/include/ffmpeg
                                     /usr/include/ffmpeg
                                     $ENV{H3D_EXTERNAL_ROOT}/include  
                                     $ENV{H3D_EXTERNAL_ROOT}/include/ffmpeg/include
                                     ../../External/include    
                                     ../../External/include/ffmpeg/include
                               DOC "Path in which the file avcodec.h is located." )
  MARK_AS_ADVANCED(FFMPEG_AVCODEC_INCLUDE_DIR)

  FIND_PATH(FFMPEG_AVFORMAT_INCLUDE_DIR NAMES avformat.h
                            PATHS /usr/local/include/libavformat
                                  /usr/include/libavformat
                                  /usr/local/include/ffmpeg
                                  /usr/include/ffmpeg
                                  /usr/include/libavformat
                                  $ENV{H3D_EXTERNAL_ROOT}/include  
                                  $ENV{H3D_EXTERNAL_ROOT}/include/ffmpeg/include
                                  ../../External/include    
                                  ../../External/include/ffmpeg/include
                            DOC "Path in which the file avformat.h is located." )
  MARK_AS_ADVANCED(FFMPEG_AVFORMAT_INCLUDE_DIR)

  FIND_PATH(FFMPEG_SWSCALE_INCLUDE_DIR NAMES swscale.h
                            PATHS /usr/local/include/libswscale
                                  /usr/include/libswscale
                                  /usr/local/include/ffmpeg
                                  /usr/include/ffmpeg
                                  /usr/include/libavformat
                                  $ENV{H3D_EXTERNAL_ROOT}/include  
                                  $ENV{H3D_EXTERNAL_ROOT}/include/ffmpeg/include
                                  ../../External/include    
                                  ../../External/include/ffmpeg/include
                            DOC "Path in which the file swscale.h is located." )
  MARK_AS_ADVANCED(FFMPEG_SWSCALE_INCLUDE_DIR)

  # Look for the libraries.
  FIND_LIBRARY(FFMPEG_AVCODEC_LIBRARY NAMES avcodec 
                           PATHS $ENV{H3D_EXTERNAL_ROOT}/lib
                                 ../../External/lib
                           DOC "Path to avcodec library." )
  MARK_AS_ADVANCED(FFMPEG_AVCODEC_LIBRARY)

  FIND_LIBRARY(FFMPEG_AVFORMAT_LIBRARY NAMES avformat
                              PATHS $ENV{H3D_EXTERNAL_ROOT}/lib
                                    ../../External/lib
                              DOC "Path to avformat library." )
  MARK_AS_ADVANCED(FFMPEG_AVFORMAT_LIBRARY)

  FIND_LIBRARY(FFMPEG_SWSCALE_LIBRARY NAMES swscale 
                           PATHS $ENV{H3D_EXTERNAL_ROOT}/lib
                                 ../../External/lib
                           DOC "Path to swscale library." )
  MARK_AS_ADVANCED(FFMPEG_SWSCALE_LIBRARY)

  IF( FFMPEG_AVCODEC_INCLUDE_DIR AND FFMPEG_AVFORMAT_INCLUDE_DIR AND FFMPEG_SWSCALE_INCLUDE_DIR AND
      FFMPEG_AVCODEC_LIBRARY AND FFMPEG_AVFORMAT_LIBRARY AND FFMPEG_SWSCALE_LIBRARY  )
    SET( FFMPEGMODULES_FOUND 1 )
    SET( FFMPEGMODULES_LIBRARIES ${FFMPEG_AVCODEC_LIBRARY} ${FFMPEG_AVFORMAT_LIBRARY} ${FFMPEG_SWSCALE_LIBRARY} )
    SET( FFMPEGMODULES_INCLUDE_DIRS ${FFMPEG_AVCODEC_INCLUDE_DIR} ${FFMPEG_AVFORMAT_INCLUDE_DIR} ${FFMPEG_SWSCALE_INCLUDE_DIR} )
  ENDIF( FFMPEG_AVCODEC_INCLUDE_DIR AND FFMPEG_AVFORMAT_INCLUDE_DIR AND FFMPEG_SWSCALE_INCLUDE_DIR AND
         FFMPEG_AVCODEC_LIBRARY AND FFMPEG_AVFORMAT_LIBRARY AND FFMPEG_SWSCALE_LIBRARY  )



ENDIF( PKG_CONFIG_FOUND )

# Copy the results to the output variables.
IF(FFMPEGMODULES_FOUND)
  SET(FFMPEG_FOUND 1)
  SET(FFMPEG_LIBRARIES ${FFMPEGMODULES_LIBRARIES})
  SET(FFMPEG_INCLUDE_DIRS ${FFMPEGMODULES_INCLUDE_DIRS})
ELSE(FFMPEGMODULES_FOUND)
  SET(FFMPEG_FOUND 0)
  SET(FFMPEG_LIBRARIES)
  SET(FFMPEG_INCLUDE_DIRS)
ENDIF(FFMPEGMODULES_FOUND)

MARK_AS_ADVANCED( FFMPEG_LIBRARIES )
MARK_AS_ADVANCED( FFMPEG_INCLUDE_DIRS )

# Report the results.
IF(NOT FFMPEG_FOUND)
  SET(FFMPEG_DIR_MESSAGE
    "FFmpeg was not found. Make sure cmake variables with prefix FFMPEG set to the directories containing the include and lib files for ffmpeg. If you do not have the library you will not be able to use video textures under linux.")
  IF(NOT FFMPEG_FIND_QUIETLY)
    MESSAGE(STATUS "${FFMPEG_DIR_MESSAGE}")
  ELSE(NOT FFMPEG_FIND_QUIETLY)
    IF(FFMPEG_FIND_REQUIRED)
      MESSAGE(FATAL_ERROR "${FFMPEG_DIR_MESSAGE}")
    ENDIF(FFMPEG_FIND_REQUIRED)
  ENDIF(NOT FFMPEG_FIND_QUIETLY)
ENDIF(NOT FFMPEG_FOUND)
