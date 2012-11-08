//////////////////////////////////////////////////////////////////////////////
//    Copyright 2006-2012, SenseGraphics AB
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

#include <wx/apptrait.h>
#include <wx/stdpaths.h>
#include <wx/filename.h>
#include <wx/gbsizer.h>
#include "WxFrame.h"
#include "Envini.h"

// ---------------------------------------------------------------------------
//  Includes (to open X3D files)
// ---------------------------------------------------------------------------

#include <H3D/VrmlParser.h>
#include <H3D/Group.h>
#include <H3D/Viewpoint.h>
#include <H3D/ViewpointGroup.h>
#include <H3D/INIFile.h>
#include <H3D/GraphicsCachingOptions.h>
#include <H3D/DebugOptions.h>
#include <H3D/HapticsOptions.h>
#include <H3D/GeometryBoundTreeOptions.h>
#include <H3D/CollisionOptions.h>
#include <H3D/H3DNavigation.h>
#include <H3D/HapticsRenderers.h>

#include <H3DUtil/DynamicLibrary.h>

using namespace std;
using namespace H3D;

inline string toStr( const wxString &s ) {
# if(wxUSE_UNICODE)
  char *b = new char[s.size()+1];
  const wchar_t *wb = s.c_str();
  for( unsigned int i = 0; i < s.size(); i++ ) {
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

void insertLineBreak(stringstream &inputstream, stringstream &outputstream, int charCount){
  char *line = new char[charCount];
  int length;
  int counter = 0;
  string l;

  inputstream.seekg (0, ios::end);
  length = inputstream.tellg();
  inputstream.seekg (0, ios::beg);

  while (!inputstream.eof() && counter<length)
  {
    inputstream.getline(line,charCount);
    inputstream.clear();
    inputstream.seekg(0,ios_base::cur);
    l = string(line);
    outputstream << l << endl;
    counter += charCount;
  }
	delete []line;
}
/*******************Required Class***********************/

#if wxUSE_DRAG_AND_DROP

void onDropFiles( wxCoord x, wxCoord y,
                  const wxArrayString& filenames,
                  void *arg ) {
  WxFrame *f = static_cast< WxFrame * >( arg );
  size_t n_files = filenames.GetCount();
  // load the first file, ignore the rest
  if( n_files > 0 ) {
    f->clearData();
    string filename( filenames[0].mb_str() );
    f->loadFile( filename );
    f->recentFiles->AddFileToHistory ( wxString( filename.c_str(), wxConvUTF8) );
    f->SetStatusText(wxT("File loaded"), 0);
    f->SetStatusText( wxString(filename.c_str(),wxConvUTF8), 1 );
  }
};

#endif

/*******************Global Constants*********************/
static const wxChar *AUTHOR    = wxT("\nSenseGraphics\n\nCopyright 2006-2009.\nAll Rights Reserved.");
static const wxChar *ABOUT     = wxT("About");
static const wxChar *FILETYPES = wxT( "x3d or vrml 2.0 files|*.x3d;*.x3dv;*.wrl|All files|*.*"
                                   );

/******************Internal definitions**************/
namespace WxFrameInternals {
  wxString wx_as_graphics = wxT("As graphics");
  wxString wx_front_and_back = wxT("Front and back");
  wxString wx_front = wxT("Front only");
  wxString wx_back = wxT("Back only");
  string as_graphics = "AS_GRAPHICS";
  string front_and_back = "FRONT_AND_BACK";
  string front = "FRONT";
  string back = "BACK";

  wxString wx_always = wxT("Always");
  wxString wx_never = wxT("Never");
  wxString wx_transform_changed = wxT("Transform changed");
  string always = "ALWAYS";
  string never = "NEVER";
  string transform_changed = "TRANSFORM_CHANGED";

  wxString wx_OBB = wxT("Oriented bounding box");
  wxString wx_AABB = wxT("Axis-aligned bounding box");
  wxString wx_SPHERE = wxT("Sphere");
  string str_OBB = "OBB";
  string str_AABB = "AABB";
  string str_SPHERE = "SPHERE";

  wxString wx_FEEDBACK = wxT("Feedback buffer");
  wxString wx_DEPTH = wxT("Depth buffer");
  wxString wx_CUSTOM = wxT("Custom");
  string str_FEEDBACK = "FEEDBACK_BUFFER";
  string str_DEPTH = "DEPTH_BUFFER";
  string str_CUSTOM = "CUSTOM";
}

/*******************Constructor*********************/
WxFrame::WxFrame( wxWindow *_parent, wxWindowID _id,
                        const wxString& _title, const wxPoint& _pos,
                        const wxSize& _size, long _style,
                        const wxString& _name ):
  wxFrame(_parent, _id, _title, _pos, _size, _style, _name ),
  navTypeCount(0),
  deviceCount(0),
  navigationDevices(NULL),
  avatar_collision( true ),
  loaded_first_file( false ),
  change_nav_type( new ChangeNavType ),
  handle_action_key( new HandleActionKey ),
  itemIdViewpointMap(),
  current_viewpoint_id(0)
{
  lastOpenedFilepath = "";
  wxAcceleratorEntry entries[1];
  entries[0].Set(wxACCEL_NORMAL, (int) WXK_F11, FRAME_TOGGLE_FULLSCREEN);
  wxAcceleratorTable accel(1, entries);
  SetAcceleratorTable(accel);

  scene.reset( new Scene );
  ks.reset ( new KeySensor );
  viewpoint.reset( new Viewpoint );
  device_info.reset (NULL);

  glwindow = new WxWidgetsWindow(this);
#if wxUSE_DRAG_AND_DROP
  glwindow->onFileDraggedAndDroppedFunction( &onDropFiles, this );
#endif
  int width, height;
  GetClientSize(&width, &height);
  glwindow->width->setValue(width);
  glwindow->height->setValue(height);
    
  scene->window->push_back( glwindow );
  
  wxString console_string = wxT("Console");
  the_console = new WxConsoleDialog( this, wxID_ANY, console_string,
                                   wxDefaultPosition, wxDefaultSize,
                                   wxDEFAULT_DIALOG_STYLE );
  tree_view_dialog = new H3DViewerTreeViewDialog( this ); 
#ifdef HAVE_WXPROPGRID
  program_settings_dialog = new H3DViewerFieldValuesDialogPropGrid( this ); 
  program_settings_dialog->displayFieldsFromProgramSettings();
  program_settings_dialog->destroy_on_close = false;
#endif
  plugins_dialog = new H3DViewerPluginsDialog( this ); 
  frameRates = new FrameRateDialog( this );

  current_viewpoint = (X3DViewpointNode *) NULL;
  mydevice = (DeviceInfo *) NULL;
  mynav = (NavigationInfo *) NULL;

  //Haptics on by default
  /*********TODO: Save this in registry for next session**************/
  //lastDeviceStatus = true;

  //Main Menu Bar
  menuBar  = (wxMenuBar *) NULL;

  //Top level menus
  fileMenu = (wxMenu *)  NULL;
  rendererMenu = (wxMenu *) NULL;
  viewpointMenu = (wxMenu *) NULL;
  navigationMenu = (wxMenu *) NULL;
  advancedMenu = (wxMenu *) NULL;
  helpMenu = (wxMenu *)  NULL;

  //Submenus
  hapticsRenderer = (wxMenu *) NULL;
  renderMode = (wxMenu *) NULL;
  stereoRenderMode = (wxMenu *) NULL;

  global_settings.reset( new GlobalSettings );
  global_settings->options->push_back( new CollisionOptions );
  global_settings->options->push_back( new DebugOptions );
  global_settings->options->push_back( new GeometryBoundTreeOptions );
  global_settings->options->push_back( new GraphicsCachingOptions );
  global_settings->options->push_back( new HapticsOptions );
  global_settings->options->push_back( new OpenHapticsOptions );
  stereo_info.reset( new StereoInfo );

  //File History
  recentFiles = (wxFileHistory *) NULL;
  h3dConfig = (wxConfigBase *) NULL;

  //Status Bar
  CreateStatusBar(2);

  /******************Menu Bar Items*****************/
  //File menu
  fileMenu = new wxMenu;
  fileMenu->Append(FRAME_OPEN,wxT("&Open file...\tCtrl+O"),wxT("Open a file"));
  fileMenu->Append(FRAME_OPEN_URL, wxT("&Open file from URL..."), wxT("Open a file ")
                   wxT("from UR") );
  fileMenu->Append(FRAME_CLOSE, wxT("&Close file\tCtrl+F4"),wxT("Close file"));
  fileMenu->AppendSeparator();
  fileMenu->Append(FRAME_CHOOSEDIR, wxT("Change Working Directory"), wxT("Change ")
                   wxT("working directory..."));
  // 09.10.14 reload page
  fileMenu->Append(FRAME_RELOAD, wxT("&Reload\tF5"), wxT("Reload the file"));

  fileMenu->AppendSeparator();
  fileMenu->Append(FRAME_PLUGINS, wxT("Plugins"),
                   wxT("Show/change installed plugins")); 
  fileMenu->AppendSeparator();
  fileMenu->Append(FRAME_EXIT,wxT("E&xit\tCtrl+X"), wxT("Exit"));

  //File History
  recentFiles = new wxFileHistory;
  recentFiles->UseMenu(fileMenu);

  //Initialize config object
  wxConfigBase *h3dConfig = wxConfigBase::Get();

  //Load file history and settings from previous session(s)
  LoadMRU();

  // Create settings dialog
  settings = new SettingsDialog(this, this );
  // Load settings for dialog
  LoadSettings( true );

  LoadPlugins();

  //Submenus for Renderer Menu
  //hapticsRenderer
  hapticsRenderer = new wxMenu;
  hapticsRenderer->AppendRadioItem(FRAME_OPENHAPTICS, wxT("Openhaptics"), 
                                   wxT("Openhaptics Renderer"));
  hapticsRenderer->AppendRadioItem(FRAME_CHAI3D, wxT("CHAI3D"), wxT("CHAI3D Renderer"));
  hapticsRenderer->AppendRadioItem(FRAME_GODOBJECT, wxT("GodObject"), 
                                   wxT("GodObject Renderer"));
  hapticsRenderer->AppendRadioItem(FRAME_RUSPINI, wxT("Ruspini"), 
                                   wxT("Ruspini Renderer"));

  //renderMode
  renderMode = new wxMenu;
  renderMode->AppendRadioItem(FRAME_RENDERMODE_DEFAULT,
                              wxT("Default"),
                              wxT("Use settings from X3D scene."));
  renderMode->AppendRadioItem(FRAME_RENDERMODE_FILLED, 
                              wxT("Force filled"),
                              wxT("Draw all geometries filled"));
  renderMode->AppendRadioItem(FRAME_RENDERMODE_WIREFRAME, 
                              wxT("Force wireframe"),
                              wxT("Draw all geometries as wireframe."));
  renderMode->AppendRadioItem(FRAME_RENDERMODE_POINTS,
                              wxT("Force points"),
                              wxT("Draw the vertices of all geometries as points."));

  //stereoRenderMode
  stereoRenderMode = new wxMenu;
  stereoRenderMode->AppendRadioItem(FRAME_MONO, wxT("No Stereo"),
                                    wxT("Disable stereo display"));
  stereoRenderMode->AppendRadioItem(FRAME_QUADBUFFERED, wxT("Quad Buffered Stereo"),
                                    wxT("Quad Buffered Stereo"));
  stereoRenderMode->AppendRadioItem(FRAME_HORZSPLIT, wxT("Horizontal Split"),
                                    wxT("Horizontal Split"));
  stereoRenderMode->AppendRadioItem(FRAME_HORZSPLITKEEPASPECT,
                                    wxT("Horizontal Split Keep Aspect"),
                                    wxT("Horizontal Split with aspect ratio kept the same."));
  stereoRenderMode->AppendRadioItem(FRAME_VERTSPLIT, wxT("Vertical Split"),
                                    wxT("Vertical Split"));
  stereoRenderMode->AppendRadioItem(FRAME_VERTSPLITKEEPASPECT,
                                    wxT("Vertical Split Keep Aspect"),
                                    wxT("Vertical Split with aspect ratio kept the same."));
  stereoRenderMode->AppendRadioItem(FRAME_HORZINTERLACED,
                                    wxT("Horizontal Interlaced"),
                                    wxT("Horizontal Interlaced"));
  stereoRenderMode->AppendRadioItem(FRAME_VERTINTERLACED,
                                    wxT("Vertical Interlaced"),
                                    wxT("Vertical Interlaced"));
  stereoRenderMode->AppendRadioItem(FRAME_CHECKERINTERLACED,
                                    wxT("Checker Interlaced"),
                                    wxT("Checker Interlaced"));
  stereoRenderMode->AppendRadioItem(FRAME_SHARPDISPLAY, wxT("Sharp Display"),
                                    wxT("Optimized for Sharp display systems"));
  stereoRenderMode->AppendRadioItem(FRAME_REDBLUE, wxT("Red-Blue Stereo"),
                                    wxT("Red-Blue Stereo mode"));
  stereoRenderMode->AppendRadioItem(FRAME_REDGREEN, wxT("Red-Green Stereo"),
                                    wxT("Red-Green Stereo mode"));
  stereoRenderMode->AppendRadioItem(FRAME_REDCYAN, wxT("Red-Cyan Stereo"),
                                    wxT("Red-Cyan Stereo mode"));
  stereoRenderMode->AppendRadioItem(FRAME_HDMI720P, wxT("HDMI Frame packed 720p"),
                                    wxT("HDMI Frame packed 720p"));
  stereoRenderMode->AppendRadioItem(FRAME_HDMI1080P, wxT("HDMI Frame packed 1080p"),
                                    wxT("HDMI Frame packed 1080p"));
#ifdef HAVE_DX9
  stereoRenderMode->AppendRadioItem(FRAME_NVIDIA_3DVISION, wxT("3DVision from NVidia"),
                                    wxT("3DVision from NVidia. Requires executable name change."));
#endif  
  //Renderer Menu
  rendererMenu = new wxMenu;
  rendererMenu->AppendCheckItem(FRAME_FULLSCREEN, wxT("Fullscreen Mode\tF11"),
                                wxT("View in fullscreen"));
  rendererMenu->AppendCheckItem(FRAME_MIRROR, wxT("Mirror in Y"),
                                wxT("Mirror Scene in Y"));
  rendererMenu->AppendSeparator();
  rendererMenu->Append(FRAME_CHOOSERENDERER, 
                       wxT("Choose Haptics Renderer"),
                       hapticsRenderer, wxT("Select a haptics renderer"));

  rendererMenu->Append(FRAME_RENDERMODE, 
                       wxT("Select render mode"), renderMode,
                       wxT("Select the render mode"));

  rendererMenu->Append(FRAME_STEREORENDERMODE, 
                       wxT("Select stereo mode"), stereoRenderMode,
                       wxT("Select the stereo rendering mode"));
  rendererMenu->AppendSeparator();
  rendererMenu->Append(FRAME_SETTINGS, wxT("Settings..."),
                       wxT("Scenegraph rendering options"));

  //Viewpoint Menu
  viewpointMenu = new wxMenu;

  //Navigation Menu
  navigationMenu = new wxMenu;

  //Advanced Menu
  advancedMenu = new wxMenu;
  advancedMenu->Append(FRAME_CONSOLE, wxT("Show Console\tF10"),
                       wxT("Show the message console"));
  advancedMenu->Append(FRAME_TREEVIEW, wxT("Show Tree View\tF9"),
                       wxT("Show the scene as a tree, making it possible to inspect and change values at runtime."));
   advancedMenu->Append(FRAME_FRAMERATE, wxT("Show Framerates\tF8"),
                       wxT("Show the frame rates of graphics and haptics loop"));
#ifdef HAVE_WXPROPGRID
   advancedMenu->Append(FRAME_PROGRAMSETTINGS, wxT("Show Program Settings\tF7"),
                        wxT("Show the program settings for the current scene."));  
#endif
	advancedMenu->AppendSeparator();
	advancedMenu->AppendCheckItem( FRAME_KEEPVIEWPOINTONLOAD, wxT("New viewpoint on load file"),
																 wxT("If checked new viewpoints are loaded when a new file is loaded, otherwise old viewpoint is kept.") );
	advancedMenu->AppendCheckItem( FRAME_ROUTESENDSEVENTS, wxT("Route sends events"),
																 wxT("If checked routes sends events when set up, i.e. not as in X3D spec.") );
	h3dConfig->SetPath(wxT("/Settings"));
  bool new_viewpoint_on_load = true;
  h3dConfig->Read(wxT("new_viewpoint_on_load"), &new_viewpoint_on_load);
	advancedMenu->Check( FRAME_KEEPVIEWPOINTONLOAD, new_viewpoint_on_load );
	bool route_sends_events = true;
  h3dConfig->Read(wxT("route_sends_events"), &route_sends_events);
	advancedMenu->Check( FRAME_ROUTESENDSEVENTS, route_sends_events );
	global_settings->x3dROUTESendsEvent->setValue( route_sends_events );
  //Help Menu
  helpMenu = new wxMenu;
  //helpMenu->Append(FRAME_HELP, wxT("Help"));
  helpMenu->Append(wxID_ABOUT, wxT("About"));

  //Install Menu Bar
  menuBar = new wxMenuBar;
  menuBar->Append(fileMenu, wxT("&File"));
  menuBar->Append(rendererMenu, wxT("&Rendering"));
  menuBar->Append(viewpointMenu, wxT("&Viewpoints"));
  menuBar->Append(navigationMenu, wxT("&Navigation"));
  speed_slider = new SpeedDialog( this, this );
  speed_slider->Show( false );
  menuBar->Append(advancedMenu, wxT("&Advanced"));
  menuBar->Append(helpMenu, wxT("&Help"));
  SetMenuBar(menuBar);

  //Disable some menus initially
  //Top menu items
  menuBar->EnableTop(2, false);
  menuBar->EnableTop(3, false);

  //Certain options in rendererMenu
  rendererMenu->Enable(FRAME_CHOOSERENDERER, false);
  rendererMenu->Enable(FRAME_RENDERMODE, false);
  rendererMenu->Enable(FRAME_STEREORENDERMODE, false);

  change_nav_type->setOwnerWindows( glwindow, this );
  handle_action_key->setOwnerWindows( glwindow, this );
  ks->keyPress->route( change_nav_type );
  ks->actionKeyPress->route( handle_action_key );
#ifdef WIN32
  wxIcon tmpIcon( wxT( "IDI_ICON1" ), wxBITMAP_TYPE_ICO_RESOURCE );
  SetIcon( tmpIcon );
#endif
  Layout();
}

void WxFrame::ChangeNavType::update() {
  NavigationInfo *mynav =0;
  if(NavigationInfo::getActive()){
    mynav = NavigationInfo::getActive();
  }
  string s = static_cast< SFString * >(routes_in[0])->getValue();
  if( s.empty() ) return;
  if( s == "+" ) {
    if( mynav ) {
      frame->speed_slider->setSpeed(
        mynav->speed->getValue() + speed_increment, true, true );
    } else {
      frame->speed_slider->setSpeed(
        glwindow->default_speed + speed_increment, true, true );
    }
  } else if( s == "-" ) {
    if( mynav ) {
      frame->speed_slider->setSpeed(
        mynav->speed->getValue() - speed_increment, true, true );
    } else {
      frame->speed_slider->setSpeed(
        glwindow->default_speed - speed_increment, true, true );
    }
  }
}

void WxFrame::HandleActionKey::update() {
  int key = static_cast< SFInt32 * >(routes_in[0])->getValue();
   switch( key ) {
     case KeySensor::HOME:  {
       // Goes to the initial viewpoint
       wxCommandEvent fake_event;
       fake_event.SetId( frame->itemIdViewpointMap.begin()->first );
       frame->ChangeViewpoint( fake_event );
       break;
     }
     case KeySensor::END: {
       //Goes to the final viewpoint 
       wxCommandEvent fake_event;
       map< int, X3DViewpointNode * >::iterator last_item = frame->itemIdViewpointMap.end();
       last_item--;
       fake_event.SetId( last_item->first );
       frame->ChangeViewpoint( fake_event );
       break;
     }
     case KeySensor::PGDN: {
       // Goes to the next viewpoint, if the active viewpoint is the
       // last one the next viewpoint is set to the first viewpoint
       wxCommandEvent fake_event;
       int new_int = frame->current_viewpoint_id + 1;
       map< int, X3DViewpointNode * >::iterator last_item = frame->itemIdViewpointMap.end();
       last_item--;
       if( new_int > last_item->first ) {
         new_int = frame->itemIdViewpointMap.begin()->first;
       }
       fake_event.SetId( new_int );
       frame->ChangeViewpoint( fake_event );
       break;
     }
     case KeySensor::PGUP: {
       // Goes to the previous viewpoint, if the active viewpoint is the
       // first one the previous viewpoint is set to the last viewpoint
       wxCommandEvent fake_event;
       int new_int = frame->current_viewpoint_id - 1;
       if( new_int < frame->itemIdViewpointMap.begin()->first ) {
         map< int, X3DViewpointNode * >::iterator last_item = frame->itemIdViewpointMap.end();
         last_item--;
         new_int = last_item->first;
       }
       fake_event.SetId( new_int );
       frame->ChangeViewpoint( fake_event );
       break;
     }
     default: {}
  }
}

/*******************Event Table*********************/
BEGIN_EVENT_TABLE(WxFrame, wxFrame)
  EVT_MENU (FRAME_EXIT, WxFrame::OnExit)
  EVT_MENU (FRAME_OPEN, WxFrame::OnOpenFile)
  EVT_MENU (FRAME_RELOAD, WxFrame::OnReload)    // 09.10.14 reload frame
  EVT_MENU_RANGE (wxID_FILE1, wxID_FILE9, WxFrame::OnMRUFile)
  EVT_MENU (FRAME_OPEN_URL, WxFrame::OnOpenFileURL)
  EVT_MENU (FRAME_CLOSE, WxFrame::OnCloseFile)
  EVT_MENU (FRAME_CHOOSEDIR, WxFrame::OnChooseDir)
  EVT_MENU (FRAME_FULLSCREEN, WxFrame::OnFullscreen)
  EVT_MENU (FRAME_SETTINGS, WxFrame::OnSettings)
  EVT_MENU (FRAME_TOGGLE_FULLSCREEN, WxFrame::ToggleFullscreen)
  EVT_MENU (FRAME_MIRROR, WxFrame::MirrorScene)
  EVT_MENU_RANGE (FRAME_MONO, FRAME_HDMI1080P, WxFrame::StereoRenderMode)
  EVT_MENU_RANGE (FRAME_RENDERMODE_DEFAULT, 
                  FRAME_RENDERMODE_POINTS, WxFrame::RenderMode )
  EVT_MENU (FRAME_CONSOLE, WxFrame::ShowConsole)
  EVT_MENU (FRAME_TREEVIEW, WxFrame::ShowTreeView)
  EVT_MENU (FRAME_PLUGINS, WxFrame::ShowPluginsDialog)
  EVT_MENU (FRAME_FRAMERATE, WxFrame::ShowFrameRate)
  EVT_MENU (FRAME_PROGRAMSETTINGS, WxFrame::ShowProgramSettings)
	EVT_MENU (FRAME_KEEPVIEWPOINTONLOAD, WxFrame::OnKeepViewpointOnLoadCheck )
	EVT_MENU (FRAME_ROUTESENDSEVENTS, WxFrame::OnRouteSendsEventsCheck )
  EVT_MENU (FRAME_VIEWPOINT, WxFrame::ChangeViewpoint)
  EVT_MENU (FRAME_RESET_VIEWPOINT, WxFrame::ResetViewpoint)
  EVT_MENU (FRAME_NAVIGATION, WxFrame::ChangeNavigation)
  EVT_MENU_RANGE (FRAME_MOUSE_NAV, FRAME_HAPTICSDEVICE_NAV,
                  WxFrame::ChangeNavigationDevice)
  EVT_MENU (BASIC_COLLISION, WxFrame::ChangeCollision )
  EVT_MENU (FRAME_SPEED, WxFrame::OnSpeed )
  EVT_MENU_RANGE (FRAME_OPENHAPTICS, FRAME_RUSPINI, WxFrame::ChangeRenderer)
  //EVT_MENU (FRAME_DEVICECONTROL, WxFrame::ToggleHaptics)
  EVT_MENU (wxID_ABOUT, WxFrame::OnAbout)
  //EVT_MENU (FRAME_HELP, WxFrame::OnHelp)
  EVT_IDLE (WxFrame::OnIdle)
  EVT_CLOSE(WxFrame::OnWindowExit)
END_EVENT_TABLE()


/*******************Event Table*********************/

void SettingsDialog::handleSettingsChange (wxCommandEvent & event) {
  int id = event.GetId(); 
  /// TODO: Why is this not a switch case? Or at least a big if else case.
  if( id == ID_DRAW_BOUNDS ||
      id == ID_DRAW_TRIANGLES ||
      id == ID_DRAW_BOUND_TREE ||
      id == ID_DRAW_TREE_DEPTH ) {
    DebugOptions *dgo = NULL;

    wx_frame->global_settings->getOptionNode( dgo );

    if( id == ID_DRAW_BOUNDS ) {
      dgo->drawBound->setValue( event.IsChecked() );
      H3DDisplayListObject::DisplayList::rebuildAllDisplayLists();
      on_cancel_rebuild_displaylist = true;
    }
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
      H3DDisplayListObject::DisplayList::rebuildAllDisplayLists();
      on_cancel_rebuild_displaylist = true;
    } else if( id == ID_DRAW_TREE_DEPTH ) {
      dgo->drawBoundTree->setValue( event.GetInt() );
      H3DDisplayListObject::DisplayList::rebuildAllDisplayLists();
      on_cancel_rebuild_displaylist = true;
    }

  } else if( id == ID_USE_DISPLAY_LISTS ||
             id == ID_CACHE_ONLY_GEOMS ) {

    GraphicsCachingOptions *gco = NULL;
    wx_frame->global_settings->getOptionNode( gco );

    if( id == ID_USE_DISPLAY_LISTS ) 
      gco->useCaching->setValue( event.IsChecked() );
    else if( id == ID_CACHE_ONLY_GEOMS ) {
      gco->cacheOnlyGeometries->setValue( event.IsChecked() );
    }
  } else if( id == ID_TOUCHABLE_FACE ||
             id == ID_DYNAMIC_MODE ||
             id == ID_MAX_DISTANCE ||
             id == ID_LOOK_AHEAD_FACTOR ||
             id == ID_USE_BOUND_TREE ||
             id == ID_INTERPOLATE_FORCE_EFFECTS ) {

    HapticsOptions *ho = NULL;
    wx_frame->global_settings->getOptionNode( ho );
    if( id == ID_TOUCHABLE_FACE ) {
      int i = event.GetSelection();
      if( i == 0 ) ho->touchableFace->setValue( WxFrameInternals::as_graphics );
      else if( i == 1 ) ho->touchableFace->setValue( WxFrameInternals::front_and_back );
      else if( i == 2 ) ho->touchableFace->setValue( WxFrameInternals::front );
      else if( i == 3 ) ho->touchableFace->setValue( WxFrameInternals::back );
    } else if( id == ID_DYNAMIC_MODE ) {
      int i = event.GetSelection();
      if( i == 0 ) ho->dynamicMode->setValue(
        WxFrameInternals::transform_changed );
      else if( i == 1 ) ho->dynamicMode->setValue( WxFrameInternals::never );
      else if( i == 2 ) ho->dynamicMode->setValue( WxFrameInternals::always );
    } else if( id == ID_MAX_DISTANCE ) {
      ho->maxDistance->setValue( atof( event.GetString().mb_str() ) );
    } else if( id == ID_LOOK_AHEAD_FACTOR ) {
      ho->lookAheadFactor->setValue( atof( event.GetString().mb_str() ) );
    } else if( id == ID_USE_BOUND_TREE ) {
      ho->useBoundTree->setValue( event.IsChecked() );
    } else if( id == ID_INTERPOLATE_FORCE_EFFECTS ) {
      ho->interpolateForceEffects->setValue( event.IsChecked() );
    }
  } else if( id == ID_FOCAL_DISTANCE ||
             id == ID_INTEROCULAR_DISTANCE ) {
    StereoInfo * stereo_info = StereoInfo::getActive();

    if( id == ID_FOCAL_DISTANCE ) {
      stereo_info->focalDistance->setValue( atof( event.GetString().mb_str() ) );
    } else if( id == ID_INTEROCULAR_DISTANCE ) {
      stereo_info->interocularDistance->setValue(
        atof( event.GetString().mb_str() ) );
    }
  } else if( id == ID_OH_SHAPE_TYPE ||
             id == ID_ADAPTIVE_VIEWPORT ||
             id == ID_HAPTIC_CAMERA ||
             id == ID_FULL_GEOMETRY_RENDER ) {
#ifdef WIN32
    if( DynamicLibrary::load( "hd.dll" ) &&
        DynamicLibrary::load( "hl.dll" ) ) {
#endif

    OpenHapticsOptions *oho = NULL;
    wx_frame->global_settings->getOptionNode( oho );
    if( id == ID_OH_SHAPE_TYPE ) {
      int i = event.GetSelection();
      if( i == 0 ) oho->GLShape->setValue( WxFrameInternals::str_FEEDBACK );
      else if( i == 1 ) oho->GLShape->setValue( WxFrameInternals::str_DEPTH );
      else if( i == 2 ) oho->GLShape->setValue( WxFrameInternals::str_CUSTOM );
    } else if( id == ID_ADAPTIVE_VIEWPORT ) {
      oho->useAdaptiveViewport->setValue( event.IsChecked() );
    } else if( id == ID_HAPTIC_CAMERA ) {
      oho->useHapticCameraView->setValue( event.IsChecked() );
    } else if( id == ID_FULL_GEOMETRY_RENDER ) {
      oho->forceFullGeometryRender->setValue( event.IsChecked() );
    }
#ifdef WIN32
    }
#endif
  } else if( id == ID_BOUND_TYPE ) {

    GeometryBoundTreeOptions *gbto = NULL;
    wx_frame->global_settings->getOptionNode( gbto );
    int i = event.GetSelection();
    if( i == 0 ) gbto->boundType->setValue( WxFrameInternals::str_OBB );
    else if( i == 1 ) gbto->boundType->setValue( WxFrameInternals::str_AABB );
    else if( i == 2 ) gbto->boundType->setValue(
      WxFrameInternals::str_SPHERE );
    H3DDisplayListObject::DisplayList::rebuildAllDisplayLists();
    on_cancel_rebuild_displaylist = true;
  }

  if( id == ID_PROXY_RADIUS ) {
    wx_frame->setProxyRadius( atof( event.GetString().mb_str() ) );
  }

}

void SettingsDialog::handleSpinEvent (wxSpinEvent & event) {
  DebugOptions *dgo = NULL;
  wx_frame->global_settings->getOptionNode( dgo );
  int id = event.GetId(); 
  if( dgo ) {
    if( (id == ID_DRAW_TREE_DEPTH) && (boundTree) ) {
      dgo->drawBoundTree->setValue( event.GetInt() );
    } 
    treeDepth = event.GetInt();
    H3DDisplayListObject::DisplayList::rebuildAllDisplayLists();
    on_cancel_rebuild_displaylist = true;
  }

  GraphicsCachingOptions *gco = NULL;
  wx_frame->global_settings->getOptionNode( gco );

  if( gco ) {
    if( id == ID_CACHING_DELAY ) 
      gco->cachingDelay->setValue( event.GetInt() );
  }

  GeometryBoundTreeOptions *gbto = NULL;
  wx_frame->global_settings->getOptionNode( gbto );

  if( gbto ) {
    if( id == ID_MAX_TRIANGLES )  {
      gbto->maxTrianglesInLeaf->setValue( event.GetInt() );
      H3DDisplayListObject::DisplayList::rebuildAllDisplayLists();
      on_cancel_rebuild_displaylist = true;
    }
  }
}


/*******************Member Functions*********************/

bool WxFrame::loadFile( const string &filename) {
#ifdef WIN32
  UINT old_error_mode;
  old_error_mode = SetErrorMode( 0 );
#endif
  char *r = getenv( "H3D_ROOT" );
  string h3d_root = r ? r : ""; 

#ifdef WIN32
  char *home = getenv( "HOMEPATH" );
  string ini_file_path = home ? string(home) + string( "/h3dload.ini" ) :
                                string("h3dload.ini");
#else
  char *home = getenv( "HOME" );
  string ini_file_path = home ? string(home) + string( "/.h3dload.ini" ) :
                                string(".h3dload.ini");
#endif

  bool ini_file_exists = false;
  ifstream check_ini_file( ini_file_path.c_str() );
  if( check_ini_file.is_open() ) {
    ini_file_exists = true;
  }
  check_ini_file.close();
  
  if( !ini_file_exists ){
    ini_file_path = h3d_root + "/settings/h3dload.ini";
    
    ifstream check_ini_file( ini_file_path.c_str() );
    if( check_ini_file.is_open() ) {
      ini_file_exists = true; }
    check_ini_file.close();
  }
  
  INIFile ini_file( ini_file_path );

  lastOpenedFilepath = filename;


  //Clear existing data
  viewpoint.reset( NULL );
  tree_view_dialog->clearTreeView();

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

  manualCursorControl = GET_BOOL("graphical", "manualCursorControl", false);
  if( char *buffer = getenv("H3D_MANUALCURSORCONTROL") ) {
    if (strcmp( buffer, "TRUE" ) == 0 ){
      manualCursorControl = true; }
    else if (strcmp( buffer, "FALSE" ) == 0 ){
      manualCursorControl = false; }
    else
      Console(4) << "Invalid value \"" << buffer 
      << "\" on environment "
      << "variable H3D_MANUALCURSORCONTROL. Must be TRUE or FALSE. "
      << endl;
  }

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
    X3D::DEFNodes dn;

    DeviceInfo *di = DeviceInfo::getActive();
    if( !di ) {
      if( deviceinfo_file.size() ) {
        try {
          device_info = X3D::createX3DNodeFromURL( deviceinfo_file );
        } catch( const Exception::H3DException &e ) {
          Console(3) << "Warning: Could not create default DeviceInfo node "
            << "from file \"" << deviceinfo_file << "\": "
            << e << endl;
        }
      }



      AutoRef< Node > default_stylus( 0 );
      di = DeviceInfo::getActive();
      if( di && stylus_file.size() ) {
        try {
          default_stylus = X3D::createX3DNodeFromURL( stylus_file,
                                                      &default_stylus_dn );
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
    unsigned int device_info_size = (unsigned int)(DeviceInfo::getAllDeviceInfos().size());

    Console(3) << "Loading " << filename << endl;
    scene->loadSceneRoot( filename );

    DeviceInfo::DeviceInfoList device_infos = DeviceInfo::getAllDeviceInfos();
    if( di && device_infos.size() > device_info_size ) {
      unsigned int j = 0;
      for( DeviceInfo::DeviceInfoList::iterator i = device_infos.begin();
           i != device_infos.end(); i++ ) {
        if( j < device_info_size )
          (*i)->set_bind->setValue( false );
        else {
          (*i)->set_bind->setValue( true );
          break;
        }
        j++;
      }
    }

    /**********************Reset mirrored and fullscreen********************/
    if( !loaded_first_file ) {
      rendererMenu->Check(FRAME_FULLSCREEN, fullscreen);
      rendererMenu->Check(FRAME_MIRROR, mirrored);
      lastmirror = mirrored;
    }

    /****************************Navigation Info****************************/
    // Reset default speed for window.
    //Enable Navigation Menu
    menuBar->EnableTop(3, true);
    buildNavMenu();
    speed_slider->setSpeed();

    //Enable graphical rendering options in rendererMenu
    rendererMenu->Enable(FRAME_RENDERMODE, true);
    rendererMenu->Enable(FRAME_STEREORENDERMODE, true);
    rendererMenu->Enable(FRAME_CHOOSERENDERER, true);

    /****************************Device Info****************************/
    //Enable Device Menu
    mydevice = DeviceInfo::getActive();
    if (mydevice && (mydevice->device->size() > 0) ) {
      allDevices = mydevice->device->getValue();

      for( NodeVector::const_iterator nv = allDevices.begin();
           nv != allDevices.end(); nv++ ) {
        H3DHapticsRendererNode *renderer =
          static_cast < H3DHapticsDevice *> (*nv)->hapticsRenderer->getValue();
        RuspiniRenderer *ruspini_renderer = 
          dynamic_cast< RuspiniRenderer * >( renderer );

        if( ruspini_renderer ) {
          stringstream proxy_text;
          proxy_text << ruspini_renderer->proxyRadius->getValue();
          settings->proxy_radius_text->
            SetValue( wxString(proxy_text.str().c_str(),wxConvUTF8) );
          SaveRuspiniSettings(false);
          hapticsRenderer->Check( FRAME_RUSPINI, true );
          //break;
        } else if( dynamic_cast< GodObjectRenderer * >(renderer) ) {
          hapticsRenderer->Check( FRAME_GODOBJECT, true );
        }
  #ifdef HAVE_CHAI3D
        else if( dynamic_cast< Chai3DRenderer * >(renderer) ) {
          hapticsRenderer->Check( FRAME_CHAI3D, true );
        }
  #endif
  #ifdef WIN32
        else if( DynamicLibrary::load( "hd.dll" ) &&
            DynamicLibrary::load( "hl.dll" ) ) {
  #else
        else
  #endif
        if( dynamic_cast< OpenHapticsRenderer * >(renderer) ) {
          hapticsRenderer->Check( FRAME_OPENHAPTICS, true );
        }
  #ifdef WIN32
        }
  #endif
      }
    }

    //create a Viewpoint if it does not exist.
    if( !Viewpoint::getActive() ) {
      if( viewpoint_file.size() ) {
        try {
          viewpoint = X3D::createX3DNodeFromURL( viewpoint_file );
        } catch( const Exception::H3DException &e ) {
          viewpoint.reset( new Viewpoint );
          Console(3) << "Warning: Could not create default Viewpoint node "
                     << "from file \"" << viewpoint_file << "\": "
                     << e << endl;
        }
      } else if( !ini_file_exists )
        viewpoint.reset( new Viewpoint );
    }

    if( !ini_file_exists && device_infos.empty() ) {
      // No device info exists and no device info file either. Simply
      // create an anydevice and use that one in the scene.
      device_info = X3D::createX3DNodeFromString(
        "<DeviceInfo>"
        "  <AnyDevice>"
        "    <GodObjectRenderer/>"
        "  </AnyDevice>"
        "</DeviceInfo>" );

      di = DeviceInfo::getActive();
      if( di ) {
        // Create stylus.
        AutoRef< Node > default_stylus( 0 );
        default_stylus = X3D::createX3DNodeFromString(
          "  <Group>"
          "    <Transform>"
          "      <Shape>"
          "        <Appearance>"
          "          <Material/>"
          "        </Appearance>"
          "        <Sphere DEF=\"PROXY\" radius=\"0.0025\" />"
          "      </Shape>"
          "    </Transform>"
          "  </Group>",
          &default_stylus_dn );

        // Create a calibration matrix that will account for not being the H3D
        // default viewpoint.
        X3DViewpointNode * active_vp = X3DViewpointNode::getActive();
        // Set matrix to calibrate the device for a default X3D Viewpoint.
        // That is, no orientation and position 0, 0, 10. 
        Matrix4f pos_cal_matrix( 1, 0, 0, 0,
                                 0, 1, 0, 0,
                                 0, 0, 1, (H3DFloat)9.4,
                                 0, 0, 0, 1 );
        if( active_vp ) {
          // There is already a viewpoint defined, use this one.
          Vec3f position_part = active_vp->position->getValue();
          position_part.z -= 0.6f;
          pos_cal_matrix = Matrix4f( active_vp->orientation->getValue() );
          pos_cal_matrix[0][3] = position_part.x;
          pos_cal_matrix[1][3] = position_part.y;
          pos_cal_matrix[2][3] = position_part.z;
        }

        // Set stylus and calibration matrix.
        for( DeviceInfo::MFDevice::const_iterator i = di->device->begin();
             i != di->device->end(); i++ ) {
          H3DHapticsDevice *d = static_cast< H3DHapticsDevice * >(*i);
          d->positionCalibration->setValue( pos_cal_matrix );
          if( !d->stylus->getValue() )
            d->stylus->setValue( default_stylus );
        }
        hapticsRenderer->Check( FRAME_GODOBJECT, true );
      }
    }

  /****************************Intialize Viewpoints***************************/
    //Enable Viewpoints Menu
    menuBar->EnableTop(2, true);
    VPlist = GetTopLevelViews();
    current_viewpoint = Viewpoint::getActive();
    current_viewpoint_id = FRAME_VIEWPOINT;
    
    BuildViewpointsMenu( VPlist );

    if( X3DBindableNode::getStack( "DeviceInfo" ).size() > 1 ) {
      device_info.reset( NULL );
    }

    // create a window to display
      
    //This next line is used to set the icon file h3d.ico, when created.
    //theWxframe->SetIcon(wxIcon(wxT("h3d_icn")));
    
    // Using this line instead of the two previous lines will make
    // WxWidgetsWindow create an instance of a wxframe with no menus and use
    // this as parent to the canvas.
    // WxWidgetsWindow *glwindow = new WxWidgetsWindow();

    if( !loaded_first_file ) {
      loaded_first_file = true;
      this->glwindow->fullscreen->setValue( fullscreen );
      this->glwindow->mirrored->setValue( mirrored );
      this->glwindow->manualCursorControl->setValue( manualCursorControl );
      this->glwindow->renderMode->setValue( render_mode );
      if( render_mode == "MONO" )
        stereoRenderMode->Check( FRAME_MONO, true );
      else if( render_mode == "QUAD_BUFFERED_STEREO" )
        stereoRenderMode->Check( FRAME_QUADBUFFERED, true );
      else if( render_mode == "HORIZONTAL_SPLIT" )
        stereoRenderMode->Check( FRAME_HORZSPLIT, true );
      else if( render_mode == "HORIZONTAL_SPLIT_KEEP_RATIO" )
        stereoRenderMode->Check( FRAME_HORZSPLITKEEPASPECT, true );
      else if( render_mode == "VERTICAL_SPLIT" )
        stereoRenderMode->Check( FRAME_VERTSPLIT, true );
      else if( render_mode == "VERTICAL_SPLIT_KEEP_RATIO" )
        stereoRenderMode->Check( FRAME_VERTSPLITKEEPASPECT, true );
      else if( render_mode == "HORIZONTAL_INTERLACED" )
        stereoRenderMode->Check( FRAME_HORZINTERLACED, true );
      else if( render_mode == "VERTICAL_INTERLACED" )
        stereoRenderMode->Check( FRAME_VERTINTERLACED, true );
      else if( render_mode == "CHECKER_INTERLACED" )
        stereoRenderMode->Check( FRAME_CHECKERINTERLACED, true );
      else if( render_mode == "VERTICAL_INTERLACED_GREEN_SHIFT" )
        stereoRenderMode->Check( FRAME_SHARPDISPLAY, true );
      else if( render_mode == "RED_BLUE_STEREO" )
        stereoRenderMode->Check( FRAME_REDBLUE, true );
      else if( render_mode == "RED_GREEN_STEREO" )
        stereoRenderMode->Check( FRAME_REDGREEN, true );
      else if( render_mode == "RED_CYAN_STEREO" )
        stereoRenderMode->Check( FRAME_REDCYAN, true );
      else if( render_mode == "HDMI_FRAME_PACKED_720P" )
        stereoRenderMode->Check( FRAME_HDMI720P, true );
      else if( render_mode == "HDMI_FRAME_PACKED_1080P" )
        stereoRenderMode->Check( FRAME_HDMI1080P, true );
#ifdef HAVE_DX9
      else if( render_mode == "NVIDIA_3DVISION" )
        stereoRenderMode->Check( FRAME_NVIDIA_3DVISION, true );
#endif
    }

    tree_view_dialog->showEntireSceneAsTree( true );
  } 
    catch (const Exception::QuitAPI &e) {
      throw e;
    }
    catch (const Exception::H3DException &e) {
    viewpoint.reset( new Viewpoint );
    stringstream org,reformated;
    org << e;
    insertLineBreak(org,reformated,100);
    wxMessageBox( wxString(reformated.str().c_str(),wxConvUTF8), wxT("Error"),
                  wxOK | wxICON_EXCLAMATION);
#ifdef WIN32
    SetErrorMode( old_error_mode );
#endif
    return false;
  }

  global_settings.reset( GlobalSettings::getActive() );
  if( !global_settings.get() ) {
    global_settings.reset( new GlobalSettings );
		// Set x3dROUTESendsEvent
		global_settings->x3dROUTESendsEvent->setValue( advancedMenu->IsChecked( FRAME_ROUTESENDSEVENTS ) );
  } else {
		advancedMenu->Check( FRAME_ROUTESENDSEVENTS, global_settings->x3dROUTESendsEvent->getValue() );
	}

  // Set CollisionOptions or update page.
  CollisionOptions * co = 0;
  global_settings->getOptionNode( co );
  wxMenuItem * col_item = navigationMenu->
    FindItemByPosition( navigationMenu->GetMenuItemCount() - 4 );

  if( co ) {
    avatar_collision = co->avatarCollision->getValue();
    col_item->Check( avatar_collision );
  } else {
    co = new CollisionOptions;
    co->avatarCollision->
      setValue( avatar_collision );
    global_settings->options->push_back( co );
  }

  // Set DebugOptions or update page.
  DebugOptions * debug_options = 0;
  global_settings->getOptionNode( debug_options );
  if( debug_options ) {
    settings->draw_triangles_box->
      SetValue( debug_options->drawHapticTriangles->getValue() );
    settings->draw_bound_box->
      SetValue( debug_options->drawBound->getValue() );
    H3DInt32 draw_bound_tree = debug_options->drawBoundTree->getValue();
    if( draw_bound_tree >= 0 ) {
      settings->draw_tree_box->SetValue( true );
      settings->boundTree = true;
      settings->depth_spin->SetValue( draw_bound_tree );
      settings->treeDepth = draw_bound_tree;
    } else {
      settings->draw_tree_box->SetValue( false );
      settings->boundTree = false;
      settings->depth_spin->SetValue( 0 );
      settings->treeDepth = 0;
    }
    SaveDebugOptions(false);
  } else {
    debug_options = new DebugOptions;
    debug_options->drawHapticTriangles->
      setValue( settings->draw_triangles_box->GetValue() );
    debug_options->drawBound->
      setValue( settings->draw_bound_box->GetValue() );
    if( settings->draw_tree_box->GetValue() ) {
      debug_options->drawBoundTree->
        setValue( settings->treeDepth );
    }
    global_settings->options->push_back( debug_options );
  }

  // Set GraphicsCachingOptions or update page.
  GraphicsCachingOptions * gco = 0;
  global_settings->getOptionNode( gco );

  if( gco ) {
    settings->display_list_checkbox->SetValue( gco->useCaching->getValue() );
    settings->only_geoms_checkbox->
      SetValue( gco->cacheOnlyGeometries->getValue() );
    settings->caching_delay_spin->SetValue( gco->cachingDelay->getValue() );
    SaveGraphicsCachingOptions(false);
  } else {
    gco = new GraphicsCachingOptions;
    gco->useCaching->setValue( settings->display_list_checkbox->GetValue() );
    gco->cacheOnlyGeometries->
      setValue( settings->only_geoms_checkbox->GetValue() );
    gco->cachingDelay->setValue( settings->caching_delay_spin->GetValue() );
    global_settings->options->push_back( gco );
  }

  // Set HapticsOptions or update page.
  HapticsOptions * ho = 0;
  global_settings->getOptionNode( ho );

  if( ho ) {
    string touchable_face = ho->touchableFace->getValue();
    if( touchable_face == WxFrameInternals::as_graphics )
      settings->face_choice->
        SetStringSelection( WxFrameInternals::wx_as_graphics );
    else if( touchable_face == WxFrameInternals::front_and_back )
      settings->face_choice->
        SetStringSelection( WxFrameInternals::wx_front_and_back );
    else if( touchable_face == WxFrameInternals::front )
      settings->face_choice->SetStringSelection( WxFrameInternals::wx_front );
    else if( touchable_face == WxFrameInternals::back )
      settings->face_choice->SetStringSelection( WxFrameInternals::wx_back );

    string dynamic_mode = ho->dynamicMode->getValue();
    if( dynamic_mode == WxFrameInternals::transform_changed )
      settings->dynamic_mode_choice->
        SetStringSelection( WxFrameInternals::wx_transform_changed );
    else if( dynamic_mode == WxFrameInternals::never )
      settings->dynamic_mode_choice->
        SetStringSelection( WxFrameInternals::wx_never );
    else if( dynamic_mode == WxFrameInternals::always )
      settings->dynamic_mode_choice->
        SetStringSelection( WxFrameInternals::wx_always );

    stringstream max_dist;
    max_dist  <<  ho->maxDistance->getValue();
    settings->max_distance_text->
      ChangeValue( wxString( max_dist.str().c_str(), wxConvUTF8 ) );
    stringstream laf;
    laf << ho->lookAheadFactor->getValue();
    settings->look_ahead_text->
      ChangeValue( wxString( laf.str().c_str(), wxConvUTF8 ) );
    settings->use_bound_tree_checkbox->
      SetValue( ho->useBoundTree->getValue() );
    settings->interpolate_force_effects_checkbox->
      SetValue( ho->interpolateForceEffects->getValue() );
    SaveHapticsOptions(false);
  } else {
    ho = new HapticsOptions;
    int i = settings->face_choice->GetSelection();
    if( i == 0 ) ho->touchableFace->setValue( WxFrameInternals::as_graphics );
    else if( i == 1 ) ho->touchableFace->setValue(
                        WxFrameInternals::front_and_back );
    else if( i == 2 ) ho->touchableFace->setValue( WxFrameInternals::front );
    else if( i == 3 ) ho->touchableFace->setValue( WxFrameInternals::back );

    i = settings->dynamic_mode_choice->GetSelection();
    if( i == 0 ) ho->dynamicMode->setValue( 
      WxFrameInternals::transform_changed );
    else if( i == 1 ) ho->dynamicMode->setValue( WxFrameInternals::never );
    else if( i == 2 ) ho->dynamicMode->setValue( WxFrameInternals::always );

    ho->maxDistance->setValueFromString(
                    toStr( settings->max_distance_text->GetValue() ) );
    ho->lookAheadFactor->setValueFromString(
                    toStr( settings->look_ahead_text->GetValue() ) );
    ho->useBoundTree->
      setValue( settings->use_bound_tree_checkbox->GetValue() );
    ho->interpolateForceEffects->
      setValue( settings->interpolate_force_effects_checkbox->GetValue() );
    global_settings->options->push_back( ho );
  }

  // Set GeometryBoundTreeOptions or update page.
  GeometryBoundTreeOptions * gbto = 0;
  global_settings->getOptionNode( gbto );

  if( gbto ) {
    string bound_type = gbto->boundType->getValue();
    if( bound_type == WxFrameInternals::str_OBB ) {
      settings->bound_choice->SetStringSelection( WxFrameInternals::wx_OBB );
    } else if( bound_type == WxFrameInternals::str_AABB ) {
      settings->bound_choice->SetStringSelection( WxFrameInternals::wx_AABB );
    } else if( bound_type == WxFrameInternals::str_SPHERE ) {
      settings->bound_choice->SetStringSelection(
        WxFrameInternals::wx_SPHERE );
    }
    settings->max_triangles_spin->
      SetValue( gbto->maxTrianglesInLeaf->getValue() );
    SaveGeometryBoundTreeOptions(false);
  } else {
    gbto = new GeometryBoundTreeOptions;
    wxString bound_choice = settings->bound_choice->GetStringSelection();
    if( bound_choice == WxFrameInternals::wx_OBB ) {
      gbto->boundType->setValue( WxFrameInternals::str_OBB );
    } else if( bound_choice == WxFrameInternals::wx_AABB ) {
      gbto->boundType->setValue( WxFrameInternals::str_AABB );
    } else if( bound_choice == WxFrameInternals::wx_SPHERE ) {
      gbto->boundType->setValue( WxFrameInternals::str_SPHERE );
    }
    gbto->maxTrianglesInLeaf->
      setValue( settings->max_triangles_spin->GetValue() );
    global_settings->options->push_back( gbto );
  }

  // Set OpenHapticsOptions or update page.
#ifdef WIN32
  if( DynamicLibrary::load( "hd.dll" ) &&
      DynamicLibrary::load( "hl.dll" ) ) {
#endif
  OpenHapticsOptions *oho = 0;
  global_settings->getOptionNode( oho );

  if( oho ) {
    settings->adaptive_viewport->
      SetValue( oho->useAdaptiveViewport->getValue() );
    settings->haptic_camera->
      SetValue( oho->useHapticCameraView->getValue() );
    settings->full_geom_render->
      SetValue( oho->forceFullGeometryRender->getValue() );
    string gl_shape = oho->GLShape->getValue();
    if( gl_shape == WxFrameInternals::str_FEEDBACK ) {
      settings->shape_choice->SetStringSelection(
        WxFrameInternals::wx_FEEDBACK );
    } else if( gl_shape == WxFrameInternals::str_DEPTH ) {
      settings->shape_choice->SetStringSelection( WxFrameInternals::wx_DEPTH );
    } else if( gl_shape == WxFrameInternals::str_CUSTOM ) {
      settings->shape_choice->SetStringSelection(
        WxFrameInternals::wx_CUSTOM );
    }
    SaveOpenHapticsOptions(false);
  } else {
    oho = new OpenHapticsOptions;
    oho->useAdaptiveViewport->
      setValue( settings->adaptive_viewport->GetValue() );
    oho->useHapticCameraView->
      setValue( settings->haptic_camera->GetValue() );
    oho->forceFullGeometryRender->
      setValue( settings->full_geom_render->GetValue() );
    wxString shape_choice = settings->shape_choice->GetStringSelection();
    if( shape_choice == WxFrameInternals::wx_FEEDBACK ) {
      oho->GLShape->setValue( WxFrameInternals::str_FEEDBACK );
    } else if( shape_choice == WxFrameInternals::wx_DEPTH ) {
      oho->GLShape->setValue( WxFrameInternals::str_DEPTH );
    } else if( shape_choice == WxFrameInternals::wx_CUSTOM ) {
      oho->GLShape->setValue( WxFrameInternals::str_CUSTOM );
    }

    global_settings->options->push_back( oho );
  }
#ifdef WIN32
  }
#endif

  // Set StereoInfo or update page.
  StereoInfo * si = StereoInfo::getActive();

  if( si ) {
    stringstream foc_dist;
    foc_dist  <<  si->focalDistance->getValue();
    settings->focal_distance_text->
      ChangeValue( wxString( foc_dist.str().c_str(), wxConvUTF8 ) );
    stringstream intoc_dist;
    intoc_dist << si->interocularDistance->getValue();
    settings->interocular_distance_text->
      ChangeValue( wxString( intoc_dist.str().c_str(), wxConvUTF8 ) );
    SaveStereoInfo(false);
  } else {
    si = new StereoInfo;
    stereo_info.reset( si );
    si->focalDistance->setValueFromString(
                    toStr( settings->focal_distance_text->GetValue() ) );
    si->interocularDistance->setValueFromString(
                    toStr( settings->interocular_distance_text->GetValue() ) );
  }

  H3DDisplayListObject::DisplayList::rebuildAllDisplayLists();
  settings->on_cancel_rebuild_displaylist = false;
#ifdef WIN32
  SetErrorMode( old_error_mode );
#endif
  return true;
}

//Clear data when closing file
void WxFrame::clearData () {
  scene->setSceneRoot( NULL );
	h3dConfig->SetPath(wxT("/Settings"));
  bool new_viewpoint_on_load = true;
  h3dConfig->Read(wxT("new_viewpoint_on_load"), &new_viewpoint_on_load);
	if( new_viewpoint_on_load ) {
		X3DViewpointNode::ViewpointList viewpoint_list = X3DViewpointNode::getAllViewpoints();
		for( X3DViewpointNode::ViewpointList::iterator i = viewpoint_list.begin();
				 i != viewpoint_list.end(); i++ ) {
			(*i)->set_bind->setValue( false );
		}
	}
  viewpoint.reset( new Viewpoint );

  DestroyViewpointsMenu();

  //Delete items from navigation menu & disconnect events
  for (int j = 0; j < navTypeCount; j++) {
    navigationMenu->Destroy(FRAME_NAVIGATION + j);
    Disconnect(FRAME_NAVIGATION + j,wxEVT_COMMAND_MENU_SELECTED,
               wxCommandEventHandler(WxFrame::ChangeNavigation));
  }

  // Find all separators and destroy them, if the item is not a separator
  // then just remove it.
  while( navigationMenu->GetMenuItemCount() != 0 ) {
    wxMenuItem * temp_menu_item = navigationMenu->FindItemByPosition( 0 );
    if( temp_menu_item != navigationDevices ) {
      navigationMenu->Destroy( temp_menu_item->GetId() );
    } else {
      navigationMenu->Remove( temp_menu_item->GetId() );
    }
  }

	global_settings->set_bind->setValue( false );
  global_settings.reset( 0 );
  stereo_info.reset( 0 );
}


//Open a file
void WxFrame::OnOpenFileURL(wxCommandEvent & event) {
   auto_ptr< wxTextEntryDialog > text_dialog( new wxTextEntryDialog ( this,
                             wxT("Enter the location of the file here"),
                             wxT("Open file from UR"),
                             wxT("")) );
   if( text_dialog->ShowModal() == wxID_OK ) {
     string s(text_dialog->GetValue().mb_str());
     clearData();
     loadFile( s );
     SetStatusText( wxString(s.c_str(),wxConvUTF8), 1 );
     SetStatusText( wxT("URL loaded"), 0 );
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
#ifdef WIN32
    wxString wx_filename = currentPath + wxT("\\") + currentFilename;
#else
    wxString wx_filename = currentPath + wxT("/") + currentFilename;
#endif
    string filename(wx_filename.mb_str());
    clearData();
    loadFile( filename );
    recentFiles->AddFileToHistory ( wx_filename );
    SetStatusText(wxT("File loaded"), 0);
    SetStatusText( wxString(filename.c_str(),wxConvUTF8), 1 );
  }
}

//Open a file from file history
void WxFrame::OnMRUFile(wxCommandEvent & event)
{
  wxString filename(recentFiles->GetHistoryFile(event.GetId() - wxID_FILE1));
  if(!filename.IsEmpty()) {
#ifdef WIN32
    SetCurrentFilename(filename.AfterLast('\\') );
    SetCurrentPath(filename.BeforeLast('\\') );
    wxString wx_filename = currentPath + wxT("\\") + currentFilename;
#else
    SetCurrentFilename(filename.AfterLast('/') );
    SetCurrentPath(filename.BeforeLast('/') );
    wxString wx_filename = currentPath + wxT("/") + currentFilename;
#endif
    string filename(wx_filename.mb_str());
    clearData();
    loadFile( filename );
    SetStatusText(wxT("File loaded"), 0);
    SetStatusText(wxString(lastOpenedFilepath.c_str(),wxConvUTF8), 1);
    // remove and add back, to make the file jump on top
    recentFiles->RemoveFileFromHistory( event.GetId() - wxID_FILE1 );
    recentFiles->AddFileToHistory ( wx_filename );

  }
}

//Close File
void WxFrame::OnCloseFile(wxCommandEvent & event) {
  if (lastOpenedFilepath.empty()) {
    return;
  }
  lastOpenedFilepath.clear();
  //clearData();
  scene->setSceneRoot( NULL );
  //t->children->clear();
  tree_view_dialog->clearTreeView();
  if( !Viewpoint::getActive() )
    viewpoint.reset( new Viewpoint );
  SetStatusText(wxT("File closed"), 0);
  SetStatusText(wxT(""),1);

  //Disable menus again
  menuBar->EnableTop(2, false);
  menuBar->EnableTop(3, false);

  //Disable items in rendererMenu again
  rendererMenu->Enable(FRAME_CHOOSERENDERER, false);
  rendererMenu->Enable(FRAME_RENDERMODE, false);
  rendererMenu->Enable(FRAME_STEREORENDERMODE, false);
}

void WxFrame::OnChooseDir(wxCommandEvent & event) {
  auto_ptr< wxDirDialog >  d( new wxDirDialog( this, 
                                               wxT("Choose a directory"),
                                               GetCurrentPath(), 
                                               0, 
                                               wxDefaultPosition ) );
  if (d->ShowModal() == wxID_OK)
  {
    SetCurrentPath(d->GetPath());
  }
}

//About dialog
void WxFrame::OnAbout(wxCommandEvent & event)
{ wxString t = wxTheApp->GetAppName();

  t.append( AUTHOR );
  
  wxMessageDialog aboutDialog ( this, t, ABOUT, wxOK);
  aboutDialog.ShowModal();
}

//Help event
//void WxFrame::OnHelp(wxCommandEvent & event)
//{
//
//}

//Idle event
void WxFrame::OnIdle(wxIdleEvent &event) {
  static bool flag;
  TimeStamp now;
  if ( now - last_viewmenu_update > 0.5 && 
       (X3DViewpointNode::viewpointsChanged() || 
       ViewpointGroup::viewpointGroupsChanged()) ) {
    list< Node * > v = GetTopLevelViews();
    DestroyViewpointsMenu();
    BuildViewpointsMenu( v );
  }
}

//Fullscreen mode
void WxFrame::OnFullscreen (wxCommandEvent & event)
{
  SetFullscreen( true );
}

void WxFrame::SetFullscreen( bool fullscreen ) {
  if( glwindow->fullscreen->getValue() != fullscreen ) {
    if( fullscreen ) {
      WxFrame::ShowFullScreen(true, wxFULLSCREEN_NOMENUBAR | 
			      wxFULLSCREEN_NOTOOLBAR | wxFULLSCREEN_NOBORDER | 
			      wxFULLSCREEN_NOCAPTION | wxFULLSCREEN_NOSTATUSBAR);
      glwindow->fullscreen->setValue( true );
      rendererMenu->Check(FRAME_FULLSCREEN, true);
      SetStatusText(wxT("Press F11 to exit fullscreen mode"), 0);
      SetStatusText(wxT("Viewing in Fullscreen"), 1);
    } else {
      WxFrame::ShowFullScreen(false, wxFULLSCREEN_ALL);
      glwindow->fullscreen->setValue( false );
      rendererMenu->Check(FRAME_FULLSCREEN, false);
      SetStatusText(currentFilename, 0);
      SetStatusText(currentPath, 1);
    }
  }
}

//Restore from fullscreen
void WxFrame::ToggleFullscreen(wxCommandEvent & event)
{
  SetFullscreen( !glwindow->fullscreen->getValue() );
}

void WxFrame::MirrorScene(wxCommandEvent & event)
{
  lastmirror = glwindow->mirrored->getValue();
  glwindow->mirrored->setValue(!lastmirror);
  if ( glwindow->mirrored->getValue() ) {
    SetStatusText(wxT("Scene mirrored in Y"), 0);
  }
  else {
    SetStatusText(currentFilename, 0);
  }
}

//Render Mode
void WxFrame::StereoRenderMode(wxCommandEvent & event)
{
  std::string renderMode;
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
    case FRAME_HORZSPLITKEEPASPECT:
      renderMode = "HORIZONTAL_SPLIT_KEEP_RATIO";
      break;
    case FRAME_VERTSPLIT:
      renderMode = "VERTICAL_SPLIT";
      break;
    case FRAME_VERTSPLITKEEPASPECT:
      renderMode = "VERTICAL_SPLIT_KEEP_RATIO";
      break;
    case FRAME_HORZINTERLACED:
      renderMode = "HORIZONTAL_INTERLACED";
      break;
    case FRAME_VERTINTERLACED:
      renderMode = "VERTICAL_INTERLACED";
      break;
    case FRAME_CHECKERINTERLACED:
      renderMode = "CHECKER_INTERLACED";
      break;
    case FRAME_SHARPDISPLAY:
      renderMode = "VERTICAL_INTERLACED_GREEN_SHIFT";
      break;
    case FRAME_REDBLUE:
      renderMode = "RED_BLUE_STEREO";
      break;
    case FRAME_REDGREEN:
      renderMode = "RED_GREEN_STEREO";
      break;
    case FRAME_REDCYAN:
      renderMode = "RED_CYAN_STEREO";
      break;
    case FRAME_HDMI720P:
      renderMode = "HDMI_FRAME_PACKED_720P";
      break;
    case FRAME_HDMI1080P:
      renderMode = "HDMI_FRAME_PACKED_1080P";
      break;
    case FRAME_NVIDIA_3DVISION:
      renderMode = "NVIDIA_3DVISION";
      break;
  }
  glwindow->renderMode->setValue( renderMode.c_str() );
  // This is neeeded to avoid color changes when switching to stereo render
  // modes if the file was loaded with mono render mode.
  H3DDisplayListObject::DisplayList::rebuildAllDisplayLists();
}


//Render Mode
void WxFrame::RenderMode(wxCommandEvent & event)
{
  GlobalSettings *settings = GlobalSettings::getActive();
  if( settings ) {
    if( event.GetId() == FRAME_RENDERMODE_DEFAULT ) 
      settings->renderMode->setValue( "DEFAULT" );
    else if( event.GetId() == FRAME_RENDERMODE_FILLED ) 
      settings->renderMode->setValue( "FILLED" );
    else if( event.GetId() == FRAME_RENDERMODE_WIREFRAME ) 
      settings->renderMode->setValue( "WIREFRAME" );
    else if( event.GetId() == FRAME_RENDERMODE_POINTS ) 
      settings->renderMode->setValue( "POINTS" );
  }
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
#ifdef WIN32
      if( DynamicLibrary::load( "hd.dll" ) &&
          DynamicLibrary::load( "hl.dll" ) ) {
#endif
        for (NodeVector::const_iterator nv = allDevices.begin(); 
          nv != allDevices.end(); nv++) {
            static_cast < H3DHapticsDevice *> 
              (*nv)->hapticsRenderer->setValue(new OpenHapticsRenderer);
        }
#ifdef WIN32
      }
#endif
      break;
    case FRAME_CHAI3D:
#ifdef HAVE_CHAI3D
        for (NodeVector::const_iterator nv = allDevices.begin(); 
              nv != allDevices.end(); nv++) {
          static_cast < H3DHapticsDevice *> 
            (*nv)->hapticsRenderer->setValue(new Chai3DRenderer);
        }
#endif
      break;
    case FRAME_GODOBJECT:
      for (NodeVector::const_iterator nv = allDevices.begin(); 
             nv != allDevices.end(); nv++) {
          static_cast < H3DHapticsDevice *> (*nv)->
              hapticsRenderer->setValue(new GodObjectRenderer);
      }
      break;
    case FRAME_RUSPINI:
        for (NodeVector::const_iterator nv = allDevices.begin(); 
                 nv != allDevices.end(); nv++) {
            static_cast < H3DHapticsDevice *> (*nv)->
              hapticsRenderer->setValue(new RuspiniRenderer);
        }
        setProxyRadius( settings->getProxyRadius() );
      break;
  }
}

//Toggle haptics
//void WxFrame::ToggleHaptics (wxCommandEvent & event) {
//  for (NodeVector::const_iterator nv = allDevices.begin(); 
//        nv != allDevices.end(); nv++) {
//    if (lastDeviceStatus) {
//      static_cast < H3DHapticsDevice *> (*nv)->disableDevice();
//      lastDeviceStatus = false;
//    }
//    else {
//      static_cast < H3DHapticsDevice *> (*nv)->enableDevice();
//      lastDeviceStatus = true;
//    }
//  }
//}

//Show console event
void WxFrame::ShowConsole(wxCommandEvent & event)
{
  if (!the_console->Show()) {
    // already shown, bring it up
    the_console->SetFocus();
  }
}

//Show console event
void WxFrame::ShowTreeView(wxCommandEvent & event)
{
  if (!tree_view_dialog->Show()) {
    tree_view_dialog->SetFocus();
  }
}

//Show program settings window
void WxFrame::ShowProgramSettings(wxCommandEvent & event)
{
#ifdef HAVE_WXPROPGRID
  if (!program_settings_dialog->Show()) {
    program_settings_dialog->SetFocus();
  }
#endif
}

// Save option
void WxFrame::OnKeepViewpointOnLoadCheck(wxCommandEvent & event)
{
	h3dConfig = wxConfigBase::Get();
  h3dConfig->SetPath(wxT("/Settings"));
  h3dConfig->Write(wxT("new_viewpoint_on_load"), event.IsChecked());
}

void WxFrame::OnRouteSendsEventsCheck(wxCommandEvent & event)
{
	h3dConfig = wxConfigBase::Get();
  h3dConfig->SetPath(wxT("/Settings"));
  h3dConfig->Write(wxT("route_sends_events"), event.IsChecked());
	global_settings->x3dROUTESendsEvent->setValue( event.IsChecked() );
}

void WxFrame::ShowPluginsDialog(wxCommandEvent & event)
{
  plugins_dialog->Show();
}

void WxFrame::ShowFrameRate(wxCommandEvent & event)
{
  //frameRates = new FrameRateDialog( this );
  //frameRates->updateMenuItems();

  // set labels to make sure they are the right size when shown
  // (otherwise some numbers may not be displayed)
  frameRates->graphics_rate->SetLabel( wxT("100") );
  frameRates->haptics_rate->SetLabel( wxT("1000") );
  frameRates->haptics_time->SetLabel( wxT("100") );
  if (!frameRates->Show()) {
    frameRates->SetFocus();
  }
}

//Change Viewpoint
void WxFrame::ChangeViewpoint (wxCommandEvent & event)
{
  int selection = event.GetId();
  //Default viewpoint
  if ( X3DViewpointNode::getAllViewpoints().size() <= 1) {
    viewpointMenu->Check(selection, true);
  }
  else {
    itemIdViewpointMap[selection]->set_bind->setValue(true);
    current_viewpoint = itemIdViewpointMap[selection];
    viewpointMenu->Check(current_viewpoint_id, false);
    viewpointMenu->Check(selection, true);
    current_viewpoint_id = selection;
  }
}

//Reset Active Viewpoint
void WxFrame::ResetViewpoint(wxCommandEvent & event)
{
  if (current_viewpoint) {
      current_viewpoint->relOrn->setValue( Rotation() );
      current_viewpoint->relPos->setValue( Vec3f() );
  }
}

void WxFrame::ChangeCollision (wxCommandEvent & event) {
  if( global_settings.get() ) {
    CollisionOptions * col_opt = 0;
    global_settings->getOptionNode( col_opt );
    if( col_opt ) {
      avatar_collision = event.IsChecked();
      col_opt->avatarCollision->setValue( avatar_collision );
    }
  }
}

void WxFrame::OnSpeed( wxCommandEvent & event ) {
  speed_slider->Show();
}

//Change Navigation
void WxFrame::ChangeNavigation (wxCommandEvent & event)
{
  int selection = event.GetId();
  if (mynav) {
    mynav->setNavType( toStr( ( navigationMenu->GetLabelText( selection ) ) ));
  }
  else {
    glwindow->default_nav = toStr( navigationMenu->GetLabelText( selection ) );
  }
}

// 09.10.14 Reload
void WxFrame::OnReload (wxCommandEvent & event)
{
  if (lastOpenedFilepath.empty())
    return;
  clearData();
  loadFile(lastOpenedFilepath);
  SetStatusText(wxT("Reloaded"), 0);
  SetStatusText(wxString(lastOpenedFilepath.c_str(),wxConvUTF8), 1);
}

//Exit via menu
void WxFrame::OnExit (wxCommandEvent & event)
{
  SaveCollisionOptions();
  wxCommandEvent fake_event;
  // Close file so that printout in possible destructors does not
  // cause a crash if done after the console is destroyed.
  OnCloseFile( fake_event );
  Close(true);
}

//Exit via window manager
void WxFrame::OnWindowExit (wxCloseEvent & event) 
{
  SaveCollisionOptions();
  wxCommandEvent fake_event;
  // Close file so that printout in possible destructors does not
  // cause a crash if done after the console is destroyed.
  OnCloseFile( fake_event );
  Destroy();
  SaveMRU();
  delete wxConfigBase::Set((wxConfigBase *) NULL);
}

///*******************Standard trivial functions*********************/
////Get current filename
//wxString WxFrame::GetCurrentFilename()
//{
// return currentFilename;
//}

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
  h3dConfig->SetPath(wxT("/Recent/Count"));
  h3dConfig->Write(wxT("Count"), (long)recentFiles->GetCount());

  //Store each individual file info
  h3dConfig->SetPath(wxT("/Recent/List"));
  for (int i = 0; i < (int)recentFiles->GetCount(); i++) {
    wxString entry = wxString (wxT("")) << i;
    h3dConfig->Write(entry, recentFiles->GetHistoryFile(i));
  }
}

// Save settings for next initialization
void WxFrame::SaveSettings( bool to_config ) {

  SaveDebugOptions( to_config );
  SaveGeometryBoundTreeOptions( to_config );
  SaveGraphicsCachingOptions( to_config );
  SaveHapticsOptions( to_config );
  SaveOpenHapticsOptions( to_config );
  SaveStereoInfo( to_config );
  SaveRuspiniSettings( to_config );

}

void WxFrame::SaveCollisionOptions() {
  // Save collision options
  if( global_settings.get() ) {
    CollisionOptions *co = 0;
    global_settings->getOptionNode( co );
    if( co ) {
      h3dConfig = wxConfigBase::Get();
      h3dConfig->SetPath( wxT("/Settings/Collision") );
      h3dConfig->Write( wxT("avatar_collision"), avatar_collision );
    }
  }
}

void WxFrame::SaveDebugOptions( bool to_config ) {
  // Save debug options
  DebugOptions *dgo = NULL;
  global_settings->getOptionNode( dgo );
  if (dgo) {
    bool draw_bound = dgo->drawBound->getValue();
    int draw_tree = dgo->drawBoundTree->getValue();
    bool draw_triangles = dgo->drawHapticTriangles->getValue();
    if( to_config ) {
      h3dConfig = wxConfigBase::Get();
      h3dConfig->SetPath(wxT("/Settings/Debug"));
      h3dConfig->Write(wxT("draw_bound"), draw_bound );
      h3dConfig->Write(wxT("draw_tree"), draw_tree );
      h3dConfig->Write(wxT("draw_triangles"), draw_triangles );
    } else {
      non_conf_opt.draw_bound = draw_bound;
      non_conf_opt.draw_tree = draw_tree;
      non_conf_opt.draw_triangles = draw_triangles;
    }
  }
}

void WxFrame::SaveGeometryBoundTreeOptions( bool to_config ) {
  // Save geometry bound tree options
  GeometryBoundTreeOptions *gbto = NULL;
  global_settings->getOptionNode (gbto);
  if(gbto) {
    string bound_type = gbto->boundType->getValue();
    int max_tri_leaf = gbto->maxTrianglesInLeaf->getValue();
    if( to_config ) {
      h3dConfig = wxConfigBase::Get();
      h3dConfig->SetPath( wxT("/Settings/GeometryBoundTree") );
      h3dConfig->Write( wxT("boundType"),
                        wxString(bound_type.c_str(),wxConvUTF8) );
      h3dConfig->Write( wxT("maxTrianglesInLeaf"), max_tri_leaf );
    } else {
      non_conf_opt.bound_type = bound_type;
      non_conf_opt.max_triangles_in_leaf = max_tri_leaf;
    }
  }
}

void WxFrame::SaveGraphicsCachingOptions( bool to_config ) {
  // Save graphics caching options
  GraphicsCachingOptions *gco = NULL;
  global_settings->getOptionNode (gco);
  if(gco) {
    bool use_caching = gco->useCaching->getValue();
    bool cache_only_geometries = gco->cacheOnlyGeometries->getValue();
    int caching_delay = gco->cachingDelay->getValue();
    if( to_config ) {
      h3dConfig = wxConfigBase::Get();
      h3dConfig->SetPath(wxT("/Settings/GraphicsCaching"));
      h3dConfig->Write( wxT("useCaching"), use_caching );
      h3dConfig->Write( wxT("cacheOnlyGeometries"), cache_only_geometries );
      h3dConfig->Write( wxT("cachingDelay"), caching_delay );
    } else {
      non_conf_opt.use_caching = use_caching;
      non_conf_opt.cache_only_geometries = cache_only_geometries;
      non_conf_opt.caching_delay = caching_delay;
    }
  }
}

void WxFrame::SaveHapticsOptions( bool to_config ) {
  // Save haptics options
  HapticsOptions *ho = NULL;
  global_settings->getOptionNode (ho);
  if(ho) {
    string touchable_face = ho->touchableFace->getValue();
    string dynamic_mode = ho->dynamicMode->getValue();
    float max_distance = ho->maxDistance->getValue();
    float look_ahead_factor = ho->lookAheadFactor->getValue();
    bool use_bound_tree = ho->useBoundTree->getValue();
    bool interpolate_force_effects = ho->interpolateForceEffects->getValue();
    if( to_config ) {
      h3dConfig = wxConfigBase::Get();
      h3dConfig->SetPath(wxT("/Settings/Haptics"));
      h3dConfig->Write( wxT("touchableFace"), 
                        wxString( touchable_face.c_str(), wxConvUTF8 ) );
      h3dConfig->Write( wxT("dynamicMode"), 
                        wxString( dynamic_mode.c_str(), wxConvUTF8 ) );
      h3dConfig->Write( wxT("maxDistance"), max_distance );
      h3dConfig->Write( wxT("lookAheadFactor"), look_ahead_factor );
      h3dConfig->Write( wxT("useBoundTree"), use_bound_tree );
      h3dConfig->Write( wxT("interpolateForceEffects"),
                        interpolate_force_effects );
    } else {
      non_conf_opt.touchable_face = touchable_face;
      non_conf_opt.dynamic_mode = dynamic_mode;
      non_conf_opt.max_distance = max_distance;
      non_conf_opt.look_ahead_factor = look_ahead_factor;
      non_conf_opt.use_bound_tree = use_bound_tree;
      non_conf_opt.interpolate_force_effects = interpolate_force_effects;
    }
  }
}

void WxFrame::SaveOpenHapticsOptions( bool to_config ) {
  // Save openhaptics options
  OpenHapticsOptions *oho = NULL;
  global_settings->getOptionNode (oho);
  if(oho) {
    bool use_adaptive_viewport = oho->useAdaptiveViewport->getValue();
    bool use_haptic_camera_view = oho->useHapticCameraView->getValue();
    string gl_shape = oho->GLShape->getValue();
    bool force_full_geometry_render = oho->forceFullGeometryRender->getValue();
    if( to_config ) {
      h3dConfig = wxConfigBase::Get();
      h3dConfig->SetPath(wxT("/Settings/OpenHaptics"));
      h3dConfig->Write( wxT("useAdaptiveViewport"), use_adaptive_viewport );
      h3dConfig->Write( wxT("useHapticCameraView"), use_haptic_camera_view );
      h3dConfig->Write( wxT("GLShape"),
                        wxString( gl_shape.c_str(), wxConvUTF8 ) );
      h3dConfig->Write( wxT("forceFullGeometryRender"),
                        force_full_geometry_render );
    } else {
      non_conf_opt.use_adaptive_viewport = use_adaptive_viewport;
      non_conf_opt.use_haptic_camera_view = use_haptic_camera_view;
      non_conf_opt.gl_shape = gl_shape;
      non_conf_opt.force_full_geometry_render = force_full_geometry_render;
    }
  }
}

void WxFrame::SaveStereoInfo( bool to_config ) {
  // Save StereoInfo
  if( stereo_info.get() ) {
    float focal_distance = stereo_info->focalDistance->getValue();
    float interocular_distance = stereo_info->interocularDistance->getValue();
    if( to_config ) {
      h3dConfig = wxConfigBase::Get();
      h3dConfig->SetPath( wxT("/Settings/StereoInfo") );
      h3dConfig->Write( wxT("focalDistance"), focal_distance );
      h3dConfig->Write( wxT("interocularDistance"), interocular_distance );
    } else {
      non_conf_opt.focal_distance = focal_distance;
      non_conf_opt.interocular_distance = interocular_distance;
    }
  }
}

void WxFrame::SaveRuspiniSettings( bool to_config ) {
  // Save proxy radius
  if( to_config ) {
    h3dConfig = wxConfigBase::Get();
    h3dConfig->SetPath( wxT("/Settings/Ruspini") );
    h3dConfig->Write( wxT("proxyRadius"), settings->getProxyRadius() );
  } else {
    non_conf_opt.proxy_radius = settings->getProxyRadius();
  }
}

void WxFrame::LoadMRU () {
  h3dConfig = wxConfigBase::Get();

  //Load file history from previous sessions
  if( h3dConfig->Exists( wxT("/Recent") ) ) {
    h3dConfig->SetPath(wxT("/Recent/Count"));
    int count;
    h3dConfig->Read(wxT("Count"), &count);
    h3dConfig->SetPath(wxT("/Recent/List"));
    // Going backwards since AddFileToHistory will append to list.
    for (int i = count - 1; i >= 0; i--) {
      wxString entry = wxString(wxT("")) << i;
      if (h3dConfig->Exists(entry)) {
        recentFiles->AddFileToHistory(h3dConfig->Read(entry));
      }
    }
  }
}

void WxFrame::LoadPlugins() {
  if( plugins_dialog->DisablePluginsCheckBox->IsChecked() ) return;

  h3dConfig = wxConfigBase::Get();

 // enumeration variables
  wxString str;
  long dummy;

  h3dConfig->SetPath( wxT("/Plugins") );

#ifdef INCLUDE_PLUGINS_DIR_PLUGINS
  // add all plugins from the plugins directory
  wxFileName executable_path( 
    wxTheApp->GetTraits()->GetStandardPaths().GetExecutablePath() );
    
  wxString executable_dir = executable_path.GetPath();
  
#ifdef H3D_WINDOWS
  wxString plugin_dir = executable_dir + wxT("\\..\\plugins" );
  wxString library_spec = plugin_dir + wxT("\\*.dll");
#else
#ifdef H3D_OSX
  wxString plugin_dir = executable_dir + wxT("/../Plugins");
  wxString library_spec = plugin_dir + wxT("/*.dylib");
#else
  wxString plugin_dir = executable_dir + wxT("/../plugins");
  wxString library_spec = plugin_dir + wxT("/*.so");
#endif
#endif

 if( wxDirExists( plugin_dir ) ) {
   wxString f = wxFindFirstFile( library_spec );
   while ( !f.empty() ) {
     plugins_dialog->addPlugin( f, true );
     f = wxFindNextFile();
   }
 }


#endif  // INCLUDE_PLUGINS_DIR_PLUGINS

  // iterate through all plugins
  h3dConfig->SetPath( wxT("/Plugins") );
  bool  bCont = h3dConfig->GetFirstGroup(str, dummy);
  while ( bCont ) {
    // TODO: add handles somewhere so we can unload libraries if we want to.
    wxString library;
    bool res = h3dConfig->Read( str + wxT("/Library"), &library );    
    if( res && !library.IsEmpty() ) {
     H3DUtil::DynamicLibrary::LIBHANDLE lib = 
       H3DUtil::DynamicLibrary::load( string(library.mb_str()) );
     res = lib != NULL;
    }  
    if( !res ) {
      wxMessageBox( wxT("There was an error when trying to load plugin library \"") + str + wxT("\"" ), 
                    wxT("Error"),
                    wxOK | wxICON_EXCLAMATION);
    }
    bCont = h3dConfig->GetNextGroup(str, dummy);
  }

}

void WxFrame::LoadSettings( bool from_config ) {
  h3dConfig = wxConfigBase::Get();

  // Load settings from previous sessions
  // Load CollisionOptions settings
  if( from_config ) {
    if( h3dConfig->Exists( wxT("/Settings/Collision") ) ) {
      h3dConfig->SetPath( wxT("/Settings/Collision") );
      h3dConfig->Read( wxT("avatar_collision"), &avatar_collision );
    } else {
      CollisionOptions *co = 0;
        global_settings->getOptionNode( co );
      // on clean system
      if( co )
        avatar_collision = co->avatarCollision->getValue();
    }
  }

  // Load Debug options
  DebugOptions *dgo = NULL;
  global_settings->getOptionNode(dgo);
  if(dgo) {
    bool draw_bound;
    bool draw_triangles;
    int draw_tree;
    if( from_config ) {
      if(h3dConfig->Exists(wxT("/Settings/Debug"))) {
        h3dConfig->SetPath(wxT("/Settings/Debug"));
        h3dConfig->Read(wxT("draw_bound"), &draw_bound);
        h3dConfig->Read(wxT("draw_triangles"), &draw_triangles);
        h3dConfig->Read(wxT("draw_tree"), &draw_tree);
      } else {
        // on clean system
        draw_bound = dgo->drawBound->getValue();
        draw_triangles = dgo->drawHapticTriangles->getValue();
        draw_tree = dgo->drawBoundTree->getValue();
      }
    } else {
      draw_bound = non_conf_opt.draw_bound;
      draw_triangles = non_conf_opt.draw_triangles;
      draw_tree = non_conf_opt.draw_tree;
    }

    dgo->drawBound->setValue(draw_bound);
    settings->draw_bound_box->SetValue( draw_bound );
    dgo->drawHapticTriangles->setValue(draw_triangles);
    settings->draw_triangles_box->SetValue( draw_triangles );
    dgo->drawBoundTree->setValue(draw_tree);

    if( draw_tree >= 0 ) {
      settings->draw_tree_box->SetValue( true );
      settings->boundTree = true;
      settings->depth_spin->SetValue( draw_tree );
      settings->treeDepth = draw_tree;
    } else {
      settings->draw_tree_box->SetValue( false );
      settings->boundTree = false;
      settings->depth_spin->SetValue( 0 );
      settings->treeDepth = 0;
    }
  }

  // Load Geometry bound tree options
  GeometryBoundTreeOptions *gbto = NULL;
  global_settings->getOptionNode( gbto );
  if(gbto) {
    string bound_type;
    int max_triangles_in_leaf;
    if( from_config ) {
      if (h3dConfig->Exists( wxT("/Settings/GeometryBoundTree")) ) {
        wxString bound_type_wx;
        h3dConfig->SetPath( wxT("/Settings/GeometryBoundTree") );
        h3dConfig->Read( wxT("boundType"), &bound_type_wx );
        bound_type = toStr( bound_type_wx );
        h3dConfig->Read( wxT("maxTrianglesinLeaf"), &max_triangles_in_leaf );
      } else {
        // on clean system
        bound_type = gbto->boundType->getValue();
        max_triangles_in_leaf = gbto->maxTrianglesInLeaf->getValue();
      }
    } else {
      bound_type = non_conf_opt.bound_type;
      max_triangles_in_leaf = non_conf_opt.max_triangles_in_leaf;
    }
    gbto->boundType->setValue( bound_type );
    if( bound_type == WxFrameInternals::str_OBB ) {
      settings->bound_choice->SetStringSelection( WxFrameInternals::wx_OBB );
    } else if( bound_type == WxFrameInternals::str_AABB ) {
      settings->bound_choice->SetStringSelection( WxFrameInternals::wx_AABB );
    } else if( bound_type == WxFrameInternals::str_SPHERE ) {
      settings->bound_choice->SetStringSelection(
        WxFrameInternals::wx_SPHERE );
    }
    gbto->maxTrianglesInLeaf->setValue( max_triangles_in_leaf );
    settings->max_triangles_spin->SetValue( max_triangles_in_leaf );
  }

  // Load Graphics caching options
  GraphicsCachingOptions *gco = NULL;
  global_settings->getOptionNode(gco);
  if(gco) {
    bool use_caching;
    bool cache_only_geometries;
    int caching_delay;
    if( from_config ) {
      if (h3dConfig->Exists(wxT("/Settings/GraphicsCaching"))) {
        h3dConfig->SetPath(wxT("/Settings/GraphicsCaching"));
        h3dConfig->Read(wxT("useCaching"), &use_caching, true );
        h3dConfig->Read(wxT("cacheOnlyGeometries"), &cache_only_geometries, false);
        h3dConfig->Read(wxT("cachingDelay"), &caching_delay, 5);
      } else {
        // on clean system
        use_caching = gco->useCaching->getValue();
        cache_only_geometries = gco->cacheOnlyGeometries->getValue();
        caching_delay = gco->cachingDelay->getValue();
      }
    } else {
      use_caching = non_conf_opt.use_caching;
      cache_only_geometries = non_conf_opt.cache_only_geometries;
      caching_delay = non_conf_opt.caching_delay;
    }

    gco->useCaching->setValue( use_caching );
    settings->display_list_checkbox->SetValue( use_caching );
    gco->cacheOnlyGeometries->setValue( cache_only_geometries );
    settings->only_geoms_checkbox->SetValue( cache_only_geometries );
    gco->cachingDelay->setValue( caching_delay );
    settings->caching_delay_spin->SetValue( caching_delay );
  }

  // Load Haptics options
  HapticsOptions *ho = NULL;
  global_settings->getOptionNode(ho);
  if(ho) {
    bool use_bound_tree;
    string touchable_face;
    string dynamic_mode;
    double max_distance;
    double look_ahead_factor;
    bool interpolate_force_effects;
    
    if( from_config ) {
      if (h3dConfig->Exists(wxT("/Settings/Haptics"))) {
        h3dConfig->SetPath(wxT("/Settings/Haptics"));
        h3dConfig->Read( wxT("useBoundTree"), &use_bound_tree );
        wxString touchable_face_wx;
        h3dConfig->Read( wxT("touchableFace"), &touchable_face_wx );
        touchable_face = toStr( touchable_face_wx );
        wxString dynamic_mode_wx;
        h3dConfig->Read( wxT("dynamicMode"), &dynamic_mode_wx );
        dynamic_mode = toStr( dynamic_mode_wx );
        h3dConfig->Read( wxT("maxDistance"), &max_distance );
        h3dConfig->Read( wxT("lookAheadFactor"), &look_ahead_factor );
        h3dConfig->Read( wxT("interpolateForceEffects"),
                         &interpolate_force_effects );
      } else {
        // on clean system
        use_bound_tree = ho->useBoundTree->getValue();
        touchable_face = ho->touchableFace->getValue();
        dynamic_mode = ho->dynamicMode->getValue();
        max_distance = ho->maxDistance->getValue();
        look_ahead_factor = ho->lookAheadFactor->getValue();
        interpolate_force_effects = ho->interpolateForceEffects->getValue();
      }
    } else {
      use_bound_tree = non_conf_opt.use_bound_tree;
      touchable_face = non_conf_opt.touchable_face;
      dynamic_mode = non_conf_opt.dynamic_mode;
      max_distance = non_conf_opt.max_distance;
      look_ahead_factor = non_conf_opt.look_ahead_factor;
      interpolate_force_effects = non_conf_opt.interpolate_force_effects;
    }

    ho->touchableFace->setValue( touchable_face );
    if( touchable_face == WxFrameInternals::as_graphics )
      settings->face_choice->
        SetStringSelection( WxFrameInternals::wx_as_graphics );
    else if( touchable_face == WxFrameInternals::front_and_back )
      settings->face_choice->
        SetStringSelection( WxFrameInternals::wx_front_and_back );
    else if( touchable_face == WxFrameInternals::front )
      settings->face_choice->SetStringSelection( WxFrameInternals::wx_front );
    else if( touchable_face == WxFrameInternals::back )
      settings->face_choice->SetStringSelection( WxFrameInternals::wx_back );

    ho->dynamicMode->setValue( dynamic_mode );
    if( dynamic_mode == WxFrameInternals::transform_changed )
      settings->dynamic_mode_choice->
        SetStringSelection( WxFrameInternals::wx_transform_changed );
    else if( dynamic_mode == WxFrameInternals::never )
      settings->dynamic_mode_choice->
        SetStringSelection( WxFrameInternals::wx_never );
    else if( dynamic_mode == WxFrameInternals::always )
      settings->dynamic_mode_choice->
        SetStringSelection( WxFrameInternals::wx_never );

    ho->maxDistance->setValue( max_distance );
    stringstream max_dist;
    max_dist  <<  max_distance;
    settings->max_distance_text->
      ChangeValue( wxString( max_dist.str().c_str(), wxConvUTF8 ) );

    ho->lookAheadFactor->setValue( look_ahead_factor );
    stringstream laf;
    laf << look_ahead_factor;
    settings->look_ahead_text->
      ChangeValue( wxString( laf.str().c_str(), wxConvUTF8 ) );

    ho->useBoundTree->setValue( use_bound_tree );
    settings->use_bound_tree_checkbox->SetValue( use_bound_tree );
    ho->interpolateForceEffects->setValue( interpolate_force_effects );
    settings->interpolate_force_effects_checkbox->
      SetValue( interpolate_force_effects );
  }

  // Load Haptics options
  OpenHapticsOptions *oho = NULL;
  global_settings->getOptionNode(oho);
  if(oho) {
    bool use_adaptive_viewport;
    bool use_haptic_camera_view;
    string gl_shape;
    bool force_full_geometry_render;

    if( from_config ) {
      if( h3dConfig->Exists( wxT("/Settings/OpenHaptics") ) ) {
        h3dConfig->SetPath(wxT("/Settings/OpenHaptics"));
        h3dConfig->Read( wxT("useAdaptiveViewport"), &use_adaptive_viewport );
        h3dConfig->Read( wxT("useHapticCameraView"), &use_haptic_camera_view );
        wxString gl_shape_wx;
        h3dConfig->Read( wxT("GLShape"), &gl_shape_wx );
        gl_shape = toStr( gl_shape_wx );
        h3dConfig->Read( wxT("forceFullGeometryRender"),
          &force_full_geometry_render );
      } else {
        // on clean system
        use_adaptive_viewport = oho->useAdaptiveViewport->getValue();
        use_haptic_camera_view = oho->useHapticCameraView->getValue();
        gl_shape = oho->GLShape->getValue();
        force_full_geometry_render = oho->forceFullGeometryRender->getValue();
      }
    } else {
      use_adaptive_viewport = non_conf_opt.use_adaptive_viewport;
      use_haptic_camera_view = non_conf_opt.use_haptic_camera_view;
      gl_shape = non_conf_opt.gl_shape;
      force_full_geometry_render = non_conf_opt.force_full_geometry_render;
    }

    oho->useAdaptiveViewport->setValue( use_adaptive_viewport );
    settings->adaptive_viewport->SetValue( use_adaptive_viewport );
    oho->useHapticCameraView->setValue( use_haptic_camera_view );
    settings->haptic_camera->SetValue( use_haptic_camera_view );
    oho->GLShape->setValue( gl_shape );
    if( gl_shape == WxFrameInternals::str_FEEDBACK ) {
      settings->shape_choice->SetStringSelection(
        WxFrameInternals::wx_FEEDBACK );
    } else if( gl_shape == WxFrameInternals::str_DEPTH ) {
      settings->shape_choice->SetStringSelection( WxFrameInternals::wx_DEPTH );
    } else if( gl_shape == WxFrameInternals::str_CUSTOM ) {
      settings->shape_choice->SetStringSelection(
        WxFrameInternals::wx_CUSTOM );
    }
    oho->forceFullGeometryRender->setValue( force_full_geometry_render );
    settings->full_geom_render->SetValue( force_full_geometry_render );
  }

  // Load StereoInfo
  if( stereo_info.get() ) {
    double focal_distance;
    double interocular_distance;
    if( from_config ) {
      if( h3dConfig->Exists( wxT("/Settings/StereoInfo") ) ) {
        h3dConfig->SetPath( wxT("/Settings/StereoInfo") );
        h3dConfig->Read( wxT("focalDistance"), &focal_distance );
        h3dConfig->Read( wxT("interocularDistance"), &interocular_distance );
      } else {
        // Only do this if there is nothing in the registry yet.
        focal_distance = stereo_info->focalDistance->getValue();
        interocular_distance = stereo_info->interocularDistance->getValue();
      }
    } else {
      focal_distance = non_conf_opt.focal_distance;
      interocular_distance = non_conf_opt.interocular_distance;
    }

    stereo_info->focalDistance->setValue( focal_distance );
    stringstream foc_dist;
    foc_dist << focal_distance;
    settings->focal_distance_text->
      SetValue( wxString( foc_dist.str().c_str(), wxConvUTF8 ) );
    stereo_info->interocularDistance->setValue( interocular_distance );
    stringstream int_dist;
    int_dist << interocular_distance;
    settings->interocular_distance_text->
      SetValue( wxString( int_dist.str().c_str(), wxConvUTF8 ) );
  }

  // Load proxy radius
  double proxy_radius;
  if( from_config ) {
    if( h3dConfig->Exists( wxT("/Settings/Ruspini") ) ) {
      h3dConfig->SetPath( wxT("/Settings/Ruspini") );
      h3dConfig->Read( wxT("proxyRadius"), &proxy_radius );
    } else {
      // No radius has ever been set. Set to default value.
      auto_ptr< RuspiniRenderer > temp_ptr( new RuspiniRenderer );
      proxy_radius = temp_ptr->proxyRadius->getValue();
    }
  } else {
    proxy_radius = non_conf_opt.proxy_radius;
  }

  setProxyRadius( proxy_radius );
  stringstream proxy_text;
  proxy_text << proxy_radius;
  settings->proxy_radius_text->
          SetValue( wxString(proxy_text.str().c_str(),wxConvUTF8) );
}

string WxFrame::navTypeToNavMenuString( const string &nav_type ) {
  if( nav_type == "EXAMINE" ) return "EXAMINE\tAlt-e";
  if( nav_type == "FLY" ) return "FLY\tAlt-f";
  if( nav_type == "WALK" ) return "WALK\tAlt-w";
  if( nav_type == "LOOKAT" ) return "LOOKAT\tAlt-l";
  if( nav_type == "NONE" ) return "NONE\tAlt-n";
  return nav_type;
}

//Build Navigation Menu
void WxFrame::buildNavMenu () {
  //Get active navigation info object
  if (NavigationInfo::getActive()) {
    mynav = NavigationInfo::getActive();
    //Store allowed navigation types and count
    vector<string> navTypes = mynav->type->getValue();

    if (mynav->getUsedNavType() == "NONE") {
      navigationMenu->Append(FRAME_NAVIGATION, wxT("Unavailable"),
                             wxT("Navigation Disabled"));
      navigationMenu->Enable(FRAME_NAVIGATION, false);
    } else {
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
      navTypeCount = int(allowedTypes.size());
      int j = 0;
      for (vector<string>::iterator menuList = allowedTypes.begin(); 
           menuList != allowedTypes.end(); menuList++) {
        string typeName = (*menuList);
        navigationMenu->AppendRadioItem(FRAME_NAVIGATION + j, wxString(navTypeToNavMenuString(typeName).c_str(),wxConvUTF8),
                                        wxT("Select a navigation mode"));
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
      navigationMenu->AppendRadioItem(FRAME_NAVIGATION + j, wxString(navTypeToNavMenuString( (*allList) ).c_str(),wxConvUTF8),
                                      wxT("Select a navigation mode"));
      Connect(FRAME_NAVIGATION + j,wxEVT_COMMAND_MENU_SELECTED, 
              wxCommandEventHandler(WxFrame::ChangeNavigation));
      j++;
    }
    glwindow->default_nav = "EXAMINE";
  }

  navigationMenu->AppendSeparator();
  navigationMenu->AppendCheckItem( BASIC_COLLISION, wxT( "Avatar collision" ),
    wxT("Turn on and off collision between avatar and objects in scene"));
  navigationMenu->
    FindItemByPosition( navigationMenu->GetMenuItemCount() - 1 )->
      Check( avatar_collision );
  navigationMenu->Append( FRAME_SPEED, wxT("Speed"),
                 wxT("Adjust navigation speed. + to increase, - to decrease." ));

  navigationMenu->AppendSeparator();
  if( !navigationDevices ) {
    wxMenu *tmp_menu = new wxMenu;
    tmp_menu->AppendCheckItem(FRAME_MOUSE_NAV, wxT("Mouse"), 
                                       wxT("Mouse can be used to navigate scene"));
    tmp_menu->Check( FRAME_MOUSE_NAV, true );
    tmp_menu->AppendCheckItem(FRAME_KEYBOARD_NAV, wxT("Keyboard"),
                              wxT("Keyboard can be used to navigate scene"));
    tmp_menu->Check( FRAME_KEYBOARD_NAV, true );
    tmp_menu->AppendCheckItem(FRAME_SWS_NAV, wxT("SpaceWareSensor"), 
      wxT("SpaceWareSensor can be used to navigate scene."));
    tmp_menu->AppendCheckItem( FRAME_HAPTICSDEVICE_NAV,
                                        wxT("Haptics Device"),
                          wxT("A haptics device can be used to navigate scene"));
    navigationDevices =
      navigationMenu->AppendSubMenu( tmp_menu, wxT("Navigation Devices"),
                    wxT("Toggle on an off which devices to use for navigation") );
  } else {
    navigationMenu->Append( navigationDevices );
  }
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
  SaveSettings( false );
  settings->Show();
}


IMPLEMENT_CLASS(FrameRateDialog, wxDialog)

BEGIN_EVENT_TABLE(FrameRateDialog, wxDialog)
  EVT_KEY_DOWN(FrameRateDialog::OnKeyDown)
END_EVENT_TABLE()

FrameRateDialog::FrameRateDialog(wxWindow* win ) :
  wxDialog (win, wxID_ANY, wxT("Frame rates"), wxDefaultPosition, wxDefaultSize,
            wxDEFAULT_DIALOG_STYLE) {
  graphics_rate = NULL;
  haptics_rate = NULL;

  topsizer = new wxBoxSizer( wxVERTICAL );

  updateMenuItems();

  Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(FrameRateDialog::OnKeyDown));

  SetSizer( topsizer );      // use the sizer for layout

  topsizer->SetSizeHints( this );   // set size hints to honour minimum size
  Layout();
}

void FrameRateDialog::OnKeyDown(wxKeyEvent& event) {
  wxMessageBox(wxT("hello from  framerate"));
  if (event.GetKeyCode() == WXK_ESCAPE || event.GetKeyCode() == WXK_F9) {
    Hide();
  }
}

void FrameRateDialog::updateMenuItems() {
  unsigned int nr_devices = 0;
  DeviceInfo *di = DeviceInfo::getActive();
  if( di ) {
    nr_devices = di->device->size();
  }

  wxBoxSizer* framerate_sizer = new wxBoxSizer( wxHORIZONTAL );
  framerate_sizer->Add( new wxStaticText( this, wxID_ANY, wxT("&Graphics:")), 0,
                        wxALL|wxALIGN_CENTER_VERTICAL, 5);
  graphics_rate = new wxStaticText( this, wxID_ANY, wxT("&1000"), 
                                    wxDefaultPosition, 
                                    wxDefaultSize,wxALIGN_RIGHT|wxST_NO_AUTORESIZE );
  framerate_sizer->Add( graphics_rate, 0,
                        wxALL|wxALIGN_RIGHT, 5);
  
  framerate_sizer->Add( new wxStaticText( this, wxID_ANY, wxT("&Haptics:")), 0,
                        wxALL|wxALIGN_CENTER_VERTICAL, 5);
  haptics_rate = new wxStaticText( this, wxID_ANY, wxT("&1000"), 
                                   wxDefaultPosition, 
                                   wxDefaultSize,wxALIGN_RIGHT|wxST_NO_AUTORESIZE );
  framerate_sizer->Add( haptics_rate, 0,
                        wxALL|wxALIGN_RIGHT, 5);

  framerate_sizer->Add( new wxStaticText( this, wxID_ANY, wxT("&Haptics time(10-5 s):")), 0,
                        wxALL|wxALIGN_CENTER_VERTICAL, 5);
  haptics_time = new wxStaticText( this, wxID_ANY, wxT("&100"), 
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

  Scene *scene = *Scene::scenes.begin();
  
  stringstream s;
  s  <<  (int)scene->frameRate->getValue();

  graphics_rate->SetLabel( wxString(s.str().c_str(),wxConvUTF8) );
  if( di && di->device->size() > 0 ) {
    H3DHapticsDevice *hd = di->device->getValueByIndex( 0 );
    stringstream hs;
    hs  <<  (int)hd->hapticsRate->getValue();
    haptics_rate->SetLabel( wxString(hs.str().c_str(),wxConvUTF8) );

    stringstream hts;
    hts << (int)(hd->hapticsLoopTime->getValue() * 1e5 );
    haptics_time->SetLabel( wxString(hts.str().c_str(),wxConvUTF8) );
  }
}


//-------------
// Viewpoints
//-------------
//Build Viewpoints Menu
void WxFrame::BuildViewpointsMenu( list< Node * > v_list ) {
  int i = 0;
  int unnamed_vp = 1;
  int unnamed_vg = 1;
  BuildViewpointsSubMenu( v_list, viewpointMenu, i, unnamed_vp, unnamed_vg );
  viewpointMenu->AppendSeparator();
  viewpointMenu->Append(FRAME_RESET_VIEWPOINT, 
                                     wxT("Reset"),
                                     wxT("Reset current viewpoint"));
  current_viewpoint = X3DViewpointNode::getActive();
}

void WxFrame::BuildViewpointsSubMenu( 
  list< Node* > v_list, wxMenu * menu, int &count, int &unnamed_vp, int &unnamed_vg ) {
  for ( list<Node*>::iterator v = v_list.begin(); v != v_list.end(); v++ ) {
    if ( ViewpointGroup * vg = dynamic_cast< ViewpointGroup* >(*v) ) {
      string v_description = vg->description->getValue();
      if( v_description == "" ) {
        stringstream v_dscr;
        v_dscr << "Unnamed Viewpoint Group " << unnamed_vg;
        v_description = v_dscr.str();
        unnamed_vg++;
      }
      // create submenu
      wxMenu *submenu = new wxMenu();
      menu->AppendSubMenu( submenu, wxString(v_description.c_str(),
               wxConvUTF8) );
      BuildViewpointsSubMenu( vg->getChildrenAsList(), submenu, count, unnamed_vp, unnamed_vg );
    } else {
      X3DViewpointNode * vp = static_cast< X3DViewpointNode * >( *v ); 
      string v_description = vp->description->getValue();
      if( v_description == "" ) {
        stringstream v_dscr;
        v_dscr << "Unnamed Viewpoint " << unnamed_vp;
        v_description = v_dscr.str();
        unnamed_vp++;
      }      
      int id = FRAME_VIEWPOINT+count;
      menu->AppendCheckItem( id, wxString(v_description.c_str(),wxConvUTF8),
                            wxT("Select a viewpoint"));
      if ( vp == Viewpoint::getActive() ) {
        menu->Check(id, true);
        current_viewpoint_id = id;
      }
      Connect(id, wxEVT_COMMAND_MENU_SELECTED,
              wxCommandEventHandler(WxFrame::ChangeViewpoint));
      itemIdViewpointMap[id] = vp;
      count++;
    }
  }
}

void WxFrame::DestroyViewpointsMenu() {
  DestroyViewpointsSubMenu( viewpointMenu );
  itemIdViewpointMap.clear();
}

void WxFrame::DestroyViewpointsSubMenu( wxMenu * menu ) {
  //Delete items from viewpoint menu & disconnect events
  wxMenuItemList items = menu->GetMenuItems();
  for ( wxMenuItemList::iterator i = items.begin();
    i != items.end(); i++ ) {
    int id = (*i)->GetId();
    wxMenu * submenu = (*i)->GetSubMenu();
    if ( submenu != NULL ) {          
      DestroyViewpointsSubMenu( submenu );
      viewpointMenu->Destroy( *i );
    } else {
      // if is a viewpoint, disconnect events
      if ( !(*i)->IsSeparator() && id != FRAME_RESET_VIEWPOINT ) {
        Disconnect(id, wxEVT_COMMAND_MENU_SELECTED,
                  wxCommandEventHandler(WxFrame::ChangeViewpoint));
      }
      viewpointMenu->Destroy( id );
    }
  }
}

list< Node * > WxFrame::GetTopLevelViews() {
  X3DViewpointNode::ViewpointList vp_list = 
    X3DViewpointNode::getAllViewpoints();
  ViewpointGroup::ViewpointGroupList vg_list = 
    ViewpointGroup::getAllViewpointGroups();
  list< Node * > v_list;
  for ( ViewpointGroup::ViewpointGroupList::iterator i = vg_list.begin();
    i != vg_list.end(); i++ ) {
    if ( (*i)->isTopLevel() || !(*i)->inSceneGraph() )
      v_list.push_back( *i );
  }
  for ( X3DViewpointNode::ViewpointList::iterator i = vp_list.begin();
    i != vp_list.end(); i++ ) {
    if ( (*i)->isTopLevel() || !(*i)->inSceneGraph() )
      v_list.push_back( *i );
  }
  return v_list;
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
  EVT_CHOICE(ID_DYNAMIC_MODE, SettingsDialog::handleSettingsChange)
  EVT_TEXT(ID_MAX_DISTANCE, SettingsDialog::handleSettingsChange )
  EVT_TEXT(ID_LOOK_AHEAD_FACTOR, SettingsDialog::handleSettingsChange )
  EVT_CHECKBOX( ID_USE_BOUND_TREE, SettingsDialog::handleSettingsChange)
  EVT_CHECKBOX( ID_INTERPOLATE_FORCE_EFFECTS,
                SettingsDialog::handleSettingsChange)
  
  EVT_TEXT( ID_FOCAL_DISTANCE, SettingsDialog::handleSettingsChange )
  EVT_TEXT( ID_INTEROCULAR_DISTANCE, SettingsDialog::handleSettingsChange )

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

SettingsDialog::SettingsDialog(wxWindow* win, WxFrame *w ):
    wx_frame( w ), on_cancel_rebuild_displaylist( false )
{


    SetExtraStyle(wxDIALOG_EX_CONTEXTHELP|wxWS_EX_VALIDATE_RECURSIVELY);

    int tabImage1 = -1;
    int tabImage2 = -1;
    int tabImage3 = -1;

    int resizeBorder = wxRESIZE_BORDER;

    m_imageList = NULL;

    Create(win, wxID_ANY, wxT("Settings"), wxDefaultPosition, wxDefaultSize,
        wxDEFAULT_DIALOG_STYLE|
        (int)wxPlatform::IfNot(wxOS_WINDOWS_CE, resizeBorder)
    );

    // If using a toolbook, also follow Mac style and don't create buttons
    CreateButtons(wxOK | wxCANCEL |
                        (int)wxPlatform::IfNot(wxOS_WINDOWS_CE, wxHELP)
    ); 

    wxBookCtrlBase* notebook = GetBookCtrl();
    notebook->SetImageList(m_imageList);

    wxPanel* generalSettings = CreateGeneralSettingsPage(notebook );
    wxPanel* openhaptics_settings = CreateOpenHapticsSettingsPage(notebook);
    wxPanel* ruspini_settings = CreateRuspiniSettingsPage(notebook);
    wxPanel* debug_settings = CreateDebugSettingsPage(notebook);

    notebook->AddPage(generalSettings, wxT("General"), true, tabImage1);
    notebook->AddPage(openhaptics_settings, wxT("OpenHaptics"), false,tabImage2);
    notebook->AddPage(ruspini_settings, wxT("Ruspini"), false,tabImage2);
    notebook->AddPage(debug_settings, wxT("Debug"), false, tabImage3);

    LayoutDialog();
}

SettingsDialog::~SettingsDialog()
{
}

wxPanel* SettingsDialog::CreateDebugSettingsPage(wxWindow* parent ) {
    wxPanel* panel = new wxPanel(parent, wxID_ANY);

    wxBoxSizer *topSizer = new wxBoxSizer( wxVERTICAL );
    wxBoxSizer *item0 = new wxBoxSizer( wxVERTICAL );

    wxBoxSizer* draw_bound_sizer = new wxBoxSizer( wxHORIZONTAL );
    draw_bound_box = new wxCheckBox( panel, ID_DRAW_BOUNDS,
                                     wxT("&Draw geometry bounding boxes"),
                                     wxDefaultPosition, wxDefaultSize);
    draw_bound_sizer->Add(draw_bound_box, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
    item0->Add( draw_bound_sizer, 0, wxGROW|wxALL, 0);

    wxBoxSizer* draw_triangles_sizer = new wxBoxSizer( wxHORIZONTAL );
    draw_triangles_box = new wxCheckBox( panel,
                                         ID_DRAW_TRIANGLES,
                                         wxT("&Draw haptic triangles"),
                                         wxDefaultPosition,
                                         wxDefaultSize );
    draw_triangles_sizer->Add(draw_triangles_box, 0, 
                              wxALL|wxALIGN_CENTER_VERTICAL, 5);
    item0->Add( draw_triangles_sizer, 0, wxGROW|wxALL, 0);

    wxBoxSizer* draw_tree_sizer = new wxBoxSizer( wxHORIZONTAL );
    draw_tree_box = new wxCheckBox( panel, ID_DRAW_BOUND_TREE,
                                    wxT("&Draw bound tree"),
                                    wxDefaultPosition, 
                                    wxDefaultSize );
    draw_tree_sizer->Add(draw_tree_box, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
    item0->Add( draw_tree_sizer, 0, wxGROW|wxALL, 0);

    wxBoxSizer* depth_sizer = new wxBoxSizer( wxHORIZONTAL );
    depth_sizer->Add(new wxStaticText(panel, wxID_ANY, wxT("&Depth:")), 0,
                     wxALL|wxALIGN_CENTER_VERTICAL, 5);
    
    depth_spin = new wxSpinCtrl( panel, ID_DRAW_TREE_DEPTH,
                                 wxEmptyString, wxDefaultPosition,
                                 wxSize(80, wxDefaultCoord));
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


wxPanel* SettingsDialog::CreateOpenHapticsSettingsPage(wxWindow* parent ) {
    wxPanel* panel = new wxPanel(parent, wxID_ANY);

    wxBoxSizer *topSizer = new wxBoxSizer( wxVERTICAL );
    wxBoxSizer *item0 = new wxBoxSizer( wxVERTICAL );

    wxBoxSizer* adaptive_viewport_sizer = new wxBoxSizer( wxHORIZONTAL );
    adaptive_viewport = new wxCheckBox( panel, ID_ADAPTIVE_VIEWPORT,
                                        wxT("&Use adaptive viewport"),
                                        wxDefaultPosition, 
                                        wxDefaultSize );
    adaptive_viewport_sizer->Add(adaptive_viewport, 0,
                                 wxALL|wxALIGN_CENTER_VERTICAL, 5);
    item0->Add( adaptive_viewport_sizer, 0, wxGROW|wxALL, 0);

    wxBoxSizer* haptic_camera_sizer = new wxBoxSizer( wxHORIZONTAL );
    haptic_camera = new wxCheckBox( panel, ID_HAPTIC_CAMERA,
                                    wxT("&Use haptic camera view"), 
                                    wxDefaultPosition,
                                    wxDefaultSize );
    haptic_camera_sizer->Add(haptic_camera, 0,
                             wxALL|wxALIGN_CENTER_VERTICAL, 5);
    item0->Add( haptic_camera_sizer, 0, wxGROW|wxALL, 0);

    wxBoxSizer* full_geom_render_sizer = new wxBoxSizer( wxHORIZONTAL );
    full_geom_render = new wxCheckBox( panel, 
                                       ID_FULL_GEOMETRY_RENDER,
                                       wxT("&Force full geometry render"),
                                       wxDefaultPosition, wxDefaultSize );
    full_geom_render_sizer->Add(full_geom_render, 0,
                                wxALL|wxALIGN_CENTER_VERTICAL, 5);
    item0->Add( full_geom_render_sizer, 0, wxGROW|wxALL, 0);

    wxArrayString shape_choices;
    shape_choices.Add(WxFrameInternals::wx_FEEDBACK);
    shape_choices.Add(WxFrameInternals::wx_DEPTH);
    shape_choices.Add(WxFrameInternals::wx_CUSTOM);

    wxBoxSizer* shape_sizer = new wxBoxSizer( wxHORIZONTAL );
 
    shape_sizer->Add(new wxStaticText(panel, wxID_ANY,
                                      wxT("&Default shape type:")), 0,
                                      wxALL|wxALIGN_CENTER_VERTICAL, 5);
    shape_choice = new wxChoice( panel, ID_OH_SHAPE_TYPE,
                                 wxDefaultPosition, wxDefaultSize,
                                 shape_choices );
    shape_sizer->Add(shape_choice, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
    item0->Add(shape_sizer, 0, wxGROW|wxALL, 5);

    topSizer->Add( item0, 1, wxGROW|wxALIGN_CENTRE|wxALL, 5 );

    panel->SetSizer(topSizer);
    topSizer->Fit(panel);

    return panel;
}

wxPanel* SettingsDialog::CreateRuspiniSettingsPage( wxWindow* parent ) {
    wxPanel* panel = new wxPanel(parent, wxID_ANY);

    wxBoxSizer *topSizer = new wxBoxSizer( wxHORIZONTAL );
 
    topSizer->Add(new wxStaticText(panel, wxID_ANY,
                                    wxT("&Proxy radius(m):")), 
                   0,
                   wxALL, 5);

    proxy_radius_text = new wxTextCtrl( panel, 
                                        ID_PROXY_RADIUS,
                                        wxEmptyString,
                                        wxDefaultPosition,
                                        wxSize(40, wxDefaultCoord));
    topSizer->Add(proxy_radius_text, 0, wxALL, 5);

    panel->SetSizer(topSizer);
    topSizer->Fit(panel);

    return panel;
}

wxPanel* SettingsDialog::CreateGeneralSettingsPage(wxWindow* parent ) {
  bool use_display_lists = true;
  bool cache_only_geometries = false;
  int caching_delay = 5;

  wxPanel* panel = new wxPanel(parent, wxID_ANY);

  wxBoxSizer *topSizer = new wxBoxSizer( wxVERTICAL );
  wxBoxSizer *item0 = new wxBoxSizer( wxVERTICAL );

  //// Graphics caching
  wxStaticBox* graphics_caching_box = new wxStaticBox(panel, wxID_ANY,
                                                       wxT("Graphics caching:"));

  wxBoxSizer* graphics_caching_sizer = new wxStaticBoxSizer( 
                                            graphics_caching_box, 
                                            wxVERTICAL 
                                            );
  item0->Add(graphics_caching_sizer, 0, wxGROW|wxALL, 5);

  wxBoxSizer* display_list_sizer = new wxBoxSizer( wxHORIZONTAL );
  display_list_checkbox = new wxCheckBox( panel,
                                          ID_USE_DISPLAY_LISTS,
                                          wxT("&Use display lists"),
                                          wxDefaultPosition, wxDefaultSize );
  display_list_checkbox->SetValue( use_display_lists );
  display_list_sizer->Add(display_list_checkbox, 0,
                            wxALL|wxALIGN_CENTER_VERTICAL, 5);
  graphics_caching_sizer->Add(display_list_sizer, 0, wxGROW|wxALL, 0);
    
  only_geoms_checkbox = new wxCheckBox( panel,
                                        ID_CACHE_ONLY_GEOMS,
                                        wxT("&Cache only geometries"),
                                        wxDefaultPosition,
                                        wxDefaultSize );
  only_geoms_checkbox->SetValue( cache_only_geometries );
  display_list_sizer->Add(only_geoms_checkbox, 0,
                            wxALL|wxALIGN_CENTER_VERTICAL, 5);
//  graphics_caching_sizer->Add(only_geoms_sizer, 0, wxGROW|wxALL, 0);

  wxBoxSizer* caching_delay_sizer = new wxBoxSizer( wxHORIZONTAL );
  caching_delay_sizer->Add(new wxStaticText(panel, wxID_ANY,
                             wxT("&Caching delay:")), 0,
                             wxALL|wxALIGN_CENTER_VERTICAL, 5);
    
  caching_delay_spin = new wxSpinCtrl( panel, ID_CACHING_DELAY,
                                       wxEmptyString,
                                       wxDefaultPosition,
                                       wxSize(80,wxDefaultCoord) );
  caching_delay_spin->SetValue( caching_delay );
  caching_delay_sizer->Add(caching_delay_spin, 0,
                           wxALL|wxALIGN_CENTER_HORIZONTAL|
                           wxALIGN_CENTER_VERTICAL, 5);
  graphics_caching_sizer->Add( caching_delay_sizer );


  int bound_type = 1;
  int max_triangles_in_leaf = 1;

  // Geometry bound
  wxStaticBox* bound_tree_box = new wxStaticBox(panel, wxID_ANY,
                                                wxT("Bound tree:"));

  wxArrayString bound_choices;
  bound_choices.Add(WxFrameInternals::wx_OBB);
  bound_choices.Add(WxFrameInternals::wx_AABB);
  bound_choices.Add(WxFrameInternals::wx_SPHERE);

  wxBoxSizer* bound_tree_sizer = new wxStaticBoxSizer( bound_tree_box,
                                                       wxVERTICAL );
  item0->Add(bound_tree_sizer, 0, wxGROW|wxALL, 5);

  wxBoxSizer* itemSizer2 = new wxBoxSizer( wxHORIZONTAL );

  bound_choice = new wxChoice( panel, ID_BOUND_TYPE, wxDefaultPosition,
                               wxDefaultSize, bound_choices );

  itemSizer2->Add( new wxStaticText(panel, wxID_ANY, wxT("&Bound type:")), 0, 
                  wxALL|wxALIGN_CENTER_VERTICAL, 5);

  itemSizer2->Add(bound_choice, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
  bound_tree_sizer->Add(itemSizer2, 0, wxGROW|wxALL, 5);

  wxBoxSizer* max_triangles_sizer = new wxBoxSizer( wxHORIZONTAL );
  max_triangles_sizer->Add(new wxStaticText(panel, wxID_ANY,
                           wxT("&Max triangles:")), 0, 
                           wxALL|wxALIGN_CENTER_VERTICAL, 5);
    
  max_triangles_spin = new wxSpinCtrl( panel, ID_MAX_TRIANGLES, 
                                       wxEmptyString,
                                       wxDefaultPosition,
                                       wxSize(80,wxDefaultCoord) );
  max_triangles_spin->SetRange( 1, 1000 );
  max_triangles_sizer->Add(max_triangles_spin, 0,
                           wxALL|wxALIGN_CENTER_HORIZONTAL|
                           wxALIGN_CENTER_VERTICAL, 5);
  bound_tree_sizer->Add( max_triangles_sizer );

  //// haptics options

  wxStaticBox* haptics_box = new wxStaticBox(panel, wxID_ANY,
                                             wxT("Haptics options:"));
  wxBoxSizer* haptics_box_sizer = new wxStaticBoxSizer( haptics_box, 
                                                        wxVERTICAL );

  wxArrayString face_choices;
  face_choices.Add( WxFrameInternals::wx_as_graphics );
  face_choices.Add(WxFrameInternals::wx_front_and_back);
  face_choices.Add(WxFrameInternals::wx_front);
  face_choices.Add(WxFrameInternals::wx_back);

  wxBoxSizer* face_choice_sizer = new wxBoxSizer( wxHORIZONTAL );
  face_choice_sizer->Add(new wxStaticText(panel, wxID_ANY,
                                          wxT("&Touchable face:")), 0,
                                          wxALL|wxALIGN_CENTER_VERTICAL, 5);
  face_choice = new wxChoice( panel, ID_TOUCHABLE_FACE,
                              wxDefaultPosition, wxDefaultSize,
                              face_choices );
  face_choice_sizer->Add(face_choice, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
  haptics_box_sizer->Add(face_choice_sizer, 0, wxGROW|wxALL, 5);

  wxArrayString dynamic_mode_choices;
  dynamic_mode_choices.Add(WxFrameInternals::wx_transform_changed);
  dynamic_mode_choices.Add(WxFrameInternals::wx_never);
  dynamic_mode_choices.Add(WxFrameInternals::wx_always);

  wxBoxSizer* dynamic_mode_sizer = new wxBoxSizer( wxHORIZONTAL );
  dynamic_mode_sizer->Add( new wxStaticText( panel, wxID_ANY,
                                             wxT( "&Dynamic mode:" )), 0,
                                             wxALL|wxALIGN_CENTER_VERTICAL, 5);
  dynamic_mode_choice = new wxChoice( panel, ID_DYNAMIC_MODE,
                                      wxDefaultPosition, wxDefaultSize,
                                      dynamic_mode_choices );
  dynamic_mode_sizer->Add( dynamic_mode_choice, 0,
                           wxALL|wxALIGN_CENTER_VERTICAL, 5);
  haptics_box_sizer->Add( dynamic_mode_sizer, 0, wxGROW|wxALL, 5);

  wxBoxSizer* max_distance_sizer = new wxBoxSizer( wxHORIZONTAL );
  max_distance_sizer->Add(new wxStaticText(panel, wxID_ANY,
                                           wxT("&Max distance:")), 0,
                                           wxALL|wxALIGN_CENTER_VERTICAL, 5);
  max_distance_text = new wxTextCtrl( panel, ID_MAX_DISTANCE,
                                      wxEmptyString,
                                      wxDefaultPosition,
                                      wxSize(40, wxDefaultCoord) );
  max_distance_sizer->Add(max_distance_text, 0,
                          wxALL|wxALIGN_CENTER_VERTICAL, 5);
  haptics_box_sizer->Add(max_distance_sizer, 0, wxGROW|wxALL, 5);

  wxBoxSizer* look_ahead_sizer = new wxBoxSizer( wxHORIZONTAL );
  look_ahead_sizer->Add(new wxStaticText(panel, wxID_ANY,
                                         wxT("&Look ahead factor:")), 0,
                                         wxALL|wxALIGN_CENTER_VERTICAL, 5);
  look_ahead_text = new wxTextCtrl( panel, ID_LOOK_AHEAD_FACTOR,
                                    wxEmptyString,
                                    wxDefaultPosition,
                                    wxSize(40, wxDefaultCoord) );
  look_ahead_sizer->Add(look_ahead_text, 0, wxALL|wxALIGN_CENTER_VERTICAL,5);
  haptics_box_sizer->Add(look_ahead_sizer, 0, wxGROW|wxALL, 5);

  wxBoxSizer* use_bound_tree_sizer = new wxBoxSizer( wxHORIZONTAL );
  use_bound_tree_checkbox = new wxCheckBox( panel,
                                            ID_USE_BOUND_TREE, 
                                            wxT("&Use bound tree"),
                                            wxDefaultPosition,
                                            wxDefaultSize );
  use_bound_tree_sizer->Add(use_bound_tree_checkbox, 0,
                            wxALL|wxALIGN_CENTER_VERTICAL, 5);

  haptics_box_sizer->Add(use_bound_tree_sizer, 0, wxGROW|wxALL, 0);

  wxBoxSizer* interpolate_force_effects_sizer = new wxBoxSizer( wxHORIZONTAL );
  interpolate_force_effects_checkbox = new wxCheckBox(
                      panel,
                      ID_INTERPOLATE_FORCE_EFFECTS, 
                      wxT("&Interpolate force effects"),
                      wxDefaultPosition,
                      wxDefaultSize );
  interpolate_force_effects_sizer->Add(interpolate_force_effects_checkbox, 0,
                            wxALL|wxALIGN_CENTER_VERTICAL, 5);
  haptics_box_sizer->
    Add(interpolate_force_effects_sizer, 0, wxGROW|wxALL, 0);

  item0->Add(haptics_box_sizer, 0, wxGROW|wxLEFT|wxRIGHT, 5);

  // StereoInfo Options
  item0->Add(5, 5, 1, wxALL, 0);
  wxStaticBox* stereo_info_box = new wxStaticBox( panel, wxID_ANY,
                                                  wxT("StereoInfo options:"));
  wxBoxSizer* stereo_info_box_sizer = new wxStaticBoxSizer( stereo_info_box,
                                                            wxVERTICAL );

  wxBoxSizer* focal_distance_sizer = new wxBoxSizer( wxHORIZONTAL );
  focal_distance_sizer->Add( new wxStaticText(panel, wxID_ANY,
                             wxT("&Focal Distance:")), 0,
                             wxALL|wxALIGN_CENTER_VERTICAL, 5 );
  focal_distance_text = new wxTextCtrl( panel, ID_FOCAL_DISTANCE,
                                        wxEmptyString,
                                        wxDefaultPosition,
                                        wxSize(40, wxDefaultCoord) );
  
  focal_distance_sizer->Add(focal_distance_text, 0,
    wxALL|wxALIGN_CENTER_VERTICAL, 5);

  stereo_info_box_sizer->Add(focal_distance_sizer, 0, wxGROW|wxALL, 0);

  wxBoxSizer* interocular_distance_sizer = new wxBoxSizer( wxHORIZONTAL );
  interocular_distance_sizer->Add( new wxStaticText(panel, wxID_ANY,
                                   wxT("&Interocular Distance:")), 0,
                                   wxALL|wxALIGN_CENTER_VERTICAL, 5);
  interocular_distance_text = new wxTextCtrl( panel,
                                              ID_INTEROCULAR_DISTANCE,
                                              wxEmptyString,
                                              wxDefaultPosition,
                                              wxSize(40, wxDefaultCoord) );
  interocular_distance_sizer->Add( interocular_distance_text, 0,
                                   wxALL|wxALIGN_CENTER_VERTICAL, 5);

  stereo_info_box_sizer->Add(interocular_distance_sizer, 0, wxGROW|wxALL, 0);

  item0->Add(stereo_info_box_sizer, 0, wxGROW|wxLEFT|wxRIGHT, 5);
  // End StereoInfo options.

  topSizer->Add( item0, 1, wxGROW|wxALIGN_CENTRE|wxALL, 5 );
  topSizer->AddSpacer(5);

  panel->SetSizer(topSizer);
  topSizer->Fit(panel);

  return panel;
}

void SettingsDialog::OnOk (wxCommandEvent & event) {
  static_cast< WxFrame* >(this->GetParent())->SaveSettings( true );
  on_cancel_rebuild_displaylist = false;
  this->Show(false);
}

void SettingsDialog::OnCancel (wxCommandEvent & event) {
  static_cast< WxFrame* >(this->GetParent())->LoadSettings( false );
  if( on_cancel_rebuild_displaylist )
    H3DDisplayListObject::DisplayList::rebuildAllDisplayLists();
  on_cancel_rebuild_displaylist = false;
  this->Show(false);
}

void WxFrame::setProxyRadius( float r ) {

  for (NodeVector::const_iterator nv = allDevices.begin(); 
       nv != allDevices.end(); nv++) {
    RuspiniRenderer *renderer = dynamic_cast< RuspiniRenderer * >(
      static_cast < H3DHapticsDevice *> (*nv)->
        hapticsRenderer->getValue() );

    if( renderer ) {
      renderer->proxyRadius->setValue( r );
      Node *proxy = default_stylus_dn.getNode("PROXY");
      if( proxy ) {
        SFFloat *radius = 
          dynamic_cast< SFFloat * >( proxy->getField("radius") );
        if( radius ) 
          radius->setValue( r );
      }
    }
  }
}

SpeedDialog::SpeedDialog( wxWindow* parent, WxFrame *f ):
  wxDialog( parent, wxID_ANY, wxString( _T("Navigation Speed") ) ),
    wx_frame( f ) {
  wxGridBagSizer *top_sizer = new wxGridBagSizer;
  top_sizer->SetFlexibleDirection( wxBOTH );
  top_sizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
  float max_speed = 50;
  int max_tick_value = 500;
  speed_per_tick = max_speed / max_tick_value;

  wxStaticText *min_label =
    new wxStaticText( this,
                      wxID_ANY,
                      wxT( "0" ),
                      wxDefaultPosition,
                      wxDefaultSize,
                      wxALIGN_LEFT|wxST_NO_AUTORESIZE );
  min_label->Wrap( -1 );
  min_label->SetMinSize( wxSize( 60, -1 ) );
  top_sizer->Add( min_label, wxGBPosition( 0, 0 ),
                  wxGBSpan( 1, 1 ), wxALL, 5 );

  value_label = new wxStaticText( this,
                                  wxID_ANY,
                                  wxT( "1" ),
                                  wxDefaultPosition,
                                  wxDefaultSize,
                                  wxALIGN_CENTER|wxST_NO_AUTORESIZE );
  value_label->SetMinSize( wxSize( 60, -1 ) );
  value_label->Wrap( -1 );
  top_sizer->Add( value_label, wxGBPosition( 0, 1 ),
                  wxGBSpan( 1, 1 ), wxALL, 5 );

  stringstream max_speed_stm;
  max_speed_stm << max_speed;
  wxStaticText *max_label =
    new wxStaticText( this,
                      wxID_ANY,
                      wxString(max_speed_stm.str().c_str(),wxConvUTF8),
                      wxDefaultPosition,
                      wxDefaultSize,
                      wxALIGN_RIGHT|wxST_NO_AUTORESIZE );
  max_label->SetMinSize( wxSize( 60, -1 ) );
  max_label->Wrap( -1 );
  top_sizer->Add( max_label, wxGBPosition( 0, 2 ),
                  wxGBSpan( 1, 1 ), wxALL, 5 );

  speed_slider = new wxSlider( this, FRAME_SPEED_SLIDER,
                               1, 0, max_tick_value );
  speed_slider->SetMinSize( wxSize( 200, -1 ) );


  //top_sizer->Add(title_sizer, 0, wxALL, 5);
  top_sizer->Add( speed_slider, wxGBPosition( 1, 0 ),
                  wxGBSpan( 1, 3 ), wxALL, 5 );

  SetSizer(top_sizer);

  top_sizer->SetSizeHints(this);
  top_sizer->Fit(this);
}

IMPLEMENT_CLASS(SpeedDialog, wxDialog)
BEGIN_EVENT_TABLE(SpeedDialog, wxDialog)
  EVT_COMMAND_SCROLL( FRAME_SPEED_SLIDER, 
                      SpeedDialog::handleSliderEvent)
END_EVENT_TABLE()

void SpeedDialog::handleSliderEvent( wxScrollEvent &event ) {
  int id = event.GetId();

  if( id == FRAME_SPEED_SLIDER ) {
    float new_value = event.GetPosition() * speed_per_tick;
    setSpeed( new_value, true, false );
  }
}

void SpeedDialog::setSpeed() {
  NavigationInfo *mynav = NavigationInfo::getActive();
  if( mynav ) {
    setSpeed( mynav->speed->getValue(), true, true, false );
  } else {
    setSpeed( wx_frame->glwindow->default_speed, true, true, false );
  }
}

void SpeedDialog::setSpeed( float speed,
                            bool update_label,
                            bool update_slider,
                            bool update_scene_speed ) {
  if( speed < 0 )
    speed = 0;

  if( update_label ) {
    stringstream new_value_stm;
    new_value_stm << speed;
    value_label->SetLabel( wxString(new_value_stm.str().c_str(),wxConvUTF8) );
  }

  if( update_slider ) {
    speed_slider->SetValue( (int)( speed / speed_per_tick ) );
  }

  if( update_scene_speed ) {
    NavigationInfo *mynav = NavigationInfo::getActive();
    if( mynav ) {
      mynav->speed->setValue( speed );
    }
    wx_frame->glwindow->default_speed = speed;
  }
}

