#include "TreeViewerFieldValuesDialog.h"

#include <H3D/Group.h>
#include <H3D/SFString.h>

TreeViewerFieldValuesDialog::TreeViewerFieldValuesDialog( wxWindow* parent )
:
  FieldValuesDialog( parent ),
  displayed_node( NULL )
{

}


void TreeViewerFieldValuesDialog::displayFieldsFromNode( Node *n ) {
    bool new_node = n != displayed_node.get();
  displayed_node.reset( n );
  if( !n ) {
#ifdef DEFAULT_VALUES
    default_values_node.reset( NULL );
#endif
    SetTitle(wxT(""));
    if( FieldValuesGrid->GetNumberRows() > 0 )
      FieldValuesGrid->DeleteRows( 0, FieldValuesGrid->GetNumberRows() );
    return;
  }

  if( new_node ) {
    FieldValuesGrid->SaveEditControlValue();
  }

  SetTitle( wxString(n->getTypeName().c_str(),wxConvUTF8) );
  H3DNodeDatabase *db = H3DNodeDatabase::lookupTypeId( typeid( *n ) );
#ifdef DEFAULT_VALUES
  if( new_node ) {
    default_values_node.reset( db->createNode() );
  }
#endif

  list< FieldDBElement * > init_only_fields;
  list< FieldDBElement * > input_only_fields;
  list< FieldDBElement * > output_only_fields;
  list< FieldDBElement * > input_output_fields;

  for( H3DNodeDatabase::FieldDBConstIterator i = db->fieldDBBegin();
       db->fieldDBEnd() != i; i++ ) {
    Field *f = i.getField( n ); 
    if( dynamic_cast< SFNode * >( f ) ||
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
    rows++;
  }

  for( list< FieldDBElement * >::iterator i = init_only_fields.begin();
       i != init_only_fields.end(); i++ ) {
    Field *f = (*i)->getField( n ); 
    updateRowFromFieldDB( rows, n, *i, new_node );
    rows++;
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
    rows++;
  }

  for( list< FieldDBElement * >::iterator i = input_output_fields.begin();
       i != input_output_fields.end(); i++ ) {
    Field *f = (*i)->getField( n ); 
    updateRowFromFieldDB( rows, n, *i, new_node );
    rows++;
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
    rows++;
  }

  for( list< FieldDBElement * >::iterator i = output_only_fields.begin();
       i != output_only_fields.end(); i++ ) {
    Field *f = (*i)->getField( n ); 
    updateRowFromFieldDB( rows, n, *i, new_node );
    rows++;
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
    rows++;
  }

  for( list< FieldDBElement * >::iterator i = input_only_fields.begin();
       i != input_only_fields.end(); i++ ) {
    Field *f = (*i)->getField( n ); 
    updateRowFromFieldDB( rows, n, *i, new_node );
    rows++;
  }

  if( rows < FieldValuesGrid->GetNumberRows() ) {
    FieldValuesGrid->DeleteRows( rows, FieldValuesGrid->GetNumberRows() - rows );
  }

}

void TreeViewerFieldValuesDialog::OnIdle( wxIdleEvent& event ) {
  TimeStamp now;
  if( now - last_fields_update > 0.1 ) {
    displayFieldsFromNode( displayed_node.get() );
    last_fields_update = now;
  }
}

void TreeViewerFieldValuesDialog::OnCellEdit( wxGridEvent& event ) {
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

void TreeViewerFieldValuesDialog::updateRowFromFieldDB( int row, 
                                                        Node *n,
                                                        FieldDBElement *db,
                                                        bool new_node ) {
  Field *f = db->getField( n );
  if( SFNode *sfnode = dynamic_cast< SFNode * >( f ) ) {
    
  } else if( MFNode *mfnode = dynamic_cast< MFNode * >( f ) ) {
    
  } else {
    wxGridCellEditor *current_editor = FieldValuesGrid->GetCellEditor( row, 1 );
    wxGridCellRenderer *current_renderer = FieldValuesGrid->GetCellRenderer( row, 1 );
    wxGridCellEditor *editor = current_editor;
    wxGridCellRenderer *renderer = current_renderer;
    string value;
    string default_value;
    Field *default_field = db->getField( default_values_node.get() );
    bool allow_cell_update = true;

    FieldValuesGrid->SetCellSize( row, 0, 1, 1 );
    FieldValuesGrid->SetCellAlignment(row, 0, wxALIGN_LEFT, wxALIGN_TOP);
    FieldValuesGrid->SetReadOnly( row, 0 );
    FieldValuesGrid->SetCellBackgroundColour( row, 0, *wxWHITE );
    FieldValuesGrid->SetCellTextColour( row, 0, *wxBLACK );

    FieldValuesGrid->SetCellSize( row, 1, 1, 1 );
    FieldValuesGrid->SetCellAlignment(row, 1, wxALIGN_LEFT, wxALIGN_TOP);
    FieldValuesGrid->SetCellBackgroundColour( row, 1, *wxWHITE );
    FieldValuesGrid->SetCellTextColour( row, 1, *wxBLACK );
    if( f->getAccessType() == Field::INITIALIZE_ONLY ||
        f->getAccessType() == Field::OUTPUT_ONLY ) {
      FieldValuesGrid->SetReadOnly( row, 1, true );
    } else {
      FieldValuesGrid->SetReadOnly( row, 1, false );
    }


    if( SFString *sfstring = dynamic_cast< SFString * >( f ) ) {
      // set renderer to string renderer
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
               i != valid_values.end(); i++ ) {
            choices.Add( wxString( (*i).c_str() ));
          }
          editor = new wxGridCellChoiceEditor( choices, false );
        }
      } else {
        // no valid values specified, use string editor.
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
        renderer = new wxGridCellBoolRenderer;
      }
      
      // set editor
      if( !dynamic_cast< wxGridCellBoolEditor * >( current_editor ) ) {
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
    if( string( FieldValuesGrid->GetCellValue( row, 0 ).mb_str() ) != f->getName() ||
        changed_color )
      FieldValuesGrid->SetCellValue( row, 0, wxString( f->getName().c_str() , wxConvUTF8) );
    
    // set field value if changed and not currently being edited.
    if( allow_cell_update &&
        string( FieldValuesGrid->GetCellValue( row, 1 ).mb_str() ) != value &&
        !FieldValuesGrid->IsCellEditControlEnabled() ){
      FieldValuesGrid->SetCellValue( row, 1, wxString( value.c_str(), wxConvUTF8) );
    }
  }
}


