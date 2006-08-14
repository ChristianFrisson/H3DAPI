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
/// \file X3DGeometryNode.h
/// \brief Header file for X3DGeometryNode, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __X3DGEOMETRYNODE_H__
#define __X3DGEOMETRYNODE_H__

#include "X3DChildNode.h"
#include "H3DBoundedObject.h"
#include "H3DDisplayListObject.h"
#include "MFBool.h"
#include "MFVec3f.h"
#include "AutoPtrVector.h"
#include "H3DOptionNode.h"
#include "MFNode.h"
#ifdef HAVE_OPENHAPTICS
#include <HL/hl.h>
#endif

namespace H3D {

  class HLHapticsDevice;

  /// \ingroup AbstractNodes
  /// \class X3DGeometryNode
  /// This is the base node type for all geometry in X3D. 
  ///
  /// 
  class H3DAPI_API X3DGeometryNode : 
    public X3DChildNode,
    public H3DBoundedObject,
    public H3DDisplayListObject {
  public:
    typedef TypedMFNode< H3DOptionNode > MFOptionsNode;

    /// Constructor.
    X3DGeometryNode( Inst< SFNode      >  _metadata = 0,
                     Inst< SFBound     > _bound = 0,
                     Inst< DisplayList > _displayList = 0,
                     Inst< MFBool      > _isTouched = 0,
                     Inst< MFVec3f     > _force = 0,
                     Inst< MFVec3f     > _contactPoint = 0,
                     Inst< MFVec3f     > _contactNormal = 0);
    /// Get the first option node of the type of the pointer given as argument
    /// from the renderOptions fieeld
    /// The option argument will contain the node afterwards, or NULL if no
    /// option of that type exists.
    template< class OptionNodeType >
    void getOptionNode( typename OptionNodeType * &option ) {
       for( MFOptionsNode::const_iterator i = options->begin();
           i != options->end(); i++ ) {
        OptionNodeType *options = dynamic_cast< OptionNodeType * >( *i );
        if( options ) option = options;
        return;
      }
      option = NULL;
    }

 #ifdef HAVE_OPENHAPTICS   
    /// Destructor. Deletes the hl_shape_id.
    virtual ~X3DGeometryNode();
 
    /// Get a shape id to be used for rendering of this geometry with HLAPI for
    /// the given haptics device.
    /// Since the geometry can appear in several places in the scene graph
    /// it can contain several shape ids (one for each place). Which one to
    /// get is determined by the index argument.
    HLuint getHLShapeId( HLHapticsDevice *hd,
                         unsigned int index );

    /// Returns a either a HLFeedbackShape or a HLDepthBufferShape with
    /// the X3DGeometryNode. Which type depents on possible 
    /// OpenHapticsOptions nodes in the options field and
    /// the default settings in OpenHapticsSettings bindable node.
    HapticShape *getOpenGLHapticShape( H3DSurfaceNode *_surface,
                                       const Matrix4f &_transform,
                                       HLint _nr_vertices = -1 );
#endif
    /// Returns the default xml containerField attribute value.
    /// For this node it is "geometry".
    ///
    virtual string defaultXMLContainerField() {
      return "geometry";
    }
   
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

    /// The normal at the last contact points of the HapticDevices on the
    /// geometry.
    /// The field contains a normal for each of the points in contactPoint. 
    /// The value will be an empty vector before the first contact.
    ///
    /// <b>Access type:</b> outputOnly
    auto_ptr< MFVec3f >  contactNormal;

    /// Contains nodes with options for haptics and graphics rendering.
    ///
    /// <b>Access type:</b> inputOnly
    auto_ptr< MFOptionsNode > options;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;

  protected:
  #ifdef HAVE_OPENHAPTICS
    /// HL event callback function for when the geometry is touched.
    static void HLCALLBACK touchCallback( HLenum event,
                                          HLuint object,
                                          HLenum thread,
                                          HLcache *cache,
                                          void *userdata );

    /// HL event callback function for when the geometry is not touched
    /// any longer. 
    static void HLCALLBACK untouchCallback( HLenum event,
                                            HLuint object,
                                            HLenum thread,
                                            HLcache *cache,
                                            void *userdata );

    /// HL event callback function for when the proxy moves while in
    /// contact with the geometry.
    static void HLCALLBACK motionCallback( HLenum event,
                                           HLuint object,
                                           HLenum thread,
                                           HLcache *cache,
                                           void *userdata );

    typedef map< HLHapticsDevice *, vector< HLuint > > ShapeIdMap;

    /// HL identifiers for the geometry.
    ShapeIdMap hl_shape_ids;

    struct CallbackData {
      CallbackData( X3DGeometryNode *g, int i ): geometry( g ), device_index( i ) {}
      X3DGeometryNode *geometry;
      int device_index;
    };
    
    AutoPtrVector< CallbackData > callback_data; 
  #endif
  };
}

#endif
