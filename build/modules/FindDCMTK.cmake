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

SET( DCMTK_LIBRARY_POSTFIX "" )

IF( MSVC80 )
  SET( DCMTK_LIBRARY_POSTFIX "_vc8" )
ELSEIF( MSVC90 )
  SET( DCMTK_LIBRARY_POSTFIX "_vc9" )
ENDIF( MSVC80 )

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
		 /usr/local/dicom/include )
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
                 /usr/local/dicom/include )
MARK_AS_ADVANCED(DCMTK_ofstd_INCLUDE_DIR)

FIND_LIBRARY( DCMTK_ofstd_LIBRARY "ofstd${DCMTK_LIBRARY_POSTFIX}"
              PATHS ${DCMTK_DIR}/ofstd/libsrc
                    ${DCMTK_DIR}/ofstd/libsrc/Release
                    ${DCMTK_DIR}/ofstd/libsrc/Debug
                    ${DCMTK_DIR}/ofstd/Release
                    ${DCMTK_DIR}/ofstd/Debug
                    ${DCMTK_DIR}/lib
                    $ENV{H3D_EXTERNAL_ROOT}/lib
                    $ENV{H3D_ROOT}/../External/lib
                    ../../External/lib
                    ${module_file_path}/../../../External/lib
                    ${module_file_path}/../../dcmtk/lib
                    /usr/local/dicom/lib )
MARK_AS_ADVANCED(DCMTK_ofstd_LIBRARY)

FIND_PATH( DCMTK_dcmjpeg_INCLUDE_DIR NAMES dcmtk/dcmjpeg/djdecode.h
           PATHS ${DCMTK_DIR}/dcmjpeg/include 
                 ${DCMTK_DIR}/include/dcmjpeg
                 ${DCMTK_DIR}/include
                 $ENV{H3D_EXTERNAL_ROOT}/include
                 $ENV{H3D_ROOT}/../External/include
                 ../../External/include
                 ${module_file_path}/../../../External/include
                 ${module_file_path}/../../dcmtk/include
		 /usr/local/dicom/include )
MARK_AS_ADVANCED(DCMTK_dcmjpeg_INCLUDE_DIR)

FIND_LIBRARY( DCMTK_dcmjpeg_LIBRARY dcmjpeg${DCMTK_LIBRARY_POSTFIX}
              PATHS ${DCMTK_DIR}/dcmjpeg/libsrc
                    ${DCMTK_DIR}/dcmjpeg/libsrc/Release
                    ${DCMTK_DIR}/dcmjpeg/libsrc/Debug
                    ${DCMTK_DIR}/dcmjpeg/Release
                    ${DCMTK_DIR}/dcmjpeg/Debug
                    ${DCMTK_DIR}/lib
                    $ENV{H3D_EXTERNAL_ROOT}/lib
                    $ENV{H3D_ROOT}/../External/lib
                    ../../External/lib
                    ${module_file_path}/../../../External/lib
                    ${module_file_path}/../../dcmtk/lib
		    /usr/local/dicom/lib )
MARK_AS_ADVANCED(DCMTK_dcmjpeg_LIBRARY)


FIND_LIBRARY( DCMTK_ijg8_LIBRARY ijg8${DCMTK_LIBRARY_POSTFIX}
              PATHS ${DCMTK_DIR}/dcmjpeg/libijg8/libsrc
                    ${DCMTK_DIR}/dcmjpeg/libijg8/libsrc/Release
                    ${DCMTK_DIR}/dcmjpeg/libijg8/libsrc/Debug
                    ${DCMTK_DIR}/dcmjpeg/libijg8/Release
                    ${DCMTK_DIR}/dcmjpeg/libijg8/Debug
                    ${DCMTK_DIR}/lib
                    $ENV{H3D_EXTERNAL_ROOT}/lib
                    $ENV{H3D_ROOT}/../External/lib
                    ../../External/lib
                    ${module_file_path}/../../../External/lib
                    ${module_file_path}/../../dcmtk/lib
		    /usr/local/dicom/lib )
MARK_AS_ADVANCED(DCMTK_ijg8_LIBRARY)

FIND_LIBRARY( DCMTK_ijg12_LIBRARY ijg12${DCMTK_LIBRARY_POSTFIX}
              PATHS ${DCMTK_DIR}/dcmjpeg/libijg12/libsrc
                    ${DCMTK_DIR}/dcmjpeg/libijg12/libsrc/Release
                    ${DCMTK_DIR}/dcmjpeg/libijg12/libsrc/Debug
                    ${DCMTK_DIR}/dcmjpeg/libijg12/Release
                    ${DCMTK_DIR}/dcmjpeg/libijg12/Debug
                    ${DCMTK_DIR}/lib
                    $ENV{H3D_EXTERNAL_ROOT}/lib
                    $ENV{H3D_ROOT}/../External/lib
                    ../../External/lib
                    ${module_file_path}/../../../External/lib
                    ${module_file_path}/../../dcmtk/lib 
		    /usr/local/dicom/lib )
MARK_AS_ADVANCED(DCMTK_ijg12_LIBRARY)

FIND_LIBRARY( DCMTK_ijg16_LIBRARY ijg16${DCMTK_LIBRARY_POSTFIX}
              PATHS ${DCMTK_DIR}/dcmjpeg/libijg16/libsrc
                    ${DCMTK_DIR}/dcmjpeg/libijg16/libsrc/Release
                    ${DCMTK_DIR}/dcmjpeg/libijg16/libsrc/Debug
                    ${DCMTK_DIR}/dcmjpeg/libijg16/Release
                    ${DCMTK_DIR}/dcmjpeg/libijg16/Debug
                    ${DCMTK_DIR}/lib
                    $ENV{H3D_EXTERNAL_ROOT}/lib
                    $ENV{H3D_ROOT}/../External/lib
                    ../../External/lib
                    ${module_file_path}/../../../External/lib
                    ${module_file_path}/../../dcmtk/lib 
		    /usr/local/dicom/lib )
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
		 /usr/local/dicom/include )
MARK_AS_ADVANCED(DCMTK_dcmdata_INCLUDE_DIR)

FIND_LIBRARY( DCMTK_dcmdata_LIBRARY dcmdata${DCMTK_LIBRARY_POSTFIX}
              PATHS ${DCMTK_DIR}/dcmdata/libsrc
                    ${DCMTK_DIR}/dcmdata/libsrc/Release
                    ${DCMTK_DIR}/dcmdata/libsrc/Debug
                    ${DCMTK_DIR}/dcmdata/Release
                    ${DCMTK_DIR}/dcmdata/Debug
                    ${DCMTK_DIR}/lib
                    $ENV{H3D_EXTERNAL_ROOT}/lib
                    $ENV{H3D_ROOT}/../External/lib
                    ../../External/lib
                    ${module_file_path}/../../../External/lib
                    ${module_file_path}/../../dcmtk/lib 
		    /usr/local/dicom/lib )
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
		  /usr/local/dicom/include )
MARK_AS_ADVANCED(DCMTK_dcmimgle_INCLUDE_DIR)

FIND_LIBRARY( DCMTK_dcmimgle_LIBRARY dcmimgle${DCMTK_LIBRARY_POSTFIX}
              PATHS ${DCMTK_DIR}/dcmimgle/libsrc
                    ${DCMTK_DIR}/dcmimgle/libsrc/Release
                    ${DCMTK_DIR}/dcmimgle/libsrc/Debug
                    ${DCMTK_DIR}/dcmimgle/Release
                    ${DCMTK_DIR}/dcmimgle/Debug
                    ${DCMTK_DIR}/lib
                    $ENV{H3D_EXTERNAL_ROOT}/lib
                    $ENV{H3D_ROOT}/../External/lib
                    ../../External/lib
                    ${module_file_path}/../../../External/lib
                    ${module_file_path}/../../dcmtk/lib
		     /usr/local/dicom/lib )
MARK_AS_ADVANCED(DCMTK_dcmimgle_LIBRARY)

FIND_LIBRARY( DCMTK_dcmimage_LIBRARY dcmimage${DCMTK_LIBRARY_POSTFIX}
              PATHS ${DCMTK_DIR}/dcmimage/libsrc
                    ${DCMTK_DIR}/dcmimage/libsrc/Release
                    ${DCMTK_DIR}/dcmimage/libsrc/Debug
                    ${DCMTK_DIR}/dcmimage/Release
                    ${DCMTK_DIR}/dcmimage/Debug
                    ${DCMTK_DIR}/lib
                    $ENV{H3D_EXTERNAL_ROOT}/lib
                    $ENV{H3D_ROOT}/../External/lib
                    ../../External/lib
                    ${module_file_path}/../../../External/lib
                    ${module_file_path}/../../dcmtk/lib
 		    /usr/local/dicom/lib )
MARK_AS_ADVANCED(DCMTK_dcmimage_LIBRARY)

#FIND_LIBRARY(DCMTK_dcmnet_LIBRARY dcmnet${DCMTK_LIBRARY_POSTFIX}
#${DCMTK_DIR}/dcmnet/libsrc/Release
#${DCMTK_DIR}/dcmnet/libsrc/Debug
#${DCMTK_DIR}/dcmnet/libsrc/
#$ENV{H3D_EXTERNAL_ROOT}/lib
#$ENV{H3D_ROOT}/../External/lib
#../../External/lib
#${module_file_path}/../../../External/lib
#)


IF( DCMTK_config_INCLUDE_DIR )
IF( DCMTK_ofstd_INCLUDE_DIR )
IF( DCMTK_ofstd_LIBRARY )
IF( DCMTK_dcmdata_INCLUDE_DIR )
IF( DCMTK_dcmdata_LIBRARY )
IF( DCMTK_dcmimgle_INCLUDE_DIR )
IF( DCMTK_dcmimgle_LIBRARY )
IF( DCMTK_dcmimage_LIBRARY )
IF( DCMTK_dcmjpeg_INCLUDE_DIR )
IF( DCMTK_dcmjpeg_LIBRARY )

  SET( DCMTK_FOUND "YES" )
  SET( DCMTK_INCLUDE_DIR
    ${DCMTK_config_INCLUDE_DIR}
    ${DCMTK_ofstd_INCLUDE_DIR}
    ${DCMTK_dcmdata_INCLUDE_DIR}
    ${DCMTK_dcmimgle_INCLUDE_DIR}
    ${DCMTK_dcmjpeg_INCLUDE_DIR}
  )

  SET( DCMTK_LIBRARIES
    ${DCMTK_dcmimgle_LIBRARY}
    ${DCMTK_dcmdata_LIBRARY}
    ${DCMTK_ofstd_LIBRARY}
    ${DCMTK_config_LIBRARY}
    ${DCMTK_dcmimage_LIBRARY}
    ${DCMTK_dcmjpeg_LIBRARY}
    ${DCMTK_ijg8_LIBRARY}
    ${DCMTK_ijg12_LIBRARY}
    ${DCMTK_ijg16_LIBRARY}
  )

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

  IF( WIN32 )
    SET( DCMTK_LIBRARIES ${DCMTK_LIBRARIES} netapi32 ws2_32 )
  ENDIF( WIN32 )

ENDIF( DCMTK_dcmjpeg_LIBRARY )
ENDIF( DCMTK_dcmjpeg_INCLUDE_DIR )
ENDIF( DCMTK_dcmimage_LIBRARY )
ENDIF( DCMTK_dcmimgle_LIBRARY )
ENDIF( DCMTK_dcmimgle_INCLUDE_DIR )
ENDIF( DCMTK_dcmdata_LIBRARY )
ENDIF( DCMTK_dcmdata_INCLUDE_DIR )
ENDIF( DCMTK_ofstd_LIBRARY )
ENDIF( DCMTK_ofstd_INCLUDE_DIR )
ENDIF( DCMTK_config_INCLUDE_DIR )

# Report the results.
IF(NOT DCMTK_FOUND)
  SET(DCMTK_DIR_MESSAGE
    "DCMTK was not found. Make sure DCMTK_LIBRARY and DCMTK_INCLUDE_DIR are set to where you have your dcmtk header and lib files. If you do not have the library you will not be able to use the DicomImageLoader node.")
  IF(DCMTK_FIND_REQUIRED)
      MESSAGE(FATAL_ERROR "${DCMTK_DIR_MESSAGE}")
  ELSEIF(NOT DCMTK_FIND_QUIETLY)
    MESSAGE(STATUS "${DCMTK_DIR_MESSAGE}")
  ENDIF(DCMTK_FIND_REQUIRED)
ENDIF(NOT DCMTK_FOUND)

