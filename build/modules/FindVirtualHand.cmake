# - Find VirtualHand SDK
# Find the native VirtualHand headers and libraries.
#
#  VirtualHand_INCLUDE_DIR -  where to find headers, etc.
#  VirtualHand_LIBRARIES    - List of libraries when using VirtualHand.
#  VirtualHand_FOUND        - True if VirtualHand found.

SET( vti_registry_file_path "" )
IF( NOT "$ENV{VTI_REGISTRY_FILE}" STREQUAL  "" )
  GET_FILENAME_COMPONENT(vti_registry_file_path "$ENV{VTI_REGISTRY_FILE}" PATH )
ENDIF( NOT "$ENV{VTI_REGISTRY_FILE}" STREQUAL  "" )

# Look for the header file.
FIND_PATH( VirtualHand_INCLUDE_DIR NAMES vhandtk/vht.h
           PATHS ${vti_registry_file_path}/../../Development/include
                  C:/Program/Immersion Corporation/VirtualHand/Development/include
                  "C:/Program Files/Immersion Corporation/VirtualHand/Development/include"
           DOC "Path in which the file vhandtk/vht.h is located." )
MARK_AS_ADVANCED(VirtualHand_INCLUDE_DIR)

# Look for the library.

FIND_LIBRARY( VirtualHand_Device_LIBRARY NAMES VirtualHandDevice
              PATHS ${vti_registry_file_path}/../../Development/lib/winnt_386
                    C:/Program/Immersion Corporation/VirtualHand/Development/lib/winnt_386
                    "C:/Program Files/Immersion Corporation/VirtualHand/Development/lib/winnt_386"
              DOC "Path to VirtualHandDevice library." )

FIND_LIBRARY( VirtualHand_Core_LIBRARY NAMES VirtualHandCore
              PATHS ${vti_registry_file_path}/../../Development/lib/winnt_386
                    C:/Program/Immersion Corporation/VirtualHand/Development/lib/winnt_386
                    "C:/Program Files/Immersion Corporation/VirtualHand/Development/lib/winnt_386"
              DOC "Path to VirtualHandCore library." )

MARK_AS_ADVANCED(VirtualHand_Device_LIBRARY)
MARK_AS_ADVANCED(VirtualHand_Core_LIBRARY)

# Copy the results to the output variables.
IF(VirtualHand_INCLUDE_DIR AND VirtualHand_Device_LIBRARY AND VirtualHand_Core_LIBRARY)
  SET(VirtualHand_FOUND 1)
  SET(VirtualHand_INCLUDE_DIR ${VirtualHand_INCLUDE_DIR} )
  SET(VirtualHand_LIBRARIES ${VirtualHand_Device_LIBRARY} ${VirtualHand_Core_LIBRARY})
ELSE(VirtualHand_INCLUDE_DIR AND VirtualHand_Device_LIBRARY AND VirtualHand_Core_LIBRARY)
  SET(VirtualHand_FOUND 0)
  SET(VirtualHand_LIBRARIES)
  SET(VirtualHand_INCLUDE_DIR)
ENDIF(VirtualHand_INCLUDE_DIR AND VirtualHand_Device_LIBRARY AND VirtualHand_Core_LIBRARY)

# Report the results.
IF(NOT VirtualHand_FOUND)
  SET(VirtualHand_DIR_MESSAGE
    "VirtualHand was not found. Make sure VirtualHand_Device_LIBRARY, VirtualHand_Core_LIBRARY and VirtualHand_INCLUDE_DIR are set.")
  IF(VirtualHand_FIND_REQUIRED)
    MESSAGE(FATAL_ERROR "${VirtualHand_DIR_MESSAGE}")
  ELSEIF(NOT VirtualHand_FIND_QUIETLY)
    MESSAGE(STATUS "${VirtualHand_DIR_MESSAGE}")
  ENDIF(VirtualHand_FIND_REQUIRED)
ENDIF(NOT VirtualHand_FOUND)
