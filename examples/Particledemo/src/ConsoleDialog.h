//////////////////////////////////////////////////////////////////////////////
//    Copyright 2006-2013, SenseGraphics AB
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
/// \file ConsoleDialog.h
/// \brief Header file for ConsoleDialog.
///
//
//
//////////////////////////////////////////////////////////////////////////////

#ifndef __CONSOLEDIALOG__
#define __CONSOLEDIALOG__

// ---------------------------------------------------------------------------
//  Console Dialog Class
// ---------------------------------------------------------------------------
#include <wx/wx.h>
#include <sstream>
#include <memory>
class ConsoleDialog: public wxDialog
{
public:
  /// Constructor
  ConsoleDialog ( wxWindow *parent,
          wxWindowID id,
          const wxString &title,
          const wxPoint& pos = wxDefaultPosition,
          const wxSize& size = wxDefaultSize,
          long style = wxDEFAULT_DIALOG_STYLE
          );

  /// Destructor.
  ~ConsoleDialog();

  /// wx interface.
  wxTextCtrl *logText;

  /// Used to make sure that output is done in the main wx thread.
  friend void wxLockGUI( void * );

  /// Reset changes in wxLockGUI.
  friend void wxUnlockGUI( void * );

protected:
  // Stream used for temporary input in other thread.
  std::stringstream other_thread_output;
  // The console stream. The contents of other_thread_output is eventuelly
  // transferred to this stream.
  std::auto_ptr< std::ostream >console_stream;
  // Called when console is closed.
  void OnConsoleClose (wxCommandEvent & event);
  // Called when clear button is pressed.
  void OnConsoleClear (wxCommandEvent & event);
  // Called on idle.
  void OnIdle( wxIdleEvent &event );
  // Declare event table.
  DECLARE_EVENT_TABLE();
 
};

#endif
