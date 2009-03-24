///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 16 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "TreeViewer.h"

///////////////////////////////////////////////////////////////////////////

TreeViewDialog::TreeViewDialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );
	
	m_splitter1 = new wxSplitterWindow( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_3D );
	m_splitter1->SetSashGravity( 1 );
	m_splitter1->Connect( wxEVT_IDLE, wxIdleEventHandler( TreeViewDialog::m_splitter1OnIdle ), NULL, this );
	m_panel1 = new wxPanel( m_splitter1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxHORIZONTAL );
	
	TreeViewTree = new wxTreeCtrl( m_panel1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTR_DEFAULT_STYLE );
	bSizer3->Add( TreeViewTree, 1, wxALL|wxEXPAND, 5 );
	
	m_panel1->SetSizer( bSizer3 );
	m_panel1->Layout();
	bSizer3->Fit( m_panel1 );
	m_panel2 = new wxPanel( m_splitter1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxVERTICAL );
	
	FieldValuesGrid = new wxGrid( m_panel2, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	
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
	bSizer4->Add( FieldValuesGrid, 1, wxALL|wxEXPAND, 5 );
	
	m_panel2->SetSizer( bSizer4 );
	m_panel2->Layout();
	bSizer4->Fit( m_panel2 );
	m_splitter1->SplitVertically( m_panel1, m_panel2, 346 );
	bSizer1->Add( m_splitter1, 1, wxEXPAND, 5 );
	
	this->SetSizer( bSizer1 );
	this->Layout();
	
	// Connect Events
	this->Connect( wxEVT_IDLE, wxIdleEventHandler( TreeViewDialog::OnIdle ) );
	TreeViewTree->Connect( wxEVT_COMMAND_TREE_SEL_CHANGED, wxTreeEventHandler( TreeViewDialog::OnNodeSelected ), NULL, this );
	FieldValuesGrid->Connect( wxEVT_GRID_CELL_CHANGE, wxGridEventHandler( TreeViewDialog::OnCellEdit ), NULL, this );
}

TreeViewDialog::~TreeViewDialog()
{
	// Disconnect Events
	this->Disconnect( wxEVT_IDLE, wxIdleEventHandler( TreeViewDialog::OnIdle ) );
	TreeViewTree->Disconnect( wxEVT_COMMAND_TREE_SEL_CHANGED, wxTreeEventHandler( TreeViewDialog::OnNodeSelected ), NULL, this );
	FieldValuesGrid->Disconnect( wxEVT_GRID_CELL_CHANGE, wxGridEventHandler( TreeViewDialog::OnCellEdit ), NULL, this );
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
