///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 16 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __H3DViewer__
#define __H3DViewer__

#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/menu.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/treectrl.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/grid.h>
#include <wx/splitter.h>
#include <wx/frame.h>
#include <wx/stattext.h>
#include <wx/listbox.h>
#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/richtext/richtextctrl.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class TreeViewDialog
///////////////////////////////////////////////////////////////////////////////
class TreeViewDialog : public wxFrame 
{
	private:
	
	protected:
		wxMenuBar* m_menubar1;
		wxMenu* RightClickMenu;
		wxMenu* RightClickMenuGeometry;
		wxSplitterWindow* m_splitter1;
		wxPanel* m_panel1;
		wxPanel* m_panel2;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnClose( wxCloseEvent& event ){ event.Skip(); }
		virtual void OnIdle( wxIdleEvent& event ){ event.Skip(); }
		virtual void OnTreeViewCollapseAll( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnTreeViewExpandAll( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnTreeViewCollapseChildren( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnTreeViewNodeWatch( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnTreeViewSaveX3D( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnTreeViewSaveSTL( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnTreeRightClick( wxTreeEvent& event ){ event.Skip(); }
		virtual void OnNodeSelected( wxTreeEvent& event ){ event.Skip(); }
		virtual void OnCellEdit( wxGridEvent& event ){ event.Skip(); }
		
	
	public:
		wxTreeCtrl* TreeViewTree;
		wxGrid* FieldValuesGrid;
		TreeViewDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Scene tree view"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 594,453 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
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
class FieldValuesDialog : public wxFrame 
{
	private:
	
	protected:
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnIdle( wxIdleEvent& event ){ event.Skip(); }
		virtual void OnCellEdit( wxGridEvent& event ){ event.Skip(); }
		
	
	public:
		wxGrid* FieldValuesGrid;
		FieldValuesDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 236,335 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		~FieldValuesDialog();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class PluginsDialog
///////////////////////////////////////////////////////////////////////////////
class PluginsDialog : public wxDialog 
{
	private:
	
	protected:
		wxStaticText* m_staticText1;
		wxListBox* InstalledPluginsList;
		wxButton* m_button2;
		wxButton* m_button1;
		
		wxRichTextCtrl* PluginInfoText;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnInstalledPluginSelected( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnAddPluginButton( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnRemovePluginButton( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnDisablePluginCheckbox( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnURLEvent( wxTextUrlEvent& event ){ event.Skip(); }
		
	
	public:
		wxCheckBox* DisablePluginsCheckBox;
		PluginsDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Plugins"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 420,326 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER );
		~PluginsDialog();
	
};

#endif //__H3DViewer__
