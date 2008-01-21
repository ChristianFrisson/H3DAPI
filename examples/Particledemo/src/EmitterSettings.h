//////////////////////////////////////////////////////////////////////////////
//    Copyright 2007, SenseGraphics AB
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
/// \file Emittersettings.h
/// \brief Header file for EmitterSettings
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
#include <wx/tokenzr.h>

// H3D includes
#include <H3D/PointEmitter.h>
#include <H3D/ConeEmitter.h>
#include <H3D/ExplosionEmitter.h>
#include <H3D/DeviceInfo.h>
#include <H3D/ParticleSystem.h>
#include <H3D/SurfaceEmitter.h>

using namespace std;
using namespace H3D;

// ---------------------------------------------------------------------------
//  Base Emitter Dialog Declaration
//  All dialogs are derived from this base class
// ---------------------------------------------------------------------------

class EmitterDialog: public wxPropertySheetDialog
{
DECLARE_CLASS(EmitterDialog);
public:
  EmitterDialog ();
  ~EmitterDialog ();

  void setParticleSystem (ParticleSystem * PS);
  void setEmitter (X3DParticleEmitterNode * emitter);
  void setDevice (H3DHapticsDevice * hdev);

protected:
    enum {
        ID_SHOW_TOOLTIPS = 100,

        ID_SPEED,
        ID_VARIATION,
        ID_MASS,
        ID_SURFACE_AREA,
    };

    wxImageList*    m_imageList;
    ParticleSystem * ePS;
    H3DHapticsDevice * ehdev;

DECLARE_EVENT_TABLE()
};

// ---------------------------------------------------------------------------
//  PointEmitter Dialog Declaration
// ---------------------------------------------------------------------------

class PointEmitterDialog: public EmitterDialog
{
DECLARE_CLASS(PointEmitterDialog);
public:
  PointEmitterDialog(wxWindow* parent, ParticleSystem* PS,
                     H3DHapticsDevice* hdev);
  ~PointEmitterDialog();

  void handleSettingsChange (wxCommandEvent & event);
  wxPanel* CreatePointEmitterSettingsPage(wxWindow* parent);  

protected:
  enum {
    ID_DIRECTION,
    ID_POSITION,
  };
  wxImageList*    m_imageList;
  PointEmitter * pointEmitter;
  double val;

DECLARE_EVENT_TABLE()
};

// ---------------------------------------------------------------------------
//  PolylineEmitter Dialog Declaration
// ---------------------------------------------------------------------------

class PolylineEmitterDialog: public EmitterDialog
{
DECLARE_CLASS(PolylineEmitterDialog)
public:
    PolylineEmitterDialog(wxWindow* parent, ParticleSystem* PS,
                          H3DHapticsDevice* hdev);
    ~PolylineEmitterDialog();

  /*void handleSettingsChange (wxCommandEvent & event);
  void handleSpinEvent (wxSpinEvent & event);
  void OnOk (wxCommandEvent & event);
  void OnCancel (wxCommandEvent & event);*/

  wxPanel* CreatePolylineEmitterSettingsPage(wxWindow* parent);  
  
protected:

    enum {
        ID_COORD_INDEX,
    };

    wxImageList*    m_imageList;
//    PolylineEmitter * polylineEmitter;
//    ParticleSystem * ePS;

DECLARE_EVENT_TABLE()
};

// ---------------------------------------------------------------------------
//  VolumeEmitter Dialog Declaration
// ---------------------------------------------------------------------------

class VolumeEmitterDialog: public EmitterDialog
{
DECLARE_CLASS(VolumeEmitterDialog)
public:
    VolumeEmitterDialog(wxWindow* parent, ParticleSystem* PS,
                        H3DHapticsDevice* hdev);
    ~VolumeEmitterDialog();

  /*void handleSettingsChange (wxCommandEvent & event);
  void handleSpinEvent (wxSpinEvent & event);
  void OnOk (wxCommandEvent & event);
  void OnCancel (wxCommandEvent & event);*/

  wxPanel* CreateVolumeEmitterSettingsPage(wxWindow* parent);  
  
protected:

    enum {
        ID_COORD_INDEX,
    };

    wxImageList*    m_imageList;
//    VolumeEmitter * volumeEmitter;
//    ParticleSystem * ePS;

DECLARE_EVENT_TABLE()
};

// ---------------------------------------------------------------------------
//  ConeEmitter Dialog Declaration
// ---------------------------------------------------------------------------

class ConeEmitterDialog: public EmitterDialog
{
DECLARE_CLASS(ConeEmitterDialog);
public:
  ConeEmitterDialog(wxWindow* parent, ParticleSystem* PS, 
                    H3DHapticsDevice * hdev);
  ~ConeEmitterDialog();

  void handleSettingsChange (wxCommandEvent & event);
  wxPanel* CreateConeEmitterSettingsPage(wxWindow* parent);  

protected:
  enum {
    ID_DIRECTION,
    ID_POSITION,
  };
  wxImageList*    m_imageList;
  ConeEmitter * coneEmitter;
  double val;

DECLARE_EVENT_TABLE()
};

// ---------------------------------------------------------------------------
//  ExplosionEmitter Dialog Declaration
// ---------------------------------------------------------------------------

class ExplosionEmitterDialog: public EmitterDialog
{
DECLARE_CLASS(ExplosionEmitterDialog);
public:
  ExplosionEmitterDialog(wxWindow* parent, ParticleSystem* PS,
                         H3DHapticsDevice* hdev);
  ~ExplosionEmitterDialog();

  void handleSettingsChange (wxCommandEvent & event);
  wxPanel* CreateExplosionEmitterSettingsPage(wxWindow* parent);  

protected:
  enum {
    ID_POSITION,
  };
  wxImageList*    m_imageList;
  ExplosionEmitter * explosionEmitter;
  double val;

DECLARE_EVENT_TABLE()
};

// ---------------------------------------------------------------------------
//  SurfaceEmitter Dialog Declaration
// ---------------------------------------------------------------------------

class SurfaceEmitterDialog: public EmitterDialog
{
DECLARE_CLASS(SurfaceEmitterDialog);
public:
  SurfaceEmitterDialog(wxWindow* parent, ParticleSystem* PS,
                       H3DHapticsDevice* hdev);
  ~SurfaceEmitterDialog();

  void handleSettingsChange (wxCommandEvent & event);
  wxPanel* CreateSurfaceEmitterSettingsPage(wxWindow* parent);  

protected:
  enum {
    ID_COORD_INDEX,
    ID_SURFACE,
  };
  wxImageList*    m_imageList;
  SurfaceEmitter * surfaceEmitter;
  double val;

DECLARE_EVENT_TABLE()
};

//class SurfaceEmitterGeometryDialog: public wxPropertySheetDialog
//{
//DECLARE_CLASS(SurfaceEmitterGeometryDialog);
//public:
//  EmitterDialog ();
//  ~EmitterDialog ();
//
//  void setParticleSystem (ParticleSystem * PS);
//  void setEmitter (X3DParticleEmitterNode * emitter);
//
//protected:
//    enum {
//        ID_SHOW_TOOLTIPS = 100,
//
//        ID_SPEED,
//        ID_VARIATION,
//        ID_MASS,
//        ID_SURFACE_AREA,
//    };
//
//    wxImageList*    m_imageList;
//    ParticleSystem * ePS;
//
//DECLARE_EVENT_TABLE()
//};
