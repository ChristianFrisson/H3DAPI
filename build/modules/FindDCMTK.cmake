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

include( H3DExternalSearchPath )
GET_FILENAME_COMPONENT( module_file_path ${CMAKE_CURRENT_LIST_FILE} PATH )
get_external_search_paths_h3d( module_include_search_paths module_lib_search_paths ${module_file_path} "static" )

SET( MSVC_BEFORE_VS2010 OFF )
if( MSVC )
  if( ${MSVC_VERSION} LESS 1600 )
    SET( MSVC_BEFORE_VS2010 ON )
  endif()
endif()

IF( MSVC_BEFORE_VS2010 )
  SET( H3D_MSVC_VERSION 6 )
  SET( TEMP_MSVC_VERSION 1299 )
  WHILE( ${MSVC_VERSION} GREATER ${TEMP_MSVC_VERSION} )
    MATH( EXPR H3D_MSVC_VERSION "${H3D_MSVC_VERSION} + 1" )
    MATH( EXPR TEMP_MSVC_VERSION "${TEMP_MSVC_VERSION} + 100" )
  ENDWHILE( ${MSVC_VERSION} GREATER ${TEMP_MSVC_VERSION} )
  SET( DCMTK_LIBRARY_POSTFIX "_vc${H3D_MSVC_VERSION}" )
ENDIF()

IF( NOT WIN32 )
SET( DCMTK_HAVE_CONFIG_H "NO" CACHE BOOL "On some systems the compile flag -DHAVE_CONFIG_H needs to be defined because of DCMTK. If DCMTK headers are causing problem turn this flag on." )
IF( DCMTK_HAVE_CONFIG_H )
  SET(CMAKE_CXX_FLAGS "-DHAVE_CONFIG_H")
ENDIF( DCMTK_HAVE_CONFIG_H )
ENDIF( NOT WIN32 )


FIND_PATH( DCMTK_config_INCLUDE_DIR NAMES dcmtk/config/osconfig.h
           PATHS ${DCMTK_DIR}/config/include 
                 ${DCMTK_DIR}/include 
                 ${module_include_search_paths}
                 ${module_file_path}/../../dcmtk/include
                 /usr/local/dicom/include
           DOC "Path in which the file dcmtk/config/osconfig.h is located." )
MARK_AS_ADVANCED(DCMTK_config_INCLUDE_DIR)

set( DCMTK_lib_names_internal ofstd oflog dcmjpeg dcmdata dcmimgle dcmimage )
if( DCMTK_lib_names )
  set( DCMTK_lib_names_internal ${DCMTK_lib_names} )
endif()

set( DCMTK_lib_ijg_names_internal ijg8 ijg12 ijg16 )
if( DCMTK_lib_ijg_names )
  set( DCMTK_lib_ijg_names_internal ${DCMTK_lib_ijg_names} )
endif()

foreach( dcmtk_lib_name ${DCMTK_lib_names_internal} )
  FIND_PATH( DCMTK_${dcmtk_lib_name}_INCLUDE_DIR NAMES dcmtk/${dcmtk_lib_name}
           PATHS ${DCMTK_DIR}/${dcmtk_lib_name}/include 
                 ${DCMTK_DIR}/include/${dcmtk_lib_name}
                 ${DCMTK_DIR}/include
                 ${module_include_search_paths}
                 ${module_file_path}/../../dcmtk/include
                 /usr/local/dicom/include
           DOC "Path to dcmtk/${dcmtk_lib_name} is located." )
  MARK_AS_ADVANCED(DCMTK_${dcmtk_lib_name}_INCLUDE_DIR)

  FIND_LIBRARY( DCMTK_${dcmtk_lib_name}_LIBRARY "${dcmtk_lib_name}${DCMTK_LIBRARY_POSTFIX}"
                PATHS ${DCMTK_DIR}/${dcmtk_lib_name}/libsrc
                      ${DCMTK_DIR}/${dcmtk_lib_name}/libsrc/Release
                      ${DCMTK_DIR}/${dcmtk_lib_name}/libsrc/Debug
                      ${DCMTK_DIR}/${dcmtk_lib_name}/Release
                      ${DCMTK_DIR}/${dcmtk_lib_name}/Debug
                      ${DCMTK_DIR}/lib
                      ${module_lib_search_paths}
                      ${module_file_path}/../../dcmtk/lib
                      /usr/local/dicom/lib
                DOC "Path to ${dcmtk_lib_name}${DCMTK_LIBRARY_POSTFIX} library." )
  MARK_AS_ADVANCED(DCMTK_${dcmtk_lib_name}_LIBRARY)
  
  IF( NOT MSVC_BEFORE_VS2010 )
    # Visual Studio versions later than 2008 needs debug versions to compile in debug
    FIND_LIBRARY( DCMTK_${dcmtk_lib_name}_DEBUG_LIBRARY "${dcmtk_lib_name}${DCMTK_LIBRARY_POSTFIX}_d"
                  PATHS ${DCMTK_DIR}/${dcmtk_lib_name}/libsrc
                        ${DCMTK_DIR}/${dcmtk_lib_name}/libsrc/Release
                        ${DCMTK_DIR}/${dcmtk_lib_name}/libsrc/Debug
                        ${DCMTK_DIR}/${dcmtk_lib_name}/Release
                        ${DCMTK_DIR}/${dcmtk_lib_name}/Debug
                        ${DCMTK_DIR}/lib
                        ${module_lib_search_paths}
                        ${module_file_path}/../../dcmtk/lib
                        /usr/local/dicom/lib
                  DOC "Path to ${dcmtk_lib_name}${DCMTK_LIBRARY_POSTFIX}_d library." )
    MARK_AS_ADVANCED(DCMTK_${dcmtk_lib_name}_DEBUG_LIBRARY)
  ENDIF()
endforeach( dcmtk_lib_name )

foreach( dcmtk_lib_ijg_name ${DCMTK_lib_ijg_names_internal} )
  FIND_LIBRARY( DCMTK_${dcmtk_lib_ijg_name}_LIBRARY ${dcmtk_lib_ijg_name}${DCMTK_LIBRARY_POSTFIX}
              PATHS ${DCMTK_DIR}/dcmjpeg/lib${dcmtk_lib_ijg_name}/libsrc
                    ${DCMTK_DIR}/dcmjpeg/lib${dcmtk_lib_ijg_name}/libsrc/Release
                    ${DCMTK_DIR}/dcmjpeg/lib${dcmtk_lib_ijg_name}/libsrc/Debug
                    ${DCMTK_DIR}/dcmjpeg/lib${dcmtk_lib_ijg_name}/Release
                    ${DCMTK_DIR}/dcmjpeg/lib${dcmtk_lib_ijg_name}/Debug
                    ${DCMTK_DIR}/lib
                    ${module_lib_search_paths}
                    ${module_file_path}/../../dcmtk/lib
                    /usr/local/dicom/lib
              DOC "Path to ${dcmtk_lib_ijg_name}${DCMTK_LIBRARY_POSTFIX} library." )
  MARK_AS_ADVANCED(DCMTK_${dcmtk_lib_ijg_name}_LIBRARY)
  
  IF( NOT MSVC_BEFORE_VS2010 )
    FIND_LIBRARY( DCMTK_${dcmtk_lib_ijg_name}_DEBUG_LIBRARY ${dcmtk_lib_ijg_name}${DCMTK_LIBRARY_POSTFIX}_d
                  PATHS ${DCMTK_DIR}/dcmjpeg/lib${dcmtk_lib_ijg_name}/libsrc
                        ${DCMTK_DIR}/dcmjpeg/lib${dcmtk_lib_ijg_name}/libsrc/Release
                        ${DCMTK_DIR}/dcmjpeg/lib${dcmtk_lib_ijg_name}/libsrc/Debug
                        ${DCMTK_DIR}/dcmjpeg/lib${dcmtk_lib_ijg_name}/Release
                        ${DCMTK_DIR}/dcmjpeg/lib${dcmtk_lib_ijg_name}/Debug
                        ${DCMTK_DIR}/lib
                        ${module_lib_search_paths}
                        ${module_file_path}/../../dcmtk/lib
                        /usr/local/dicom/lib
                  DOC "Path to ${dcmtk_lib_ijg_name}${DCMTK_LIBRARY_POSTFIX}_d library." )
    MARK_AS_ADVANCED(DCMTK_${dcmtk_lib_ijg_name}_DEBUG_LIBRARY)
  ENDIF()
endforeach( dcmtk_lib_ijg_name )

SET( HAVE_INCLUDE_DIRS ON )
SET( HAVE_RELEASE_LIBS ON )
SET( HAVE_DEBUG_LIBS ON )

# check that we have all include dirs, release libs and debug libs.
foreach( dcmtk_lib_name ${DCMTK_lib_names_internal} )
  if( NOT DCMTK_${dcmtk_lib_name}_INCLUDE_DIR )
    SET( HAVE_INCLUDE_DIRS OFF )
  endif()
  if( NOT DCMTK_${dcmtk_lib_name}_LIBRARY )
    SET( HAVE_RELEASE_LIBS OFF )
  endif()
  if( NOT DCMTK_${dcmtk_lib_name}_DEBUG_LIBRARY )
    SET( HAVE_DEBUG_LIBS OFF )
  endif()
endforeach( dcmtk_lib_name )

foreach( dcmtk_lib_ijg_name ${DCMTK_lib_ijg_names_internal} )
  if( NOT DCMTK_${dcmtk_lib_ijg_name}_LIBRARY )
    SET( HAVE_RELEASE_LIBS OFF )
  endif()
  if( NOT DCMTK_${dcmtk_lib_ijg_name}_DEBUG_LIBRARY )
    SET( HAVE_DEBUG_LIBS OFF )
  endif()
endforeach( dcmtk_lib_ijg_name )

SET( HAVE_TIFF_OR_NO_TIFF_NEEDED 1 )
IF( HAVE_INCLUDE_DIRS AND HAVE_RELEASE_LIBS )
  SET( DCMTK_Config_file_name "dummy" )
  IF( WIN32 )
    SET( DCMTK_Config_file_name "cfwin32" )
  ELSE( WIN32 )
    SET( DCMTK_Config_file_name "cfunix" )
  ENDIF( WIN32 )
  
  SET( DCMTK_Config_file ${DCMTK_config_INCLUDE_DIR}/dcmtk/config/${DCMTK_Config_file_name}.h )
  if( NOT EXISTS ${DCMTK_Config_file} )
    if( EXISTS ${DCMTK_config_INCLUDE_DIR}/dcmtk/config/osconfig.h )
      SET( DCMTK_Config_file ${DCMTK_config_INCLUDE_DIR}/dcmtk/config/osconfig.h )
    else()
      SET( DCMTK_Config_file "" )
    endif()
  endif()
  if( DCMTK_Config_file )
    SET( regex_to_find "#define[ ]+WITH_LIBTIFF" )
    FILE( STRINGS ${DCMTK_Config_file} list_of_defines REGEX ${regex_to_find} )
    LIST( LENGTH list_of_defines list_of_defines_length )
    IF( list_of_defines_length )
      # Dicom is compiled with tiff support. Find libtiff as well.
      FIND_PACKAGE( TIFF )
      IF(NOT TIFF_FOUND)
        SET( HAVE_TIFF_OR_NO_TIFF_NEEDED 0 )
      ENDIF(NOT TIFF_FOUND)
    ENDIF( list_of_defines_length )
  endif()
ENDIF()

IF( HAVE_INCLUDE_DIRS AND HAVE_RELEASE_LIBS AND HAVE_TIFF_OR_NO_TIFF_NEEDED )
  SET( DCMTK_FOUND "YES" )
  SET( DCMTK_INCLUDE_DIR "" )
  SET( DCMTK_LIBRARIES "" )
  
  foreach( dcmtk_lib_name ${DCMTK_lib_names_internal} )
    SET( DCMTK_INCLUDE_DIR ${DCMTK_INCLUDE_DIR} ${DCMTK_${dcmtk_lib_name}_INCLUDE_DIR} )
  endforeach( dcmtk_lib_name )

  IF( NOT MSVC_BEFORE_VS2010 )
    # MSVC after version 10(2010) needs debug libraries since it cannot compile with
    # the release versions
    foreach( dcmtk_lib_name ${DCMTK_lib_names_internal} )
      SET( DCMTK_LIBRARIES ${DCMTK_LIBRARIES} optimized ${DCMTK_${dcmtk_lib_name}_LIBRARY} debug ${DCMTK_${dcmtk_lib_name}_DEBUG_LIBRARY} )
    endforeach( dcmtk_lib_name )
    foreach( DCMTK_lib_ijg_name ${DCMTK_lib_ijg_names_internal} )
      SET( DCMTK_LIBRARIES ${DCMTK_LIBRARIES} optimized ${DCMTK_${DCMTK_lib_ijg_name}_LIBRARY} debug ${DCMTK_${DCMTK_lib_ijg_name}_DEBUG_LIBRARY} )
    endforeach( DCMTK_lib_ijg_name )
  ELSE()
    
    IF( NOT MSVC_BEFORE_VS2010 )
      MESSAGE( STATUS "DCMTK debug libraries not found. Debug compilation might not work with DCMTK." )
    ENDIF()

    foreach( dcmtk_lib_name ${DCMTK_lib_names_internal} )
      SET( DCMTK_LIBRARIES ${DCMTK_LIBRARIES} ${DCMTK_${dcmtk_lib_name}_LIBRARY} )
    endforeach( dcmtk_lib_name )
    foreach( DCMTK_lib_ijg_name ${DCMTK_lib_ijg_names_internal} )
      SET( DCMTK_LIBRARIES ${DCMTK_LIBRARIES} ${DCMTK_${DCMTK_lib_ijg_name}_LIBRARY} )
    endforeach( DCMTK_lib_ijg_name )
  ENDIF()

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

