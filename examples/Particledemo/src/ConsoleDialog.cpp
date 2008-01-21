//////////////////////////////////////////////////////////////////////////////
//    Copyright 2007, SenseGraphics AB
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
/// \brief Implementation file for ConsoleDialog
///
//
//////////////////////////////////////////////////////////////////////////////

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------

#include <consoleDialog.h>

// H3DUtil includes
#include <H3DUtil/Console.h>

using namespace std;


// ---------------------------------------------------------------------------
//  Implementation
// ---------------------------------------------------------------------------

/*******************Constructor*********************/

consoleDialog::consoleDialog ( wxWindow *parent,
							   wxWindowID id,
							   const wxString &title,
							   const wxPoint& pos,
							   const wxSize& size,
							   long style
							   ):
wxDialog (parent, id, title, pos, size, style)
{
	wxBoxSizer *topsizer = new wxBoxSizer( wxVERTICAL );

	// create text ctrl with minimal size 400x200
	logText = (wxTextCtrl *) NULL;
	logText = new wxTextCtrl ( this, -1, "", wxDefaultPosition, wxSize(400, 200),
                             wxTE_MULTILINE | wxTE_READONLY );

	topsizer->Add(logText, 
				  1,            // make vertically stretchable
				  wxEXPAND |    // make horizontally stretchable
				  wxALL,        //   and make border all around
				  10 );         // set border width to 10 */

    wxButton *closeButton = new wxButton( this, wxID_CLOSE, "Close" );

    wxBoxSizer *button_sizer = new wxBoxSizer( wxHORIZONTAL );
    button_sizer->Add(closeButton, 
	  				  0,           // make horizontally unstretchable
					  wxALL,       // make border all around (implicit top alignment)
					  10 );        // set border width to 10

	topsizer->Add(button_sizer,
				  0,                // make vertically unstretchable
				  wxALIGN_CENTER ); // no border and centre horizontally

	SetSizer( topsizer );      // use the sizer for layout

	topsizer->SetSizeHints( this );   // set size hints to honour minimum size

#ifndef MACOSX
	//TODO: console does not work on Mac
  std::streambuf *sbOld = std::cerr.rdbuf();
	std::cerr.rdbuf(logText);
	ostream *t = new ostream(logText);
  H3DUtil::Console.setOutputStream( *t );
	
  std::cerr.rdbuf(sbOld); 

#endif
}

/*******************Event Table*********************/
BEGIN_EVENT_TABLE(consoleDialog, wxDialog)
	EVT_BUTTON (wxID_CLOSE, consoleDialog::OnConsoleClose)
END_EVENT_TABLE()

/*******************Member Functions*********************/
void consoleDialog::OnConsoleClose(wxCommandEvent &event)
{
	Close(TRUE);
}
