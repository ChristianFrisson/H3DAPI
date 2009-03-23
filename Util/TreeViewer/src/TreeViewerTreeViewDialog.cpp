#include "TreeViewerTreeViewDialog.h"
#include "TreeViewerApp.h"

TreeViewerTreeViewDialog::TreeViewerTreeViewDialog( wxWindow* parent )
:
TreeViewDialog( parent )
{
  TreeViewTree->AddRoot( wxT("World") );
  // add the bindable nodes in the tree view
  bindable_tree_id = TreeViewTree->AppendItem( TreeViewTree->GetRootItem(), 
                                               wxT("Active bindable nodes") );
}


void TreeViewerTreeViewDialog::OnNodeSelected( wxTreeEvent& event ) {

  TreeViewerFieldValuesDialog *field_values = 
    static_cast< TreeViewerApp * >(wxTheApp )->field_values_dialog;
 TreeIdMap::iterator ni = node_map.find( event.GetItem().m_pItem );
  if( ni == node_map.end() ) {
    field_values->displayFieldsFromNode( NULL );
  } else {
    field_values->displayFieldsFromNode( (*ni).second );
  }
}

void TreeViewerTreeViewDialog::showEntireSceneAsTree( wxTreeItemId tree_id ) {
  //node_map.clear();

  // show the scene in the tree view.
  //addNodeToTree( tree_id, *Scene::scenes.begin() );
  list< H3D::Node * > l;
  l.push_back( *Scene::scenes.begin() );

  updateNodeTree( tree_id, l );


  l.clear();  
  const X3DBindableNode::StackMapType &stacks = X3DBindableNode::getStackMap();
  for( X3DBindableNode::StackMapType::const_iterator i = stacks.begin(); 
       i != stacks.end();i++ ) {
    l.push_back( X3DBindableNode::getActive( (*i).first ) );
  }
  updateNodeTree( bindable_tree_id, l );
}

void TreeViewerTreeViewDialog::addNodeToTree( wxTreeItemId tree_id, 
                                              H3D::Node *n,
                                              bool expand ) {
  if( !n ) return;

  // the name in the tree is NodeType(DEFed name)
  string tree_string = n->getTypeName();
  if( n->hasName() ) {
    tree_string = tree_string + " (" + n->getName() + ")";
  }

  // add an item for this node in the tree
  wxTreeItemId new_id = TreeViewTree->AppendItem( tree_id, wxString( tree_string.c_str(), wxConvUTF8 ) );
  unsigned int s1 = node_map.size();

  // add an entry for the tree_id-node pair 
  node_map[ new_id.m_pItem ] = n;


  // recursively add all the child nodes of the node to the tree
  H3DNodeDatabase *db = H3DNodeDatabase::lookupTypeId( typeid( *n ) );
  for( H3DNodeDatabase::FieldDBConstIterator i = db->fieldDBBegin();
       db->fieldDBEnd() != i; i++ ) {
    Field *f = n->getField( *i );
    
    if( SFNode *sfnode = dynamic_cast< SFNode * >( f ) ) {
      if( sfnode->getAccessType() != Field::INPUT_ONLY ) {
        addNodeToTree( new_id, sfnode->getValue(), expand );
      }
    } else if( MFNode *mfnode = dynamic_cast< MFNode * >( f ) ) {
      if( mfnode->getAccessType() != Field::INPUT_ONLY ) {
        for( MFNode::const_iterator i = mfnode->begin(); i != mfnode->end(); i++ ) {
          addNodeToTree( new_id, *i, expand );
        }
      }
    }
  }

  // make the tree be open down to leaves or not.
  if( expand ) 
    TreeViewTree->Expand( new_id );
}


void TreeViewerTreeViewDialog::updateNodeTree( wxTreeItemId tree_id, 
                                               list< H3D::Node *> nodes ) {

  // find all children of tree_id
  list< wxTreeItemId > children_ids;
  wxTreeItemIdValue cookie;
  wxTreeItemId id = TreeViewTree->GetFirstChild( tree_id, cookie );
  while( id.IsOk() ) {
    children_ids.push_back( id );
    id = TreeViewTree->GetNextChild( id, cookie );
  }
  
  // update each of the tree ids that already exists. Either by updating
  // them if they still refer to a node or deleting them otherwise.
  bool have_node_in_tree = false;
  for( list< wxTreeItemId >::iterator i = children_ids.begin();
       i != children_ids.end(); i++ ) {
    // find the node corresponding to the id in the current tree view.

    if( node_map.find( (*i).m_pItem ) == node_map.end() ) continue;

    Node *id_node = node_map[ (*i).m_pItem ]; 

    // check if this node still exists in the new node structure
    list< H3D::Node * >::iterator ni = std::find( nodes.begin(), nodes.end(), id_node );
    if( ni != nodes.end() ) {
      // the node the tree id refers to still exists on this level
      // so recurse down.

      // find all child nodes of the node
      list< H3D::Node * > child_nodes;
      H3DNodeDatabase *db = H3DNodeDatabase::lookupTypeId( typeid( *id_node ) );
      for( H3DNodeDatabase::FieldDBConstIterator j = db->fieldDBBegin();
           db->fieldDBEnd() != j; j++ ) {
        Field *f = id_node->getField( *j );

        if( SFNode *sfnode = dynamic_cast< SFNode * >( f ) ) {
          if( sfnode->getAccessType() != Field::INPUT_ONLY ) {
            Node *n = sfnode->getValue();
            if( n ) child_nodes.push_back( sfnode->getValue() );
          }
        } else if( MFNode *mfnode = dynamic_cast< MFNode * >( f ) ) {
          if( mfnode->getAccessType() != Field::INPUT_ONLY ) {
            for( MFNode::const_iterator mf = mfnode->begin(); 
                 mf != mfnode->end(); mf++ ) {
              if( *mf ) child_nodes.push_back( *mf );
            }
          }
        }        
      }
      // update
      updateNodeTree( *i, child_nodes );
      nodes.erase( ni );
    } else {
      // the node does not exist, so remove the tree id.
      TreeViewTree->Delete( *i );
      node_map.erase( (*i).m_pItem ); 
    }
  }

  // add all new nodes to the tree. 
  for( list< H3D::Node *>::iterator i = nodes.begin();
       i != nodes.end(); i++ ) {
    addNodeToTree( tree_id, *i, false );
  }

  // make the tree be open down to leaves by default.
  //TreeViewTree->Expand( tree_id );
}
