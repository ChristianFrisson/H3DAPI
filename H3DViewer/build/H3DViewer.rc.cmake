// Microsoft Visual C++ generated resource script.
//
//#include "resource.h"

#define APSTUDIO_READONLY_SYMBOLS
/////////////////////////////////////////////////////////////////////////////
//
// Generated from the TEXTINCLUDE 2 resource.
//
#include "windows.h"

/////////////////////////////////////////////////////////////////////////////
#undef APSTUDIO_READONLY_SYMBOLS

/////////////////////////////////////////////////////////////////////////////
// English (U.S.) resources

#if !defined(AFX_RESOURCE_DLL) || defined(AFX_TARG_ENU)
#ifdef _WIN32
LANGUAGE LANG_ENGLISH, SUBLANG_ENGLISH_US
#pragma code_page(1252)
#endif //_WIN32

#ifdef APSTUDIO_INVOKED
/////////////////////////////////////////////////////////////////////////////
//
// TEXTINCLUDE
//

1 TEXTINCLUDE 
BEGIN
    "resource.h\0"
END

2 TEXTINCLUDE 
BEGIN
    "#include ""afxres.h""\r\n"
    "\0"
END

3 TEXTINCLUDE 
BEGIN
    "\r\n"
    "\0"
END

#endif    // APSTUDIO_INVOKED


#include <wx/msw/wx.rc>

/////////////////////////////////////////////////////////////////////////////
//
// Version
//

VS_VERSION_INFO VERSIONINFO
 FILEVERSION  ${H3DViewer_MAJOR_VERSION}, ${H3DViewer_MINOR_VERSION}, ${H3DViewer_BUILD_VERSION}, ${H3DViewer_SVN_VERSION}
 PRODUCTVERSION  ${H3DViewer_MAJOR_VERSION}, ${H3DViewer_MINOR_VERSION}, ${H3DViewer_BUILD_VERSION}, ${H3DViewer_SVN_VERSION}
 FILEFLAGSMASK 0x17L
#ifdef _DEBUG
 FILEFLAGS 0x1L
#else
 FILEFLAGS 0x0L
#endif
 FILEOS 0x4L
 FILETYPE 0x1L
 FILESUBTYPE 0x0L
BEGIN
    BLOCK "StringFileInfo"
    BEGIN
        BLOCK "041d04b0"
        BEGIN
            VALUE "Comments", "This is H3DViewer. A browser for viewing X3D files with H3D content."
            VALUE "CompanyName", "SenseGraphics AB"
            VALUE "FileDescription", "H3DViewer Application"
            VALUE "FileVersion", "${H3DViewer_MAJOR_VERSION}, ${H3DViewer_MINOR_VERSION}, ${H3DViewer_BUILD_VERSION}, ${H3DViewer_SVN_VERSION}"
            VALUE "InternalName", "H3DViewer"
            VALUE "LegalCopyright", "Copyright (C) 2009"
            VALUE "OriginalFilename", "${H3DViewer_Output_Name}"
            VALUE "ProductName", "H3DViewer Application"
            VALUE "ProductVersion", "${H3DViewer_MAJOR_VERSION}, ${H3DViewer_MINOR_VERSION}, ${H3DViewer_BUILD_VERSION}, ${H3DViewer_SVN_VERSION}"
        END
    END
    BLOCK "VarFileInfo"
    BEGIN
        VALUE "Translation", 0x41d, 1200
    END
END


/////////////////////////////////////////////////////////////////////////////
//
// Icon
//

// Icon with lowest ID value placed first to ensure application icon
// remains consistent on all systems.
IDI_ICON1               ICON                    "${H3DViewer_SOURCE_DIR}\H3DViewer.ico"
#endif    // English (U.S.) resources
/////////////////////////////////////////////////////////////////////////////



#ifndef APSTUDIO_INVOKED
/////////////////////////////////////////////////////////////////////////////
//
// Generated from the TEXTINCLUDE 3 resource.
//


/////////////////////////////////////////////////////////////////////////////
#endif    // not APSTUDIO_INVOKED

