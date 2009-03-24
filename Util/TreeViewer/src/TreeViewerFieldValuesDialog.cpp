#include "TreeViewerFieldValuesDialog.h"

#include <H3D/Group.h>

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

  SetTitle( wxString(n->getTypeName().c_str(),wxConvUTF8) );
  H3DNodeDatabase *db = H3DNodeDatabase::lookupTypeId( typeid( *n ) );
#ifdef DEFAULT_VALUES
  if( new_node ) {
    default_values_node.reset( db->createNode() );
  }
#endif
  unsigned int rows = 0;
  for( H3DNodeDatabase::FieldDBConstIterator i = db->fieldDBBegin();
       db->fieldDBEnd() != i; i++ ) {
    Field *f = i.getField( n ); //n->getField( *i );
      
    if( SFNode *sfnode = dynamic_cast< SFNode * >( f ) ) {
        
    } else if( MFNode *mfnode = dynamic_cast< MFNode * >( f ) ) {

    } /*else if( SFBool *sfbool = dynamic_cast< SFBool * >( f ) ) {
      if( rows >= FieldValuesGrid->GetNumberRows() ) {
         FieldValuesGrid->AppendRows(1);
         FieldValuesGrid->SetCellRenderer( rows, 1, new wxGridCellBoolRenderer );
         FieldValuesGrid->SetCellEditor( rows, 1, new wxGridCellBoolEditor );
      }

      if( string( FieldValuesGrid->GetCellValue( rows, 0 ).mb_str() ) != *i )
        FieldValuesGrid->SetCellValue( rows, 0, wxString( (*i).c_str() , wxConvUTF8) );
      rows++;
      } */ else if( ParsableField *pfield = dynamic_cast< ParsableField * >( f ) ) {
      if( pfield->getAccessType() != Field::INPUT_ONLY ) {
        if( rows >= FieldValuesGrid->GetNumberRows() )
          FieldValuesGrid->AppendRows(1);
        if( string( FieldValuesGrid->GetCellValue( rows, 0 ).mb_str() ) != *i )
          FieldValuesGrid->SetCellValue( rows, 0, wxString( (*i).c_str() , wxConvUTF8) );

        string value = pfield->getValueAsString();
        if( string( FieldValuesGrid->GetCellValue( rows, 1 ).mb_str() ) != value )
          FieldValuesGrid->SetCellValue( rows, 1, wxString( value.c_str(), wxConvUTF8) );

#ifdef DEFAULT_VALUES
         ParsableField *df = 
           static_cast< ParsableField * >( i.getField( default_values_node.get() ) );
         if( df->getValueAsString() == pfield->getValueAsString() ) {
           FieldValuesGrid->SetCellTextColour( rows, 0, *wxBLACK );
           FieldValuesGrid->SetCellTextColour( rows, 1, *wxBLACK );
         } else {
           FieldValuesGrid->SetCellTextColour( rows, 0, *wxRED );
           FieldValuesGrid->SetCellTextColour( rows, 1, *wxRED );
         }
#endif

        rows++;


      }
    }
  
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
      if( ParsableField *pf = dynamic_cast< ParsableField * >( f ) ) {
        try {
          pf->setValueFromString( s );
        } catch(...) {

        }
      }
    }
  }
}
