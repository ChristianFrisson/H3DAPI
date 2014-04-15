//////////////////////////////////////////////////////////////////////////////
//    Copyright 2006-2014, SenseGraphics AB
//
//    This file is part of H3D API.
//
//    H3D API is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    H3D API is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with H3D API; if not, write to the Free Software
//    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//    A commercial license is also available. Please contact us at 
//    www.sensegraphics.com for more information.
//
//
/// \file H3DViewer.cpp
/// \brief CPP file for H3DViewer.
///
//
//////////////////////////////////////////////////////////////////////////////

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include "WxFrame.h"
#include "WxWidgetsWindow.h"

#include <H3D/Scene.h>

#include <H3D/PythonScript.h>

#include <wx/cmdline.h>

// ---------------------------------------------------------------------------
//  Required classes and definitions
// ---------------------------------------------------------------------------
using namespace std;
using namespace H3D;

inline string toStr( const wxString &s ) {
# if(wxUSE_UNICODE)
  char *b = new char[s.size()+1];
  const wchar_t *wb = s.c_str();
  for( unsigned int i = 0; i < s.size(); ++i ) {
    b[i] = (char)(wb[i]);
  }
  
  b[s.size()] = '\0';
  string sb(b);
  delete[] b;
  return sb;
#else
  return string( s.c_str() );
#endif
}  

const wxCmdLineEntryDesc gCmdLineDesc[] = 
  {
    { wxCMD_LINE_SWITCH,
#if( defined( wxUSE_UNICODE ) && wxMAJOR_VERSION == 2 && wxMINOR_VERSION <= 8 )
      wxT("dp"), wxT("disable_plugins"),
      wxT("No registered plugins are loaded on startup. They are however still listed in the plugins dialog and can be enabled if desired.")
#else
      "dp", "disable_plugins",
      "No registered plugins are loaded on startup. They are however still listed in the plugins dialog and can be enabled if desired."
#endif
    },
    { wxCMD_LINE_PARAM, NULL, NULL, 
#if( defined( wxUSE_UNICODE ) && wxMAJOR_VERSION == 2 && wxMINOR_VERSION <= 8 )
      wxT("File to load"), 
#else 
      "File to load", 
#endif
wxCMD_LINE_VAL_STRING,
      wxCMD_LINE_PARAM_OPTIONAL }, 
    { wxCMD_LINE_NONE, NULL, NULL, NULL, wxCMD_LINE_VAL_NONE, 0} };

// Define a new application type
class MyApp: public wxApp
{
public:
  MyApp():
    theWxFrame( NULL ) {

  }
  virtual bool OnInit();
  virtual void MacOpenFile(const wxString &fileName) {
    if( theWxFrame ) {
      theWxFrame->clearData();
      theWxFrame->loadFile(toStr(fileName)); 
    } else {
      cmd_line_filename = fileName; 
    }
  }
  virtual void OnIdle(wxIdleEvent& event);
  virtual bool OnExceptionInMainLoop();
  virtual void OnInitCmdLine(wxCmdLineParser& parser) {
    parser.SetDesc (gCmdLineDesc); 
  }

  virtual bool OnCmdLineParsed(wxCmdLineParser& parser) {
    disable_plugin_dialog = parser.Found( wxString( "dp", wxConvUTF8 ) );
    for (int i = 0; i < (int)parser.GetParamCount(); ++i) {
      cmd_line_filename = parser.GetParam(i);
    }

    return true;
  }
protected:
  wxString cmd_line_filename;
  bool disable_plugin_dialog;
  WxFrame *theWxFrame;
  DECLARE_EVENT_TABLE()
};

BEGIN_EVENT_TABLE(MyApp, wxApp)
  EVT_IDLE(MyApp::OnIdle)
END_EVENT_TABLE()

void MyApp::OnIdle(wxIdleEvent& event) {
  for( set< Scene * >::iterator i = Scene::scenes.begin();
    i != Scene::scenes.end();
    ++i ) {
    if( (*i)->isActive() )
      (*i)->idle();
  }

#if defined( H3D_WINDOWS ) || defined( H3D_OSX ) || ( wxMAJOR_VERSION <= 2 && wxMINOR_VERSION < 9 )
  wxApp::OnIdle(event);
#endif
}

IMPLEMENT_APP(MyApp)

bool MyApp::OnExceptionInMainLoop() {
  try {
    throw;
  }
  catch (const Exception::QuitAPI &) {
    // Ensure we exit cleanly even on QuitAPI exception
    wxCommandEvent fake_event;
    theWxFrame->OnExit ( fake_event );
    return false;
  }
  catch (const Exception::H3DException &e) {
     stringstream s;
   s << e;
     wxMessageBox( wxString(s.str().c_str(),wxConvLibc),
                   wxT("Error"), wxOK | wxICON_EXCLAMATION);
    return false;
  }
  wxApp::OnExceptionInMainLoop();  
}

bool MyApp::OnInit()
{
  try {
    // call default behaviour (mandatory)
    if (!wxApp::OnInit())
      return false;

    SetVendorName(_T("SenseGraphics AB"));
    wxString tmp_string = H3DVIEWER_APP_NAME;
#ifdef H3D_ARCH64
    tmp_string += wxT("(x64)");
#else
    tmp_string += wxT("(x86)");
#endif 
#ifndef H3DVIEWER_STANDALONE
    tmp_string += wxT("(dev)");
#endif
#ifdef _DEBUG
    tmp_string += wxT("(debug)");
#endif
    SetAppName( tmp_string );
  
    Console.setShowLevel( false );

#ifdef H3DAPI_LIB
    initializeH3D();
#endif
    
#ifdef HAVE_PYTHON
    char argvd[32][256] = { '\0' };
    static char *(argv[32]);
    for( int idx = 0 ; idx < wxApp::argc ; ++idx ){
      strncpy(argvd[idx],wxString(wxApp::argv[idx]).mb_str(wxConvUTF8),255);
      argv[idx] = argvd[idx];
    }
    PythonScript::setargv( wxApp::argc, argv );
#endif
    
    // create a window to display
    theWxFrame = new WxFrame(NULL, wxID_ANY, wxT("H3DViewer"),
           wxDefaultPosition, wxSize(800, 600), wxDEFAULT_FRAME_STYLE, wxT("H3D Player"), !cmd_line_filename.IsEmpty(), disable_plugin_dialog );
    
    theWxFrame->Show(true);
    
  if( !cmd_line_filename.IsEmpty() ) {
      theWxFrame->clearData();
      theWxFrame->loadFile(toStr(cmd_line_filename));
    }

    //This next line is used to set the icon file h3d.ico, when created.
    //theWxframe->SetIcon(wxIcon(_T("h3d_icn")));
    
    
    // Using this line instead of the two previous lines will make
    // WxWidgetsWindow create an instance of a wxframe with no menus and use
    // this as parent to the canvas.
    // WxWidgetsWindow *glwindow = new WxWidgetsWindow();

  } catch (const Exception::H3DException &e) {
    cerr << e << endl;
    return false;
  }

  return true;
}

