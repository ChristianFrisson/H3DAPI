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
/// \file H3DViewerTreeViewDialog.cpp
/// \brief CPP file for H3DViewerTreeViewDialog.
///
//
//
//////////////////////////////////////////////////////////////////////////////
#include "H3DViewerTreeViewDialog.h"
#include "H3DViewerPopupMenus.h"
#include <fstream>
#include <wx/wx.h>
#include <H3D/Scene.h>
#include <H3D/Viewpoint.h>
#include <H3D/IndexedTriangleSet.h>
#include <H3D/Coordinate.h>
#include <H3D/MatrixTransform.h>
#include <H3D/X3DShapeNode.h>
#include <H3D/IndexedFaceSet.h>
#include <H3D/X3DGeometryNode.h>
#include <H3D/X3D.h>

H3DViewerTreeViewDialog::H3DViewerTreeViewDialog( wxWindow* parent )
:
  TreeViewDialog( parent ),
  shown_last_loop( false )
{
  TreeViewTree->AddRoot( wxT("World") );
  // add the bindable nodes in the tree view
  bindable_tree_id = TreeViewTree->AppendItem( TreeViewTree->GetRootItem(), 
                                               wxT("Active bindable nodes") );
  TreeViewTree->Expand( TreeViewTree->GetRootItem() );

#ifdef USE_PROPGRID
  field_values_panel = new H3DViewerFieldValuesPanelPropGrid( SplitterWindow, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL  );
#else
  field_values_panel = new H3DViewerFieldValuesPanel( SplitterWindow, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL  );
#endif
  
  menu_container = new H3DViewerPopupMenus( this, this);
  displayFieldsFromNode( NULL );
  if( SplitterWindow->IsSplit() )
    SplitterWindow->Unsplit();
  SplitterWindow->SplitVertically( TreeViewPanel, field_values_panel, 283 );
  this->Layout();
}

H3DViewerTreeViewDialog::~H3DViewerTreeViewDialog() {
  // If the menu bar is set to NULL in the constructor it will not be
  // cleaned up. Done here explicitly.
  //if( GetMenuBar() == NULL )
  //  delete m_menubar1;
}

void H3DViewerTreeViewDialog::OnNodeSelected( wxTreeEvent& event ) {

 TreeIdMap::iterator ni = node_map.find( event.GetItem().m_pItem );
  if( ni == node_map.end() ) {
    selected_node.reset( NULL );
  } else {
    selected_node.reset( (*ni).second.get() );
  }
}

int H3DViewerTreeViewDialog::getNrTriangles( X3DGeometryNode *geom ) {
  IndexedFaceSet *ifs = dynamic_cast< IndexedFaceSet * >( geom );
  if( ifs ) {
    // IndexedFaceSet nrTriangles function is an upper bound so
    // try to calculate a more exact number here.
    const vector< int > &index = ifs->coordIndex->getValue();
    
    unsigned int i = 0;
    unsigned int nr_triangles = 0;
    
    while( i < index.size() ) {
      unsigned int nr_face_vertices = 0;
      while( i < index.size() && index[i++] != -1 ) {
        ++nr_face_vertices;
      }
      
      if( nr_face_vertices >= 3 ) {
        nr_triangles += nr_face_vertices - 2;
      }
    }
    return nr_triangles;
  }
  return geom->nrTriangles();
}

void H3DViewerTreeViewDialog::showEntireSceneAsTree( H3DViewerTreeViewDialog::ExpandMode expand_new ) {
  // show the scene in the tree view.
  list< pair< H3D::Node *, string > > l;
  Scene *s =  *Scene::scenes.begin();
  l.push_back(make_pair(s, s->defaultXMLContainerField() ) );

  updateNodeTree( TreeViewTree->GetRootItem(), l, expand_new );

  l.clear();  
  const X3DBindableNode::StackMapType &stacks = X3DBindableNode::getStackMap();
  for( X3DBindableNode::StackMapType::const_iterator i = stacks.begin(); 
       i != stacks.end();++i ) {
    X3DBindableNode *b = X3DBindableNode::getActive( (*i).first );
    if( b ) l.push_back( make_pair(b, b->defaultXMLContainerField() ) );
  }
  updateNodeTree( bindable_tree_id, l, H3DViewerTreeViewDialog::EXPAND_NONE );
}

void H3DViewerTreeViewDialog::addNodeToTree( wxTreeItemId tree_id, 
                                             H3D::Node *n,
                                             string container_field,
                                             H3DViewerTreeViewDialog::ExpandMode expand ) {

  if( !n ) return;

  if( n->getProtoInstanceParent() ) {
    n = n->getProtoInstanceParent();
  }

  // the name in the tree is NodeType(DEFed name)
  string tree_string = n->getTypeName();
  if( n->hasName() ) {
    tree_string = tree_string + " (" + n->getName() + ")";
  }

  X3DGeometryNode *geom = dynamic_cast< X3DGeometryNode * >( n );
  if( geom ) {

    int nr_triangles = getNrTriangles( geom );
    if( nr_triangles == -1 ) {
      tree_string = tree_string + " (Approx nr triangles: Unknown )";
    } else {
      char nr_triangles_str[255];
      sprintf(nr_triangles_str, " (Approx nr triangles: %d )", getNrTriangles(geom) );
      tree_string = tree_string + nr_triangles_str;
    }
  }

  if( container_field != n->defaultXMLContainerField() ) {
    tree_string = tree_string + " (cf: " + container_field + ")";
  }
  // add an item for this node in the tree
  wxTreeItemId new_id = TreeViewTree->AppendItem( tree_id, wxString( tree_string.c_str(), wxConvUTF8 ) );
  unsigned int s1 = node_map.size();

  // add an entry for the tree_id-node pair 
  node_map[ new_id.m_pItem ].reset( n );

  bool expand_new_id = ( expand == H3DViewerTreeViewDialog::EXPAND_ALL || 
                        (expand == H3DViewerTreeViewDialog::EXPAND_GROUP && (dynamic_cast< X3DGroupingNode * >( n ) || dynamic_cast< Scene * >( n ) ) ) );
  // recursively add all the child nodes of the node to the tree
  H3DNodeDatabase *db = H3DNodeDatabase::lookupNodeInstance( n );
  for( H3DNodeDatabase::FieldDBConstIterator i = db->fieldDBBegin();
       db->fieldDBEnd() != i; ++i ) {
    Field *f = i.getField( n ); //n->getField( *i );
    
    if( SFNode *sfnode = dynamic_cast< SFNode * >( f ) ) {
      if( sfnode->getAccessType() != Field::INPUT_ONLY ) {
        addNodeToTree( new_id, sfnode->getValue(), sfnode->getName(), expand_new_id ? expand : H3DViewerTreeViewDialog::EXPAND_NONE );
      }
    } else if( MFNode *mfnode = dynamic_cast< MFNode * >( f ) ) {
      if( mfnode->getAccessType() != Field::INPUT_ONLY ) {
        for( MFNode::const_iterator i = mfnode->begin(); i != mfnode->end(); ++i ) {
          addNodeToTree( new_id, *i, mfnode->getName(), expand_new_id ? expand : H3DViewerTreeViewDialog::EXPAND_NONE );
        }
      }
    }
  }

  // make the tree be open down to leaves or not.
  if( expand_new_id ) {
    TreeViewTree->Expand( new_id );
  } 
}


void H3DViewerTreeViewDialog::updateNodeTree( wxTreeItemId tree_id, 
                                              list< pair< H3D::Node *, string > > nodes,
                                              H3DViewerTreeViewDialog::ExpandMode expand_new,
                                              bool check_if_expanded ) {

  // find all children of tree_id
  list< wxTreeItemId > children_ids;
  wxTreeItemIdValue cookie;

  wxTreeItemId id = TreeViewTree->GetFirstChild( tree_id, cookie );
  if( check_if_expanded )
    if( id.IsOk() && TreeViewTree->HasChildren(tree_id ) && !TreeViewTree->IsExpanded( tree_id ) ) { 
      return;
    }
  while( id.IsOk() ) {
    children_ids.push_back( id );
    id = TreeViewTree->GetNextSibling( id );
  }
  
  // update each of the tree ids that already exists. Either by updating
  // them if they still refer to a node or deleting them otherwise.
  bool have_node_in_tree = false;
  for( list< wxTreeItemId >::iterator i = children_ids.begin();
       i != children_ids.end(); ++i ) {
    // find the node corresponding to the id in the current tree view.

    if( node_map.find( (*i).m_pItem ) == node_map.end() ) {
      continue;
    }

    Node *id_node = node_map[ (*i).m_pItem ].get(); 

    // check if this node still exists in the new node structure
    list< pair< H3D::Node *, string > >::iterator ni;
    for( ni = nodes.begin(); ni != nodes.end(); ++ni ) {
      Node *node = (*ni).first;
      if( node->getProtoInstanceParent() ) {
        node = node->getProtoInstanceParent(); 
      }
      if( node == id_node ) break;
    }

    if( ni != nodes.end() ) {
      // the node the tree id refers to still exists on this level
      // so recurse down.

      // find all child nodes of the node
      list< pair< H3D::Node *, string > > child_nodes;
      H3DNodeDatabase *db = H3DNodeDatabase::lookupNodeInstance( id_node );
      for( H3DNodeDatabase::FieldDBConstIterator j = db->fieldDBBegin();
           db->fieldDBEnd() != j; ++j ) {
         Field *f = j.getField( id_node ); //Field *f = id_node->getField( *j );

        if( SFNode *sfnode = dynamic_cast< SFNode * >( f ) ) {
          if( sfnode->getAccessType() != Field::INPUT_ONLY ) {
            Node *n = sfnode->getValue();
            if( n ) child_nodes.push_back( make_pair( sfnode->getValue(), sfnode->getName() ) );
          }
        } else if( MFNode *mfnode = dynamic_cast< MFNode * >( f ) ) {
          if( mfnode->getAccessType() != Field::INPUT_ONLY ) {
            for( MFNode::const_iterator mf = mfnode->begin(); 
                 mf != mfnode->end(); ++mf ) {
              if( *mf ) child_nodes.push_back( make_pair( *mf, mfnode->getName() ) );
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
  for( list< pair< H3D::Node *, string > >::iterator i = nodes.begin();
       i != nodes.end(); ++i ) {
    addNodeToTree( tree_id, (*i).first, (*i).second, expand_new );
  }

  // make the tree be open down to leaves by default.
  //TreeViewTree->Expand( tree_id );
}


void H3DViewerTreeViewDialog::deleteTree( const wxTreeItemId &id ) { 
  list< wxTreeItemId > children_ids;
  wxTreeItemIdValue cookie;
  wxTreeItemId child_id = TreeViewTree->GetFirstChild( id, cookie );
  while( child_id.IsOk() ) {
    children_ids.push_back( child_id );
    child_id = TreeViewTree->GetNextSibling( child_id );
  }

  for( list< wxTreeItemId >::iterator i = children_ids.begin();
       i != children_ids.end(); ++i ) {
    deleteTree( *i );
  }

  node_map.erase( id.m_pItem ); 
  TreeViewTree->Delete( id );
}

void H3DViewerTreeViewDialog::displayFieldsFromNode( Node *n ) {
  field_values_panel->displayFieldsFromNode( n );
}
 
void H3DViewerTreeViewDialog::clearTreeView() {
  list< pair< Node *, string> > l;
  updateNodeTree( TreeViewTree->GetRootItem(), l );
  updateNodeTree( bindable_tree_id, l, H3DViewerTreeViewDialog::EXPAND_NONE, false );
  displayFieldsFromNode( NULL );

}

void H3DViewerTreeViewDialog::OnIdle( wxIdleEvent& event ) {
  try {
  if( IsShown() ) {
    
    if( selected_node.get() != field_values_panel->getDisplayedNode() ) {
      displayFieldsFromNode( selected_node.get() );
    }

    field_values_panel->OnIdle( event );

    TimeStamp now;
    if( now - last_tree_update > 1 ) {
      if( shown_last_loop ) {
        showEntireSceneAsTree( H3DViewerTreeViewDialog::EXPAND_NONE );
      } else {
        showEntireSceneAsTree( H3DViewerTreeViewDialog::EXPAND_GROUP );
      }
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

void H3DViewerTreeViewDialog::expandTree( const wxTreeItemId &id ) {
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

void H3DViewerTreeViewDialog::collapseTree( const wxTreeItemId &id ) {
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


void H3DViewerTreeViewDialog::OnTreeRightClick( wxTreeEvent& event ) {
  TreeViewTree->SelectItem( event.GetItem() );
  TreeIdMap::iterator ni = node_map.find( event.GetItem().m_pItem );
  X3DGeometryNode *geom = NULL;
  X3DTextureNode * tex = NULL;
  
  if( ni != node_map.end() ) {
   geom = dynamic_cast< X3DGeometryNode * >( (*ni).second.get() );
   tex = dynamic_cast< X3DTextureNode * >( (*ni).second.get() );
  }

  if(geom) PopupMenu( menu_container->RightClickMenuGeometry );
  else if(tex) PopupMenu( menu_container->RightClickMenuTexture );
  else PopupMenu( menu_container->RightClickMenu );
}


void H3DViewerTreeViewDialog::OnClose( wxCloseEvent& event ) {
  Hide();
}

void H3DViewerTreeViewDialog::btnCloseClick(wxCommandEvent& event) {
  Hide();
}

void H3DViewerTreeViewDialog::collectAllTriangles( Node *n, 
                                                   const Matrix4f &transform,
                                                   vector< Vec3f > &triangles ) {

  if( !n ) return;

  
  if( X3DShapeNode *shape = dynamic_cast< X3DShapeNode * >( n ) ) {
    X3DGeometryNode *geom = shape->geometry->getValue();
    if( geom ) {
      vector< HAPI::Collision::Triangle > tris;
      geom->boundTree->getValue()->getAllTriangles( tris );
      for( unsigned int i = 0; i < tris.size(); ++i ) {
        triangles.push_back( transform * (Vec3f) tris[i].a );
        triangles.push_back( transform * (Vec3f) tris[i].b );
        triangles.push_back( transform * (Vec3f) tris[i].c );
      }
    }
  } 

  MatrixTransform *t = dynamic_cast< MatrixTransform * >( n );
  Matrix4f new_transform = t ? t->matrix->getValue() * transform :transform;

  H3DNodeDatabase *db = H3DNodeDatabase::lookupNodeInstance( n );
  for( H3DNodeDatabase::FieldDBConstIterator i = db->fieldDBBegin();
       db->fieldDBEnd() != i; ++i ) {
    Field *f = i.getField( n ); 
    if( SFNode *sfnode = dynamic_cast< SFNode * >( f ) ) {
      collectAllTriangles( sfnode->getValue(), new_transform, triangles );
    } else if( MFNode *mfnode = dynamic_cast< MFNode * >( f ) ) {
      for( unsigned int j = 0; j < mfnode->size(); ++j ) {
        Node *n = mfnode->getValueByIndex( j ); 
        collectAllTriangles( n, new_transform, triangles );
      }
    }
  }
}

