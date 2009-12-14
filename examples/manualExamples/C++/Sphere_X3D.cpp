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
protected:
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
  string scene_graph_string = "<Group>"
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
  X3D::DEFNodes def_nodes = X3D::DEFNodes();

  // createX3DNodeFromString returns an AutoRef containing a pointer 
  // to the top most node in the given string
  AutoRef< Node > group( X3D::createX3DNodeFromString( 
                            scene_graph_string, &def_nodes ) );

  // Getting the nodes needed for routes
  MouseSensor *mouse_sensor = 
    static_cast< MouseSensor * > (def_nodes.getNode("MS"));
  Material *material = 
    static_cast< Material * > (def_nodes.getNode("MATERIAL"));
  
  // Creating and instance of the Color class needed for routes.
  auto_ptr< Color > myColor( new Color() );

  // Setting up routes
  mouse_sensor->leftButton->route( myColor );
  myColor->route( material->diffuseColor );

  /// Create a new scene
  AutoRef< Scene > scene( new Scene );
  
  // create a window to display
  GLUTWindow *gl_window = new GLUTWindow;
  
  // add the window to the scene.
  scene->window->push_back( gl_window );
  
  // add our group node that is to be displayed to the scene.
  scene->sceneRoot->setValue( group.get() );
  
  // start the main loop
  Scene::mainLoop();
}
