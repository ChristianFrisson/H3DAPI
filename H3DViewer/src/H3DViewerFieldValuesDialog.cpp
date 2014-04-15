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
/// \file H3DViewerFieldValuesDialog.cpp
/// \brief CPP file for H3DViewerFieldValuesDialog.
///
//
//
//////////////////////////////////////////////////////////////////////////////
#include "H3DViewerFieldValuesDialog.h"

#include <H3D/Group.h>
#include <H3D/SFString.h>
#include <H3D/PythonScript.h>

H3DViewerFieldValuesPanel::H3DViewerFieldValuesPanel( wxWindow* parent,
                  wxWindowID id, 
                  const wxPoint& pos, 
                  const wxSize& size, 
                  long style ) : 
  wxPanel( parent, id, pos, size, style ),
  displayed_node( NULL )
{
  wxBoxSizer* bSizer2;
  bSizer2 = new wxBoxSizer( wxVERTICAL );
  
  FieldValuesGrid = new wxGrid( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
  
  // Grid
  FieldValuesGrid->CreateGrid( 2, 2 );
  FieldValuesGrid->EnableEditing( true );
  FieldValuesGrid->EnableGridLines( true );
  FieldValuesGrid->EnableDragGridSize( false );
  FieldValuesGrid->SetMargins( 0, 0 );
  
  // Columns
  FieldValuesGrid->SetColSize( 0, 95 );
  FieldValuesGrid->SetColSize( 1, 102 );
  FieldValuesGrid->EnableDragColMove( false );
  FieldValuesGrid->EnableDragColSize( true );
  FieldValuesGrid->SetColLabelSize( 15 );
  FieldValuesGrid->SetColLabelValue( 0, wxT("Field") );
  FieldValuesGrid->SetColLabelValue( 1, wxT("Value") );
  FieldValuesGrid->SetColLabelAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );
  
  // Rows
  FieldValuesGrid->EnableDragRowSize( true );
  FieldValuesGrid->SetRowLabelSize( 0 );
  FieldValuesGrid->SetRowLabelAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );
  
  // Label Appearance
  
  // Cell Defaults
  FieldValuesGrid->SetDefaultCellAlignment( wxALIGN_LEFT, wxALIGN_TOP );
  bSizer2->Add( FieldValuesGrid, 1, wxALL|wxEXPAND, 5 );
  
  this->SetSizer( bSizer2 );
  this->Layout();
  
  // Connect Events
  FieldValuesGrid->Connect( wxEVT_GRID_CELL_CHANGE, wxGridEventHandler( H3DViewerFieldValuesPanel::OnCellEdit ), NULL, this );
}

H3DViewerFieldValuesPanel::~H3DViewerFieldValuesPanel() {
  FieldValuesGrid->Disconnect( wxEVT_GRID_CELL_CHANGE, wxGridEventHandler( H3DViewerFieldValuesPanel::OnCellEdit ), NULL, this );
}


void H3DViewerFieldValuesPanel::displayFieldsFromNode( Node *n ) {
  bool new_node = n != displayed_node.get();
  displayed_node.reset( n );
  if( !n ) {
#ifdef DEFAULT_VALUES
    default_values_node.reset( NULL );
#endif
    if( FieldValuesGrid->GetNumberRows() > 0 )
      FieldValuesGrid->DeleteRows( 0, FieldValuesGrid->GetNumberRows() );
    return;
  }

  // Not accessing the fields here. createNode does not need the
  // db specific to this node instance.
  H3DNodeDatabase *db = H3DNodeDatabase::lookupTypeId( typeid( *n ) );

#ifdef DEFAULT_VALUES
  if( new_node && !n->getProtoInstanceParent() ) {
    default_values_node.reset( db->createNode() );
  }
#endif

  updateGridFromNode( FieldValuesGrid,
                      n,
                      default_values_node.get(),
                      new_node );
}

void H3DViewerFieldValuesPanel::updateGridFromNode( wxGrid *FieldValuesGrid,
                                                     Node *n,
                                                     Node *default_values_node,
                                                     bool new_node ) {
  if( new_node ) {
    FieldValuesGrid->SaveEditControlValue();
  }

  H3DNodeDatabase *db = H3DNodeDatabase::lookupNodeInstance( n );

  list< FieldDBElement * > init_only_fields;
  list< FieldDBElement * > input_only_fields;
  list< FieldDBElement * > output_only_fields;
  list< FieldDBElement * > input_output_fields;

  for( H3DNodeDatabase::FieldDBConstIterator i = db->fieldDBBegin();
       db->fieldDBEnd() != i; ++i ) {
    Field *f = i.getField( n ); 
   
    if( !f ||
        dynamic_cast< SFNode * >( f ) ||
        dynamic_cast< MFNode * >( f ) ) {
      continue;    
    }

    if( f->getAccessType() == Field::INITIALIZE_ONLY ) {
      init_only_fields.push_back( i.getFieldDBElement() );
    } else if( f->getAccessType() == Field::INPUT_ONLY ) {
      input_only_fields.push_back( i.getFieldDBElement() );
    } else if( f->getAccessType() == Field::OUTPUT_ONLY ) {
      output_only_fields.push_back( i.getFieldDBElement() );
    } else if( f->getAccessType() == Field::INPUT_OUTPUT ) {
      input_output_fields.push_back( i.getFieldDBElement() );
    }
  }

  int rows = 0;

  // initialize only fields.
  if( init_only_fields.size() > 0 ) {
    if( rows >= FieldValuesGrid->GetNumberRows() )
      FieldValuesGrid->AppendRows(1);
    FieldValuesGrid->SetCellSize( rows, 0, 1, 2 );
    FieldValuesGrid->SetCellAlignment(rows, 0, wxALIGN_CENTRE, wxALIGN_CENTRE);
    FieldValuesGrid->SetCellValue(rows, 0, _T("Initialize only"));
    FieldValuesGrid->SetReadOnly( rows, 0 );
    FieldValuesGrid->SetCellBackgroundColour( rows, 0, wxColour(159, 222,222) );
    FieldValuesGrid->SetCellTextColour( rows, 0, wxColour(100, 100,100) );
    ++rows;
  }

  for( list< FieldDBElement * >::iterator i = init_only_fields.begin();
       i != init_only_fields.end(); ++i ) {
    Field *f = (*i)->getField( n ); 
    Field *default_field = (*i)->getField( default_values_node );
    updateRowFromField( FieldValuesGrid, rows, f, default_field, new_node );
    ++rows;
  }

  // input/output  fields.
  if( input_output_fields.size() > 0 ) {
    if( rows >= FieldValuesGrid->GetNumberRows() )
      FieldValuesGrid->AppendRows(1);
    FieldValuesGrid->SetCellSize( rows, 0, 1, 2 );
    FieldValuesGrid->SetCellAlignment( rows, 0, wxALIGN_CENTRE, wxALIGN_CENTRE);
    FieldValuesGrid->SetCellValue( rows, 0, _T("Input/Output"));
    FieldValuesGrid->SetReadOnly( rows, 0 );
    FieldValuesGrid->SetCellBackgroundColour( rows, 0, wxColour(159, 222,222) );
    FieldValuesGrid->SetCellTextColour( rows, 0, wxColour(100, 100,100) );
    ++rows;
  }

  for( list< FieldDBElement * >::iterator i = input_output_fields.begin();
       i != input_output_fields.end(); ++i ) {
    Field *f = (*i)->getField( n ); 
    Field *default_field = (*i)->getField( default_values_node );
    updateRowFromField( FieldValuesGrid, rows, f, default_field, new_node );
    ++rows;
  }
  
  // output only fields.
  if( output_only_fields.size() > 0 ) {
    if( rows >= FieldValuesGrid->GetNumberRows() )
      FieldValuesGrid->AppendRows(1);
    FieldValuesGrid->SetCellSize( rows, 0, 1, 2 );
    FieldValuesGrid->SetCellAlignment( rows, 0, wxALIGN_CENTRE, wxALIGN_CENTRE);
    FieldValuesGrid->SetCellValue( rows, 0, _T("Output only"));
    FieldValuesGrid->SetReadOnly( rows, 0 );
    FieldValuesGrid->SetCellBackgroundColour( rows, 0, wxColour(159, 222,222) );
    FieldValuesGrid->SetCellTextColour( rows, 0, wxColour(100, 100,100) );
    ++rows;
  }

  for( list< FieldDBElement * >::iterator i = output_only_fields.begin();
       i != output_only_fields.end(); ++i ) {
    Field *f = (*i)->getField( n ); 
    Field *default_field = (*i)->getField( default_values_node );
    updateRowFromField( FieldValuesGrid, rows, f, default_field, new_node );
    ++rows;
  }

  // input only fields.
  if( input_only_fields.size() > 0 ) {
    if( rows >= FieldValuesGrid->GetNumberRows() )
      FieldValuesGrid->AppendRows(1);
    FieldValuesGrid->SetCellSize( rows, 0, 1, 2 );
    FieldValuesGrid->SetCellAlignment( rows, 0, wxALIGN_CENTRE, wxALIGN_CENTRE);
    FieldValuesGrid->SetCellValue( rows, 0, _T("Input only"));
    FieldValuesGrid->SetReadOnly( rows, 0 );
    FieldValuesGrid->SetCellBackgroundColour( rows, 0, wxColour(159, 222,222) );
    FieldValuesGrid->SetCellTextColour( rows, 0, wxColour(100, 100,100) );
    ++rows;
  }

  for( list< FieldDBElement * >::iterator i = input_only_fields.begin();
       i != input_only_fields.end(); ++i ) {
    Field *f = (*i)->getField( n ); 
    Field *default_field = (*i)->getField( default_values_node );
    updateRowFromField( FieldValuesGrid, rows, f, default_field, new_node );
    ++rows;
  }

#ifdef HAVE_PYTHONSCRIPT
  // if PythonScript node add top-level fields.
  PythonScript *ps = dynamic_cast< PythonScript * >( n );
  if( ps ) {
    vector< pair< string, Field *> > python_fields;
    ps->getTopLevelFields( python_fields );
  
    // python fields.
    if( python_fields.size() > 0 ) {
      if( rows >= FieldValuesGrid->GetNumberRows() )
        FieldValuesGrid->AppendRows(1);
      FieldValuesGrid->SetCellSize( rows, 0, 1, 2 );
      FieldValuesGrid->SetCellAlignment( rows, 0, wxALIGN_CENTRE, wxALIGN_CENTRE);
      FieldValuesGrid->SetCellValue( rows, 0, _T("Python fields"));
      FieldValuesGrid->SetReadOnly( rows, 0 );
      FieldValuesGrid->SetCellBackgroundColour( rows, 0, wxColour(159, 222,222) );
      FieldValuesGrid->SetCellTextColour( rows, 0, wxColour(100, 100,100) );
      ++rows;
    }

    for( vector< pair< string, Field *> >::iterator i = python_fields.begin();
         i != python_fields.end(); ++i ) {
      Field *f = (*i).second;
      const string &name = (*i).first;
      if( dynamic_cast< SFNode * >( f ) || 
          dynamic_cast< MFNode * >( f ) ) continue;
        
      updateRowFromField( FieldValuesGrid, rows, f, f, new_node, name );
      ++rows;
    }
  }
#endif
  if( rows < FieldValuesGrid->GetNumberRows() ) {
    FieldValuesGrid->DeleteRows( rows, FieldValuesGrid->GetNumberRows() - rows );
  }

  

}

void H3DViewerFieldValuesPanel::OnIdle( wxIdleEvent& event ) {
  try {
    if( IsShown() ) {
      TimeStamp now;
      if( now - last_fields_update > 0.1 ) {
  displayFieldsFromNode( displayed_node.get() );
  last_fields_update = now;
      }
    }
  } catch( ... ) {
    // ignore any errors
  }
}

void H3DViewerFieldValuesPanel::OnCellEdit( wxGridEvent& event ) {
  if( displayed_node.get() ) {
    int col = event.GetCol();
    int row = event.GetRow();
    string s( FieldValuesGrid->GetCellValue( row, col ).mb_str() );
    if( col == 1 ) {
      string field_name( FieldValuesGrid->GetCellValue( row, 0 ).mb_str());
      Field *f = displayed_node->getField( field_name );
      if( SFBool *sfbool = dynamic_cast< SFBool * >( f ) ) {
         sfbool->setValue( s == "1" );
      } else if( ParsableField *pf = dynamic_cast< ParsableField * >( f ) ) {
        try {
          pf->setValueFromString( s );
        } catch(...) {

        }
      }
    }
  }
}


void H3DViewerFieldValuesPanel::updateRowFromField( wxGrid *FieldValuesGrid,
                                                     int row, 
                                                     Field *f,
                                                     Field *default_field,
                                                     bool new_node,
                                                     const string &custom_field_name ) {
  if(!default_field) default_field = f;
  
  if( SFNode *sfnode = dynamic_cast< SFNode * >( f ) ) {
    
  } else if( MFNode *mfnode = dynamic_cast< MFNode * >( f ) ) {
    
  } else {
    wxGridCellEditor *current_editor = FieldValuesGrid->GetCellEditor( row, 1 );
    wxGridCellRenderer *current_renderer = FieldValuesGrid->GetCellRenderer( row, 1 );
    wxGridCellEditor *editor = current_editor;
    wxGridCellRenderer *renderer = current_renderer;
    string value;
    string default_value;
    string field_name = custom_field_name;
    if( field_name.empty() ) field_name = f->getName();

    bool allow_cell_update = true;

    FieldValuesGrid->SetCellSize( row, 0, 1, 1 );
    FieldValuesGrid->SetCellAlignment(row, 0, wxALIGN_LEFT, wxALIGN_TOP);
    FieldValuesGrid->SetReadOnly( row, 0 );
    FieldValuesGrid->SetCellBackgroundColour( row, 0, *wxWHITE );

    FieldValuesGrid->SetCellSize( row, 1, 1, 1 );
    FieldValuesGrid->SetCellAlignment(row, 1, wxALIGN_LEFT, wxALIGN_TOP);
    FieldValuesGrid->SetCellBackgroundColour( row, 1, *wxWHITE );
    if( f->getAccessType() == Field::INITIALIZE_ONLY ||
        f->getAccessType() == Field::OUTPUT_ONLY ) {
      FieldValuesGrid->SetReadOnly( row, 1, true );
    } else {
      FieldValuesGrid->SetReadOnly( row, 1, false );
    }


    if( SFString *sfstring = dynamic_cast< SFString * >( f ) ) {
      // set renderer to string renderer
      // GetCellRenderer increases reference count.
      // Decrease reference count to clean up memory properly before
      // replacing pointer.
      renderer->DecRef();
      renderer = FieldValuesGrid->GetDefaultRenderer();
      // use a choice editor if a set of valid values have been
      // specified for the field.
      if( sfstring->hasValidValues() ) {    
        // set editor
        if( !dynamic_cast< wxGridCellChoiceEditor * >( current_editor ) ||
            new_node ) {
          const set<string > &valid_values = sfstring->getValidValues();
          wxArrayString choices;
          choices.Alloc( valid_values.size() );
          for( set<string>::const_iterator i = valid_values.begin();
               i != valid_values.end(); ++i ) {
            choices.Add( wxString( (*i).c_str(), wxConvUTF8 ));
          }
          // GetCellEditor increases reference count.
          // Decrease reference count to clean up memory properly before
          // replacing pointer.
          editor->DecRef();
          editor = new wxGridCellChoiceEditor( choices, false );
        }
      } else {
        // no valid values specified, use string editor.
        // GetCellEditor increases reference count.
        // Decrease reference count to clean up memory properly before
        // replacing pointer.
        editor->DecRef();
        editor = FieldValuesGrid->GetDefaultEditor();
      }
      
      // set value
      value = "";
      if( sfstring->getAccessType() != Field::INPUT_ONLY ) {
        value = sfstring->getValue();
        default_value = static_cast< SFString * >( default_field )->getValue();
      }
    } else if( SFBool *sfbool = dynamic_cast< SFBool * >( f ) ) {
      // set renderer
      if( !dynamic_cast< wxGridCellBoolRenderer * >( current_renderer ) ) {
        // GetCellRenderer increases reference count.
        // Decrease reference count to clean up memory properly before
        // replacing pointer.
        renderer->DecRef();
        renderer = new wxGridCellBoolRenderer;
      }
      
      // set editor
      if( !dynamic_cast< wxGridCellBoolEditor * >( current_editor ) ) {
        // GetCellEditor increases reference count.
        // Decrease reference count to clean up memory properly before
        // replacing pointer.
        editor->DecRef();
        editor = new wxGridCellBoolEditor;
      }
      
      if( FieldValuesGrid->GetGridCursorRow() ==  row ) {
        if( f->getAccessType() != Field::INPUT_ONLY )
          FieldValuesGrid->SaveEditControlValue();
        allow_cell_update = FieldValuesGrid->GetGridCursorCol() !=  1;
      }
      
      // set value
      value = "0";
      if( sfbool->getAccessType() != Field::INPUT_ONLY ) {
        bool checked = sfbool->getValue();
        value = checked ? "1" : "0";
      }
      
      // set default value
      default_value = "0";
      if( sfbool->getAccessType() != Field::INPUT_ONLY  ) {
        SFBool *sf = static_cast< SFBool *>( default_field );
        bool checked = sf->getValue();
        default_value = checked ? "1" : "0";
      }
    }  else if( ParsableField *pfield = dynamic_cast< ParsableField * >( f ) ) {
      // set renderer and editor
      // GetCellEditor and GetCellRenderer increases reference count.
      // Decrease reference count to clean up memory properly before
      // replacing pointer.
      editor->DecRef();
      renderer->DecRef();
      editor = FieldValuesGrid->GetDefaultEditor();
      renderer = FieldValuesGrid->GetDefaultRenderer();
      
      // set value
      if( pfield->getAccessType() != Field::INPUT_ONLY ) {
        value = pfield->getValueAsString();
      }
      
      // set default value
      default_value = "0";
      if( pfield->getAccessType() != Field::INPUT_ONLY  ) {
        ParsableField *df = static_cast< ParsableField * >( default_field );
        default_value = df->getValueAsString();
      }
    }
    
    // if not enough row, append a new one
    if( row >= FieldValuesGrid->GetNumberRows() )
      FieldValuesGrid->AppendRows(1);
    
    // set the editor and renderer of the cell
    FieldValuesGrid->SetCellRenderer( row, 1, renderer );
    FieldValuesGrid->SetCellEditor( row, 1, editor );
    
    bool changed_color = false;
#ifdef DEFAULT_VALUES
    wxColour current_color = FieldValuesGrid->GetCellTextColour( row, 0 );
    // set the color of the text.
    if( value == default_value ) {
      FieldValuesGrid->SetCellTextColour( row, 0, *wxBLACK );
      FieldValuesGrid->SetCellTextColour( row, 1, *wxBLACK );
    } else {
      FieldValuesGrid->SetCellTextColour( row, 0, *wxRED );
      FieldValuesGrid->SetCellTextColour( row, 1, *wxRED );
    }
   
    changed_color = FieldValuesGrid->GetCellTextColour( row, 0 ) != current_color;
#endif
    
    //  set the field name value if it has changed.
    if( string( FieldValuesGrid->GetCellValue( row, 0 ).mb_str() ) != field_name ||
        changed_color )
      FieldValuesGrid->SetCellValue( row, 0, wxString( field_name.c_str() , wxConvUTF8) );
    
    // set field value if changed and not currently being edited.
    if( allow_cell_update &&
        string( FieldValuesGrid->GetCellValue( row, 1 ).mb_str() ) != value &&
        !FieldValuesGrid->IsCellEditControlEnabled() ){
      FieldValuesGrid->SetCellValue( row, 1, wxString( value.c_str(), wxConvUTF8) );
    }
  }
}




H3DViewerFieldValuesDialog::H3DViewerFieldValuesDialog( wxWindow* parent, 
              wxWindowID id, 
              const wxString& title, 
              const wxPoint& pos, 
              const wxSize& size, 
              long style ) : 
  wxFrame( parent, id, title, pos, size, style ) {

  this->SetSizeHints( wxDefaultSize, wxDefaultSize );
  
  wxBoxSizer* bSizer2;
  bSizer2 = new wxBoxSizer( wxVERTICAL );
  
  field_values_panel = new H3DViewerFieldValuesPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize );

  bSizer2->Add( field_values_panel, 1, wxALL|wxEXPAND, 5 );
  this->SetSizer( bSizer2 );
  this->Layout();
  
  // Connect Events
  this->Connect( wxEVT_IDLE, wxIdleEventHandler( H3DViewerFieldValuesPanel::OnIdle ), NULL, field_values_panel );
}

H3DViewerFieldValuesDialog::~H3DViewerFieldValuesDialog()
{
  // Disconnect Events
  this->Disconnect( wxEVT_IDLE, wxIdleEventHandler( H3DViewerFieldValuesPanel::OnIdle ), NULL, field_values_panel );
}

void H3DViewerFieldValuesDialog::displayFieldsFromNode( Node *n ) {
  if( !n ) {
    SetTitle(wxT(""));
  } else {
    SetTitle( wxString(n->getTypeName().c_str(),wxConvUTF8) );
  }
  field_values_panel->displayFieldsFromNode( n );
}
