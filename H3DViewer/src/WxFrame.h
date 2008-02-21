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
/// \file WxFrame.h
/// \brief Header file for WxFrame.
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <wx/wx.h>
#include <wx/propdlg.h> 
#include <wx/bookctrl.h>
#include <wx/docview.h>
#include <wx/spinctrl.h>
#include <wx/config.h>
#include <wx/confbase.h>

#include "WxWidgetsWindow.h"
#include <H3D/GlobalSettings.h>
#include <H3D/StereoInfo.h>

#include <H3D/Group.h>
#include <H3D/Transform.h>
#include <H3D/Scene.h>
#include <H3D/KeySensor.h>
#include <H3D/MouseSensor.h>
#include <H3D/SpaceWareSensor.h>
#include <H3D/DeviceInfo.h>
#include <H3D/INIFile.h>
#include <H3D/ResourceResolver.h>
#include <H3D/NavigationInfo.h>
#include <H3D/X3D.h>

#include <H3DUtil/Console.h>

#include "ConsoleDialog.h"

using namespace std;
using namespace H3D;

// ---------------------------------------------------------------------------
//  Property Sheet Dialog
// ---------------------------------------------------------------------------

class WxFrame;

class SettingsDialog: public wxPropertySheetDialog
{
DECLARE_CLASS(SettingsDialog)
public:
    SettingsDialog(wxWindow* parent, GlobalSettings *gs, WxFrame *f );
    ~SettingsDialog();

  void handleSettingsChange (wxCommandEvent & event);
  void handleSpinEvent (wxSpinEvent & event);
  void OnOk (wxCommandEvent & event);
  void OnCancel (wxCommandEvent & event);

  wxPanel* CreateGeneralSettingsPage(wxWindow* parent, GlobalSettings *gs );
  wxPanel* CreateOpenHapticsSettingsPage(wxWindow* parent, GlobalSettings *gs);
  wxPanel* CreateRuspiniSettingsPage(wxWindow* parent, GlobalSettings *gs);
  wxPanel* CreateDebugSettingsPage(wxWindow* parent, GlobalSettings *gs);

  inline float getProxyRadius() {
    return std::strtod( proxy_radius_text->GetValue().c_str(), NULL );
  }
  
protected:

  enum {
    ID_SHOW_TOOLTIPS = 100,

    ID_AUTO_SAVE,
    ID_AUTO_SAVE_MINS,
    ID_LOAD_LAST_PROJECT,

    ID_APPLY_SETTINGS_TO,
    ID_BACKGROUND_STYLE,
    ID_FONT_SIZE,

    ID_MAX_TRIANGLES,
    ID_USE_DISPLAY_LISTS,
    ID_CACHE_ONLY_GEOMS,
    ID_CACHING_DELAY,
    ID_PROXY_RADIUS,
    ID_BOUND_TYPE,
    ID_MAX_DISTANCE,
    ID_TOUCHABLE_FACE,
    ID_LOOK_AHEAD_FACTOR,
    ID_USE_BOUND_TREE,
    ID_OH_SHAPE_TYPE,
    ID_ADAPTIVE_VIEWPORT,
    ID_HAPTIC_CAMERA,
    ID_FULL_GEOMETRY_RENDER,
    ID_DRAW_BOUNDS,
    ID_DRAW_TRIANGLES,
    ID_DRAW_BOUND_TREE,
    ID_DRAW_TREE_DEPTH,
    ID_USE_COLLISION_DETECTION,
    ID_FOCAL_DISTANCE,
    ID_INTEROCULAR_DISTANCE

  };

  wxImageList*    m_imageList;
  WxFrame *wx_frame;
  bool boundTree;
  int treeDepth;
  wxTextCtrl* proxy_radius_text;

DECLARE_EVENT_TABLE()
};


class FrameRateDialog: public wxDialog {
  DECLARE_CLASS(FrameRateDialog)
  public:
  FrameRateDialog(wxWindow* parent );
  
  void updateFrameRates();
  void updateMenuItems();

  wxBoxSizer *topsizer;
  wxStaticText *graphics_rate;
  wxStaticText *haptics_rate;
  wxStaticText *haptics_time;
//  vector< wxStaticText * > haptics_rate;
DECLARE_EVENT_TABLE()
};



// ---------------------------------------------------------------------------
//  WxFrame Definition
// ---------------------------------------------------------------------------

class WxFrame: public wxFrame
{
public:
  WxFrame( wxWindow *_parent, wxWindowID _id, const wxString& _title,
              const wxPoint& _pos, const wxSize& _size,
              long _style = wxDEFAULT_FRAME_STYLE,
              const wxString& name = wxT("H3D Player") );

  //Pointer to WxWidgetsWindow
  WxWidgetsWindow *glwindow;

  /***************Interface Features*****************/
  //Main Menu Bar
  wxMenuBar  *menuBar;
  //Menu Bar Items
  wxMenu     *fileMenu, *rendererMenu, *deviceMenu, *viewpointMenu,
             *navigationMenu, *advancedMenu, *helpMenu, *navigationDevices;
  //Submenu items
  wxMenu     *hapticsRenderer, *renderMode;
  //File History Menu
  wxFileHistory *recentFiles;

  //Config object to save information (settings, file history etc...)
  wxConfigBase *h3dConfig;

  bool loadFile( const std::string &file );
  void clearData();

  /***************Member Functions*****************/
  void OnOpenFile	(wxCommandEvent & event);
  void OnMRUFile  (wxCommandEvent & event);
  void OnOpenFileURL	(wxCommandEvent & event);
  void OnCloseFile	(wxCommandEvent & event);
  void OnChooseDir  (wxCommandEvent & event);
  void OnExit     	(wxCommandEvent & event);
  void OnWindowExit (wxCloseEvent & event);
  void OnAbout		(wxCommandEvent & event);
  void OnHelp		(wxCommandEvent & event);
  void OnFullscreen	(wxCommandEvent & event);
  void RestoreWindow(wxCommandEvent & event);
  void MirrorScene	(wxCommandEvent & event);
  void RenderMode	(wxCommandEvent & event);
  void ShowConsole	(wxCommandEvent & event);
  void ShowFrameRate	(wxCommandEvent & event);
  void GetSelection (wxMenuEvent & event);
  void ChangeViewpoint (wxCommandEvent & event);
  void ResetViewpoint (wxCommandEvent & event);
  void ChangeNavigation (wxCommandEvent & event);
  void ChangeNavigationDevice (wxCommandEvent & event);
  void ChangeRenderer (wxCommandEvent & event);
  void ToggleHaptics  (wxCommandEvent & event);
  void OnSettings( wxCommandEvent & event);

  /***************Standard trivial functions***************/
  wxString GetCurrentFilename();
  void SetCurrentFilename(wxString);
  void SetCurrentPath(wxString);
  wxString GetCurrentPath();
  bool validateNavType(string);
  void SaveMRU ();
  void SaveSettings ();
  void LoadMRU();
  void LoadSettings ();
  void buildNavMenu();
  //void readSettingsFromINIFile( const string &filename,GlobalSettings *gs );
  void setProxyRadius( float r );

  void updateFrameRates() {
    frameRates->updateFrameRates();
  }
private:
	wxString currentFilename;
	wxString currentPath;
	bool lastmirror;
  bool lastDeviceStatus;
	//wxString renderModeSelection;
	int selection;
	int viewpointCount;
	int navTypeCount;
	int deviceCount;
	X3DViewpointNode::ViewpointList VPlist;
	NavigationInfo *mynav;
	DeviceInfo *mydevice;
	H3DHapticsDevice *myH3Ddevice;
	NodeVector allDevices;
	X3DViewpointNode *defaultvp;
	
	//One time intialization variables
	string settings_path;
	string common_path;
	string deviceinfo_file;
	string stylus_file;
	string viewpoint_file;
	string render_mode;

	//KeyRotation *kr;

	//Autoref Variables
	AutoRef< Scene > scene;
	AutoRef< KeySensor > ks;
	AutoRef< MouseSensor > ms;
	AutoRef< SpaceWareSensor > ss;
	AutoRef< Transform > t;
	AutoRef< Node > device_info;
	AutoRef< Node > viewpoint;
	AutoRef< Node > default_stylus;
  AutoRef< Group > g;

  float original_proxy_radius;
  float current_proxy_radius;
  X3D::DEFNodes default_stylus_dn;

	consoleDialog *  theConsole;
	FrameRateDialog *  frameRates;
	SettingsDialog * settings;

  AutoRef< GlobalSettings > global_settings;

  AutoRef< StereoInfo > stereo_info;

  friend class SettingsDialog;

protected:
  DECLARE_EVENT_TABLE()
};




enum
{ FRAME_EXIT    =   wxID_HIGHEST + 1,
  FRAME_OPEN,
  FRAME_OPEN_URL,
  FRAME_CLOSE,
  FRAME_CHOOSEDIR,
  FRAME_FULLSCREEN,
  FRAME_RESTORE,
  FRAME_MIRROR,
  FRAME_CONSOLE,
  FRAME_FRAMERATE,
  FRAME_SELECTION,
  FRAME_VIEWPOINT,
  FRAME_NAVIGATION = 6500,
  FRAME_DEVICE = 6550,
  FRAME_CHOOSERENDERER = 6600,
  FRAME_OPENHAPTICS,
  FRAME_CHAI3D,
  FRAME_GODOBJECT,
  FRAME_RUSPINI,
  FRAME_RENDERMODE,
  FRAME_MONO,
  FRAME_QUADBUFFERED,
  FRAME_HORZSPLIT,
  FRAME_VERTSPLIT,
  FRAME_VERTSPLITKEEPASPECT,
  FRAME_HORZINTERLACED,
  FRAME_VERTINTERLACED,
  FRAME_SHARPDISPLAY,
  FRAME_REDBLUE,
  FRAME_REDCYAN,
  FRAME_DEVICECONTROL,
  FRAME_HAPTICSCONTROL,
  BASIC_PREFRENDERER,
  BASIC_WIREFRAME,
  BASIC_NOTEXTURES,
  BASIC_SMOOTHSHADING,
  BASIC_SELECT,
  BASIC_NAVWALK,
  BASIC_NAVEXAMINE,
  BASIC_NAVFLY,
  BASIC_DIR,
  FRAME_ABOUT,
  FRAME_HELP,
  FRAME_SETTINGS,
  FRAME_RESET_VIEWPOINT,
  FRAME_MOUSE_NAV,
  FRAME_KEYBOARD_NAV,
  FRAME_SWS_NAV,
  FRAME_HAPTICSDEVICE_NAV
};


