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
/// \file NurbsTextureCoordinate.h
/// \brief Header file for NurbsTextureCoordinate, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __NURBSTEXTURECOORDINATE_H__
#define __NURBSTEXTURECOORDINATE_H__

#include <H3D/X3DGeometricPropertyNode.h>
#include <H3D/MFVec2f.h>
#include <H3D/MFFloat.h>
#include <H3D/SFInt32.h>
#include <H3D/MFDouble.h>
#include "GL/glew.h"

namespace H3D {

  /// \ingroup X3DNodes
  /// \class NurbsTextureCoordinate
  /// \brief The NurbsTextureCoordinate node is a NURBS surface existing in the
  /// parametric domain of its surface host specifying the mapping of the 
  /// texture onto the surface.
  ///
  /// The parameters are as specified in NurbsTextureCoordinate with the
  /// exception that the control points are specified in (u, v) coordinates.
  ///
  /// The tessellation process generates 2D texture coordinates.
  /// If a NurbsTextureCoordinate is undefined, texture coordinates are
  /// computed by the client on the basis of parametric step size.
  /// Conventional vertex parameters do not apply on NURBS surfaces because 
  /// triangles are only available after polygonalization. However, the 
  /// conventional texture transform may be used.
  ///
  /// NurbsTextureCoordinate nodes are accessed through the texCoord field 
  /// of a node derived from NurbsTextureCoordinate.
  /// A NurbsTextureCoordinate node separately encountered is ignored.
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/NurbsTextureCoordinate.x3d">NurbsTextureCoordinate.x3d</a>
  ///     ( <a href="examples/NurbsTextureCoordinate.x3d.html">Source</a> )
  ///
  /// \par Internal routes:
  /// \dotfile NurbsTextureCoordinate.dot
  // according to the X3D standard NurbsTextureCoordinate should inherit 
  // from X3DNode so we cheat here to make it easier in 
  // NurbsTextureCoordinate.
  class H3DAPI_API NurbsTextureCoordinate : 
    public X3DGeometricPropertyNode {
  public:

    /// Calls OpenGL routines to render the texture.
    void renderTexForNurbs( GLUnurbsObj *nurbs_object );

    /// Destructor.
    ~NurbsTextureCoordinate(){ 
      if(u_knots != NULL ) 
        delete [] u_knots;
      if(v_knots != NULL ) 
        delete [] v_knots;  
      if(withWeights != NULL ) 
        delete [] withWeights;}

    /// Constructor.
    NurbsTextureCoordinate(       Inst< SFNode   >  _metadata    = 0,
                                 Inst< MFVec2f  > _controlPoint = 0,
                                 Inst< MFFloat  > _weight        = 0,
                                 Inst< SFInt32  > _uDimension    = 0,
                                 Inst< SFInt32  > _vDimension    = 0,
                                 Inst< MFDouble > _uKnot        = 0,
                                 Inst< MFDouble > _vKnot        = 0,
                                 Inst< SFInt32  > _uOrder        = 0,
                                 Inst< SFInt32  > _vOrder        = 0 );

    /// Returns the default xml containerField attribute value.
    /// For this node it is "texCoord".
    ///
    virtual string defaultXMLContainerField() {
      return "texCoord";
    }

    /// controlPoint defines a set of control points of dimension 
    /// uDimension × vDimension. This set of points defines a mesh where the
    /// points do not have a uniform spacing. uDimension points define a 
    /// polyline in u-direction followed by further u-polylines with the 
    /// v-parameter in ascending order. The number of control points shall be
    /// equal or greater than the order.
    /// The control points are specified as (u, v) coordinates in this node.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// 
    /// \dotfile NurbsTextureCoordinate_controlPoint.dot 
    auto_ptr< MFVec2f >  controlPoint;

    /// A weight value that shall be greater than zero is assigned to each
    /// controlPoint. The ordering of the values is equivalent to the 
    /// ordering of the control point values. The number of values shall
    /// be identical to the number of control points. If the length of the
    /// weight vector is 0, the default weight 1.0 is assumed for each control
    /// point, thus defining a non-Rational curve. If the number of weight 
    /// values is less than the number of control points, all weight values
    /// shall be ignored and a value of 1.0 shall be used.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// 
    /// \dotfile NurbsTextureCoordinate_weight.dot
    auto_ptr< MFFloat > weight;

    /// Defines the number of control points in the u dimension.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 0 \n
    /// 
    /// \dotfile NurbsTextureCoordinate_uDimension.dot
    auto_ptr< SFInt32  >  uDimension;

    /// Defines the number of control points in the v dimension.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 0 \n
    /// 
    /// \dotfile NurbsTextureCoordinate_vDimension.dot
    auto_ptr< SFInt32  >  vDimension;

    /// Defines the knot values of the surface in the u dimension.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// 
    /// \dotfile NurbsTextureCoordinate_uKnot.dot
    auto_ptr< MFDouble >  uKnot;

    /// Defines the knot values of the surface in the v dimension.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// 
    /// \dotfile NurbsTextureCoordinate_vKnot.dot
    auto_ptr< MFDouble >  vKnot;

    /// Defines the order of the surface in the u dimension.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 3 \n
    /// 
    /// \dotfile NurbsTextureCoordinate_uOrder.dot
    auto_ptr< SFInt32  > uOrder;

    /// Defines the order of the surface in the v dimension.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 3 \n
    /// 
    /// \dotfile NurbsTextureCoordinate_vOrder.dot
    auto_ptr< SFInt32  > vOrder;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;

  protected:
    // Used for temporary storage.
    GLfloat * withWeights;
    GLfloat * u_knots;
    GLfloat * v_knots;
  };
}

#endif
