// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <fstream>
#include "X3D.h"
#include <string.h>

#include <iostream>

#include "X3DSAX2Handlers.h"
#include "GLUTWindow.h"
#include "Group.h"
#include "Transform.h"
#include "Scene.h"
#include "KeySensor.h"
#include "MouseSensor.h"
#include "DEFNodes.h"
#include "Viewpoint.h"
#include "DeviceInfo.h"
#include "INIFile.h"

#ifdef MACOSX
#include "FreeImage.h"
#include <xercesc/util/PlatformUtils.hpp>
#endif


using namespace std;
using namespace H3D;

class KeyRotation: public TypedField< SFRotation, 
              Types< SFInt32,
              SFBool,
              SFVec2f > > {
  virtual void update() {
    bool button_pressed = static_cast< SFBool * >(routes_in[1])->getValue();
    Vec2f motion = static_cast< SFVec2f * >(routes_in[2])->getValue();
    motion.y = -motion.y;

    if( button_pressed && motion * motion > Constants::f_epsilon ) {
      Vec2f perp = Vec2f( -motion.y, motion.x );
      perp.normalize();
      value = Rotation( perp.x, perp.y, 0, motion.length() * 0.01f ) * value;
    } else {
      if( event.ptr == routes_in[0] ) {
        int key = static_cast< SFInt32 * >(routes_in[0])->getValue();
        if( key == KeySensor::UP ) {
          value = Rotation( 1,0,0,-0.1f ) * value;
        }
        if( key == KeySensor::DOWN ) {
          value = Rotation( 1,0,0,0.1f ) * value;
        }
        if( key == KeySensor::LEFT ) {
          value = Rotation( 0,1,0,-0.1f ) * value;
        }
        if( key == KeySensor::RIGHT ) {
          value = Rotation( 0,1,0,0.1f ) * value;
        }
      }
    }
  }
};

H3D_API_EXCEPTION( QuitAPIException );

class QuitAPIField: public AutoUpdate< SFString > {
  virtual void update() {
    string s = static_cast< SFString * >(routes_in[0])->getValue();
    if( s[0] == 27 ) {
      throw QuitAPIException();
    }
  }
};


int main(int argc, char* argv[]) {
  Console.setShowLevel(false);
  
  if (argc < 2){
    Console(4) << "Usage: " << argv[0] << " <X3D file>" << endl;
    return 1;
  }

  const char* xml_file = argv[1];
  try {
    AutoRef< KeySensor > ks( new KeySensor );
    AutoRef< MouseSensor > ms( new MouseSensor );
    X3D::DEFNodes dn;
    KeyRotation *kr = new KeyRotation;
    QuitAPIField *quit_api = new QuitAPIField;
    AutoRef< Transform > t( new Transform );
    Console(3) << "Loading " << xml_file << endl;
    AutoRef< Node > device_info;
    AutoRef< Node > viewpoint;
    AutoRef< Scene > scene( new Scene );

    char * buffer;

    buffer = getenv ("H3D_DEFAULT_DEVICEINFO");

    if( buffer ) {
      try {
        device_info = X3D::createX3DNodeFromURL( buffer );
      } catch( const Exception::H3DException &e ) {
        Console(3) << "Warning: Could not create default DeviceInfo node "
                   << "from file \"" << buffer << "\" pointed to by "
                   << "environment variable H3D_DEFAULT_DEVICEINFO. " 
                   << e << endl;
      }
    } else {
      // ini file setting
      buffer = getenv( "H3D_ROOT" );

      if( buffer ) {
        string path = string( buffer ) + 
          "/settings/current/deviceinfo.x3d";
        try {
          device_info = X3D::createX3DNodeFromURL( path );
        } catch( const Exception::H3DException &e ) {
          Console(3) << "Warning: Could not create default DeviceInfo node "
                     << "from file \"" << path << "\" "
                     << e << endl;
        }
      }
    }
    
    DeviceInfo *di = DeviceInfo::getActive();
    if( di ) {
      string stylus_path = string( buffer ) + 
        "/settings/current/stylus.x3d";
      AutoRef< Node > default_stylus;
      try {
        default_stylus = X3D::createX3DNodeFromURL( stylus_path );
      } catch( const Exception::H3DException &e ) {
        Console( 4 ) << "Warning: Could not create default stylus"
                     << "from file \"" << stylus_path << "\" "
                     << e << endl;
      }
      
      for( DeviceInfo::MFDevice::const_iterator i = di->device->begin();
           i != di->device->end(); i++ ) {
        H3DHapticsDevice *d = static_cast< H3DHapticsDevice * >(*i);
        if( !d->stylus->getValue() )
          d->stylus->setValue( default_stylus );
      }
    }

    t->children->push_back( X3D::createX3DFromURL( xml_file, 
                                                   &dn ) );
    
    // set up routes to rotate the model
    ks->keyPress->route( quit_api );
    ks->actionKeyPress->route( kr );
    ms->leftButton->route( kr );
    ms->motion->route( kr );
    kr->route( t->rotation );
		  
    AutoRef< Group > g( new Group );     
    
    // create a Viewpoint if it does not exist.
    if( !Viewpoint::getActive() ) {
      buffer = getenv ("H3D_DEFAULT_VIEWPOINT");
      if( buffer ) {
        try {
          viewpoint = X3D::createX3DNodeFromURL( buffer );
        } catch( const Exception::H3DException &e ) {
          Console(3) << "Warning: Could not create default Viewpoint node "
                     << "from file \"" << buffer << "\" pointed to by "
                     << "environment variable H3D_DEFAULT_VIEWPOINT. " 
                     << e << endl;
        }
      } else {
        // ini file setting
        buffer = getenv( "H3D_ROOT" );
        
        if( buffer ) {
          string path = string( buffer ) + 
            "/settings/current/viewpoint.x3d";
          try {
            viewpoint = X3D::createX3DNodeFromURL( path );
          } catch( const Exception::H3DException &e ) {
            Console(3) << "Warning: Could not create default Viewpoint node "
                       << "from file \"" << path << "\" "
                       << e << endl;
          }
        } 
      }
    }

    if( X3DBindableNode::getStack( "DeviceInfo" ).size() > 1 ) {
      device_info.reset( NULL );
    }

    g->children->push_back( t.get() );

    bool fullscreen = false;
    bool mirrored = false;
    string render_mode = "MONO";

    buffer = getenv( "H3D_ROOT" );
    if( buffer ) {
      INIFile ini_file( string( buffer ) + "/settings/h3dload.ini" );
      if( ini_file.hasOption( "graphical", "mirrored" ) ) {
        mirrored = ini_file.getBoolean( "graphical", "mirrored" );
      }
      if( ini_file.hasOption( "graphical", "fullscreen" ) ) {
        fullscreen = ini_file.getBoolean( "graphical", "fullscreen" );
      }
      if( ini_file.hasOption( "graphical", "rendermode" ) ) {
        render_mode = ini_file.get( "graphical", "rendermode" );
      }
    }

    // check enviroment variables to see if the window parameters has
    // been set by the user.
    buffer = getenv ("H3D_FULLSCREEN");
    if( buffer!=NULL ) {
      if (strcmp( buffer, "TRUE" ) == 0 )
        fullscreen = true;
      else if(strcmp( buffer, "FALSE" ) != 0 )
        Console(4) << "Invalid valid value \"" << buffer 
                   << "\" on environment "
                   << "variable H3D_FULLSCREEN. Must be TRUE or FALSE. "
                   << endl;
    }

    buffer = getenv ("H3D_MIRRORED");
    if( buffer!=NULL ) {
      if (strcmp( buffer, "TRUE" ) == 0 )
        mirrored = true;
      else if(strcmp( buffer, "FALSE" ) != 0 )
        Console(4) << "Invalid valid value \"" << buffer 
                   << "\" on environment "
             << "variable H3D_MIRRORED. Must be TRUE or FALSE. "<< endl;
    }

    buffer = getenv ("H3D_RENDERMODE");
    if( buffer!=NULL ) {
      render_mode = buffer;
    }
		  
    // create a window to display
    GLUTWindow *glwindow = new GLUTWindow;
    GLUTWindow *glwindow2 = new GLUTWindow;

    glwindow->fullscreen->setValue( fullscreen );
    glwindow->mirrored->setValue( mirrored );
    glwindow->renderMode->setValue( render_mode );
    scene->window->push_back( glwindow );
 //   scene->window->push_back( glwindow2 );
    scene->sceneRoot->setValue( g.get() );
    Scene::mainLoop();
  }
  catch (const QuitAPIException &) {
  }

  catch (const Exception::H3DException &e) {
    Console(4) << e << endl;
  }

}

