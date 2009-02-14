#include "TreeViewerTreeViewDialog.h"
#include "TreeViewerApp.h"

TreeViewerTreeViewDialog::TreeViewerTreeViewDialog( wxWindow* parent )
:
TreeViewDialog( parent )
{
  TreeViewTree->AddRoot( wxT("World") );
}


void TreeViewerTreeViewDialog::OnNodeSelected( wxTreeEvent& event ) {

  TreeViewerFieldValuesDialog *field_values = 
    static_cast< TreeViewerApp * >(wxTheApp )->field_values_dialog;
   field_values->displayFieldsFromNode( node_map[ event.GetItem().m_pItem ] );
}

void TreeViewerTreeViewDialog::showNodeAsTree( Node *n ) {
  node_map.clear();
  //addSceneToTree( TreeViewTree->GetRootItem(), n );
  addSceneToTree( TreeViewTree->GetRootItem(), *Scene::scenes.begin() );
  
  wxTreeItemId new_id = TreeViewTree->AppendItem( TreeViewTree->GetRootItem(), wxT("Active bindable nodes") );
  const X3DBindableNode::StackMapType &stacks = X3DBindableNode::getStackMap();
  for( X3DBindableNode::StackMapType::const_iterator i = stacks.begin(); i != stacks.end();i++ ) {
    addSceneToTree( new_id, X3DBindableNode::getActive( (*i).first ) );
  }
}

void TreeViewerTreeViewDialog::addSceneToTree( wxTreeItemId tree_id, H3D::Node *n ) {
  if( !n ) return;

  string tree_string = n->getTypeName();
  if( n->hasName() ) {
    tree_string = tree_string + " (" + n->getName() + ")";
  }
  wxTreeItemId new_id = TreeViewTree->AppendItem( tree_id, wxString( tree_string.c_str(), wxConvUTF8 ) );
  unsigned int s1 = node_map.size();
  node_map[ new_id.m_pItem ] = n;


    H3DNodeDatabase *db = H3DNodeDatabase::lookupTypeId( typeid( *n ) );
    for( H3DNodeDatabase::FieldDBConstIterator i = db->fieldDBBegin();
         db->fieldDBEnd() != i; i++ ) {
      Field *f = n->getField( *i );
      
      if( SFNode *sfnode = dynamic_cast< SFNode * >( f ) ) {
        if( sfnode->getAccessType() != Field::INPUT_ONLY ) {
          addSceneToTree( new_id, sfnode->getValue() );
        }
      } else if( MFNode *mfnode = dynamic_cast< MFNode * >( f ) ) {
        if( mfnode->getAccessType() != Field::INPUT_ONLY ) {
          for( MFNode::const_iterator i = mfnode->begin(); i != mfnode->end(); i++ ) {
            addSceneToTree( new_id, *i );
          }
        }
      }
    }
   TreeViewTree->Expand( tree_id );
}
