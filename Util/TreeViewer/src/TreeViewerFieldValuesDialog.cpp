#include "TreeViewerFieldValuesDialog.h"

#include <H3D/Group.h>

TreeViewerFieldValuesDialog::TreeViewerFieldValuesDialog( wxWindow* parent )
:
  FieldValuesDialog( parent ),
  displayed_node( NULL )
{

}


void TreeViewerFieldValuesDialog::displayFieldsFromNode( Node *n ) {
  displayed_node = n;
  if( !n ) {
    SetTitle(wxT(""));
    if( FieldValuesGrid->GetNumberRows() > 0 )
      FieldValuesGrid->DeleteRows( 0, FieldValuesGrid->GetNumberRows() );
    return;
  }

  SetTitle( wxString(n->getTypeName().c_str(),wxConvUTF8) );
  H3DNodeDatabase *db = H3DNodeDatabase::lookupTypeId( typeid( *n ) );
  unsigned int rows = 0;
  for( H3DNodeDatabase::FieldDBConstIterator i = db->fieldDBBegin();
       db->fieldDBEnd() != i; i++ ) {
    Field *f = n->getField( *i );
      
    if( SFNode *sfnode = dynamic_cast< SFNode * >( f ) ) {
        
    } else if( MFNode *mfnode = dynamic_cast< MFNode * >( f ) ) {
       
    } else if( ParsableField *pfield = dynamic_cast< ParsableField * >( f ) ) {
      if( pfield->getAccessType() != Field::INPUT_ONLY ) {
        if( rows >= FieldValuesGrid->GetNumberRows() )
          FieldValuesGrid->AppendRows(1);
        if( string( FieldValuesGrid->GetCellValue( rows, 0 ).mb_str() ) != *i )
          FieldValuesGrid->SetCellValue( rows, 0, wxString( (*i).c_str() , wxConvUTF8) );

        string value = pfield->getValueAsString();
        if( string( FieldValuesGrid->GetCellValue( rows, 1 ).mb_str() ) != value )
          FieldValuesGrid->SetCellValue( rows, 1, wxString( value.c_str(), wxConvUTF8) );
        rows++;
      }
    }
  
  }

  if( rows < FieldValuesGrid->GetNumberRows() ) {
    FieldValuesGrid->DeleteRows( rows, FieldValuesGrid->GetNumberRows() - rows );
  }

}


void TreeViewerFieldValuesDialog::OnIdle( wxIdleEvent& event ) {
  displayFieldsFromNode( displayed_node );
}

void TreeViewerFieldValuesDialog::OnCellEdit( wxGridEvent& event ) {
  if( displayed_node ) {
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
