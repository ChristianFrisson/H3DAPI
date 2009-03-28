#include "TreeViewerTreeViewDialog.h"
#include "TreeViewerFieldValuesDialog.h"
#include <fstream>
#include <wx/wx.h>
#include <H3D/Scene.h>
#include <H3D/Viewpoint.h>
#include <H3D/X3D.h>

TreeViewerTreeViewDialog::TreeViewerTreeViewDialog( wxWindow* parent )
:
  TreeViewDialog( parent ),
  shown_last_loop( false ),
  displayed_node( NULL )
{
  TreeViewTree->AddRoot( wxT("World") );
  // add the bindable nodes in the tree view
  bindable_tree_id = TreeViewTree->AppendItem( TreeViewTree->GetRootItem(), 
                                               wxT("Active bindable nodes") );
  TreeViewTree->Expand( TreeViewTree->GetRootItem() ); 
  SetMenuBar( NULL );
}


void TreeViewerTreeViewDialog::OnNodeSelected( wxTreeEvent& event ) {

 TreeIdMap::iterator ni = node_map.find( event.GetItem().m_pItem );
  if( ni == node_map.end() ) {
    displayFieldsFromNode( NULL );
  } else {
    displayFieldsFromNode( (*ni).second.get() );
  }
}

void TreeViewerTreeViewDialog::showEntireSceneAsTree( bool expand_new ) {
  // show the scene in the tree view.
  list< H3D::Node * > l;
  l.push_back( *Scene::scenes.begin() );

  updateNodeTree( TreeViewTree->GetRootItem(), l, expand_new );

  l.clear();  
  const X3DBindableNode::StackMapType &stacks = X3DBindableNode::getStackMap();
  for( X3DBindableNode::StackMapType::const_iterator i = stacks.begin(); 
       i != stacks.end();i++ ) {
    X3DBindableNode *b = X3DBindableNode::getActive( (*i).first );
    if( b ) l.push_back( b );
  }
  updateNodeTree( bindable_tree_id, l, false );
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
  node_map[ new_id.m_pItem ].reset( n );


  // recursively add all the child nodes of the node to the tree
  H3DNodeDatabase *db = H3DNodeDatabase::lookupTypeId( typeid( *n ) );
  for( H3DNodeDatabase::FieldDBConstIterator i = db->fieldDBBegin();
       db->fieldDBEnd() != i; i++ ) {
    Field *f = i.getField( n ); //n->getField( *i );
    
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
                                               list< H3D::Node *> nodes,
                                               bool expand_new ) {

  // find all children of tree_id
  list< wxTreeItemId > children_ids;
  wxTreeItemIdValue cookie;
  wxTreeItemId id = TreeViewTree->GetFirstChild( tree_id, cookie );
  while( id.IsOk() ) {
    children_ids.push_back( id );
    id = TreeViewTree->GetNextSibling( id );
  }
  
  // update each of the tree ids that already exists. Either by updating
  // them if they still refer to a node or deleting them otherwise.
  bool have_node_in_tree = false;
  for( list< wxTreeItemId >::iterator i = children_ids.begin();
       i != children_ids.end(); i++ ) {
    // find the node corresponding to the id in the current tree view.

    if( node_map.find( (*i).m_pItem ) == node_map.end() ) {
      continue;
    }

    Node *id_node = node_map[ (*i).m_pItem ].get(); 

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
         Field *f = j.getField( id_node ); //Field *f = id_node->getField( *j );

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
      updateNodeTree( *i, child_nodes, expand_new );
      nodes.erase( ni );
    } else {
      // the node does not exist, so remove the tree id and all its children.
      deleteTree( *i );
    }
  }

  // add all new nodes to the tree. 
  for( list< H3D::Node *>::iterator i = nodes.begin();
       i != nodes.end(); i++ ) {
    addNodeToTree( tree_id, *i, expand_new );
  }

  // make the tree be open down to leaves by default.
  //TreeViewTree->Expand( tree_id );
}


void TreeViewerTreeViewDialog::deleteTree( const wxTreeItemId &id ) { 
  list< wxTreeItemId > children_ids;
  wxTreeItemIdValue cookie;
  wxTreeItemId child_id = TreeViewTree->GetFirstChild( id, cookie );
  while( child_id.IsOk() ) {
    children_ids.push_back( child_id );
    child_id = TreeViewTree->GetNextChild( child_id, cookie );
  }

  for( list< wxTreeItemId >::iterator i = children_ids.begin();
       i != children_ids.end(); i++ ) {
    deleteTree( *i );
  }

  node_map.erase( id.m_pItem ); 
  TreeViewTree->Delete( id );
}

void TreeViewerTreeViewDialog::displayFieldsFromNode( Node *n ) {
  bool new_node = n != displayed_node.get();
  displayed_node.reset( n );
  if( !n ) {
#ifdef DEFAULT_VALUES
    default_values_node.reset( NULL );
#endif
    //SetTitle(wxT(""));
    if( FieldValuesGrid->GetNumberRows() > 0 )
      FieldValuesGrid->DeleteRows( 0, FieldValuesGrid->GetNumberRows() );
    return;
  }

  //SetTitle( wxString(n->getTypeName().c_str(),wxConvUTF8) );
  H3DNodeDatabase *db = H3DNodeDatabase::lookupTypeId( typeid( *n ) );
#ifdef DEFAULT_VALUES
  if( new_node ) {
    default_values_node.reset( db->createNode() );
  }
#endif
  int rows = 0;
  for( H3DNodeDatabase::FieldDBConstIterator i = db->fieldDBBegin();
       db->fieldDBEnd() != i; i++ ) {
    Field *f = i.getField( n ); //n->getField( *i );
      
    if( SFNode *sfnode = dynamic_cast< SFNode * >( f ) ) {
        
    } else if( MFNode *mfnode = dynamic_cast< MFNode * >( f ) ) {

    } else {
      wxGridCellEditor *current_editor = FieldValuesGrid->GetCellEditor( rows, 1 );
      wxGridCellRenderer *current_renderer = FieldValuesGrid->GetCellRenderer( rows, 1 );
      wxGridCellEditor *editor = current_editor;
      wxGridCellRenderer *renderer = current_renderer;
      string value;
      string default_value;
      Field *default_field = i.getField( default_values_node.get() );
      bool allow_cell_update = true;

      if( SFBool *sfbool = dynamic_cast< SFBool * >( f ) ) {
        // set renderer
        if( !dynamic_cast< wxGridCellBoolRenderer * >( current_renderer ) ) {
          renderer = new wxGridCellBoolRenderer;
        }
        
        // set editor
        if( !dynamic_cast< wxGridCellBoolEditor * >( current_editor ) ) {
          editor = new wxGridCellBoolEditor;
        }

        if( FieldValuesGrid->GetGridCursorRow() ==  rows ) {
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

      // if not enough rows, append a new one
      if( rows >= FieldValuesGrid->GetNumberRows() )
        FieldValuesGrid->AppendRows(1);
        
      // set the editor and renderer of the cell
      FieldValuesGrid->SetCellRenderer( rows, 1, renderer );
      FieldValuesGrid->SetCellEditor( rows, 1, editor );
      
      bool changed_color = false;
#ifdef DEFAULT_VALUES
      wxColour current_color = FieldValuesGrid->GetCellTextColour( rows, 0 );
      // set the color of the text.
      if( value == default_value ) {
        FieldValuesGrid->SetCellTextColour( rows, 0, *wxBLACK );
        FieldValuesGrid->SetCellTextColour( rows, 1, *wxBLACK );
      } else {
        FieldValuesGrid->SetCellTextColour( rows, 0, *wxRED );
        FieldValuesGrid->SetCellTextColour( rows, 1, *wxRED );
      }
      changed_color = FieldValuesGrid->GetCellTextColour( rows, 0 ) != current_color;
#endif

      //  set the field name value if it has changed.
      if( string( FieldValuesGrid->GetCellValue( rows, 0 ).mb_str() ) != *i ||
          changed_color )
        FieldValuesGrid->SetCellValue( rows, 0, wxString( (*i).c_str() , wxConvUTF8) );
        
      // set field value if changed and not currently being edited.
      if( allow_cell_update &&
          string( FieldValuesGrid->GetCellValue( rows, 1 ).mb_str() ) != value &&
          !FieldValuesGrid->IsCellEditControlEnabled() ){
         FieldValuesGrid->SetCellValue( rows, 1, wxString( value.c_str(), wxConvUTF8) );
      }
        
      rows++;
    }
  }
  
  if( rows < FieldValuesGrid->GetNumberRows() ) {
    FieldValuesGrid->DeleteRows( rows, FieldValuesGrid->GetNumberRows() - rows );
  }

}

void TreeViewerTreeViewDialog::clearTreeView() {
  list< Node * > l;
  updateNodeTree( TreeViewTree->GetRootItem(), l );
  updateNodeTree( bindable_tree_id, l, false );
  displayFieldsFromNode( NULL );

}

void TreeViewerTreeViewDialog::OnIdle( wxIdleEvent& event ) {
  try {
  if( IsShown() ) {
    TimeStamp now;
    if( now - last_fields_update > 0.1 ) {
      displayFieldsFromNode( displayed_node.get() );
      last_fields_update = now;
    }

    if( now - last_tree_update > 1 ) {
      showEntireSceneAsTree( !shown_last_loop );
      last_tree_update = now;
    }
  } else if( shown_last_loop ) {
    // make sure we do not hold any references to any nodes by clearing
    // it.
    clearTreeView();
  }

  shown_last_loop = IsShown();
  } catch( ... ) {
    // ignore any errors
  }
}

void TreeViewerTreeViewDialog::OnCellEdit( wxGridEvent& event ) {
  if( displayed_node.get() ) {
    int col = event.GetCol();
    int row = event.GetRow();
    string s( FieldValuesGrid->GetCellValue( row, col ).mb_str() );
    if( col == 1 ) {
      string field_name( FieldValuesGrid->GetCellValue( row, 0 ).mb_str());
      Field *f = displayed_node->getField( field_name );
      if( SFBool *sfbool = dynamic_cast< SFBool * >( f ) ) {
         sfbool->setValue( s == "1" );
         Console(4) << (s == "1") << endl;
      } else if( ParsableField *pf = dynamic_cast< ParsableField * >( f ) ) {
        try {
          pf->setValueFromString( s );
        } catch(...) {

        }
      }
    }
  }
}

void TreeViewerTreeViewDialog::expandTree( const wxTreeItemId &id ) {
  if( id.IsOk() ) {
    wxTreeItemIdValue cookie;
    wxTreeItemId child_id = TreeViewTree->GetFirstChild( id, cookie );
    while( child_id.IsOk() ) {
      expandTree( child_id );
      child_id = TreeViewTree->GetNextSibling( child_id );
    }
    TreeViewTree->Expand( id );
  }
}

void TreeViewerTreeViewDialog::collapseTree( const wxTreeItemId &id ) {
  if( id.IsOk() ) {
    wxTreeItemIdValue cookie;
    wxTreeItemId child_id = TreeViewTree->GetFirstChild( id, cookie );
    while( child_id.IsOk() ) {
      collapseTree( child_id );
      child_id = TreeViewTree->GetNextSibling( child_id );
    }
    TreeViewTree->Collapse( id );
  }
}

void TreeViewerTreeViewDialog::OnTreeRightClick( wxTreeEvent& event ) {
  TreeViewTree->SelectItem( event.GetItem() );
  PopupMenu( RightClickMenu );
}

/// Callback for collapse all menu choice.
void TreeViewerTreeViewDialog::OnTreeViewCollapseAll( wxCommandEvent& event ) {
  wxTreeItemId id = TreeViewTree->GetSelection();
  if( id.IsOk() ) {
    collapseTree( id );
  }
}

/// Callback for expand all menu choice.
void TreeViewerTreeViewDialog::OnTreeViewExpandAll( wxCommandEvent& event ) {
  wxTreeItemId id = TreeViewTree->GetSelection();
  if( id.IsOk() ) {
    expandTree( id );
  }
}

/// Callback for collapse children menu choice.
void TreeViewerTreeViewDialog::OnTreeViewCollapseChildren( wxCommandEvent& event ) {
  wxTreeItemId id = TreeViewTree->GetSelection();
  wxTreeItemIdValue cookie;
  wxTreeItemId child_id = TreeViewTree->GetFirstChild( id, cookie );
  while( child_id.IsOk() ) {
    collapseTree( child_id );
    child_id = TreeViewTree->GetNextSibling( child_id );
  }
}

/// Callback for node watch menu choice.
void TreeViewerTreeViewDialog::OnTreeViewNodeWatch( wxCommandEvent& event ) {
  wxTreeItemId id = TreeViewTree->GetSelection();
  
  TreeIdMap::iterator ni = node_map.find( id.m_pItem );
  if( ni == node_map.end() ) {
    wxMessageBox( wxT("Selected tree item is not a node"),
                  wxT("Error"),
                  wxOK | wxICON_EXCLAMATION);
  } else {
    TreeViewerFieldValuesDialog *fv = new TreeViewerFieldValuesDialog( this );
    fv->displayFieldsFromNode( (*ni).second.get() );
    fv->Show();
  }
}

/// Callback for node save x3d menu choice.
void TreeViewerTreeViewDialog::OnTreeViewSaveX3D( wxCommandEvent& event ) {
  wxTreeItemId id = TreeViewTree->GetSelection();
  
  TreeIdMap::iterator ni = node_map.find( id.m_pItem );
  if( ni == node_map.end() ) {
    wxMessageBox( wxT("Selected tree item is not a node"),
                  wxT("Error"),
                  wxOK | wxICON_EXCLAMATION);
  } else {
    wxFileDialog *file_dialog = new wxFileDialog ( this,
                                                   wxT("File to save as.."),
                                                   wxT(""),
                                                   wxT(""),
                                                   wxT("*.*"),
                                                   wxSAVE,
                                                   wxDefaultPosition) ;

    if (file_dialog->ShowModal() == wxID_OK) {
      std::string filename(file_dialog->GetPath().mb_str());
      std::ofstream os( filename.c_str() );
      if( os.fail() ) {
        wxMessageBox( wxT("Unable to open selected file"), 
                      wxT("Error"),
                      wxOK | wxICON_EXCLAMATION);
      }
      
      try {
        X3D::writeNodeAsX3D( os,
                             (*ni).second.get() );
      } catch (const Exception::H3DException &e) {
        stringstream s;
        s << e;
        wxMessageBox( wxString(s.str().c_str(),wxConvUTF8), wxT("Error"),
                      wxOK | wxICON_EXCLAMATION);
      }
      os.close();
    }
  }
}

void TreeViewerTreeViewDialog::OnClose( wxCloseEvent& event ) {
  Hide();
}
