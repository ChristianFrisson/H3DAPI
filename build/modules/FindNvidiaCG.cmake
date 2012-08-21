# - Find the NVIDIA CG Toolkit
#
#  NVIDIACG_INCLUDE_DIR -  where to find cg.h, etc.
#  NVIDIACG_LIBRARIES    - List of libraries when using NVIDIA CG Toolkit.
#  NVIDIACG_FOUND        - True if the NVIDIA CG Toolkit is found.

GET_FILENAME_COMPONENT(module_file_path ${CMAKE_CURRENT_LIST_FILE} PATH )

IF( CMAKE_CL_64 )
  SET( LIB "lib64" )
ELSE( CMAKE_CL_64 )
  SET( LIB "lib32" )
ENDIF( CMAKE_CL_64 )

# Look for the header file.
FIND_PATH( NVIDIACG_INCLUDE_DIR NAMES cg.h cgGL.h
           PATHS /usr/local/include
                 $ENV{H3D_EXTERNAL_ROOT}/include  
                 $ENV{H3D_EXTERNAL_ROOT}/include/Cg
                 $ENV{H3D_EXTERNAL_ROOT}/include/cg
                 $ENV{H3D_ROOT}/../External/include  
                 $ENV{H3D_ROOT}/../External/include/Cg
                 $ENV{H3D_ROOT}/../External/include/cg
                 ../../External/include
                 ../../External/include/Cg
                 ../../External/include/cg
                 ${module_file_path}/../../../External/include
                 ${module_file_path}/../../../External/include/Cg
                 ${module_file_path}/../../../External/include/cg
           DOC "Path in which the file cg.h and cgGL.h are located." )
MARK_AS_ADVANCED(NVIDIACG_INCLUDE_DIR)

# Look for the library cg.
# Does this work on UNIX systems? (LINUX)
FIND_LIBRARY( NVIDIACG_LIBRARY NAMES cg Cg
              PATHS $ENV{H3D_EXTERNAL_ROOT}/${LIB}
                    $ENV{H3D_ROOT}/../External/${LIB}
                    ../../External/${LIB}
                    ${module_file_path}/../../../External/${LIB}
              DOC "Path to cg library." )
MARK_AS_ADVANCED(NVIDIACG_LIBRARY)

# Look for the library cg.
# Does this work on UNIX systems? (LINUX)
FIND_LIBRARY( NVIDIACG_CGGL_LIBRARY NAMES cgGL CgGL
              PATHS $ENV{H3D_EXTERNAL_ROOT}/${LIB}
                    $ENV{H3D_ROOT}/../External/${LIB}
                    ../../External/${LIB}
                    ${module_file_path}/../../../External/${LIB}
              DOC "Path to cgGL library." )
MARK_AS_ADVANCED(NVIDIACG_CGGL_LIBRARY)

# Copy the results to the output variables.
IF(NVIDIACG_INCLUDE_DIR AND NVIDIACG_LIBRARY AND NVIDIACG_CGGL_LIBRARY)
  SET(NVIDIACG_FOUND 1)
  SET(NVIDIACG_LIBRARIES ${NVIDIACG_LIBRARY} ${NVIDIACG_CGGL_LIBRARY})
  SET(NVIDIACG_INCLUDE_DIR ${NVIDIACG_INCLUDE_DIR})
ELSE(NVIDIACG_INCLUDE_DIR AND NVIDIACG_LIBRARY AND NVIDIACG_CGGL_LIBRARY)
  SET(NVIDIACG_FOUND 0)
  SET(NVIDIACG_LIBRARIES)
  SET(NVIDIACG_INCLUDE_DIR)
ENDIF(NVIDIACG_INCLUDE_DIR AND NVIDIACG_LIBRARY AND NVIDIACG_CGGL_LIBRARY)

# Report the results.
IF(NOT NVIDIACG_FOUND)
  SET(NVIDIACG_DIR_MESSAGE
    "NVIDIAs CG Toolkit was not found. Make sure NVIDIACG_LIBRARY and NVIDIACG_INCLUDE_DIR are set.")
  IF(NvidiaCG_FIND_REQUIRED)
    MESSAGE(FATAL_ERROR "${NVIDIACG_DIR_MESSAGE}")
  ELSEIF(NOT NvidiaCG_FIND_QUIETLY)
    MESSAGE(STATUS "${NVIDIACG_DIR_MESSAGE}")
  ENDIF(NvidiaCG_FIND_REQUIRED)
ENDIF(NOT NVIDIACG_FOUND)
