#include "TreeViewerMainFrame.h"

#include <wx/wx.h>
#include <H3D/Scene.h>
#include <H3D/X3D.h>
#include <H3D/Node.h>
#include <H3D/H3DNodeDatabase.h>

#include "TreeViewerApp.h"

using namespace H3D;

TreeViewerMainFrame::TreeViewerMainFrame( wxWindow* parent )
:
MainFrame( parent )
{

}


void TreeViewerMainFrame::OnOpenFile( wxCommandEvent& event )
{
  wxFileDialog *openFileDialog = new wxFileDialog ( this,
                             wxT("Open file"),
			     wxT(""),
                             wxT(""),
                             wxT("*.*"),
                             wxOPEN,
                             wxDefaultPosition) ;

  //Open an X3D file
  if (openFileDialog->ShowModal() == wxID_OK) {
    std::string filename(openFileDialog->GetPath().mb_str());
    TreeViewerApp::h3d_scene->sceneRoot->setValue( X3D::createX3DFromURL( filename.c_str() ) );
  }

  TreeViewerTreeViewDialog *tree = 
    static_cast< TreeViewerApp * >(wxTheApp )->tree_dialog;
  TreeViewerFieldValuesDialog *field_values = 
    static_cast< TreeViewerApp * >(wxTheApp )->field_values_dialog;
 
  //addSceneToTree( tree->TreeViewTree, root, TreeViewerApp::h3d_scene->sceneRoot->getValue() );
  tree->showNodeAsTree( TreeViewerApp::h3d_scene->sceneRoot->getValue() );
  field_values->displayFieldsFromNode( TreeViewerApp::h3d_scene->sceneRoot->getValue() );
}

void TreeViewerMainFrame::OnClose( wxCommandEvent& event )
{
	// TODO: Implement OnClose
}

void TreeViewerMainFrame::OnQuit( wxCommandEvent& event )
{
	// TODO: Implement OnQuit
}

void TreeViewerMainFrame::OnIdle( wxIdleEvent& event ) {
  for( set< Scene * >::iterator i = Scene::scenes.begin();
       i != Scene::scenes.end();
       i++ ) {
    if( (*i)->isActive() )
      (*i)->idle();
  }
}
