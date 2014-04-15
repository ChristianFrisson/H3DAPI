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
//
/// \file H3DLoad.cpp
/// \brief CPP file for H3DLoad application. H3DLoad is a X3D/VRML loader
/// using H3DAPI.
//
//
//////////////////////////////////////////////////////////////////////////////

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <fstream>
#include <H3D/X3D.h>
#include <string.h>

#include <iostream>

#include <H3D/VrmlParser.h>
#include <H3D/GLUTWindow.h>
#include <H3D/Group.h>
#include <H3D/Transform.h>
#include <H3D/Scene.h>
#include <H3D/KeySensor.h>
#include <H3D/MouseSensor.h>
#ifndef MACOSX
#include <H3D/SpaceWareSensor.h>
#endif
#include <H3D/DEFNodes.h>
#include <H3D/Viewpoint.h>
#include <H3D/DeviceInfo.h>
#include <H3D/INIFile.h>
#include <H3D/ResourceResolver.h>
#include <H3D/PythonScript.h>
#include <H3D/NavigationInfo.h>
#include <H3D/H3DNavigation.h>
#include <H3D/PeriodicUpdate.h>

using namespace std;
using namespace H3D;

class ChangeViewport : public PeriodicUpdate< SFInt32> { 
  public:
    inline void setOwnerWindow( H3DWindowNode * owner_window ) {
      glwindow = owner_window;
    }
  protected:
  virtual void update() {
    int key = static_cast< SFInt32 * >(routes_in[0])->getValue();
    X3DViewpointNode::ViewpointList vp_list = X3DViewpointNode::getAllViewpoints();
    X3DViewpointNode *active_vp = X3DViewpointNode::getActive();
    //int no_of_viewpoints = vp_list.size();
    typedef X3DViewpointNode::ViewpointList::iterator ListIterator;

    switch( key ) {
      case KeySensor::HOME:  {
        // Goes to the initial viewpoint
        (*vp_list.begin())->set_bind->setValue(true);
        break;
      }
      case KeySensor::END: {
        //Goes to the final viewpoint 
        (vp_list.back())->set_bind->setValue(true);
        break;        
      }
      case KeySensor::PGDN: {
        // Goes to the next viewpoint, if the active viewpoint is the
        // last one the next viewpoint is set to the first viewpoint
        ListIterator it = find(vp_list.begin(),vp_list.end(),active_vp);
        //Checks if the active viewpoint exists
        if(it!=vp_list.end()) {
          ++it;
          //Checks if the active viewpoint is the last one
          if(it==vp_list.end()){
            (*vp_list.begin())->set_bind->setValue(true);
          }
          else{
            (*it)->set_bind->setValue(true);
          }
        }
        break;
      }
      case KeySensor::PGUP: {
        // Goes to the previous viewpoint, if the active viewpoint is the
        // first one the previous viewpoint is set to the last viewpoint
        ListIterator it = find(vp_list.begin(),vp_list.end(),active_vp);
        //Checks if the active viewpoint exists
        if(it!=vp_list.end()){
          //Checks if the ative viewpoint is the first one
          if(it==vp_list.begin()){
            (vp_list.back())->set_bind->setValue(true);
          }
          else{
            it--;
            (*it)->set_bind->setValue(true);
          }
        }        
        break;
      }
      case KeySensor::F1: {
        if( H3DNavigation::isEnabled( H3DNavigation::MOUSE ) )
          H3DNavigation::disableDevice( H3DNavigation::MOUSE );
        else
          H3DNavigation::enableDevice( H3DNavigation::MOUSE );
        break;
      }
      case KeySensor::F2: {
        if( H3DNavigation::isEnabled( H3DNavigation::KEYBOARD ) )
          H3DNavigation::disableDevice( H3DNavigation::KEYBOARD );
        else
          H3DNavigation::enableDevice( H3DNavigation::KEYBOARD );
        break;
      }
      case KeySensor::F3: {
        if( H3DNavigation::isEnabled( H3DNavigation::HAPTICSDEVICE ) )
          H3DNavigation::disableDevice( H3DNavigation::HAPTICSDEVICE );
        else
          H3DNavigation::enableDevice( H3DNavigation::HAPTICSDEVICE );
        break;
      }
      case KeySensor::F4: {
        if( H3DNavigation::isEnabled( H3DNavigation::SWS ) )
          H3DNavigation::disableDevice( H3DNavigation::SWS );
        else
          H3DNavigation::enableDevice( H3DNavigation::SWS );
        break;
      }
      case KeySensor::F5: {
        X3DViewpointNode * vp = X3DViewpointNode::getActive();
        if ( vp ) {
          vp->relOrn->setValue( Rotation() );
          vp->relPos->setValue( Vec3f() );
        }
        break;
      }
      case KeySensor::F11: {
        if( glwindow ) {
          glwindow->fullscreen->setValue( !glwindow->fullscreen->getValue() );
        }
        break;
      }
      default: {}
    }
  }

  H3DWindowNode *glwindow;
};
    
class ChangeNavType : public PeriodicUpdate< SFString > { 
public:
  ChangeNavType() : glwindow( 0 ), speed_increment( 0.1f ) {}

  inline void setOwnerWindow( H3DWindowNode * owner_window ) {
    glwindow = owner_window;
  }

protected:
  virtual void update() {
    NavigationInfo *mynav =0;
    if(NavigationInfo::getActive()){
      mynav = NavigationInfo::getActive();
    }
    string s = static_cast< SFString * >(routes_in[0])->getValue();
    if( s.empty() ) return;

    if( s[0] == 27 ) {
      throw Exception::QuitAPI();
    } else if( s == "w") {
      // Set navigation type to WALK
      if(mynav){
        mynav->setNavType("WALK");
      }
      else{
        glwindow->default_nav = "WALK"; 
      }
    } else if( s == "f" ) {
      // Set navigation type to FLY
      if(mynav){
        mynav->setNavType("FLY");
      }
      else{
        glwindow->default_nav = "FLY"; 
      }
    } else if( s == "e" ) {
      // Set navigation type to EXAMINE
      if(mynav){
        mynav->setNavType("EXAMINE");
      }
      else{
        glwindow->default_nav = "EXAMINE"; 
      }
    } else if( s == "l" ) {
      // Set navigation type to LOOKAT
      if(mynav){
        mynav->setNavType("LOOKAT");
      }
      else{
        glwindow->default_nav = "LOOKAT"; 
      }
    } else if( s == "n" ) {
      // Set navigation type to NONE
      if(mynav){
        mynav->setNavType("NONE");
      }
      else{
        glwindow->default_nav = "NONE"; 
      }
    } else if( s == "+" ) {
      if( mynav ) {
        mynav->speed->setValue( mynav->speed->getValue() + speed_increment );
      } else {
        glwindow->default_speed += speed_increment;
      }
    } else if( s == "-" ) {
      if( mynav ) {
        H3DFloat tmp_speed = mynav->speed->getValue() - speed_increment;
        if( tmp_speed < 0 )
          tmp_speed = 0;
        mynav->speed->setValue( tmp_speed );
      } else {
        H3DFloat tmp_speed = glwindow->default_speed - speed_increment;
        if( tmp_speed  < 0 )
          tmp_speed = 0;
        glwindow->default_speed = tmp_speed;
      }
    }
  }

  H3DWindowNode *glwindow;
  H3DFloat speed_increment;
};

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


int main(int argc, char* argv[]) {
#ifdef H3DAPI_LIB
  initializeH3D();
#endif

#ifdef HAVE_PYTHON
  PythonScript::setargv( argc, argv );
#endif
  // Settings and command line arguments ---

  // Help message

  stringstream str;
  str << H3DAPI_MAJOR_VERSION << "." << H3DAPI_MINOR_VERSION << "." << H3DAPI_BUILD_VERSION;

#ifdef H3D_ARCH64
  str << " x64";
#else
  str << " x86";
#endif 

  string version_string = str.str();

  string command = argv[0];
  string help_message = "\n";
  help_message += "H3DLoad(v" + version_string + ")\n";
  help_message += "Usage: " + command + " [Options] <X3D file>\n";
  help_message += "\n";
  help_message += "Options:\n";
  help_message += "\n";
  help_message += " -a --antialiasing      Use fullscreen anti-aliasing(default)\n";
  help_message += " -A --no-antialiasing   Don't fullscreen anti-aliasing\n";
  help_message += " -m --mirror            Flip screen upside down\n";
  help_message += " -M --no-mirror         Suppress flipping screen\n";
  help_message += " -f --fullscreen        Open in fullscreen mode\n";
  help_message += " -F --no-fullscreen     Suppress fullscreen mode\n";
  help_message += "    --screen=<W>x<H>    Set width and height for window\n";
  help_message += "\n";
  help_message += "    --deviceinfo=<file> Use <file> as device info\n";
  help_message += "    --stylus=<file>     Use <file> as stylus model\n";
  help_message += "    --viewpoint=<file>  Use <file> as viewpoint\n";
  help_message += "    --rendermode=<mode> Use render mode <mode>\n";
  help_message += " -s --spacemouse        Use 3DConnexion space mouse to\n";
  help_message += "                        navigate scene.\n";
  help_message += "\n";
  help_message += " -h --help              This help message\n";
  help_message += "\n";
  help_message += " When the scene is running the following keyBindings can ";
  help_message += " be used to navigate. \n";
  help_message += " To set active viewpoint: \n";
  help_message += " home                   Navigates to the initial viewpoint\n";
  help_message += " end                    Navigates to the final viewpoint\n";
  help_message += " pgup                   Navigates to the previous viewpoint\n";
  help_message += " pgdn                   Navigates to the next viewpoint. \n";
  help_message += "To set navitationType: \n";
  help_message += " w                      Set navigationType to WALK\n";
  help_message += " f                      Set navigationType to FLY\n";
  help_message += " l                      Set navigationType to LOOKAT\n";
  help_message += " e                      Set navigationType to EXAMINE\n";
  help_message += " n                      Set navigationType to NONE\n";
  help_message += "Other navigation options:\n";
  help_message += "+                       Increase navigation speed.\n";
  help_message += "-                       Decrease navigation speed.\n";
  help_message += "To set devices used for navigation: \n";
  help_message += " F1                     Enable/Disable mouse.";
  help_message += " Default is enabled.\n";
  help_message += " F2                     Enable/Disable keyboard.";
  help_message += " Default is enabled.\n";
  help_message += " F3                     Enable/Disable haptics device.";
  help_message += " Default is disabled.\n";
  help_message += " F4                     Enable/Disable SpaceWareSensor.";
  help_message += " Default is disabled.\n";
  help_message += " F5                     Reset current active viewpoint to ";
  help_message += "its starting position.\n";
  help_message += "\n";
  

  // H3D root, XML file and setup file


  vector<string> xml_files;

  char *r = getenv( "H3D_ROOT" );
  string h3d_root = r ? r : ""; 

#ifdef WIN32
  char *home = getenv( "HOMEPATH" );
  string ini_file_path = home ? string(home) + string( "/h3dload.ini" ) :
                                string("h3dload.ini");
#else
  char *home = getenv( "HOME" );
  string ini_file_path = home ? string(home) + string( "/.h3dload.ini" ) :
                                string(".h3dload.ini");
#endif

  bool ini_file_exists = false;
  ifstream check_ini_file( ini_file_path.c_str() );
  if( check_ini_file.is_open() ) {
    ini_file_exists = true;
  }
  check_ini_file.close();
  
  if( !ini_file_exists ){
    ini_file_path = h3d_root + "/settings/h3dload.ini";
    
    ifstream check_ini_file( ini_file_path.c_str() );
    if( check_ini_file.is_open() ) {
      ini_file_exists = true; }
    check_ini_file.close();
  }
  
  INIFile ini_file( ini_file_path );
  
  // Console settings

  bool console_show_time = GET_BOOL("console", "show_time", false);
  bool console_show_level = GET_BOOL("console", "show_level", false);
  int console_output_level = GET_INT("console", "output_level", 3 );

  ostream *console_ostream = 0;
  string ostream_str = GET4("H3D_CONSOLE_OSTREAM",
    "console","ostream",(string)"cerr");
  if ( ostream_str == "cerr" )
    console_ostream = &cerr;
  else if( ostream_str == "cout" )
    console_ostream = &cout;
  else
    Console(4) << "Invalid value \"" <<  ostream_str
    << "\" on ostream variable. Must be cerr or cout."<< endl;

  Console.setShowTime(console_show_time);
  Console.setShowLevel(console_show_level);
  Console.setOutputLevel(console_output_level);
  Console.setOutputStream(*console_ostream);

  // Graphics, devices, models and such
  string settings_path = 
    GET_ENV_INI_DEFAULT( "H3D_DISPLAY",
    h3d_root + "/settings/display/",
    "display","type",
    h3d_root + "/settings/common/" );

  string common_path =  h3d_root + "/settings/common/";

  string deviceinfo_file =
    GET_ENV_INI_DEFAULT_FILE( ini_file, "H3D_DEFAULT_DEVICEINFO",
    settings_path + "/device/",
    common_path + "/device/",
    "haptics device","device" );

  string stylus_file =
    GET_ENV_INI_DEFAULT_FILE( ini_file, "H3D_STYLUS",
    common_path + "/stylus/",
    common_path + "/stylus/",
    "haptics device","stylus" );

  string viewpoint_file =
    GET_ENV_INI_DEFAULT_FILE( ini_file, "H3D_DEFAULT_VIEWPOINT",
    settings_path + "/viewpoint/",
    common_path + "/viewpoint/",
    "graphical", "viewpoint" );

  string render_mode = GET4( "H3D_RENDERMODE",
    "graphical", "rendermode",
    (string)"MONO" );

  bool manualCursorControl = GET_BOOL("graphical", "manualCursorControl", false);
  if( char *buffer = getenv("H3D_MANUALCURSORCONTROL") ) {
    if (strcmp( buffer, "TRUE" ) == 0 ){
      manualCursorControl = true; }
    else if (strcmp( buffer, "FALSE" ) == 0 ){
      manualCursorControl = false; }
    else
      Console(4) << "Invalid value \"" << buffer 
      << "\" on environment "
      << "variable H3D_MANUALCURSORCONTROL. Must be TRUE or FALSE. "
      << endl;
  }
  
  string gamemode = GET4( "H3D_GAMEMODE",
                          "graphical", "gamemode", (string)"" );
  
  int width = GET_INT("graphical", "width", 640 );
  int height = GET_INT("graphical", "height", 480 );

  bool fullscreen    = GET_BOOL("graphical", "fullscreen", false);
  if( char *buffer = getenv("H3D_FULLSCREEN") ) {
    if (strcmp( buffer, "TRUE" ) == 0 ){
      fullscreen = true; }
    else if (strcmp( buffer, "FALSE" ) == 0 ){
      fullscreen = false; }
    else
      Console(4) << "Invalid value \"" << buffer 
      << "\" on environment "
      << "variable H3D_FULLSCREEN. Must be TRUE or FALSE. "
      << endl;
  }

  bool mirrored      = GET_BOOL("graphical", "mirrored", false);
  if( char *buffer = getenv("H3D_MIRRORED") ) {
    if (strcmp( buffer, "TRUE" ) == 0 ){
      mirrored = true; }
    else if (strcmp( buffer, "FALSE" ) == 0 ){
      mirrored = false; }
    else
      Console(4) << "Invalid value \"" << buffer 
      << "\" on environment "
      << "variable H3D_MIRRORED. Must be TRUE or FALSE. "<< endl;
  }

  bool use_space_mouse = false;
  bool antialiasing = true;
  // Command line arguments ---

  for( int i = 1 ; i < argc ; ++i ){

    if( argv[i][0] != '-' ){
      xml_files.push_back( string(argv[i]) );
      continue;
    }

    switch(argv[i][1]){

      // Long arguments ---

    case '-':

      if( !strcmp(argv[i]+2,"help") ){
        std::cout << help_message << endl;
        return 0;
      }

      else if( !strcmp(argv[i]+2,"mirror") ){
        mirrored = true; }
      else if( !strcmp(argv[i]+2,"antialiasing") ){
        antialiasing = true; }
      else if( !strcmp(argv[i]+2,"no-antialiasing") ){
        antialiasing = false; }

      else if( !strcmp(argv[i]+2,"no-mirror") ){
        mirrored = false; }

      else if( !strcmp(argv[i]+2,"manualcursorcontrol") ){
        manualCursorControl = true; }

      else if( !strcmp(argv[i]+2,"fullscreen") ){
        fullscreen = true; }

      else if( !strcmp(argv[i]+2,"no-fullscreen") ){
        fullscreen = false; }

      else if( !strncmp(argv[i]+2,"screen=",
        strlen("screen=")) ){
          height = atoi( strstr(argv[i],"x")+1 );
          *strstr(argv[i],"x") = '\0';
          width = atoi( strstr(argv[i],"=")+1 );
      }

      else if( !strncmp(argv[i]+2,"deviceinfo=",
        strlen("deviceinfo=")) ){
          deviceinfo_file = strstr(argv[i],"=")+1; }

      else if( !strncmp(argv[i]+2,"stylus=",
        strlen("stylus=")) ){
          stylus_file = strstr(argv[i],"=")+1; }

      else if( !strncmp(argv[i]+2,"viewpoint=",
        strlen("viewpoint=")) ){
          viewpoint_file = strstr(argv[i],"=")+1; }

      else if( !strncmp(argv[i]+2,"rendermode=",
        strlen("rendermode=")) ){
          render_mode = strstr(argv[i],"=")+1; }

      else if( !strncmp(argv[i]+2,"gamemode=",
        strlen("gamemode=")) ){
          gamemode = strstr(argv[i],"=")+1; }

      else if( !strcmp(argv[i]+2,"spacemouse") ){
        use_space_mouse = true; }
      else {
        Console(4) << "Unknown argument "
          << "'" << argv[i] << "'" << endl; }
      break;

      // Short arguments ---

    case 'm':
      mirrored = true;
      break;

    case 'M':
      mirrored = false;
      break;

    case 'a':
      antialiasing = true;
      break;

    case 'A':
      antialiasing = false;
      break;

    case 'f':
      fullscreen = true;
      break;

    case 'F':
      fullscreen = false;
      break;

    case 's':
      use_space_mouse = true;
      break;

    default:
      Console(4) << "Unknown argument "
        << "'" << argv[i] << "'" << endl;
    }
  }

  if (!xml_files.size()){
    Console(4) << help_message << endl;
    return 1;
  }


  // Loading X3D file and setting up VR environment ---
  AutoRef< Scene > scene( new Scene );
  try {
    AutoRef< KeySensor > ks( new KeySensor );

    X3D::DEFNodes dn;
    auto_ptr< ChangeViewport > change_viewpoint( new ChangeViewport );
    auto_ptr< ChangeNavType > change_nav_type( new ChangeNavType );
    AutoRef< Node > device_info;
    AutoRef< Node > viewpoint;
    

    SAI::Browser *sai_browser = scene->getSAIBrowser();
    sai_browser->setName( "H3DLoad" );
    sai_browser->setVersion( version_string );

    if( deviceinfo_file.size() ){
      try {
        device_info = X3D::createX3DNodeFromURL( deviceinfo_file );
      } catch( const Exception::H3DException &e ) {
        Console(3) << "Warning: Could not create default DeviceInfo node "
          << "from file \"" << deviceinfo_file << "\": "
          << e << endl;
      }
    }

    DeviceInfo *di = DeviceInfo::getActive();
    unsigned int device_info_size = (unsigned int) DeviceInfo::getAllDeviceInfos().size();
    if( di && stylus_file.size() ) {
      AutoRef< Node > default_stylus;
      try {
        default_stylus = X3D::createX3DNodeFromURL( stylus_file );
      } catch( const Exception::H3DException &e ) {
        Console( 4 ) << "Warning: Could not create default stylus "
          << "from file \"" << stylus_file << "\": "
          << e << endl;
      }

      for( DeviceInfo::MFDevice::const_iterator i = di->device->begin();
        i != di->device->end(); ++i ) {
          H3DHapticsDevice *d = static_cast< H3DHapticsDevice * >(*i);
          if( !d->stylus->getValue() )
            d->stylus->setValue( default_stylus );
      }
    }

    // create a window to display
    GLUTWindow *glwindow = new GLUTWindow;
    change_nav_type->setOwnerWindow( glwindow );
    change_viewpoint->setOwnerWindow( glwindow );
    ks->keyPress->route( change_nav_type );  //###########
    glwindow->fullscreen->setValue( fullscreen );
    glwindow->mirrored->setValue( mirrored );
    glwindow->renderMode->setValue( render_mode );
    glwindow->manualCursorControl->setValue( manualCursorControl );
    glwindow->gameMode->setValue( gamemode );
    glwindow->width->setValue(width);
    glwindow->height->setValue(height);
    glwindow->useFullscreenAntiAliasing->setValue( antialiasing );
    scene->window->push_back( glwindow );

    AutoRef< Group > g( new Group );
    for( vector<string>::iterator file = xml_files.begin() ;
      file != xml_files.end() ; ++file ){
      Console(3) << "Loading " << *file << endl;
      scene->loadSceneRoot( *file );
    }

    DeviceInfo::DeviceInfoList device_infos = DeviceInfo::getAllDeviceInfos();
    if( di && device_infos.size() > device_info_size ) {
      unsigned int j = 0;
      for( DeviceInfo::DeviceInfoList::iterator i = device_infos.begin();
           i != device_infos.end(); ++i ) {
        if( j < device_info_size )
          di->set_bind->setValue( false );
        else {
          (*i)->set_bind->setValue( true );
          break;
        }
        ++j;
      }
    }

    ks->actionKeyPress->route( change_viewpoint );  //###########

#ifndef MACOSX
    if( use_space_mouse ) {
      if( !H3DNavigation::isEnabled( H3DNavigation::SWS ) )
          H3DNavigation::enableDevice( H3DNavigation::SWS );
    }
#endif    
    // create a Viewpoint if it does not exist.
    if( !X3DViewpointNode::getActive() && viewpoint_file.size() ) {
      try {
        viewpoint = X3D::createX3DNodeFromURL( viewpoint_file );
      } catch( const Exception::H3DException &e ) {
        Console(3) << "Warning: Could not create default Viewpoint node "
          << "from file \"" << viewpoint_file << "\": "
          << e << endl;
      }
    }

    if( X3DBindableNode::getStack( "DeviceInfo" ).size() > 1 ) {
      device_info.reset( NULL );
    }

    dn.clear();

    Scene::mainLoop();
  }
  catch (const Exception::QuitAPI &) {
  }

  catch (const Exception::H3DException &e) {
    Console(4) << e << endl;

    // This is required to ensure the scene is deleted
    // in all cases probably due to circular references
    scene->setSceneRoot ( NULL );

#ifdef H3DAPI_LIB
    deinitializeH3D();
#endif
    return 1;
  }

  // This is required to ensure the scene is deleted
  // in all cases probably due to circular references
  scene->setSceneRoot ( NULL );

#ifdef H3DAPI_LIB
  deinitializeH3D();
#endif
  return 0;
}

