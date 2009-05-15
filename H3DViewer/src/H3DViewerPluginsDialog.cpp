#include "H3DViewerPluginsDialog.h"
#include <wx/wx.h>
#include <wx/confbase.h>
#include <memory>

#include <H3D/LibraryInfo.h>
#include <H3DUtil/DynamicLibrary.h>

H3DViewerPluginsDialog::H3DViewerPluginsDialog( wxWindow* parent )
:
PluginsDialog( parent ) {

  // read plugins from configuration
  wxConfigBase *h3dConfig = wxConfigBase::Get();

  // enumeration variables
  wxString str;
  long dummy;

  h3dConfig->SetPath( wxT("/Plugins") );

  bool checked = false;
  h3dConfig->Read( wxT("Enabled"), &checked );
  DisablePluginsCheckBox->SetValue( checked );

  // iterate through all plugins
  bool  bCont = h3dConfig->GetFirstGroup(str, dummy);
  while ( bCont ) {
    InstalledPluginsList->Insert( str, 0 );
    bCont = h3dConfig->GetNextGroup(str, dummy);
  }
}

void H3DViewerPluginsDialog::OnInstalledPluginSelected( wxCommandEvent& event ) {
  wxRichTextAttr urlStyle;
  urlStyle.SetTextColour(*wxBLUE);
  urlStyle.SetFontUnderlined(true);

  wxString selected_plugin = InstalledPluginsList->GetStringSelection();
  
  wxConfigBase *h3dConfig = wxConfigBase::Get();
  h3dConfig->SetPath( wxT("/Plugins/") + selected_plugin );

  wxString str;
  bool success;
  
  PluginInfoText->Clear();
  
  // plugin name
  PluginInfoText->BeginBold();
  PluginInfoText->WriteText( wxT( "Plugin: " ) );
	PluginInfoText->EndBold();
  wxString web;
  bool s = h3dConfig->Read( wxT( "Web" ), &web );
  if( s && !web.IsEmpty() ) {
    PluginInfoText->BeginStyle( urlStyle );
    PluginInfoText->BeginURL( web );
  }
  PluginInfoText->WriteText( selected_plugin );
  if( s && !web.IsEmpty()) {
    PluginInfoText->EndURL();
    PluginInfoText->EndStyle();
  }
  PluginInfoText->Newline();

  // version
  success = h3dConfig->Read( wxT( "Version" ), &str );
  if( success && !str.IsEmpty() ) {
    PluginInfoText->BeginBold();
    PluginInfoText->WriteText( wxT( "Version: " ) );
	  PluginInfoText->EndBold();
    PluginInfoText->WriteText( str );
    PluginInfoText->Newline();
  }

  // developer
  success = h3dConfig->Read( wxT( "Developer" ), &str );
  if( success && !str.IsEmpty() ) {
    PluginInfoText->BeginBold();
    PluginInfoText->WriteText( wxT( "Developer: " ) );
	  PluginInfoText->EndBold();
    wxString web;
    bool s = h3dConfig->Read( wxT( "DeveloperWeb" ), &web );
    if( s && !web.IsEmpty() ) {
      PluginInfoText->BeginStyle( urlStyle );
      PluginInfoText->BeginURL( web );
    }
    PluginInfoText->WriteText( str );
    if( s && !web.IsEmpty() ) {
      PluginInfoText->EndURL();
      PluginInfoText->EndStyle();
    }
    PluginInfoText->Newline();
  }
    
  // info 
  success = h3dConfig->Read( wxT( "Info" ), &str );
  if( success && !str.IsEmpty() ) {
    PluginInfoText->BeginBold();
    PluginInfoText->WriteText( wxT( "Info: " ) );
  	PluginInfoText->EndBold();
    PluginInfoText->WriteText( str );
    PluginInfoText->Newline();
  }

  // library 
  success = h3dConfig->Read( wxT( "Library" ), &str );
  if( success && !str.IsEmpty() ) {
    PluginInfoText->BeginBold();
    PluginInfoText->WriteText( wxT( "Library: " ) );
	  PluginInfoText->EndBold();
    PluginInfoText->WriteText( str );
    PluginInfoText->Newline();
  }
  
  PluginInfoText->Show();
  Layout();
}

void H3DViewerPluginsDialog::OnAddPluginButton( wxCommandEvent& event ) {
  std::auto_ptr< wxFileDialog > openFileDialog( new wxFileDialog ( this,
                                                              wxT("Open file"),
                                                              wxT(""),
                                                              wxT(""),
#ifdef H3D_WINDOWS
                                                              wxT("*.dll"),
#else
#ifdef H3D_OSX
                                                              wxT("*.dylib"),
#else
                                                              wxT("*.so"),
#endif 
#endif
                                                              wxOPEN,
                                                              wxDefaultPosition) );
  if (openFileDialog->ShowModal() == wxID_OK) {
    wxString file = openFileDialog->GetPath();

    wxString name = openFileDialog->GetFilename();
    wxString version(wxT(""));
    wxString developer(wxT(""));
    wxString developer_web(wxT(""));
    wxString web(wxT(""));
    wxString info(wxT(""));

    H3DUtil::DynamicLibrary::LIBHANDLE lib = 
      H3DUtil::DynamicLibrary::load( string( file.mb_str() ) );

    if( lib == NULL  ) {
      wxMessageBox( wxT("Invalid library file"), wxT("Error"),
                  wxOK | wxICON_EXCLAMATION);
      return;
    }

    H3D::LibraryInfo (*info_func)(void) = (H3D::LibraryInfo (*)(void)) H3DUtil::DynamicLibrary::getSymbolAddress( lib, "getLibraryInfo" );

    if( info_func ) {
      H3D::LibraryInfo lib_info;
      lib_info = info_func();
      name = wxString( lib_info.name, wxConvUTF8 );
      version = wxString( lib_info.version, wxConvUTF8 );
      developer = wxString( lib_info.developer, wxConvUTF8 ); 
      developer_web = wxString( lib_info.developer_web, wxConvUTF8 ); 
      web = wxString( lib_info.web, wxConvUTF8 ); 
      info = wxString( lib_info.info, wxConvUTF8 ); 
    }

    wxConfigBase *h3dConfig = wxConfigBase::Get();

    h3dConfig->SetPath( wxT("/Plugins" ) );
    if( !h3dConfig->HasGroup( name ) ){
      InstalledPluginsList->Insert( name, 0 );
    } else {
      int answer = wxMessageBox( wxT("Plugin named \"") + name + wxT("\" already installed. Replace current plugin?" ), 
                                 wxT("Warning"),
                                 wxYES_NO| wxICON_EXCLAMATION);
      if( answer == wxNO ) return;
    }

    h3dConfig->SetPath( wxT("/Plugins/") + name );
    h3dConfig->Write( wxT(""), name );
    h3dConfig->Write( wxT("Plugin"), name );
    h3dConfig->Write( wxT("Version"), version );
    h3dConfig->Write( wxT("Developer"), developer );
    h3dConfig->Write( wxT("DeveloperWeb"), developer_web );
    h3dConfig->Write( wxT("Web"), web );
    h3dConfig->Write( wxT("Info"), info );
    h3dConfig->Write( wxT("Library"), file );
    h3dConfig->Flush();


  }  
}

void H3DViewerPluginsDialog::OnRemovePluginButton( wxCommandEvent& event ) {
  int selected_index = InstalledPluginsList->GetSelection();
  if( selected_index == wxNOT_FOUND ) return;

	wxString selected_plugin = InstalledPluginsList->GetStringSelection();
  
  wxConfigBase *h3dConfig = wxConfigBase::Get();
  h3dConfig->SetPath( wxT("/Plugins") );
  
  h3dConfig->DeleteGroup( selected_plugin );
  InstalledPluginsList->Delete( selected_index );
}

void H3DViewerPluginsDialog::OnDisablePluginCheckbox( wxCommandEvent& event ) {
  wxConfigBase *h3dConfig = wxConfigBase::Get();
  h3dConfig->SetPath( wxT("/Plugins") );
  bool checked = DisablePluginsCheckBox->IsChecked();
  h3dConfig->Write( wxT("Enabled"), checked );
}

void H3DViewerPluginsDialog::OnURLEvent( wxTextUrlEvent& event ) {
  wxLaunchDefaultBrowser( event.GetString() );
}
