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
/// \file HumanHand.h
/// \brief Header file for HumanHand.
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __HUMANHANDH3D_H__
#define __HUMANHANDH3D_H__

// H3D includes
#include <H3D/X3DGeometryNode.h>
#include <H3D/CyberGloveSensor.h>
#include <H3D/DependentNodeFields.h>

#ifdef HAVE_VIRTUAL_HAND_SDK
// Virtual Hand SDK includes
#include <vhandtk/vhtHumanHand.h>
#include <vhandtk/vhtOglDrawer.h>
#endif

namespace H3D {

  /// \ingroup H3DNodes
  /// \class HumanHand
  /// \brief This is a X3DGeometryNode which renders a hand using the Virtual
  /// Hand SDK.
  ///
  /// The use of this node is to render the hand from a CyberGlove in H3DAPI.
  /// A correctly installed CyberGlove is needed and the Device Manager have to
  /// be started and a default configuration has to be set. For some reason the
  /// geometry can not be scaled properly if the scaling of a parent transform
  /// is different from 1 (see the VHS Programmers Guide v2.5 ). Therefore the
  /// scaling of a parent transform will be completely ignored. Scaling can
  /// therefore only be set at initialization through the scale field.
  ///
  /// Note: collision functions does nothing or returns false.
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/HumanHand.x3d">HumanHand.x3d</a>
  ///     ( <a href="examples/HumanHand.x3d.html">Source</a> )
  ///
  /// \par Internal routes:
  /// \dotfile HumanHand.dot
  // TODO: Ask virtual hand SDK people if the bound of the default hand
  // geometry can somehow be obtained, it does not seem like it.
  class H3DAPI_API HumanHand : public X3DGeometryNode {
  public:

    /// Field class used to check if a CyberGloveSensor is used and then
    /// set up routes from fields in the CyberGloveSensor to correctly redraw
    /// when hand changes.
    class SFCyberGloveSensor: public TypedSFNode< CyberGloveSensor > {
    protected:
      virtual void onAdd( Node *node ) {
        TypedSFNode< CyberGloveSensor >::onAdd( node );
        if( node ) {
          CyberGloveSensor * cgs = static_cast< CyberGloveSensor * >(node);
          HumanHand *hh = static_cast< HumanHand * >(owner);
          cgs->thumb->route( hh->displayList );
          cgs->index->route( hh->displayList );
          cgs->middle->route( hh->displayList );
          cgs->ring->route( hh->displayList );
          cgs->pinky->route( hh->displayList );
        }
      }

      virtual void onRemove( Node *node ) {
        if( node ) {
          CyberGloveSensor * cgs = static_cast< CyberGloveSensor * >(node);
          HumanHand *hh = static_cast< HumanHand * >(owner);
          cgs->thumb->unroute( hh->displayList );
          cgs->index->unroute( hh->displayList );
          cgs->middle->unroute( hh->displayList );
          cgs->ring->unroute( hh->displayList );
          cgs->pinky->unroute( hh->displayList );
        }
        TypedSFNode< CyberGloveSensor >::onRemove( node );
      }
    };

    /// Update geometric bounds of the HumanHand.
    /// routes_in[0] - The cyberGlove field of HumanHand.
    /// routes_in[1] - The scale field of HumanHand.
    /// routes_in[2] - The C++ only sceneGraphScaling field of HumanHand.
    class SFBound: public TypedField< X3DGeometryNode::SFBound,
                                      Types< SFCyberGloveSensor,
                                             SFFloat, SFVec3f > > {
    protected:
      /// Update the bound from the size field. 
      virtual void update() {
        Vec3f b;
        if( static_cast< SFCyberGloveSensor * >( routes_in[0] )->getValue() ) {
          Vec3f scene_graph_scale =
            static_cast< SFVec3f * >(routes_in[2])->getValue();
          // The units of the virtual hand is centimeters, so a bounding box of
          // about 30 centimeters in every direction should enclose the hand.
          // The scale will scale down the hand to the correct size, by default
          // the scale is 0.01 in order to translate into meters.
          if( scene_graph_scale.length() > Constants::f_epsilon ) {
            H3DFloat scale_field =
              static_cast< SFFloat * >(routes_in[1])->getValue();
            b = Vec3f( 35, 35, 35 ) * scale_field;
            b.x *= 1/scene_graph_scale.x;
            b.y *= 1/scene_graph_scale.y;
            b.z *= 1/scene_graph_scale.z;
          }
        }
        BoxBound *bb = new BoxBound;
        bb->size->setValue( b );
        value = bb;
      }
    };

    /// Constructor.
    HumanHand( Inst< SFNode             > _metadata        = 0,
               Inst< SFBound            > _bound           = 0,
               Inst< DisplayList        > _displayList     = 0,
               Inst< SFCyberGloveSensor > _cyberGlove      = 0,
               Inst< SFString           > _gloveHandedness = 0,
               Inst< SFFloat            > _scale           = 0 );

    /// Destructor
    ~HumanHand() {
      CyberGloveSensor * cyber_glove = cyberGlove->getValue();
      if( cyber_glove )
        cyber_glove->resetGlove();
#ifdef HAVE_VIRTUAL_HAND_SDK
      drawer.reset( 0 );
      if( hand.get() )
        hand->disconnect();
      hand.reset( 0 );
#endif
    }

    /// Initializes auto_ptrs to Virtual Hand SDK.
    virtual void initialize();

    // render the hand.
    virtual void render();

    // updates the hand data.
    virtual void traverseSG( TraverseInfo &ti );

    /// The number of triangles renderered in this geometry.
    /// This is simply an estimate since the exact number is unknown at
    /// the moment.
    virtual int nrTriangles() {
      return 2000;
    }

    /// Detect intersection between a line segment and the HumanHands bounding
    /// box.
    /// \param from The start of the line segment.
    /// \param to The end of the line segment.
    /// \param result Contains info about the closest intersection for every
    /// object that intersects the line.
    /// \returns true if intersected, false otherwise.
    virtual bool lineIntersect( const Vec3f &from,
                                const Vec3f &to,
                                LineIntersectResult &result ) { return false; }

    /// Find closest point on the HumanHands bounding box to point p.
    /// \param p The point to find the closest point to.
    /// \param result A struct containing various results of closest
    /// points such as which geometries the closest points where
    /// detected on.
    virtual void closestPoint( const Vec3f &p,
                               NodeIntersectResult &result ) {}

    /// Detect collision between a moving sphere and the HumanHands bounding
    /// box.
    /// \param radius The radius of the sphere
    /// \param from The start position of the sphere
    /// \param to The end position of the sphere.
    /// \param result A struct containing various results of intersections
    /// such as which geometries intersected the moving sphere.
    /// \returns true if intersected, false otherwise.
    virtual bool movingSphereIntersect( H3DFloat radius,
                                        const Vec3f &from, 
                                        const Vec3f &to,
                                        NodeIntersectResult &result )
    { return false; }

    /// An instance of the cyberGlove
    ///
    /// <b>Access type: </b> initializeOnly \n
    /// \dotfile HumanHand_cyberGlove.dot
    auto_ptr< SFCyberGloveSensor> cyberGlove;

    /// Decides whether the CyberGlove is a left or right hand.
    ///
    /// <b>Access type: </b> initializeOnly \n
    /// <b>Valid values: </b> leftHand, rightHand \n
    /// \dotfile HumanHand_gloveHandedness.dot
    auto_ptr< SFString> gloveHandedness;

    /// The default hand geometrys units in the Virtual Hand SDK
    /// seems to be in centimeters. This scales from meters to centimeters.
    /// If a custom made hand geometry is used change the scale if needed.
    ///
    /// <b>Access type: </b> initializeOnly \n
    /// <b>Default value: </b> 0.01 \n
    /// \dotfile HumanHand_scale.dot
    auto_ptr< SFFloat > scale;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;

  private:
#ifdef HAVE_VIRTUAL_HAND_SDK
    // VHSDK classes
    auto_ptr< vhtHumanHand > hand;
    auto_ptr< vhtOglDrawer > drawer;
    vhtTransform3D * cameraXForm;
#endif

    // Internal field used to modify the bound. Since scaling does not work
    // when drawing the hand because of limitation in Virtual Hand SDK this
    // field is needed, it simply stores the x,y,z OpenGL scaling.
    auto_ptr< SFVec3f > sceneGraphScaling;
  };
}

#endif
