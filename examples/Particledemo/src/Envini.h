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
/// \file Envini.h
/// \brief Header file for environment variables for ParticleDemo.
///
//
//
//////////////////////////////////////////////////////////////////////////////

// H3D includes
#include <H3D/INIFile.h>

// std includes
#include <fstream>

using namespace std;
using namespace H3D;

#define GET4(ENV,GROUP,VAR,DEFAULT) \
( getenv(ENV) ?                     \
  string(getenv(ENV)) :             \
  ( ini_file.hasOption(GROUP,VAR) ? \
    ini_file.get( GROUP, VAR ) :    \
    DEFAULT ) )

#define GET_ENV_INI_DEFAULT(ENV,PATH,GROUP,VAR,DEFAULT) \
( getenv(ENV) ?                                         \
  string(getenv(ENV)) :                                 \
  ( ini_file.hasOption(GROUP,VAR) ?                     \
    PATH + ini_file.get( GROUP, VAR ) :                 \
    DEFAULT ) )

string GET_ENV_INI_DEFAULT_FILE( INIFile &ini_file,
                            const string &ENV,
                            const string &DISPLAY_PATH,
                            const string &COMMON_PATH,
                            const string &GROUP,
                            const string &VAR ) {
  char *env = getenv(ENV.c_str());
  if( env ) return env;
  
  if( ini_file.hasOption(GROUP,VAR) ) { 
    string option = ini_file.get( GROUP, VAR );
    
    ifstream inp( (DISPLAY_PATH + option).c_str() );
    inp.close();
    if(!inp.fail()) return DISPLAY_PATH + option;

    inp.clear();
    inp.open( (COMMON_PATH + option).c_str() );
    inp.close();
    if(!inp.fail()) return COMMON_PATH + option;
  }
  return "";
}

#define GET_INT(GROUP,VAR,DEFAULT)  \
( ini_file.hasOption(GROUP,VAR) ? \
  atoi(ini_file.get( GROUP, VAR ).c_str()) :    \
  DEFAULT )

#define GET_BOOL(GROUP,VAR,DEFAULT)   \
( ini_file.hasOption(GROUP,VAR) ?     \
  ini_file.getBoolean( GROUP, VAR ) : \
  DEFAULT )


//  AutoRef needs to be declared global
/*AutoRef< Scene > scene( new Scene );
AutoRef< KeySensor > ks( new KeySensor );
AutoRef< MouseSensor > ms( new MouseSensor );
#ifndef MACOSX
  AutoRef< SpaceWareSensor > ss;
#endif
AutoRef< Transform > t( new Transform );
AutoRef< Node > device_info;
AutoRef< Node > viewpoint;
AutoRef< Group > g( new Group );*/
