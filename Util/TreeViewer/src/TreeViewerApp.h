#ifndef __TREEVIEWERAPP_H
#define __TREEVIEWERAPP_H

#include <wx/wx.h>
#include <H3D/Scene.h>
#include "WxWidgetsWindow.h"
#include "TreeViewerTreeViewDialog.h"
#include "TreeViewerFieldValuesDialog.h"

class TreeViewerApp : public wxApp
{
public:
  TreeViewerApp();
  virtual ~TreeViewerApp();
  virtual bool OnInit();

  static H3D::AutoRef< H3D::Scene > h3d_scene;
  static H3D::AutoRef< H3D::WxWidgetsWindow > h3d_window;

  TreeViewerTreeViewDialog *tree_dialog;
  TreeViewerFieldValuesDialog *field_values_dialog;
};

DECLARE_APP(TreeViewerApp)

#endif 
