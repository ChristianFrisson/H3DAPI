///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Nov  6 2013)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __H3DVIEWER_H__
#define __H3DVIEWER_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/treectrl.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/splitter.h>
#include <wx/button.h>
#include <wx/frame.h>
#include <wx/stattext.h>
#include <wx/listbox.h>
#include <wx/checkbox.h>
#include <wx/richtext/richtextctrl.h>
#include <wx/dialog.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/menu.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class TreeViewDialog
///////////////////////////////////////////////////////////////////////////////
class TreeViewDialog : public wxFrame 
{
	private:
	
	protected:
		wxSplitterWindow* SplitterWindow;
		wxPanel* TreeViewPanel;
		wxPanel* FieldValueViewPanel;
		wxButton* btnClose;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnClose( wxCloseEvent& event ) { event.Skip(); }
		virtual void OnIdle( wxIdleEvent& event ) { event.Skip(); }
		virtual void OnTreeRightClick( wxTreeEvent& event ) { event.Skip(); }
		virtual void OnNodeSelected( wxTreeEvent& event ) { event.Skip(); }
		virtual void btnCloseClick( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		wxTreeCtrl* TreeViewTree;
		
		TreeViewDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Scene tree view"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 594,496 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		
		~TreeViewDialog();
		
		void SplitterWindowOnIdle( wxIdleEvent& )
		{
			SplitterWindow->SetSashPosition( 283 );
			SplitterWindow->Disconnect( wxEVT_IDLE, wxIdleEventHandler( TreeViewDialog::SplitterWindowOnIdle ), NULL, this );
		}
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class PluginsDialog
///////////////////////////////////////////////////////////////////////////////
class PluginsDialog : public wxDialog 
{
	private:
	
	protected:
		wxBoxSizer* PluginsSizer;
		wxStaticText* m_staticText1;
		wxListBox* InstalledPluginsList;
		wxBoxSizer* AddRemoveButtonSizer;
		wxButton* AddPluginButton;
		wxButton* RemovePluginButton;
		wxRichTextCtrl* PluginInfoText;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnKeyDown( wxKeyEvent& event ) { event.Skip(); }
		virtual void OnInstalledPluginSelected( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnAddPluginButton( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnRemovePluginButton( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnDisablePluginCheckbox( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnURLEvent( wxTextUrlEvent& event ) { event.Skip(); }
		
	
	public:
		wxCheckBox* DisablePluginsCheckBox;
		
		PluginsDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Plugins"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 420,336 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER ); 
		~PluginsDialog();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class MenuContainer
///////////////////////////////////////////////////////////////////////////////
class MenuContainer : public wxFrame 
{
	private:
	
	protected:
		wxMenuBar* m_menubar2;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnTreeViewCollapseAll( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnTreeViewExpandAll( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnTreeViewCollapseChildren( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnTreeViewDeleteNode( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnTreeViewAddChildNode( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnTreeViewNodeWatch( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnTreeViewSaveX3D( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnTreeViewSaveVRML( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnTreeViewSaveTrianglesX3D( wxCommandEvent& event ) { event.Skip(); }
		virtual void onTreeViewLookAt( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnTreeViewSaveNrrd( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnTreeViewSavePng( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnTreeViewSaveSTL( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		wxMenu* RightClickMenu;
		wxMenu* RightClickMenuTexture;
		wxMenu* RightClickMenuGeometry;
		
		MenuContainer( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,300 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		
		~MenuContainer();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class MenuContainer2
///////////////////////////////////////////////////////////////////////////////
class MenuContainer2 : public wxMenuBar 
{
	private:
	
	protected:
	
	public:
		
		MenuContainer2( long style = 0 );
		~MenuContainer2();
	
};

#endif //__H3DVIEWER_H__
