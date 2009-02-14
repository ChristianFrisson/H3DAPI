#include "TreeViewerFieldValuesDialog.h"

#include <H3D/Group.h>

TreeViewerFieldValuesDialog::TreeViewerFieldValuesDialog( wxWindow* parent )
:
FieldValuesDialog( parent )
{

}


void TreeViewerFieldValuesDialog::displayFieldsFromNode( Node *n ) {
 
  displayed_node = n;
  if( !n ) {
    SetTitle("");
    FieldValuesGrid->DeleteRows( 0, FieldValuesGrid->GetNumberRows() );
    return;
  }

  SetTitle( n->getTypeName() );
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
        if( FieldValuesGrid->GetCellValue( rows, 0 ) != *i )
          FieldValuesGrid->SetCellValue( rows, 0, *i );

        string value = pfield->getValueAsString();
        if( FieldValuesGrid->GetCellValue( rows, 1 ) != value )
          FieldValuesGrid->SetCellValue( rows, 1, value );
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
    string s = FieldValuesGrid->GetCellValue( row, col );
    if( col == 1 ) {
      string field_name = FieldValuesGrid->GetCellValue( row, 0 );
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
