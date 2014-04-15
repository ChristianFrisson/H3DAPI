//////////////////////////////////////////////////////////////////////////////
//    Copyright 2009-2014, SenseGraphics AB
//
//    This file is part of H3D API.
//
//    H3D API is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    H3D API is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with H3D API; if not, write to the Free Software
//    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//    A commercial license is also available. Please contact us at 
//    www.sensegraphics.com for more information.
//
//
/// \file H3DViewerPluginsDialog.cpp
/// \brief CPP file for H3DViewerPluginsDialog.
///
//
//
//////////////////////////////////////////////////////////////////////////////
#include "H3DViewerPluginsDialog.h"
#include <wx/wx.h>
#include <wx/confbase.h>
#include <memory>

#include <H3DViewerConfig.h>
#include <H3D/LibraryInfo.h>
#include <H3DUtil/DynamicLibrary.h>

inline string toStr( const wxString &s ) {
# if(wxUSE_UNICODE)
  char *b = new char[s.size()+1];
  const wchar_t *wb = s.c_str();
  for( unsigned int i = 0; i < s.size(); ++i ) {
    b[i] = (char)(wb[i]);
  }
  
  b[s.size()] = '\0';
  string sb(b);
  delete[] b;
  return sb;
#else
  return string( s.c_str() );
#endif
}


H3DViewerPluginsDialog::H3DViewerPluginsDialog( wxWindow* parent )
:
PluginsDialog( parent ) {

  // read plugins from configuration
  wxConfigBase *h3dConfig = wxConfigBase::Get();

  // enumeration variables
  wxString str;
  long dummy;

#ifndef ALLOW_ADDING_PLUGINS 
  AddPluginButton->Hide();
  RemovePluginButton->Hide();
#endif 
 
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
                                                              wxFD_OPEN,
                                                              wxDefaultPosition) );
  if (openFileDialog->ShowModal() == wxID_OK) {
    wxString file = openFileDialog->GetPath();

    bool res = addPlugin( file );

    if( !res  ) {
      wxMessageBox( wxT("Invalid library file"), wxT("Error"),
                  wxOK | wxICON_EXCLAMATION);
      return;
    }
  }
}

void H3DViewerPluginsDialog::OnRemovePluginButton( wxCommandEvent& event ) {
  int selected_index = InstalledPluginsList->GetSelection();
  if( selected_index == wxNOT_FOUND ) return;

  wxString selected_plugin = InstalledPluginsList->GetStringSelection();
  
  removePlugin( selected_plugin );
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


bool H3DViewerPluginsDialog::addPlugin( const wxString &path, bool force_overwrite ) {
  H3DUtil::DynamicLibrary::LIBHANDLE lib = 
    H3DUtil::DynamicLibrary::load( toStr( path ) );

  // Could not load library. 
  if( lib == NULL  ) {
    return false;
  }

  H3D::LibraryInfo (*info_func)(void) = 
    (H3D::LibraryInfo (*)(void)) H3DUtil::DynamicLibrary::getSymbolAddress( lib, "getLibraryInfo" );

  wxString name(wxT("") );
  wxString version(wxT(""));
  wxString developer(wxT(""));
  wxString developer_web(wxT(""));
  wxString web(wxT(""));
  wxString info(wxT(""));

  if( info_func ) {
    H3D::LibraryInfo lib_info;
    lib_info = info_func();
    name = wxString( lib_info.name, wxConvUTF8 );
    version = wxString( lib_info.version, wxConvUTF8 );
    developer = wxString( lib_info.developer, wxConvUTF8 ); 
    developer_web = wxString( lib_info.developer_web, wxConvUTF8 ); 
    web = wxString( lib_info.web, wxConvUTF8 ); 
    info = wxString( lib_info.info, wxConvUTF8 ); 
  } else {
    size_t pos = path.find_last_of( wxT("/\\") );
    if( pos != string::npos ) {
      name = path.substr( pos + 1, path.size() - (pos + 1) );
    } else { 
      name = path;
    }
  }



  wxConfigBase *h3dConfig = wxConfigBase::Get();
  
  h3dConfig->SetPath( wxT("/Plugins" ) );
  if( !h3dConfig->HasGroup( name ) ){
    InstalledPluginsList->Insert( name, 0 );
  } else {
    if( !force_overwrite ) {
      int answer = wxMessageBox( wxT("Plugin named \"") + name + wxT("\" already installed. Replace current plugin?" ), 
                                 wxT("Warning"),
                                 wxYES_NO| wxICON_EXCLAMATION);
      if( answer == wxNO ) return false;
    }
  }

  h3dConfig->SetPath( wxT("/Plugins/") + name );
  //  h3dConfig->Write( wxT(""), name );
  h3dConfig->Write( wxT("Plugin"), name );
  h3dConfig->Write( wxT("Version"), version );
  h3dConfig->Write( wxT("Developer"), developer );
  h3dConfig->Write( wxT("DeveloperWeb"), developer_web );
  h3dConfig->Write( wxT("Web"), web );
  h3dConfig->Write( wxT("Info"), info );
  h3dConfig->Write( wxT("Library"), path );
  h3dConfig->Flush();
  return true;
}

bool H3DViewerPluginsDialog::removePlugin( const wxString &plugin_name ) {
  wxConfigBase *h3dConfig = wxConfigBase::Get();
  h3dConfig->SetPath( wxT("/Plugins") );
  
  int pos = InstalledPluginsList->FindString( plugin_name );
  
  if( pos != wxNOT_FOUND ) {
    h3dConfig->DeleteGroup( plugin_name );
    InstalledPluginsList->Delete( pos );
    return true;
  } else {
    return false;
  }
}
