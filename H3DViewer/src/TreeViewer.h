///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 16 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __TreeViewer__
#define __TreeViewer__

#include <wx/treectrl.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/grid.h>
#include <wx/splitter.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class TreeViewDialog
///////////////////////////////////////////////////////////////////////////////
class TreeViewDialog : public wxDialog 
{
	private:
	
	protected:
		wxSplitterWindow* m_splitter1;
		wxPanel* m_panel1;
		wxPanel* m_panel2;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnIdle( wxIdleEvent& event ){ event.Skip(); }
		virtual void OnNodeSelected( wxTreeEvent& event ){ event.Skip(); }
		virtual void OnCellEdit( wxGridEvent& event ){ event.Skip(); }
		
	
	public:
		wxTreeCtrl* TreeViewTree;
		wxGrid* FieldValuesGrid;
		TreeViewDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Tree view"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 588,474 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER );
		~TreeViewDialog();
		void m_splitter1OnIdle( wxIdleEvent& )
		{
		m_splitter1->SetSashPosition( 346 );
		m_splitter1->Disconnect( wxEVT_IDLE, wxIdleEventHandler( TreeViewDialog::m_splitter1OnIdle ), NULL, this );
		}
		
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class FieldValuesDialog
///////////////////////////////////////////////////////////////////////////////
class FieldValuesDialog : public wxDialog 
{
	private:
	
	protected:
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnIdle( wxIdleEvent& event ){ event.Skip(); }
		virtual void OnCellEdit( wxGridEvent& event ){ event.Skip(); }
		
	
	public:
		wxGrid* FieldValuesGrid;
		FieldValuesDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 197,265 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER );
		~FieldValuesDialog();
	
};

#endif //__TreeViewer__
