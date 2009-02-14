///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 16 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "TreeViewer.h"

///////////////////////////////////////////////////////////////////////////

MainFrame::MainFrame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	MainMenuBar = new wxMenuBar( 0 );
	FileMenu = new wxMenu();
	wxMenuItem* OpenFileMenuChoice;
	OpenFileMenuChoice = new wxMenuItem( FileMenu, wxID_ANY, wxString( wxT("Open file...") ) , wxEmptyString, wxITEM_NORMAL );
	FileMenu->Append( OpenFileMenuChoice );
	
	wxMenuItem* CloseFileMenuChoice;
	CloseFileMenuChoice = new wxMenuItem( FileMenu, wxID_ANY, wxString( wxT("Close") ) , wxEmptyString, wxITEM_NORMAL );
	FileMenu->Append( CloseFileMenuChoice );
	
	FileMenu->AppendSeparator();
	
	wxMenuItem* QuitProgram;
	QuitProgram = new wxMenuItem( FileMenu, wxID_ANY, wxString( wxT("Quit") ) , wxEmptyString, wxITEM_NORMAL );
	FileMenu->Append( QuitProgram );
	
	MainMenuBar->Append( FileMenu, wxT("File") );
	
	this->SetMenuBar( MainMenuBar );
	
	
	// Connect Events
	this->Connect( wxEVT_IDLE, wxIdleEventHandler( MainFrame::OnIdle ) );
	this->Connect( OpenFileMenuChoice->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnOpenFile ) );
	this->Connect( CloseFileMenuChoice->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnClose ) );
	this->Connect( QuitProgram->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnQuit ) );
}

MainFrame::~MainFrame()
{
	// Disconnect Events
	this->Disconnect( wxEVT_IDLE, wxIdleEventHandler( MainFrame::OnIdle ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnOpenFile ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnClose ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnQuit ) );
}

TreeViewDialog::TreeViewDialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );
	
	TreeViewTree = new wxTreeCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTR_DEFAULT_STYLE );
	bSizer1->Add( TreeViewTree, 1, wxALL|wxEXPAND, 5 );
	
	this->SetSizer( bSizer1 );
	this->Layout();
	
	// Connect Events
	TreeViewTree->Connect( wxEVT_COMMAND_TREE_SEL_CHANGED, wxTreeEventHandler( TreeViewDialog::OnNodeSelected ), NULL, this );
}

TreeViewDialog::~TreeViewDialog()
{
	// Disconnect Events
	TreeViewTree->Disconnect( wxEVT_COMMAND_TREE_SEL_CHANGED, wxTreeEventHandler( TreeViewDialog::OnNodeSelected ), NULL, this );
}

FieldValuesDialog::FieldValuesDialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxVERTICAL );
	
	FieldValuesGrid = new wxGrid( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	
	// Grid
	FieldValuesGrid->CreateGrid( 2, 2 );
	FieldValuesGrid->EnableEditing( true );
	FieldValuesGrid->EnableGridLines( true );
	FieldValuesGrid->EnableDragGridSize( false );
	FieldValuesGrid->SetMargins( 0, 0 );
	
	// Columns
	FieldValuesGrid->SetColSize( 0, 95 );
	FieldValuesGrid->SetColSize( 1, 102 );
	FieldValuesGrid->EnableDragColMove( false );
	FieldValuesGrid->EnableDragColSize( true );
	FieldValuesGrid->SetColLabelSize( 15 );
	FieldValuesGrid->SetColLabelValue( 0, wxT("Field") );
	FieldValuesGrid->SetColLabelValue( 1, wxT("Value") );
	FieldValuesGrid->SetColLabelAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );
	
	// Rows
	FieldValuesGrid->EnableDragRowSize( true );
	FieldValuesGrid->SetRowLabelSize( 0 );
	FieldValuesGrid->SetRowLabelAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );
	
	// Label Appearance
	
	// Cell Defaults
	FieldValuesGrid->SetDefaultCellAlignment( wxALIGN_LEFT, wxALIGN_TOP );
	bSizer2->Add( FieldValuesGrid, 1, wxALL|wxEXPAND, 5 );
	
	this->SetSizer( bSizer2 );
	this->Layout();
	
	// Connect Events
	this->Connect( wxEVT_IDLE, wxIdleEventHandler( FieldValuesDialog::OnIdle ) );
	FieldValuesGrid->Connect( wxEVT_GRID_CELL_CHANGE, wxGridEventHandler( FieldValuesDialog::OnCellEdit ), NULL, this );
}

FieldValuesDialog::~FieldValuesDialog()
{
	// Disconnect Events
	this->Disconnect( wxEVT_IDLE, wxIdleEventHandler( FieldValuesDialog::OnIdle ) );
	FieldValuesGrid->Disconnect( wxEVT_GRID_CELL_CHANGE, wxGridEventHandler( FieldValuesDialog::OnCellEdit ), NULL, this );
}
