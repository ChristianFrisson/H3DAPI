//////////////////////////////////////////////////////////////////////////////
//    Copyright 2010-2014, SenseGraphics AB
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
/// \file H3DViewerFieldValuesDialogPropGrid.h
/// \brief Header file for H3DViewerFieldValuesDialogPropGrid.
///
//
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __H3DViewerFieldValuesDialogPropGrid__
#define __H3DViewerFieldValuesDialogPropGrid__

#define USE_VECTOR_PROPERTIES

#include <H3DViewerConfig.h>

#ifdef HAVE_WXPROPGRID

#define DEFAULT_VALUES
#include "H3DViewer.h"

#include <H3D/Scene.h>
#include <H3D/Node.h>
#include <H3DUtil/AutoPtrVector.h>

#include "wx/wx.h"
#include "wx/propgrid/propgrid.h"
#include "wx/propgrid/advprops.h"

///////////////////////////////////////////////////////////////////////////////
/// Class FieldValuesDialog
///////////////////////////////////////////////////////////////////////////////

class H3DLongStringProperty : public wxLongStringProperty {
public:
  H3DLongStringProperty( bool read_only,
                         const wxString& label = wxPG_LABEL,
                         const wxString& name = wxPG_LABEL,
                         const wxString& value = wxEmptyString );
  virtual bool OnButtonClick( wxPropertyGrid* propgrid, wxString& value );

  void setReadOnlyDialog( bool _read_only ) {
    read_only = _read_only;
    ChangeFlag( wxPG_PROP_NOEDITOR, read_only );
  }

 protected:
  bool read_only;
};

class H3DViewerFieldValuesPanelPropGrid : public wxPanel {
public:
  wxPropertyGrid* FieldValuesGrid;
  H3DViewerFieldValuesPanelPropGrid( wxWindow* parent, 
                                     wxWindowID id = wxID_ANY, 
                                     const wxPoint& pos = wxDefaultPosition, 
                                     const wxSize& size = wxSize( 236,335 ), 
                                     long style = wxTAB_TRAVERSAL );
  ~H3DViewerFieldValuesPanelPropGrid();
        

  void OnIdle( wxIdleEvent& event );
  virtual void OnPropertyChanged( wxPropertyGridEvent& event );
  
  void displayFieldsFromNode( H3D::Node * );
  void displayFieldsFromProgramSettings();

  // Get the node which values are currently being shown.
  inline H3D::Node *getDisplayedNode() {
    return displayed_node.get();
  }

  static void populateGridFromNode( wxPropertyGrid *FieldValuesGrid,
                                    H3D::Node *n,
                                    H3D::Node *default_values_node,
                                    H3DUtil::AutoPtrVector< H3D::Field > &property_update_fields );
  static void populateGridFromProgramSettings( wxPropertyGrid *FieldValuesGrid,
                                               H3DUtil::AutoPtrVector< H3D::Field > &property_update_fields );
  
 protected:

  class PropertyUpdater: public H3D::Field {
  public:
  PropertyUpdater( wxPGProperty *p, 
                   void *_default_value = NULL, 
                   unsigned int _default_value_size = 0): 
    property(p),
      default_value( (unsigned char * )_default_value ),
    default_value_size( _default_value_size )   {}
    
  protected:    
    virtual void update();

    bool isDefaultValue( Field *f );

    wxPGProperty *property;
    std::auto_ptr< unsigned char > default_value;

    // the size of default_value pointer in bytes.
    unsigned int default_value_size;

  };
  
  static wxPGProperty *getPropertyFromField( H3D::Field *f,
                                             const std::string &field_name = "" );
  
  static void setupProperty( wxPropertyGrid *grid,
                             H3D::Field *f,
                             H3D::Field *default_field,
                             H3DUtil::AutoPtrVector< H3D::Field > &property_update_fields,
                             const std::string &property_name = "" );

  static void settingsCallback( H3D::Scene::ProgramSettingsEvent event,
                                const H3D::Scene::ProgramSetting &setting,
                                void *data );
  H3DUtil::AutoPtrVector< H3D::Field  > property_update_fields;
  
  H3D::TimeStamp last_fields_update;
  H3DUtil::AutoRef< H3D::Node > displayed_node;
  unsigned int mode;  
};


class H3DViewerFieldValuesDialogPropGrid : public wxFrame {
public:
  wxPropertyGrid* FieldValuesGrid;
  H3DViewerFieldValuesDialogPropGrid( wxWindow* parent, 
                                      wxWindowID id = wxID_ANY, 
                                      const wxString& title = wxEmptyString, 
                                      const wxPoint& pos = wxDefaultPosition, 
                                      const wxSize& size = wxSize( 236,335 ), 
                                      long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL,
                                      bool destroy_on_close = true);
  ~H3DViewerFieldValuesDialogPropGrid();

  void OnClose( wxCloseEvent& event );
  void displayFieldsFromNode( H3D::Node * );
  void displayFieldsFromProgramSettings();

  H3DViewerFieldValuesPanelPropGrid *prop_grid_panel;
  bool destroy_on_close;
};




// -----------------------------------------------------------------------


class H3DwxVector
{
public:
 H3DwxVector( unsigned int nr_values = 0 ):
  values( nr_values, 0.0 ){

  }

  inline unsigned int nrValues() const {
    return values.size();
  }

  inline double getValue( unsigned int i ) const {
    return values[i];
  }

  inline void setValue( unsigned int i, double value ) {
    values[i] = value;
  }

  std::vector< double > values;
};

inline bool operator == (const H3DwxVector& a, const H3DwxVector& b)
{
  return a.values == b.values;
}

WX_PG_DECLARE_VARIANT_DATA(H3DwxVector)

class wxVectorProperty : public wxPGProperty
{
    WX_PG_DECLARE_PROPERTY_CLASS(wxVectorProperty)
public:

    wxVectorProperty( const wxString& label = wxPG_LABEL,
                      const wxString& name = wxPG_LABEL,
                      const wxArrayString &element_labels = wxArrayString(),
                      const H3DwxVector& value = H3DwxVector(0),
                      bool use_spin_ctrl = true );

    virtual ~wxVectorProperty();

    virtual wxVariant ChildChanged( wxVariant& thisValue,
                                    int childIndex,
                                    wxVariant& childValue ) const;
    virtual void RefreshChildren();

protected:
};


#endif // HAVE_WXPROPGRID
#endif // __H3DViewerFieldValuesDialog__
