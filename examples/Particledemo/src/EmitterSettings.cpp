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
/// \file EmitterSettings.cpp
/// \brief Implementation file for EmitterSettings
///
//
//////////////////////////////////////////////////////////////////////////////

#include <EmitterSettings.h>

using namespace std;
using namespace H3D;
using namespace HAPI;

// ---------------------------------------------------------------------------
//  Base Class: EmitterDialog Definition
// ---------------------------------------------------------------------------

IMPLEMENT_CLASS(EmitterDialog, wxPropertySheetDialog)

BEGIN_EVENT_TABLE(EmitterDialog, wxPropertySheetDialog)
END_EVENT_TABLE()

EmitterDialog::EmitterDialog()
{

}

EmitterDialog::~EmitterDialog()
{

}

void EmitterDialog::setParticleSystem(ParticleSystem *PS)
{
  ePS = PS;
}

void EmitterDialog::setDevice(H3DHapticsDevice *hdev)
{
  ehdev = hdev;
}

// ---------------------------------------------------------------------------
//  PointEmitterDialog Definition
// ---------------------------------------------------------------------------

IMPLEMENT_CLASS(PointEmitterDialog, wxPropertySheetDialog)

BEGIN_EVENT_TABLE(PointEmitterDialog, wxPropertySheetDialog)
  EVT_TEXT(ID_SPEED, PointEmitterDialog::handleSettingsChange)
  EVT_TEXT(ID_VARIATION, PointEmitterDialog::handleSettingsChange)
  EVT_TEXT(ID_MASS, PointEmitterDialog::handleSettingsChange)
  EVT_TEXT(ID_SURFACE_AREA, PointEmitterDialog::handleSettingsChange)
  EVT_TEXT(ID_DIRECTION, PointEmitterDialog::handleSettingsChange)
  EVT_TEXT(ID_POSITION, PointEmitterDialog::handleSettingsChange)
END_EVENT_TABLE()


PointEmitterDialog::PointEmitterDialog(wxWindow* win, ParticleSystem* PS,
                                       H3DHapticsDevice* hdev)
{
    SetExtraStyle(wxDIALOG_EX_CONTEXTHELP|wxWS_EX_VALIDATE_RECURSIVELY);

    int tabImage1 = -1;
    int tabImage2 = -1;
    int tabImage3 = -1;

    int resizeBorder = wxRESIZE_BORDER;

    m_imageList = NULL;

    Create(win, wxID_ANY, _("Point Emitter"), wxDefaultPosition, wxDefaultSize,
        wxDEFAULT_DIALOG_STYLE| (int)wxPlatform::IfNot(wxOS_WINDOWS_CE, 
        resizeBorder)
    );

    // If using a toolbook, also follow Mac style and don't create buttons
    CreateButtons(wxOK | wxCANCEL |
                        (int)wxPlatform::IfNot(wxOS_WINDOWS_CE, wxHELP)
    ); 

    wxBookCtrlBase* notebook = GetBookCtrl();
    notebook->SetImageList(m_imageList);

    wxPanel* PointEmitterSettings = CreatePointEmitterSettingsPage(notebook);

    notebook->AddPage(PointEmitterSettings, _("Particle Properties"), true, 
                      tabImage1);

    LayoutDialog();

    //Set pointEmitter pointer to NULL initally
    pointEmitter = (PointEmitter *) NULL;
    pointEmitter = new PointEmitter;
    //Make ePS point to frame ParticleSystem
    //PS->emitter->setValue(pointEmitter);
    setParticleSystem (PS);
    setDevice (hdev);
    //PS->emitter->setValue(pointEmitter);
}

PointEmitterDialog::~PointEmitterDialog()
{

}

wxPanel* PointEmitterDialog::CreatePointEmitterSettingsPage(wxWindow* parent)
{
    wxPanel* panel = new wxPanel(parent, wxID_ANY);

    wxBoxSizer *topSizer = new wxBoxSizer( wxVERTICAL );
    wxBoxSizer *item0 = new wxBoxSizer( wxVERTICAL );

    //// SPEED

    wxBoxSizer* speedSizer = new wxBoxSizer( wxHORIZONTAL );
    speedSizer->Add(new wxStaticText(panel, wxID_ANY, 
                    _("&Speed            :")), 0, 
                    wxALL|wxALIGN_CENTER_VERTICAL, 5);
    wxTextCtrl* pointEmitterSpeed = new wxTextCtrl (panel, ID_SPEED, 
                                                   wxEmptyString, 
                                                   wxDefaultPosition, 
                                                   wxSize(40, wxDefaultCoord));
    speedSizer->Add(pointEmitterSpeed, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
    item0->Add(speedSizer, 0, wxGROW|wxALL, 0);

    ////VARIATION

    wxBoxSizer* variationSizer = new wxBoxSizer( wxHORIZONTAL );
    variationSizer->Add(new wxStaticText(panel, wxID_ANY, 
                        _("&Variation        :")), 0, 
                        wxALL|wxALIGN_CENTER_VERTICAL, 5);
    wxTextCtrl* pointEmitterSpeedVariation = new wxTextCtrl (panel, 
                                                 ID_VARIATION, 
                                                 wxEmptyString, 
                                                 wxDefaultPosition, 
                                                 wxSize(40, wxDefaultCoord));
    variationSizer->Add(pointEmitterSpeedVariation, 0, 
                        wxALL|wxALIGN_CENTER_VERTICAL, 5);
    item0->Add(variationSizer, 0, wxGROW|wxALL, 0);

    //// MASS

    wxBoxSizer* massSizer = new wxBoxSizer( wxHORIZONTAL );
    massSizer->Add(new wxStaticText(panel, wxID_ANY, 
                                    _("&Mass              :")), 0, 
                                    wxALL|wxALIGN_CENTER_VERTICAL, 5);
    wxTextCtrl* pointEmitterMass = new wxTextCtrl(panel, ID_MASS, 
                                                  wxEmptyString, 
                                                  wxDefaultPosition, 
                                                  wxSize(40, wxDefaultCoord));
    massSizer->Add(pointEmitterMass, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
    item0->Add(massSizer, 0, wxGROW|wxALL, 0);

    //// SURFACE AREA

    wxBoxSizer* areaSizer = new wxBoxSizer( wxHORIZONTAL );
    areaSizer->Add(new wxStaticText(panel, wxID_ANY, _("&Surface Area :")), 0, 
                   wxALL|wxALIGN_CENTER_VERTICAL, 5);
    wxTextCtrl* pointEmitterArea = new wxTextCtrl(panel, ID_SURFACE_AREA, 
                                                  wxEmptyString, 
                                                  wxDefaultPosition, 
                                                  wxSize(40, wxDefaultCoord));
    areaSizer->Add(pointEmitterArea, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
    item0->Add(areaSizer, 0, wxGROW|wxALL, 0);

    //// DIRECTION
    wxBoxSizer* directionSizer = new wxBoxSizer( wxHORIZONTAL );
    directionSizer->Add(new wxStaticText(panel, wxID_ANY, 
                        _("&Direction        :")), 0, 
                        wxALL|wxALIGN_CENTER_VERTICAL, 5);
    wxTextCtrl* pointEmitterDirection = new wxTextCtrl(panel, ID_DIRECTION, 
                                                   wxEmptyString, 
                                                   wxDefaultPosition, 
                                                   wxSize(40, wxDefaultCoord));
    directionSizer->Add(pointEmitterDirection, 0, 
                        wxALL|wxALIGN_CENTER_VERTICAL, 5);
    item0->Add(directionSizer, 0, wxGROW|wxALL, 0);

    //// POSITION
    wxBoxSizer* positionSizer = new wxBoxSizer( wxHORIZONTAL );
    positionSizer->Add(new wxStaticText(panel, wxID_ANY, 
                        _("&Position          :")), 0, 
                        wxALL|wxALIGN_CENTER_VERTICAL, 5);
    wxTextCtrl* pointEmitterPosition = new wxTextCtrl(panel, ID_POSITION, 
                                                   wxEmptyString, 
                                                   wxDefaultPosition, 
                                                   wxSize(40, wxDefaultCoord));
    positionSizer->Add(pointEmitterPosition, 0, 
                       wxALL|wxALIGN_CENTER_VERTICAL, 5);
    item0->Add(positionSizer, 0, wxGROW|wxALL, 0);

    topSizer->Add( item0, 1, wxGROW|wxALIGN_CENTRE|wxALL, 5 );

    panel->SetSizer(topSizer);
    topSizer->Fit(panel);

    return panel;
}

void PointEmitterDialog::handleSettingsChange (wxCommandEvent & event) {
  int id = event.GetId();
  pointEmitter = dynamic_cast<PointEmitter *> (ePS->emitter->getValue());
  if (!pointEmitter) {
    pointEmitter = new PointEmitter;
    ePS->emitter->setValue(pointEmitter);
    ehdev->trackerPosition->route(pointEmitter->position);
  }

  if ( id == ID_SPEED )
    pointEmitter->speed->setValue( atof( event.GetString() ) );
  else if (id == ID_VARIATION)
    pointEmitter->variation->setValue ( atof( event.GetString() ) );
  else if (id == ID_MASS )
    pointEmitter->mass->setValue( atof( event.GetString() ) );
  else if (id == ID_SURFACE_AREA )
    pointEmitter->surfaceArea->setValue( atof( event.GetString() ) );
  else if (id == ID_DIRECTION) {
    wxString temp_string = event.GetString();
    wxStringTokenizer vec3_tkz( temp_string, wxT(" "), wxTOKEN_DEFAULT );
    vector< double > direction_values;
    while ( vec3_tkz.HasMoreTokens() ) {
        wxString value = vec3_tkz.GetNextToken();
        if ( value.ToDouble(&val) ) {
          direction_values.push_back( val );
        }
    }
    vector< double >::iterator iter = direction_values.begin();
    int size = direction_values.size();
    // Default values
    double x = 0, y = 1, z = 0;
    switch (size) {
      case 0:
        break;
      case 1:
        x = *iter;
        y = 0;
        z = 0;
        break;
      case 2:
        x = *iter;
        iter++;
        y = *iter;
        z = 0;
        break;
      case 3:
        x = *iter;
        iter++;
        y = *iter;
        iter++;
        z = *iter;
        break;
    }
    pointEmitter->direction->setValue(Vec3f(x, y, z));
  }
  else if (id == ID_POSITION) {
    wxString temp_string = event.GetString();
    wxStringTokenizer vec3_tkz( temp_string, wxT(" "), wxTOKEN_DEFAULT );
    vector< double > position_values;
    while ( vec3_tkz.HasMoreTokens() ) {
        wxString value = vec3_tkz.GetNextToken();
        if ( value.ToDouble(&val) ) {
          position_values.push_back( val );
        }
    }
    vector< double >::iterator iter = position_values.begin();
    int size = position_values.size();
    // Default values
    double x = 0, y = 0, z = 0;
    switch (size) {
      case 0:
        break;
      case 1:
        x = *iter;
        y = 0;
        z = 0;
        break;
      case 2:
        x = *iter;
        iter++;
        y = *iter;
        z = 0;
        break;
      case 3:
        x = *iter;
        iter++;
        y = *iter;
        iter++;
        z = *iter;
        break;
    }
    pointEmitter->position->setValue(Vec3f(x, y, z));
  }
}

// ---------------------------------------------------------------------------
//  PolylineEmitterDialog Definition
// ---------------------------------------------------------------------------

IMPLEMENT_CLASS(PolylineEmitterDialog, wxPropertySheetDialog)

BEGIN_EVENT_TABLE(PolylineEmitterDialog, wxPropertySheetDialog)

END_EVENT_TABLE()


PolylineEmitterDialog::PolylineEmitterDialog(wxWindow* win, ParticleSystem* PS,
                                             H3DHapticsDevice* hdev)
{
    SetExtraStyle(wxDIALOG_EX_CONTEXTHELP|wxWS_EX_VALIDATE_RECURSIVELY);

    int tabImage1 = -1;
    int tabImage2 = -1;
    int tabImage3 = -1;

    int resizeBorder = wxRESIZE_BORDER;

    m_imageList = NULL;

    Create(win, wxID_ANY, _("Polyline Emitter"), wxDefaultPosition, 
           wxDefaultSize, wxDEFAULT_DIALOG_STYLE| 
           (int)wxPlatform::IfNot(wxOS_WINDOWS_CE, resizeBorder)
    );

    // If using a toolbook, also follow Mac style and don't create buttons
    CreateButtons(wxOK | wxCANCEL |
                        (int)wxPlatform::IfNot(wxOS_WINDOWS_CE, wxHELP)
    ); 

    wxBookCtrlBase* notebook = GetBookCtrl();
    notebook->SetImageList(m_imageList);

    wxPanel* PolylineEmitterSettings = 
      CreatePolylineEmitterSettingsPage(notebook);

    notebook->AddPage(PolylineEmitterSettings, _("Properties"), true, 
                      tabImage1);

    LayoutDialog();

    //Set polylineEmitter pointer to NULL initally
//    polylineEmitter = (PolylineEmitter *) NULL;
//    polylineEmitter = new PolylineEmitter;
    //Make ePS point to frame ParticleSystem
    setParticleSystem (PS);
    setDevice (hdev);

}

PolylineEmitterDialog::~PolylineEmitterDialog()
{

}

wxPanel* PolylineEmitterDialog::
  CreatePolylineEmitterSettingsPage(wxWindow* parent)
{
    wxPanel* panel = new wxPanel(parent, wxID_ANY);

    wxBoxSizer *topSizer = new wxBoxSizer( wxVERTICAL );
    wxBoxSizer *item0 = new wxBoxSizer( wxVERTICAL );

    //// SPEED

    wxBoxSizer* speedSizer = new wxBoxSizer( wxHORIZONTAL );
    speedSizer->Add(new wxStaticText(panel, wxID_ANY, 
                    _("&Particle Speed            :")), 0, 
                    wxALL|wxALIGN_CENTER_VERTICAL, 5);
    wxTextCtrl* polylineEmitterSpeed = new wxTextCtrl (panel, ID_SPEED, 
                                                   wxEmptyString, 
                                                   wxDefaultPosition, 
                                                   wxSize(40, wxDefaultCoord));
    speedSizer->Add(polylineEmitterSpeed, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
    item0->Add(speedSizer, 0, wxGROW|wxALL, 0);

    //// COORDINATE INDEX

    wxBoxSizer* coordIndexSizer = new wxBoxSizer( wxHORIZONTAL );
    coordIndexSizer->Add(new wxStaticText(panel, wxID_ANY, 
                         _("&Coordinate Index       :")), 0, 
                         wxALL|wxALIGN_CENTER_VERTICAL, 5);
    wxTextCtrl* polylineEmitterCoordIndex = new wxTextCtrl (panel, 
                                            ID_COORD_INDEX, 
                                            wxEmptyString, 
                                            wxDefaultPosition, 
                                            wxSize(40, wxDefaultCoord));
    coordIndexSizer->Add(polylineEmitterCoordIndex, 0, 
                         wxALL|wxALIGN_CENTER_VERTICAL, 5);
    item0->Add(coordIndexSizer, 0, wxGROW|wxALL, 0);

    //// MASS

    wxBoxSizer* massSizer = new wxBoxSizer( wxHORIZONTAL );
    massSizer->Add(new wxStaticText(panel, wxID_ANY, 
                   _("&Particle Mass              :")), 0, 
                   wxALL|wxALIGN_CENTER_VERTICAL, 5);
    wxTextCtrl* polylineEmitterMass = new wxTextCtrl(panel, ID_MASS, 
                                      wxEmptyString, 
                                      wxDefaultPosition, 
                                      wxSize(40, wxDefaultCoord));
    massSizer->Add(polylineEmitterMass, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
    item0->Add(massSizer, 0, wxGROW|wxALL, 0);

    //// SURFACE AREA

    wxBoxSizer* areaSizer = new wxBoxSizer( wxHORIZONTAL );
    areaSizer->Add(new wxStaticText(panel, wxID_ANY, 
                   _("&Particle Surface Area :")), 0, 
                   wxALL|wxALIGN_CENTER_VERTICAL, 5);
    wxTextCtrl* polylineEmitterArea = new wxTextCtrl(panel, ID_MASS, 
                                                  wxEmptyString, 
                                                  wxDefaultPosition, 
                                                  wxSize(40, wxDefaultCoord));
    areaSizer->Add(polylineEmitterArea, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
    item0->Add(areaSizer, 0, wxGROW|wxALL, 0);

    topSizer->Add( item0, 1, wxGROW|wxALIGN_CENTRE|wxALL, 5 );

    panel->SetSizer(topSizer);
    topSizer->Fit(panel);

    return panel;
}

// ---------------------------------------------------------------------------
//  VolumeEmitterDialog Definition
// ---------------------------------------------------------------------------

IMPLEMENT_CLASS(VolumeEmitterDialog, wxPropertySheetDialog)

BEGIN_EVENT_TABLE(VolumeEmitterDialog, wxPropertySheetDialog)

END_EVENT_TABLE()


VolumeEmitterDialog::VolumeEmitterDialog(wxWindow* win, ParticleSystem* PS,
                                         H3DHapticsDevice* hdev)
                                         
{
    SetExtraStyle(wxDIALOG_EX_CONTEXTHELP|wxWS_EX_VALIDATE_RECURSIVELY);

    int tabImage1 = -1;
    int tabImage2 = -1;
    int tabImage3 = -1;

    int resizeBorder = wxRESIZE_BORDER;

    m_imageList = NULL;

    Create(win, wxID_ANY, _("Volume Emitter"), wxDefaultPosition, wxDefaultSize,
        wxDEFAULT_DIALOG_STYLE| (int)wxPlatform::IfNot(wxOS_WINDOWS_CE, 
        resizeBorder)
    );

    // If using a toolbook, also follow Mac style and don't create buttons
    CreateButtons(wxOK | wxCANCEL |
                        (int)wxPlatform::IfNot(wxOS_WINDOWS_CE, wxHELP)
    ); 

    wxBookCtrlBase* notebook = GetBookCtrl();
    notebook->SetImageList(m_imageList);

    wxPanel* VolumeEmitterSettings = CreateVolumeEmitterSettingsPage(notebook);

    notebook->AddPage(VolumeEmitterSettings, _("Properties"), true, tabImage1);

    LayoutDialog();

    setParticleSystem (PS);
    setDevice (hdev);

}

VolumeEmitterDialog::~VolumeEmitterDialog()
{

}

wxPanel* VolumeEmitterDialog::CreateVolumeEmitterSettingsPage(wxWindow* parent)
{
    wxPanel* panel = new wxPanel(parent, wxID_ANY);

    wxBoxSizer *topSizer = new wxBoxSizer( wxVERTICAL );
    wxBoxSizer *item0 = new wxBoxSizer( wxVERTICAL );

    //// SPEED

    wxBoxSizer* speedSizer = new wxBoxSizer( wxHORIZONTAL );
    speedSizer->Add(new wxStaticText(panel, wxID_ANY, 
                    _("&Particle Speed            :")), 
                    0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
    wxTextCtrl* volumeEmitterSpeed = new wxTextCtrl (panel, ID_SPEED, 
                                                   wxEmptyString, 
                                                   wxDefaultPosition, 
                                                   wxSize(40, wxDefaultCoord));
    speedSizer->Add(volumeEmitterSpeed, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
    item0->Add(speedSizer, 0, wxGROW|wxALL, 0);

    //// COORDINATE INDEX

    wxBoxSizer* coordIndexSizer = new wxBoxSizer( wxHORIZONTAL );
    coordIndexSizer->Add(new wxStaticText(panel, wxID_ANY, 
                         _("&Coordinate Index       :")), 0, 
                         wxALL|wxALIGN_CENTER_VERTICAL, 5);
    wxTextCtrl* volumeEmitterCoordIndex = new wxTextCtrl (panel, 
                                                ID_COORD_INDEX, 
                                                wxEmptyString, 
                                                wxDefaultPosition, 
                                                wxSize(40, wxDefaultCoord));
    coordIndexSizer->Add(volumeEmitterCoordIndex, 0, 
                         wxALL|wxALIGN_CENTER_VERTICAL, 5);
    item0->Add(coordIndexSizer, 0, wxGROW|wxALL, 0);

    //// MASS

    wxBoxSizer* massSizer = new wxBoxSizer( wxHORIZONTAL );
    massSizer->Add(new wxStaticText(panel, wxID_ANY, 
                   _("&Particle Mass              :")), 0, 
                   wxALL|wxALIGN_CENTER_VERTICAL, 5);
    wxTextCtrl* volumeEmitterMass = new wxTextCtrl(panel, ID_MASS, 
                                                wxEmptyString, 
                                                wxDefaultPosition, 
                                                wxSize(40, wxDefaultCoord));
    massSizer->Add(volumeEmitterMass, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
    item0->Add(massSizer, 0, wxGROW|wxALL, 0);

    //// SURFACE AREA

    wxBoxSizer* areaSizer = new wxBoxSizer( wxHORIZONTAL );
    areaSizer->Add(new wxStaticText(panel, wxID_ANY, 
                   _("&Particle Surface Area :")), 0, 
                   wxALL|wxALIGN_CENTER_VERTICAL, 5);
    wxTextCtrl* volumeEmitterArea = new wxTextCtrl(panel, ID_MASS, 
                                          wxEmptyString, 
                                          wxDefaultPosition, 
                                          wxSize(40, wxDefaultCoord));
    areaSizer->Add(volumeEmitterArea, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
    item0->Add(areaSizer, 0, wxGROW|wxALL, 0);

    topSizer->Add( item0, 1, wxGROW|wxALIGN_CENTRE|wxALL, 5 );

    panel->SetSizer(topSizer);
    topSizer->Fit(panel);

    return panel;
}

// ---------------------------------------------------------------------------
//  ConeEmitterDialog Definition
// ---------------------------------------------------------------------------

IMPLEMENT_CLASS(ConeEmitterDialog, wxPropertySheetDialog)

BEGIN_EVENT_TABLE(ConeEmitterDialog, wxPropertySheetDialog)
  EVT_TEXT(ID_SPEED, ConeEmitterDialog::handleSettingsChange)
  EVT_TEXT(ID_VARIATION, ConeEmitterDialog::handleSettingsChange)
  EVT_TEXT(ID_MASS, ConeEmitterDialog::handleSettingsChange)
  EVT_TEXT(ID_SURFACE_AREA, ConeEmitterDialog::handleSettingsChange)
  EVT_TEXT(ID_DIRECTION, ConeEmitterDialog::handleSettingsChange)
  EVT_TEXT(ID_POSITION, ConeEmitterDialog::handleSettingsChange)
END_EVENT_TABLE()


ConeEmitterDialog::ConeEmitterDialog(wxWindow* win, ParticleSystem* PS,
                                     H3DHapticsDevice* hdev)
{
    SetExtraStyle(wxDIALOG_EX_CONTEXTHELP|wxWS_EX_VALIDATE_RECURSIVELY);

    int tabImage1 = -1;
    int tabImage2 = -1;
    int tabImage3 = -1;

    int resizeBorder = wxRESIZE_BORDER;

    m_imageList = NULL;

    Create(win, wxID_ANY, _("Cone Emitter"), wxDefaultPosition, wxDefaultSize,
        wxDEFAULT_DIALOG_STYLE| (int)wxPlatform::IfNot(wxOS_WINDOWS_CE, 
        resizeBorder)
    );

    // If using a toolbook, also follow Mac style and don't create buttons
    CreateButtons(wxOK | wxCANCEL |
                        (int)wxPlatform::IfNot(wxOS_WINDOWS_CE, wxHELP)
    ); 

    wxBookCtrlBase* notebook = GetBookCtrl();
    notebook->SetImageList(m_imageList);

    wxPanel* ConeEmitterSettings = CreateConeEmitterSettingsPage(notebook);

    notebook->AddPage(ConeEmitterSettings, _("Particle Properties"), true, 
                      tabImage1);

    LayoutDialog();


    //Set coneEmitter pointer to NULL initally
    coneEmitter = (ConeEmitter *) NULL;
    //coneEmitter = new ConeEmitter;
    //Make ePS point to frame ParticleSystem
    setParticleSystem (PS);
    setDevice (hdev);
}

ConeEmitterDialog::~ConeEmitterDialog()
{

}

wxPanel* ConeEmitterDialog::CreateConeEmitterSettingsPage(wxWindow* parent)
{
    wxPanel* panel = new wxPanel(parent, wxID_ANY);

    wxBoxSizer *topSizer = new wxBoxSizer( wxVERTICAL );
    wxBoxSizer *item0 = new wxBoxSizer( wxVERTICAL );

    //// SPEED

    wxBoxSizer* speedSizer = new wxBoxSizer( wxHORIZONTAL );
    speedSizer->Add(new wxStaticText(panel, wxID_ANY, 
      _("&Speed            :")), 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
    //wxString item = wxString ("") << coneEmitter->speed->getValue();
    //coneEmitterSpeed << item;
    wxTextCtrl* coneEmitterSpeed = new wxTextCtrl (panel, ID_SPEED,
      wxEmptyString, wxDefaultPosition, wxSize(40, wxDefaultCoord));
    speedSizer->Add(coneEmitterSpeed, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
    item0->Add(speedSizer, 0, wxGROW|wxALL, 0);

    ////VARIATION

    wxBoxSizer* variationSizer = new wxBoxSizer( wxHORIZONTAL );
    variationSizer->Add(new wxStaticText(panel, wxID_ANY, 
                        _("&Variation        :")), 0, 
                        wxALL|wxALIGN_CENTER_VERTICAL, 5);
    wxTextCtrl* coneEmitterSpeedVariation = new wxTextCtrl (panel, 
                                                   ID_VARIATION, 
                                                   wxEmptyString, 
                                                   wxDefaultPosition, 
                                                   wxSize(40, wxDefaultCoord));
    variationSizer->Add(coneEmitterSpeedVariation, 0, 
                        wxALL|wxALIGN_CENTER_VERTICAL, 5);
    item0->Add(variationSizer, 0, wxGROW|wxALL, 0);

    //// MASS

    wxBoxSizer* massSizer = new wxBoxSizer( wxHORIZONTAL );
    massSizer->Add(new wxStaticText(panel, wxID_ANY, 
                    _("&Mass              :")), 
                    0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
    wxTextCtrl* coneEmitterMass = new wxTextCtrl(panel, ID_MASS, 
                                            wxEmptyString, 
                                            wxDefaultPosition, 
                                            wxSize(40, wxDefaultCoord));
    massSizer->Add(coneEmitterMass, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
    item0->Add(massSizer, 0, wxGROW|wxALL, 0);

    //// SURFACE AREA

    wxBoxSizer* areaSizer = new wxBoxSizer( wxHORIZONTAL );
    areaSizer->Add(new wxStaticText(panel, wxID_ANY, _("&Surface Area :")), 0, 
                                    wxALL|wxALIGN_CENTER_VERTICAL, 5);
    wxTextCtrl* coneEmitterArea = new wxTextCtrl(panel, ID_MASS, wxEmptyString,
                                                 wxDefaultPosition, 
                                                 wxSize(40, wxDefaultCoord));
    areaSizer->Add(coneEmitterArea, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
    item0->Add(areaSizer, 0, wxGROW|wxALL, 0);

    //// DIRECTION
    wxBoxSizer* directionSizer = new wxBoxSizer( wxHORIZONTAL );
    directionSizer->Add(new wxStaticText(panel, wxID_ANY, 
                        _("&Direction        :")), 0, 
                        wxALL|wxALIGN_CENTER_VERTICAL, 5);
    wxTextCtrl* coneEmitterDirection = new wxTextCtrl(panel, ID_DIRECTION, 
                                                wxEmptyString, 
                                                wxDefaultPosition,
                                                wxSize(40, wxDefaultCoord));
    directionSizer->Add(coneEmitterDirection, 0, wxALL|wxALIGN_CENTER_VERTICAL,
                        5);
    item0->Add(directionSizer, 0, wxGROW|wxALL, 0);

    //// POSITION
    wxBoxSizer* positionSizer = new wxBoxSizer( wxHORIZONTAL );
    positionSizer->Add(new wxStaticText(panel, wxID_ANY, 
                        _("&Position          :")), 0, 
                        wxALL|wxALIGN_CENTER_VERTICAL, 5);
    wxTextCtrl* coneEmitterPosition = new wxTextCtrl(panel, ID_POSITION, 
                                              wxEmptyString, 
                                              wxDefaultPosition, 
                                              wxSize(40, wxDefaultCoord));
    positionSizer->Add(coneEmitterPosition, 0, wxALL|wxALIGN_CENTER_VERTICAL,
                       5);
    item0->Add(positionSizer, 0, wxGROW|wxALL, 0);

    topSizer->Add( item0, 1, wxGROW|wxALIGN_CENTRE|wxALL, 5 );

    panel->SetSizer(topSizer);
    topSizer->Fit(panel);

    return panel;
}

void ConeEmitterDialog::handleSettingsChange (wxCommandEvent & event) {
  int id = event.GetId();
  coneEmitter = dynamic_cast<ConeEmitter *> (ePS->emitter->getValue());
  if (!coneEmitter) {
    coneEmitter = new ConeEmitter;
    ePS->emitter->setValue(coneEmitter);
    ehdev->trackerPosition->route(coneEmitter->position);
  }
  if ( id == ID_SPEED )
    coneEmitter->speed->setValue( atof( event.GetString() ) );
  else if (id == ID_VARIATION)
    coneEmitter->variation->setValue ( atof( event.GetString() ) );
  else if (id == ID_MASS )
    coneEmitter->mass->setValue( atof( event.GetString() ) );
  else if (id == ID_SURFACE_AREA )
    coneEmitter->surfaceArea->setValue( atof( event.GetString() ) );
  else if (id == ID_DIRECTION) {
    wxString temp_string = event.GetString();
    wxStringTokenizer vec3_tkz( temp_string, wxT(" "), wxTOKEN_DEFAULT );
    vector< double > direction_values;
    while ( vec3_tkz.HasMoreTokens() ) {
        wxString value = vec3_tkz.GetNextToken();
        if ( value.ToDouble(&val) ) {
          direction_values.push_back( val );
        }
    }
    vector< double >::iterator iter = direction_values.begin();
    int size = direction_values.size();
    // Default values
    double x = 0, y = 1, z = 0;
    switch (size) {
      case 0:
        break;
      case 1:
        x = *iter;
        y = 0;
        z = 0;
        break;
      case 2:
        x = *iter;
        iter++;
        y = *iter;
        z = 0;
        break;
      case 3:
        x = *iter;
        iter++;
        y = *iter;
        iter++;
        z = *iter;
        break;
    }
    coneEmitter->direction->setValue(Vec3f(x, y, z));
  }
  else if (id == ID_POSITION) {
    wxString temp_string = event.GetString();
    wxStringTokenizer vec3_tkz( temp_string, wxT(" "), wxTOKEN_DEFAULT );
    vector< double > position_values;
    while ( vec3_tkz.HasMoreTokens() ) {
        wxString value = vec3_tkz.GetNextToken();
        if ( value.ToDouble(&val) ) {
          position_values.push_back( val );
        }
    }
    vector< double >::iterator iter = position_values.begin();
    int size = position_values.size();
    // Default values
    double x = 0, y = 0, z = 0;
    switch (size) {
      case 0:
        break;
      case 1:
        x = *iter;
        y = 0;
        z = 0;
        break;
      case 2:
        x = *iter;
        iter++;
        y = *iter;
        z = 0;
        break;
      case 3:
        x = *iter;
        iter++;
        y = *iter;
        iter++;
        z = *iter;
        break;
    }
    coneEmitter->position->setValue(Vec3f(x, y, z));
  }
}

// ---------------------------------------------------------------------------
//  ExplosionEmitterDialog Definition
// ---------------------------------------------------------------------------

IMPLEMENT_CLASS(ExplosionEmitterDialog, wxPropertySheetDialog)

BEGIN_EVENT_TABLE(ExplosionEmitterDialog, wxPropertySheetDialog)
  EVT_TEXT(ID_SPEED, ExplosionEmitterDialog::handleSettingsChange)
  EVT_TEXT(ID_VARIATION, ExplosionEmitterDialog::handleSettingsChange)
  EVT_TEXT(ID_MASS, ExplosionEmitterDialog::handleSettingsChange)
  EVT_TEXT(ID_SURFACE_AREA, ExplosionEmitterDialog::handleSettingsChange)
  EVT_TEXT(ID_POSITION, ExplosionEmitterDialog::handleSettingsChange)
END_EVENT_TABLE()


ExplosionEmitterDialog::ExplosionEmitterDialog(wxWindow* win, 
                                               ParticleSystem* PS,
                                               H3DHapticsDevice* hdev)
{
    SetExtraStyle(wxDIALOG_EX_CONTEXTHELP|wxWS_EX_VALIDATE_RECURSIVELY);

    int tabImage1 = -1;
    int tabImage2 = -1;
    int tabImage3 = -1;

    int resizeBorder = wxRESIZE_BORDER;

    m_imageList = NULL;

    Create(win, wxID_ANY, _("Explosion Emitter"), wxDefaultPosition, wxDefaultSize,
        wxDEFAULT_DIALOG_STYLE| (int)wxPlatform::IfNot(wxOS_WINDOWS_CE, 
        resizeBorder)
    );

    // If using a toolbook, also follow Mac style and don't create buttons
    CreateButtons(wxOK | wxCANCEL |
                        (int)wxPlatform::IfNot(wxOS_WINDOWS_CE, wxHELP)
    ); 

    wxBookCtrlBase* notebook = GetBookCtrl();
    notebook->SetImageList(m_imageList);

    wxPanel* ExplosionEmitterSettings = 
      CreateExplosionEmitterSettingsPage(notebook);

    notebook->AddPage(ExplosionEmitterSettings, _("Particle Properties"), true,
                      tabImage1);

    LayoutDialog();

    //Set pointEmitter pointer to NULL initally
    explosionEmitter = (ExplosionEmitter *) NULL;
    explosionEmitter = new ExplosionEmitter;
    //Make ePS point to frame ParticleSystem
    setParticleSystem (PS);
    setDevice (hdev);
}

ExplosionEmitterDialog::~ExplosionEmitterDialog()
{

}

wxPanel* ExplosionEmitterDialog::
  CreateExplosionEmitterSettingsPage(wxWindow* parent)
{
    wxPanel* panel = new wxPanel(parent, wxID_ANY);

    wxBoxSizer *topSizer = new wxBoxSizer( wxVERTICAL );
    wxBoxSizer *item0 = new wxBoxSizer( wxVERTICAL );

    //// SPEED

    wxBoxSizer* speedSizer = new wxBoxSizer( wxHORIZONTAL );
    speedSizer->Add(new wxStaticText(panel, wxID_ANY, 
                    _("&Speed            :")), 0, 
                    wxALL|wxALIGN_CENTER_VERTICAL, 5);
    wxTextCtrl* explosionEmitterSpeed = new wxTextCtrl (panel, ID_SPEED, 
                                                   wxEmptyString, 
                                                   wxDefaultPosition, 
                                                   wxSize(40, wxDefaultCoord));
    speedSizer->Add(explosionEmitterSpeed, 0, 
                    wxALL|wxALIGN_CENTER_VERTICAL, 5);
    item0->Add(speedSizer, 0, wxGROW|wxALL, 0);

    ////VARIATION

    wxBoxSizer* variationSizer = new wxBoxSizer( wxHORIZONTAL );
    variationSizer->Add(new wxStaticText(panel, wxID_ANY, 
                        _("&Variation        :")), 0, 
                        wxALL|wxALIGN_CENTER_VERTICAL, 5);
    wxTextCtrl* explosionEmitterSpeedVariation = new wxTextCtrl (panel, 
                                                   ID_VARIATION, 
                                                   wxEmptyString, 
                                                   wxDefaultPosition, 
                                                   wxSize(40, wxDefaultCoord));
    variationSizer->Add(explosionEmitterSpeedVariation, 0, 
                        wxALL|wxALIGN_CENTER_VERTICAL, 5);
    item0->Add(variationSizer, 0, wxGROW|wxALL, 0);

    //// MASS

    wxBoxSizer* massSizer = new wxBoxSizer( wxHORIZONTAL );
    massSizer->Add(new wxStaticText(panel, wxID_ANY, 
                   _("&Mass              :")), 0, 
                   wxALL|wxALIGN_CENTER_VERTICAL, 5);
    wxTextCtrl* explosionEmitterMass = new wxTextCtrl(panel, ID_MASS, 
                                                   wxEmptyString, 
                                                   wxDefaultPosition, 
                                                   wxSize(40, wxDefaultCoord));
    massSizer->Add(explosionEmitterMass, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
    item0->Add(massSizer, 0, wxGROW|wxALL, 0);

    //// SURFACE AREA

    wxBoxSizer* areaSizer = new wxBoxSizer( wxHORIZONTAL );
    areaSizer->Add(new wxStaticText(panel, wxID_ANY, _("&Surface Area :")), 0, 
                                    wxALL|wxALIGN_CENTER_VERTICAL, 5);
    wxTextCtrl* explosionEmitterArea = new wxTextCtrl(panel, ID_MASS, 
                                                 wxEmptyString, 
                                                 wxDefaultPosition, 
                                                 wxSize(40, wxDefaultCoord));
    areaSizer->Add(explosionEmitterArea, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
    item0->Add(areaSizer, 0, wxGROW|wxALL, 0);

    //// POSITION
    wxBoxSizer* positionSizer = new wxBoxSizer( wxHORIZONTAL );
    positionSizer->Add(new wxStaticText(panel, wxID_ANY, 
                                        _("&Position          :")), 0, 
                                        wxALL|wxALIGN_CENTER_VERTICAL, 5);
    wxTextCtrl* explosionEmitterPosition = new wxTextCtrl(panel, ID_POSITION, 
                                                  wxEmptyString, 
                                                  wxDefaultPosition, 
                                                  wxSize(40, wxDefaultCoord));
    positionSizer->Add(explosionEmitterPosition, 0, 
                        wxALL|wxALIGN_CENTER_VERTICAL, 5);
    item0->Add(positionSizer, 0, wxGROW|wxALL, 0);

    topSizer->Add( item0, 1, wxGROW|wxALIGN_CENTRE|wxALL, 5 );

    panel->SetSizer(topSizer);
    topSizer->Fit(panel);

    return panel;
}

void ExplosionEmitterDialog::handleSettingsChange (wxCommandEvent & event) {
  int id = event.GetId();
  explosionEmitter = 
    dynamic_cast<ExplosionEmitter *> (ePS->emitter->getValue());
  if (!explosionEmitter) {
    explosionEmitter = new ExplosionEmitter;
    ePS->emitter->setValue(explosionEmitter);
    ehdev->trackerPosition->route(explosionEmitter->position);
  }

  if ( id == ID_SPEED )
    explosionEmitter->speed->setValue( atof( event.GetString() ) );
  else if (id == ID_VARIATION)
    explosionEmitter->variation->setValue ( atof( event.GetString() ) );
  else if (id == ID_MASS )
    explosionEmitter->mass->setValue( atof( event.GetString() ) );
  else if (id == ID_SURFACE_AREA )
    explosionEmitter->surfaceArea->setValue( atof( event.GetString() ) );
  else if (id == ID_POSITION) {
    wxString temp_string = event.GetString();
    wxStringTokenizer vec3_tkz( temp_string, wxT(" "), wxTOKEN_DEFAULT );
    vector< double > position_values;
    while ( vec3_tkz.HasMoreTokens() ) {
        wxString value = vec3_tkz.GetNextToken();
        if ( value.ToDouble(&val) ) {
          position_values.push_back( val );
        }
    }
    vector< double >::iterator iter = position_values.begin();
    int size = position_values.size();
    // Default values
    double x = 0, y = 0, z = 0;
    switch (size) {
      case 0:
        break;
      case 1:
        x = *iter;
        y = 0;
        z = 0;
        break;
      case 2:
        x = *iter;
        iter++;
        y = *iter;
        z = 0;
        break;
      case 3:
        x = *iter;
        iter++;
        y = *iter;
        iter++;
        z = *iter;
        break;
    }
    explosionEmitter->position->setValue(Vec3f(x, y, z));
  }
}

// ---------------------------------------------------------------------------
//  SurfaceEmitterDialog Definition
// ---------------------------------------------------------------------------

IMPLEMENT_CLASS(SurfaceEmitterDialog, wxPropertySheetDialog)

BEGIN_EVENT_TABLE(SurfaceEmitterDialog, wxPropertySheetDialog)
  EVT_TEXT(ID_SPEED, SurfaceEmitterDialog::handleSettingsChange)
  EVT_TEXT(ID_VARIATION, SurfaceEmitterDialog::handleSettingsChange)
  EVT_TEXT(ID_MASS, SurfaceEmitterDialog::handleSettingsChange)
  EVT_TEXT(ID_SURFACE_AREA, SurfaceEmitterDialog::handleSettingsChange)
  EVT_TEXT(ID_SURFACE, SurfaceEmitterDialog::handleSettingsChange)
  EVT_TEXT(ID_COORD_INDEX, SurfaceEmitterDialog::handleSettingsChange)
END_EVENT_TABLE()


SurfaceEmitterDialog::SurfaceEmitterDialog(wxWindow* win, ParticleSystem* PS,
                                           H3DHapticsDevice* hdev)
{
    SetExtraStyle(wxDIALOG_EX_CONTEXTHELP|wxWS_EX_VALIDATE_RECURSIVELY);

    int tabImage1 = -1;
    int tabImage2 = -1;
    int tabImage3 = -1;

    int resizeBorder = wxRESIZE_BORDER;

    m_imageList = NULL;

    Create(win, wxID_ANY, _("Surface Emitter"), wxDefaultPosition, wxDefaultSize,
        wxDEFAULT_DIALOG_STYLE| (int)wxPlatform::IfNot(wxOS_WINDOWS_CE, 
        resizeBorder)
    );

    // If using a toolbook, also follow Mac style and don't create buttons
    CreateButtons(wxOK | wxCANCEL |
                        (int)wxPlatform::IfNot(wxOS_WINDOWS_CE, wxHELP)
    ); 

    wxBookCtrlBase* notebook = GetBookCtrl();
    notebook->SetImageList(m_imageList);

    wxPanel* SurfaceEmitterSettings = CreateSurfaceEmitterSettingsPage(notebook);

    notebook->AddPage(SurfaceEmitterSettings, _("Particle Properties"), true, 
        tabImage1);

    LayoutDialog();

    //Set pointEmitter pointer to NULL initally
    surfaceEmitter = (SurfaceEmitter *) NULL;
    surfaceEmitter = new SurfaceEmitter;
    //Make ePS point to frame ParticleSystem
    setParticleSystem (PS);
    setDevice (hdev);
    //ePS = PS;
}

SurfaceEmitterDialog::~SurfaceEmitterDialog()
{

}

wxPanel* SurfaceEmitterDialog::
  CreateSurfaceEmitterSettingsPage(wxWindow* parent)
{
    wxPanel* panel = new wxPanel(parent, wxID_ANY);

    wxBoxSizer *topSizer = new wxBoxSizer( wxVERTICAL );
    wxBoxSizer *item0 = new wxBoxSizer( wxVERTICAL );

    //// SPEED

    wxBoxSizer* speedSizer = new wxBoxSizer( wxHORIZONTAL );
    speedSizer->Add(new wxStaticText(panel, wxID_ANY, 
                    _("&Speed            :")), 0, 
                    wxALL|wxALIGN_CENTER_VERTICAL, 5);
    wxTextCtrl* surfaceEmitterSpeed = new wxTextCtrl (panel, ID_SPEED, 
                                               wxEmptyString, 
                                               wxDefaultPosition, 
                                               wxSize(40, wxDefaultCoord));
    speedSizer->Add(surfaceEmitterSpeed, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
    item0->Add(speedSizer, 0, wxGROW|wxALL, 0);

    ////VARIATION

    wxBoxSizer* variationSizer = new wxBoxSizer( wxHORIZONTAL );
    variationSizer->Add(new wxStaticText(panel, wxID_ANY, 
                        _("&Variation        :")), 0, 
                        wxALL|wxALIGN_CENTER_VERTICAL, 5);
    wxTextCtrl* surfaceEmitterSpeedVariation = new wxTextCtrl (panel, 
                                                   ID_VARIATION, 
                                                   wxEmptyString, 
                                                   wxDefaultPosition,
                                                   wxSize(40, wxDefaultCoord));
    variationSizer->Add(surfaceEmitterSpeedVariation, 0, 
                        wxALL|wxALIGN_CENTER_VERTICAL, 5);
    item0->Add(variationSizer, 0, wxGROW|wxALL, 0);

    //// MASS

    wxBoxSizer* massSizer = new wxBoxSizer( wxHORIZONTAL );
    massSizer->Add(new wxStaticText(panel, wxID_ANY, 
                   _("&Mass              :")), 0, 
                   wxALL|wxALIGN_CENTER_VERTICAL, 5);
    wxTextCtrl* surfaceEmitterMass = new wxTextCtrl(panel, ID_MASS, 
                                                wxEmptyString, 
                                                wxDefaultPosition, 
                                                wxSize(40, wxDefaultCoord));
    massSizer->Add(surfaceEmitterMass, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
    item0->Add(massSizer, 0, wxGROW|wxALL, 0);

    //// SURFACE AREA

    wxBoxSizer* areaSizer = new wxBoxSizer( wxHORIZONTAL );
    areaSizer->Add(new wxStaticText(panel, wxID_ANY, _("&Surface Area :")), 0, 
                                    wxALL|wxALIGN_CENTER_VERTICAL, 5);
    wxTextCtrl* surfaceEmitterArea = new wxTextCtrl(panel, ID_MASS, 
                                              wxEmptyString, 
                                              wxDefaultPosition, 
                                              wxSize(40, wxDefaultCoord));
    areaSizer->Add(surfaceEmitterArea, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
    item0->Add(areaSizer, 0, wxGROW|wxALL, 0);

    //// COORDINATE INDEX

    wxBoxSizer* coordIndexSizer = new wxBoxSizer( wxHORIZONTAL );
    coordIndexSizer->Add(new wxStaticText(panel, wxID_ANY, 
                         _("&Coordinate Index       :")), 0, 
                         wxALL|wxALIGN_CENTER_VERTICAL, 5);
    wxTextCtrl* surfaceEmitterCoordIndex = new wxTextCtrl (panel, 
                                                ID_COORD_INDEX, 
                                                wxEmptyString, 
                                                wxDefaultPosition, 
                                                wxSize(40, wxDefaultCoord));
    coordIndexSizer->Add(surfaceEmitterCoordIndex, 0, 
                         wxALL|wxALIGN_CENTER_VERTICAL, 5);
    item0->Add(coordIndexSizer, 0, wxGROW|wxALL, 0);

    topSizer->Add( item0, 1, wxGROW|wxALIGN_CENTRE|wxALL, 5 );

    panel->SetSizer(topSizer);
    topSizer->Fit(panel);

    return panel;
}

void SurfaceEmitterDialog::handleSettingsChange (wxCommandEvent & event) {
  int id = event.GetId();
  surfaceEmitter = dynamic_cast<SurfaceEmitter *> (ePS->emitter->getValue());
  if (!surfaceEmitter) {
    surfaceEmitter = new SurfaceEmitter;
    ePS->emitter->setValue(surfaceEmitter);
    //ehdev->trackerPosition->route(explosionEmitter->position);
  }

  if ( id == ID_SPEED )
    surfaceEmitter->speed->setValue( atof( event.GetString() ) );
  else if (id == ID_VARIATION)
    surfaceEmitter->variation->setValue ( atof( event.GetString() ) );
  else if (id == ID_MASS )
    surfaceEmitter->mass->setValue( atof( event.GetString() ) );
  else if (id == ID_SURFACE_AREA )
    surfaceEmitter->surfaceArea->setValue( atof( event.GetString() ) );
  //else if (id == ID_COORD_INDEX )
  //else if (id == ID_SURFACE)
}