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
/// \file Anchor.h
/// \brief Header file for Anchor, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __ANCHOR_H__
#define __ANCHOR_H__

#include <H3D/X3DGroupingNode.h>
#include <H3D/X3DViewpointNode.h>
#include <H3D/MFString.h>
#include <H3D/PeriodicUpdate.h>
#include <H3D/Scene.h>

namespace H3D {

  /// \ingroup X3DNodes
  /// \class Anchor
  /// \brief The Anchor grouping node retrieves the content of a URL when the
  /// user activates (e.g., clicks) some geometry contained within the Anchor
  /// node's children. If the URL points to a valid X3D file, that world
  /// replaces the world of which the Anchor node is a part (except when the
  /// parameter field, described below, alters this behaviour). If non-X3D data
  /// is retrieved, the browser shall determine how to handle that data;
  /// typically, it will be passed to an appropriate non-X3D browser.
  ///
  /// Exactly how a user activates geometry contained by the Anchor node
  /// depends on the pointing device and is determined by the X3D browser.
  /// Typically, clicking with the pointing device will result in the new scene
  /// replacing the current scene. An Anchor node with an empty url does
  /// nothing when its children are chosen. For a description of how multiple
  /// Anchors and pointing-device sensors are resolved on activation see the
  /// Pointing Device Sensor Component in the X3D specification.
  ///
  /// The description field in the Anchor node specifies a textual description
  /// of the Anchor node. This may be used by browser-specific user interfaces
  /// that wish to present users with more detailed information about the
  /// Anchor.
  ///
  /// The parameter  field may be used to supply any additional information to
  /// be interpreted by the browser. Each string shall consist of
  /// "keyword=value" pairs. For example, some browsers allow the specification
  /// of a "target" for a link to display a link in another part of an HTML
  /// document. The parameter field is then:
  ///
  /// Anchor { \n
  ///  parameter [ "target=name_of_frame" ]; \n
  ///  ... \n
  /// }\n
  ///
  /// An Anchor node may be used to bind the initial Viewpoint node in a world
  /// by specifying a URL ending with "#ViewpointName" where "ViewpointName" is
  /// the DEF name of a viewpoint defined in the X3D file.
  ///
  /// EXAMPLE \n
  ///
  /// Anchor { \n
  ///   url "http://www.school.edu/X3D/someScene.wrl#OverView";\n
  ///     children  Shape { geometry Box {} };\n
  /// }\n
  ///
  /// specifies an anchor that loads the X3D file "someScene.wrl" and binds the
  /// initial user view to the Viewpoint node named "OverView" when the Anchor
  /// node's geometry (Box) is activated. If the named Viewpoint node is not
  /// found in the X3D file, the X3D file is loaded using the default Viewpoint
  /// node binding stack rules.
  ///
  /// If the url field is specified in the form "#ViewpointName"
  /// (i.e., no file name), the Viewpoint node with the given name
  /// ("ViewpointName") in the Anchor's run-time name scope(s) shall be bound
  /// (set_bind TRUE). The results are undefined if there are multiple
  /// Viewpoints with the same name in the Anchor's run-time name scope(s).
  /// The results are undefined if the Anchor node is not part of any run-time
  /// name scope or is part of more than one run-time name scope.
  /// ( RUN-TIME scope not implemented, if the name exist the first created
  /// X3DViewpoint with the correct name will be used )
  ///  
  /// For example:
  ///
  /// Anchor { \n
  ///   url "#Doorway"; \n
  ///   children Shape { geometry Sphere {} }; \n
  /// } \n
  ///
  /// binds the viewer to the viewpoint defined by the "Doorway" viewpoint in
  /// the current world when the sphere is activated. In this case, if the
  /// Viewpoint is not found, no action occurs on activation.
  /// NOTE: This node is not fully tested and may crash now and then when used.
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/Anchor.x3d">Anchor.x3d</a>
  ///     ( <a href="examples/Anchor.x3d.html">Source</a> )
  class H3DAPI_API Anchor : public X3DGroupingNode {
  public:

    /// Takes care of changing the scene when an object in the Anchor is
    /// selected (by clicking on it with the mouse).
    class H3DAPI_API GeometrySelected: public PeriodicUpdate < SFBool > {
    protected:
      // Check the url and call replaceScene if the url is correct.
      virtual void update();

    };
#ifdef __BORLANDC__
    friend class GeometrySelected;
#endif

    /// Constructor.
    Anchor( Inst< AddChildren    > _addChildren    = 0,
            Inst< RemoveChildren > _removeChildren = 0,
            Inst< MFChild        > _children       = 0,
            Inst< SFString       > _description    = 0,
            Inst< SFNode         > _metadata       = 0,
            Inst< SFBound        > _bound          = 0,
            Inst< MFString       > _parameter      = 0,
            Inst< MFString       > _url            = 0,
            Inst< SFVec3f        > _bboxCenter     = 0,
            Inst< SFVec3f        > _bboxSize       = 0 );

    ~Anchor();

    /// Replaces the world in the scene with a new one.
    /// \param new_world The top node of the new world.
    /// \param new_vp The viewpoint to use after changing the world.
    /// \param the_anchor The Anchor node causing the call to this function.
    static void replaceScene( AutoRef< Node > new_world,
                              const X3DViewpointNode *new_vp,
                              const Anchor *the_anchor );

    /// Replaces the world in the scene with a new one. Uses the static
    /// protected parameters new_scene_root, old_anchor, new_world_vp set
    /// internally by the Anchor. Used in order to replace the sceneRoot
    /// from Scene::idle instead of the routing network since calling it
    /// from the routing network might cause corruption in heap.
    static void replaceSceneRoot( Scene * the_scene );

    /// Checks which Scenes the given anchor is part of.
    /// \param group_node The X3DGroupingNode to traverse in order to find
    /// the anchor.
    /// \param anchor_to_find The anchor that is tested for inclusion in the
    /// scene defined by the group_node.
    /// \returns true if the anchor is found in the group_node, false
    /// otherwise.
    static bool isAnchorInScene( const X3DGroupingNode *group_node,
                                 const Anchor *anchor_to_find );

    /// The description field in the Anchor node specifies a textual
    /// description of the Anchor node.
    ///
    /// <b>Access type:</b> inputOutput
    /// 
    /// \dotfile Anchor_description.dot
    auto_ptr< SFString >  description;

    /// The parameter  field may be used to supply any additional information
    /// to be interpreted by the browser. Each string shall consist of
    /// "keyword=value" pairs.
    ///
    /// <b>Access type:</b> inputOutput
    /// 
    /// \dotfile Anchor_parameter.dot
    auto_ptr< MFString >  parameter;

    /// Contains path to file (or name of Viewpoint) used when clicking on a
    /// geometry in the children field.
    ///
    /// <b>Access type:</b> inputOutput
    /// 
    /// \dotfile Anchor_url.dot
    auto_ptr< MFString >  url;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  protected:
    // Internal variables used to control when to replace the world
    // ( or change viewpoint ).
    auto_ptr< GeometrySelected > on_click;
    auto_ptr< X3DPointingDeviceSensorNode > intern_pdsn;
    static AutoRef< Anchor > old_anchor;
    static string new_world_url;
    static string new_world_vp;
    static string new_world_base_url;
    string internal_base_url;
  };
}

#endif
