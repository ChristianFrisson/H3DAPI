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
/// \file WxConsoleDialog.h
/// \brief Header file for WxConsoleDialog.
///
//
//
//////////////////////////////////////////////////////////////////////////////

#ifndef __WXCONSOLEDIALOG__
#define __WXCONSOLEDIALOG__

// ---------------------------------------------------------------------------
//  Console Dialog Class
// ---------------------------------------------------------------------------
#include <wx/wx.h>
#include <wx/clipbrd.h>
#include <sstream>
#include <memory>
#include <H3DUtil/Threads.h>

/// This class provides a wxDialog with the output from H3DUtil
/// Console messages.
class WxConsoleDialog: public wxDialog
{
public:
  /// Constructor
  WxConsoleDialog ( wxWindow *parent,
          wxWindowID id,
          const wxString &title,
          const wxPoint& pos = wxDefaultPosition,
          const wxSize& size = wxDefaultSize,
          long style = wxDEFAULT_DIALOG_STYLE
          );

  /// Destructor.
  ~WxConsoleDialog();

  /// wx interface.
  wxTextCtrl *logText;

  /// 
  wxClipboard *clip_board;

  /// Used to make sure that output is done in the main wx thread.
  friend void wxLockGUI( void * );

  /// Reset changes in wxLockGUI.
  friend void wxUnlockGUI( void * );

protected:

  /// ConsoleStreamBuf class is a specialization of a streambuf object
  /// in order to let data from a stream be passed into a wxTextCtrl. This
  /// can then be used to create a ostream to use together with H3DUtil::Console
  /// to get all messages from H3D into a wxTextCtrl.
  /// To do this we override the virtual function to pass the data into the 
  /// wxTextCtrl. We can only do wx calls from within the main wx thread though
  /// so we check that this is the case. If the console is used from another 
  /// thread all such data will be put into the other_threads_text member variable.
  /// This can then be put into the wxTextCtrl at a later stage (in the OnIdle function).
  class ConsoleStreamBuf: public std::streambuf {
  public:
    
    /// Constructor.
    ConsoleStreamBuf( wxTextCtrl *text ):
    text_ctrl( text ) {
      // reserve memory for the string to avoid having to reallocate.
      other_threads_text.Alloc(1000);
    }

    /// This function needs to be here in order for xsputn to be called.
    /// Not sure why..
    int overflow( int c ) {
      // return something different from EOF
      return 0;
    }

    /// Transfer the string to wxTextCtrl.
    std::streamsize xsputn ( const char * s, std::streamsize n );

    /// Lock to be used for accessing the other_threads_text member.
    H3DUtil::MutexLock text_lock;

    /// Text added to H3DUtil::Console from other threads than the main
    /// wx thread. Access to this variable have to be done with the text_lock
    /// mutex lock for thread safety.
    wxString other_threads_text;

    /// The wxTextCtrl to output text to.
    wxTextCtrl *text_ctrl;
  };

  // the buffer object for console_stream. Pointer is owned by
  // the console_stream object.
  ConsoleStreamBuf *console_stream_buf;

  // the streambuf of cerr and cout that was used when this WxConsoleDialog was created.
  std::streambuf *orig_cerr_buf, *orig_cout_buf;

  // The console stream. The contents of other_thread_output is eventuelly
  // transferred to this stream.
  std::auto_ptr< std::ostream >console_stream;
  // Called when console is closed.
  void OnConsoleClose (wxCommandEvent & event);
  // Called when clear button is pressed.
  void OnConsoleClear (wxCommandEvent & event);
  // Called when copy button is pressed.
  void OnCopyToClipboard (wxCommandEvent &event);
  // Called on idle.
  void OnIdle( wxIdleEvent &event );
  // Declare event table.
  DECLARE_EVENT_TABLE();
 
};

#endif
