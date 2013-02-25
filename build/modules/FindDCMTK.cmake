# - find DCMTK libraries
#
#  DCMTK_INCLUDE_DIR   - Directories to include to use DCMTK
#  DCMTK_LIBRARIES     - Files to link against to use DCMTK
#  DCMTK_FOUND         - If false, don't try to use DCMTK
#  DCMTK_DIR           - (optional) Source directory for DCMTK
#
# DCMTK_DIR can be used to make it simpler to find the various include
# directories and compiled libraries if you've just compiled it in the
# source tree. Just set it to the root of the tree where you extracted
# the source.
#
# Written for VXL by Amitha Perera.
# 

SET( DCMTK_DIR "" CACHE PATH "Set this to the root of the installed dcmtk files to find include files and libraries." )
MARK_AS_ADVANCED(DCMTK_DIR)

GET_FILENAME_COMPONENT(module_file_path ${CMAKE_CURRENT_LIST_FILE} PATH )

IF( MSVC )
  SET( H3D_MSVC_VERSION 6 )
  SET( TEMP_MSVC_VERSION 1299 )
  WHILE( ${MSVC_VERSION} GREATER ${TEMP_MSVC_VERSION} )
    MATH( EXPR H3D_MSVC_VERSION "${H3D_MSVC_VERSION} + 1" )
    MATH( EXPR TEMP_MSVC_VERSION "${TEMP_MSVC_VERSION} + 100" )
  ENDWHILE( ${MSVC_VERSION} GREATER ${TEMP_MSVC_VERSION} )
  SET( DCMTK_LIBRARY_POSTFIX "_vc${H3D_MSVC_VERSION}" )
ENDIF( MSVC )

IF( CMAKE_CL_64 )
  SET( LIB "lib64" )
ELSE( CMAKE_CL_64 )
  SET( LIB "lib32" )
ENDIF( CMAKE_CL_64 )

IF( NOT WIN32 )
SET( DCMTK_HAVE_CONFIG_H "NO" CACHE BOOL "On some systems the compile flag -DHAVE_CONFIG_H needs to be defined because of DCMTK. If DCMTK headers are causing problem turn this flag on." )
IF( DCMTK_HAVE_CONFIG_H )
  SET(CMAKE_CXX_FLAGS "-DHAVE_CONFIG_H")
ENDIF( DCMTK_HAVE_CONFIG_H )
ENDIF( NOT WIN32 )

FIND_PATH( DCMTK_config_INCLUDE_DIR NAMES dcmtk/config/osconfig.h
           PATHS ${DCMTK_DIR}/config/include 
                 ${DCMTK_DIR}/include 
                 $ENV{H3D_EXTERNAL_ROOT}/include
                 $ENV{H3D_ROOT}/../External/include
                 ../../External/include
                 ${module_file_path}/../../../External/include
                 ${module_file_path}/../../dcmtk/include
                 /usr/local/dicom/include
           DOC "Path in which the file dcmtk/config/osconfig.h is located." )
MARK_AS_ADVANCED(DCMTK_config_INCLUDE_DIR)

FIND_PATH( DCMTK_ofstd_INCLUDE_DIR NAMES dcmtk/ofstd/ofstdinc.h
           PATHS ${DCMTK_DIR}/ofstd/include 
                 ${DCMTK_DIR}/include/ofstd
                 ${DCMTK_DIR}/include
                 $ENV{H3D_EXTERNAL_ROOT}/include
                 $ENV{H3D_ROOT}/../External/include
                 ../../External/include
                 ${module_file_path}/../../../External/include
                 ${module_file_path}/../../dcmtk/include
                 /usr/local/dicom/include
           DOC "Path in which the file dcmtk/ofstd/ofstdinc.h is located." )
MARK_AS_ADVANCED(DCMTK_ofstd_INCLUDE_DIR)

FIND_LIBRARY( DCMTK_ofstd_LIBRARY "ofstd${DCMTK_LIBRARY_POSTFIX}"
              PATHS ${DCMTK_DIR}/ofstd/libsrc
                    ${DCMTK_DIR}/ofstd/libsrc/Release
                    ${DCMTK_DIR}/ofstd/libsrc/Debug
                    ${DCMTK_DIR}/ofstd/Release
                    ${DCMTK_DIR}/ofstd/Debug
                    ${DCMTK_DIR}/lib
                    $ENV{H3D_EXTERNAL_ROOT}/${LIB}/static
                    $ENV{H3D_ROOT}/../External/${LIB}/static
                    ../../External/${LIB}/static
                    ${module_file_path}/../../../External/${LIB}/static
                    ${module_file_path}/../../dcmtk/lib
                    /usr/local/dicom/lib
              DOC "Path to ofstd${DCMTK_LIBRARY_POSTFIX} library." )
MARK_AS_ADVANCED(DCMTK_ofstd_LIBRARY)

FIND_PATH( DCMTK_oflog_INCLUDE_DIR NAMES dcmtk/oflog/oflog.h
           PATHS ${DCMTK_DIR}/oflog/include 
                 ${DCMTK_DIR}/include/oflog
                 ${DCMTK_DIR}/include
                 $ENV{H3D_EXTERNAL_ROOT}/include
                 $ENV{H3D_ROOT}/../External/include
                 ../../External/include
                 ${module_file_path}/../../../External/include
                 ${module_file_path}/../../dcmtk/include
                 /usr/local/dicom/include
           DOC "Path in which the file dcmtk/oflog/oflog.h is located." )
MARK_AS_ADVANCED(DCMTK_oflog_INCLUDE_DIR)

FIND_LIBRARY( DCMTK_oflog_LIBRARY "oflog${DCMTK_LIBRARY_POSTFIX}"
              PATHS ${DCMTK_DIR}/oflog/libsrc
                    ${DCMTK_DIR}/oflog/libsrc/Release
                    ${DCMTK_DIR}/oflog/libsrc/Debug
                    ${DCMTK_DIR}/oflog/Release
                    ${DCMTK_DIR}/oflog/Debug
                    ${DCMTK_DIR}/lib
                    $ENV{H3D_EXTERNAL_ROOT}/${LIB}/static
                    $ENV{H3D_ROOT}/../External/${LIB}/static
                    ../../External/${LIB}/static
                    ${module_file_path}/../../../External/${LIB}/static
                    ${module_file_path}/../../dcmtk/lib
                    /usr/local/dicom/lib
              DOC "Path to oflog${DCMTK_LIBRARY_POSTFIX} library." )
MARK_AS_ADVANCED(DCMTK_oflog_LIBRARY)

FIND_PATH( DCMTK_dcmjpeg_INCLUDE_DIR NAMES dcmtk/dcmjpeg/djdecode.h
           PATHS ${DCMTK_DIR}/dcmjpeg/include 
                 ${DCMTK_DIR}/include/dcmjpeg
                 ${DCMTK_DIR}/include
                 $ENV{H3D_EXTERNAL_ROOT}/include
                 $ENV{H3D_ROOT}/../External/include
                 ../../External/include
                 ${module_file_path}/../../../External/include
                 ${module_file_path}/../../dcmtk/include
                 /usr/local/dicom/include
           DOC "Path in which the file dcmtk/dcmjpeg/djdecode.h is located." )
MARK_AS_ADVANCED(DCMTK_dcmjpeg_INCLUDE_DIR)

FIND_LIBRARY( DCMTK_dcmjpeg_LIBRARY dcmjpeg${DCMTK_LIBRARY_POSTFIX}
              PATHS ${DCMTK_DIR}/dcmjpeg/libsrc
                    ${DCMTK_DIR}/dcmjpeg/libsrc/Release
                    ${DCMTK_DIR}/dcmjpeg/libsrc/Debug
                    ${DCMTK_DIR}/dcmjpeg/Release
                    ${DCMTK_DIR}/dcmjpeg/Debug
                    ${DCMTK_DIR}/lib
                    $ENV{H3D_EXTERNAL_ROOT}/${LIB}/static
                    $ENV{H3D_ROOT}/../External/${LIB}/static
                    ../../External/${LIB}/static
                    ${module_file_path}/../../../External/${LIB}/static
                    ${module_file_path}/../../dcmtk/lib
                    /usr/local/dicom/lib
              DOC "Path to dcmjpeg${DCMTK_LIBRARY_POSTFIX} library." )
MARK_AS_ADVANCED(DCMTK_dcmjpeg_LIBRARY)


FIND_LIBRARY( DCMTK_ijg8_LIBRARY ijg8${DCMTK_LIBRARY_POSTFIX}
              PATHS ${DCMTK_DIR}/dcmjpeg/libijg8/libsrc
                    ${DCMTK_DIR}/dcmjpeg/libijg8/libsrc/Release
                    ${DCMTK_DIR}/dcmjpeg/libijg8/libsrc/Debug
                    ${DCMTK_DIR}/dcmjpeg/libijg8/Release
                    ${DCMTK_DIR}/dcmjpeg/libijg8/Debug
                    ${DCMTK_DIR}/lib
                    $ENV{H3D_EXTERNAL_ROOT}/${LIB}/static
                    $ENV{H3D_ROOT}/../External/${LIB}/static
                    ../../External/${LIB}/static
                    ${module_file_path}/../../../External/${LIB}/static
                    ${module_file_path}/../../dcmtk/lib
                    /usr/local/dicom/lib
              DOC "Path to ijg8${DCMTK_LIBRARY_POSTFIX} library." )
MARK_AS_ADVANCED(DCMTK_ijg8_LIBRARY)

FIND_LIBRARY( DCMTK_ijg12_LIBRARY ijg12${DCMTK_LIBRARY_POSTFIX}
              PATHS ${DCMTK_DIR}/dcmjpeg/libijg12/libsrc
                    ${DCMTK_DIR}/dcmjpeg/libijg12/libsrc/Release
                    ${DCMTK_DIR}/dcmjpeg/libijg12/libsrc/Debug
                    ${DCMTK_DIR}/dcmjpeg/libijg12/Release
                    ${DCMTK_DIR}/dcmjpeg/libijg12/Debug
                    ${DCMTK_DIR}/lib
                    $ENV{H3D_EXTERNAL_ROOT}/${LIB}/static
                    $ENV{H3D_ROOT}/../External/${LIB}/static
                    ../../External/${LIB}/static
                    ${module_file_path}/../../../External/${LIB}/static
                    ${module_file_path}/../../dcmtk/lib 
                    /usr/local/dicom/lib
              DOC "Path to ijg12${DCMTK_LIBRARY_POSTFIX} library." )
MARK_AS_ADVANCED(DCMTK_ijg12_LIBRARY)

FIND_LIBRARY( DCMTK_ijg16_LIBRARY ijg16${DCMTK_LIBRARY_POSTFIX}
              PATHS ${DCMTK_DIR}/dcmjpeg/libijg16/libsrc
                    ${DCMTK_DIR}/dcmjpeg/libijg16/libsrc/Release
                    ${DCMTK_DIR}/dcmjpeg/libijg16/libsrc/Debug
                    ${DCMTK_DIR}/dcmjpeg/libijg16/Release
                    ${DCMTK_DIR}/dcmjpeg/libijg16/Debug
                    ${DCMTK_DIR}/lib
                    $ENV{H3D_EXTERNAL_ROOT}/${LIB}/static
                    $ENV{H3D_ROOT}/../External/${LIB}/static
                    ../../External/${LIB}/static
                    ${module_file_path}/../../../External/${LIB}/static
                    ${module_file_path}/../../dcmtk/lib 
                    /usr/local/dicom/lib
              DOC "Path to ijg16${DCMTK_LIBRARY_POSTFIX} library." )
MARK_AS_ADVANCED(DCMTK_ijg16_LIBRARY)

FIND_PATH( DCMTK_dcmdata_INCLUDE_DIR dcmtk/dcmdata/dctypes.h
           PATHS ${DCMTK_DIR}/dcmdata/include
                 ${DCMTK_DIR}/include/dcmdata
                 ${DCMTK_DIR}/include
                 $ENV{H3D_EXTERNAL_ROOT}/include
                 $ENV{H3D_ROOT}/../External/include
                 ../../External/include
                 ${module_file_path}/../../../External/include
                 ${module_file_path}/../../dcmtk/include
                 /usr/local/dicom/include
           DOC "Path in which the file dcmtk/dcmdata/dctypes.h is located." )
MARK_AS_ADVANCED(DCMTK_dcmdata_INCLUDE_DIR)

FIND_LIBRARY( DCMTK_dcmdata_LIBRARY dcmdata${DCMTK_LIBRARY_POSTFIX}
              PATHS ${DCMTK_DIR}/dcmdata/libsrc
                    ${DCMTK_DIR}/dcmdata/libsrc/Release
                    ${DCMTK_DIR}/dcmdata/libsrc/Debug
                    ${DCMTK_DIR}/dcmdata/Release
                    ${DCMTK_DIR}/dcmdata/Debug
                    ${DCMTK_DIR}/lib
                    $ENV{H3D_EXTERNAL_ROOT}/${LIB}/static
                    $ENV{H3D_ROOT}/../External/${LIB}/static
                    ../../External/${LIB}/static
                    ${module_file_path}/../../../External/${LIB}/static
                    ${module_file_path}/../../dcmtk/lib 
                    /usr/local/dicom/lib
              DOC "Path to dcmdata${DCMTK_LIBRARY_POSTFIX} library." )
MARK_AS_ADVANCED(DCMTK_dcmdata_LIBRARY)


FIND_PATH( DCMTK_dcmimgle_INCLUDE_DIR dcmtk/dcmimgle/dcmimage.h
           PATHS ${DCMTK_DIR}/dcmimgle/include
                 ${DCMTK_DIR}/include/dcmimgle
                 ${DCMTK_DIR}/include
                 $ENV{H3D_EXTERNAL_ROOT}/include
                 $ENV{H3D_ROOT}/../External/include
                 ../../External/include
                 ${module_file_path}/../../../External/include
                 ${module_file_path}/../../dcmtk/include 
                 /usr/local/dicom/include
           DOC "Path in which the file dcmtk/dcmimgle/dcmimage.h is located." )
MARK_AS_ADVANCED(DCMTK_dcmimgle_INCLUDE_DIR)

FIND_LIBRARY( DCMTK_dcmimgle_LIBRARY dcmimgle${DCMTK_LIBRARY_POSTFIX}
              PATHS ${DCMTK_DIR}/dcmimgle/libsrc
                    ${DCMTK_DIR}/dcmimgle/libsrc/Release
                    ${DCMTK_DIR}/dcmimgle/libsrc/Debug
                    ${DCMTK_DIR}/dcmimgle/Release
                    ${DCMTK_DIR}/dcmimgle/Debug
                    ${DCMTK_DIR}/lib
                    $ENV{H3D_EXTERNAL_ROOT}/${LIB}/static
                    $ENV{H3D_ROOT}/../External/${LIB}/static
                    ../../External/${LIB}/static
                    ${module_file_path}/../../../External/${LIB}/static
                    ${module_file_path}/../../dcmtk/lib
                    /usr/local/dicom/lib
              DOC "Path to dcmimgle${DCMTK_LIBRARY_POSTFIX} library." )
MARK_AS_ADVANCED(DCMTK_dcmimgle_LIBRARY)

FIND_LIBRARY( DCMTK_dcmimage_LIBRARY dcmimage${DCMTK_LIBRARY_POSTFIX}
              PATHS ${DCMTK_DIR}/dcmimage/libsrc
                    ${DCMTK_DIR}/dcmimage/libsrc/Release
                    ${DCMTK_DIR}/dcmimage/libsrc/Debug
                    ${DCMTK_DIR}/dcmimage/Release
                    ${DCMTK_DIR}/dcmimage/Debug
                    ${DCMTK_DIR}/lib
                    $ENV{H3D_EXTERNAL_ROOT}/${LIB}/static
                    $ENV{H3D_ROOT}/../External/${LIB}/static
                    ../../External/${LIB}/static
                    ${module_file_path}/../../../External/${LIB}/static
                    ${module_file_path}/../../dcmtk/lib
                    /usr/local/dicom/lib
              DOC "Path to dcmimage${DCMTK_LIBRARY_POSTFIX} library." )
MARK_AS_ADVANCED(DCMTK_dcmimage_LIBRARY)

IF( MSVC10 )
  # Visual Studio 2010 needs debug versions to compile in debug
  FIND_LIBRARY( DCMTK_ofstd_DEBUG_LIBRARY "ofstd${DCMTK_LIBRARY_POSTFIX}_d"
                PATHS ${DCMTK_DIR}/ofstd/libsrc
                      ${DCMTK_DIR}/ofstd/libsrc/Release
                      ${DCMTK_DIR}/ofstd/libsrc/Debug
                      ${DCMTK_DIR}/ofstd/Release
                      ${DCMTK_DIR}/ofstd/Debug
                      ${DCMTK_DIR}/lib
                      $ENV{H3D_EXTERNAL_ROOT}/${LIB}/static
                      $ENV{H3D_ROOT}/../External/${LIB}/static
                      ../../External/${LIB}/static
                      ${module_file_path}/../../../External/${LIB}/static
                      ${module_file_path}/../../dcmtk/lib
                      /usr/local/dicom/lib
                DOC "Path to ofstd${DCMTK_LIBRARY_POSTFIX}_d library." )
  MARK_AS_ADVANCED(DCMTK_ofstd_DEBUG_LIBRARY)

  FIND_LIBRARY( DCMTK_dcmjpeg_DEBUG_LIBRARY dcmjpeg${DCMTK_LIBRARY_POSTFIX}_d
                PATHS ${DCMTK_DIR}/dcmjpeg/libsrc
                      ${DCMTK_DIR}/dcmjpeg/libsrc/Release
                      ${DCMTK_DIR}/dcmjpeg/libsrc/Debug
                      ${DCMTK_DIR}/dcmjpeg/Release
                      ${DCMTK_DIR}/dcmjpeg/Debug
                      ${DCMTK_DIR}/lib
                      $ENV{H3D_EXTERNAL_ROOT}/${LIB}/static
                      $ENV{H3D_ROOT}/../External/${LIB}/static
                      ../../External/${LIB}/static
                      ${module_file_path}/../../../External/${LIB}/static
                      ${module_file_path}/../../dcmtk/lib
                      /usr/local/dicom/lib
                DOC "Path to dcmjpeg${DCMTK_LIBRARY_POSTFIX}_d library." )
  MARK_AS_ADVANCED(DCMTK_dcmjpeg_DEBUG_LIBRARY)


  FIND_LIBRARY( DCMTK_ijg8_DEBUG_LIBRARY ijg8${DCMTK_LIBRARY_POSTFIX}_d
                PATHS ${DCMTK_DIR}/dcmjpeg/libijg8/libsrc
                      ${DCMTK_DIR}/dcmjpeg/libijg8/libsrc/Release
                      ${DCMTK_DIR}/dcmjpeg/libijg8/libsrc/Debug
                      ${DCMTK_DIR}/dcmjpeg/libijg8/Release
                      ${DCMTK_DIR}/dcmjpeg/libijg8/Debug
                      ${DCMTK_DIR}/lib
                      $ENV{H3D_EXTERNAL_ROOT}/${LIB}/static
                      $ENV{H3D_ROOT}/../External/${LIB}/static
                      ../../External/${LIB}/static
                      ${module_file_path}/../../../External/${LIB}/static
                      ${module_file_path}/../../dcmtk/lib
                      /usr/local/dicom/lib
                DOC "Path to ijg8${DCMTK_LIBRARY_POSTFIX}_d library." )
  MARK_AS_ADVANCED(DCMTK_ijg8_DEBUG_LIBRARY)

  FIND_LIBRARY( DCMTK_ijg12_DEBUG_LIBRARY ijg12${DCMTK_LIBRARY_POSTFIX}_d
                PATHS ${DCMTK_DIR}/dcmjpeg/libijg12/libsrc
                      ${DCMTK_DIR}/dcmjpeg/libijg12/libsrc/Release
                      ${DCMTK_DIR}/dcmjpeg/libijg12/libsrc/Debug
                      ${DCMTK_DIR}/dcmjpeg/libijg12/Release
                      ${DCMTK_DIR}/dcmjpeg/libijg12/Debug
                      ${DCMTK_DIR}/lib
                      $ENV{H3D_EXTERNAL_ROOT}/${LIB}/static
                      $ENV{H3D_ROOT}/../External/${LIB}/static
                      ../../External/${LIB}/static
                      ${module_file_path}/../../../External/${LIB}/static
                      ${module_file_path}/../../dcmtk/lib
                      /usr/local/dicom/lib
                DOC "Path to ijg12${DCMTK_LIBRARY_POSTFIX}_d library." )
  MARK_AS_ADVANCED(DCMTK_ijg12_DEBUG_LIBRARY)

  FIND_LIBRARY( DCMTK_ijg16_DEBUG_LIBRARY ijg16${DCMTK_LIBRARY_POSTFIX}_d
                PATHS ${DCMTK_DIR}/dcmjpeg/libijg16/libsrc
                      ${DCMTK_DIR}/dcmjpeg/libijg16/libsrc/Release
                      ${DCMTK_DIR}/dcmjpeg/libijg16/libsrc/Debug
                      ${DCMTK_DIR}/dcmjpeg/libijg16/Release
                      ${DCMTK_DIR}/dcmjpeg/libijg16/Debug
                      ${DCMTK_DIR}/lib
                      $ENV{H3D_EXTERNAL_ROOT}/${LIB}/static
                      $ENV{H3D_ROOT}/../External/${LIB}/static
                      ../../External/${LIB}/static
                      ${module_file_path}/../../../External/${LIB}/static
                      ${module_file_path}/../../dcmtk/lib 
                      /usr/local/dicom/lib
                DOC "Path to ijg16${DCMTK_LIBRARY_POSTFIX}_d library." )
  MARK_AS_ADVANCED(DCMTK_ijg16_DEBUG_LIBRARY)

  FIND_LIBRARY( DCMTK_dcmdata_DEBUG_LIBRARY dcmdata${DCMTK_LIBRARY_POSTFIX}_d
                PATHS ${DCMTK_DIR}/dcmdata/libsrc
                      ${DCMTK_DIR}/dcmdata/libsrc/Release
                      ${DCMTK_DIR}/dcmdata/libsrc/Debug
                      ${DCMTK_DIR}/dcmdata/Release
                      ${DCMTK_DIR}/dcmdata/Debug
                      ${DCMTK_DIR}/lib
                      $ENV{H3D_EXTERNAL_ROOT}/${LIB}/static
                      $ENV{H3D_ROOT}/../External/${LIB}/static
                      ../../External/${LIB}/static
                      ${module_file_path}/../../../External/${LIB}/static
                      ${module_file_path}/../../dcmtk/lib 
                      /usr/local/dicom/lib
                DOC "Path to dcmdata${DCMTK_LIBRARY_POSTFIX}_d library." )
  MARK_AS_ADVANCED(DCMTK_dcmdata_DEBUG_LIBRARY)

  FIND_LIBRARY( DCMTK_dcmimgle_DEBUG_LIBRARY dcmimgle${DCMTK_LIBRARY_POSTFIX}_d
                PATHS ${DCMTK_DIR}/dcmimgle/libsrc
                      ${DCMTK_DIR}/dcmimgle/libsrc/Release
                      ${DCMTK_DIR}/dcmimgle/libsrc/Debug
                      ${DCMTK_DIR}/dcmimgle/Release
                      ${DCMTK_DIR}/dcmimgle/Debug
                      ${DCMTK_DIR}/lib
                      $ENV{H3D_EXTERNAL_ROOT}/${LIB}/static
                      $ENV{H3D_ROOT}/../External/${LIB}/static
                      ../../External/${LIB}/static
                      ${module_file_path}/../../../External/${LIB}/static
                      ${module_file_path}/../../dcmtk/lib
                      /usr/local/dicom/lib
                DOC "Path to dcmimgle${DCMTK_LIBRARY_POSTFIX}_d library." )
  MARK_AS_ADVANCED(DCMTK_dcmimgle_DEBUG_LIBRARY)

  FIND_LIBRARY( DCMTK_dcmimage_DEBUG_LIBRARY dcmimage${DCMTK_LIBRARY_POSTFIX}_d
                PATHS ${DCMTK_DIR}/dcmimage/libsrc
                      ${DCMTK_DIR}/dcmimage/libsrc/Release
                      ${DCMTK_DIR}/dcmimage/libsrc/Debug
                      ${DCMTK_DIR}/dcmimage/Release
                      ${DCMTK_DIR}/dcmimage/Debug
                      ${DCMTK_DIR}/lib
                      $ENV{H3D_EXTERNAL_ROOT}/${LIB}/static
                      $ENV{H3D_ROOT}/../External/${LIB}/static
                      ../../External/${LIB}/static
                      ${module_file_path}/../../../External/${LIB}/static
                      ${module_file_path}/../../dcmtk/lib
                      /usr/local/dicom/lib
                DOC "Path to dcmimage${DCMTK_LIBRARY_POSTFIX}_d library." )
  MARK_AS_ADVANCED(DCMTK_dcmimage_DEBUG_LIBRARY)
ENDIF( MSVC10 )


#FIND_LIBRARY(DCMTK_dcmnet_LIBRARY dcmnet${DCMTK_LIBRARY_POSTFIX}
#${DCMTK_DIR}/dcmnet/libsrc/Release
#${DCMTK_DIR}/dcmnet/libsrc/Debug
#${DCMTK_DIR}/dcmnet/libsrc/
#$ENV{H3D_EXTERNAL_ROOT}/${LIB}/static
#$ENV{H3D_ROOT}/../External/${LIB}/static
#../../External/${LIB}/static
#${module_file_path}/../../../External/${LIB}/static
#)


SET( HAVE_INCLUDE_DIRS "FALSE" )
SET( HAVE_RELEASE_LIBS "FALSE" )
SET( HAVE_DEBUG_LIBS "FALSE" )

# check that we have all include dirs
IF( DCMTK_config_INCLUDE_DIR AND  
    DCMTK_ofstd_INCLUDE_DIR AND 
    DCMTK_dcmdata_INCLUDE_DIR AND 
    DCMTK_dcmimgle_INCLUDE_DIR AND
    DCMTK_dcmjpeg_INCLUDE_DIR )
  SET( HAVE_INCLUDE_DIRS "TRUE" )
ENDIF( DCMTK_config_INCLUDE_DIR AND  
       DCMTK_ofstd_INCLUDE_DIR AND 
       DCMTK_dcmdata_INCLUDE_DIR AND 
       DCMTK_dcmimgle_INCLUDE_DIR AND
       DCMTK_dcmjpeg_INCLUDE_DIR )

# check that we have all release libs
IF( DCMTK_ofstd_LIBRARY AND 
    DCMTK_dcmdata_LIBRARY AND 
    DCMTK_dcmimgle_LIBRARY AND
    DCMTK_dcmimage_LIBRARY AND
    DCMTK_dcmjpeg_LIBRARY )
  SET( HAVE_RELEASE_LIBS "TRUE" )
ENDIF( DCMTK_ofstd_LIBRARY AND 
       DCMTK_dcmdata_LIBRARY AND 
       DCMTK_dcmimgle_LIBRARY AND
       DCMTK_dcmimage_LIBRARY AND
       DCMTK_dcmjpeg_LIBRARY )

# check if we have the debug libs
IF( DCMTK_ofstd_DEBUG_LIBRARY AND 
    DCMTK_dcmdata_DEBUG_LIBRARY AND 
    DCMTK_dcmimgle_DEBUG_LIBRARY AND
    DCMTK_dcmimage_DEBUG_LIBRARY AND
    DCMTK_dcmjpeg_DEBUG_LIBRARY )
  SET( HAVE_DEBUG_LIBS "TRUE" )
ENDIF( DCMTK_ofstd_DEBUG_LIBRARY AND 
       DCMTK_dcmdata_DEBUG_LIBRARY AND 
       DCMTK_dcmimgle_DEBUG_LIBRARY AND
       DCMTK_dcmimage_DEBUG_LIBRARY AND
       DCMTK_dcmjpeg_DEBUG_LIBRARY )

SET( DCMTK_Config_file_name "dummy" )
IF( WIN32 )
  SET( DCMTK_Config_file_name "cfwin32" )
ELSE( WIN32 )
  SET( DCMTK_Config_file_name "cfunix" )
ENDIF( WIN32 )

SET( HAVE_TIFF_OR_NO_TIFF_NEEDED 1 )
IF( HAVE_INCLUDE_DIRS AND HAVE_RELEASE_LIBS AND EXISTS ${DCMTK_config_INCLUDE_DIR}/dcmtk/config/${DCMTK_Config_file_name}.h )
  
  SET( regex_to_find "#define[ ]+WITH_LIBTIFF" )
  FILE( STRINGS ${DCMTK_config_INCLUDE_DIR}/dcmtk/config/${DCMTK_Config_file_name}.h list_of_defines REGEX ${regex_to_find} )
  LIST( LENGTH list_of_defines list_of_defines_length )
  IF( list_of_defines_length )
    # Dicom is compiled with tiff support. Find libtiff as well.
    FIND_PACKAGE( TIFF )
    IF(NOT TIFF_FOUND)
      SET( HAVE_TIFF_OR_NO_TIFF_NEEDED 0 )
    ENDIF(NOT TIFF_FOUND)
  ENDIF( list_of_defines_length )
ENDIF( HAVE_INCLUDE_DIRS AND HAVE_RELEASE_LIBS AND EXISTS ${DCMTK_config_INCLUDE_DIR}/dcmtk/config/${DCMTK_Config_file_name}.h )
       
IF( HAVE_INCLUDE_DIRS AND HAVE_RELEASE_LIBS AND HAVE_TIFF_OR_NO_TIFF_NEEDED )
  SET( DCMTK_FOUND "YES" )
  SET( DCMTK_INCLUDE_DIR
    ${DCMTK_config_INCLUDE_DIR}
    ${DCMTK_ofstd_INCLUDE_DIR}
    ${DCMTK_dcmdata_INCLUDE_DIR}
    ${DCMTK_dcmimgle_INCLUDE_DIR}
    ${DCMTK_dcmjpeg_INCLUDE_DIR}
  )

  IF( MSVC10 AND HAVE_DEBUG_LIBS )
    # MSVC10 needs debug libraries since it cannot compile with
    # the release versions
    SET( DCMTK_LIBRARIES 
         optimized ${DCMTK_dcmimgle_LIBRARY}
         optimized ${DCMTK_dcmdata_LIBRARY}
         optimized ${DCMTK_ofstd_LIBRARY}
         optimized ${DCMTK_dcmimage_LIBRARY}
         optimized ${DCMTK_dcmjpeg_LIBRARY}
         optimized ${DCMTK_ijg8_LIBRARY}
         optimized ${DCMTK_ijg12_LIBRARY}
         optimized ${DCMTK_ijg16_LIBRARY}
        )

     SET( DCMTK_LIBRARIES  ${DCMTK_LIBRARIES} 
         debug ${DCMTK_dcmimgle_DEBUG_LIBRARY}
         debug ${DCMTK_dcmdata_DEBUG_LIBRARY}
         debug ${DCMTK_ofstd_DEBUG_LIBRARY}
         debug ${DCMTK_dcmimage_DEBUG_LIBRARY}
         debug ${DCMTK_dcmjpeg_DEBUG_LIBRARY}
         debug ${DCMTK_ijg8_DEBUG_LIBRARY}
         debug ${DCMTK_ijg12_DEBUG_LIBRARY}
         debug ${DCMTK_ijg16_DEBUG_LIBRARY}
        )

  ELSE( MSVC10 AND HAVE_DEBUG_LIBS)
    
    IF( MSVC10 )
      MESSAGE( STATUS "DCMTK debug libraries not found. Debug compilation might not work with DCMTK." )
    ENDIF( MSVC10 )


    SET( DCMTK_LIBRARIES 
         ${DCMTK_dcmimgle_LIBRARY}
         ${DCMTK_dcmdata_LIBRARY}
         ${DCMTK_ofstd_LIBRARY}
         ${DCMTK_dcmimage_LIBRARY}
         ${DCMTK_dcmjpeg_LIBRARY}
         ${DCMTK_ijg8_LIBRARY}
         ${DCMTK_ijg12_LIBRARY}
         ${DCMTK_ijg16_LIBRARY}
        )
  ENDIF( MSVC10 AND HAVE_DEBUG_LIBS)



  IF(DCMTK_imagedb_LIBRARY)
   SET( DCMTK_LIBRARIES
   ${DCMTK_LIBRARIES}
   ${DCMTK_imagedb_LIBRARY}
   )
  ENDIF(DCMTK_imagedb_LIBRARY)

  IF(DCMTK_dcmnet_LIBRARY)
   SET( DCMTK_LIBRARIES
   ${DCMTK_LIBRARIES}
   ${DCMTK_dcmnet_LIBRARY}
   )
  ENDIF(DCMTK_dcmnet_LIBRARY)

  IF(DCMTK_oflog_LIBRARY)
   SET( DCMTK_LIBRARIES
   ${DCMTK_LIBRARIES}
   ${DCMTK_oflog_LIBRARY}
   )
  ENDIF(DCMTK_oflog_LIBRARY)

  IF( WIN32 )
    SET( DCMTK_LIBRARIES ${DCMTK_LIBRARIES} netapi32 ws2_32 )
  ENDIF( WIN32 )
  
  IF( TIFF_FOUND )
    SET( DCMTK_INCLUDE_DIR ${DCMTK_INCLUDE_DIR} ${TIFF_INCLUDE_DIR} )
    SET( DCMTK_LIBRARIES ${DCMTK_LIBRARIES} ${TIFF_LIBRARIES} )
  ENDIF( TIFF_FOUND )

ENDIF( HAVE_INCLUDE_DIRS AND HAVE_RELEASE_LIBS AND HAVE_TIFF_OR_NO_TIFF_NEEDED )

# Report the results.
IF(NOT DCMTK_FOUND)
  SET( DCMTK_DIR_MESSAGE
       "DCMTK was not found. Make sure all cmake variables with prefix DCMTK_ set, see each one for description.")
  IF( NOT HAVE_TIFF_OR_NO_TIFF_NEEDED )
    SET( DCMTK_DIR_MESSAGE "${DCMTK_DIR_MESSAGE} Also make sure that TIFF is found." )
  ENDIF( NOT HAVE_TIFF_OR_NO_TIFF_NEEDED )
  IF(DCMTK_FIND_REQUIRED)
    SET( DCMTK_DIR_MESSAGE
       "${DCMTK_DIR_MESSAGE} You need the Dicom Toolkit libraries and headers to compile.")
    MESSAGE(FATAL_ERROR "${DCMTK_DIR_MESSAGE}")
  ELSEIF(NOT DCMTK_FIND_QUIETLY)
    SET( DCMTK_DIR_MESSAGE
       "${DCMTK_DIR_MESSAGE} If you do not have the Dicom Toolkit libraries and headers you will not be able to load dicom images.")
    MESSAGE(STATUS "${DCMTK_DIR_MESSAGE}")
  ENDIF(DCMTK_FIND_REQUIRED)
ENDIF(NOT DCMTK_FOUND)

