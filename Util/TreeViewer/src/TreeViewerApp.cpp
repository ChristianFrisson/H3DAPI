#include "TreeViewerApp.h"
#include "TreeViewerMainFrame.h"
#include "WxWidgetsWindow.h"
#include <H3D/Group.h>

H3D::AutoRef< H3D::Scene > TreeViewerApp::h3d_scene;
H3D::AutoRef< H3D::WxWidgetsWindow > TreeViewerApp::h3d_window;

IMPLEMENT_APP(TreeViewerApp)

TreeViewerApp::TreeViewerApp()
{
}

TreeViewerApp::~TreeViewerApp()
{
}

bool TreeViewerApp::OnInit()
{
  TreeViewerMainFrame* frame = new TreeViewerMainFrame( (wxWindow*)NULL );
  h3d_window.reset( new H3D::WxWidgetsWindow( frame ) );

  // init H3D stuff
  int width, height;
  frame->GetClientSize(&width, &height);
  h3d_window->width->setValue(width);
  h3d_window->height->setValue(height);

  h3d_scene.reset( new H3D::Scene );
  h3d_scene->window->push_back( h3d_window.get() );
  h3d_scene->sceneRoot->setValue( new H3D::Group );

  frame->Show();

  field_values_dialog = new TreeViewerFieldValuesDialog( frame );
  field_values_dialog->Show();

  tree_dialog = new TreeViewerTreeViewDialog( frame );
  tree_dialog->Show();



  SetTopWindow( frame );
  return true;
}

