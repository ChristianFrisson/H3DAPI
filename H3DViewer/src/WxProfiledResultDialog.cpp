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

#include "WxProfiledResultDialog.h"
#ifdef HAVE_PROFILER

#include <H3DUtil/Console.h>
#include <wx/clipbrd.h>
#include <H3D/Scene.h>

using namespace std;


WxProfiledResultDialog::WxProfiledResultDialog ( wxWindow *parent,
                 wxWindowID id,
                 const wxString &title,
                 const wxPoint& pos,
                 const wxSize& size,
                 long style
                 ): wxDialog(parent, id, title, pos, size, style)
{
  wxBoxSizer *topsizer = new wxBoxSizer( wxVERTICAL );
  // create text ctrl with minimal size 400x200
  logText = (wxTextCtrl *) NULL;
 /* wxRichTextCtrl* logText = new wxRichTextCtrl ( this, -1, wxT(""),
                             wxDefaultPosition, wxSize(400, 200),
                             wxTE_MULTILINE | wxTE_READONLY|wxTE_RICH2);*/
  logText = new wxTextCtrl( this, -1, wxT(""),
                             wxDefaultPosition, wxSize(400, 200),
                             wxTE_MULTILINE | wxTE_READONLY|wxTE_RICH );

  logText->SetFont(wxSystemSettings::GetFont(wxSYS_ANSI_FIXED_FONT));
  //logText->SetDoubleBuffered(true);
  logText->SetFocus();
  clip_board = new wxClipboard();
  //logText->ShowNativeCaret(false);
  //logText->HideNativeCaret();
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
  
  test = 0;
  
  s =  *H3D::Scene::scenes.begin();
}

WxProfiledResultDialog::~WxProfiledResultDialog() {
  delete clip_board;
}

/*******************Event Table*********************/
BEGIN_EVENT_TABLE(WxProfiledResultDialog, wxDialog)
  EVT_BUTTON (wxID_CLOSE, WxProfiledResultDialog::OnConsoleClose)
  EVT_BUTTON (wxID_CLEAR, WxProfiledResultDialog::OnConsoleClear)
  EVT_BUTTON (wxID_ANY,   WxProfiledResultDialog::OnCopyToClipboard)
  EVT_IDLE (WxProfiledResultDialog::OnIdle)
  
END_EVENT_TABLE()

/*******************Member Functions*********************/
void WxProfiledResultDialog::OnConsoleClose(wxCommandEvent &event) {
  Close(TRUE);
}

void WxProfiledResultDialog::OnCopyToClipboard(wxCommandEvent &event){
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
  }
}

void WxProfiledResultDialog::OnConsoleClear(wxCommandEvent &event) {
  WxProfiledResultDialog::logText->Clear();
}

void WxProfiledResultDialog::OnScrollbar(wxScrollEvent& event){
  H3DUtil::Console(4)<<"scroll bar event happen"<<std::endl;
}

void WxProfiledResultDialog::OnPaint(wxPaintEvent &evetn){
}
void WxProfiledResultDialog::OnIdle(wxIdleEvent &event) {

  std::string output = s->profiledResult->getValueAsString();

  if(!output.empty())
  {
    logText->SetValue(output); 
  }
}
#endif