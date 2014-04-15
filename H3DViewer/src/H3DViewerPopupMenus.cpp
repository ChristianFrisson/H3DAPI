//////////////////////////////////////////////////////////////////////////////
//    Copyright 2011-2014, SenseGraphics AB
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
/// \file H3DViewerPopupMenus.cpp
/// \brief CPP file for H3DViewerPopupMenus.
///
//
//
//////////////////////////////////////////////////////////////////////////////
#include "H3DViewerPopupMenus.h"

#include <fstream>
#include <wx/wx.h>
#include <H3D/Scene.h>
#include <H3D/Viewpoint.h>
#include <H3D/IndexedTriangleSet.h>
#include <H3D/Coordinate.h>
#include <H3D/MatrixTransform.h>
#include <H3D/X3DTexture2DNode.h>
#include <H3D/X3DTexture3DNode.h>
#include <H3D/X3DShapeNode.h>
#include <H3D/X3DGeometryNode.h>
#include <H3D/X3D.h>

#include <H3DUtil/LoadImageFunctions.h>

/// Callback for collapse all menu choice.
void H3DViewerPopupMenus::OnTreeViewCollapseAll( wxCommandEvent& event ) {
  wxTreeItemId id = treeview_dialog->TreeViewTree->GetSelection();
  if( id.IsOk() ) {
    treeview_dialog->collapseTree( id );
  }
}

/// Callback for expand all menu choice.
void H3DViewerPopupMenus::OnTreeViewExpandAll( wxCommandEvent& event ) {
  wxTreeItemId id = treeview_dialog->TreeViewTree->GetSelection();
  if( id.IsOk() ) {
    treeview_dialog->expandTree( id );
  }
}

/// Callback for collapse children menu choice.
void H3DViewerPopupMenus::OnTreeViewCollapseChildren( wxCommandEvent& event ) {
  wxTreeItemId id = treeview_dialog->TreeViewTree->GetSelection();
  wxTreeItemIdValue cookie;
  wxTreeItemId child_id = treeview_dialog->TreeViewTree->GetFirstChild( id, cookie );
  while( child_id.IsOk() ) {
    treeview_dialog->collapseTree( child_id );
    child_id = treeview_dialog->TreeViewTree->GetNextSibling( child_id );
  }
}

/// Callback for node watch menu choice.
void H3DViewerPopupMenus::OnTreeViewNodeWatch( wxCommandEvent& event ) {
  wxTreeItemId id = treeview_dialog->TreeViewTree->GetSelection();
  
  H3DViewerTreeViewDialog::TreeIdMap::iterator ni = treeview_dialog->node_map.find( id.m_pItem );
  if( ni == treeview_dialog->node_map.end() ) {
    wxMessageBox( wxT("Selected tree item is not a node"),
                  wxT("Error"),
                  wxOK | wxICON_EXCLAMATION);
  } else {
#ifdef USE_PROPGRID
    H3DViewerFieldValuesDialogPropGrid *fv = new H3DViewerFieldValuesDialogPropGrid( this );
#else
    H3DViewerFieldValuesDialog *fv = new H3DViewerFieldValuesDialog( this );
#endif
    fv->displayFieldsFromNode( (*ni).second.get() );
    fv->Show();
  }
}

/// Callback for node save x3d menu choice.
void H3DViewerPopupMenus::OnTreeViewSaveX3D( wxCommandEvent& event ) {
  wxTreeItemId id = treeview_dialog->TreeViewTree->GetSelection();
  
  H3DViewerTreeViewDialog::TreeIdMap::iterator ni = treeview_dialog->node_map.find( id.m_pItem );
  if( ni == treeview_dialog->node_map.end() ) {
    wxMessageBox( wxT("Selected tree item is not a node"),
                  wxT("Error"),
                  wxOK | wxICON_EXCLAMATION);
  } else {
    auto_ptr< wxFileDialog > file_dialog ( new wxFileDialog ( this,
                                                              wxT("File to save as.."),
                                                              wxT(""),
                                                              wxT(""),
                                                              wxT("*.*"),
                                                              wxFD_SAVE,
                                                              wxDefaultPosition) );
    
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

/// Callback for node save VRML menu choice.
void H3DViewerPopupMenus::OnTreeViewSaveVRML( wxCommandEvent& event ) {
  wxTreeItemId id = treeview_dialog->TreeViewTree->GetSelection();
  
  H3DViewerTreeViewDialog::TreeIdMap::iterator ni = treeview_dialog->node_map.find( id.m_pItem );
  if( ni == treeview_dialog->node_map.end() ) {
    wxMessageBox( wxT("Selected tree item is not a node"),
                  wxT("Error"),
                  wxOK | wxICON_EXCLAMATION);
  } else {
    auto_ptr< wxFileDialog > file_dialog ( new wxFileDialog ( this,
                                                              wxT("File to save as.."),
                                                              wxT(""),
                                                              wxT(""),
                                                              wxT("*.*"),
                                                              wxFD_SAVE,
                                                              wxDefaultPosition) );

    if (file_dialog->ShowModal() == wxID_OK) {
      std::string filename(file_dialog->GetPath().mb_str());
      std::ofstream os( filename.c_str() );
      if( os.fail() ) {
        wxMessageBox( wxT("Unable to open selected file"), 
                      wxT("Error"),
                      wxOK | wxICON_EXCLAMATION);
      }
      
      try {
        X3D::writeNodeAsVRML( os,
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


/// Callback for node save x3d menu choice.
void H3DViewerPopupMenus::OnTreeViewSaveSTL( wxCommandEvent& event ) {
  wxTreeItemId id = treeview_dialog->TreeViewTree->GetSelection();
  
  H3DViewerTreeViewDialog::TreeIdMap::iterator ni = treeview_dialog->node_map.find( id.m_pItem );
  if( ni == treeview_dialog->node_map.end() ) {
    wxMessageBox( wxT("Selected tree item is not a node"),
                  wxT("Error"),
                  wxOK | wxICON_EXCLAMATION);
    return;
  } else {
    X3DGeometryNode *geom = 
      dynamic_cast< X3DGeometryNode * >( (*ni).second.get() );

    if( !geom ) {
      wxMessageBox( wxT("Selected tree item is not a geometry node"),
                    wxT("Error"),
                    wxOK | wxICON_EXCLAMATION);
      return;
    }

    auto_ptr< wxFileDialog > file_dialog( new wxFileDialog ( this,
                                                             wxT("File to save as.."),
                                                             wxT(""),
                                                             wxT(""),
                                                             wxT("*.*"),
                                                             wxFD_SAVE,
                                                             wxDefaultPosition) );

    if (file_dialog->ShowModal() == wxID_OK) {
      std::string filename(file_dialog->GetPath().mb_str());
      std::ofstream os( filename.c_str() );
      if( os.fail() ) {
        wxMessageBox( wxT("Unable to open selected file"), 
                      wxT("Error"),
                      wxOK | wxICON_EXCLAMATION);
      }
      
      try {
        X3D::writeGeometryAsSTL( os, geom );
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

/// Callback for node save x3d menu choice.
void H3DViewerPopupMenus::OnTreeViewSaveTrianglesX3D( wxCommandEvent& event ) {
  wxTreeItemId id = treeview_dialog->TreeViewTree->GetSelection();
  
  H3DViewerTreeViewDialog::TreeIdMap::iterator ni = treeview_dialog->node_map.find( id.m_pItem );
  if( ni == treeview_dialog->node_map.end() ) {
    wxMessageBox( wxT("Selected tree item is not a node"),
                  wxT("Error"),
                  wxOK | wxICON_EXCLAMATION);
  } else {
    wxFileDialog *file_dialog = new wxFileDialog ( this,
                                                   wxT("File to save as.."),
                                                   wxT(""),
                                                   wxT(""),
                                                   wxT("*.*"),
                                                   wxFD_SAVE,
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
        Node *n = (*ni).second.get();

        AutoRef< IndexedTriangleSet > its( new IndexedTriangleSet );

        vector< Vec3f > triangles;
        triangles.reserve( 200 );
        treeview_dialog->collectAllTriangles( n, Matrix4f(), triangles ); 

        Coordinate *c = new Coordinate;
        c->point->setValue( triangles );
        vector< int > indices;
        indices.reserve( triangles.size() );
        for( unsigned int i = 0; i < triangles.size(); ++i ) {
          indices.push_back( i );
        }

        its->coord->setValue( c );
        its->index->setValue( indices );
       
        X3D::writeNodeAsX3D( os, its.get() );
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

void H3DViewerPopupMenus::OnTreeViewDeleteNode( wxCommandEvent& event ) {
  wxTreeItemId id = treeview_dialog->TreeViewTree->GetSelection();
  
  H3DViewerTreeViewDialog::TreeIdMap::iterator ni = treeview_dialog->node_map.find( id.m_pItem );
  if( ni == treeview_dialog->node_map.end() ) {
    wxMessageBox( wxT("Selected tree item is not a node"),
                  wxT("Error"),
                  wxOK | wxICON_EXCLAMATION);
    return;
  }

  Node *selected_node = (*ni).second.get();

  if( dynamic_cast< Scene * >( selected_node ) ) {
    wxMessageBox( wxT("Deletion of Scene node not allowed."),
                  wxT("Error"),
                  wxOK | wxICON_EXCLAMATION);
    return;
  }

  if( dynamic_cast< H3DWindowNode * >( selected_node ) ) {
    wxMessageBox( wxT("Deletion of H3DWindowNode node not allowed."),
                  wxT("Error"),
                  wxOK | wxICON_EXCLAMATION);
    return;
  }

  wxTreeItemId parent_id = treeview_dialog->TreeViewTree->GetItemParent( id );
  H3DViewerTreeViewDialog::TreeIdMap::iterator ni_parent = treeview_dialog->node_map.find( parent_id.m_pItem );

  if( ni_parent == treeview_dialog->node_map.end() ) {
    wxMessageBox( wxT("Selected tree item does not have a parent"),
                  wxT("Error"),
                  wxOK | wxICON_EXCLAMATION);
    return;
  }

  Node *n = (*ni_parent).second.get();

  H3DNodeDatabase *db = H3DNodeDatabase::lookupNodeInstance( n );
  for( H3DNodeDatabase::FieldDBConstIterator i = db->fieldDBBegin();
       db->fieldDBEnd() != i; ++i ) {
    Field *f = i.getField( n ); 
    if( SFNode *sfnode = dynamic_cast< SFNode * >( f ) ) {
      if( sfnode->getValue() == selected_node ) {
        if( sfnode->getAccessType() == Field::OUTPUT_ONLY ||
            sfnode->getAccessType() == Field::INITIALIZE_ONLY ) {
          wxMessageBox( wxT("Deletion of node in INITIALIZE_ONLY or OUTPUT_ONLY field not allowed."),
                        wxT("Error"),
                        wxOK | wxICON_EXCLAMATION);
        } else {
          sfnode->setValue( NULL );
        }
        return;
      }
    } else if( MFNode *mfnode = dynamic_cast< MFNode * >( f ) ) {
      for( unsigned int j = 0; j < mfnode->size(); ++j ) {
        if( mfnode->getValueByIndex( j ) == selected_node ) { 
          if( mfnode->getAccessType() == Field::OUTPUT_ONLY ||
              mfnode->getAccessType() == Field::INITIALIZE_ONLY ) {
            wxMessageBox( wxT("Deletion of node in INITIALIZE_ONLY or OUTPUT_ONLY field not allowed."),
                          wxT("Error"),
                          wxOK | wxICON_EXCLAMATION);
          } else {
            mfnode->erase( selected_node );
          }
        }
      }
    }
  }
  
}

void H3DViewerPopupMenus::OnTreeViewAddChildNode( wxCommandEvent& event ) {
  wxTreeItemId id = treeview_dialog->TreeViewTree->GetSelection();
  
  H3DViewerTreeViewDialog::TreeIdMap::iterator ni = treeview_dialog->node_map.find( id.m_pItem );
  if( ni == treeview_dialog->node_map.end() ) {
    wxMessageBox( wxT("Selected tree item is not a node"),
                  wxT("Error"),
                  wxOK | wxICON_EXCLAMATION);
  }

  vector< wxString > node_fields;

  Node *selected_node = (*ni).second.get();
  H3DNodeDatabase *db = H3DNodeDatabase::lookupNodeInstance( selected_node );
  for( H3DNodeDatabase::FieldDBConstIterator i = db->fieldDBBegin();
       db->fieldDBEnd() != i; ++i ) {
    Field *f = i.getField( selected_node ); 
    if( SFNode *sfnode = dynamic_cast< SFNode * >( f ) ) {
      if( sfnode->getAccessType() == Field::INPUT_ONLY ||
          sfnode->getAccessType() == Field::INPUT_OUTPUT ) {
        node_fields.push_back( wxString(sfnode->getName().c_str(),wxConvUTF8) );
      } 
    } else if( MFNode *mfnode = dynamic_cast< MFNode * >( f ) ) {
      if( mfnode->getAccessType() == Field::INPUT_ONLY ||
          mfnode->getAccessType() == Field::INPUT_OUTPUT ) {
        node_fields.push_back( wxString(mfnode->getName().c_str(), wxConvUTF8) );
      }
    }
  }
  
  if( node_fields.empty() ) {
    wxMessageBox( wxT("Selected node does not have a SFNode or MFNode field."),
                  wxT("Error"),
                  wxOK | wxICON_EXCLAMATION);
    return;
  }

  string field_to_change;

  if( node_fields.size() > 1 ) {
    wxString *choices = new wxString[ node_fields.size() ];
    for( unsigned int i = 0; i < node_fields.size(); ++i ) {
      choices[i] = node_fields[i];
    }
    wxSingleChoiceDialog *choice_dialog = new wxSingleChoiceDialog ( this,
                                                                     wxT("Add/replace node in which field.."),
                                                                     wxT(""),
                                                                     node_fields.size(),
                                                                     choices );
    delete [] choices;

    if (choice_dialog->ShowModal() == wxID_OK) {
      field_to_change = std::string( node_fields[ choice_dialog->GetSelection() ].mb_str() );
    } else {
      return;
    }
  }

  wxTextEntryDialog *node_name_dialog = 
    new wxTextEntryDialog(this, 
                          wxT("Enter the name of the node type you want to use" ),
                          wxT("Add/replace node" ) );
  if (node_name_dialog->ShowModal() == wxID_OK) {
    Node *new_node = H3DNodeDatabase::createNode( std::string(node_name_dialog->GetValue().mb_str()) );
    if( !new_node ) {
      wxMessageBox( wxT("No such node type exists: " + node_name_dialog->GetValue()),
                    wxT("Error"),
                    wxOK | wxICON_EXCLAMATION);
    } else {
      Field *f = selected_node->getField( field_to_change );
      SFNode *sfnode = dynamic_cast< SFNode * >( f );
      MFNode *mfnode = dynamic_cast< MFNode * >( f );
       
      if( sfnode ) {
        AutoRef< Node > old_node( sfnode->getValue() );
        try { 
          sfnode->setValue( new_node );
        } catch (...) {
          sfnode->setValue( old_node.get() );
          wxMessageBox( wxT("Invalid node type for field"),
                        wxT("Error"),
                        wxOK | wxICON_EXCLAMATION);
        }
      } else if( mfnode ) {
        try { 
          mfnode->push_back( new_node );
        } catch (...) {
          wxMessageBox( wxT("Invalid node type for field"),
                        wxT("Error"),
                        wxOK | wxICON_EXCLAMATION);
        }
      }
    }
  } 
}

// Callback for node save Nrrd menu choice.
void H3DViewerPopupMenus::OnTreeViewSaveNrrd( wxCommandEvent& event ) {
#ifdef HAVE_TEEM
  wxTreeItemId id = treeview_dialog->TreeViewTree->GetSelection();
  
  H3DViewerTreeViewDialog::TreeIdMap::iterator ni = treeview_dialog->node_map.find( id.m_pItem );
  if( ni == treeview_dialog->node_map.end() ) {
    wxMessageBox( wxT("Selected tree item is not a node"),
                  wxT("Error"),
                  wxOK | wxICON_EXCLAMATION);
  } else {
    auto_ptr< wxFileDialog > file_dialog ( new wxFileDialog ( this,
                                                              wxT("File to save as.."),
                                                              wxT(""),
                                                              wxT(""),
                                                              wxT("*.nrrd"),
                                                              wxFD_SAVE,
                                                              wxDefaultPosition) );

    if (file_dialog->ShowModal() == wxID_OK) {
      try {
        Node *n = (*ni).second.get();
        Image *image = NULL;
        if( X3DTexture2DNode * tex = 
            dynamic_cast< X3DTexture2DNode * >( n ) ) {
          image = tex->image->getValue();
        } else if( X3DTexture3DNode * tex = 
                   dynamic_cast< X3DTexture3DNode * >( n ) ) {
          image = tex->image->getValue();
        }
 
        if( H3DUtil::saveImageAsNrrdFile( string(file_dialog->GetPath().mb_str()),
                                          image ) != 0 ) {
          stringstream s;
          s << "Error saving nrrd file";
          wxMessageBox( wxString(s.str().c_str(),wxConvUTF8), wxT("Error"),
                        wxOK | wxICON_EXCLAMATION);
        }
      } catch (const Exception::H3DException &e) {
        stringstream s;
        s << e;
        wxMessageBox( wxString(s.str().c_str(),wxConvUTF8), wxT("Error"),
                      wxOK | wxICON_EXCLAMATION);
      }
    }
  }
#endif
}

// Callback for node save Png menu choice.
void H3DViewerPopupMenus::OnTreeViewSavePng( wxCommandEvent& event ) {
#ifdef HAVE_FREEIMAGE
  wxTreeItemId id = treeview_dialog->TreeViewTree->GetSelection();
  
  H3DViewerTreeViewDialog::TreeIdMap::iterator ni = treeview_dialog->node_map.find( id.m_pItem );
  if( ni == treeview_dialog->node_map.end() ) {
    wxMessageBox( wxT("Selected tree item is not a node"),
                  wxT("Error"),
                  wxOK | wxICON_EXCLAMATION);
  } else {
    auto_ptr< wxFileDialog > file_dialog ( new wxFileDialog ( this,
                                                              wxT("File to save as.."),
                                                              wxT(""),
                                                              wxT(""),
                                                              wxT("*.png"),
                                                              wxFD_SAVE,
                                                              wxDefaultPosition) );

    if (file_dialog->ShowModal() == wxID_OK) {
      try {
        Node *n = (*ni).second.get();
        Image *image = NULL;
        if( X3DTexture2DNode * tex = 
            dynamic_cast< X3DTexture2DNode * >( n ) ) {
          image = tex->image->getValue();
        } else if( X3DTexture3DNode * tex = 
                   dynamic_cast< X3DTexture3DNode * >( n ) ) {
          image = tex->image->getValue();
        }
 
        if( !H3DUtil::saveFreeImagePNG( string(file_dialog->GetPath().mb_str()),
                                          *image ) ) {
          stringstream s;
          s << "Error saving png file";
          wxMessageBox( wxString(s.str().c_str(),wxConvUTF8), wxT("Error"),
                        wxOK | wxICON_EXCLAMATION);
        }
      } catch (const Exception::H3DException &e) {
        stringstream s;
        s << e;
        wxMessageBox( wxString(s.str().c_str(),wxConvUTF8), wxT("Error"),
                      wxOK | wxICON_EXCLAMATION);
      }
    }
  }
#endif
}
