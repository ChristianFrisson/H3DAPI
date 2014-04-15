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
#include <H3D/DeviceInfo.h>
#include <H3D/ResourceResolver.h>
#include <H3D/NavigationInfo.h>
#include <H3D/X3D.h>
#include <H3D/PeriodicUpdate.h>

#include <H3DUtil/Console.h>

#include "WxConsoleDialog.h"
#ifdef HAVE_PROFILER
#include "WxProfiledResultDialog.h"
#endif
#include "H3DViewerTreeViewDialog.h"
#include "H3DViewerPluginsDialog.h"
#include <H3DViewerConfig.h>

using namespace std;
using namespace H3D;

// ---------------------------------------------------------------------------
//  Property Sheet Dialog
// ---------------------------------------------------------------------------

class WxFrame;

class SpeedDialog: public wxDialog {
  DECLARE_CLASS( SpeedDialog )
public:
  SpeedDialog( WxFrame *parent );
  void handleSliderEvent( wxScrollEvent &event );

  // Calls the second version of set speed with the speed taken from
  // current navigation info if there is one, otherwise the speed is the
  // current default speed for glwindow.
  void setSpeed();

  // Called to update speed.
  // speed is the new speed to set.
  // update_label decides if the value_label text should be updated.
  // update_slider decides if the slider tick value should be updated.
  // update_scene_speed decides if the navigation speed in the scene should
  // be updated.
  void setSpeed( float speed,
                 bool update_label,
                 bool update_slider,
                 bool update_scene_speed = true );

protected:
  // The speed increase for each tick on the slider.
  float speed_per_tick;
  WxFrame *wx_frame;
  wxStaticText *value_label;
  wxSlider* speed_slider;
  DECLARE_EVENT_TABLE()
};

class SettingsDialog: public wxPropertySheetDialog
{
DECLARE_CLASS(SettingsDialog)
public:
    SettingsDialog(WxFrame* parent );
    ~SettingsDialog();

  void handleSettingsChange (wxCommandEvent & event);
  void handleSliderEvent( wxScrollEvent &event );
  void handleSpinEvent (wxSpinEvent & event);
  void OnOk (wxCommandEvent & event);
  void OnCancel (wxCommandEvent & event);

  wxPanel* CreateGeneralSettingsPage(wxWindow* parent );
  wxPanel* CreateOpenHapticsSettingsPage(wxWindow* parent);
  wxPanel* CreateRuspiniSettingsPage(wxWindow* parent);
  wxPanel* CreateDebugSettingsPage(wxWindow* parent);

  float getProxyRadius() {
    return X3D::Convert::atof( proxy_radius_text->GetValue().mb_str() );
  }

  // DebugOptionsUI
  wxSpinCtrl *depth_spin;
  wxCheckBox *draw_bound_box;
  wxCheckBox *draw_tree_box;
  wxCheckBox *draw_triangles_box;
  bool boundTree;
  int treeDepth;

  // GraphicsCachingOptionsUI
  wxCheckBox *display_list_checkbox;
  wxCheckBox *only_geoms_checkbox;
  wxSpinCtrl *caching_delay_spin;
  wxChoice *culling_choice;
  wxCheckBox *default_shadows_checkbox;
  wxSlider *shadow_darkness_slider;
  wxStaticText *shadow_darkness_static_text;
  wxTextCtrl *shadow_depth_offset_text;
  wxCheckBox *vertex_buffer_object_checkbox;

  // HapticsOptions
  wxChoice *face_choice;
  wxChoice *dynamic_mode_choice;
  wxTextCtrl *max_distance_text;
  wxTextCtrl *look_ahead_text;
  wxCheckBox *use_bound_tree_checkbox;
  wxCheckBox *interpolate_force_effects_checkbox;

  // GeometryBoundTreeOptionsGUI
  wxChoice *bound_choice;
  wxSpinCtrl *max_triangles_spin;

  // StereoInfoGUI
  wxTextCtrl *focal_distance_text;
  wxTextCtrl* interocular_distance_text;

  // OpenHapticsOptionsGUI
  wxCheckBox *adaptive_viewport;
  wxCheckBox *haptic_camera;
  wxCheckBox *full_geom_render;
  wxChoice *shape_choice;

  // Proxy radius used by ruspini
  wxTextCtrl* proxy_radius_text;

  bool on_cancel_rebuild_displaylist;

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
    ID_CULLING,
    ID_DEFAULT_SHADOWS,
    ID_VERTEX_BUFFER_OBJECT,
    ID_SHADOW_DARKNESS,
    ID_SHADOW_DEPTH_OFFSET,
    ID_PROXY_RADIUS,
    ID_BOUND_TYPE,
    ID_MAX_DISTANCE,
    ID_TOUCHABLE_FACE,
    ID_DYNAMIC_MODE,
    ID_LOOK_AHEAD_FACTOR,
    ID_USE_BOUND_TREE,
    ID_INTERPOLATE_FORCE_EFFECTS,
    ID_OH_SHAPE_TYPE,
    ID_ADAPTIVE_VIEWPORT,
    ID_HAPTIC_CAMERA,
    ID_FULL_GEOMETRY_RENDER,
    ID_DRAW_BOUNDS,
    ID_DRAW_TRIANGLES,
    ID_DRAW_BOUND_TREE,
    ID_DRAW_TREE_DEPTH,
    ID_FOCAL_DISTANCE,
    ID_INTEROCULAR_DISTANCE

  };

  wxImageList*    m_imageList;
  WxFrame *wx_frame;

DECLARE_EVENT_TABLE()
};


class FrameRateDialog: public wxDialog {
  DECLARE_CLASS(FrameRateDialog)
  public:
  FrameRateDialog(wxWindow* parent );
  
  void updateFrameRates();
  void updateMenuItems();

  void OnKeyDown(wxKeyEvent& event);

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
              const wxString& name = wxT("H3D Player"),
              bool cmd_line_filename = false,
              bool disable_plugin_dialog = false );

  ~WxFrame() {
    if( recentFiles )
      delete recentFiles;
  }

  /// Takes a navigation type such as "EXAMINE", "WALK" and returns
  /// the string to use in the navigation mode menu item.
  string navTypeToNavMenuString( const string &nav_type );

  //Pointer to WxWidgetsWindow
  WxWidgetsWindow *glwindow;

  /***************Interface Features*****************/
  //Main Menu Bar
  wxMenuBar  *menuBar;
  //Menu Bar Items
  wxMenu     *fileMenu, *rendererMenu, *viewpointMenu,
             *navigationMenu, *advancedMenu, *helpMenu;
  wxMenuItem *navigationDevices;
  //Submenu items
  wxMenu     *hapticsRenderer, *renderMode, *stereoRenderMode;
  //File History Menu
  wxFileHistory *recentFiles;

  //Config object to save information (settings, file history etc...)
  wxConfigBase *h3dConfig;

  bool loadIniFile();
  bool loadFile( const std::string &file );
  void clearData();

  /***************Member Functions*****************/
  void OnOpenFile( wxCommandEvent & event );
  void OnMRUFile( wxCommandEvent & event );
  void OnOpenFileURL( wxCommandEvent & event );
  void OnCloseFile( wxCommandEvent & event );
  void OnChooseDir( wxCommandEvent & event );
  void OnExit( wxCommandEvent & event );
  void OnWindowExit( wxCloseEvent & event );
  void OnAbout( wxCommandEvent & event );
  void OnHelp( wxCommandEvent & event );
  void ToggleFullscreen( wxCommandEvent & event );
  void MirrorScene( wxCommandEvent & event );
  void RenderMode( wxCommandEvent & event );
  void StereoRenderMode( wxCommandEvent & event );
  void ShowConsole( wxCommandEvent & event );
#ifdef HAVE_PROFILER
  void ShowProfiledResult( wxCommandEvent & event );
#endif
  void ShowTreeView( wxCommandEvent & event );
  void ShowPluginsDialog( wxCommandEvent & event );
  void ShowFrameRate( wxCommandEvent & event );
  void ShowProgramSettings( wxCommandEvent & event );
  void OnKeepViewpointOnLoadCheck( wxCommandEvent & event );
  void OnRouteSendsEventsCheck( wxCommandEvent & event );
  void OnLoadTexturesInThreadCheck( wxCommandEvent & event );
  void ChangeViewpoint( wxCommandEvent & event );
  void ResetViewpoint( wxCommandEvent & event );
  void ChangeNavigation( wxCommandEvent & event );
  void ChangeNavigationDevice( wxCommandEvent & event );
  void ChangeCollision( wxCommandEvent & event );
  void OnSpeed( wxCommandEvent & event );
  void ChangeRenderer( wxCommandEvent & event );
  //void ToggleHaptics( wxCommandEvent & event );
  void OnSettings( wxCommandEvent & event );
  void OnIdle( wxIdleEvent &event );
  void OnReload( wxCommandEvent &event);

  /***************Standard trivial functions***************/
  wxString GetCurrentFilename();
  void SetCurrentFilename(wxString);
  void SetCurrentPath(wxString);
  wxString GetCurrentPath();
  bool validateNavType(string);
  void SaveMRU ();
  void SaveSettings( bool to_config );
  void SaveCollisionOptions();
  void SaveDebugOptions( bool to_config );
  void SaveGeometryBoundTreeOptions( bool to_config );
  void SaveGraphicsCachingOptions( bool to_config );
  void SaveHapticsOptions( bool to_config );
  void SaveOpenHapticsOptions( bool to_config );
  void SaveStereoInfo( bool to_config );
  void SaveRuspiniSettings( bool to_config );
  void LoadMRU();
  void LoadSettings( bool from_config );
  void LoadPlugins();
  void buildNavMenu();
  void SetFullscreen( bool fullscreen );
  void BuildViewpointsMenu( list< Node * > vp_list );
  void BuildViewpointsSubMenu( list< Node* > vp_list, wxMenu * menu, int &count, int &unnamed_vp, int &unnamed_vg );
  void DestroyViewpointsMenu();
  void DestroyViewpointsSubMenu( wxMenu * menu );
  // returns a list of "top level" viewpoints and viewpoint groups
  list< Node * > GetTopLevelViews();

  void setProxyRadius( float r );

  void updateFrameRates() {
    frameRates->updateFrameRates();
  }

  bool isFileLoaded() {
    return a_file_is_loaded;
  }
private:
  struct NonConfigOptions {
    // DebugOptions
    bool draw_bound;
    int draw_tree;
    bool draw_triangles;

    // GeometryBoundTreeOptions
    string bound_type;
    int max_triangles_in_leaf;

    // GraphicsCachingOptions
    bool use_caching;
    bool cache_only_geometries;
    int caching_delay;
    string frustum_culling_mode;
    bool use_default_shadows;
    float default_shadow_darkness;
    float default_shadow_depth_offset;
    bool prefer_vertex_buffer_object;

    // HapticsOptions
    string touchable_face;
    string dynamic_mode;
    float max_distance;
    float look_ahead_factor;
    bool use_bound_tree;
    bool interpolate_force_effects;

    // OpenHapticsOptions
    bool use_adaptive_viewport;
    bool use_haptic_camera_view;
    string gl_shape;
    bool force_full_geometry_render;

    // StereoInfo
    float focal_distance;
    float interocular_distance;

    // Proxy radius used by ruspini
    float proxy_radius;
  };

  NonConfigOptions non_conf_opt;

  // CollisionOptions
  bool avatar_collision;

  wxString currentFilename;
  wxString currentPath;
  bool lastmirror;
  int navTypeCount;
  int deviceCount;
  list< Node * > VPlist;
  NavigationInfo *mynav;
  DeviceInfo *mydevice;
  H3DHapticsDevice *myH3Ddevice;
  NodeVector allDevices;
  X3DViewpointNode *current_viewpoint;
  string lastOpenedFilepath;

  // Check if the first file is loaded.
  bool loaded_first_file;
  // True if a file is currently loaded.
  bool a_file_is_loaded;

  // One time intialization variables
  string settings_path;
  string common_path;
  string deviceinfo_file;
  string stylus_file;
  string viewpoint_file;
  string render_mode;
  bool ini_fullscreen;
  bool ini_mirrored;
  bool manualCursorControl;

  // Autoref Variables
  AutoRef< Scene > scene;
  AutoRef< KeySensor > ks;
  AutoRef< Node > device_info;
  AutoRef< Node > viewpoint;


  X3D::DEFNodes default_stylus_dn;

  // Used in updating viewpoint menu
  TimeStamp last_viewmenu_update;

public:




  WxConsoleDialog * the_console;
#ifdef HAVE_PROFILER
  WxProfiledResultDialog * the_profiled_result;
#endif
  H3DViewerTreeViewDialog * tree_view_dialog;
#ifdef HAVE_WXPROPGRID
  H3DViewerFieldValuesDialogPropGrid * program_settings_dialog;
#endif
  H3DViewerPluginsDialog * plugins_dialog;
  MenuContainer *menu_container;
  FrameRateDialog *  frameRates;
  SettingsDialog * settings;
  SpeedDialog *speed_slider;

  AutoRef< GlobalSettings > global_settings;

  AutoRef< StereoInfo > stereo_info;

  long getFullScreenStyle();
  void hideAllDialogs();
  void showPreviouslyHiddenDialogs();
  bool check_dialogs_position_because_of_fullscreen_and_not_quadro;
  vector< wxWindow * > dialogs_hidden_because_of_fullscreen;

  friend class SettingsDialog;

  class ChangeNavType : public AutoUpdate< SFString > { 
  public:
    ChangeNavType() : glwindow( 0 ), speed_increment( 0.1f ) {}

    inline void setOwnerWindows( WxWidgetsWindow * owner_window,
                                 WxFrame *_frame ) {
      glwindow = owner_window;
      frame = _frame;
    }

  protected:
    virtual void update();

    WxWidgetsWindow *glwindow;
    WxFrame * frame;
    H3DFloat speed_increment;
  };

  auto_ptr< ChangeNavType > change_nav_type;
  map< int, X3DViewpointNode * > itemIdViewpointMap;
  int current_viewpoint_id;

  class HandleActionKey : public PeriodicUpdate< SFInt32 > { 
  public:
    HandleActionKey() : glwindow( NULL ), frame( NULL ) {}

    inline void setOwnerWindows( WxWidgetsWindow * owner_window,
                                 WxFrame *_frame ) {
      glwindow = owner_window;
      frame = _frame;
    }

  protected:
    virtual void update();

    WxWidgetsWindow *glwindow;
    WxFrame * frame;
  };
  auto_ptr< HandleActionKey > handle_action_key;

  class UpdateStereoModeMenu : public PeriodicUpdate< SFString > {
  public:
    UpdateStereoModeMenu() : frame( NULL ) {}

    inline void setOwnerWindow( WxFrame *_frame ) {
      frame = _frame;
    }

  protected:
    virtual void update();

    WxFrame * frame;
  };
  auto_ptr< UpdateStereoModeMenu > updateStereoModeMenu;

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
  FRAME_TOGGLE_FULLSCREEN,
  FRAME_MIRROR,
  FRAME_CONSOLE,
#ifdef HAVE_PROFILER
  FRAME_PROFILEDRESULT,
#endif
  FRAME_TREEVIEW,
  FRAME_PLUGINS,
  FRAME_FRAMERATE,
  FRAME_PROGRAMSETTINGS,
  FRAME_KEEPVIEWPOINTONLOAD,
  FRAME_ROUTESENDSEVENTS,
  FRAME_LOADTEXTURESINTHREAD,
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
  FRAME_RENDERMODE_DEFAULT,
  FRAME_RENDERMODE_FILLED,
  FRAME_RENDERMODE_WIREFRAME,
  FRAME_RENDERMODE_POINTS,
  FRAME_STEREORENDERMODE,
  FRAME_MONO,
  FRAME_QUADBUFFERED,
  FRAME_HORZSPLIT,
  FRAME_HORZSPLITKEEPASPECT,
  FRAME_VERTSPLIT,
  FRAME_VERTSPLITKEEPASPECT,
  FRAME_HORZINTERLACED,
  FRAME_VERTINTERLACED,
  FRAME_CHECKERINTERLACED,
  FRAME_SHARPDISPLAY,
  FRAME_REDBLUE,
  FRAME_REDGREEN,
  FRAME_REDCYAN,
  FRAME_NVIDIA_3DVISION,
  FRAME_HDMI720P,
  FRAME_HDMI1080P,
  //FRAME_DEVICECONTROL,
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
  BASIC_COLLISION,
  FRAME_SPEED,
  FRAME_SPEED_SLIDER,
  //FRAME_HELP,
  FRAME_SETTINGS,
  FRAME_RESET_VIEWPOINT,
  FRAME_MOUSE_NAV,
  FRAME_KEYBOARD_NAV,
  FRAME_SWS_NAV,
  FRAME_HAPTICSDEVICE_NAV,
  FRAME_RELOAD
};

