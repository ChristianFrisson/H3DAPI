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
/// \file PhysicsModels.h
/// \brief Header file for PhysicsModels
///
//
//////////////////////////////////////////////////////////////////////////////

#include <wx/wx.h>
#include <wx/propdlg.h> 
#include <wx/bookctrl.h>
#include <wx/docview.h>
#include <wx/spinctrl.h>
#include <wx/config.h>
#include <wx/confbase.h>
#include <wx/tokenzr.h>

#include <ParticleSystem.h>
#include <WindPhysicsModel.h>
#include <BoundedPhysicsModel.h>
#include <GravityPhysicsModel.h>


using namespace std;
using namespace H3D;

// ---------------------------------------------------------------------------
//  Bounded Physics Model Dialog Declaration
// ---------------------------------------------------------------------------

class BoundedPhysicsModelDialog: public wxPropertySheetDialog
{
DECLARE_CLASS(BoundedPhysicsModelDialog);
public:
  BoundedPhysicsModelDialog (wxWindow* parent, ParticleSystem* PS);
  ~BoundedPhysicsModelDialog ();

  void handleSettingsChange (wxCommandEvent & event);
  wxPanel* CreateBoundedPhysicsModelSettingsPage(wxWindow* parent);
  void getIndex ();

protected:
  enum {
    ID_ENABLED,
    ID_GEOMETRY,
  };

  wxImageList* m_imageList;
  ParticleSystem* ePS;
  BoundedPhysicsModel* boundedPhysicsModel;
  int index;

DECLARE_EVENT_TABLE()
};

// ---------------------------------------------------------------------------
//  Gravity Physics Model Dialog Declaration
// ---------------------------------------------------------------------------

class GravityPhysicsModelDialog: public wxPropertySheetDialog
{
DECLARE_CLASS(GravityPhysicsModelDialog);
public:
  GravityPhysicsModelDialog (wxWindow* parent, ParticleSystem* PS);
  ~GravityPhysicsModelDialog ();

  void handleSettingsChange (wxCommandEvent & event);
  wxPanel* CreateGravityPhysicsModelSettingsPage(wxWindow* parent);
  void getIndex();

protected:
  enum {
    ID_ENABLED,
    ID_GRAVITY,
  };

  wxImageList* m_imageList;
  ParticleSystem* ePS;
  GravityPhysicsModel* gravityPhysicsModel;
  double val;
  int index;

DECLARE_EVENT_TABLE()
};

// ---------------------------------------------------------------------------
//  Wind Physics Model Dialog Declaration
// ---------------------------------------------------------------------------

class WindPhysicsModelDialog: public wxPropertySheetDialog
{
DECLARE_CLASS(WindPhysicsModelDialog);
public:
  WindPhysicsModelDialog (wxWindow* parent, ParticleSystem* PS);
  ~WindPhysicsModelDialog ();

  void handleSettingsChange (wxCommandEvent & event);
  wxPanel* CreateWindPhysicsModelSettingsPage(wxWindow* parent);
  void getIndex ();

  //void setParticleSystem (ParticleSystem * PS);
  //void setEmitter (X3DParticleEmitterNode * emitter);

protected:
  enum {
    ID_SHOW_TOOLTIPS = 100,
    ID_ENABLED,
    ID_DIRECTION,
    ID_GUSTINESS,
    ID_SPEED,
    ID_TURBULENCE,
  };

  wxImageList*    m_imageList;
  ParticleSystem * ePS;
  WindPhysicsModel * windPhysicsModel;
  double val;
  int index;

DECLARE_EVENT_TABLE()
};