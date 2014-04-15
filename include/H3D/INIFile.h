//////////////////////////////////////////////////////////////////////////////
//    Copyright 2004-2014, SenseGraphics AB
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
/// \file INIFile.h Class for parsing an .ini file
//
//////////////////////////////////////////////////////////////////////////////

#ifndef __INIFILE_H__
#define __INIFILE_H__

#include <string>
#include <iostream>
#include <map>
#include <H3D/H3DApi.h>

namespace H3D {

  class H3DAPI_API INIFile {
  public:
    // Constructor.
    INIFile( const std::string &file );

    // Constructor.
    INIFile( std::istream& in );
    
    // Get a value as a string.
    std::string get( std::string group, 
                     std::string name ) const;

    // Get a value as a boolean
    bool getBoolean( std::string group, 
                     std::string name ) const;

    // Check if a value exists.
    bool hasOption( std::string group, 
                    std::string name ) const;

  private:
    void parseStream( std::istream &is );

    void parseLine( std::string line, 
                    std::string group, 
                    unsigned count );

    std::map<std::string, std::map<std::string, std::string> > parsed_;
  };

};

#endif
