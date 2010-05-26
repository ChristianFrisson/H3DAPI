//////////////////////////////////////////////////////////////////////////////
//    Copyright 2004-2007, SenseGraphics AB
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
/// \file UpdateResourceFile.cpp
/// \brief CPP file for creating a small program used to update resource files
/// on Windows when CMake configuration changes.
///
//
//
//////////////////////////////////////////////////////////////////////////////
#include <windows.h>
#include <string>
#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

int main(int argc, char* argv[]) {

  if (argc < 8){
    cerr << "Usage: " << argv[0] << " <project name> <rc file> <rc.cmake file> "
         << "<major version> <minor version> <build version> <entries file> "
         << "[var_name1] [value1]" << endl
         << "Where:" << endl
         << "project name  - The name of the project" << endl
         << "rc file       - The path to the output rc file" << endl
         << "rc.cmake file - The path to the input rc.cmake file" << endl
         << "major version - Integer used to replace \"project name_MAJOR_VERSION\"" << endl
         << "minor version - Integer used to replace \"project name_MINOR_VERSION\"" << endl
         << "build version - Integer used to replace \"project name_BUILD_VERSION\"" << endl
         << "entries file  - A .svn/entries file from which to get subversion  \"project name_SVN_VERSION\"" << endl
         << "var_name1 - Optional. Name of a variable to find in rc.cmake file." << endl
         << "value1 - Optional. Value used to replace var1 if found. "
         << "The last two variables can be repeated (in pair) as many times as needed." << endl;
    return 1;
  }

  // Variable that will contain pairs of variable names and values.
  vector< pair< string, string > > configure_vars;

  // Push back major, minor and build version.
  string project_name = argv[1];
  configure_vars.push_back(
    make_pair( "${" + project_name + "_MAJOR_VERSION}", argv[4] ) );
  configure_vars.push_back(
    make_pair( "${" + project_name + "_MINOR_VERSION}", argv[5] ) );
  configure_vars.push_back(
    make_pair( "${" + project_name + "_BUILD_VERSION}", argv[6] ) );

  // Parse entries file to find the subversion number.
  // It is assumed that the interesting part of the entries file looks like
  // this:
  // dir
  // xxxx
  // Where xxxx is the revision number.
  string last_line;
  string svn_version = "0";
  // Open entries files, find svn version.
  ifstream entries( argv[7] );
  if( entries.is_open() ) {
    while( !entries.eof() ) {
      string line;
      getline( entries, line );
      if( last_line == "dir"  && !line.empty() ) {
        char last_char = ' ';
        bool check_last_char = false;
        string new_line;
        for( unsigned int i = 0; i < line.size(); i++ ) {
          if( !isspace( line[i] ) ) {
            new_line = new_line + line[i];
            if( check_last_char ) {
              if( isspace( last_char ) ) {
                cerr << "The entries file is not as expected." << endl;
                break;
              }
            } else
              check_last_char = true;
          }
          last_char = line[i];
        }
        // Remove whitespace.
        svn_version = new_line;
        break;
      }
      last_line = line;
    }
    entries.close();
  }
  configure_vars.push_back(
    make_pair( "${" + project_name + "_SVN_VERSION}", svn_version ) );

  // Add extra arguments.
  for( int i = 8; i + 1 < argc; i = i + 2 ) {
    configure_vars.push_back(
      make_pair( "${" + string(argv[i]) + "}", argv[i+1] ) );
  }

  // Open resource file.
  ifstream rc_read( argv[2] );
  if( !rc_read.is_open() ) {
    cerr << "The path to resource file is incorrect. "
         << "If the path is correct but the file is missing configure "
         << "the project through CMake again to restore the file." << endl;
    return 1;
  }

  // Open rc.cmake file.
  ifstream rc_cmake( argv[3] );
  if( !rc_cmake.is_open() ) {
    cerr << "The path to rc.cmake file is incorrect." << endl;
    return 1;
  }

  bool replace_file = false;
  string new_content = "";
  /// Read rc.cmake and replace all occurrances of variables.
  while( !rc_cmake.eof() ) {
    string new_line;
    string old_line = "";
    getline( rc_cmake, new_line );
    if( !rc_read.eof() )
      getline( rc_read, old_line );
    // Only try to find variables in lines that contains ${
    if( new_line.find( "${" ) != string::npos ) {
      for( unsigned int i = 0; i < configure_vars.size(); i++ ) {
        // Find and replace this variable, one variable might exist
        // several times on the same line so simply check a lot of times.
        size_t found_pos = new_line.find( configure_vars[i].first );
        while( found_pos != string::npos ) {
          new_line.replace( found_pos,
                            configure_vars[i].first.size(),
                            configure_vars[i].second );
          found_pos = new_line.find( configure_vars[i].first );
        }
      }
      if( new_line != old_line )
        replace_file = true;
    } else if( new_line != old_line )
      replace_file = true;
    new_content += new_line + "\n";
  }

  // Close files.
  rc_read.close();
  rc_cmake.close();

  if( replace_file ) {
    // Write output.
    ofstream rc( argv[2] );
    if( !rc.is_open() ) {
      cerr << "Could not update rc file. Write access missing." << endl;
      return 1;
    }
    
    rc << new_content;
    rc.close();
  }

  return 0;
}

