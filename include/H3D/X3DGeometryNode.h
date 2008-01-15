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
/// \file X3DGeometryNode.h
/// \brief Header file for X3DGeometryNode, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __X3DGEOMETRYNODE_H__
#define __X3DGEOMETRYNODE_H__

// H3Dapi includes
#include <H3D/X3DChildNode.h>
#include <H3D/H3DBoundedObject.h>
#include <H3D/H3DDisplayListObject.h>
#include <H3D/MFBool.h>
#include <H3D/MFVec3f.h>
#include <H3D/H3DOptionNode.h>
#include <H3D/MFNode.h>
#include <H3D/X3DTextureCoordinateNode.h>
#include <H3D/OpenHapticsOptions.h>

// HAPI includes
#include <HAPI/HAPIGLShape.h>
#include <HAPI/HapticTriangleSet.h>

// H3DUtil includes
#include <H3DUtil/AutoPtrVector.h>

namespace H3D {

  /// \ingroup AbstractNodes
  /// \class X3DGeometryNode
  /// This is the base node type for all geometry in X3D. 
  class H3DAPI_API X3DGeometryNode : 
    public X3DChildNode,
    public H3DBoundedObject,
    public H3DDisplayListObject,
    public HAPI::HAPIGLShape {
  public:
    typedef TypedMFNode< H3DOptionNode > MFOptionsNode;

    /// This is just a dummy class to get around a bug in Visual C++ 7.1
    /// If the X3DGeometry::DisplayList inherits directly from 
    /// H3DDisplayListObject::Display list the application will crash
    /// if trying to call H3DDisplayListObject::DisplayList::callList
    /// By using an intermediate class the bug dissappears.
    class H3DAPI_API BugWorkaroundDisplayList: 
      public H3DDisplayListObject::DisplayList {
    };

    /// Display list is extended in order to set front sidedness of 
    /// triangles outside the display list. 
    class H3DAPI_API DisplayList: public BugWorkaroundDisplayList {
    public:
      /// Perform front face code outside the display list.
      virtual void callList( bool build_list = true );
    };

    /// The HAPIBoundTree constructs a 
    class H3DAPI_API SFBoundTree: 
      public RefCountSField< HAPI::Collision::BinaryBoundTree > {
      virtual void update();
    };

    /// Constructor.
    X3DGeometryNode( Inst< SFNode      >  _metadata = 0,
                     Inst< SFBound     > _bound = 0,
                     Inst< DisplayList > _displayList = 0,
                     Inst< MFBool      > _isTouched = 0,
                     Inst< MFVec3f     > _force = 0,
                     Inst< MFVec3f     > _contactPoint = 0,
                     Inst< MFVec3f     > _contactNormal = 0,
                     Inst< MFVec3f     > _contactTexCoord = 0,
                     Inst< SFBoundTree > _boundTree = 0 );

    virtual void traverseSG( TraverseInfo &ti );

    /// Detect intersection between a line segment and this geometry.
    /// \param from The start of the line segment.
    /// \param to The end of the line segment.
    /// \param result Contains info about the closest intersection for every
    /// object that intersects the line.
    /// \returns true if intersected, false otherwise.
    virtual bool lineIntersect( 
      const Vec3f &from,
      const Vec3f &to,    
      LineIntersectResult &result );

    /// Find closest point on this geometry to point p.
    /// \param p The point to find the closest point to.
    /// \param closest_point Return parameter for each closest point
    /// \param normal Return parameter for normal at each closest point.
    /// \param tex_coord Return paramater for each texture coordinate at
    /// closest point
    virtual void closestPoint( const Vec3f &p,
                               vector< Vec3f > &closest_point,
                               vector< Vec3f > &normal,
                               vector< Vec3f > &tex_coord );

    /// Detect collision between a moving sphere and the geometry.
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

    /// Function overridden from HAPIGLShape. Just call the 
    /// displayList->callList per default
    virtual void glRender() {
      bool previous_allow = allowingCulling();
      bool prev_draw = draw_debug_options;
      allowCulling( false );
      draw_debug_options = false;
      displayList->callList( false );
      draw_debug_options = prev_draw;
      allowCulling( previous_allow );
    }

    /// This function should be used by the render() function to disable
    /// or enable face culling. DO NOT USE glEnable/glDisable to do
    /// this, since it will cause problems with OpenHaptics.
    inline void useCulling( bool enabled ) {
      use_culling = enabled;
    }

    /// Returns if face culling is in use or not.
    inline bool usingCulling() {
      return use_culling;
    }

    /// Control if face culling is allowed or not. Used when rendering
    /// HLFeedbackBuffer or HLDepthBuffer shapes in order not to have 
    /// back face culling on when rendering shapes with OpenHaptics.
    inline void allowCulling( bool allow ) {
      allow_culling = allow;
    } 

    /// Enabling/disabling back face culling. Same as doing  
    /// useCulling( enabled ); setCullFace( GL_BACK );
    inline void useBackFaceCulling( bool enabled ) {
      useCulling( enabled );
      setCullFace( GL_BACK );
    }

    /// Returns true if back face culling is allowed, false otherwise.
    inline bool allowingCulling() {
      return allow_culling;
    }

    /// Set which side of a polygon to cull. Valid values are GL_FRONT
    /// or GL_BACK
    inline void setCullFace( GLenum face ) {
      cull_face = face;
    }

    /// Get which face will be culled if culling is enabled.
    inline GLenum getCullFace() {
      return cull_face;
    }

    /// Get the first option node of the type of the pointer given as argument
    /// from the renderOptions fieeld
    /// The option argument will contain the node afterwards, or NULL if no
    /// option of that type exists.
    template< class OptionNodeType >
    void getOptionNode( OptionNodeType * &option ) {
       for( MFOptionsNode::const_iterator i = options->begin();
           i != options->end(); i++ ) {
        OptionNodeType *options = dynamic_cast< OptionNodeType * >( *i );
        if( options ) {
          option = options;
          return;
        }
       }
      option = NULL;
    }

    /// Get a shape id to be used for rendering of this geometry with HLAPI for
    /// the given haptics device.
    /// Since the geometry can appear in several places in the scene graph
    /// it can contain several shape ids (one for each place). Which one to
    /// get is determined by the index argument.
    int getHapticShapeId( unsigned int index );

 #ifdef HAVE_OPENHAPTICS   
    /// Destructor. Deletes the hl_shape_id.
    virtual ~X3DGeometryNode();
 
    /// Returns a either a HLFeedbackShape or a HLDepthBufferShape with
    /// the X3DGeometryNode. Which type depents on possible 
    /// OpenHapticsOptions nodes in the options field and
    /// the default settings in OpenHapticsSettings bindable node.
    HAPI::HAPIHapticShape *getOpenGLHapticShape( H3DSurfaceNode *_surface,
                                                 const Matrix4f &_transform,
                                                 int _nr_vertices = -1 );
#endif
    /// Returns the default xml containerField attribute value.
    /// For this node it is "geometry".
    ///
    virtual string defaultXMLContainerField() {
      return "geometry";
    }
    
    /// Overriden from HAPIGLShape to use the bound field in he X3DGeometryNode.
    /// An axis aligned bounding box containing  all the primitives rendered by 
    /// the glRender function. If no such bounding box is available, size
    /// should be set to Vec3( -1, -1, -1 )
    virtual void getBound( HAPI::Vec3 &center, HAPI::Vec3& size ) {
      BoxBound *b = dynamic_cast< BoxBound * >( bound->getValue() );
      if( b ) {
        center = b->center->getValue();
        size = b->size->getValue();
      } else {
        HAPIGLShape::getBound( center, size );
      }
    }

    /// Changes behaviour of X3DGeometryNode to be a thread safe
    /// RefCountedClass.
    virtual void initialize();

    /// Tells if a HapticsDevice has been in contact with the geometry
    /// in the last scenegraph loop. The field contains a boolean for 
    /// each HapticsDevice with the index as specified in the DeviceInfo node.
    /// You can get the contact point from the contactPoint field. The value 
    /// will be an empty vector before the first contact.
    ///
    /// <b>Access type:</b> outputOnly
    auto_ptr< MFBool >  isTouched;

    /// The last force delivered by this geometry to the haptics device.
    /// The field contains a force vector for each HapticsDevice with the
    /// index as specified in the DeviceInfo node. The value will be an
    /// empty vector before the first contact.
    auto_ptr< MFVec3f >  force;

    /// The last contact points of the HapticDevices on the geometry.
    /// The field contains a contact point for each HapticsDevice with the index
    /// as specified in the DeviceInfo node. The value will be an empty vector
    /// before the first contact.
    ///
    /// <b>Access type:</b> outputOnly
    auto_ptr< MFVec3f >  contactPoint;

    /// The texture coordinate of last the contact points of the HapticDevices
    /// on the geometry.
    /// The field contains a contact point for each HapticsDevice with the index
    /// as specified in the DeviceInfo node. The value will be an empty vector
    /// before the first contact.
    ///
    /// <b>Access type:</b> outputOnly
    auto_ptr< MFVec3f >  contactTexCoord;

    /// The normal at the last contact points of the HapticDevices on the
    /// geometry.
    /// The field contains a normal for each of the points in contactPoint. 
    /// The value will be an empty vector before the first contact.
    ///
    /// <b>Access type:</b> outputOnly
    auto_ptr< MFVec3f >  contactNormal;

    auto_ptr< SFBoundTree > boundTree;

    /// Contains nodes with options for haptics and graphics rendering.
    ///
    /// <b>Access type:</b> inputOnly
    auto_ptr< MFOptionsNode > options;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;

  protected:
    /// Function sent to HAPIHapticsShape created to allow for deletion of
    /// X3DGeometryNode at the correct time. The X3DGeometryNode is not
    /// automatically refernced counted when sent to HAPIHapticShape.
    static void cleanUpFunction( void *userdata) {
      static_cast< X3DGeometryNode * >(userdata)->unref();
    }

    inline void renderTexCoordForActiveTexture( const Vec3f &tc ) {
      X3DTextureCoordinateNode::renderTexCoordForActiveTexture( tc );
    }

    void createAndAddHapticShapes( TraverseInfo &ti,
                                   H3DHapticsDevice *hd,
                                   H3DInt32 hd_index,
                                   OpenHapticsOptions *openhaptics_options );

    /// identifiers for the shapes geometry.
    vector< int > haptic_shape_ids;

    bool use_culling, allow_culling;
    bool draw_debug_options;
    GLenum cull_face;
    friend class H3DHapticsDevice;
  };
}

#endif
