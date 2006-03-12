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
#include "Console.h"

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
  Console(4) << "H3DLoad build 204" << endl;
  if (argc < 2){
    Console(4) << "Usage: " << argv[0] << " <X3D file>" << endl;
    return 1;
  }

  const char* xml_file = argv[1];
  //const char* xml_file = "Test.x3d";
  try {
    // TODO: move someplace where is is initialized once
    //		  XMLPlatformUtils::Initialize();

    AutoRef< KeySensor > ks( new KeySensor );
    AutoRef< MouseSensor > ms( new MouseSensor );
    X3D::DEFNodes dn;
    KeyRotation *kr = new KeyRotation;
    QuitAPIField *quit_api = new QuitAPIField;
    AutoRef< Transform > t( new Transform );

    Console(3) << "Loading " << xml_file << endl;
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
      Console(2) << "No Viewpoint node found, creating one." << endl;

      Viewpoint *vp = new Viewpoint;
      vp->position->setValue( Vec3f( 0, 0, 0.6f ) );
    }
                         
    g->children->push_back( t.get() );

    bool fullscreen = false;
    bool mirrored = false;
    string render_mode = "MONO";

    // check enviroment variables to see if the window parameters has
    // been set by the user.
    char * buffer;
    buffer = getenv ("H3D_FULLSCREEN");
    if( buffer!=NULL ) {
      if (strcmp( buffer, "TRUE" ) == 0 )
        fullscreen = true;
      else if(strcmp( buffer, "FALSE" ) != 0 )
        Console(4) << "Invalid valid value \"" << buffer << "\" on environment "
                   << "variable H3D_FULLSCREEN. Must be TRUE or FALSE. "<< endl;
    }

    buffer = getenv ("H3D_MIRRORED");
    if( buffer!=NULL ) {
      if (strcmp( buffer, "TRUE" ) == 0 )
        mirrored = true;
      else if(strcmp( buffer, "FALSE" ) != 0 )
        Console(4) << "Invalid valid value \"" << buffer << "\" on environment "
                   << "variable H3D_MIRRORED. Must be TRUE or FALSE. "<< endl;
    }

    buffer = getenv ("H3D_RENDERMODE");
    if( buffer!=NULL ) {
      render_mode = buffer;
    }
		  
    AutoRef< Scene > scene( new Scene );
    // create a window to display
    GLUTWindow *glwindow = new GLUTWindow;

    glwindow->fullscreen->setValue( fullscreen );
    glwindow->mirrored->setValue( mirrored );
    glwindow->renderMode->setValue( render_mode );
    scene->window->push_back( glwindow );
    scene->sceneRoot->setValue( g.get() );
    Scene::mainLoop();
  }
  catch (const QuitAPIException &) {
  }

  catch (const Exception::H3DException &e) {
    Console(4) << "Exception:" << endl;
    Console(4) << "   " << e << endl;
  }

}

