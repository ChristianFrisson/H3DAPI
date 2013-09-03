//////////////////////////////////////////////////////////////////////////////
//    Copyright 2006-2013, SenseGraphics AB
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
/// \file WxMain.cpp
/// \brief CPP file for ParticleDemo
///
//
//////////////////////////////////////////////////////////////////////////////

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <WxFrame.h>
#include <WxWidgetsWindow.h>

// H3D includes
#include <H3D/VrmlParser.h>
#include <H3D/Group.h>
#include <H3D/Transform.h>
#include <H3D/Scene.h>
#include <H3D/KeySensor.h>
#include <H3D/MouseSensor.h>
#ifndef MACOSX
#include <H3D/SpaceWareSensor.h>
#endif
#include <H3D/DeviceInfo.h>
#include <H3D/INIFile.h>
#include <H3D/ResourceResolver.h>
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



H3D_API_EXCEPTION( QuitAPIException );

class QuitAPIField: public AutoUpdate< SFString > {
  virtual void update() {
    string s = static_cast< SFString * >(routes_in[0])->getValue();
    if( s[0] == 27 ) { 
      throw QuitAPIException();
    }
  }
};

const wxCmdLineEntryDesc gCmdLineDesc[] = 
  {
    { wxCMD_LINE_PARAM, NULL, NULL, 
#if( defined( wxUSE_UNICODE ) && wxMAJOR_VERSION == 2 && wxMINOR_VERSION <= 8 )
      wxT("File to load"), 
#else 
      "File to load", 
#endif
      wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL}, 
    { wxCMD_LINE_NONE, NULL, NULL, NULL, wxCMD_LINE_VAL_NONE, 0} };

// Define a new application type
class MyApp: public wxApp
{
public:
  virtual bool OnInit();

  virtual void OnIdle(wxIdleEvent& event);
  virtual bool OnExceptionInMainLoop();
  virtual void OnInitCmdLine(wxCmdLineParser& parser) {
    parser.SetDesc (gCmdLineDesc); 
  }

  virtual bool OnCmdLineParsed(wxCmdLineParser& parser) {
    for (unsigned int i = 0; i < parser.GetParamCount(); ++i) {
      cmd_line_filename = parser.GetParam(i);
    }

    return true;
  }
protected:
  wxString cmd_line_filename;

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
  catch (const QuitAPIException &) {
    return false;
  }
  catch (const Exception::H3DException &e) {
     wxMessageBox( wxString( e.message.c_str(), wxConvLibc ),
		   wxT("Error"), wxOK | wxICON_EXCLAMATION);
    return false;
  }
  wxApp::OnExceptionInMainLoop();  
}

bool MyApp::OnInit()
{
  // call default behaviour (mandatory)
  if (!wxApp::OnInit())
    return false;

  SetVendorName(_T("SenseGraphics AB"));
  SetAppName(_T("Particle Systems Demo"));

  //initializeH3D();

  // create a window to display
  WxFrame *theWxFrame = new WxFrame( NULL, wxID_ANY,
                                     wxT("Particle Systems - Demo"),
                                     wxDefaultPosition, wxSize(800, 600) );
	//glwindow constructed in the frame constructor.  Next line redundant.
	//theWxFrame->glwindow = new H3DWxWidgetsWindow(theWxFrame);
	theWxFrame->Show(true);

  if( cmd_line_filename != wxString() )
    theWxFrame->loadFile( toStr(cmd_line_filename) );
  //This next line is used to set the icon file h3d.ico, when created.
	//theWxframe->SetIcon(wxIcon(_T("h3d_icn")));

    
    // Using this line instead of the two previous lines will make
    // WxWidgetsWindow create an instance of a wxframe with no menus and use
    // this as parent to the canvas.
    // WxWidgetsWindow *glwindow = new WxWidgetsWindow();

  return true;
}

