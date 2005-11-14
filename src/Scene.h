//////////////////////////////////////////////////////////////////////////////
//    Copyright 2004, SenseGraphics AB
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
/// \file Scene.h
/// \brief Header file for Scene.
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __SCENE_H__
#define __SCENE_H__

#include "X3DChildNode.h"
#include "GLWindow.h"

namespace H3D {

  /// \ingroup Nodes
  /// \class Scene
  /// \brief The Scene node is topmost node that takes care of the rendering
  /// of the scene graph both haptically and graphically. Scene::mainLoop() 
  /// can be called to start the main event loop, e.g. start the haptic and
  /// graphical rendering. Before this function is called at least one instance
  /// of Scene must have created and be active. 
  /// 
  class H3DAPI_API Scene : public Node {
  public:

    typedef TypedMFNode< GLWindow > MFWindow;
    typedef TypedSFNode< X3DChildNode > SFChildNode;

    /// Constructor.
    Scene( Inst< SFChildNode > _sceneRoot   = 0,
           Inst< MFWindow    > _window      = 0,
           Inst< SFFloat     > _frameRate   = 0 );

    /// Destructor.
    ~Scene();

    /// Set whether this Scene instance should be active or not. If
    /// a Scene is not active it will not be rendered at all. 
    void setActive( bool _active ) {
      active = _active;
    }

    /// Returns if the Scene instance is active or not.
    bool isActive() {
      return active;
    }

    /// This function will be called by the event loop when there are no other
    /// events to process and the Scene is active. Performs the rendering.
    virtual void idle();

    /// Static function that is called to start the main event loop.
    /// Before this function is called at least one instance
    /// of Scene must have created and be active. 
    static void mainLoop();
    
    /// The scene graph to render in this scene.
    ///
    /// <b>Access type: </b> inputOutput
    auto_ptr< SFChildNode > sceneRoot;

    /// The windows to render into this scene.
    ///
    /// <b>Access type: </b> inputOutput
    auto_ptr< MFWindow >  window;

    /// The instantaneous frame rate of the scene.
    ///
    /// <b>Access type: </b> outputOnly
    auto_ptr< SFFloat >  frameRate;

    /// Current time within the simulation, updated during each graphic loop.
    ///
    /// <b>Access type: </b> outputOnly
    static SFTime *time;
  
    /// All instances of Scene that has been created.
    static set< Scene* > scenes;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;

  protected:
    /// The EventSink class makes all fields up-to-date what are routed 
    /// to it, with the exception of PeriodicUpdateFields. These are
    /// only updated when the timeToUpdate() function returs true.
    class EventSink: public Field {
    public:
      /// Constructor.
      EventSink() {
        setName( "Scene::eventSink" );
      }
    protected:
      virtual void update();
    };
  public:
    static EventSink *eventSink;

  private:
    bool active;
    // the time of the start of the last loop.
    TimeStamp last_time;
    // the TraverseInfo instance from the previous scenegraph loop.
    TraverseInfo *last_traverseinfo;
  };
}

#endif
