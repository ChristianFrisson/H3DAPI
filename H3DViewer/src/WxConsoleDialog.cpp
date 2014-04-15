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

using namespace std;

std::streamsize WxConsoleDialog::ConsoleStreamBuf::xsputn ( const char * s, 
                                                          std::streamsize n ) {
  // output to wxTextCtrl directly if in main wx thread, otherwise
  // save to temporary wxString.
  if( wxIsMainThread() ) {
    text_ctrl->AppendText( wxString( s, wxConvUTF8) );
  } else {
    text_lock.lock();
    other_threads_text.Append( wxString( s, wxConvUTF8 ) );
    text_lock.unlock();
  }
  
  return n;
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
  
  // create text ctrl with minimal size 400x200
  logText = (wxTextCtrl *) NULL;
  logText = new wxTextCtrl ( this, -1, wxT(""),
                             wxDefaultPosition, wxSize(400, 200),
                             wxTE_MULTILINE | wxTE_READONLY );

  clip_board = new wxClipboard();

  topsizer->Add(logText, 
                1,            // make vertically stretchable
                wxEXPAND |    // make horizontally stretchable
                wxALL,        //   and make border all around
                10 );         // set border width to 10 */

  // Clear button
  wxButton *clearBtn = new wxButton(this, wxID_CLEAR, wxT("Cle&ar"));

  // Close button
  wxButton *closeButton = new wxButton( this, wxID_CLOSE, wxT("&Close") );

  // Copy button
  wxButton *copyButton = new wxButton(this, wxID_ANY, wxT("&Copy"));

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
                wxALIGN_CENTER ); // no border and centre horizontally
  
  SetSizer( topsizer );      // use the sizer for layout
  
  topsizer->SetSizeHints( this );   // set size hints to honour minimum size
  
  // redirect the console to logText wxTextCtrl.
  console_stream_buf = new ConsoleStreamBuf( logText );
  console_stream.reset( new ostream( console_stream_buf ) );
  H3DUtil::Console.setOutputStream( *console_stream );

  // redirect the cout, cerr to logText wxTextCtrl so sofa output can be redirected to wxDialog
  orig_cout_buf = cout.rdbuf(); 
  orig_cerr_buf = cerr.rdbuf();
  cout.rdbuf(console_stream_buf);
  cerr.rdbuf(console_stream_buf);
}

WxConsoleDialog::~WxConsoleDialog() {
  // restore cout and cerr
  cout.rdbuf(orig_cout_buf);
  cerr.rdbuf(orig_cerr_buf);

  // The contained buffer is not deleted, set a new buffer and delete
  // buffer to clear up memory.
  streambuf * tmp_buf = console_stream->rdbuf(NULL);
  
  console_stream.reset( NULL );
  delete tmp_buf;
  delete clip_board;
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
  WxConsoleDialog::logText->Clear();
}

void WxConsoleDialog::OnCopyToClipboard(wxCommandEvent &event){
  bool copied = false;
  if(!logText->IsEmpty()){
    if(clip_board->Open()){
      clip_board->Clear();
      wxTextDataObject* tmp=new wxTextDataObject();
      tmp->SetText(logText->GetValue());
      clip_board->SetData(tmp);
      clip_board->Flush();
      copied = true;
      clip_board->Close();
    }
    /*if(!copied){
      ::wxMessageBox("The clipboard copy failed", "Error", wxICON_ERROR | wxCENTRE);
    }*/
  }
}

void WxConsoleDialog::OnIdle(wxIdleEvent &event) {
  wxString output;

  // transfer text output to console from other threads than
  // main thread to the output variable and reset the console.
  console_stream_buf->text_lock.lock();
  if( !console_stream_buf->other_threads_text.IsEmpty() ) {
    output.Alloc( 1000 );
    output.swap( console_stream_buf->other_threads_text );
  }
  console_stream_buf->text_lock.unlock();

  // Send available text to wxTextCtrl.
  if( !output.IsEmpty() ) {
    logText->AppendText( output );
  }
}
