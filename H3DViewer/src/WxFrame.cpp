//////////////////////////////////////////////////////////////////////////////
//    Copyright 2006-2007, SenseGraphics AB
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
/// \file WxFrame.cpp
/// \brief CPP file for WxFrame.
///
//
//
//////////////////////////////////////////////////////////////////////////////

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------

#include "WxFrame.h"
#include "ConsoleDialog.h"
#include <vector>
#include "WxWidgetsWindow.h"

#include <wx/wx.h>
#include "Envini.h"

// ---------------------------------------------------------------------------
//  Includes (to open X3D files)
// ---------------------------------------------------------------------------

#include <VrmlParser.h>
#include <Group.h>
#include <Transform.h>
#include <Scene.h>
#include <KeySensor.h>
#include <MouseSensor.h>

#ifndef MACOSX
#include <SpaceWareSensor.h>
#endif

#include <Viewpoint.h>
#include <X3DViewpointNode.h>
#include <DeviceInfo.h>
#include <INIFile.h>
#include <ResourceResolver.h>
#include <H3DUtil/Console.h>

#include <GraphicsCachingOptions.h>
#include <DebugOptions.h>
#include <HapticsOptions.h>
#include <GeometryBoundTreeOptions.h>
#include <OpenHapticsOptions.h>

#include <HapticsRenderers.h>
#include <HAPI/HAPIHapticsRenderer.h>

#include <H3DNavigation.h>

using namespace std;
using namespace H3D;


AutoRef< GlobalSettings > global_settings;

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
static const wxChar *TITLE     = _T("H3D Viewer ");
static const wxChar *AUTHOR    = _T("\nSenseGraphics\n\nCopyright 2006-2007.\n"
                                    "All Rights Reserved.");
static const wxChar *ABOUT     = _T("About");
static const wxChar *FILETYPES = _T( "x3d files|*.x3d|"
                                     "vrml files|*.wrl|"
                                     "All files|*.*"
                                   );

/*******************Constructor*********************/
WxFrame::WxFrame( wxWindow *_parent, wxWindowID _id,
                        const wxString& _title, const wxPoint& _pos,
                        const wxSize& _size, long _style,
                        const wxString& _name ):
  viewpointCount(0),
  navTypeCount(0),
  deviceCount(0),
  navigationDevices(0),
wxFrame(_parent, _id, _title, _pos, _size, _style, _name )
{
  wxAcceleratorEntry entries[1];
  entries[0].Set(wxACCEL_NORMAL, (int) WXK_F11, FRAME_RESTORE);
  wxAcceleratorTable accel(1, entries);
  SetAcceleratorTable(accel);

  	scene.reset( new Scene );
	ks.reset ( new KeySensor );
	ms.reset ( new MouseSensor );
	#ifndef MACOSX
	ss.reset (0);
	#endif
	t.reset ( new Transform );
	default_stylus.reset (NULL);
	viewpoint.reset (NULL);
	device_info.reset (NULL);
	g.reset ( new Group );

	glwindow = new WxWidgetsWindow(this);
	int width, height;
	GetClientSize(&width, &height);
	glwindow->width->setValue(width);
  glwindow->height->setValue(height);

	t->children->clear();
	g->children->push_back( t.get() );
	scene->window->push_back( glwindow );
	scene->sceneRoot->setValue( g.get() );

  wxString console_string = "Console";
  theConsole = new consoleDialog(this, wxID_ANY, console_string, 
                                 wxDefaultPosition, wxDefaultSize,
                                 wxDEFAULT_DIALOG_STYLE);
  frameRates = new FrameRateDialog( this );

  defaultvp = (X3DViewpointNode *) NULL;
  mydevice = (DeviceInfo *) NULL;
  mynav = (NavigationInfo *) NULL;

  //Haptics on by default
  /*********TODO: Save this in registry for next session**************/
  lastDeviceStatus = true;

  //Main Menu Bar
  menuBar  = (wxMenuBar *) NULL;

  //Top level menus
  fileMenu = (wxMenu *)	NULL;
  rendererMenu = (wxMenu *) NULL;
  deviceMenu = (wxMenu *) NULL;
  viewpointMenu = (wxMenu *) NULL;
  navigationMenu = (wxMenu *) NULL;
  advancedMenu = (wxMenu *) NULL;
  helpMenu = (wxMenu *)	NULL;

  //Submenus
  hapticsRenderer = (wxMenu *) NULL;
  renderMode = (wxMenu *) NULL;

  global_settings.reset( new GlobalSettings );
  global_settings->options->push_back( new DebugOptions );
  global_settings->options->push_back( new GraphicsCachingOptions );
  global_settings->options->push_back( new GeometryBoundTreeOptions );
  global_settings->options->push_back( new OpenHapticsOptions );
  global_settings->options->push_back( new HapticsOptions );

  //File History
  recentFiles = (wxFileHistory *) NULL;
  h3dConfig = (wxConfigBase *) NULL;

  //Status Bar
  CreateStatusBar(2);

  /******************Menu Bar Items*****************/
  //File menu
  fileMenu = new wxMenu;
  fileMenu->Append(FRAME_OPEN,"&Open file...\tCtrl+O","Open a file");
  fileMenu->Append(FRAME_OPEN_URL, "&Open file from URL...", "Open a file "
                   "from URL" );
  fileMenu->Append(FRAME_CLOSE, "&Close file","Close file");
  fileMenu->AppendSeparator();
  fileMenu->Append(FRAME_CHOOSEDIR, "Change Working Directory", "Change "
                   "working directory...");
  fileMenu->AppendSeparator();
  fileMenu->Append(FRAME_EXIT,"E&xit\tCtrl+X", "Exit");

  //File History
  recentFiles = new wxFileHistory;
  recentFiles->UseMenu(fileMenu);

  //Initialize config object
  wxConfigBase *h3dConfig = wxConfigBase::Get();

  //Load file history and settings from previous session(s)
  LoadMRU();
  LoadSettings();

  //Create settings dialog
  settings = new SettingsDialog(this, global_settings.get(), this );

  //Submenus for Renderer Menu
  //hapticsRenderer
  hapticsRenderer = new wxMenu;
  hapticsRenderer->AppendRadioItem(FRAME_OPENHAPTICS, "Openhaptics", 
                                   "Openhaptics Renderer");
  hapticsRenderer->AppendRadioItem(FRAME_CHAI3D, "CHAI3D", "CHAI3D Renderer");
  hapticsRenderer->AppendRadioItem(FRAME_GODOBJECT, "GodObject (prototype)", 
                                   "GodObject Renderer");
  hapticsRenderer->AppendRadioItem(FRAME_RUSPINI, "Ruspini (early prototype)", 
                                   "Ruspini Renderer");

  //renderMode
  renderMode = new wxMenu;
  renderMode->AppendRadioItem(FRAME_MONO, "No Stereo",
                              "Disable stereo display");
  renderMode->AppendRadioItem(FRAME_QUADBUFFERED, "Quad Buffered Stereo",
                              "Quad Buffered Stereo");
  renderMode->AppendRadioItem(FRAME_HORZSPLIT, "Horizontal Split",
                              "Horizontal Split");
  renderMode->AppendRadioItem(FRAME_VERTSPLIT, "Vertical Split",
                              "Vertical Split");
  renderMode->AppendRadioItem(FRAME_HORZINTERLACED, "Horizontal Interlaced",
                              "Horizontal Interlaced");
  renderMode->AppendRadioItem(FRAME_VERTINTERLACED, "Vertical Interlaced",
                              "Vertical Interlaced");
  renderMode->AppendRadioItem(FRAME_SHARPDISPLAY, "Sharp Display",
                              "Optimized for Sharp display systems");
  renderMode->AppendRadioItem(FRAME_REDBLUE, "Red-Blue Stereo",
                              "Red-Blue Stereo mode");
  renderMode->AppendRadioItem(FRAME_REDCYAN, "Red-Cyan Stereo",
                              "Red-Cyan Stereo mode");

  //Renderer Menu
  rendererMenu = new wxMenu;
  rendererMenu->AppendCheckItem(FRAME_FULLSCREEN, "Fullscreen Mode",
                                "View in fullscreen");
  rendererMenu->AppendCheckItem(FRAME_MIRROR, "Mirror in Y",
                                "Mirror Scene in Y");
  rendererMenu->AppendSeparator();
  rendererMenu->Append(FRAME_CHOOSERENDERER, "Choose Haptics Renderer",
                       hapticsRenderer, "Select a haptics renderer");
  rendererMenu->Append(FRAME_RENDERMODE, "Select Render Mode", renderMode,
                       "Graphical Renderer Options");
  rendererMenu->AppendSeparator();
  rendererMenu->Append(FRAME_SETTINGS, "Settings...",
                       "Scenegraph rendering options");

  //Device Control Menu
  deviceMenu = new wxMenu;
  deviceMenu->Append(FRAME_DEVICECONTROL, "Start Device(s)", 
                      "Start haptic devices");
  deviceMenu->Append(FRAME_DEVICECONTROL, "Stop Device(s)",
                      "Stop haptic devices");

  //Viewpoint Menu
  viewpointMenu = new wxMenu;

  //Navigation Menu
  navigationMenu = new wxMenu;

  //Advanced Menu
  advancedMenu = new wxMenu;
  advancedMenu->Append(FRAME_CONSOLE, "Show Console",
                       "Show the message console");
  advancedMenu->Append(FRAME_FRAMERATE, "Show frame rates",
                       "Show the frame rates of graphics and haptics loop");
  
  //Help Menu
  helpMenu = new wxMenu;
  helpMenu->Append(FRAME_HELP, "Help");
  helpMenu->Append(FRAME_ABOUT, "About");

  //Install Menu Bar
  menuBar = new wxMenuBar;
  menuBar->Append(fileMenu, "&File");
  menuBar->Append(rendererMenu, "&Rendering");
  menuBar->Append(deviceMenu, "&Device Control");
  menuBar->Append(viewpointMenu, "&Viewpoints");
  menuBar->Append(navigationMenu, "&Navigation");
  menuBar->Append(advancedMenu, "&Advanced");
  menuBar->Append(helpMenu, "&Help");
  SetMenuBar(menuBar);

  //Disable some menus initially
  //Top menu items
  menuBar->EnableTop(2, false);
  menuBar->EnableTop(3, false);
  menuBar->EnableTop(4, false);

  //Certain options in rendererMenu
  rendererMenu->Enable(FRAME_CHOOSERENDERER, false);
  rendererMenu->Enable(FRAME_RENDERMODE, false);

}

/*******************Event Table*********************/
BEGIN_EVENT_TABLE(WxFrame, wxFrame)
	EVT_MENU (FRAME_EXIT, WxFrame::OnExit)
	EVT_MENU (FRAME_OPEN, WxFrame::OnOpenFile)
  EVT_MENU_RANGE (wxID_FILE1, wxID_FILE9, WxFrame::OnMRUFile)
  EVT_MENU (FRAME_OPEN_URL, WxFrame::OnOpenFileURL)
	EVT_MENU (FRAME_CLOSE, WxFrame::OnCloseFile)
  EVT_MENU (FRAME_CHOOSEDIR, WxFrame::OnChooseDir)
	EVT_MENU (FRAME_FULLSCREEN, WxFrame::OnFullscreen)
	EVT_MENU (FRAME_SETTINGS, WxFrame::OnSettings)
  EVT_MENU (FRAME_RESTORE, WxFrame::RestoreWindow)
	EVT_MENU (FRAME_MIRROR, WxFrame::MirrorScene)
  EVT_MENU_RANGE (FRAME_MONO, FRAME_REDCYAN, WxFrame::RenderMode)
	EVT_MENU (FRAME_CONSOLE, WxFrame::ShowConsole)
	EVT_MENU (FRAME_FRAMERATE, WxFrame::ShowFrameRate)
	EVT_MENU_HIGHLIGHT (FRAME_SELECTION, WxFrame::GetSelection)
	EVT_MENU (FRAME_VIEWPOINT, WxFrame::ChangeViewpoint)
	EVT_MENU (FRAME_RESET_VIEWPOINT, WxFrame::ResetViewpoint)
	EVT_MENU (FRAME_NAVIGATION, WxFrame::ChangeNavigation)
  EVT_MENU_RANGE (FRAME_MOUSE_NAV, FRAME_HAPTICSDEVICE_NAV,
                  WxFrame::ChangeNavigationDevice)
  EVT_MENU_RANGE (FRAME_OPENHAPTICS, FRAME_RUSPINI, WxFrame::ChangeRenderer)
  EVT_MENU (FRAME_DEVICECONTROL, WxFrame::ToggleHaptics)
	EVT_MENU (FRAME_ABOUT, WxFrame::OnAbout)
	EVT_MENU (FRAME_HELP, WxFrame::OnHelp)
  EVT_CLOSE(WxFrame::OnWindowExit)
END_EVENT_TABLE()


/*******************Event Table*********************/

void SettingsDialog::handleSettingsChange (wxCommandEvent & event) {
  DebugOptions *dgo = NULL;
  global_settings->getOptionNode( dgo );
  int id = event.GetId(); 
  if( dgo ) {
    if( id == ID_DRAW_BOUNDS ) 
      dgo->drawBound->setValue( event.IsChecked() );
    else if( id == ID_DRAW_TRIANGLES ) 
      dgo->drawHapticTriangles->setValue( event.IsChecked() );
    else if( id == ID_DRAW_BOUND_TREE ) {
      if( event.IsChecked() ) {
        dgo->drawBoundTree->setValue( treeDepth );
        boundTree = true;
      }
      else {
        dgo->drawBoundTree->setValue( -1 );
        boundTree = false;
      }
    } else if( id == ID_DRAW_TREE_DEPTH ) {
      dgo->drawBoundTree->setValue( event.GetInt() );
    }
      
    H3DDisplayListObject::DisplayList::rebuildAllDisplayLists();    
  }

  GraphicsCachingOptions *gco = NULL;
  global_settings->getOptionNode( gco );

  if( gco ) {
    if( id == ID_USE_DISPLAY_LISTS ) 
      gco->useCaching->setValue( event.IsChecked() );
    else if( id == ID_CACHE_ONLY_GEOMS ) {
      gco->cacheOnlyGeometries->setValue( event.IsChecked() );
    }
  }

  HapticsOptions *ho = NULL;
  global_settings->getOptionNode( ho );

  if( ho ) {
    if( id == ID_TOUCHABLE_FACE ) {
      int i = event.GetSelection();
      if( i == 0 ) ho->touchableFace->setValue( "AS_GRAPHICS" );
      else if( i == 1 ) ho->touchableFace->setValue( "FRONT_AND_BACK" );
      else if( i == 2 ) ho->touchableFace->setValue( "FRONT" );
      else if( i == 3 ) ho->touchableFace->setValue( "BACK" );
    } else if( id == ID_MAX_DISTANCE ) {
      ho->maxDistance->setValue( atof( event.GetString() ) );
    } else if( id == ID_LOOK_AHEAD_FACTOR ) {
      ho->lookAheadFactor->setValue( atof( event.GetString() ) );
    } else if( id == ID_USE_BOUND_TREE ) {
      ho->useBoundTree->setValue( event.IsChecked() );
    }
  }

  OpenHapticsOptions *oho = NULL;

  global_settings->getOptionNode( oho );

  if( oho ) {
    if( id == ID_OH_SHAPE_TYPE ) {
      int i = event.GetSelection();
      if( i == 0 ) oho->GLShape->setValue( "FEEDBACK_BUFFER" );
      else if( i == 1 ) oho->GLShape->setValue( "DEPTH_BUFFER" );
      else if( i == 2 ) oho->GLShape->setValue( "CUSTOM" );
    } else if( id == ID_ADAPTIVE_VIEWPORT ) {
      oho->useAdaptiveViewport->setValue( event.IsChecked() );
    } else if( id == ID_HAPTIC_CAMERA ) {
      oho->useHapticCameraView->setValue( event.IsChecked() );
    } else if( id == ID_FULL_GEOMETRY_RENDER ) {
      oho->forceFullGeometryRender->setValue( event.IsChecked() );
    }
  }

  GeometryBoundTreeOptions *gbto = NULL;
  global_settings->getOptionNode( gbto );

  if( gbto ) {
    if( id == ID_BOUND_TYPE ) {
      int i = event.GetSelection();
      if( i == 0 ) gbto->boundType->setValue( "OBB" );
      else if( i == 1 ) gbto->boundType->setValue( "AABB" );
      else if( i == 2 ) gbto->boundType->setValue( "SPHERE" );
    }
  }

  if( id == ID_PROXY_RADIUS ) {
    wx_frame->setProxyRadius( atof( event.GetString() ) );
  }
}

void SettingsDialog::handleSpinEvent (wxSpinEvent & event) {
  DebugOptions *dgo = NULL;
  global_settings->getOptionNode( dgo );
  int id = event.GetId(); 
  if( dgo ) {
    if( (id == ID_DRAW_TREE_DEPTH) && (boundTree) ) {
      dgo->drawBoundTree->setValue( event.GetInt() );
    } 
    treeDepth = event.GetInt();
    H3DDisplayListObject::DisplayList::rebuildAllDisplayLists();    
  }

  GraphicsCachingOptions *gco = NULL;
  global_settings->getOptionNode( gco );

  if( gco ) {
    if( id == ID_CACHING_DELAY ) 
      gco->cachingDelay->setValue( event.GetInt() );
  }

  GeometryBoundTreeOptions *gbto = NULL;
  global_settings->getOptionNode( gbto );

  if( gbto ) {
    if( id == ID_MAX_TRIANGLES ) 
      gbto->maxTrianglesInLeaf->setValue( event.GetInt() );
  }
}


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
      Console(4) << "Invalid value \"" << buffer 
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
      Console(4) << "Invalid value \"" << buffer 
                 << "\" on environment "
                 << "variable H3D_MIRRORED. Must be TRUE or FALSE. "<< endl;
  }

  bool use_space_mouse = false;

  // Loading X3D file and setting up VR environment ---
  
  try {
#ifndef MACOSX
    if( use_space_mouse ) ss.reset( new SpaceWareSensor );
#endif
    X3D::DEFNodes dn;
    QuitAPIField *quit_api = new QuitAPIField;

    if (!DeviceInfo::getActive()) {    
    if( deviceinfo_file.size() ){
      try {
        device_info = X3D::createX3DNodeFromURL( deviceinfo_file );
      } catch( const Exception::H3DException &e ) {
        Console(3) << "Warning: Could not create default DeviceInfo node "
                   << "from file \"" << deviceinfo_file << "\": "
                   << e << endl;
      }
    }

    DeviceInfo *di = DeviceInfo::getActive();
    if( di && stylus_file.size() ) {
      try {
        default_stylus = X3D::createX3DNodeFromURL( stylus_file, 
                                                    &default_stylus_dn );
        Node *proxy = default_stylus_dn.getNode("PROXY");
        if( proxy ) {
          SFFloat *radius = 
            dynamic_cast< SFFloat * >( proxy->getField("radius") );
          if( radius ) 
            original_proxy_radius = radius->getValue();
          setProxyRadius( original_proxy_radius );
        }
      } catch( const Exception::H3DException &e ) {
        Console( 4 ) << "Warning: Could not create default stylus "
                     << "from file \"" << stylus_file << "\": "
                     << e << endl;
      }
      
      for( DeviceInfo::MFDevice::const_iterator i = di->device->begin();
           i != di->device->end(); i++ ) {
        H3DHapticsDevice *d = static_cast< H3DHapticsDevice * >(*i);
        if( !d->stylus->getValue() )
          d->stylus->setValue( default_stylus );
      }
    }
    }

    DeviceInfo::DeviceInfoList DEVlist = DeviceInfo::getAllDeviceInfos();
    int devcount = DEVlist.size();

    Console(3) << "Loading " << filename << endl;
    if ( filename.size() > 4 && 
         filename.find( ".wrl", filename.size()-5 ) != string::npos )
      t->children->push_back( X3D::createVRMLFromURL( filename, 
                                                      &dn ) );
      else
        t->children->push_back( X3D::createX3DFromURL( filename, 
                                                       &dn ) );

    /**********************Reset mirrored and fullscreen********************/
  rendererMenu->Check(FRAME_FULLSCREEN, false);
  rendererMenu->Check(FRAME_MIRROR, false);

	/****************************Navigation Info****************************/
	//Enable Navigation Menu
	menuBar->EnableTop(4, true);
	buildNavMenu();

  //Enable graphical rendering options in rendererMenu
  rendererMenu->Enable(FRAME_RENDERMODE, true);
  rendererMenu->Enable(FRAME_CHOOSERENDERER, true);

	/****************************Device Info****************************/
	//Enable Device Menu
  mydevice = DeviceInfo::getActive();
  if (mydevice && (mydevice->device->size() > 0) ) {
    menuBar->EnableTop(2, true);
	allDevices = mydevice->device->getValue();
  }

/*
#ifndef MACOSX
    if( use_space_mouse )
      ss->instantRotation->route( kr );
#endif
*/

#ifndef MACOSX
    if( use_space_mouse )
    g->children->push_back(ss.get());
#endif
    //create a Viewpoint if it does not exist.
    if( !Viewpoint::getActive() && viewpoint_file.size() ) {
      try {
        viewpoint = X3D::createX3DNodeFromURL( viewpoint_file );
      } catch( const Exception::H3DException &e ) {
        Console(3) << "Warning: Could not create default Viewpoint node "
                   << "from file \"" << viewpoint_file << "\": "
                   << e << endl;
      }
    }

/****************************Intialize Viewpoints***************************/
  //Enable Viewpoints Menu
  menuBar->EnableTop(3, true);
  VPlist = X3DViewpointNode::getViewpointHierarchy();
  defaultvp = Viewpoint::getActive();
  
  //Store number of viewpoints for processing later
  viewpointCount = VPlist.size();
  
  //Create Viewpoints menu using list
  //If no viewpoints are specified in the file
  if (viewpointCount <= 1) {
    viewpointMenu->Append(FRAME_VIEWPOINT, "Default", "Default Viewpoint");
    viewpointMenu->Enable(FRAME_VIEWPOINT, true);
  }
  else {
    int i = 0;
    for (X3DViewpointNode::ViewpointList::iterator vp = VPlist.begin(); 
           vp != VPlist.end(); vp++) {
      string vpDescription = (*vp)->description->getValue();
			viewpointMenu->AppendRadioItem(FRAME_VIEWPOINT + i, 
                                     vpDescription.c_str(),
                                     "Select a viewpoint");
      if ((*vp) == Viewpoint::getActive()) {
        viewpointMenu->Check(FRAME_VIEWPOINT+i, true);
      }
			Connect(FRAME_VIEWPOINT +i,wxEVT_MENU_HIGHLIGHT,
              wxMenuEventHandler(WxFrame::GetSelection));
			Connect(FRAME_VIEWPOINT +i,wxEVT_COMMAND_MENU_SELECTED,
              wxCommandEventHandler(WxFrame::ChangeViewpoint));
      i++;
		}
  }
  viewpointMenu->AppendSeparator();
  viewpointMenu->Append(FRAME_RESET_VIEWPOINT, 
                                     "Reset",
                                     "Reset current viewpoint");
    
    if( X3DBindableNode::getStack( "DeviceInfo" ).size() > 1 ) {
      device_info.reset( NULL );
    }

    // create a window to display
    
	//This next line is used to set the icon file h3d.ico, when created.
	//theWxframe->SetIcon(wxIcon(_T("h3d_icn")));
  
  // Using this line instead of the two previous lines will make
  // WxWidgetsWindow create an instance of a wxframe with no menus and use
  // this as parent to the canvas.
  // WxWidgetsWindow *glwindow = new WxWidgetsWindow();

	this->glwindow->fullscreen->setValue( fullscreen );
  this->glwindow->mirrored->setValue( mirrored );
  this->glwindow->renderMode->setValue( render_mode );

	
  scene->sceneRoot->setValue( g.get() );
  }
  catch (const Exception::H3DException &e) {
    wxMessageBox(e.message.c_str(), "Error", wxOK | wxICON_EXCLAMATION);
    return false;
  }
  return true;
}

//Clear data when closing file
void WxFrame::clearData () {
  t->children->clear();
	viewpoint.reset (NULL);

	//Delete items from viewpoint menu & disconnect events
	for (int i = 0; i <= viewpointCount; i++) {
		viewpointMenu->Destroy(FRAME_VIEWPOINT + i);
		Disconnect(FRAME_VIEWPOINT + i,wxEVT_MENU_HIGHLIGHT,
               wxMenuEventHandler(WxFrame::GetSelection));
		Disconnect(FRAME_VIEWPOINT + i,wxEVT_COMMAND_MENU_SELECTED,
               wxCommandEventHandler(WxFrame::ChangeViewpoint));
	}
  viewpointMenu->Destroy( FRAME_RESET_VIEWPOINT ); 
  
  // Find all separators and destroy them, if the item is not a separator
  // something is wrong but not enough to quit.
  while( viewpointMenu->GetMenuItemCount() != 0 ) {
    wxMenuItem * temp_menu_item = viewpointMenu->FindItemByPosition( 0 );
    if( temp_menu_item->IsSeparator() ) {
      viewpointMenu->Destroy( temp_menu_item->GetId() );
    } else {
      Console(4) << "Viewpoint menu might look strange from now on." << endl;
      break;
    }
  }

	//Delete items from navigation menu & disconnect events
	for (int j = 0; j <= navTypeCount; j++) {
		navigationMenu->Destroy(FRAME_NAVIGATION + j);
		Disconnect(FRAME_NAVIGATION + j,wxEVT_MENU_HIGHLIGHT,
               wxMenuEventHandler(WxFrame::GetSelection));
		Disconnect(FRAME_NAVIGATION + j,wxEVT_COMMAND_MENU_SELECTED,
               wxCommandEventHandler(WxFrame::ChangeNavigation));
	}

  // Find all separators and destroy them, if the item is not a separator
  // then just remove it.
  while( navigationMenu->GetMenuItemCount() != 0 ) {
    wxMenuItem * temp_menu_item = navigationMenu->FindItemByPosition( 0 );
    if( temp_menu_item->IsSeparator() ) {
      navigationMenu->Destroy( temp_menu_item->GetId() );
    } else {
      navigationMenu->Remove( temp_menu_item->GetId() );
    }
  }

  deviceMenu->Destroy(FRAME_DEVICECONTROL);

}


//Open a file
void WxFrame::OnOpenFileURL(wxCommandEvent & event) {
   auto_ptr< wxTextEntryDialog > text_dialog( new wxTextEntryDialog ( this,
													   "Enter the location of the file here",
													   "Open file from URL",
													   "") );
   if( text_dialog->ShowModal() == wxID_OK ) {
     string s(text_dialog->GetValue());
     clearData();
     loadFile( s );
   }
}

void WxFrame::OnOpenFile(wxCommandEvent & event)
{
	auto_ptr< wxFileDialog > openFileDialog( new wxFileDialog ( this,
													   "Open file",
													   GetCurrentPath(),
													   "",
													   FILETYPES,
													   wxOPEN,
													   wxDefaultPosition) );

	//Open an X3D file
	if (openFileDialog->ShowModal() == wxID_OK) {
	  SetCurrentFilename(openFileDialog->GetFilename());	
	  SetCurrentPath(openFileDialog->GetDirectory());
    SetStatusText(GetCurrentFilename(), 0);
    SetStatusText(openFileDialog->GetDirectory(),1);
#ifdef WIN32
    wxString wx_filename = currentPath + "\\" + currentFilename;
#else
    wxString wx_filename = currentPath + "/" + currentFilename;
#endif
    string filename(wx_filename);
    clearData();
    loadFile( filename );
    recentFiles->AddFileToHistory ( wx_filename );
  }
}

//Open a file from file history
void WxFrame::OnMRUFile(wxCommandEvent & event)
{
  wxString filename(recentFiles->GetHistoryFile(event.GetId() - wxID_FILE1));
  if (filename != "") {
    clearData();
    loadFile( filename.c_str() );
  }
}

//Close File
void WxFrame::OnCloseFile(wxCommandEvent & event) {
  //clearData();
  t->children->clear();
	SetStatusText("Open a file...", 0);
  SetStatusText("",1);

	//Disable menus again
  menuBar->EnableTop(2, false);
	menuBar->EnableTop(3, false);
	menuBar->EnableTop(4, false);

  //Disable items in rendererMenu again
  rendererMenu->Enable(FRAME_CHOOSERENDERER, false);
  rendererMenu->Enable(FRAME_RENDERMODE, false);
}

void WxFrame::OnChooseDir(wxCommandEvent & event) {
  wxDirDialog *d = new wxDirDialog (this, 
                                    "Choose a directory", 
                                    GetCurrentPath(), 
                                    0, 
                                    wxDefaultPosition);
  if (d->ShowModal() == wxID_OK)
  {
    SetCurrentPath(d->GetPath());
  }
}

//About dialog
void WxFrame::OnAbout(wxCommandEvent & event)
{ wxString t = TITLE;

  t.append( AUTHOR );
  
  wxMessageDialog aboutDialog ( this, t, ABOUT, wxOK);
  aboutDialog.ShowModal();
}

//Help event
void WxFrame::OnHelp(wxCommandEvent & event)
{

}

//Fullscreen mode
void WxFrame::OnFullscreen (wxCommandEvent & event)
{
	WxFrame::ShowFullScreen(true, wxFULLSCREEN_NOMENUBAR | 
                             wxFULLSCREEN_NOTOOLBAR | wxFULLSCREEN_NOBORDER | 
                             wxFULLSCREEN_NOCAPTION );
	glwindow->fullscreen->setValue( true );
	rendererMenu->Check(FRAME_FULLSCREEN, true);
	SetStatusText("Press F11 to exit fullscreen mode", 0);
  SetStatusText("Viewing in Fullscreen", 1);
}

//Restore from fullscreen
void WxFrame::RestoreWindow(wxCommandEvent & event)
{
	WxFrame::ShowFullScreen(false, wxFULLSCREEN_ALL);
	glwindow->fullscreen->setValue( false );
	rendererMenu->Check(FRAME_FULLSCREEN, false);
	SetStatusText(currentFilename, 0);
	SetStatusText(currentPath, 1);
}

void WxFrame::MirrorScene(wxCommandEvent & event)
{
	lastmirror = glwindow->mirrored->getValue();
	glwindow->mirrored->setValue(!lastmirror);
	if ( glwindow->mirrored->getValue() ) {
		SetStatusText("Scene mirrored in Y", 0);
	}
	else {
		SetStatusText(currentFilename, 0);
	}
}

//Render Mode
void WxFrame::RenderMode(wxCommandEvent & event)
{
  wxString renderMode;
  switch ( event.GetId() ) {
    case FRAME_MONO:
      renderMode = "MONO";
      break;
		case FRAME_QUADBUFFERED:
			renderMode = "QUAD_BUFFERED_STEREO";
			break;
		case FRAME_HORZSPLIT:
  		renderMode = "HORIZONTAL_SPLIT";
			break;
		case FRAME_VERTSPLIT:
			renderMode = "VERTICAL_SPLIT";
			break;
		case FRAME_HORZINTERLACED:
			renderMode = "HORIZONTAL_INTERLACED";
			break;
		case FRAME_VERTINTERLACED:
			renderMode = "VERTICAL_INTERLACED";
			break;
		case FRAME_SHARPDISPLAY:
			renderMode = "VERTICAL_INTERLACED_GREEN_SHIFT";
			break;
		case FRAME_REDBLUE:
			renderMode = "RED_BLUE_STEREO";
			break;
		case FRAME_REDCYAN:
			renderMode = "RED_CYAN_STEREO";
			break;
	}
	glwindow->renderMode->setValue( renderMode.c_str());
}

void WxFrame::ChangeNavigationDevice( wxCommandEvent & event ) {
  
  switch ( event.GetId() ) {
    case FRAME_MOUSE_NAV:
      if( H3DNavigation::isEnabled( H3DNavigation::MOUSE ) )
        H3DNavigation::disableDevice( H3DNavigation::MOUSE );
      else
        H3DNavigation::enableDevice( H3DNavigation::MOUSE );
      break;
    case FRAME_KEYBOARD_NAV:
      if( H3DNavigation::isEnabled( H3DNavigation::KEYBOARD ) )
        H3DNavigation::disableDevice( H3DNavigation::KEYBOARD );
      else
        H3DNavigation::enableDevice( H3DNavigation::KEYBOARD );
      break;
    case FRAME_SWS_NAV:
      if( H3DNavigation::isEnabled( H3DNavigation::SWS ) )
        H3DNavigation::disableDevice( H3DNavigation::SWS );
      else
        H3DNavigation::enableDevice( H3DNavigation::SWS );
      break;
    case FRAME_HAPTICSDEVICE_NAV:
      if( H3DNavigation::isEnabled( H3DNavigation::HAPTICSDEVICE ) )
        H3DNavigation::disableDevice( H3DNavigation::HAPTICSDEVICE );
      else
        H3DNavigation::enableDevice( H3DNavigation::HAPTICSDEVICE );
      break;
    default: {}
  }
}

//Choose Haptics Renderer
void WxFrame::ChangeRenderer(wxCommandEvent & event)
{
  switch ( event.GetId() ) {
    case FRAME_OPENHAPTICS:
      for (NodeVector::const_iterator nv = allDevices.begin(); 
            nv != allDevices.end(); nv++) {
        static_cast < H3DHapticsDevice *> 
            (*nv)->hapticsRenderer->setValue(new OpenHapticsRenderer);
       }
      setProxyRadius( original_proxy_radius );
      break;
    case FRAME_CHAI3D:
#ifdef HAVE_CHAI3D
        for (NodeVector::const_iterator nv = allDevices.begin(); 
              nv != allDevices.end(); nv++) {
          static_cast < H3DHapticsDevice *> 
            (*nv)->hapticsRenderer->setValue(new Chai3DRenderer);
        }
        setProxyRadius( original_proxy_radius );
#endif
      break;
    case FRAME_GODOBJECT:
      for (NodeVector::const_iterator nv = allDevices.begin(); 
             nv != allDevices.end(); nv++) {
          static_cast < H3DHapticsDevice *> (*nv)->
              hapticsRenderer->setValue(new GodObjectRenderer);
        }
      setProxyRadius( original_proxy_radius );
	    break;
    case FRAME_RUSPINI:
        for (NodeVector::const_iterator nv = allDevices.begin(); 
                 nv != allDevices.end(); nv++) {
            static_cast < H3DHapticsDevice *> (*nv)->
              hapticsRenderer->setValue(new RuspiniRenderer);
        }
        setProxyRadius( current_proxy_radius );
		  break;
  }
}

//Toggle haptics
void WxFrame::ToggleHaptics (wxCommandEvent & event) {
  for (NodeVector::const_iterator nv = allDevices.begin(); 
        nv != allDevices.end(); nv++) {
    if (lastDeviceStatus) {
      static_cast < H3DHapticsDevice *> (*nv)->disableDevice();
      lastDeviceStatus = false;
    }
    else {
      static_cast < H3DHapticsDevice *> (*nv)->enableDevice();
      lastDeviceStatus = true;
    }
  }
}

//Show console event
void WxFrame::ShowConsole(wxCommandEvent & event)
{
  theConsole->Show();
}

void WxFrame::ShowFrameRate(wxCommandEvent & event)
{
  //frameRates = new FrameRateDialog( this );
  //frameRates->updateMenuItems();

  // set labels to make sure they are the right size when shown
  // (otherwise some numbers may not be displayed)
  frameRates->graphics_rate->SetLabel( "100" );
  frameRates->haptics_rate->SetLabel( "1000" );
  frameRates->haptics_time->SetLabel( "100" );
  frameRates->Show();
}

//Change Viewpoint
void WxFrame::ChangeViewpoint (wxCommandEvent & event)
{	
	//Default viewpoint
	if (viewpointCount <= 1) {
		//do nothing
	}
	else {
		//Find viewpoint index
		int index = selection - FRAME_VIEWPOINT;
		//Get to that index in the viewpoint list
		X3DViewpointNode::ViewpointList::iterator vp = VPlist.begin();
		int i = 0;
		for (i = 0; i < index; i++) {
			vp++;
		}
		//Enable that viewpoint
		(*vp)->set_bind->setValue(true);
		defaultvp = *vp;
		Console (3) << "Viewpoint Set" << endl;
    viewpointMenu->Check(selection, true);
	}
}

//Reset Active Viewpoint
void WxFrame::ResetViewpoint(wxCommandEvent & event)
{
  if (defaultvp) {
      defaultvp->rel_orn = Rotation();
      defaultvp->rel_pos = Vec3f();
  }
}

//Change Navigation
void WxFrame::ChangeNavigation (wxCommandEvent & event)
{
  if (mynav) {
    mynav->setNavType ((navigationMenu->GetLabel(selection)).c_str());
  }
  else {
    glwindow->default_nav = (navigationMenu->GetLabel(selection)).c_str();
  }
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
  SaveMRU();
  SaveSettings();
  delete wxConfigBase::Set((wxConfigBase *) NULL);
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

////Save file history for next initialization
void WxFrame::SaveMRU () {
  h3dConfig = wxConfigBase::Get();

  //Store number of files in history
  h3dConfig->SetPath(_T("/Recent/Count"));
  h3dConfig->Write(_T("Count"), (long) recentFiles->GetCount());

  //Store each individual file info
  h3dConfig->SetPath(_T("/Recent/List"));
  for (int i = 0; i < (int)recentFiles->GetCount(); i++) {
    wxString entry = wxString ("") << i;
    h3dConfig->Write(entry, recentFiles->GetHistoryFile(i));
  }
}

///Save settings for next initialization
void WxFrame::SaveSettings () {
  h3dConfig = wxConfigBase::Get();

  //Save debug options
  DebugOptions *dgo = NULL;
  global_settings->getOptionNode( dgo );
  if (dgo) {
    h3dConfig->SetPath(_T("/Settings/Debug"));
    h3dConfig->Write(_T("draw_bound"), dgo->drawBound->getValue());
    h3dConfig->Write(_T("draw_tree"), dgo->drawBoundTree->getValue());
    //h3dConfig->Write(_T("tree_depth"), tree_depth);
    h3dConfig->Write(_T("draw_triangles"), 
                     dgo->drawHapticTriangles->getValue());
  }

  //Save graphics caching options
  GraphicsCachingOptions *gco = NULL;
  global_settings->getOptionNode (gco);
  if (gco) {
    h3dConfig->SetPath(_T("/Settings/GraphicsCaching"));
    h3dConfig->Write(_T("useCaching"), (bool) gco->useCaching->getValue());
    h3dConfig->Write(_T("cacheOnlyGeometries"),
                     (bool) gco->cacheOnlyGeometries->getValue());
    h3dConfig->Write(_T("cachingDelay"), (int) gco->cachingDelay->getValue());
  }

  //Save haptics options
  HapticsOptions *ho = NULL;
  global_settings->getOptionNode (ho);
  if (ho) {
    h3dConfig->SetPath(_T("/Settings/Haptics"));
    h3dConfig->Write(_T("touchableFace"), wxString( ho->touchableFace->getValue().c_str() ));
    h3dConfig->Write(_T("maxDistance"), ho->maxDistance->getValue());
    h3dConfig->Write(_T("lookAheadFactor"), ho->lookAheadFactor->getValue());
    h3dConfig->Write(_T("useBoundTree"), (bool) ho->useBoundTree->getValue());
  }

  //Save openhaptics options
  OpenHapticsOptions *oho = NULL;
  global_settings->getOptionNode (oho);
  if (oho) {
    h3dConfig->SetPath(_T("/Settings/OpenHaptics"));
    h3dConfig->Write(_T("useAdaptiveViewport"),
                     oho->useAdaptiveViewport->getValue());
    h3dConfig->Write(_T("useHapticCameraView"),
                     oho->useHapticCameraView->getValue());
    h3dConfig->Write(_T("GLShape"), wxString(oho->GLShape->getValue().c_str() ) );
    h3dConfig->Write(_T("forceFullGeometryRender"),
                     oho->forceFullGeometryRender->getValue());
  }

  //Save geometry bound tree options
  GeometryBoundTreeOptions *gbto = NULL;
  global_settings->getOptionNode (gbto);
  if (gbto) {
    h3dConfig->SetPath(_T("/Settings/GeometryBoundTree"));
    h3dConfig->Write(_T("boundType"), wxString(gbto->boundType->getValue().c_str()));
    h3dConfig->Write(_T("maxTrianglesInLeaf"),
                     gbto->maxTrianglesInLeaf->getValue());
  }
}

void WxFrame::LoadMRU () {
  h3dConfig = wxConfigBase::Get();

  //Load file history from previous sessions
  h3dConfig->SetPath(_T("/Recent/Count"));
  int count = h3dConfig->Read(_T("Count"), 9);
  h3dConfig->SetPath(_T("/Recent/List"));
  for (int i = 0; i < count; i++) {
    wxString entry = wxString("") << i;
    if (h3dConfig->Exists(entry)) {
      recentFiles->AddFileToHistory (h3dConfig->Read(entry));
    }
  }
}

void WxFrame::LoadSettings () {
  h3dConfig = wxConfigBase::Get();

  //Load settings from previous sessions
  //Debug options
  if (h3dConfig->Exists("/Settings/Debug")) {
    h3dConfig->SetPath(_T("/Settings/Debug"));
    DebugOptions *dgo = NULL;
    global_settings->getOptionNode(dgo);
    if (dgo) {
      bool draw_bound;
      bool draw_triangles;
      int draw_tree;
      h3dConfig->Read("draw_bound", &draw_bound);
      h3dConfig->Read("draw_triangles", &draw_triangles);
      h3dConfig->Read("draw_tree", &draw_tree);
      dgo->drawBound->setValue(draw_bound);
      dgo->drawHapticTriangles->setValue(draw_triangles);
      dgo->drawBoundTree->setValue(draw_tree);
    }
  }

  //Graphics caching options
  if (h3dConfig->Exists("/Settings/GraphicsCaching")) {
    h3dConfig->SetPath(_T("/Settings/GraphicsCaching"));
    GraphicsCachingOptions *gco = NULL;
    global_settings->getOptionNode(gco);
    if (gco) {
      bool useCaching;
      bool cacheOnlyGeometries;
      int cachingDelay;
      h3dConfig->Read("useCaching", &useCaching);
      h3dConfig->Read("cacheOnlyGeometries", &cacheOnlyGeometries);
      h3dConfig->Read("cachingDelay", &cachingDelay);
      gco->useCaching->setValue(useCaching);
      gco->cacheOnlyGeometries->setValue(cacheOnlyGeometries);
      gco->cachingDelay->setValue(cachingDelay);
    }
  }

  //Haptics options
  if (h3dConfig->Exists("/Settings/Haptics")) {
    h3dConfig->SetPath(_T("/Settings/Haptics"));
    HapticsOptions *ho = NULL;
    global_settings->getOptionNode(ho);
    if (ho) {
      bool useBoundTree;
      wxString touchableFace;
      h3dConfig->Read("useBoundTree", &useBoundTree);
      h3dConfig->Read("touchableFace", &touchableFace);
      ho->touchableFace->setValue(touchableFace.c_str());
      ho->maxDistance->setValueFromString(
                       h3dConfig->Read("maxDistance").c_str());
      ho->lookAheadFactor->setValueFromString(
                           h3dConfig->Read("lookAheadFactor").c_str());
      ho->useBoundTree->setValue(useBoundTree);
    }
  }

  //OpenHaptics options
/*  if (h3dConfig->Exists("/Settings/OpenHaptics")) {
    h3dConfig->SetPath(_T("/Settings/OpenHaptics"));
    OpenHapticsOptions *oho = NULL;
    global_settings->getOptionNode(oho);
    if (oho) {
      oho->useAdaptiveViewport->setValue(h3dConfig->
        Read("useAdaptiveViewport"));
      oho->useHapticCameraView->setValue(h3dConfig->
        Read("useHapticCameraView"));
      oho->GLShape->setValue((h3dConfig->Read("GLShape")).c_str());
      oho->forceFullGeometryRender->setValue(h3dConfig->
        Read("forceFullGeometryRender"));
    }
  } */

  //Geometry bound tree options
  if (h3dConfig->Exists("/Settings/GeometryBoundTree")) {
    h3dConfig->SetPath(_T("/Settings/GeometryBoundTree"));
    GeometryBoundTreeOptions *gbto = NULL;
    global_settings->getOptionNode( gbto );
    if (gbto) {
      wxString boundType;
      int maxTrianglesinLeaf;
      h3dConfig->Read("boundType", &boundType);
      h3dConfig->Read("maxTrianglesinLeaf", &maxTrianglesinLeaf);
      gbto->boundType->setValue(boundType.c_str());
      gbto->maxTrianglesInLeaf->setValue(maxTrianglesinLeaf);
    }
  }
}

//Build Navigation Menu
void WxFrame::buildNavMenu () {
	//Get active navigation info object
	if (NavigationInfo::getActive()) {
		mynav = NavigationInfo::getActive();
		//Store allowed navigation types and count
		vector<string> navTypes = mynav->type->getValue();

		if (mynav->getUsedNavType() == "NONE") {
			navigationMenu->Append(FRAME_NAVIGATION, "Unavailable",
                             "Navigation Disabled");
			navigationMenu->Enable(FRAME_NAVIGATION, false);
		}
		else {
			vector<string> allowedTypes;
			vector<string>::iterator navList = navTypes.begin();
      bool hasAny = false;
			for (vector<string>::iterator navList = navTypes.begin(); 
           navList != navTypes.end(); navList++) {
				if (validateNavType(*navList) && (*navList != "NONE")) {
					if (allowedTypes.empty()) {
            if ((*navList != "ANY")) {
              allowedTypes.push_back(*navList);
            }
            else {
              allowedTypes.push_back("EXAMINE");
              allowedTypes.push_back("FLY");
              allowedTypes.push_back("WALK");
              allowedTypes.push_back("LOOKAT");
              break;
            }
          }
					else {
						bool found = false;
						for (vector<string>::iterator allowedList = allowedTypes.begin(); 
                 allowedList != allowedTypes.end(); allowedList++) {
              if ((*navList == "ANY")) {
                hasAny = true;
                found = true;
              }
              else if ((*allowedList) == (*navList)) {
								found = true;
								break;
							}
						}
						if (!found) {
							allowedTypes.push_back(*navList);
						}
					}
				}
      }
      if (hasAny) {
        vector<string> allTypes;
        allTypes.push_back("EXAMINE");
        allTypes.push_back("FLY");
        allTypes.push_back("WALK");
        allTypes.push_back("LOOKAT");
   			vector<string>::iterator allList = allTypes.begin();
  			for (vector<string>::iterator allList = allTypes.begin();
             allList != allTypes.end(); allList++) {
          bool found = false;
          for (vector<string>::iterator allowedList = allowedTypes.begin();
               allowedList != allowedTypes.end(); allowedList++) {
            if ((*allowedList) == (*allList)) {
              found = true;
              break;
            }
          }
          if (!found) {
            allowedTypes.push_back(*allList);
          }
        }
      }
			navTypeCount = allowedTypes.size();
			int j = 0;
			for (vector<string>::iterator menuList = allowedTypes.begin(); 
           menuList != allowedTypes.end(); menuList++) {
				string typeName = (*menuList);
				navigationMenu->AppendRadioItem(FRAME_NAVIGATION + j, typeName.c_str(),
                                        "Select a navigation mode");
				Connect(FRAME_NAVIGATION + j,wxEVT_MENU_HIGHLIGHT,
                wxMenuEventHandler(WxFrame::GetSelection));
				Connect(FRAME_NAVIGATION + j,wxEVT_COMMAND_MENU_SELECTED,
                wxCommandEventHandler(WxFrame::ChangeNavigation));
				j++;
      }

    int index = 0;
 		  for (vector<string>::iterator menuList = allowedTypes.begin();
           menuList != allowedTypes.end(); menuList++) {
        if (mynav->getUsedNavType() == (*menuList)) {
          navigationMenu->Check(FRAME_NAVIGATION+index, true);
          break;
        }
        index++;
      }
    }
  }
  else {
    mynav = 0;
    vector<string> allTypes;
    allTypes.push_back("EXAMINE");
    allTypes.push_back("FLY");
    allTypes.push_back("WALK");
    allTypes.push_back("LOOKAT");
    allTypes.push_back("NONE");
    navTypeCount = 5;
    int j = 0;
    for (vector<string>::iterator allList = allTypes.begin();
         allList != allTypes.end(); allList++) {
      navigationMenu->AppendRadioItem(FRAME_NAVIGATION + j, (*allList).c_str(),
                                      "Select a navigation mode");
		  Connect(FRAME_NAVIGATION + j,wxEVT_MENU_HIGHLIGHT,
              wxMenuEventHandler(WxFrame::GetSelection));
			Connect(FRAME_NAVIGATION + j,wxEVT_COMMAND_MENU_SELECTED, 
              wxCommandEventHandler(WxFrame::ChangeNavigation));
			j++;
    }
    //mynav->setNavType("EXAMINE");
    glwindow->default_nav = "EXAMINE";
  }

  navigationMenu->AppendSeparator();
  if( !navigationDevices ) {
    navigationDevices = new wxMenu;
    navigationDevices->AppendCheckItem(FRAME_MOUSE_NAV, "Mouse", 
      "Mouse can be used to navigate scene");
    navigationDevices->Check( FRAME_MOUSE_NAV, true );
    navigationDevices->AppendCheckItem(FRAME_KEYBOARD_NAV, "Keyboard",
                              "Keyboard can be used to navigate scene");
    navigationDevices->Check( FRAME_KEYBOARD_NAV, true );
    navigationDevices->AppendCheckItem(FRAME_SWS_NAV, "SpaceWareSensor", 
      "SpaceWareSensor can be used to navigate scene.");
    navigationDevices->AppendCheckItem( FRAME_HAPTICSDEVICE_NAV,
                                        "Haptics Device",
                          "A haptics device can be used to navigate scene");
  }
  navigationMenu->AppendSubMenu( navigationDevices, "Navigation Devices",
                  "Toggle on an off which devices to use for navigation" );
}


//Validate NavType
//Checks whether a navigation type is valid
bool WxFrame::validateNavType (string a) {
	if ( a == "ANY" || a == "EXAMINE" || a == "FLY" || a == "WALK" ||
       a == "LOOKAT" || a == "NONE" ) {
		return true;
	}
	return false;
}


void WxFrame::OnSettings (wxCommandEvent & event) {
  //settings = new SettingsDialog(this, global_settings.get() );
  settings->Show();
}


IMPLEMENT_CLASS(FrameRateDialog, wxDialog)

BEGIN_EVENT_TABLE(FrameRateDialog, wxDialog)
END_EVENT_TABLE()

FrameRateDialog::FrameRateDialog(wxWindow* win ) :
  wxDialog (win, wxID_ANY, "Frame rates", wxDefaultPosition, wxDefaultSize,
            wxDEFAULT_DIALOG_STYLE) {
  graphics_rate = NULL;
  haptics_rate = NULL;

  topsizer = new wxBoxSizer( wxVERTICAL );
  updateMenuItems();

	SetSizer( topsizer );      // use the sizer for layout

	topsizer->SetSizeHints( this );   // set size hints to honour minimum size
}

void FrameRateDialog::updateMenuItems() {
  unsigned int nr_devices = 0;
  DeviceInfo *di = DeviceInfo::getActive();
  if( di ) {
    nr_devices = di->device->size();
  }

  topsizer = new wxBoxSizer( wxVERTICAL );

  wxBoxSizer* framerate_sizer = new wxBoxSizer( wxHORIZONTAL );
  framerate_sizer->Add( new wxStaticText( this, wxID_ANY, _("&Graphics:")), 0,
                        wxALL|wxALIGN_CENTER_VERTICAL, 5);
  graphics_rate = new wxStaticText( this, wxID_ANY, _("&1000"), 
                                    wxDefaultPosition, 
                                    wxDefaultSize,wxALIGN_RIGHT|wxST_NO_AUTORESIZE );
  framerate_sizer->Add( graphics_rate, 0,
                        wxALL|wxALIGN_RIGHT, 5);
  
  framerate_sizer->Add( new wxStaticText( this, wxID_ANY, _("&Haptics:")), 0,
                        wxALL|wxALIGN_CENTER_VERTICAL, 5);
  haptics_rate = new wxStaticText( this, wxID_ANY, _("&1000"), 
                                   wxDefaultPosition, 
                                   wxDefaultSize,wxALIGN_RIGHT|wxST_NO_AUTORESIZE );
  framerate_sizer->Add( haptics_rate, 0,
                        wxALL|wxALIGN_RIGHT, 5);

  framerate_sizer->Add( new wxStaticText( this, wxID_ANY, _("&Haptics time(10-5 s):")), 0,
                        wxALL|wxALIGN_CENTER_VERTICAL, 5);
  haptics_time = new wxStaticText( this, wxID_ANY, _("&100"), 
                                   wxDefaultPosition, 
                                   wxDefaultSize,wxALIGN_RIGHT|wxST_NO_AUTORESIZE );
  framerate_sizer->Add( haptics_time, 0,
                        wxALL|wxALIGN_RIGHT, 5);

  topsizer->Add(framerate_sizer, 0,
                wxALL|wxALIGN_CENTER_VERTICAL, 5 );    

}

void FrameRateDialog::updateFrameRates() {
  unsigned int nr_devices = 0;
  DeviceInfo *di = DeviceInfo::getActive();
  if( di ) {
    nr_devices = di->device->size();
  }

  //  if( nr_devices != haptics_rate.size() ) {
  //    updateMenuItems();
  //  }

  /*  wxBoxSizer * topsizer = new wxBoxSizer( wxVERTICAL );
  wxBoxSizer* framerate_sizer = new wxBoxSizer( wxHORIZONTAL );
  framerate_sizer->Add( new wxStaticText( this, wxID_ANY, _("&Graphics:")), 0,
                        wxALL|wxALIGN_CENTER_VERTICAL, 5);
  framerate_sizer->Add( new wxStaticText( this, wxID_ANY, _("&64")), 0,
                        wxALL|wxALIGN_CENTER_VERTICAL, 5);

	topsizer->Add(framerate_sizer, 0,
                wxALL|wxALIGN_CENTER_VERTICAL, 5 );

                SetSizer( topsizer ); */
  Scene *scene = *Scene::scenes.begin();
  
  stringstream s;
  s  <<  (int)scene->frameRate->getValue();

  graphics_rate->SetLabel( s.str() );
  if( di && di->device->size() > 0 ) {
    H3DHapticsDevice *hd = di->device->getValueByIndex( 0 );
    stringstream hs;
    hs  <<  (int)hd->hapticsRate->getValue();
    haptics_rate->SetLabel( hs.str() );

    stringstream hts;
    hts << (int)(hd->hapticsLoopTime->getValue() * 1e5 );
    haptics_time->SetLabel( hts.str() );
  }
}

// ----------------------------------------------------------------------------
// SettingsDialog
// ----------------------------------------------------------------------------

IMPLEMENT_CLASS(SettingsDialog, wxPropertySheetDialog)

BEGIN_EVENT_TABLE(SettingsDialog, wxPropertySheetDialog)
	EVT_CHECKBOX (ID_DRAW_BOUNDS, SettingsDialog::handleSettingsChange)
	EVT_CHECKBOX (ID_DRAW_BOUND_TREE, SettingsDialog::handleSettingsChange)
	EVT_SPINCTRL (ID_DRAW_TREE_DEPTH, SettingsDialog::handleSpinEvent)
  EVT_CHECKBOX( ID_DRAW_TRIANGLES, SettingsDialog::handleSettingsChange)

  EVT_CHECKBOX (ID_USE_DISPLAY_LISTS, SettingsDialog::handleSettingsChange)
  EVT_CHECKBOX (ID_CACHE_ONLY_GEOMS, SettingsDialog::handleSettingsChange)
  EVT_SPINCTRL (ID_CACHING_DELAY, SettingsDialog::handleSpinEvent )

  EVT_CHOICE(ID_TOUCHABLE_FACE, SettingsDialog::handleSettingsChange)
  EVT_TEXT(ID_MAX_DISTANCE, SettingsDialog::handleSettingsChange )
  EVT_TEXT(ID_LOOK_AHEAD_FACTOR, SettingsDialog::handleSettingsChange )
  EVT_CHECKBOX( ID_USE_BOUND_TREE, SettingsDialog::handleSettingsChange)

  EVT_CHOICE( ID_OH_SHAPE_TYPE, SettingsDialog::handleSettingsChange)
  EVT_CHECKBOX( ID_ADAPTIVE_VIEWPORT, SettingsDialog::handleSettingsChange)
  EVT_CHECKBOX( ID_HAPTIC_CAMERA, SettingsDialog::handleSettingsChange)
  EVT_CHECKBOX( ID_FULL_GEOMETRY_RENDER, SettingsDialog::handleSettingsChange )
  
  EVT_TEXT( ID_PROXY_RADIUS, SettingsDialog::handleSettingsChange )

  EVT_SPINCTRL (ID_MAX_TRIANGLES, SettingsDialog::handleSpinEvent)
  EVT_CHOICE( ID_BOUND_TYPE, SettingsDialog::handleSettingsChange )

  EVT_BUTTON (wxID_OK, SettingsDialog::OnOk)
  EVT_BUTTON (wxID_CANCEL, SettingsDialog::OnCancel)

END_EVENT_TABLE()

  SettingsDialog::SettingsDialog(wxWindow* win, GlobalSettings *gs, WxFrame *w ):
    wx_frame( w )
{


    SetExtraStyle(wxDIALOG_EX_CONTEXTHELP|wxWS_EX_VALIDATE_RECURSIVELY);

    int tabImage1 = -1;
    int tabImage2 = -1;
    int tabImage3 = -1;

    int resizeBorder = wxRESIZE_BORDER;

    m_imageList = NULL;

    Create(win, wxID_ANY, _("Settings"), wxDefaultPosition, wxDefaultSize,
        wxDEFAULT_DIALOG_STYLE|
        (int)wxPlatform::IfNot(wxOS_WINDOWS_CE, resizeBorder)
    );

    // If using a toolbook, also follow Mac style and don't create buttons
    CreateButtons(wxOK | wxCANCEL |
                        (int)wxPlatform::IfNot(wxOS_WINDOWS_CE, wxHELP)
    ); 

    wxBookCtrlBase* notebook = GetBookCtrl();
    notebook->SetImageList(m_imageList);

    wxPanel* generalSettings = CreateGeneralSettingsPage(notebook, gs );
    wxPanel* openhaptics_settings = CreateOpenHapticsSettingsPage(notebook,gs);
    wxPanel* ruspini_settings = CreateRuspiniSettingsPage(notebook,gs);
    wxPanel* debug_settings = CreateDebugSettingsPage(notebook, gs);

    notebook->AddPage(generalSettings, _("General"), true, tabImage1);
    notebook->AddPage(openhaptics_settings, _("OpenHaptics"), false,tabImage2);
    notebook->AddPage(ruspini_settings, _("Ruspini"), false,tabImage2);
    notebook->AddPage(debug_settings, _("Debug"), false, tabImage3);

    LayoutDialog();
}

SettingsDialog::~SettingsDialog()
{
    
}

wxPanel* SettingsDialog::CreateDebugSettingsPage(wxWindow* parent,
                                                 GlobalSettings *gs ) {
    wxPanel* panel = new wxPanel(parent, wxID_ANY);

    wxBoxSizer *topSizer = new wxBoxSizer( wxVERTICAL );
    wxBoxSizer *item0 = new wxBoxSizer( wxVERTICAL );

    DebugOptions *debug = NULL;

    bool draw_bound = false;
    bool draw_tree = false;
    int draw_tree_depth = 1;
    bool draw_triangles = false;
      

  if( gs ) {
    gs->getOptionNode( debug );
    if( debug ) {
      draw_bound = debug->drawBound->getValue();
      int d = debug->drawBoundTree->getValue();
      if( d != -1 ) {
        draw_tree_depth = d;
        treeDepth = d;
      }
      draw_triangles = debug->drawHapticTriangles->getValue();
      if( draw_tree_depth >= 1 ) {
        draw_tree = true;
      }
    }
  }

    //// adaptive viewpoirt

    wxBoxSizer* draw_bound_sizer = new wxBoxSizer( wxHORIZONTAL );
    wxCheckBox* draw_bound_box = new wxCheckBox(panel, ID_DRAW_BOUNDS,
                                            _("&Draw geometry bounding boxes"),
                                             wxDefaultPosition, wxDefaultSize);
    draw_bound_sizer->Add(draw_bound_box, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
    draw_bound_box->SetValue( draw_bound );
    item0->Add( draw_bound_sizer, 0, wxGROW|wxALL, 0);

    wxBoxSizer* draw_triangles_sizer = new wxBoxSizer( wxHORIZONTAL );
    wxCheckBox* draw_triangles_box = new wxCheckBox(panel, ID_DRAW_TRIANGLES, 
                                                   _("&Draw haptic triangles"),
                                                   wxDefaultPosition,
                                                   wxDefaultSize);
    draw_triangles_box->SetValue( draw_triangles );
    draw_triangles_sizer->Add(draw_triangles_box, 0, 
                              wxALL|wxALIGN_CENTER_VERTICAL, 5);
    item0->Add( draw_triangles_sizer, 0, wxGROW|wxALL, 0);

    wxBoxSizer* draw_tree_sizer = new wxBoxSizer( wxHORIZONTAL );
    wxCheckBox* draw_tree_box = new wxCheckBox(panel, ID_DRAW_BOUND_TREE,
                                               _("&Draw bound tree"),
                                               wxDefaultPosition, 
                                               wxDefaultSize);
    draw_tree_box->SetValue( draw_tree );
    draw_tree_sizer->Add(draw_tree_box, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
    item0->Add( draw_tree_sizer, 0, wxGROW|wxALL, 0);

    wxBoxSizer* depth_sizer = new wxBoxSizer( wxHORIZONTAL );
    depth_sizer->Add(new wxStaticText(panel, wxID_ANY, _("&Depth:")), 0,
                     wxALL|wxALIGN_CENTER_VERTICAL, 5);
    
    wxSpinCtrl* depth_spin = new wxSpinCtrl(panel, ID_DRAW_TREE_DEPTH,
                                            wxEmptyString, wxDefaultPosition,
                                            wxSize(80, wxDefaultCoord));
    depth_spin->SetValue( draw_tree_depth );
    depth_sizer->Add(depth_spin, 0, 
                     wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL,
                     5);
    item0->Add( depth_sizer );

    //Store
  
    topSizer->Add( item0, 1, wxGROW|wxALIGN_CENTRE|wxALL, 5 );

    panel->SetSizer(topSizer);
    topSizer->Fit(panel);

    return panel;
}


wxPanel* SettingsDialog::CreateOpenHapticsSettingsPage(wxWindow* parent,
                                                       GlobalSettings *gs) {
    wxPanel* panel = new wxPanel(parent, wxID_ANY);

    wxBoxSizer *topSizer = new wxBoxSizer( wxVERTICAL );
    wxBoxSizer *item0 = new wxBoxSizer( wxVERTICAL );

    OpenHapticsOptions *oho = NULL;

    bool use_adaptive_viewport = true;
    bool use_haptic_camera_view = true;
    int shape_type = 0;
    bool force_full_geometry_render = false;
      

  if( gs ) {
    gs->getOptionNode( oho );
    if( oho ) {
      use_adaptive_viewport = oho->useAdaptiveViewport->getValue();
      use_haptic_camera_view = oho->useHapticCameraView->getValue();
      force_full_geometry_render = oho->forceFullGeometryRender->getValue();

      const string &shape = oho->GLShape->getValue();
      if( shape == "FEEDBACK_BUFFER" ) shape_type = 0;
      else if( shape == "DEPTH_BUFFER" ) shape_type = 1;
      else if( shape == "CUSTOM" ) shape_type = 2;
    }
  }


    //// adaptive viewpoirt

    wxBoxSizer* adaptive_viewport_sizer = new wxBoxSizer( wxHORIZONTAL );
    wxCheckBox* adaptive_viewport = new wxCheckBox(panel, ID_ADAPTIVE_VIEWPORT,
                                                   _("&Use adaptive viewport"),
                                                   wxDefaultPosition, 
                                                   wxDefaultSize);
    adaptive_viewport->SetValue( use_adaptive_viewport );
    adaptive_viewport_sizer->Add(adaptive_viewport, 0,
                                 wxALL|wxALIGN_CENTER_VERTICAL, 5);
    item0->Add( adaptive_viewport_sizer, 0, wxGROW|wxALL, 0);

    wxBoxSizer* haptic_camera_sizer = new wxBoxSizer( wxHORIZONTAL );
    wxCheckBox* haptic_camera = new wxCheckBox(panel, ID_HAPTIC_CAMERA,
                                               _("&Use haptic camera view"), 
                                               wxDefaultPosition,
                                               wxDefaultSize);
    haptic_camera->SetValue( use_haptic_camera_view );
    haptic_camera_sizer->Add(haptic_camera, 0,
                             wxALL|wxALIGN_CENTER_VERTICAL, 5);
    item0->Add( haptic_camera_sizer, 0, wxGROW|wxALL, 0);

    wxBoxSizer* full_geom_render_sizer = new wxBoxSizer( wxHORIZONTAL );
    wxCheckBox* full_geom_render = new wxCheckBox(panel, 
                                             ID_FULL_GEOMETRY_RENDER,
                                             _("&Force full geometry render"),
                                             wxDefaultPosition, wxDefaultSize);
    full_geom_render->SetValue( force_full_geometry_render );
    full_geom_render_sizer->Add(full_geom_render, 0,
                                wxALL|wxALIGN_CENTER_VERTICAL, 5);
    item0->Add( full_geom_render_sizer, 0, wxGROW|wxALL, 0);

    wxArrayString shape_choices;
    shape_choices.Add(wxT("Feedback buffer"));
    shape_choices.Add(wxT("Depth buffer"));
    shape_choices.Add(wxT("Custom"));

    wxBoxSizer* shape_sizer = new wxBoxSizer( wxHORIZONTAL );
 
    shape_sizer->Add(new wxStaticText(panel, wxID_ANY,
                                      _("&Default shape type:")), 0,
                                      wxALL|wxALIGN_CENTER_VERTICAL, 5);
    wxChoice* shape_choice = new wxChoice(panel, ID_OH_SHAPE_TYPE,
                                          wxDefaultPosition, wxDefaultSize,
                                          shape_choices );
    shape_choice->SetSelection( shape_type );
    shape_sizer->Add(shape_choice, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
    item0->Add(shape_sizer, 0, wxGROW|wxALL, 5);

    topSizer->Add( item0, 1, wxGROW|wxALIGN_CENTRE|wxALL, 5 );

    panel->SetSizer(topSizer);
    topSizer->Fit(panel);

    return panel;
}

wxPanel* SettingsDialog::CreateRuspiniSettingsPage( wxWindow* parent,
                                                    GlobalSettings *gs) {
    wxPanel* panel = new wxPanel(parent, wxID_ANY);

    wxBoxSizer *topSizer = new wxBoxSizer( wxHORIZONTAL );
 
    topSizer->Add(new wxStaticText(panel, wxID_ANY,
                                    _("&Proxy radius(m):")), 
                   0,
                   wxALL, 5);

    wxTextCtrl* proxy_radius_text = new wxTextCtrl(panel, 
                                                   ID_PROXY_RADIUS,
                                                   wxEmptyString,
                                                   wxDefaultPosition,
                                                   wxSize(40, wxDefaultCoord));
    //    stringstream ss;
    //    ss << wx_frame->current_proxy_radius;
    proxy_radius_text->SetValue( "0.0025" ); //ss.str().c_str() );
    topSizer->Add(proxy_radius_text, 0,
                            wxALL, 5);

    panel->SetSizer(topSizer);
    topSizer->Fit(panel);

    return panel;
}

wxPanel* SettingsDialog::CreateGeneralSettingsPage(wxWindow* parent, 
                                                   GlobalSettings *gs )
{
  bool use_display_lists = true;
  bool cache_only_geometries = false;
  int caching_delay = 5;

  GraphicsCachingOptions *gco = NULL;

  if( gs ) {
    gs->getOptionNode( gco );
    if( gco ) {
      use_display_lists = gco->useCaching->getValue();
      cache_only_geometries = gco->cacheOnlyGeometries->getValue();
      caching_delay = gco->cachingDelay->getValue();
    }
  }

  wxPanel* panel = new wxPanel(parent, wxID_ANY);

    wxBoxSizer *topSizer = new wxBoxSizer( wxVERTICAL );
    wxBoxSizer *item0 = new wxBoxSizer( wxVERTICAL );

    //// Graphics caching
    wxStaticBox* graphics_caching_box = new wxStaticBox(panel, wxID_ANY,
                                                       _("Graphics caching:"));

    wxBoxSizer* graphics_caching_sizer = new wxStaticBoxSizer( 
                                            graphics_caching_box, 
                                            wxVERTICAL 
                                            );
    item0->Add(graphics_caching_sizer, 0, wxGROW|wxALL, 5);

    wxBoxSizer* display_list_sizer = new wxBoxSizer( wxHORIZONTAL );
    wxCheckBox* display_list_checkbox = new wxCheckBox(panel,
                                          ID_USE_DISPLAY_LISTS,
                                          _("&Use display lists"),
                                          wxDefaultPosition, wxDefaultSize);
    display_list_checkbox->SetValue( use_display_lists );
    display_list_sizer->Add(display_list_checkbox, 0,
                            wxALL|wxALIGN_CENTER_VERTICAL, 5);
    graphics_caching_sizer->Add(display_list_sizer, 0, wxGROW|wxALL, 0);
    
    wxCheckBox* only_geoms_checkbox = new wxCheckBox(panel,
                                                  ID_CACHE_ONLY_GEOMS,
                                                  _("&Cache only geometries"),
                                                  wxDefaultPosition,
                                                  wxDefaultSize);
    only_geoms_checkbox->SetValue( cache_only_geometries );
    display_list_sizer->Add(only_geoms_checkbox, 0,
                            wxALL|wxALIGN_CENTER_VERTICAL, 5);
  //  graphics_caching_sizer->Add(only_geoms_sizer, 0, wxGROW|wxALL, 0);

    wxBoxSizer* caching_delay_sizer = new wxBoxSizer( wxHORIZONTAL );
    caching_delay_sizer->Add(new wxStaticText(panel, wxID_ANY,
                             _("&Caching delay:")), 0,
                             wxALL|wxALIGN_CENTER_VERTICAL, 5);
    
    wxSpinCtrl* caching_delay_spin = new wxSpinCtrl(panel, ID_CACHING_DELAY,
                                                    wxEmptyString,
                                                    wxDefaultPosition,
                                                    wxSize(80,wxDefaultCoord));
    caching_delay_spin->SetValue( caching_delay );
    caching_delay_sizer->Add(caching_delay_spin, 0,
                             wxALL|wxALIGN_CENTER_HORIZONTAL|
                             wxALIGN_CENTER_VERTICAL, 5);
    graphics_caching_sizer->Add( caching_delay_sizer );


      int bound_type = 1;
      int max_triangles_in_leaf = 1;
      
      GeometryBoundTreeOptions *gbto = NULL;

      if( gs ) {
        gs->getOptionNode( gbto );
        if( gbto ) {
          max_triangles_in_leaf = gbto->maxTrianglesInLeaf->getValue();
          const string &type =  gbto->boundType->getValue();
          if( type == "OBB" ) bound_type = 0;
          else if( type == "AABB" ) bound_type = 1;
          else if( type == "SPHERE" ) bound_type = 2;
        }
      }

    // Geometry bound
    wxStaticBox* bound_tree_box = new wxStaticBox(panel, wxID_ANY,
                                                  _("Bound tree:"));

    wxArrayString bound_choices;
    bound_choices.Add(wxT("Oriented bounding box"));
    bound_choices.Add(wxT("Axis-aligned bounding box"));
    bound_choices.Add(wxT("Sphere"));

    wxBoxSizer* bound_tree_sizer = new wxStaticBoxSizer( bound_tree_box,
                                                         wxVERTICAL );
    item0->Add(bound_tree_sizer, 0, wxGROW|wxALL, 5);

    wxBoxSizer* itemSizer2 = new wxBoxSizer( wxHORIZONTAL );

    wxChoice* choice2 = new wxChoice(panel, ID_BOUND_TYPE, wxDefaultPosition,
                                     wxDefaultSize, bound_choices );
    choice2->SetSelection( bound_type );

    itemSizer2->Add(new wxStaticText(panel, wxID_ANY, _("&Bound type:")), 0, 
                    wxALL|wxALIGN_CENTER_VERTICAL, 5);
   // itemSizer2->Add(5, 5, 1, wxALL, 0);
    itemSizer2->Add(choice2, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
    bound_tree_sizer->Add(itemSizer2, 0, wxGROW|wxALL, 5);

    wxBoxSizer* max_triangles_sizer = new wxBoxSizer( wxHORIZONTAL );
    max_triangles_sizer->Add(new wxStaticText(panel, wxID_ANY,
                             _("&Max triangles:")), 0, 
                             wxALL|wxALIGN_CENTER_VERTICAL, 5);
    
    wxSpinCtrl* max_triangles_spin = new wxSpinCtrl(panel, ID_MAX_TRIANGLES, 
                                                    wxEmptyString,
                                                    wxDefaultPosition,
                                                    wxSize(80,wxDefaultCoord));
    max_triangles_spin->SetRange( 1, 1000 );
    max_triangles_spin->SetValue( max_triangles_in_leaf );
    max_triangles_sizer->Add(max_triangles_spin, 0,
                             wxALL|wxALIGN_CENTER_HORIZONTAL|
                             wxALIGN_CENTER_VERTICAL, 5);
    bound_tree_sizer->Add( max_triangles_sizer );

    //// haptics options

    HapticsOptions *ho = NULL;

    int touchable_face = 0;
    string max_distance = "0.015";
    string look_ahead_factor = "3";
    bool use_bound_tree = true;
      

  if( gs ) {
    gs->getOptionNode( ho );
    if( ho ) {
      use_bound_tree = ho->useBoundTree->getValue();

      const string &face = ho->touchableFace->getValue();
      if( face == "AS_GRAPHICS" ) touchable_face = 0;
      else if( face == "FRONT_AND_BACK" ) touchable_face = 1;
      else if( face == "FRONT" ) touchable_face = 2;
      else if( face == "BACK" ) touchable_face = 3;
      
      // TODO: maxdistance lookahead
      //max_distance = ho->drawHapticTriangles->getValue();
    }
  }

    wxStaticBox* haptics_box = new wxStaticBox(panel, wxID_ANY,
                                               _("Haptics options:"));
    wxBoxSizer* haptics_box_sizer = new wxStaticBoxSizer( haptics_box, 
                                                          wxVERTICAL );

    wxArrayString face_choices;
    face_choices.Add(wxT("As graphics"));
    face_choices.Add(wxT("Front and back"));
    face_choices.Add(wxT("Front only"));
    face_choices.Add(wxT("Back only"));

    wxBoxSizer* face_choice_sizer = new wxBoxSizer( wxHORIZONTAL );
    face_choice_sizer->Add(new wxStaticText(panel, wxID_ANY,
                                            _("&Touchable face:")), 0,
                                            wxALL|wxALIGN_CENTER_VERTICAL, 5);
    wxChoice* face_choice = new wxChoice(panel, ID_TOUCHABLE_FACE,
                                         wxDefaultPosition, wxDefaultSize,
                                         face_choices );
    face_choice->SetSelection( touchable_face );
    face_choice_sizer->Add(face_choice, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
    haptics_box_sizer->Add(face_choice_sizer, 0, wxGROW|wxALL, 5);

    wxBoxSizer* max_distance_sizer = new wxBoxSizer( wxHORIZONTAL );
    max_distance_sizer->Add(new wxStaticText(panel, wxID_ANY,
                                             _("&Max distance:")), 0,
                                             wxALL|wxALIGN_CENTER_VERTICAL, 5);
    wxTextCtrl* max_distance_text = new wxTextCtrl(panel, ID_MAX_DISTANCE,
                                                   wxEmptyString,
                                                   wxDefaultPosition,
                                                   wxSize(40, wxDefaultCoord));
    max_distance_text->SetValue( max_distance.c_str() );
    max_distance_sizer->Add(max_distance_text, 0,
                            wxALL|wxALIGN_CENTER_VERTICAL, 5);
    haptics_box_sizer->Add(max_distance_sizer, 0, wxGROW|wxALL, 5);

    wxBoxSizer* look_ahead_sizer = new wxBoxSizer( wxHORIZONTAL );
    look_ahead_sizer->Add(new wxStaticText(panel, wxID_ANY,
                                           _("&Look ahead factor:")), 0,
                                           wxALL|wxALIGN_CENTER_VERTICAL, 5);
    wxTextCtrl* look_ahead_text = new wxTextCtrl(panel, ID_LOOK_AHEAD_FACTOR,
                                                 wxEmptyString,
                                                 wxDefaultPosition,
                                                 wxSize(40, wxDefaultCoord) );
    look_ahead_text->SetValue( look_ahead_factor.c_str());
    look_ahead_sizer->Add(look_ahead_text, 0, wxALL|wxALIGN_CENTER_VERTICAL,5);
    haptics_box_sizer->Add(look_ahead_sizer, 0, wxGROW|wxALL, 5);

    wxBoxSizer* use_bound_tree_sizer = new wxBoxSizer( wxHORIZONTAL );
    wxCheckBox* use_bound_tree_checkbox = new wxCheckBox(panel,
                                                         ID_USE_BOUND_TREE, 
                                                         _("&Use bound tree"),
                                                         wxDefaultPosition,
                                                         wxDefaultSize);
    use_bound_tree_checkbox->SetValue( use_bound_tree );
    use_bound_tree_sizer->Add(use_bound_tree_checkbox, 0,
                              wxALL|wxALIGN_CENTER_VERTICAL, 5);
    //haptics_box_sizer->Add(use_bound_tree_checkbox, 0, wxGROW|wxALL, 0);
    haptics_box_sizer->Add(use_bound_tree_sizer, 0, wxGROW|wxALL, 0);

    item0->Add(haptics_box_sizer, 0, wxGROW|wxLEFT|wxRIGHT, 5);

    topSizer->Add( item0, 1, wxGROW|wxALIGN_CENTRE|wxALL, 5 );
    topSizer->AddSpacer(5);

    panel->SetSizer(topSizer);
    topSizer->Fit(panel);

    return panel;
}



void WxFrame::readSettingsFromINIFile( const string &filename, 
                                          GlobalSettings *gs ) {
  gs->options->clear();

  // default values

  INIFile ini_file( filename );

  // graphics caching options
  
  GraphicsCachingOptions *gco = new GraphicsCachingOptions;

  if( ini_file.hasOption( "graphics caching",
                          "use_display_lists" ) ) 
    gco->useCaching->setValue( ini_file.getBoolean( "graphics caching",
                                                    "use_display_lists" ) );

  if( ini_file.hasOption( "graphics caching",
                          "cache_only_geometries" ) ) 
    gco->cacheOnlyGeometries->setValue( 
                              ini_file.getBoolean( "graphics caching",
                                                  "cache_only_geometries" ) );

  if( ini_file.hasOption( "graphics caching",
                          "caching_delay" ) ) 
    gco->cachingDelay->setValue( atoi( ini_file.get( 
                                            "graphics caching",
                                            "caching_delay" ).c_str() ) );

  global_settings->options->push_back( gco );

  // debug options

  DebugOptions *debug = new DebugOptions;

  if( ini_file.hasOption( "debug",
                          "draw_bound" ) ) 
    debug->drawBound->setValue( ini_file.getBoolean( "debug",
                                                     "draw_bound" ) );

  if( ini_file.hasOption( "debug",
                          "draw_tree" ) &&
      ini_file.hasOption( "debug",
                          "draw_tree_depth" )) {
    if( ini_file.getBoolean( "debug", "draw_tree" ) ) {
      debug->drawBoundTree->setValue( atoi( ini_file.get( 
                                               "debug",
                                               "draw_tree_depth" ).c_str() ) );
    } 
  }

  if( ini_file.hasOption( "debug",
                          "draw_triangles" ) ) 
    debug->drawHapticTriangles->setValue( ini_file.getBoolean( "debug",
                                                          "draw_triangles" ) );
  global_settings->options->push_back( debug );

  // haptics options

  HapticsOptions *ho = new HapticsOptions;

  if( ini_file.hasOption( "haptics",
                          "touchable_face" ) ) 
    ho->touchableFace->setValue( ini_file.get( "haptics",
                                               "touchable_face" ) );

  if( ini_file.hasOption( "haptics",
                          "max_distance" ) ) {
    ho->maxDistance->setValue( atof( ini_file.get("haptics",
                                                  "max_distance" ).c_str() ) );
  } 

  if( ini_file.hasOption( "haptics",
                          "look_ahead_factor" ) ) {
    ho->lookAheadFactor->setValue( atof( ini_file.get( 
                                            "haptics",
                                            "look_ahead_factor" ).c_str() ) );
  } 

  if( ini_file.hasOption( "haptics",
                          "use_bound_tree" ) ) 
    ho->useBoundTree->setValue( ini_file.getBoolean( "haptics",
                                                     "use_bound_tree" ) );
  global_settings->options->push_back( ho );

  // geometry bound tree

  GeometryBoundTreeOptions *gbto = new GeometryBoundTreeOptions;

  if( ini_file.hasOption( "bound tree",
                          "max_triangles_in_leaf" ) ) 
    gbto->maxTrianglesInLeaf->setValue( atoi( ini_file.get(
                                        "bound tree",
                                        "max_triangles_in_leaf" ).c_str() ) );

  if( ini_file.hasOption( "bound tree",
                          "bound_type" ) ) {
    gbto->boundType->setValue( ini_file.get( "bound tree",
                                             "bound_type" ).c_str() );
  } 

  global_settings->options->push_back( gbto );

  // OpenHaptics

  OpenHapticsOptions *oho = new OpenHapticsOptions;

  if( ini_file.hasOption( "OpenHaptics",
                          "default_shape" ) ) 
    oho->GLShape->setValue( ini_file.get( "OpenHaptics",
                                          "default_shape" ) );

  if( ini_file.hasOption( "OpenHaptics",
                          "use_adaptive_viewport" ) ) 
    oho->useAdaptiveViewport->setValue( ini_file.getBoolean( "OpenHaptics",
                                                  "use_adaptive_viewport" ) );

  if( ini_file.hasOption( "OpenHaptics",
                          "use_haptic_camera_view" ) ) 
    oho->useHapticCameraView->setValue( ini_file.getBoolean( "OpenHaptics",
                                                  "use_haptic_camera_view" ) );

  if( ini_file.hasOption( "OpenHaptics",
                          "force_full_geometry_render" ) ) 
    oho->forceFullGeometryRender->setValue( ini_file.getBoolean( "OpenHaptics",
                                              "force_full_geometry_render" ) );

  global_settings->options->push_back( oho );
}

void SettingsDialog::OnOk (wxCommandEvent & event) {
  static_cast< WxFrame* >(this->GetParent())->SaveSettings();
  this->Show(false);
}

void SettingsDialog::OnCancel (wxCommandEvent & event) {
  static_cast< WxFrame* >(this->GetParent())->LoadSettings();
  H3DDisplayListObject::DisplayList::rebuildAllDisplayLists();
  this->Show(false);
}

void WxFrame::setProxyRadius( float r ) {
  current_proxy_radius = r;

  for (NodeVector::const_iterator nv = allDevices.begin(); 
       nv != allDevices.end(); nv++) {
    RuspiniRenderer *renderer = 
		dynamic_cast< RuspiniRenderer * >( 
          static_cast < H3DHapticsDevice *> (*nv)->
		  hapticsRenderer->getValue() );

    if( renderer ) {
      renderer->proxyRadius->setValue( current_proxy_radius );
     }
  }

  Node *proxy = default_stylus_dn.getNode("PROXY");
  if( proxy ) {
    SFFloat *radius = 
      dynamic_cast< SFFloat * >( proxy->getField("radius") );
    if( radius ) 
      radius->setValue( current_proxy_radius );
  }

}
