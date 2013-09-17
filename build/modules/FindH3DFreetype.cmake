# - Find FREETYPE
# Find the native FREETYPE headers and libraries.
#
#  FREETYPE_FOUND        - True if FREETYPE found.
#  FREETYPE_INCLUDE_DIRS -  where to find FREETYPE.h, etc.
#  FREETYPE_LIBRARIES    - List of libraries when using FREETYPE.

FIND_PROGRAM(FREETYPE_CONFIG_EXECUTABLE freetype-config
      ONLY_CMAKE_FIND_ROOT_PATH
      DOC "Path to freetype_config executable. Used to find freetype, not used on a standard Windows installation of H3DAPI.")
MARK_AS_ADVANCED( FREETYPE_CONFIG_EXECUTABLE )

IF(H3DFreetype_FIND_REQUIRED)
  IF( WIN32 )
    FIND_PACKAGE(Freetype QUIET REQUIRED)
  ELSE( WIN32 )
    FIND_PACKAGE(Freetype REQUIRED)
  ENDIF( WIN32 )
ELSE(H3DFreetype_FIND_REQUIRED)
  IF( WIN32 )
    FIND_PACKAGE(Freetype QUIET)
  ELSE( WIN32 )
    FIND_PACKAGE(Freetype)
  ENDIF( WIN32 )
ENDIF(H3DFreetype_FIND_REQUIRED)

IF( NOT FREETYPE_FOUND )
  IF( CMAKE_CL_64 )
    SET( LIB "lib64" )
  ELSE( CMAKE_CL_64 )
    SET( LIB "lib32" )
  ENDIF( CMAKE_CL_64 )

  IF(FREETYPE_CONFIG_EXECUTABLE)

    # run the freetype-config program to get cflags
    EXECUTE_PROCESS(
          COMMAND sh "${FREETYPE_CONFIG_EXECUTABLE}" --cflags
          OUTPUT_VARIABLE FREETYPE_CFLAGS
          RESULT_VARIABLE RET
          ERROR_QUIET
          )

    IF(RET EQUAL 0)
      IF( ${CMAKE_MAJOR_VERSION} EQUAL 2 AND ${CMAKE_MINOR_VERSION} EQUAL 6 )
        STRING(STRIP "${FREETYPE_CFLAGS}" FREETYPE_CFLAGS)
      ENDIF( ${CMAKE_MAJOR_VERSION} EQUAL 2 AND ${CMAKE_MINOR_VERSION} EQUAL 6 )
      SEPARATE_ARGUMENTS(FREETYPE_CFLAGS)

      # parse definitions from cxxflags; drop -D* from CFLAGS
      STRING(REGEX REPLACE "-D[^;]+;" ""
             FREETYPE_CFLAGS "${FREETYPE_CFLAGS}")

      # parse include dirs from cxxflags; drop -I prefix
      STRING(REGEX MATCHALL "-I[^;]+"
             FREETYPE_INCLUDE_DIR_ft2build "${FREETYPE_CFLAGS}")
      STRING(REGEX REPLACE "-I[^;]+;" ""
             FREETYPE_CFLAGS "${FREETYPE_CFLAGS}")
      STRING(REPLACE "-I" ""
             FREETYPE_INCLUDE_DIR_ft2build "${FREETYPE_INCLUDE_DIR_ft2build}")  
      STRING(REPLACE "\n" ""
             FREETYPE_INCLUDE_DIR_ft2build "${FREETYPE_INCLUDE_DIR_ft2build}")  
      SET( FREETYPE_INCLUDE_DIR_ft2build "${FREETYPE_INCLUDE_DIR_ft2build}" )  

    ENDIF(RET EQUAL 0)

  ENDIF(FREETYPE_CONFIG_EXECUTABLE)

  GET_FILENAME_COMPONENT(module_file_path ${CMAKE_CURRENT_LIST_FILE} PATH )

  IF( NOT FREETYPE_INCLUDE_DIR_ft2build )
    # Look for the header file.
    FIND_PATH(FREETYPE_INCLUDE_DIR_ft2build NAMES freetype/freetype.h
                                PATHS $ENV{H3D_EXTERNAL_ROOT}/include  
                                      $ENV{H3D_EXTERNAL_ROOT}/include/freetype/include
                                      $ENV{H3D_ROOT}/../External/include  
                                      $ENV{H3D_ROOT}/../External/include/freetype/include
                                      ../../External/include
                                      ../../External/include/freetype/include
                                      ${module_file_path}/../../../External/include
                                      ${module_file_path}/../../../External/include/freetype/include
                                DOC "Path in which the file freetype/freetype.h is located." )
    MARK_AS_ADVANCED(FREETYPE_INCLUDE_DIR_ft2build)
  ENDIF( NOT FREETYPE_INCLUDE_DIR_ft2build)

  # Look for the library.
  FIND_LIBRARY(FREETYPE_LIBRARY NAMES freetype freetype2311 freetype2312MT freetype2312 freetype235
                                PATHS $ENV{H3D_EXTERNAL_ROOT}/${LIB}/static
                                      $ENV{H3D_ROOT}/../External/${LIB}/static
                                      ../../External/${LIB}/static
                                      ${module_file_path}/../../../External/${LIB}/static
                                DOC "Path to freetype library." )
  MARK_AS_ADVANCED(FREETYPE_LIBRARY)

  # Copy the results to the output variables.
  IF(FREETYPE_INCLUDE_DIR_ft2build AND FREETYPE_LIBRARY)
    SET(FREETYPE_FOUND 1)
    SET(FREETYPE_LIBRARIES ${FREETYPE_LIBRARY})
    SET(FREETYPE_INCLUDE_DIRS ${FREETYPE_INCLUDE_DIR_ft2build})
  ELSE(FREETYPE_INCLUDE_DIR_ft2build AND FREETYPE_LIBRARY)
    SET(FREETYPE_FOUND 0)
    SET(FREETYPE_LIBRARIES)
    SET(FREETYPE_INCLUDE_DIRS)
  ENDIF(FREETYPE_INCLUDE_DIR_ft2build AND FREETYPE_LIBRARY)
ENDIF( NOT FREETYPE_FOUND )

# Report the results.
IF(NOT FREETYPE_FOUND)
  SET(FREETYPE_DIR_MESSAGE
    "FREETYPE was not found. Make sure FREETYPE_-named variables are set to the include directories and library files required.")
  IF(FreeType_FIND_REQUIRED)
    MESSAGE(FATAL_ERROR "${FREETYPE_DIR_MESSAGE}")
  ELSEIF(NOT FreeType_FIND_QUIETLY)
    MESSAGE(STATUS "${FREETYPE_DIR_MESSAGE}")
  ENDIF(FreeType_FIND_REQUIRED)
ENDIF(NOT FREETYPE_FOUND)
