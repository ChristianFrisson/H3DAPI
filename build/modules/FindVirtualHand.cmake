# - Find VirtualHand SDK
# Find the native VirtualHand headers and libraries.
#
#  VirtualHand_INCLUDE_DIR -  where to find headers, etc.
#  VirtualHand_LIBRARIES    - List of libraries when using VirtualHand.
#  VirtualHand_FOUND        - True if VirtualHand found.

GET_FILENAME_COMPONENT(vti_registry_file_path $ENV{VTI_REGISTRY_FILE} PATH )

# Look for the header file.
FIND_PATH( VirtualHand_INCLUDE_DIR NAMES vhandtk/vht.h
           PATHS ${vti_registry_file_path}/../../Development/include
                  C:/Program/Immersion Corporation/VirtualHand/Development/include
                  "C:/Program Files/Immersion Corporation/VirtualHand/Development/include" )
MARK_AS_ADVANCED(VirtualHand_INCLUDE_DIR)

# Look for the library.

FIND_LIBRARY( VirtualHandDevice_LIBRARY NAMES VirtualHandDevice
              PATHS ${vti_registry_file_path}/../../Development/lib/winnt_386
                  C:/Program/Immersion Corporation/VirtualHand/Development/lib/winnt_386
                  "C:/Program Files/Immersion Corporation/VirtualHand/Development/lib/winnt_386" )

FIND_LIBRARY( VirtualHandCore_LIBRARY NAMES VirtualHandCore
              PATHS ${vti_registry_file_path}/../../Development/lib/winnt_386
                  C:/Program/Immersion Corporation/VirtualHand/Development/lib/winnt_386
                  "C:/Program Files/Immersion Corporation/VirtualHand/Development/lib/winnt_386" )

MARK_AS_ADVANCED(VirtualHandDevice_LIBRARY)
MARK_AS_ADVANCED(VirtualHandCore_LIBRARY)

# Copy the results to the output variables.
IF(VirtualHand_INCLUDE_DIR AND VirtualHandDevice_LIBRARY AND VirtualHandCore_LIBRARY)

  SET(VirtualHand_FOUND 1)
  
  SET(VirtualHand_INCLUDE_DIR ${VirtualHand_INCLUDE_DIR} )
  SET(VirtualHand_LIBRARIES ${VirtualHandDevice_LIBRARY} ${VirtualHandCore_LIBRARY})
ELSE(VirtualHand_INCLUDE_DIR AND VirtualHandDevice_LIBRARY AND VirtualHandCore_LIBRARY)
  SET(VirtualHand_FOUND 0)
  SET(VirtualHand_LIBRARIES)
  SET(VirtualHand_INCLUDE_DIR)
ENDIF(VirtualHand_INCLUDE_DIR AND VirtualHandDevice_LIBRARY AND VirtualHandCore_LIBRARY)

# Report the results.
IF(NOT VirtualHand_FOUND)
  SET(VirtualHand_DIR_MESSAGE
    "VirtualHand was not found. Make sure VirtualHandDevice_LIBRARY, VirtualHandCore_LIBRARY and VirtualHand_INCLUDE_DIR are set.")
  IF(VirtualHand_FIND_REQUIRED)
    MESSAGE(FATAL_ERROR "${VirtualHand_DIR_MESSAGE}")
  ELSEIF(NOT VirtualHand_FIND_QUIETLY)
    MESSAGE(STATUS "${VirtualHand_DIR_MESSAGE}")
  ENDIF(VirtualHand_FIND_REQUIRED)
ENDIF(NOT VirtualHand_FOUND)
