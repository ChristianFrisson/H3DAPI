//////////////////////////////////////////////////////////////////////////////
//    Copyright 2004-2013, SenseGraphics AB
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

#include <H3D/X3DChildNode.h>
#include <H3D/H3DWindowNode.h>
#include <H3D/SAIFunctions.h>
#include <H3D/ShadowCaster.h>

// HAPI includes
#include <H3DUtil/Threads.h>

#include <vector>

namespace H3D {

  /// \ingroup X3DNodes
  /// \class Scene
  /// \brief The Scene node is topmost node that takes care of the rendering
  /// of the scene graph both haptically and graphically. Scene::mainLoop() 
  /// can be called to start the main event loop, e.g. start the haptic and
  /// graphical rendering. Before this function is called at least one instance
  /// of Scene must have created and be active. 
  /// 
  class H3DAPI_API Scene : public Node {
  public:

    /*    /// Class for storing meta information about the scene. This meta
    /// information is used by e.g. SAI functions in e.g. Ecmascript
    /// to provide information about the currently loaded scene.
    /// The creator of a scene should always make sure that the 
    /// scene info is up to date. The scene info is updated automatically
    /// when using the loadSceneRoot function, but if a user sets the sceneRoot
    /// directly he should also update the scene root directly to match
    /// his change.
    class H3DAPI_API SceneInfo {

      /// Get the name of the browser used to view the scene. 
      inline const string &getBrowserName() {
        return browser_name;
      }

      /// Set the name of the browser used to view the scene, e.g.
      /// "H3DLoad" or "H3DViewer"
      inline void setBrowserName( const string& name) {
        browser_name = name;
      }

    protected:
      string browser_name;
      string browser_version;

      string world_url;
      string specification_version;
      string encoding;
      
      string metadata;

      DEFNodes named_nodes;
      DEFNodes exported_nodes;
      DEFNodes imported_nodes;
      PrototypeVector protos;
      // TODO:
      //      routes
    };
    */


    typedef TypedMFNode< H3DWindowNode > MFWindow;
    typedef TypedSFNode< X3DChildNode > SFChildNode;

    /// Constructor.
    Scene( Inst< SFChildNode > _sceneRoot   = 0,
           Inst< MFWindow    > _window      = 0,
           Inst< SFFloat     > _frameRate   = 0 );

    /// Destructor.
    ~Scene();

    /// Returns the TraverseInfo from the previous sceneRoot traversal.
    inline TraverseInfo * getLastTraverseInfo() {
      return last_traverseinfo;
    }

    /// Set whether this Scene instance should be active or not. If
    /// a Scene is not active it will not be rendered at all. 
    void setActive( bool _active ) {
      active = _active;
    }

    /// Returns if the Scene instance is active or not.
    inline bool isActive() {
      return active;
    }

    /// This function will be called by the event loop when there are no other
    /// events to process and the Scene is active. Performs the rendering.
    virtual void idle();

    /// Load the scene graph that is to be the root of the Scene object.
    /// This function should always be used instead of setting sceneRoot
    /// directly in order to keep SAI information up to data.
    void loadSceneRoot( const string &url );

    /// Set the scene to use based on a SAIScene object.
    void setSceneRoot( SAI::SAIScene *scene_info );

    /// Returns the SAI::Browser object for this Scene.
    inline SAI::Browser *getSAIBrowser() {
      return &SAI_browser;
    }
    
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
    static auto_ptr< SFTime > time;
    //static SFTime *time;
  
    /// All instances of Scene that has been created.
    static set< Scene* > scenes;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;

    /// Return code for callback functions. 
    typedef enum {
      /// The callback is done and should not be called any more.
      CALLBACK_DONE,
      /// The callback should be rescheduled and called the next loop 
      /// again.
      CALLBACK_CONTINUE
    } CallbackCode;

    /// Callback function type.
    typedef CallbackCode (*CallbackFunc)(void *data); 

    /// Add a callback function to be called after scene traversal and
    /// rendering.
    static void addCallback( CallbackFunc func, void *data ) {
      callback_lock.lock();
      callbacks.push_back( make_pair( func, data ) );
      callback_lock.unlock();
    }


    struct ProgramSetting {
      /// Constructor.
    ProgramSetting( Field *_field,
                    const string &_name = "",
                    const string &_section = ""):
        field( _field ),
        name( _name ),
        section( _section ) {}
        

      Field *field;
      string name;
      string section;

    };

    typedef std::list< ProgramSetting > ProgramSettings;

    /// Add a program setting value. A program setting value is a global
    /// Field value assiciated with a name. These settings can be used in
    /// GUIs in e.g. H3DViewer to allow a user to change the values. Settings
    /// can be dynamically added and removed during the execution of a program.
    ///
    /// \param field The field to assiciate the name with. A field
    /// will be automatically removed from the settings if destructed
    /// somewhere else.   
    /// \param name The name of the parameter. If "" the name of the
    /// field will be used.
    /// \param section The type if paramter it is, e.g. graphics.
    static void addProgramSetting( Field *field,
                                   const string &name = "",
                                   const string &section = "Main settings");

    /// Remove a program setting value. 
    ///
    /// \param name The name of the parameter. If "" the name of the
    /// field will be used.
    /// \param section The type if paramter it is, e.g. graphics.
    /// \return true if successful, false otherwise.
    static bool removeProgramSetting( const string &name,
                                      const string &section = "Main settings" );

    /// Remove all program settings referring to a field. 
    ///
    /// \param  field The field which settings to remove.
    /// \return true if successfully removed, false otherwise.
    static bool removeProgramSetting( Field *field );

    /// Get a program setting value. 
    ///
    /// \param name The name of the parameter.
    /// \param section The type if paramter it is, e.g. graphics.
    /// \return The field that matches the name if it exists, NULL otherwise.
    static Field *getProgramSetting( const string &name,
                                     const string &section = "Main settings" );

    typedef ProgramSettings::iterator SettingsIterator;

    /// Get an iterator to the first element of the program settings. Adding
    /// or removing a new program setting will make this iterator invalid.
    static SettingsIterator programSettingsBegin();

    /// Get an iterator to the end of the program settings. 
    static SettingsIterator programSettingsEnd();

    typedef enum {
      ADD_SETTING,
      REMOVE_SETTING
    } ProgramSettingsEvent;
 
    /// Callback function type.
    typedef void (*ProgramSettingsCallbackFunc)( ProgramSettingsEvent event,
                                                 const ProgramSetting &settings, 
                                                 void *data); 

    typedef std::pair< ProgramSettingsCallbackFunc, void * > ProgramSettingsCallbackId;

    /// Add a callback function to be called each time a new program settings value
    /// is added, removed or changed.
    static ProgramSettingsCallbackId addProgramSettingsCallback( ProgramSettingsCallbackFunc func, 
                                                          void *data );

    /// Remove a program settings callback function
    static bool removeProgramSettingsCallback( ProgramSettingsCallbackId id );

    /// Remove all program setting callback functions
    static void clearProgramSettingsCallbacks();

    /// Find a child node for a given group node with a certain nodeType and optional name
    H3D::Node* findChildNode(H3D::Group *group, const std::string &nodeType, const std::string &nodeName="");

    /// Top down search for a node with a certain nodeType and optional name starting from node passed as argument
    H3D::Node* findNodeType(H3D::Node *node, const std::string &nodeType, const std::string &nodeName="");

    
  protected:
    SAI::Browser SAI_browser;

    static H3DUtil::MutexLock callback_lock;

    static ProgramSettings program_settings;

    typedef std::list< std::pair< CallbackFunc, void * > > CallbackList;
    typedef std::list< std::pair< ProgramSettingsCallbackFunc, void * > > ProgramSettingsCallbackList;
    // A list of the callback functions to run.
    static CallbackList callbacks;
    static ProgramSettingsCallbackList program_settings_callbacks;
    
    /// The EventSink class makes all fields up-to-date what are routed 
    /// to it, with the exception of PeriodicUpdateFields. These are
    /// only updated when the timeToUpdate() function returs true.
    class EventSink: public Field {
    public:
      /// Constructor.
      EventSink() {
        setName( "Scene::eventSink" ); 
      }
      ~EventSink(){
    }  
    protected:
      virtual void update();
    };
   
  public:
    static auto_ptr<EventSink> eventSink;
    //static EventSink *eventSink;

  private:
    bool active;
    // the time of the start of the last loop.
    TimeStamp last_time;
    // the TraverseInfo instance from the previous scenegraph loop.
    TraverseInfo *last_traverseinfo;
    // Reference to shadow caster used to cast shadows for shapes
    // in scene graph.
    AutoRef< ShadowCaster > shadow_caster;
  };
}

#endif
