//////////////////////////////////////////////////////////////////////////////
//    Copyright 2006-2014, SenseGraphics AB
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
/// \file WxConsoleDialog.cpp
/// \brief CPP file for WxConsoleDialog.
///
//
//
//////////////////////////////////////////////////////////////////////////////

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------

#include "WxConsoleDialog.h"
#include <H3DUtil/Console.h>

#ifdef max
#undef max
#endif

using namespace std;

int WxConsoleDialog::ConsoleStreamBuf::overflow( int c ) {
  if ( traits_type::eq_int_type(c,traits_type::eof()) ) {
    return traits_type::not_eof(c);
  }

  char ch= traits_type::to_char_type ( c );
  buffer+= ch;

  // Only bother the text control when we have a complete line to write
  // or if the buffer size is reached
  if ( ch == '\n' || buffer.size() >= buffer_size ) { 

    if( wxIsMainThread() ) {
      writeLine( wxString( buffer.c_str(), wxConvUTF8 ) );
      text_lock.lock();
      mainthread_text_copy.Append( wxString( buffer.c_str(), wxConvUTF8) );
      text_lock.unlock();
    } else {
      text_lock.lock();
      other_threads_text.Append( wxString( buffer.c_str(), wxConvUTF8 ) );
      text_lock.unlock();
    }

    buffer.clear();
  }

  return c;
}

void WxConsoleDialog::ConsoleStreamBuf::onIdle() {
  wxString output, output2;

  // transfer text output to console from other threads than
  // main thread to the output variable and reset the console.
  text_lock.lock();
  if( !other_threads_text.IsEmpty() ) {
    output.Alloc( 1000 );
    output.swap( other_threads_text );
  }
  text_lock.unlock();

  // Send available text to wxTextCtrl.
  if( !output.IsEmpty() ) {
    writeLine( output );
    if ( owner->filelog_enabled)
      owner->filelog_cout << output;
    owner->filelog_cout.flush();
  }

  if ( owner->filelog_enabled) {
    text_lock.lock();
    if( !mainthread_text_copy.IsEmpty() ) {
      output2.Alloc( 1000 );
      output2.swap( mainthread_text_copy );
      owner->filelog_console << output2;
      owner->filelog_console.flush();
    }
    text_lock.unlock();
  }
}

void WxConsoleDialog::ConsoleStreamBuf::writeLine( const wxString& _line ) {
  // Count new lines
  std::string s ( _line.mb_str() );
  line_count += std::max( 1, std::count( s.begin(), s.end(), '\n' ) );
  text_ctrl->SetDefaultStyle( text_style );
  text_ctrl->AppendText( _line );
  if( text_ctrl_aux ) {
    text_ctrl_aux->SetDefaultStyle( text_style );
    text_ctrl_aux->AppendText( _line );
  }
  owner->updateNotebook();
}

WxConsoleDialog::WxConsoleDialog ( wxWindow *parent,
  wxWindowID id,
  const wxString &title,
  const wxPoint& pos,
  const wxSize& size,
  long style
  ): wxDialog (parent, id, title, pos, size, style)
{
  wxBoxSizer *topsizer = new wxBoxSizer( wxVERTICAL );

  tabs = new wxNotebook( this, wxID_ANY );

  // create text ctrl with minimal size 400x200
  logText = new wxTextCtrl ( tabs, -1, wxT(""),
    wxDefaultPosition, wxSize( 750, 450 ),
    wxTE_MULTILINE | wxTE_READONLY | wxTE_RICH );

  logText->SetFont( wxFont( 10, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL ) );
  logText->SetBackgroundColour( wxColour( 50,50,50 ) );
  tabs->AddPage( logText, wxT( "All" ) );

  logTextErrors = new wxTextCtrl( tabs, -1, wxT( "" ),
    wxDefaultPosition, wxSize( 750, 450 ),
    wxTE_MULTILINE | wxTE_READONLY | wxTE_RICH );

  logTextErrors->SetFont( wxFont( 10, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL ) );
  logTextErrors->SetBackgroundColour( wxColour( 50, 50, 50 ) );

  tabs->AddPage( logTextErrors, wxT( "Errors" ) );

  logTextWarnings = new wxTextCtrl( tabs, -1, wxT( "" ),
    wxDefaultPosition, wxSize( 750, 450 ),
    wxTE_MULTILINE | wxTE_READONLY | wxTE_RICH );

  logTextWarnings->SetFont( wxFont( 10, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL ) );
  logTextWarnings->SetBackgroundColour( wxColour( 50, 50, 50 ) );

  tabs->AddPage( logTextWarnings, wxT( "Warnings" ) );

  topsizer->Add( tabs,
      1,            // make vertically stretchable
      wxEXPAND |    // make horizontally stretchable
      wxALL,        //   and make border all around
      10 );         // set border width to 10 */

  clip_board = new wxClipboard();

  // Clear button
  wxButton *clearBtn = new wxButton(this, wxID_CLEAR, wxT("Cle&ar"));

  // Close button
  wxButton *closeButton = new wxButton( this, wxID_CLOSE, wxT("&Close") );

  // Copy button
  wxButton *copyButton = new wxButton(this, wxID_ANY, wxT("Co&py"));

  // boxsizer for the buttons
  wxBoxSizer *button_sizer = new wxBoxSizer( wxHORIZONTAL );
  button_sizer->Add(clearBtn,
    0,          // make horizontally unstretchable
    wxALL,      // make border all around (implicit top alignment)
    10 );       // set border width to 10

  button_sizer->Add(closeButton, 
    0,           // make horizontally unstretchable
    wxALL,       // make border all around (implicit top alignment)
    10 );        // set border width to 10

  button_sizer->Add(copyButton,
    0,
    wxALL,
    10);

  topsizer->Add(button_sizer,
    0,                // make vertically unstretchable
    wxALIGN_CENTER ); // no border and center horizontally

  SetSizer( topsizer );      // use the sizer for layout

  topsizer->SetSizeHints( this );   // set size hints to honor minimum size

  // redirect the console to logText wxTextCtrl.

  // Default
  console_stream_buf = new ConsoleStreamBuf( *this, logText, NULL, wxTextAttr( *wxGREEN ) );
  console_stream.reset( new ostream( console_stream_buf ) );
  H3DUtil::Console.setOutputStream( *console_stream );

  // Warnings
#if wxMAJOR_VERSION >= 3
  wxColour yellow = *wxYELLOW;
#else
  wxColour yellow( 255, 255, 0 );
#endif
  console_stream_buf_w = new ConsoleStreamBuf( *this, logText, logTextWarnings, wxTextAttr ( yellow ) );
  console_stream_w.reset( new ostream( console_stream_buf_w ) );
  H3DUtil::Console.setOutputStream( *console_stream_w, H3DUtil::LogLevel::Warning );

  // Errors
  console_stream_buf_e = new ConsoleStreamBuf( *this, logText, logTextErrors, wxTextAttr( *wxRED ) );
  console_stream_e.reset( new ostream( console_stream_buf_e ) );
  H3DUtil::Console.setOutputStream( *console_stream_e, H3DUtil::LogLevel::Error );

  H3DUtil::Console.setShowLevel( true );

  // redirect the cout, cerr to logText wxTextCtrl so sofa output can be redirected to wxDialog
  orig_cout_buf = cout.rdbuf(); 
  orig_cerr_buf = cerr.rdbuf();
  cout.rdbuf(console_stream_buf);
  cerr.rdbuf(console_stream_buf_e);

  if( char *buffer = getenv("H3D_CONSOLE_LOGFILE") ) {
    if (strcmp( buffer, "TRUE" ) == 0 ){
      filelog_enabled = true; }
    else if (strcmp( buffer, "FALSE" ) == 0 ){
      filelog_enabled = false; }
  }

  if (filelog_enabled) {
    time_t t = time(0);   // get time now
    struct tm * now = localtime( & t );
    stringstream timestamp;

    timestamp << (now->tm_year + 1900)
      << (now->tm_mon + 1)
      << std::setw( 2 ) << std::setfill( '0' )
      <<  now->tm_mday << '_'
      << std::setw( 2 ) << std::setfill( '0' )
      <<  now->tm_hour
      << std::setw( 2 ) << std::setfill( '0' )
      <<  now->tm_min
      << std::setw( 2 ) << std::setfill( '0' )
      <<  now->tm_sec;

    string subdirectory;
    #ifdef WIN32
      //TCHAR currentworkingdir[256];
      //GetCurrentDirectory(256, currentworkingdir);
      //wxString plugin_dir = executable_dir + wxT("\\..\\plugins");
      //wxDirExists(plugin_dir)
      subdirectory = "log\\";
    #else
      //subdirectory = "log/";
      subdirectory = "";
    #endif

    string session_uuid;
    ifstream fh;
    fh.open("session_uuid", std::ofstream::in);
    if(fh.fail()){
      session_uuid = "";
    }
    else {
      fh >> session_uuid;
      session_uuid = "_" + session_uuid;
    }

    filelog_cout.open((subdirectory+timestamp.str() + session_uuid + "_console_cout_and_cerr_log.txt" ).c_str(), std::ofstream::out);
    filelog_console.open((subdirectory+timestamp.str() + session_uuid + "_console_main_log.txt").c_str(), std::ofstream::out);
  }
}

WxConsoleDialog::~WxConsoleDialog() {
  if (filelog_enabled) {
    filelog_cout.close();
    filelog_console.close();
  }

  // restore cout and cerr
  cout.rdbuf(orig_cout_buf);
  cerr.rdbuf(orig_cerr_buf);

  delete clip_board;

  // Clean up "default(????)" console buffer.
  streambuf * tmp_buf = console_stream->rdbuf(NULL);
  console_stream.reset( NULL );
  delete tmp_buf; tmp_buf = 0;

  // Clean up warning buffer
  streambuf * tmp_buf_w = console_stream_w->rdbuf(NULL);
  console_stream_w.reset( NULL );
  delete tmp_buf_w; tmp_buf_w = 0;

  // Clean up error buffer
  streambuf * tmp_buf_e = console_stream_e->rdbuf(NULL);
  console_stream_e.reset( NULL );
  delete tmp_buf_e; tmp_buf_e = 0;
}

/*******************Event Table*********************/
BEGIN_EVENT_TABLE(WxConsoleDialog, wxDialog)
  EVT_BUTTON (wxID_CLOSE, WxConsoleDialog::OnConsoleClose)
  EVT_BUTTON (wxID_CLEAR, WxConsoleDialog::OnConsoleClear)
  EVT_BUTTON (wxID_ANY,   WxConsoleDialog::OnCopyToClipboard)
  EVT_IDLE (WxConsoleDialog::OnIdle)
  END_EVENT_TABLE()

  /*******************Member Functions*********************/
  void WxConsoleDialog::OnConsoleClose(wxCommandEvent &event) {
    Close(TRUE);
}

void WxConsoleDialog::OnConsoleClear(wxCommandEvent &event) {
  switch( tabs->GetSelection() ) {
  case 0:
    logText->Clear();
    break;
  case 1:
    logTextErrors->Clear();
    console_stream_buf_e->line_count = 0;
    break;
  case 2:
    logTextWarnings->Clear();
    console_stream_buf_w->line_count = 0;
    break;
  }


  updateNotebook();
}

void WxConsoleDialog::OnCopyToClipboard(wxCommandEvent &event){
  wxTextCtrl* ctrl= logText;
  switch( tabs->GetSelection() ) {
  case 0:
    ctrl = logText;
    break;
  case 1:
    ctrl = logTextErrors;
    break;
  case 2:
    ctrl = logTextWarnings;
    break;
  }

  if(!ctrl->IsEmpty()){
    if(clip_board->Open()){
      clip_board->Clear();
      wxTextDataObject* tmp=new wxTextDataObject();
      tmp->SetText( ctrl->GetValue());
      clip_board->SetData(tmp);
      clip_board->Flush();
      clip_board->Close();
    }
  }
}

void WxConsoleDialog::OnIdle(wxIdleEvent &event) {
  console_stream_buf->onIdle();
  console_stream_buf_w->onIdle();
  console_stream_buf_e->onIdle();
}

void WxConsoleDialog::updateNotebook() {
  tabs->SetPageText( 1, getTabTitle ( wxT("Errors"), console_stream_buf_e->line_count ) );
  tabs->SetPageText( 2, getTabTitle( wxT("Warnings"), console_stream_buf_w->line_count ) );
}

wxString WxConsoleDialog::getTabTitle( const wxString& _title, std::size_t _count ) {
  // Avoid reporting very large numbers of messages
  if( _count < 1000 ) {
    if( _count > 0 ) {
      std::stringstream ss;
      ss << _title << " (" << _count << ")";
      return wxString ( ss.str().c_str(), wxConvUTF8 );
    } else {
      return _title;
    }
  } else {
    return _title + wxT(" (!)");
  }
}