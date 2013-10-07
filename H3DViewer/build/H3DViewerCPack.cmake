IF( NOT TARGET H3DViewer )
  MESSAGE( FATAL_ERROR "Include file H3DViewerCPack.cmake require the target H3DViewer to exist. Please add H3DAPI/H3DViewer/build/CMakeLists.txt as subdirectory first." )
ENDIF( NOT TARGET H3DViewer )

# To allow other projects that use H3DVIEWER as a subproject to add extra include directories
# when packaging.
IF( GENERATE_H3DVIEWER_CPACK_PROJECT )
  IF( WIN32 )
    # Add a cache variable which indicates where the Externals directory used for packaging
    # H3DViewer is located. If not set then FIND modules will be used instead.
    IF( NOT DEFINED H3DViewer_CPACK_EXTERNAL_ROOT )
      SET( H3DViewer_CPACK_EXTERNAL_ROOT_DEFAULT "" )
      IF( H3D_USE_DEPENDENCIES_ONLY )
        foreach( EXTERNAL_INCLUDE_DIR_TMP ${EXTERNAL_INCLUDE_DIR} )
          IF( EXISTS ${EXTERNAL_INCLUDE_DIR_TMP}/../include/pthread )
            SET( H3DViewer_CPACK_EXTERNAL_ROOT_DEFAULT "${EXTERNAL_INCLUDE_DIR_TMP}/.." )
          ENDIF( EXISTS ${EXTERNAL_INCLUDE_DIR_TMP}/../include/pthread )
        endforeach( EXTERNAL_INCLUDE_DIR_TMP ${EXTERNAL_INCLUDE_DIR} )
      ELSE( H3D_USE_DEPENDENCIES_ONLY )
        SET( H3DViewer_CPACK_EXTERNAL_ROOT_DEFAULT "$ENV{H3D_EXTERNAL_ROOT}" )
      ENDIF( H3D_USE_DEPENDENCIES_ONLY )
      SET( H3DViewer_CPACK_EXTERNAL_ROOT "${H3DViewer_CPACK_EXTERNAL_ROOT_DEFAULT}" CACHE PATH "Set to the External directory used with H3DViewer, needed to pack properly. If not set FIND_modules will be used instead." )
      MARK_AS_ADVANCED(H3DViewer_CPACK_EXTERNAL_ROOT)
    ENDIF( NOT DEFINED H3DViewer_CPACK_EXTERNAL_ROOT )
  ENDIF( WIN32 )
  
  # Set information properties about the project to install.
  SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "H3DViewer. An X3D viewer with extensions for haptics." )
  SET(CPACK_PACKAGE_VENDOR "SenseGraphics AB")
  SET(CPACK_PACKAGE_CONTACT "support@sensegraphics.com" )
  SET(CPACK_RESOURCE_FILE_LICENSE "${H3DViewer_SOURCE_DIR}/../../LICENSE")
  SET(CPACK_INSTALL_CMAKE_PROJECTS "${CMAKE_CURRENT_BINARY_DIR};H3DViewer;ALL;/")
  
  # Installation directory for package.
  SET(CPACK_PACKAGE_INSTALL_DIRECTORY "SenseGraphics" )
  
  SET(CPACK_PACKAGE_VERSION_MAJOR ${H3DViewer_MAJOR_VERSION})
  SET(CPACK_PACKAGE_VERSION_MINOR ${H3DViewer_MINOR_VERSION})
  SET(CPACK_PACKAGE_VERSION_PATCH ${H3DViewer_BUILD_VERSION})
  SET(CPACK_PACKAGE_INSTALL_REGISTRY_KEY "H3DViewer ${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}" )
  SET(CPACK_NSIS_PACKAGE_NAME "H3DViewer ${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}" )
  SET(CPACK_NSIS_UNINSTALL_NAME "H3DViewer-${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}" )
  
  IF( APPLE )
    IF( NOT DEFINED H3DVIEWER_CPACK_INCLUDE_LIBRARIES )
      SET( H3DVIEWER_CPACK_INCLUDE_LIBRARIES "NO" CACHE BOOL "Decides if all dependent shared libraries should be included in the bundle or not." )
      MARK_AS_ADVANCED(H3DVIEWER_CPACK_INCLUDE_LIBRARIES)
    ENDIF( NOT DEFINED H3DVIEWER_CPACK_INCLUDE_LIBRARIES )

    SET( CPACK_BUNDLE_NAME "H3DViewer" ) #- provides the bundle name (displayed in the finder underneath the bundle icon). 
    SET( CPACK_BUNDLE_ICON "${H3DViewer_SOURCE_DIR}/H3DViewer.icns" ) # - provides the bundle icon (displayed in the /Applications folder, on the dock, etc). 
    SET( CPACK_BUNDLE_PLIST "${H3DViewer_SOURCE_DIR}/info.plist" ) # - path to a file that will become the bundle plist. 
    SET( CPACK_BUNDLE_STARTUP_COMMAND "${H3DViewer_SOURCE_DIR}/start.sh" ) #- path to a file that will be executed when the user opens the bundle. Could be a shell-script or a binary.

    IF( H3DVIEWER_CPACK_INCLUDE_LIBRARIES )
      #Include all shared libraries in bundle
      INCLUDE("${H3DViewer_SOURCE_DIR}/OSXCPackLibraries.txt" )

      INSTALL( FILES ${OSX_PLUGIN_LIBRARIES}
               DESTINATION Plugins )

      INSTALL( CODE "EXECUTE_PROCESS( COMMAND \"python\" ${H3DViewer_SOURCE_DIR}/osx_bundle.py   
                                      WORKING_DIRECTORY \${CMAKE_INSTALL_PREFIX})" )
    ENDIF( H3DVIEWER_CPACK_INCLUDE_LIBRARIES )
  ENDIF( APPLE )
  
  IF( WIN32 )
    SET(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL "ON" )
    SET(CPACK_MONOLITHIC_INSTALL "TRUE" )
    SET( CPACK_PACKAGE_START_MENU_NAME "H3DViewer ${H3DViewer_MAJOR_VERSION}.${H3DViewer_MINOR_VERSION}" )
                           
    SET( EXTERNAL_BIN_PATH "bin32" )
    SET( EXTERNAL_BIN_REPLACE_PATH "bin64" )
    SET( CPACK_PACKAGE_NAME "H3DViewer" )
    # CPACK_NSIS_INSTALL_ROOT must be set properly because cmake does not set it correctly
    # for a 64 bit build.
    SET( CPACK_NSIS_INSTALL_ROOT "$PROGRAMFILES32" )
    SET( CPACK_NSIS_DISPLAY_NAME_POSTFIX "(x86)" )
    SET( CPACK_H3D_64_BIT "FALSE" )
    SET( CPACK_NSIS_EXECUTABLES_DIRECTORY bin32 )
    IF( CMAKE_SIZEOF_VOID_P EQUAL 8 ) # check if the system is 64 bit
      SET( EXTERNAL_BIN_PATH "bin64" )
      SET( CPACK_NSIS_EXECUTABLES_DIRECTORY bin64 )
      SET( EXTERNAL_BIN_REPLACE_PATH "bin32" )
      SET( CPACK_NSIS_INSTALL_ROOT "$PROGRAMFILES64" )
      SET( CPACK_H3D_64_BIT "TRUE" )
      SET( CPACK_NSIS_DISPLAY_NAME_POSTFIX "" )
    ENDIF( CMAKE_SIZEOF_VOID_P EQUAL 8 )
    SET(CPACK_NSIS_DISPLAY_NAME "H3DViewer${CPACK_NSIS_DISPLAY_NAME_POSTFIX} ${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}" )

    set( FEATURES_TO_INSTALL "bin" "H3DViewer/bin32" )
    SET( CPACK_NSIS_EXTRA_INSTALL_COMMANDS "\\n" )
    SET( CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS "\\n" )
    SET( TMP_CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} )
    SET( CMAKE_MODULE_PATH "" )
    SET( INCLUDES_TO_SEARCH "" )
    IF( H3DUTIL_INCLUDE_DIR )
      SET( INCLUDES_TO_SEARCH ${H3DUTIL_INCLUDE_DIR} )
    ENDIF( H3DUTIL_INCLUDE_DIR )
    IF( HAPI_INCLUDE_DIR )
      SET( INCLUDES_TO_SEARCH ${INCLUDES_TO_SEARCH} ${HAPI_INCLUDE_DIR} )
    ENDIF( HAPI_INCLUDE_DIR )
    IF( H3DAPI_INCLUDE_DIR )
      SET( INCLUDES_TO_SEARCH ${INCLUDES_TO_SEARCH} ${H3DAPI_INCLUDE_DIR} )
    ENDIF( H3DAPI_INCLUDE_DIR )
    foreach( dir_name "H3DAPI" "HAPI" "H3DUtil" )
      foreach( include_dir ${INCLUDES_TO_SEARCH} )
        STRING( FIND ${include_dir} "/${dir_name}/build" build_pos )
        IF( NOT build_pos EQUAL -1 )
          STRING( LENGTH "/${dir_name}/build" search_length )
          MATH( EXPR str_length "${build_pos} + ${search_length}" )
          STRING( SUBSTRING ${include_dir} 0 ${str_length} include_dir_part )
          SET( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${include_dir_part}/modules )
        ENDIF( NOT build_pos EQUAL -1 )
      endforeach( include_dir )
    endforeach( dir_name )
    
    INCLUDE( InstallH3DAPIAndExternals )
    SET( CMAKE_MODULE_PATH ${TMP_CMAKE_MODULE_PATH} )

    # Modify path since in the NSIS template.
    SET( CPACK_NSIS_MODIFY_PATH "ON" )
  
    IF( EXISTS ${H3DViewer_CPACK_EXTERNAL_ROOT} )
      SET( EXTERNAL_BINARIES ${EXTERNAL_BINARIES}
                             ${H3DViewer_CPACK_EXTERNAL_ROOT}/${EXTERNAL_BIN_PATH}/wxbase295u_vc_custom.dll
                             ${H3DViewer_CPACK_EXTERNAL_ROOT}/${EXTERNAL_BIN_PATH}/wxbase295u_xml_vc_custom.dll
                             ${H3DViewer_CPACK_EXTERNAL_ROOT}/${EXTERNAL_BIN_PATH}/wxmsw295u_adv_vc_custom.dll
                             ${H3DViewer_CPACK_EXTERNAL_ROOT}/${EXTERNAL_BIN_PATH}/wxmsw295u_core_vc_custom.dll
                             ${H3DViewer_CPACK_EXTERNAL_ROOT}/${EXTERNAL_BIN_PATH}/wxmsw295u_gl_vc_custom.dll
                             ${H3DViewer_CPACK_EXTERNAL_ROOT}/${EXTERNAL_BIN_PATH}/wxmsw295u_html_vc_custom.dll
                             ${H3DViewer_CPACK_EXTERNAL_ROOT}/${EXTERNAL_BIN_PATH}/wxmsw295u_propgrid_vc_custom.dll
                             ${H3DViewer_CPACK_EXTERNAL_ROOT}/${EXTERNAL_BIN_PATH}/wxmsw295u_richtext_vc_custom.dll
                             ${H3DViewer_CPACK_EXTERNAL_ROOT}/${EXTERNAL_BIN_PATH}/ode_double.dll )
      INSTALL( FILES "${H3DViewer_CPACK_EXTERNAL_ROOT}/include/ACKNOWLEDGEMENTS"
               DESTINATION H3DViewer )
      INSTALL( DIRECTORY ${H3DViewer_CPACK_EXTERNAL_ROOT}/include/ExternalLicenses/
               DESTINATION H3DViewer/ExternalLicenses )
    ENDIF( EXISTS ${H3DViewer_CPACK_EXTERNAL_ROOT} )

    foreach( binary ${EXTERNAL_BINARIES} )
      IF( EXISTS ${binary} )
        INSTALL( FILES ${binary}
                 DESTINATION H3DViewer/${DEFAULT_BIN_INSTALL} )
      ENDIF( EXISTS ${binary} )
    endforeach( binary )
    
    SET( H3D_MSVC_VERSION 6 )
    SET( TEMP_MSVC_VERSION 1299 )
    WHILE( ${MSVC_VERSION} GREATER ${TEMP_MSVC_VERSION} )
      MATH( EXPR H3D_MSVC_VERSION "${H3D_MSVC_VERSION} + 1" )
      MATH( EXPR TEMP_MSVC_VERSION "${TEMP_MSVC_VERSION} + 100" )
    ENDWHILE( ${MSVC_VERSION} GREATER ${TEMP_MSVC_VERSION} )

    IF( H3D_USE_DEPENDENCIES_ONLY )      
      # these part are added separately so that these plug in can be automatically added to H3DViewer
      INSTALL( FILES ${H3DViewer_BINARY_DIR}/../../H3DPhysics/Release/H3DPhysics_vc${H3D_MSVC_VERSION}.dll
                     ${H3DViewer_BINARY_DIR}/../../MedX3D/Release/MedX3D_vc${H3D_MSVC_VERSION}.dll
                     ${H3DViewer_BINARY_DIR}/../../UI/Release/UI_vc${H3D_MSVC_VERSION}.dll
                     CONFIGURATIONS Release
                     DESTINATION H3DViewer/plugins )
    ELSE( H3D_USE_DEPENDENCIES_ONLY )
      # these part are added separately so that these plug in can be automatically added to H3DViewer
      INSTALL( FILES ${H3DViewer_SOURCE_DIR}/../../../${DEFAULT_BIN_INSTALL}/H3DPhysics_vc${H3D_MSVC_VERSION}.dll
                     ${H3DViewer_SOURCE_DIR}/../../../${DEFAULT_BIN_INSTALL}/MedX3D_vc${H3D_MSVC_VERSION}.dll
                     ${H3DViewer_SOURCE_DIR}/../../../${DEFAULT_BIN_INSTALL}/UI_vc${H3D_MSVC_VERSION}.dll
                     CONFIGURATIONS Release
                     DESTINATION H3DViewer/plugins )
    ENDIF( H3D_USE_DEPENDENCIES_ONLY )
    
    IF( EXISTS ${H3DViewer_SOURCE_DIR}/../../Util/H3DViewerPackageExtraFiles )
      INSTALL( FILES ${H3DViewer_SOURCE_DIR}/../../Util/H3DViewerPackageExtraFiles/ReadMe.txt
             DESTINATION H3DViewer )
    ENDIF( EXISTS ${H3DViewer_SOURCE_DIR}/../../Util/H3DViewerPackageExtraFiles )
  ENDIF( WIN32 )  
  
  INCLUDE(CPack)

ENDIF( GENERATE_H3DVIEWER_CPACK_PROJECT )