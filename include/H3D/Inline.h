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
/// \file Inline.h
/// \brief Header file for Inline, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __INLINE_H__
#define __INLINE_H__

#include <H3D/X3DChildNode.h>
#include <H3D/X3DUrlObject.h>
#include <H3D/X3DBoundedObject.h>
#include <H3D/H3DDisplayListObject.h>
#include <H3D/Group.h>
#include <H3D/DEFNodes.h>

namespace H3D {

  /// \ingroup X3DNodes 
  /// \class Inline
  /// \brief The Inline node embeds an X3D scene stored at a location on
  /// the World Wide Web into the current scene. 
  ///
  /// The Inline node embeds an X3D scene stored at a location on the World
  /// Wide Web into the current scene. Exactly when the Inline scene is read
  /// and displayed is defined by the value of the load field, in profiles
  /// that support that field. In profiles that do not support the load
  /// field, exactly when the scene is read and displayed is not defined
  /// (e.g. reading the scene may be delayed until the Inline node's
  /// bounding box is visible to the viewer). Once the Inline scene is
  /// loaded, its children are added to the current scene and are treated as
  /// children of the Inline for rendering and interaction; however the
  /// children are not exposed to the current scene for routing and DEF name
  /// access unless their names have been explicitly imported into the scene
  /// using the IMPORT statement (see 4.4.6.2 IMPORT semantics). However this 
  /// can be changed using the importMode field.
  ///
  /// The importMode field defines the way that the Inline node exports its
  /// nodes and how they are imported into the scene that use the Inline node.
  ///
  /// - "DEFAULT" - Explicit EXPORT/IMPORT statements are needed as per X3D spec.
  /// - "AUTO" - All nodes with a DEF name will be exported and imported without
  /// the requirement of an explicit EXPORT/IMPORT statement.
  /// - "AUTO_IMPORT" - All nodes exported with the EXPORT statement will be 
  /// automatically imported into the scene that uses the Inline node.
  /// - "AUTO_EXPORT" - All nodes with a DEF name will be automatically exported.
  ///
  /// Each specified URL shall refer to a valid X3D file that contains a
  /// list of children nodes, prototypes and routes at the top level as
  /// described in 10.2.1 Grouping and children node types. The results are
  /// undefined if the URL refers to a file that is not an X3D file, or if
  /// the X3D file contains an invalid scene.
  ///
  /// It shall be an error to specify a file in the URL field that has a set
  /// of component definitions that is not a subset of the components of the
  /// containing world. In addition, the components shall not be of a higher
  /// support level than those used by the containing world, either
  /// implicitly or through the PROFILE declaration or additional COMPONENT
  /// statements. When an inlined world requests capabilities greater than
  /// its parent, the following actions shall occur:
  ///
  ///  - an error shall be generated, 
  ///  - the URL shall be treated as not interpretable as specified in
  ///    9.3.2 X3DUrlObject, and  
  ///  - the next URL shall be loaded and checked in accordance with 9.2
  ///    Concepts.  
  ///
  /// If the load field is set to TRUE (the default field value), the X3D
  /// file specified by the url field is loaded immediately. If the load
  /// field is set to FALSE, no action is taken. It is possible to
  /// explicitly load the URL at a later time by sending a TRUE event to the
  /// load field (e.g., as the result of a ProximitySensor or other sensor
  /// firing an event). If a FALSE event is sent to the load field of a
  /// previously loaded Inline, the contents of the Inline will be unloaded
  /// from the scene graph.
  ///
  /// An event sent to url can be used to change the scene that is inlined
  /// by the Inline node. If this value is set after the Inline is already
  /// loaded, its contents will be unloaded and the scene to which the new
  /// URL points will be loaded.
  ///
  /// The user is able to specify a bounding box for the Inline node using
  /// the bboxCenter and bboxSize fields. This is a hint to the browser and
  /// could be used for optimization purposes such as culling. 
  ///
  ///  
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/Switch.x3d">Switch.x3d</a>
  ///     ( <a href="examples/Switch.x3d.html">Source</a> )
  ///
  /// \par Internal routes:
  /// \dotfile Inline.dot
  class H3DAPI_API Inline : 
    public X3DChildNode,
    public X3DBoundedObject,
    public X3DUrlObject,
    public H3DDisplayListObject {
  public:

    /// The LoadedScene class updates itself from the load and url field
    /// and sets itself to the first url that parses successfully if the 
    /// load field is true, and to NULL otherwise.
    ///
    /// inputs[0] is the load field.
    /// inputs[1] is the url field.
    class H3DAPI_API LoadedScene: 
      public TypedField< DependentMFNode< Group,
                             FieldRef< H3DDisplayListObject,
                                       H3DDisplayListObject::DisplayList,
                                       &H3DDisplayListObject::displayList >,
                             true >,
                        Types< SFBool, MFString > > {
    public:
      /// Destructor. Clears the MFNode in order to get the correct onRemove 
      /// function to be called upon destruction. Must be done in all
      /// subclasses that override onRemove ().
      ~LoadedScene() {
        clear( ownerId() );
      }

    protected:
      typedef TypedField< DependentMFNode< Group,
                             FieldRef< H3DDisplayListObject,
                                       H3DDisplayListObject::DisplayList,
                                       &H3DDisplayListObject::displayList >,
                             true >,
                        Types< SFBool, MFString > > LoadedSceneBase;
      /// Update the group from the load and url field of the Inline node.
      virtual void update();

      /// Set up the routes we want to maintain from bound fields.
      virtual void onAdd( Node *n );

      /// Remove the routes we want to maintain from bound fields.
      virtual void onRemove( Node *n );
      
    };

    /// SFBound is specialized to update from the SFBound fields 
    /// routed to it. The resulting Bound object is the union of 
    /// the Bound objects routed to it. If the bboxSize of the 
    /// X3DGrouping node containing the SFBound field is ( -1, -1, -1 )
    /// bound fields of all Nodes in the children field of the containing
    /// Node that are instances of X3DBoundedObject are routed to it.
    /// Otherwise the bound will be a BoxBound with center and
    /// radius specified with the bboxCenter and bboxSize fields.
    ///
    class H3DAPI_API SFBound: 
      public TypedField< X3DBoundedObject::SFBound,
                         void,
                         AnyNumber< X3DBoundedObject::SFBound > > {
      /// The SFBound is updated to a bound that is the union of the 
      /// the Bound objects routed to it.
      virtual void update();
    };

    /// Constructor.
    Inline( Inst< SFNode      >  _metadata   = 0,
            Inst< DisplayList > _displayList = 0,
            Inst< MFString    >  _url        = 0,
            Inst< SFBound     > _bound       = 0,
            Inst< SFVec3f     >  _bboxCenter = 0,
            Inst< SFVec3f     >  _bboxSize   = 0,
            Inst< SFBool      >  _load       = 0,
            Inst< LoadedScene > _loadedScene = 0,
            Inst< SFString    > _importMode  = 0,
            Inst< SFBool      > _traverseOn  = 0 );

    virtual ~Inline() {
      exported_nodes.clear();
      DEF_nodes.clear();
    }

    /// Sets up the bound field using the bboxCenter and bboxSize fields.
    /// If bboxSize is (-1, -1, -1) the bound will be the union of all the
    /// bound objects of the Nodes in the children field. Otherwise it will
    /// be a BoxBound with center and origin determined by the bboxCenter
    /// and bboxOrigin fields.
    virtual void initialize() {
      const Vec3f &size = bboxSize->getValue();
      if( size.x == -1 && size.y == -1 && size.z == -1 ) {
        NodeVector children_nodes = loadedScene->getValue();
        use_union_bound = true;
        // set the values againg in order to run onNodeAdd again
        // with use_union_bound set to true to set the routes.
        loadedScene->setValue( children_nodes, id );
      } else {
        use_union_bound = false;
        BoxBound *bb = new BoxBound();
        bb->center->setValue( bboxCenter->getValue() );
        bb->size->setValue( bboxSize->getValue() );
        bound->setValue( bb );
      }
      X3DChildNode::initialize();
    }

    /// Render the inlined scenegraph, if loaded.
    virtual void render();

    /// Traverse the inline scenegraph, if loaded.
    virtual void traverseSG( TraverseInfo &ti );

    /// Detect intersection between a line segment and the Node.
    /// \param from The start of the line segment.
    /// \param to The end of the line segment.
    /// \param result Contains info about the closest intersection for every
    /// object that intersects the line.
    /// \returns true if intersected, false otherwise.
    virtual bool lineIntersect( const Vec3f &from, 
                                const Vec3f &to,    
                                LineIntersectResult &result );

    /// Find closest point on Node to p.
    /// \param p The point to find the closest point to.
    /// \param result A struct containing various results of closest
    /// points such as which geometries the closest points where
    /// detected on.
    virtual void closestPoint( const Vec3f &p,
                               NodeIntersectResult &result );

    /// Detect collision between a moving sphere and the Node.
    /// Only nodes to which collision is possible will return true
    /// \param radius The radius of the sphere
    /// \param from The start position of the sphere
    /// \param to The end position of the sphere.
    /// \param result A struct containing various results of intersections
    /// such as which geometries intersected the moving sphere.
    /// \returns true if intersected, false otherwise.
    virtual bool movingSphereIntersect( H3DFloat radius,
                                        const Vec3f &from, 
                                        const Vec3f &to,
                                        NodeIntersectResult &result );

    /// If the load field is set to TRUE (the default field value), 
    /// the X3D file specified by the url field is loaded immediately. 
    /// If the load field is set to FALSE, no action is taken.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> true \n
    /// \dotfile Inline_load.dot
    auto_ptr< SFBool >  load;
    
    /// The scene that is currently loaded by the inline node.
    ///
    /// <b>Access type:</b> outputOnly \n
    /// \dotfile Inline_loadedScene.dot
    auto_ptr< LoadedScene > loadedScene;

    /// The autoImport mode defines the way that the Inline node exports its
    /// nodes and how they are imported into the scene that use the Inline node.
    ///
    /// - "DEFAULT" - Explicit EXPORT/IMPORT statements are needed as per X3D spec.
    /// - "AUTO" - All nodes with a DEF name will be exported and imported without
    /// the requirement of an explicit EXPORT/IMPORT statement.
    /// - "AUTO_IMPORT" - All nodes exported with the EXPORT statement will be 
    /// automatically imported into the scene that uses the Inline node.
    /// - "AUTO_EXPORT" - All nodes with a DEF name will be automatically exported.
    ///
    /// <b>Access type:</b> initializeOnly \n
    /// <b>Default value:</b> "DEFAULT" \n
    /// <b>Valid values:</b> "DEFAULT", "AUTO", "AUTO_IMPORT", "AUTO_EXPORT" \n
    /// \dotfile Inline_importMode.dot
    auto_ptr< SFString > importMode;

    /// If set to false the children nodes of the Inline node will not be traversed
    /// at all, basically making them not part of the scene graph unless specific 
    /// nodes are used elsewhere with a USE statement.
    ///
    /// <b>Access type:</b> initializeOnly \n
    /// <b>Default value:</b> true \n
    /// \dotfile Inline_traverseOn.dot
    auto_ptr< SFBool > traverseOn;

    // if true a route will be set up between the bound field of the
    // loadedScene field and the bound field of the inline node. 
    bool use_union_bound;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;

    /// A DEFNodes structure from the name of nodes exported with the EXPORT statement
    /// in the url of the Inline node, to the actual node.
    X3D::DEFNodes exported_nodes;

    /// A DEFNodes structure from the name of nodes named with the DEF statement
    /// in the url of the Inline node, to the actual node.
    X3D::DEFNodes DEF_nodes;
  };
}

#endif
