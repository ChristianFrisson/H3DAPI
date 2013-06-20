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
#include <string>


// HAPI includes
#include <H3DUtil/Threads.h>
#ifdef HAVE_PROFILER
#include <H3DUtil/H3DTimer.h>
#endif
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
    #ifdef HAVE_PROFILER
    // profiledResult structure
    class profiledResultData
    {
      private:
      pthread_t thread_id;
      std::string profiled_result_string;
      std::string thread_debug_string;
      public:
      void setResult(pthread_t id,std::string result_profile)
      {
        if(!result_profile.empty())
          {this->thread_id=id;this->profiled_result_string=result_profile;}
      }
      void setResult(pthread_t id,std::string result_profile, std::string result_thread_debug)
      {
        if(!result_profile.empty())
        {
          this->thread_id=id;
          this->profiled_result_string=result_profile;
          this->thread_debug_string=result_thread_debug;
        }
      }
      void setThread_debug(std::string result){this->thread_debug_string=result;}
      bool isEmpty(){return this->profiled_result_string.empty()&&this->thread_debug_string.empty();}
      pthread_t getId(){return thread_id;}
      std::string getResult();
      //std::string getString(){return profiled_result_string;}
    };
    #endif

    typedef TypedMFNode< H3DWindowNode > MFWindow;
    typedef TypedSFNode< X3DChildNode > SFChildNode;

    /// Constructor.
    Scene( Inst< SFChildNode > _sceneRoot   = 0,
           Inst< MFWindow    > _window      = 0,
           Inst< SFFloat     > _frameRate   = 0
#ifdef HAVE_PROFILER
           ,
           Inst< MFString     > _profiledResult   = 0 
#endif
           );

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
#ifdef HAVE_PROFILER
    auto_ptr< MFString > profiledResult;
    
    Scene::profiledResultData H3D_scene_result;
    Scene::profiledResultData haptic_result;
    Scene::profiledResultData H3D_sofa_result;

    bool outputonce ;
    std::string generateProfileResult();
#endif
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

    typedef std::map < Node*, AutoRefVector<Node> > NodeParentsMap;
    typedef std::map < std::string,std::vector < std::string > > SearchFieldNameMap;

    /// Finds all the nodes of a given type (and optional name) by searching downwards from _node
    /// through all SFNode and MFNode fields.
    ///
    /// \param[in]  _node      The node to start searching down from
    /// \param[out] _result    The list of nodes found matching the search criteria. The type of this parameter
    ///                        is used to define the type of nodes to search for. Only nodes that can be dynamically
    ///                        cast to NodeType will be included in the results.
    /// \param[in]  _nodeName  The name of the node(s) to find. If not empty then only nodes matching this 
    ///                        name are found. If empty, then nodes of any name are found.
    /// \param[out] _parentMap A map from a found node to a list of the node's parents. If NULL then no
    ///                        information about the found node's parents is collected.
    /// \param[in]  _searchFieldNames A map from node type name, to a list of field names to search through for that
    ///                               node type. If NULL then all fields are searched.
    /// \param[in]  _verbose   If true, then output will be written to the Console to show the search path.
    /// \param[in]  _parent    The parent of this node. Used during recursion, should usually be left as NULL when
    ///                        called from user code.
    ///
    /// Examples:
    /// \code
    /// // Find all Shape nodes in a scene
    /// AutoRefVector<Shape> shapes;
    /// Scene::findNodes ( scene, shapes );
    ///
    /// // Find all Shape node with name "MyShape"
    /// AutoRefVector<Shape> shapes;
    /// Scene::findNodes ( scene, shapes, "MyShape" );
    ///
    /// // Find all Material nodes and their parents and display them
    ///  AutoRefVector<Material> materials;
    ///  Node::NodeParentsMap parents;
    ///  Scene::findNodes ( scene, materials, "", &parents );
    ///
    ///  for ( AutoRefVector<Material>::const_iterator i= materials.begin(); i != materials.end(); ++i ) {
    ///    Console(4) << *i << " " << (*i)->getName() << endl;
    ///    Console(4) << "Parents: " << endl;
    ///    const AutoRefVector<Node>& node_parents= parents[*i];
    ///    for ( AutoRefVector<Node>::const_iterator j= node_parents.begin(); j != node_parents.end(); ++j ) {
    ///      Console(4) << *j << " " << (*j)->getName() << endl;
    ///    }
    ///  }
    ///
    /// // Find all Material nodes, but only search through specific fields
    /// AutoRefVector<Material> materials;
    /// Node::SearchFieldNameMap search_fields;
    /// search_fields["Scene"].push_back ( "sceneRoot" );
    /// search_fields["Group"].push_back ( "children" );
    /// search_fields["Anchor"].push_back ( "children" );
    /// search_fields["Shape"].push_back ( "appearance" );
    /// search_fields["Appearance"].push_back ( "material" );
    /// Scene::findNodes ( scene, materials, "", NULL, &search_fields );
    /// \endcode
    ///
    template < typename NodeType >
    static void findNodes ( 
      Node& _node,
      AutoRefVector<NodeType>& _result, 
      const std::string& _nodeName= "",
      NodeParentsMap* _parentMap= NULL,
      SearchFieldNameMap* _searchFieldNames= NULL,
      bool _verbose= false,
      Node* _parent= NULL ) {

      if ( NodeType* n= dynamic_cast < NodeType* > ( &_node ) ) {
        if ( _nodeName.empty() || _nodeName == n->getName() ) {
          _result.push_back ( n );
          if ( _parentMap && _parent ) {
            (*_parentMap)[n].push_back ( _parent );
          }
        }
      }

      // get field names to search, if empty search all
      std::vector<std::string> field_names;
      if ( _searchFieldNames ) {
        SearchFieldNameMap::iterator i= _searchFieldNames->find ( _node.getTypeName() );
        if ( i != _searchFieldNames->end() ) {
          field_names= (*i).second;
        } else {
          // We should not search this node type at all
          return;
        }
      }

      // Iterate over all fields and recurse into children
      H3DNodeDatabase* db= H3DNodeDatabase::lookupTypeId( typeid( _node ) );

      for( H3DNodeDatabase::FieldDBConstIterator i = db->fieldDBBegin();
           db->fieldDBEnd() != i; i++ ) {
        Field *f = i.getField( &_node ); 
   
        if( f ) {
          if ( f->getAccessType() != Field::INPUT_ONLY ) {
            if ( _searchFieldNames ) {
              if ( find ( field_names.begin(), field_names.end(), f->getName() ) == field_names.end() ) {
                // This field should be skipped
                continue;
              }
            }

            if ( SFNode* sf_node= dynamic_cast< SFNode * >( f ) ) {
              Node* c= sf_node->getValue();
              if ( c ) {
                if ( _verbose ) {
                  Console(4) << "Node::findNodes(): " << getName() << " -> " << f->getName() << endl;
                }
                findNodes ( *c, _result, _nodeName, _parentMap, _searchFieldNames, _verbose, &_node );
              }
            } else if ( MFNode* mf_node= dynamic_cast< MFNode * >( f ) ) {
              const NodeVector& children= mf_node->getValue();
              for ( NodeVector::const_iterator i= children.begin(); i != children.end(); ++i ) {
                Node* c= *i;
                if ( c ) {
                  if ( _verbose ) {
                    Console(4) << "Node::findNodes(): " << getName() << " -> " << f->getName() << endl;
                  }
                  findNodes ( *c, _result, _nodeName, _parentMap, _searchFieldNames, _verbose, &_node );
                }
              }
            }
          }
        }
      }
    }
    
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
