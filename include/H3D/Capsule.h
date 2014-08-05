//////////////////////////////////////////////////////////////////////////////
//    Copyright 2010, SenseGraphics AB and Moog
//
//    This file is part of DentalSim. You may not distribute this file without
//    explicit permission from the copyright holders.
//
//
/// \file Capsule.h
/// \brief Header file for Capsule
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __CAPSULE_H__
#define __CAPSULE_H__

#include <H3D/X3DGeometryNode.h>
#include <H3D/SFFloat.h>

namespace H3D {
  /// \ingroup H3DNodes
  /// \class Capsule
  /// \brief  The Capsule node specifies a cylinder, with half-sphere as end caps,
  /// centred at (0,0,0) in the local coordinate system and with a central axis 
  /// oriented along the local Y-axis.
  ///
  /// By default, the cylinder is sized at "-1" to "+1" in all three
  /// dimensions. The radius field specifies the radius of the cylinder and end-caps 
  /// and the height field specifies the height of capsule, i.e. the distance between
  /// the centers of the half-sphere end caps. Both radius and height shall be 
  /// greater than zero. 
  ///
  /// The capsule has three parts: the side, the top and the bottom.
  /// Each part has an associated SFBool field that indicates whether the part
  /// exists (TRUE) or does not exist (FALSE). Parts which do not exist are not 
  /// rendered and not eligible for intersection tests.
  ///
  /// The solid field determines whether the cylinder is visible when viewed
  /// from the inside. If solid is false both sides of the cylinder will be 
  /// rendered.
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/Capsule.x3d">Capsule.x3d</a>
  ///     ( <a href="examples/Capsule.x3d.html">Source</a> )
  ///
  /// \par Internal routes:
  /// \dotfile Capsule.dot
  class H3DAPI_API Capsule : public X3DGeometryNode {
  public:

    /// SFBound is specialized update itself from the radius and
    /// heigh field of the Capsule node.
    ///
    /// routes_in[0] is the height field of the Capsule node.  
    /// routes_in[1] is the radius field of the Capsule node.  
    ///
    class H3DAPI_API SFBound: public X3DGeometryNode::SFBound {
    /// Update the bound from the radius field. 
    virtual void update() {
      H3DFloat h = static_cast< SFFloat * >( routes_in[0] )->getValue();
      H3DFloat r = static_cast< SFFloat * >( routes_in[1] )->getValue();
      BoxBound *bb = new BoxBound;
      H3DFloat d = 2 * r;
      bb->size->setValue( Vec3f( d, h+d, d ) );
      value = bb;
    }
  };

  /// Constructor.
  Capsule( Inst< SFNode  >  _metadata = 0,
           Inst< SFBound >  _bound    = 0,
           Inst< SFBool  >  _bottom   = 0,
           Inst< SFFloat >  _height   = 0,
           Inst< SFFloat >  _radius   = 0,
           Inst< SFBool  >  _side     = 0,
           Inst< SFBool  >  _solid    = 0,
           Inst< SFBool  >  _top      = 0 );

  /// Destructor
  ~Capsule();

  /// Renders the Capsule with OpenGL.
  virtual void render();

  /// Traverse the scenegraph. Adds a HapticCapsule if haptics is enabled.
  virtual void traverseSG( TraverseInfo &ti );    

  /// The number of triangles rendered by this geometry.
  virtual int nrTriangles();

  /// Specifies if the bottom of the Capsule should be rendered or not.
  ///
  /// <b>Access type:</b> inputOutput \n
  /// <b>Default value:</b> TRUE \n
  /// 
  /// \dotfile Capsule_bottom.dot
  auto_ptr< SFBool > bottom;
    
  /// The height of the Capsule in meters.
  ///
  /// <b>Access type:</b> inputOutput \n
  /// <b>Default value:</b> 2 \n
  /// <b>Valid range:</b> >0
  /// 
  /// \dotfile Capsule_height.dot
  auto_ptr< SFFloat > height;

  /// The radius of the Capsule.
  ///
  /// <b>Access type:</b> inputOutput \n
  /// <b>Default value:</b> 1 \n
  /// <b>Valid range:</b> >0
  /// 
  /// \dotfile Capsule_radius.dot
  auto_ptr< SFFloat > radius;

  /// Specifies if the sides of the Capsule should be rendered or not.
  ///
  /// <b>Access type:</b> inputOutput \n
  /// <b>Default value:</b> TRUE \n
  /// 
  /// \dotfile Capsule_side.dot
  auto_ptr< SFBool > side;

  /// The solid field determines whether the Capsule is visible when viewed
  /// from the inside. If solid is false both sides of the sphere will be 
  /// rendered.
  ///
  /// <b>Access type:</b> inputOutput \n
  /// <b>Default value:</b> TRUE \n
  /// 
  /// \dotfile Capsule_solid.dot
  auto_ptr< SFBool > solid;

  /// Specifies if the top of the Capsule should be rendered or not.
  ///
  /// <b>Access type:</b> inputOutput \n
  /// <b>Default value:</b> TRUE \n
  /// 
  /// \dotfile Capsule_top.dot
  auto_ptr< SFBool > top;

  /// The H3DNodeDatabase instance for this node.
  static H3DNodeDatabase database;

  protected:
  // Internal field used to know if vertex buffer object can be created.
  auto_ptr< Field > vboFieldsUpToDate;
  // The index for the vertex buffer object
  GLuint *vbo_id;

  // the number of parts around the cylinder and along the equator of the end-cap half-spheres
  static const unsigned int theta_parts = 10;
  // the number of parts along the height of the half-spheres
  static const unsigned int phi_parts = 5;
  };
}

#endif
