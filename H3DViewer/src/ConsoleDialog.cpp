//////////////////////////////////////////////////////////////////////////////
//    Copyright 2006-2007, SenseGraphics AB
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
/// \file ConsoleDialog.cpp
/// \brief CPP file for ConsoleDialog.
///
//
//
//////////////////////////////////////////////////////////////////////////////

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------

#include "ConsoleDialog.h"
#include <H3DUtil/Console.h>

using namespace std;

// ---------------------------------------------------------------------------
//  Implementation
// ---------------------------------------------------------------------------

/*******************Constructor*********************/

class ConsoleStreamBuf: public streambuf {
public:
  ConsoleStreamBuf( wxTextCtrl *text ):
    text_ctrl( text ) {}

  int overflow( int c ) {
    text_ctrl->AppendText((wxChar)c);

    // return something different from EOF
    return 0;
  }

  wxTextCtrl *text_ctrl;
};


void wxLockGUI( void *data ) {
  if( !wxIsMainThread() ) {
    // using the wxMutexGuiEnter function "works", but output is incredibly
    // slow if called from another thread than the main thread. So we 
    // make sure every output is done in the main thread instead by
    // switching the output stream temporarily to a stringstream
    // and then later output the contents of the stringstream
    // to the output stream.
    //wxMutexGuiEnter();
    consoleDialog *dialog =  static_cast< consoleDialog * >( data );
    H3DUtil::Console.setOutputStream( dialog->other_thread_output );
  }
} 

void wxUnlockGUI( void *data ) {
  if( !wxIsMainThread() ) {
    consoleDialog *dialog =  static_cast< consoleDialog * >( data );
    H3DUtil::Console.setOutputStream( *dialog->console_stream );
    //    wxMutexGuiLeave();
  }
}


consoleDialog::consoleDialog ( wxWindow *parent,
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

	topsizer->Add(logText, 
                1,            // make vertically stretchable
                wxEXPAND |    // make horizontally stretchable
                wxALL,        //   and make border all around
                10 );         // set border width to 10 */

  // Clear button
  wxButton *clearBtn = new wxButton(this, wxID_CLEAR);

  // Close button
  wxButton *closeButton = new wxButton( this, wxID_CLOSE, wxT("Close") );

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



  topsizer->Add(button_sizer,
                0,                // make vertically unstretchable
                wxALIGN_CENTER ); // no border and centre horizontally
  
	SetSizer( topsizer );      // use the sizer for layout
  
	topsizer->SetSizeHints( this );   // set size hints to honour minimum size
  
  // redirect the console to logText wxTextCtrl.
  console_stream.reset( new ostream(new ConsoleStreamBuf( logText) ) );
  H3DUtil::Console.setOutputStream( *console_stream );
  H3DUtil::Console.setLockMutexFunction( wxLockGUI, this );
  H3DUtil::Console.setUnlockMutexFunction( wxUnlockGUI, this );
}

/*******************Event Table*********************/
BEGIN_EVENT_TABLE(consoleDialog, wxDialog)
  EVT_BUTTON (wxID_CLOSE, consoleDialog::OnConsoleClose)
  EVT_BUTTON (wxID_CLEAR, consoleDialog::OnConsoleClear)
  EVT_IDLE (consoleDialog::OnIdle)
END_EVENT_TABLE()

/*******************Member Functions*********************/
void consoleDialog::OnConsoleClose(wxCommandEvent &event) {
  Close(TRUE);
}

void consoleDialog::OnConsoleClear(wxCommandEvent &event) {
  consoleDialog::logText->Clear();
}

void consoleDialog::OnIdle(wxIdleEvent &event) {
	if( other_thread_output.str().size() > 0 ) {
    H3DUtil::Console.getOutputStream() << other_thread_output.str();
    H3DUtil::Console.getOutputStream().flush();
    other_thread_output.str( "" );
  }
}
