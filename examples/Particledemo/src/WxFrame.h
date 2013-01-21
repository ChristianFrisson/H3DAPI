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
/// \file H3DWxFrame.h
/// \brief Header file for H3DWxFrame
///
//
//////////////////////////////////////////////////////////////////////////////

// wx includes
#include <wx/wx.h>
#include <wx/propdlg.h> 
#include <wx/bookctrl.h>
#include <wx/docview.h>
#include <wx/spinctrl.h>
#include <wx/config.h>
#include <wx/confbase.h>

// H3D includes
#include <H3D/GlobalSettings.h>

#include <H3D/Box.h>
#include <H3D/Cone.h>
#include <H3D/Cylinder.h>
#include <H3D/Sphere.h>

#include <H3D/Group.h>
#include <H3D/Transform.h>
#include <H3D/Scene.h>
#include <H3D/KeySensor.h>
#include <H3D/MouseSensor.h>

#include <H3D/SpaceWareSensor.h>

#include <H3D/DeviceInfo.h>
#include <H3D/INIFile.h>
#include <H3D/ResourceResolver.h>
#include <H3D/ParticleSystem.h>
#include <H3D/NavigationInfo.h>

// H3DUtil includes
#include <H3DUtil/Console.h>

// ParticleDemo includes
#include <WxWidgetsWindow.h>
#include <ConsoleDialog.h>
#include <EmitterSettings.h>
#include <PhysicsModels.h>

using namespace std;
using namespace H3D;

// ---------------------------------------------------------------------------
//  WxFrame Definition
// ---------------------------------------------------------------------------

class WxFrame: public wxFrame
{
public:
  WxFrame( wxWindow *_parent, wxWindowID _id, const wxString& _title,
              const wxPoint& _pos, const wxSize& _size,
              long _style = wxDEFAULT_FRAME_STYLE,
              const wxString& name = wxT("Particle Demo") );

  //Pointer to H3DWxWidgetsWindow
  WxWidgetsWindow *glwindow;

  /***************Interface Features*****************/
  //Main Menu Bar
  wxMenuBar  *menuBar;
  //Menu Bar Items
  wxMenu     *fileMenu, *particleMenu, *physicsMenu, *emitterMenu, *advancedMenu, *helpMenu;
  //Sub menus
  wxMenu     *geometryMenu;
  
  bool loadFile( const string &file );
  void clearData();

  /***************Member Functions*****************/
  void OnOpenFile  (wxCommandEvent & event);
  void OnOpenFileURL  (wxCommandEvent & event);
  void OnCloseFile  (wxCommandEvent & event);
  void OnExit       (wxCommandEvent & event);
  void OnWindowExit (wxCloseEvent & event);
  void OnAbout    (wxCommandEvent & event);
  void OnHelp    (wxCommandEvent & event);
  void ShowConsole  (wxCommandEvent & event);
  void GetSelection (wxMenuEvent & event);
  void OnParticleGeometry (wxCommandEvent & event);
  void ToggleParticles (wxCommandEvent & event);
  void ParticleGeneration (wxCommandEvent & event);
  void PointEmitterSettingsDialog (wxCommandEvent & event);
  void PolylineEmitterSettingsDialog (wxCommandEvent & event);
  void VolumeEmitterSettingsDialog (wxCommandEvent & event);
  void ConeEmitterSettingsDialog (wxCommandEvent & event);
  void ExplosionEmitterSettingsDialog (wxCommandEvent & event);
  void SurfaceEmitterSettingsDialog (wxCommandEvent & event);
  void BoundedPhysicsModelSettingsDialog (wxCommandEvent & event);
  void WindPhysicsModelSettingsDialog (wxCommandEvent & event);
  void GravityPhysicsModelSettingsDialog (wxCommandEvent & event);

  /***************Standard trivial functions***************/
  wxString GetCurrentFilename();
  void SetCurrentFilename(wxString);
  void SetCurrentPath(wxString);
  wxString GetCurrentPath();
  void CreateGeometries();


  bool ParticleStatus;
  //void readSettingsFromINIFile( const string &filename,GlobalSettings *gs );

private:
  wxString currentFilename;
  wxString currentPath;
  DeviceInfo *di;
  H3DHapticsDevice *hdev;
  ParticleSystem *PS;

  int selection;

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

  ConsoleDialog *  the_console;

  //Emitters
  ConeEmitterDialog * coneEmitterSettings;
  PointEmitterDialog * pointEmitterSettings;
  PolylineEmitterDialog * polylineEmitterSettings;
  VolumeEmitterDialog * volumeEmitterSettings;
  ExplosionEmitterDialog * explosionEmitterSettings;
  SurfaceEmitterDialog * surfaceEmitterSettings;

  //Physics models
  BoundedPhysicsModelDialog * boundedPhysicsModelSettings;
  GravityPhysicsModelDialog * gravityPhysicsModelSettings;
  WindPhysicsModelDialog * windPhysicsModelSettings;

  //Geometries
  Box * box;
  Cone * cone;
  Cylinder * cylinder;
  Sphere * sphere;

protected:
  DECLARE_EVENT_TABLE()
};

enum
{ FRAME_EXIT    =   wxID_HIGHEST + 1,
  FRAME_OPEN,
  FRAME_OPEN_URL,
  FRAME_CLOSE,
  FRAME_FULLSCREEN,
  FRAME_RESTORE,
  FRAME_MIRROR,
  FRAME_CONSOLE,
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
  FRAME_HORZINTERLACED,
  FRAME_VERTINTERLACED,
  FRAME_SHARPDISPLAY,
  FRAME_REDBLUE,
  FRAME_REDCYAN,
  FRAME_PARTICLECONTROL,
  FRAME_PARTICLECREATION,
  FRAME_DEVICECONTROL,
  FRAME_HAPTICSCONTROL,
  FRAME_GEOMETRY,
  FRAME_QUAD,
  FRAME_TRIANGLE,
  FRAME_POINT,
  FRAME_SPRITE,
  FRAME_LINE,
  FRAME_CONEEMITTER,
  FRAME_POINTEMITTER,
  FRAME_POLYLINEEMITTER,
  FRAME_VOLUMEEMITTER,
  FRAME_EXPLOSIONEMITTER,
  FRAME_SURFACEEMITTER,
  FRAME_BOUNDEDPHYSICSMODEL,
  FRAME_GRAVITYPHYSICSMODEL,
  FRAME_WINDPHYSICSMODEL,
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
  FRAME_SETTINGS
};


