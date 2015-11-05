//////////////////////////////////////////////////////////////////////////////
//    Copyright 2007-2013, SenseGraphics AB
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
/// \file H3DWxFrame.cpp
/// \brief Implementation file for H3DWxFrame
///
//
//////////////////////////////////////////////////////////////////////////////

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------

#include <WxFrame.h>
#include <ConsoleDialog.h>
#include <vector>
#include <WxWidgetsWindow.h>

#include <wx/wx.h>
#include <Envini.h>

// ---------------------------------------------------------------------------
//  Includes (to open X3D files)
// ---------------------------------------------------------------------------
// H3DUtil includes
#include <H3DUtil/Console.h>

// HAPI includes
#include <HAPI/HAPIHapticsRenderer.h>

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
#include <H3D/Viewpoint.h>
#include <H3D/X3DViewpointNode.h>
#include <H3D/DeviceInfo.h>
#include <H3D/INIFile.h>
#include <H3D/ResourceResolver.h>


#include <H3D/HapticsRenderers.h>

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



//AutoRef< GlobalSettings > global_settings;

/*******************Required Class***********************/

H3D_API_EXCEPTION( QuitAPIException );

class QuitAPIField: public AutoUpdate< SFString > {
  virtual void update() {
    string s = static_cast< SFString * >(routes_in[0])->getValue();
    if( s[0] == 27 ) { 
      throw QuitAPIException();
    }
  }
};

/*******************Global Constants*********************/
static const wxChar *TITLE     = _T("Particle Systems - Demo ");
static const wxChar *AUTHOR    = _T("\nRamkumar Shankar\n\nCopyright 2007.  All Rights Reserved.");
static const wxChar *ABOUT     = _T("About");
static const wxChar *FILETYPES = wxT( "x3d or vrml 2.0 files|*.x3d;*.x3dv;*.wrl|All files|*.*");

/*******************Constructor*********************/
WxFrame::WxFrame( wxWindow *_parent, wxWindowID _id,
                        const wxString& _title, const wxPoint& _pos,
                        const wxSize& _size, long _style,
                        const wxString& _name ):
wxFrame(_parent, _id, _title, _pos, _size, _style, _name )
{
   scene.reset( new Scene );
  ks.reset ( new KeySensor );
  ms.reset ( new MouseSensor );
  #ifndef MACOSX
  //ss.reset (new SpaceWareSensor);
  #endif
  t.reset ( new Transform );
  default_stylus.reset (new Node);
  viewpoint.reset (NULL);
  device_info.reset (NULL);
  g.reset ( new Group );

  glwindow = new WxWidgetsWindow(this);
  int width, height;
  GetClientSize(&width, &height);
  glwindow->width->setValue(width);
  glwindow->height->setValue(height);

  t->children->clear();
  //g->children->clear();

  //t->children->push_back( X3D::createVRMLFromString( "<Group />") );
  g->children->push_back( t.get() );
  scene->window->push_back( glwindow );
  scene->sceneRoot->setValue( g.get() );

  //scene->window->push_back( glwindow );

  wxString console_string = wxT("Console");
  the_console = new ConsoleDialog( this, wxID_ANY, console_string,
                                   wxDefaultPosition,
                                   wxDefaultSize, wxDEFAULT_DIALOG_STYLE);

  //WxWidgetsWindow *glwindow = new WxWidgetsWindow(theWxFrame);
  //Main Menu Bar
  menuBar  = (wxMenuBar *) NULL;

  //Top level menus
  fileMenu = (wxMenu *)  NULL;
  particleMenu = (wxMenu *) NULL;
  physicsMenu = (wxMenu *) NULL;
  emitterMenu = (wxMenu *) NULL;
  advancedMenu = (wxMenu *) NULL;
  helpMenu = (wxMenu *)  NULL;

  //Sub Menus
  geometryMenu = (wxMenu *) NULL;

  //Status Bar
  CreateStatusBar(2);

  /******************Menu Bar Items*****************/
  //File menu
  fileMenu = new wxMenu;
  fileMenu->Append( FRAME_OPEN, wxT("&Open file...\tCtrl+O"),
                    wxT("Open a file") );
  fileMenu->Append( FRAME_OPEN_URL,
                    wxT("&Open file from URL..."),
                    wxT("Open a file from URL") );
  fileMenu->Append( FRAME_CLOSE, wxT("&Close file"),
                    wxT("Close file") );
  fileMenu->AppendSeparator();
  fileMenu->Append(FRAME_EXIT, wxT("E&xit\tCtrl+X"), wxT("Exit") );

  //Particle Geometry sub menu
  geometryMenu = new wxMenu;
  geometryMenu->AppendRadioItem( FRAME_QUAD, wxT("Quad"),
                                 wxT("Quad Geometry") );
  geometryMenu->AppendRadioItem( FRAME_TRIANGLE,
                                 wxT("Triangle"),
                                 wxT("Triangle Geometry") );
  geometryMenu->AppendRadioItem( FRAME_POINT,
                                 wxT("Point"),
                                 wxT("Point Geometry") );
  geometryMenu->AppendRadioItem( FRAME_SPRITE,
                                 wxT("Sprite"),
                                 wxT("Sprite Geometry") );
  geometryMenu->AppendRadioItem( FRAME_LINE,
                                 wxT("Line"),
                                 wxT("Line Geometry") );

  //Particle menu
  particleMenu = new wxMenu;
  particleMenu->Append( FRAME_GEOMETRY, wxT("Geometry"),
                        geometryMenu, wxT("Select Particle Geometry") );

  //Physics menu
  physicsMenu = new wxMenu;
  physicsMenu->Append( FRAME_BOUNDEDPHYSICSMODEL,
                       wxT("Bounded Physics Model"),
                       wxT("Bounded Physics Model") );
  physicsMenu->Append( FRAME_GRAVITYPHYSICSMODEL,
                       wxT("Gravity Physics Model"),
                       wxT("Gravity Physics Model") );
  physicsMenu->Append( FRAME_WINDPHYSICSMODEL,
                       wxT("Wind Physics Model"),
                       wxT("Wind Physics Model") );

  //Emitter Menu
  emitterMenu = new wxMenu;
  emitterMenu->Append( FRAME_CONEEMITTER,
                       wxT("Cone"), wxT("Select Cone Emitter") );
  emitterMenu->Append( FRAME_EXPLOSIONEMITTER,
                       wxT("Explosion"), wxT("Select Explosion Emitter") );
  emitterMenu->Append( FRAME_POINTEMITTER,
                       wxT("Point"), wxT("Select Point Emitter") );
  emitterMenu->Append( FRAME_POLYLINEEMITTER,
                       wxT("Polyline"), wxT("Select Polyline Emitter") );
  emitterMenu->Append( FRAME_VOLUMEEMITTER,
                       wxT("Volume"), wxT("Select Volume Emitter") );
  emitterMenu->Append( FRAME_SURFACEEMITTER,
                       wxT("Surface"), wxT("Select Surface Emitter") );

  //Advanced Menu
  advancedMenu = new wxMenu;
  advancedMenu->Append( FRAME_CONSOLE,
                        wxT("Show Console"), wxT("Show the message console") );
  
  //Help Menu
  helpMenu = new wxMenu;
  helpMenu->Append( FRAME_HELP, wxT("Help") );
  helpMenu->Append( FRAME_ABOUT, wxT("About") );

  //Install Menu Bar
  menuBar = new wxMenuBar;
  menuBar->Append( fileMenu, wxT("&File") );
  menuBar->Append( particleMenu, wxT("&Particle System") );
  menuBar->Append( physicsMenu, wxT("&Physics") );
  menuBar->Append( emitterMenu, wxT("&Emitter") );
  menuBar->Append( advancedMenu, wxT("&Advanced") );
  menuBar->Append( helpMenu, wxT("&Help") );
  SetMenuBar(menuBar);

}

/*******************Event Table*********************/
BEGIN_EVENT_TABLE(WxFrame, wxFrame)
  EVT_MENU (FRAME_EXIT, WxFrame::OnExit)
  EVT_MENU (FRAME_OPEN, WxFrame::OnOpenFile)
  EVT_MENU (FRAME_OPEN_URL, WxFrame::OnOpenFileURL)
  EVT_MENU (FRAME_CLOSE, WxFrame::OnCloseFile)
  EVT_MENU (FRAME_CONSOLE, WxFrame::ShowConsole)
  EVT_MENU_HIGHLIGHT (FRAME_SELECTION, WxFrame::GetSelection)
  EVT_MENU (FRAME_ABOUT, WxFrame::OnAbout)
  EVT_MENU (FRAME_HELP, WxFrame::OnHelp)
  EVT_MENU_RANGE (FRAME_QUAD, FRAME_LINE, WxFrame::OnParticleGeometry)
  EVT_MENU (FRAME_PARTICLECONTROL, WxFrame::ToggleParticles)
  EVT_MENU (FRAME_PARTICLECREATION, WxFrame::ParticleGeneration)
  EVT_MENU (FRAME_POINTEMITTER, WxFrame::PointEmitterSettingsDialog)
  EVT_MENU (FRAME_POLYLINEEMITTER, WxFrame::PolylineEmitterSettingsDialog)
  EVT_MENU (FRAME_VOLUMEEMITTER, WxFrame::VolumeEmitterSettingsDialog)
  EVT_MENU (FRAME_CONEEMITTER, WxFrame::ConeEmitterSettingsDialog)
  EVT_MENU (FRAME_EXPLOSIONEMITTER, WxFrame::ExplosionEmitterSettingsDialog)
  EVT_MENU (FRAME_SURFACEEMITTER, WxFrame::SurfaceEmitterSettingsDialog)
  EVT_MENU (FRAME_BOUNDEDPHYSICSMODEL, WxFrame::BoundedPhysicsModelSettingsDialog)
  EVT_MENU (FRAME_GRAVITYPHYSICSMODEL, WxFrame::GravityPhysicsModelSettingsDialog)
  EVT_MENU (FRAME_WINDPHYSICSMODEL, WxFrame::WindPhysicsModelSettingsDialog)
  EVT_CLOSE(WxFrame::OnWindowExit)
END_EVENT_TABLE()

/*******************Event Table*********************/


/*******************Member Functions*********************/

bool WxFrame::loadFile( const string &filename) {
  
  char *r = getenv( "H3D_ROOT" );

  string h3d_root = r ? r : ""; 
  
  INIFile ini_file( h3d_root + "/settings/h3dload.ini" );
  
  //Clear existing data
  t->children->clear();
  viewpoint.reset (NULL);

  settings_path = 
    GET_ENV_INI_DEFAULT( "H3D_DISPLAY",
                         h3d_root + "/settings/display/",
                         "display","type",
                         h3d_root + "/settings/common/" );
  
  common_path =  h3d_root + "/settings/common/";

  deviceinfo_file =
    GET_ENV_INI_DEFAULT_FILE( ini_file, "H3D_DEFAULT_DEVICEINFO",
                              settings_path + "/device/",
                              common_path + "/device/",
                              "haptics device","device" );

  stylus_file =
    GET_ENV_INI_DEFAULT_FILE( ini_file, "H3D_STYLUS",
                              common_path + "/stylus/",
                              common_path + "/stylus/",
                              "haptics device","stylus" );
  
  viewpoint_file =
    GET_ENV_INI_DEFAULT_FILE( ini_file, "H3D_DEFAULT_VIEWPOINT",
                              settings_path + "/viewpoint/",
                              common_path + "/viewpoint/",
                              "graphical", "viewpoint" );

  render_mode = GET4( "H3D_RENDERMODE",
                             "graphical", "rendermode",
                             (string)"MONO" );
  
  bool fullscreen    = GET_BOOL("graphical", "fullscreen", false);
  if( char *buffer = getenv("H3D_FULLSCREEN") ) {
    if (strcmp( buffer, "TRUE" ) == 0 ){
      fullscreen = true; }
    else if (strcmp( buffer, "FALSE" ) == 0 ){
      fullscreen = false; }
    else
      Console(LogLevel::Error) << "Invalid valid value \"" << buffer 
                 << "\" on environment "
                 << "variable H3D_FULLSCREEN. Must be TRUE or FALSE. "
                 << endl;
  }
  
  bool mirrored      = GET_BOOL("graphical", "mirrored", false);
  if( char *buffer = getenv("H3D_MIRRORED") ) {
    if (strcmp( buffer, "TRUE" ) == 0 ){
      mirrored = true; }
    else if (strcmp( buffer, "FALSE" ) == 0 ){
      mirrored = false; }
    else
      Console(LogLevel::Error) << "Invalid valid value \"" << buffer 
                 << "\" on environment "
                 << "variable H3D_MIRRORED. Must be TRUE or FALSE. "<< endl;
  }

  bool use_space_mouse = false;

  // Loading X3D file and setting up VR environment ---
  
  try {
#ifndef MACOSX
    //if( use_space_mouse ) ss.reset( new SpaceWareSensor );
#endif
    X3D::DEFNodes dn;
//    KeyRotation *kr = new KeyRotation;
    QuitAPIField *quit_api = new QuitAPIField;

    if (!DeviceInfo::getActive()) {    
    if( deviceinfo_file.size() ){
      try {
        device_info = X3D::createX3DNodeFromURL( deviceinfo_file );
      } catch( const Exception::H3DException &e ) {
        Console(LogLevel::Warning) << "Warning: Could not create default DeviceInfo node "
                   << "from file \"" << deviceinfo_file << "\": "
                   << e << endl;
      }
    }

    di = DeviceInfo::getActive();
    hdev = (H3DHapticsDevice *) NULL;

    if( di && !(di->device->empty())) {
      hdev = di->device->getValueByIndex(0);
    }

    if( di && stylus_file.size() ) {
      try {
        default_stylus = X3D::createX3DNodeFromURL( stylus_file );
      } catch( const Exception::H3DException &e ) {
        Console(LogLevel::Error) << "Warning: Could not create default stylus "
                     << "from file \"" << stylus_file << "\": "
                     << e << endl;
      }
      
      for( DeviceInfo::MFDevice::const_iterator i = di->device->begin();
           i != di->device->end(); ++i ) {
        H3DHapticsDevice *d = static_cast< H3DHapticsDevice * >(*i);
        if( !d->stylus->getValue() )
          d->stylus->setValue( default_stylus );
      }
    }
    }

    DeviceInfo::DeviceInfoList DEVlist = DeviceInfo::getAllDeviceInfos();
    int devcount = int( DEVlist.size() );

    Console(LogLevel::Info) << "Loading " << filename << endl;
    if ( filename.size() > 4 && 
         filename.find( ".wrl", filename.size()-5 ) != string::npos )
      t->children->push_back( X3D::createVRMLFromURL( filename, 
                                                      &dn ) );
      else
        t->children->push_back( X3D::createX3DFromURL( filename, 
                                                       &dn ) );

    ParticleSystem::ParticleSystemList PSList = ParticleSystem::getAllParticleSystems();

    ParticleSystem::ParticleSystemList::iterator psiterator = PSList.begin();
    //for (psiterator = PSList.begin(); psiterator != PSList.end(); ++psiterator) {
    if(!PSList.empty()) {
      PS = PSList.front();
      if (PS->enabled->getValue()) {
        ParticleStatus = true;
        particleMenu->Insert( 0, FRAME_PARTICLECONTROL, 
                              wxT("Remove particles"),
                              wxT("Stop and remove all particles on screen") );
        particleMenu->Insert( 1, FRAME_PARTICLECREATION, 
                              wxT("Stop particle creation"),
                              wxT("Do not generate new particles") );
        particleMenu->InsertSeparator(2);
      }
      else {
        ParticleStatus = false;
        particleMenu->Insert( 0, FRAME_PARTICLECONTROL,
                              wxT("Display particles"),
                              wxT("Start and display all particles on screen") );
        particleMenu->Insert( 1, FRAME_PARTICLECREATION,
                              wxT("Start particle creation"),
                              wxT("Resume generation of new particles") );
        particleMenu->InsertSeparator(2);
      }
    }
    else {
      wxMessageDialog PSnotfound ( this, 
       wxT("Particle System node not found.\nCertain menus will be disabled."),
       wxT("Particle System"), wxOK);
      PSnotfound.ShowModal();
      menuBar->EnableTop(1, false);
    }

    pointEmitterSettings = new PointEmitterDialog(this, PS, hdev);
    polylineEmitterSettings = new PolylineEmitterDialog(this, PS, hdev);
    volumeEmitterSettings = new VolumeEmitterDialog(this, PS, hdev);
    coneEmitterSettings = new ConeEmitterDialog(this, PS, hdev);
    explosionEmitterSettings = new ExplosionEmitterDialog(this, PS, hdev);
    surfaceEmitterSettings =  new SurfaceEmitterDialog(this, PS, hdev);
    //}

  /****************************Device Info****************************/
  //Enable Device Menu

#ifndef MACOSX
    if( use_space_mouse )
      g->children->push_back(ss.get());
#endif    
    // create a Viewpoint if it does not exist.
    if( !Viewpoint::getActive() && viewpoint_file.size() ) {
      try {
        viewpoint = X3D::createX3DNodeFromURL( viewpoint_file );
      } catch( const Exception::H3DException &e ) {
        Console(LogLevel::Warning) << "Warning: Could not create default Viewpoint node "
                   << "from file \"" << viewpoint_file << "\": "
                   << e << endl;
      }
    }
    
    if( X3DBindableNode::getStack( "DeviceInfo" ).size() > 1 ) {
      device_info.reset( NULL );
    }

    this->glwindow->fullscreen->setValue( fullscreen );
    this->glwindow->mirrored->setValue( mirrored );
    this->glwindow->renderMode->setValue( render_mode );

  
    scene->window->push_back( glwindow );
    scene->sceneRoot->setValue( g.get() );
  }
  catch (const Exception::H3DException &e) {
    wxMessageBox( wxString( e.message.c_str(), wxConvLibc ),
                  wxT("Error"), wxOK | wxICON_EXCLAMATION );
    return false;
  }
  return true;
}

//Clear data when closing file
void WxFrame::clearData () {
  t->children->clear();
  viewpoint.reset (NULL);
}


//Open a file
void WxFrame::OnOpenFileURL(wxCommandEvent & event) {
   auto_ptr< wxTextEntryDialog > text_dialog( new wxTextEntryDialog ( this,
                             wxT("Enter the location of the file here"),
                             wxT("Open file from URL"),
                             wxT("") ) );
   if( text_dialog->ShowModal() == wxID_OK ) {
     string s( toStr( text_dialog->GetValue() ) );
     clearData();
     loadFile( s );
   }
}

void WxFrame::OnOpenFile(wxCommandEvent & event)
{
  auto_ptr< wxFileDialog > openFileDialog( new wxFileDialog ( this,
                             wxT("Open file"),
                             GetCurrentPath(),
                             wxT(""),
                             FILETYPES,
                             wxFD_OPEN,
                             wxDefaultPosition) );

  //Open an X3D file
  if (openFileDialog->ShowModal() == wxID_OK) {
    SetCurrentFilename(openFileDialog->GetFilename());  
    SetCurrentPath(openFileDialog->GetDirectory());
    SetStatusText(GetCurrentFilename(), 0);
    SetStatusText(openFileDialog->GetDirectory(),1);
#ifdef WIN32
    wxString wx_filename = currentPath + wxT("\\") + currentFilename;
#else
    wxString wx_filename = currentPath + wxT("/") + currentFilename;
#endif
    string filename( toStr(wx_filename) );
    clearData();
    loadFile( filename );
  }
}

//Close File
void WxFrame::OnCloseFile(wxCommandEvent & event) {
  //clearData();
  t->children->clear();
  SetStatusText(wxT("Open a file..."), 0);
  SetStatusText(wxT(""),1);
}


//About dialog
void WxFrame::OnAbout(wxCommandEvent & event)
{ wxString t = TITLE;

  t.append( AUTHOR );
  
  wxMessageDialog aboutDialog ( this, t, ABOUT, wxOK);
  aboutDialog.ShowModal();
}

//Particle Geometry event
void WxFrame::OnParticleGeometry(wxCommandEvent & event)
{
  string geometry;
  switch ( event.GetId() ) {
    case FRAME_QUAD:
      geometry = "QUAD";
      break;
    case FRAME_TRIANGLE:
      geometry = "TRIANGLE";
      break;
    case FRAME_POINT:
      geometry = "POINT";
      break;
    case FRAME_SPRITE:
      geometry = "SPRITE";
      break;
    case FRAME_LINE:
      geometry = "LINE";
      break;
  }
  PS->geometryType->setValue( geometry.c_str() );
}

//Toggle Particles
void WxFrame::ToggleParticles(wxCommandEvent & event)
{
  if (ParticleStatus) {
    PS->enabled->setValue(false);
    ParticleStatus = false;
    particleMenu->Destroy(FRAME_PARTICLECONTROL);
    particleMenu->Insert( 0, FRAME_PARTICLECONTROL, wxT("Display Particles"),
                          wxT("Start and display all particles on screen") );
  }
  else {
    PS->enabled->setValue(true);
    ParticleStatus = true;
    particleMenu->Destroy(FRAME_PARTICLECONTROL);
    particleMenu->Insert( 0, FRAME_PARTICLECONTROL, wxT("Remove Particles"),
                          wxT("Stop and remove all particles on screen") );
  }
}

//Toggle Particle Generation
void WxFrame::ParticleGeneration(wxCommandEvent & event)
{
  /*if (ParticleStatus) {
    PS->createParticles->setValue(false);
    ParticleStatus = false;
    particleMenu->Destroy(FRAME_PARTICLECREATION);
    particleMenu->Insert(1, FRAME_PARTICLECREATION, "Start particle creation", "Resume generation of new particles");
  }
  else {
    PS->createParticles->setValue(true);
    ParticleStatus = true;
    particleMenu->Destroy(FRAME_PARTICLECREATION);
    particleMenu->Insert(1, FRAME_PARTICLECREATION, "Stop particle creation", "Do not generate new particles");
  }*/

}

//Point Emitter settings
void WxFrame::PointEmitterSettingsDialog(wxCommandEvent & event)
{
  pointEmitterSettings->Show();
}

//Polyline Emitter settings
void WxFrame::PolylineEmitterSettingsDialog(wxCommandEvent & event)
{
  polylineEmitterSettings->Show();
}

//Volume Emitter settings
void WxFrame::VolumeEmitterSettingsDialog(wxCommandEvent & event)
{
  volumeEmitterSettings->Show();
}

//Cone Emitter settings
void WxFrame::ConeEmitterSettingsDialog(wxCommandEvent & event)
{
  coneEmitterSettings->Show();
}

//Explosion Emitter settings
void WxFrame::ExplosionEmitterSettingsDialog(wxCommandEvent & event)
{
  explosionEmitterSettings->Show();
}

//Surface Emitter settings
void WxFrame::SurfaceEmitterSettingsDialog(wxCommandEvent & event)
{
  surfaceEmitterSettings->Show();
}

//Bounded Physics Model settings
void WxFrame::BoundedPhysicsModelSettingsDialog(wxCommandEvent & event)
{
  boundedPhysicsModelSettings = new BoundedPhysicsModelDialog(this, PS);
  boundedPhysicsModelSettings->Show();
}

//Wind Physics Model settings
void WxFrame::WindPhysicsModelSettingsDialog(wxCommandEvent & event)
{
  windPhysicsModelSettings = new WindPhysicsModelDialog(this, PS);
  windPhysicsModelSettings->Show();
}

//Gravity Physics Model settings
void WxFrame::GravityPhysicsModelSettingsDialog(wxCommandEvent & event)
{
  gravityPhysicsModelSettings = new GravityPhysicsModelDialog(this, PS);
  gravityPhysicsModelSettings->Show();
}

//Help event
void WxFrame::OnHelp(wxCommandEvent & event)
{

}

//Show console event
void WxFrame::ShowConsole(wxCommandEvent & event)
{
  the_console->Show();
}

//Gets Menu Selections
void WxFrame::GetSelection (wxMenuEvent & event)
{
  selection = event.GetMenuId();
}

//Exit via menu
void WxFrame::OnExit (wxCommandEvent & event)
{
  Close(true);
}

//Exit via window manager
void WxFrame::OnWindowExit (wxCloseEvent & event) 
{
  Destroy();
}

/*******************Standard trivial functions*********************/
//Get current filename
wxString WxFrame::GetCurrentFilename()
{
 return currentFilename;
}

//Set current filename
void WxFrame::SetCurrentFilename(wxString n)
{
 currentFilename = n;
}

//Get current path
wxString WxFrame::GetCurrentPath()
{
 return currentPath;
}

//Set current path
void WxFrame::SetCurrentPath(wxString n)
{
 currentPath = n;
}

/*******************Create Geometries for use with emitters and physics models*********************/
void WxFrame::CreateGeometries()
{
  ////Box
  box = (Box *) NULL;
  box = new Box;
  box->size->setValue(Vec3f(0.2f, 0.2f, 0.2f));
  box->solid->setValue(true);

  ////Cone
  cone = (Cone *) NULL;
  cone = new Cone;
  cone->bottom->setValue(true);
  cone->bottomRadius->setValue(0.1f);
  cone->height->setValue(0.2f);
  cone->side->setValue(true);
  cone->solid->setValue(true);

  ////Cylinder
  cylinder = (Cylinder *) NULL;
  cylinder = new Cylinder;
  cylinder->bottom->setValue(true);
  cylinder->height->setValue(0.2f);
  cylinder->radius->setValue(0.1f);
  cylinder->side->setValue(true);
  cylinder->solid->setValue(true);
  cylinder->top->setValue(true);

  ////Sphere
  sphere = (Sphere *) NULL;
  sphere = new Sphere;
  sphere->radius->setValue(0.1f);
  sphere->solid->setValue(true);
}
