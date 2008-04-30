// This is the example from the H3D Manual(Examples section)

// Files included that are needed for setting up the scene graph
#include <H3D/X3D.h>
#include <H3D/Scene.h>
#include <H3D/MouseSensor.h>
#include <H3D/Material.h>
#include <H3D/GLUTWindow.h>

using namespace H3D;

// The Color class is of type SFColor and its value is determined by 
// the SFBool field that is routed to it. If its value is true the color
// is red, otherwise it is blue.
class Color : public TypedField< SFColor, SFBool > {
  virtual void update() {
    if( static_cast< SFBool * >(event.ptr)->getValue() )
      value = RGB( 1, 0, 0 );
    else
      value = RGB( 0, 0, 1 );
  }
};

int main(int argc, char* argv[]) {
  
  // Set up the scene graph by specifying a string 
  // and using createX3DNodeFromString
  string theSceneGraphString = "<Group>"
                             "<Viewpoint position=\"0 0 1\" />"
                             "  <Shape>"
                             "    <Appearance>"
                             "      <Material DEF=\"MATERIAL\" />"
                             "    </Appearance>"
                             "    <Sphere radius=\"0.1\" />"
                             "  </Shape>"
                             "  <MouseSensor DEF=\"MS\" />"
                             "</Group>";

  // myDefNodes contains functions for getting a Node from the scenegraph
  // by giving the DEF name of the node as a string.
  X3D::DEFNodes *myDefNodes = new X3D::DEFNodes();

  // createX3DNodeFromString returns an AutoRef containing a pointer 
  // to the top most node in the given string
  AutoRef< Node > myGroup( X3D::createX3DNodeFromString( 
                              theSceneGraphString, myDefNodes ) );

  // Getting the nodes needed for routes
  MouseSensor *myMouseSensor = 
    static_cast< MouseSensor * > (myDefNodes->getNode("MS"));
  Material *myMaterial = 
    static_cast< Material * > (myDefNodes->getNode("MATERIAL"));
  
  // Creating and instance of the Color class needed for routes.
  Color *myColor = new Color();

  // Setting up routes
  myMouseSensor->leftButton->route( myColor );
  myColor->route( myMaterial->diffuseColor );

  /// Create a new scene
  AutoRef< Scene > scene( new Scene );
  
  // create a window to display
  GLUTWindow *glwindow = new GLUTWindow;
  
  // add the window to the scene.
  scene->window->push_back( glwindow );
  
  // add our group node that is to be displayed to the scene.
  scene->sceneRoot->setValue( myGroup.get() );
  
  // start the main loop
  Scene::mainLoop();
}
