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
/// \file H3DPythonConsole.cpp
/// \brief CPP file for H3DPythonConsole.
///
//
//
//////////////////////////////////////////////////////////////////////////////
#include "H3DPythonConsole.h"

#if defined(HAVE_PYTHON) && defined(USE_PYTHON_CONSOLE)

using namespace H3D;

H3DPythonConsole::H3DPythonConsole ( 
  wxWindow* parent, 
  PythonScript& _pythonNode ) :
PythonConsole ( parent ),
command_index ( 0 ) {
  python_node.reset ( &_pythonNode );

  std::string welcome_msg=
    "Python console attached to PythonScript node " + python_node->getName() + "\n";

  // Try to get a custome welcome message from the script node
  welcome_msg+= python_node->execute ( "initConsole() if 'initConsole' in dir() else None" ) + "\n\n";

  m_textCtrlOutput->AppendText ( wxString ( welcome_msg.c_str(), wxConvUTF8 ) );



  m_textCtrlInput->SetFocus ();
}

void H3DPythonConsole::onKeyDown( wxKeyEvent& event ) {

  switch ( event.GetKeyCode () ) {
  case WXK_UP:
    command_index--;
    break;
  case WXK_DOWN:
    command_index++;
    break;
  default:
    event.Skip ();
    return;
  }

  command_index= std::min ( std::max ( 0, command_index ), (int)command_history.size()-1 );
  if ( command_index >= 0 ) {
    m_textCtrlInput->SetValue ( 
      wxString ( command_history[command_index].c_str(), wxConvUTF8 ) );
    m_textCtrlInput->SetInsertionPointEnd ();
  }
}

void H3DPythonConsole::onExecuteCommand ( wxCommandEvent& event ) {  
  std::string command ( m_textCtrlInput->GetValue ().mb_str() );

  m_textCtrlOutput->AppendText ( wxString ( command.c_str(), wxConvUTF8 ) );
  m_textCtrlOutput->AppendText ( wxT("\n") );

  std::string result= python_node->execute ( command );

  if ( !result.empty () ) {
    result= ">> " + result;
  }

  m_textCtrlOutput->AppendText ( wxString ( result.c_str(), wxConvUTF8 ) );
  m_textCtrlInput->Clear();

  command_history.push_back ( command );
  command_index= command_history.size();
}

#endif // HAVE_PYTHON
