// This is the example from the H3D Manual(Examples section)

// Files included that are needed for setting up the scene graph
#include <H3D/X3D.h>
#include <H3D/Scene.h>
#include <H3D/MouseSensor.h>
#include <H3D/Material.h>
#include <H3D/Shape.h>
#include <H3D/Transform.h>
#include <H3D/GLUTWindow.h>

using namespace H3D;

// Global variables.
// The geometry to add at each click.
AutoRef< Node > sphere(
  X3D::createX3DNodeFromString( "<Sphere radius=\"0.02\" /> ") );

X3D::DEFNodes myDefNodes;
AutoRef< Node > group(
  X3D::createX3DNodeFromString( "<Group>\n"
                                "<Viewpoint position=\"0 0 1\" />\n"
                                "  <Group DEF=\"GROUP\" />\n"
                                "  <MouseSensor DEF=\"MS\" />\n"
                                "</Group>", 
                                &myDefNodes ) );

// The AddSphere class adds a new sphere to the group nodes children
// field each time a field routed to it generates an event.
class AddSphere : public AutoUpdate< SFBool > { 
protected:
  virtual void update() { 
    AutoUpdate< SFBool >::update();
    if( value ) { 
      X3D::DEFNodes tempDefNodes;
      AutoRef< Transform > tempTransform(
                     static_cast< Transform * >(  X3D::createX3DNodeFromString(
"<Transform>\n"
"  <Shape DEF=\"SHAPE\" >\n"
"     <Appearance>\n"
"        <Material DEF=\"MATERIAL\" />\n"
"     </Appearance>\n"
"  </Shape> \n"
"</Transform>", &tempDefNodes ).get() ) );

      RGB c = RGB( (H3DFloat)rand()/RAND_MAX,
                   (H3DFloat)rand()/RAND_MAX,
                   (H3DFloat)rand()/RAND_MAX );
      Material *m;
      tempDefNodes.getNode( "MATERIAL", m );
      m->diffuseColor->setValue( c );

      Shape *s;
      tempDefNodes.getNode( "SHAPE", s );
      s->geometry->setValue( sphere );

      tempTransform->translation->setValue( Vec3f( c.r * 0.5f - 0.25f,
                                                   c.g * 0.5f - 0.25f,
                                                   c.b * 0.5f - 0.25f ) );
      Group *g;
      myDefNodes.getNode( "GROUP", g );
      g->children->push_back( tempTransform.get() );
    }
  }
};

int main(int argc, char* argv[]) {
  // Creating and instance of the AddSphere class needed for routes.
  auto_ptr< AddSphere > myAddSphere( new AddSphere() );
  // Setting up routes
  MouseSensor *ms;
  myDefNodes.getNode( "MS", ms );
  ms->leftButton->route( myAddSphere );

  /// Create a new scene
  AutoRef< Scene > scene( new Scene );
  
  // create a window to display
  GLUTWindow *glwindow = new GLUTWindow;
  
  // add the window to the scene.
  scene->window->push_back( glwindow );
  
  // add our group node that is to be displayed to the scene.
  scene->sceneRoot->setValue( group.get() );
  
  // start the main loop
  Scene::mainLoop();

}
