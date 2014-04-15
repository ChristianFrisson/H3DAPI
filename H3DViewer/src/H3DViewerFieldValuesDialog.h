//////////////////////////////////////////////////////////////////////////////
//    Copyright 2009-2014, SenseGraphics AB
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
/// \file H3DViewerFieldValuesDialog.h
/// \brief Header file for H3DViewerFieldValuesDialog.
///
//
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __H3DViewerFieldValuesDialog__
#define __H3DViewerFieldValuesDialog__

#define DEFAULT_VALUES
#include "H3DViewer.h"
#include <wx/grid.h>

#include <H3D/Node.h>



using namespace H3D;

/** Implementing FieldValuesDialog */
class H3DViewerFieldValuesPanel : public wxPanel
{
public:
  /** Constructor */
  H3DViewerFieldValuesPanel( wxWindow* parent, 
           wxWindowID id = wxID_ANY, 
           const wxPoint& pos = wxDefaultPosition, 
           const wxSize& size = wxSize( 236,335 ), 
           long style = wxTAB_TRAVERSAL );
  ~H3DViewerFieldValuesPanel();

  void OnIdle( wxIdleEvent& event );
  virtual void OnCellEdit( wxGridEvent& event );

  void displayFieldsFromNode( Node * );

  // Get the node which values are currently being shown.
  inline Node *getDisplayedNode() {
    return displayed_node.get();
  }

  static void updateGridFromNode( wxGrid *FieldValuesGrid,
                                  Node *n,
                                  Node *default_values_node,
                                  bool new_node );
protected:
  
  // Update the specified row in the field values grid with
  // values from the field.
  // row is the index of the row
  // f is the Field which value should be displayed.
  // default_field is a Field that contains the defualt value of the field.
  // new_node is true if this value was not displayed last loop.
  // field_name, if non-empty, specifies the string to show as the
  // name of the field.
  static void updateRowFromField( wxGrid *FieldValuesGrid,
                                  int row, 
                                  Field *f,
                                  Field *default_field,
                                  bool new_node,
                                  const string &field_name = "" );

  TimeStamp last_fields_update;
#ifdef DEFAULT_VALUES
  auto_ptr< Node > default_values_node;
#endif
  AutoRef< Node > displayed_node;
  wxGrid *FieldValuesGrid;
};


class H3DViewerFieldValuesDialog : public wxFrame {
 public:
  H3DViewerFieldValuesPanel* field_values_panel;
  
  H3DViewerFieldValuesDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 236,335 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
  
  ~H3DViewerFieldValuesDialog();

  void displayFieldsFromNode( H3D::Node * );
};

#endif // __H3DViewerFieldValuesDialog__
