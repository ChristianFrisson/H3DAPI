///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Nov  6 2013)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "H3DViewer.h"

///////////////////////////////////////////////////////////////////////////

TreeViewDialog::TreeViewDialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer14;
	bSizer14 = new wxBoxSizer( wxVERTICAL );
	
	SplitterWindow = new wxSplitterWindow( this, wxID_ANY, wxDefaultPosition, wxSize( -1,500 ), wxSP_3D );
	SplitterWindow->SetSashGravity( 1 );
	SplitterWindow->Connect( wxEVT_IDLE, wxIdleEventHandler( TreeViewDialog::SplitterWindowOnIdle ), NULL, this );
	
	SplitterWindow->SetMinSize( wxSize( -1,500 ) );
	
	TreeViewPanel = new wxPanel( SplitterWindow, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxVERTICAL );
	
	TreeViewTree = new wxTreeCtrl( TreeViewPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTR_DEFAULT_STYLE );
	bSizer3->Add( TreeViewTree, 1, wxALL|wxEXPAND, 5 );
	
	
	TreeViewPanel->SetSizer( bSizer3 );
	TreeViewPanel->Layout();
	bSizer3->Fit( TreeViewPanel );
	FieldValueViewPanel = new wxPanel( SplitterWindow, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	SplitterWindow->SplitVertically( TreeViewPanel, FieldValueViewPanel, 283 );
	bSizer14->Add( SplitterWindow, 1, wxEXPAND, 5 );
	
	btnClose = new wxButton( this, wxID_CANCEL, wxT("Close"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer14->Add( btnClose, 0, wxALIGN_RIGHT|wxALL, 5 );
	
	
	bSizer1->Add( bSizer14, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer1 );
	this->Layout();
	
	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( TreeViewDialog::OnClose ) );
	this->Connect( wxEVT_IDLE, wxIdleEventHandler( TreeViewDialog::OnIdle ) );
	TreeViewTree->Connect( wxEVT_COMMAND_TREE_ITEM_RIGHT_CLICK, wxTreeEventHandler( TreeViewDialog::OnTreeRightClick ), NULL, this );
	TreeViewTree->Connect( wxEVT_COMMAND_TREE_SEL_CHANGED, wxTreeEventHandler( TreeViewDialog::OnNodeSelected ), NULL, this );
	btnClose->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( TreeViewDialog::btnCloseClick ), NULL, this );
}

TreeViewDialog::~TreeViewDialog()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( TreeViewDialog::OnClose ) );
	this->Disconnect( wxEVT_IDLE, wxIdleEventHandler( TreeViewDialog::OnIdle ) );
	TreeViewTree->Disconnect( wxEVT_COMMAND_TREE_ITEM_RIGHT_CLICK, wxTreeEventHandler( TreeViewDialog::OnTreeRightClick ), NULL, this );
	TreeViewTree->Disconnect( wxEVT_COMMAND_TREE_SEL_CHANGED, wxTreeEventHandler( TreeViewDialog::OnNodeSelected ), NULL, this );
	btnClose->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( TreeViewDialog::btnCloseClick ), NULL, this );
	
}

PluginsDialog::PluginsDialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxHORIZONTAL );
	
	PluginsSizer = new wxBoxSizer( wxVERTICAL );
	
	m_staticText1 = new wxStaticText( this, wxID_ANY, wxT("Installed plugins:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1->Wrap( -1 );
	PluginsSizer->Add( m_staticText1, 0, wxALL, 5 );
	
	InstalledPluginsList = new wxListBox( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 ); 
	InstalledPluginsList->SetMinSize( wxSize( 180,200 ) );
	
	PluginsSizer->Add( InstalledPluginsList, 0, wxALL, 5 );
	
	AddRemoveButtonSizer = new wxBoxSizer( wxHORIZONTAL );
	
	AddPluginButton = new wxButton( this, wxID_ANY, wxT("Add.."), wxDefaultPosition, wxSize( -1,-1 ), 0 );
	AddRemoveButtonSizer->Add( AddPluginButton, 0, wxALL, 5 );
	
	RemovePluginButton = new wxButton( this, wxID_ANY, wxT("Remove.."), wxDefaultPosition, wxDefaultSize, 0 );
	AddRemoveButtonSizer->Add( RemovePluginButton, 0, wxALL, 5 );
	
	
	PluginsSizer->Add( AddRemoveButtonSizer, 1, wxEXPAND, 5 );
	
	DisablePluginsCheckBox = new wxCheckBox( this, wxID_ANY, wxT("Disable plugins"), wxDefaultPosition, wxDefaultSize, 0 );
	PluginsSizer->Add( DisablePluginsCheckBox, 0, wxALL, 5 );
	
	
	bSizer5->Add( PluginsSizer, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer( wxVERTICAL );
	
	PluginInfoText = new wxRichTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_AUTO_URL|wxTE_READONLY|wxVSCROLL|wxHSCROLL|wxNO_BORDER|wxWANTS_CHARS );
	PluginInfoText->Hide();
	
	bSizer7->Add( PluginInfoText, 1, wxEXPAND | wxALL, 5 );
	
	
	bSizer5->Add( bSizer7, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer5 );
	this->Layout();
	
	// Connect Events
	this->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( PluginsDialog::OnKeyDown ) );
	InstalledPluginsList->Connect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( PluginsDialog::OnInstalledPluginSelected ), NULL, this );
	AddPluginButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PluginsDialog::OnAddPluginButton ), NULL, this );
	RemovePluginButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PluginsDialog::OnRemovePluginButton ), NULL, this );
	DisablePluginsCheckBox->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( PluginsDialog::OnDisablePluginCheckbox ), NULL, this );
	PluginInfoText->Connect( wxEVT_COMMAND_TEXT_URL, wxTextUrlEventHandler( PluginsDialog::OnURLEvent ), NULL, this );
}

PluginsDialog::~PluginsDialog()
{
	// Disconnect Events
	this->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( PluginsDialog::OnKeyDown ) );
	InstalledPluginsList->Disconnect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( PluginsDialog::OnInstalledPluginSelected ), NULL, this );
	AddPluginButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PluginsDialog::OnAddPluginButton ), NULL, this );
	RemovePluginButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PluginsDialog::OnRemovePluginButton ), NULL, this );
	DisablePluginsCheckBox->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( PluginsDialog::OnDisablePluginCheckbox ), NULL, this );
	PluginInfoText->Disconnect( wxEVT_COMMAND_TEXT_URL, wxTextUrlEventHandler( PluginsDialog::OnURLEvent ), NULL, this );
	
}

MenuContainer::MenuContainer( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	this->Hide();
	
	m_menubar2 = new wxMenuBar( 0 );
	RightClickMenu = new wxMenu();
	wxMenuItem* TreeViewCollapseAll;
	TreeViewCollapseAll = new wxMenuItem( RightClickMenu, wxID_ANY, wxString( wxT("Collapse all") ) , wxEmptyString, wxITEM_NORMAL );
	RightClickMenu->Append( TreeViewCollapseAll );
	
	wxMenuItem* TreeViewExpandAll;
	TreeViewExpandAll = new wxMenuItem( RightClickMenu, wxID_ANY, wxString( wxT("Expand all") ) , wxEmptyString, wxITEM_NORMAL );
	RightClickMenu->Append( TreeViewExpandAll );
	
	wxMenuItem* TreeViewCollapseChildren;
	TreeViewCollapseChildren = new wxMenuItem( RightClickMenu, wxID_ANY, wxString( wxT("Collapse children") ) , wxEmptyString, wxITEM_NORMAL );
	RightClickMenu->Append( TreeViewCollapseChildren );
	
	RightClickMenu->AppendSeparator();
	
	wxMenuItem* TreeViewDeleteNode;
	TreeViewDeleteNode = new wxMenuItem( RightClickMenu, wxID_ANY, wxString( wxT("Delete node") ) , wxEmptyString, wxITEM_NORMAL );
	RightClickMenu->Append( TreeViewDeleteNode );
	
	wxMenuItem* TreeViewAddChildNode;
	TreeViewAddChildNode = new wxMenuItem( RightClickMenu, wxID_ANY, wxString( wxT("Add/replace child node..") ) , wxEmptyString, wxITEM_NORMAL );
	RightClickMenu->Append( TreeViewAddChildNode );
	
	RightClickMenu->AppendSeparator();
	
	wxMenuItem* TreeViewNodeWatch;
	TreeViewNodeWatch = new wxMenuItem( RightClickMenu, wxID_ANY, wxString( wxT("Add node field watch") ) , wxEmptyString, wxITEM_NORMAL );
	RightClickMenu->Append( TreeViewNodeWatch );
	
	wxMenuItem* TreeViewSaveX3D;
	TreeViewSaveX3D = new wxMenuItem( RightClickMenu, wxID_ANY, wxString( wxT("Save node snapshot as X3D..") ) , wxEmptyString, wxITEM_NORMAL );
	RightClickMenu->Append( TreeViewSaveX3D );
	
	wxMenuItem* TreeViewSaveVRML;
	TreeViewSaveVRML = new wxMenuItem( RightClickMenu, wxID_ANY, wxString( wxT("Save node snapshot as VRML..") ) , wxEmptyString, wxITEM_NORMAL );
	RightClickMenu->Append( TreeViewSaveVRML );
	
	wxMenuItem* TreeViewSaveTrianglesX3D;
	TreeViewSaveTrianglesX3D = new wxMenuItem( RightClickMenu, wxID_ANY, wxString( wxT("Save all triangles as IndexedTriangleSet..") ) , wxEmptyString, wxITEM_NORMAL );
	RightClickMenu->Append( TreeViewSaveTrianglesX3D );
	
	wxMenuItem* TreeViewLookAt;
	TreeViewLookAt = new wxMenuItem( RightClickMenu, wxID_ANY, wxString( wxT("Look at") ) , wxEmptyString, wxITEM_NORMAL );
	RightClickMenu->Append( TreeViewLookAt );
	
	m_menubar2->Append( RightClickMenu, wxT("Action") ); 
	
	RightClickMenuTexture = new wxMenu();
	wxMenuItem* TreeViewCollapseAll11;
	TreeViewCollapseAll11 = new wxMenuItem( RightClickMenuTexture, wxID_ANY, wxString( wxT("Collapse all") ) , wxEmptyString, wxITEM_NORMAL );
	RightClickMenuTexture->Append( TreeViewCollapseAll11 );
	
	wxMenuItem* TreeViewExpandAll11;
	TreeViewExpandAll11 = new wxMenuItem( RightClickMenuTexture, wxID_ANY, wxString( wxT("Expand all") ) , wxEmptyString, wxITEM_NORMAL );
	RightClickMenuTexture->Append( TreeViewExpandAll11 );
	
	wxMenuItem* TreeViewCollapseChildren11;
	TreeViewCollapseChildren11 = new wxMenuItem( RightClickMenuTexture, wxID_ANY, wxString( wxT("Collapse children") ) , wxEmptyString, wxITEM_NORMAL );
	RightClickMenuTexture->Append( TreeViewCollapseChildren11 );
	
	RightClickMenuTexture->AppendSeparator();
	
	wxMenuItem* TreeViewDeleteNode11;
	TreeViewDeleteNode11 = new wxMenuItem( RightClickMenuTexture, wxID_ANY, wxString( wxT("Delete node") ) , wxEmptyString, wxITEM_NORMAL );
	RightClickMenuTexture->Append( TreeViewDeleteNode11 );
	
	wxMenuItem* TreeViewAddChildNode11;
	TreeViewAddChildNode11 = new wxMenuItem( RightClickMenuTexture, wxID_ANY, wxString( wxT("Add/replace child node..") ) , wxEmptyString, wxITEM_NORMAL );
	RightClickMenuTexture->Append( TreeViewAddChildNode11 );
	
	RightClickMenuTexture->AppendSeparator();
	
	wxMenuItem* TreeViewNodeWatch11;
	TreeViewNodeWatch11 = new wxMenuItem( RightClickMenuTexture, wxID_ANY, wxString( wxT("Add node field watch") ) , wxEmptyString, wxITEM_NORMAL );
	RightClickMenuTexture->Append( TreeViewNodeWatch11 );
	
	wxMenuItem* TreeViewSaveNrrd;
	TreeViewSaveNrrd = new wxMenuItem( RightClickMenuTexture, wxID_ANY, wxString( wxT("Save texture image in NRRD format..") ) , wxEmptyString, wxITEM_NORMAL );
	RightClickMenuTexture->Append( TreeViewSaveNrrd );
	
	wxMenuItem* TreeViewSavePng;
	TreeViewSavePng = new wxMenuItem( RightClickMenuTexture, wxID_ANY, wxString( wxT("Save texture image in png format..") ) , wxT("For 3D-textures depth will be 0."), wxITEM_NORMAL );
	RightClickMenuTexture->Append( TreeViewSavePng );
	
	m_menubar2->Append( RightClickMenuTexture, wxT("Action") ); 
	
	RightClickMenuGeometry = new wxMenu();
	wxMenuItem* TreeViewCollapseAll1;
	TreeViewCollapseAll1 = new wxMenuItem( RightClickMenuGeometry, wxID_ANY, wxString( wxT("Collapse all") ) , wxEmptyString, wxITEM_NORMAL );
	RightClickMenuGeometry->Append( TreeViewCollapseAll1 );
	
	wxMenuItem* TreeViewExpandAll1;
	TreeViewExpandAll1 = new wxMenuItem( RightClickMenuGeometry, wxID_ANY, wxString( wxT("Expand all") ) , wxEmptyString, wxITEM_NORMAL );
	RightClickMenuGeometry->Append( TreeViewExpandAll1 );
	
	wxMenuItem* TreeViewCollapseChildren1;
	TreeViewCollapseChildren1 = new wxMenuItem( RightClickMenuGeometry, wxID_ANY, wxString( wxT("Collapse children") ) , wxEmptyString, wxITEM_NORMAL );
	RightClickMenuGeometry->Append( TreeViewCollapseChildren1 );
	
	RightClickMenuGeometry->AppendSeparator();
	
	wxMenuItem* TreeViewDeleteNode1;
	TreeViewDeleteNode1 = new wxMenuItem( RightClickMenuGeometry, wxID_ANY, wxString( wxT("Delete node") ) , wxEmptyString, wxITEM_NORMAL );
	RightClickMenuGeometry->Append( TreeViewDeleteNode1 );
	
	wxMenuItem* TreeViewAddChildNode1;
	TreeViewAddChildNode1 = new wxMenuItem( RightClickMenuGeometry, wxID_ANY, wxString( wxT("Add/replace child node..") ) , wxEmptyString, wxITEM_NORMAL );
	RightClickMenuGeometry->Append( TreeViewAddChildNode1 );
	
	RightClickMenuGeometry->AppendSeparator();
	
	wxMenuItem* TreeViewNodeWatch1;
	TreeViewNodeWatch1 = new wxMenuItem( RightClickMenuGeometry, wxID_ANY, wxString( wxT("Add node field watch") ) , wxEmptyString, wxITEM_NORMAL );
	RightClickMenuGeometry->Append( TreeViewNodeWatch1 );
	
	wxMenuItem* TreeViewSaveX3D1;
	TreeViewSaveX3D1 = new wxMenuItem( RightClickMenuGeometry, wxID_ANY, wxString( wxT("Save node snapshot as X3D..") ) , wxEmptyString, wxITEM_NORMAL );
	RightClickMenuGeometry->Append( TreeViewSaveX3D1 );
	
	wxMenuItem* TreeViewSaveVRML1;
	TreeViewSaveVRML1 = new wxMenuItem( RightClickMenuGeometry, wxID_ANY, wxString( wxT("Save node snapshot as VRML..") ) , wxEmptyString, wxITEM_NORMAL );
	RightClickMenuGeometry->Append( TreeViewSaveVRML1 );
	
	wxMenuItem* TreeViewSaveSTL;
	TreeViewSaveSTL = new wxMenuItem( RightClickMenuGeometry, wxID_ANY, wxString( wxT("Save geometry triangles as STL..") ) , wxEmptyString, wxITEM_NORMAL );
	RightClickMenuGeometry->Append( TreeViewSaveSTL );
	
	m_menubar2->Append( RightClickMenuGeometry, wxT("Action") ); 
	
	this->SetMenuBar( m_menubar2 );
	
	
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( TreeViewCollapseAll->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MenuContainer::OnTreeViewCollapseAll ) );
	this->Connect( TreeViewExpandAll->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MenuContainer::OnTreeViewExpandAll ) );
	this->Connect( TreeViewCollapseChildren->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MenuContainer::OnTreeViewCollapseChildren ) );
	this->Connect( TreeViewDeleteNode->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MenuContainer::OnTreeViewDeleteNode ) );
	this->Connect( TreeViewAddChildNode->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MenuContainer::OnTreeViewAddChildNode ) );
	this->Connect( TreeViewNodeWatch->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MenuContainer::OnTreeViewNodeWatch ) );
	this->Connect( TreeViewSaveX3D->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MenuContainer::OnTreeViewSaveX3D ) );
	this->Connect( TreeViewSaveVRML->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MenuContainer::OnTreeViewSaveVRML ) );
	this->Connect( TreeViewSaveTrianglesX3D->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MenuContainer::OnTreeViewSaveTrianglesX3D ) );
	this->Connect( TreeViewLookAt->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MenuContainer::onTreeViewLookAt ) );
	this->Connect( TreeViewCollapseAll11->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MenuContainer::OnTreeViewCollapseAll ) );
	this->Connect( TreeViewExpandAll11->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MenuContainer::OnTreeViewExpandAll ) );
	this->Connect( TreeViewCollapseChildren11->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MenuContainer::OnTreeViewCollapseChildren ) );
	this->Connect( TreeViewDeleteNode11->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MenuContainer::OnTreeViewDeleteNode ) );
	this->Connect( TreeViewAddChildNode11->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MenuContainer::OnTreeViewAddChildNode ) );
	this->Connect( TreeViewNodeWatch11->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MenuContainer::OnTreeViewNodeWatch ) );
	this->Connect( TreeViewSaveNrrd->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MenuContainer::OnTreeViewSaveNrrd ) );
	this->Connect( TreeViewSavePng->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MenuContainer::OnTreeViewSavePng ) );
	this->Connect( TreeViewCollapseAll1->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MenuContainer::OnTreeViewCollapseAll ) );
	this->Connect( TreeViewExpandAll1->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MenuContainer::OnTreeViewExpandAll ) );
	this->Connect( TreeViewCollapseChildren1->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MenuContainer::OnTreeViewCollapseChildren ) );
	this->Connect( TreeViewDeleteNode1->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MenuContainer::OnTreeViewDeleteNode ) );
	this->Connect( TreeViewAddChildNode1->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MenuContainer::OnTreeViewAddChildNode ) );
	this->Connect( TreeViewNodeWatch1->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MenuContainer::OnTreeViewNodeWatch ) );
	this->Connect( TreeViewSaveX3D1->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MenuContainer::OnTreeViewSaveX3D ) );
	this->Connect( TreeViewSaveVRML1->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MenuContainer::OnTreeViewSaveVRML ) );
	this->Connect( TreeViewSaveSTL->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MenuContainer::OnTreeViewSaveSTL ) );
}

MenuContainer::~MenuContainer()
{
	// Disconnect Events
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MenuContainer::OnTreeViewCollapseAll ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MenuContainer::OnTreeViewExpandAll ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MenuContainer::OnTreeViewCollapseChildren ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MenuContainer::OnTreeViewDeleteNode ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MenuContainer::OnTreeViewAddChildNode ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MenuContainer::OnTreeViewNodeWatch ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MenuContainer::OnTreeViewSaveX3D ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MenuContainer::OnTreeViewSaveVRML ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MenuContainer::OnTreeViewSaveTrianglesX3D ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MenuContainer::onTreeViewLookAt ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MenuContainer::OnTreeViewCollapseAll ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MenuContainer::OnTreeViewExpandAll ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MenuContainer::OnTreeViewCollapseChildren ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MenuContainer::OnTreeViewDeleteNode ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MenuContainer::OnTreeViewAddChildNode ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MenuContainer::OnTreeViewNodeWatch ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MenuContainer::OnTreeViewSaveNrrd ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MenuContainer::OnTreeViewSavePng ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MenuContainer::OnTreeViewCollapseAll ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MenuContainer::OnTreeViewExpandAll ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MenuContainer::OnTreeViewCollapseChildren ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MenuContainer::OnTreeViewDeleteNode ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MenuContainer::OnTreeViewAddChildNode ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MenuContainer::OnTreeViewNodeWatch ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MenuContainer::OnTreeViewSaveX3D ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MenuContainer::OnTreeViewSaveVRML ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MenuContainer::OnTreeViewSaveSTL ) );
	
}

MenuContainer2::MenuContainer2( long style ) : wxMenuBar( style )
{
}

MenuContainer2::~MenuContainer2()
{
}
