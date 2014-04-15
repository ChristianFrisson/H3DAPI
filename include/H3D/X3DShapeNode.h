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
/// \file X3DShapeNode.h
/// \brief Header file for X3DShapeNode, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __X3DSHAPENODE_H__
#define __X3DSHAPENODE_H__

#include <H3D/X3DChildNode.h>
#include <H3D/X3DBoundedObject.h>
#include <H3D/X3DAppearanceNode.h>
#include <H3D/X3DGeometryNode.h>
#include <H3D/DependentNodeFields.h>
#include <H3D/H3DDisplayListObject.h>
#include <H3D/H3DShadowObjectNode.h>

namespace H3D {
  /// \ingroup AbstractNodes
  /// \class X3DShapeNode
  /// This is the base node type for all Shape nodes.
  ///
  /// \par Internal routes:
  /// \dotfile X3DShapeNode.dot
  class H3DAPI_API X3DShapeNode : 
    public X3DChildNode,
    public X3DBoundedObject,
    public H3DDisplayListObject {
  public:
    /// SFAppearanceNode is dependent on the displayList field of its
    /// encapsulated X3DAppearanceNode node, i.e. an event from that
    /// field will trigger an event from the SFAppearanceNode as well.
    typedef DependentSFNode< X3DAppearanceNode,
                             FieldRef<H3DDisplayListObject, 
                                      H3DDisplayListObject::DisplayList,
                                      &H3DDisplayListObject::displayList >, 
                             true > 
    SFAppearanceNode;
        
    /// SFHapticGeometry contains a X3DGeometryNode.
    typedef TypedSFNode< X3DGeometryNode > SFHapticGeometry;
 
    /// SFGeometryNode is dependent on the displayList field of its
    /// encapsulated X3DGeometryNode node, i.e. an event from that
    /// field will trigger an event from the SFGeometryNode as
    /// well. Also we ovverride onAdd and onRemove in order to uphold
    /// a route between the encapsulated X3DGeometryNode's bound field
    /// to the bound field of the X3DShapeNode the field resides in.
    ///
    class SFGeometryNode: 
      public DependentSFNode< X3DGeometryNode, 
      FieldRef< H3DDisplayListObject,
      H3DDisplayListObject::DisplayList,
      &H3DDisplayListObject::displayList >, true > {
        typedef DependentSFNode< X3DGeometryNode, 
        FieldRef< H3DDisplayListObject,
                  H3DDisplayListObject::DisplayList,
                  &H3DDisplayListObject::displayList >, true > BaseField;
    public:
    
      /// Constructor.
      SFGeometryNode(): 
        owner( NULL ) {}

      /// Destructor. Sets the value to NULL in order to get the correct
      /// onAdd and onRemove functions to be called. 
      ~SFGeometryNode() {
        value = NULL;
      }

      /// This function will be called when the value of RefCountField
      /// changes. As soon as the value changes onRemove will
      /// be called on the old value and onAdd will be called
      /// on the new value. 
      /// \param n The new value.
      ///
      virtual void onAdd( Node *n ) {
        BaseField::onAdd( n );
        if( n && owner->use_geometry_bound ) {
          X3DGeometryNode *g = static_cast< X3DGeometryNode * >( n );
          g->bound->route( owner->bound );
        }
      }
      
      /// This function will be called when the value of RefCountField
      /// changes. As soon as the value changes onRemove will
      /// be called on the old value and onAdd will be called
      /// on the new value. 
      /// \param n The old value.
      ///
      virtual void onRemove( Node *n ) {
        BaseField::onRemove( n );
        if( n && owner->use_geometry_bound ) {
          X3DGeometryNode *g = static_cast< X3DGeometryNode * >( n );
          g->bound->unroute( owner->bound );
        }
      }

      // the shape node that the instance of the SFGeometry field
      // is in.
      X3DShapeNode *owner;

    };

    /// This is just a dummy class to get around a bug in Visual C++ 7.1
    /// If the X3DGeometry::DisplayList inherits directly from 
    /// H3DDisplayListObject::Display list the application will crash
    /// if trying to call H3DDisplayListObject::DisplayList::callList
    /// By using an intermediate class the bug dissappears.
    class H3DAPI_API BugWorkaroundDisplayList: 
      public H3DDisplayListObject::DisplayList {
      /// Returns true if we have a valid display list built that can
      /// be called. We have to override this method here since the 
      /// render() function of the X3DShapeNode have the possibility
      /// to break the cache. The display list can then only be considered
      /// valid if we know that the cache will not be broken. 
      virtual bool haveValidDisplayList();
    };

    /// Display list is extended in 
    class H3DAPI_API DisplayList: public BugWorkaroundDisplayList {
    public: 
      /// Perform front face code outside the display list.
      virtual void callList( bool build_list = true );
    };


    typedef TypedSFNode< H3DShadowObjectNode > SFShadowObjectNode;  

    /// Constructor.
    X3DShapeNode( Inst< SFAppearanceNode > _appearance     = 0,
                  Inst< SFGeometryNode   > _geometry       = 0,
                  Inst< SFHapticGeometry > _hapticGeometry = 0,
                  Inst< SFNode           > _metadata       = 0,
                  Inst< SFBound          > _bound          = 0,
                  Inst< SFVec3f          > _bboxCenter     = 0,
                  Inst< SFVec3f          > _bboxSize       = 0,
                  Inst< DisplayList      > _displayList    = 0
                  );

    /// Sets up the bound field using the bboxCenter and bboxSize fields.
    /// If bboxSize is (-1, -1, -1) the bound will be the bound of the
    /// geometry field. Otherwise it will be a BoxBound with center
    /// and origin determined by the bboxCenter and bboxOrigin fields.
    virtual void initialize() {
      const Vec3f &size = bboxSize->getValue();
      if( size.x == -1 && size.y == -1 && size.z == -1 ) {
        use_geometry_bound = true;
        X3DGeometryNode *g = 
          static_cast< X3DGeometryNode * >( geometry->getValue() );
        if( g ) {
          g->bound->route( this->bound );
        }
      } else {
        BoxBound *bb = new BoxBound();
        bb->center->setValue( bboxCenter->getValue() );
        bb->size->setValue( bboxSize->getValue() );
        bound->setValue( bb );
      }
      X3DChildNode::initialize();
    }

    /// Render the shape using OpenGL.
    virtual void render();

    /// Traverse the scenegraph. Calls traverseSG on appeance and geometry.
    virtual void traverseSG( TraverseInfo &ti );

    /// Detect intersection between a line segment and the X3DGeometryNode
    /// in the field geometry.
    /// \param from The start of the line segment.
    /// \param to The end of the line segment.
    /// \param result Contains info about the closest intersection for every
    /// object that intersects the line.
    /// \returns true if intersected, false otherwise.
    virtual bool lineIntersect(
      const Vec3f &from,
      const Vec3f &to,
      LineIntersectResult &result );

    /// Find closest point on the node in the field geometry.
    /// \param p The point to find the closest point to.
    /// \param result A struct containing various results of closest
    /// points such as which geometries the closest points where
    /// detected on.
    virtual void closestPoint( const Vec3f &p,
                               NodeIntersectResult &result );

    /// Detect collision between a moving sphere and the geometry
    /// in the geometry field.
    /// \param radius The radius of the sphere
    /// \param from The start position of the sphere
    /// \param to The end position of the sphere.
    /// \returns true if intersected, false otherwise.
    /// \param result A struct containing various results of intersections
    /// such as which geometries intersected the moving sphere.
    virtual bool movingSphereIntersect( H3DFloat radius,
                                        const Vec3f &from, 
                                        const Vec3f &to,
                                        NodeIntersectResult &result );

    /// Tells which type of geometry should be rendered.
    typedef enum {
      /// render only transparent objects
      TRANSPARENT_ONLY,
      /// render only the front face of transparent objects
      TRANSPARENT_FRONT,
      /// render only the back face of transparent objects
      TRANSPARENT_BACK,
      /// render only non-transparent objects
      SOLID,
      /// render all objects
      ALL
    } GeometryRenderMode;

    static GeometryRenderMode geometry_render_mode;

    /// The field containing the X3DAppearance node to be used when
    /// rendering the shape.
    /// 
    /// <b>Access type:</b> inputOutput
    /// 
    /// \dotfile X3DShapeNode_appearance.dot
    auto_ptr<    SFAppearanceNode  >  appearance;

    /// Contains the X3DGeometryNode to be rendered.
    /// 
    /// <b>Access type:</b> inputOutput
    /// 
    /// \dotfile X3DShapeNode_geometry.dot
    auto_ptr<    SFGeometryNode  >  geometry;

    /// If specified, contains a X3DGeometryNode to be rendered haptically.
    /// If NULL the geometry in the geometry field will be used.
    /// 
    /// <b>Access type:</b> inputOutput
    /// 
    /// \dotfile X3DShapeNode_hapticGeometry.dot
    auto_ptr<    SFHapticGeometry  >  hapticGeometry;

    /// If specified, contains a H3DShadowObjectNode to be used for
    /// shadows.
    /// If NULL the standard shadow volume of the geometry 
    /// will be used.
    /// 
    /// <b>Access type:</b> inputOutput
    /// 
    /// \dotfile X3DShapeNode_shadowVolume.dot
    auto_ptr<    SFShadowObjectNode >  shadowVolume;

    // if true a route will be set up between the bound field of the
    // geometry node in this field and the bound field of the shape. 
    bool use_geometry_bound;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;

    /// Set to true if lighting should be disabled when no Appearance
    /// or Material node has been specified as per X3D spec. Will be set
    /// to false in e.g. H3DWindowNode if a default Material node has 
    /// been specified in a DefaultAppearance option node.
    static bool disable_lighting_if_no_app;
  protected:

    // Adress of traverseInfo 
    // only interested in adress, what it points to will be invalid.
    // It is needed to correctly set traverse_multipass_transparency
    // if a X3DShapeNode is used in several places in the scene graph.
    TraverseInfo *prev_travinfoadr;
    // If the node is in several places in the scene graph it might
    // use different values for setting multipass transparency in
    // traverseSG. The last one is the one that will be used. This
    // stores values during a traverseSG pass and "or" it with
    // new values. If any of the pass returns true then multipass
    // transparency should be used.
    bool traverse_multipass_transparency;

    // The GlobalSettings node that was in used in last call to 
    // traverseSG
    AutoRef<Node> last_global_settings;
  };
}

#endif
