//////////////////////////////////////////////////////////////////////////////
//    Copyright 2004-2007, SenseGraphics AB
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
/// \file X3DPrototypeInstance.h
/// \brief Header file for X3DPrototypeInstance, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __X3DPROTOTYPEINSTANCE_H__
#define __X3DPROTOTYPEINSTANCE_H__

#include <H3D/X3DNode.h>
#include <H3D/H3DDynamicFieldsObject.h>

// forward declaration for Bison VRML parser.
namespace yy {
  class VrmlParser;
  class location;
}
namespace H3D {

  // forward declaration.
  namespace X3D {
    class X3DSAX2Handlers;
  }

  /// \ingroup AbstractNodes
  /// \class X3DPrototypeInstance
  /// \brief This abstract node type is the base type for all prototype 
  /// instances in the X3D system. Any user-defined nodes declared with
  /// PROTO or EXTERNPROTO are instantiated using this base type. An
  /// X3DPrototypeInstance may be place anywhere in the scene graph where
  /// it is legal to place the first node declared within the prototype 
  /// instance. For example, if the base type of first node is 
  /// X3DAppearanceNode, that prototype may be instantiated anywhere in the
  /// scene graph that allows for an appearance node system. 
  ///
  class H3DAPI_API X3DPrototypeInstance: 
    public X3DNode,
    public H3DDynamicFieldsObject {
  public:

    /// Constructor.
    X3DPrototypeInstance( Inst< SFNode>  _metadata = 0 );
  
    /// Renders prototyped_node.
    virtual void render();

    /// Traverses prototyped_node.
    virtual void traverseSG( TraverseInfo &ti );
    
    /// Set the node that is the internal scenegraph of the prototype.
    virtual void setPrototypedNode( Node *n ) {
      prototyped_node.reset( n );
    }
    
    /// Get the node that is the internal scenegraph of the prototype.
    virtual Node *getPrototypedNode() {
      return prototyped_node.get();
    }

    /// Returns the default xml containerField attribute value.
    virtual string defaultXMLContainerField() {
      if( prototyped_node.get() )
        return prototyped_node->defaultXMLContainerField();
      else
        return X3DNode::defaultXMLContainerField();
    }

    /// Detect intersection between a line segment and the Node.
    /// \param from The start of the line segment.
    /// \param to The end of the line segment.
    /// \param result Contains info about the closest intersection for every
    /// object that intersects the line.
    /// \returns true if intersected, false otherwise.
    virtual bool lineIntersect( 
      const Vec3f &from, 
      const Vec3f &to,    
      LineIntersectResult &result ) {
      if( prototyped_node.get() )
        return prototyped_node->lineIntersect( from, to, result );
      else return false;
    }

    /// Find closest point on Node to p.
    /// \param p The point to find the closest point to.
    /// \param result A struct containing various results of closest
    /// points such as which geometries the closest points where
    /// detected on.
    virtual void closestPoint( const Vec3f &p,
                               NodeIntersectResult &result ){
      if( prototyped_node.get() )
        return prototyped_node->closestPoint( p, result );
    }

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
                                        NodeIntersectResult &result ) {
      if( prototyped_node.get() )
        return prototyped_node->movingSphereIntersect( radius,
                                                       from, to, result );
      else return false;
    }

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
    
    /// We need to make it possible for X3DSAX2Handlers to set up routes to fields
    /// withing the X3DPrototypeNode, so it must have access to the id member variable.
    friend class X3D::X3DSAX2Handlers;

    /// And the same for the VRML parser. Bison code handles routing,
    /// so yy::vrml_parser must be a friend:
    friend class yy::VrmlParser;
  protected:
    AutoRef< Node > prototyped_node;
  };
}

#endif
