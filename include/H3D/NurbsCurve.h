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
/// \file NurbsCurve.h
/// \brief Header file for NurbsCurve, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __NURBSCURVE_H__
#define __NURBSCURVE_H__

#include <H3D/X3DParametricGeometryNode.h>
#include <H3D/DependentNodeFields.h>
#include <H3D/SFInt32.h>
#include <H3D/MFDouble.h>
#include <H3D/SFBool.h>
#include <H3D/X3DCoordinateNode.h>
#include <H3D/X3DBoundedObject.h>

namespace H3D {

  /// \ingroup X3DNode
  /// \class NurbsCurve
  /// \brief The NurbsCurve node is a geometry node defining a parametric
  ///  curve in 3D space.
  ///
  ///  The tessellation field gives a hint to the curve tessellator by
  ///  setting an absolute number of subdivision steps. These values shall
  ///  be greater than or equal to the Order field. A value of 0 indicates
  ///  that the browser choose a suitable tessellation. Interpretation of 
  ///  values below 0 is implementation dependent.
  ///
  ///  For an implementation subdividing the surface into an equal number
  ///  of subdivision steps, tessellation values are interpreted as follows:
  ///  
  ///  - if a tessellation value is greater than 0, the number of tessellation
  ///  points is:
  ///  tessellation+1;
  /// 
  ///  - if a tessellation value is smaller than 0, the number of tessellation
  ///  points is:
  ///   -tessellation *(number of control points)+1;
  ///
  ///  - if a tessellation value is 0, the number of tessellation points is:
  ///  (2 * (number of control points)+1. 
  /// 
  ///  For implementations doing tessellations based on chord length, 
  ///  tessellation values less than zero are interpreted as the maximum 
  ///  chord length deviation in pixels. Implementations doing fully automatic
  ///  tessellation may ignore the tessellation hint parameters.
  /// 
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/NurbsCurve.x3d">NurbsCurve.x3d</a>
  ///     ( <a href="examples/NurbsCurve.x3d.html">Source</a> )
  ///
  /// \par Internal routes:
  /// \dotfile NurbsCurve.dot
  class H3DAPI_API NurbsCurve : 
    public X3DParametricGeometryNode {
  public:

    /// SFCoordinateNode is dependent on coordChanged field in 
    /// X3DCoordinateNode.
    typedef DependentSFNode< X3DCoordinateNode,
      FieldRef< X3DGeometricPropertyNode,
      Field,
      &X3DCoordinateNode::propertyChanged > > 
      SFCoordinateNode;


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
      SFCoordinateNode > {
    protected:
      /// The SFBound is updated to a bound that is the union of the 
      /// the Bound objects routed to it.
      virtual void update();

    private:
      friend class NurbsCurve;
    };

    /// Constructor.
    NurbsCurve(  Inst< SFNode >  _metadata      = 0,
      Inst< SFBound           > _bound          = 0,
      Inst< DisplayList       > _displayList    = 0,
      Inst< MFBool            > _isTouched      = 0,
      Inst< MFVec3f           > _force          = 0,
      Inst< MFVec3f           > _contactPoint   = 0,
      Inst< MFVec3f           > _contactNormal  = 0,
      Inst< SFCoordinateNode  > _controlPoint   = 0,
      Inst< SFInt32           > _tessellation   = 0,
      Inst< MFDouble          > _weight         = 0,
      Inst< MFDouble          > _knot           = 0,
      Inst< SFInt32           > _order          = 0,
      Inst< SFBool            > _closed         = 0 );


    /// Render the NurbsCurve with OpenGL.
    virtual void render();

    /// The tessellation field gives a hint to the curve tessellator by setting
    /// an absolute number of subdivision steps. These values shall be greater 
    /// than or equal to the Order field.
    /// The tessellation values are interpreted as follows:
    ///    
    /// - if a tessellation valie is greater than 0, the number of tessellation
    /// points is: tessellation+1;
    ///
    /// - if a tessellation value is smaller than 0, the number of tessellation    
    /// points is: -tessellation * (number of control points)+1;    
    ///    
    /// - if a tessellation value is 0, the number of tessellation points is:
    /// 2 *(number of control points)+1..    
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 0 \n
    /// 
    /// \dotfile NurbsCurve_tessellation.dot
    auto_ptr< SFInt32 > tessellation;

    /// A weight value that shall be greater than zero is assigned to each 
    /// controlPoint. The ordering of the values is equivalent to the ordering
    /// of the control point values. The number of values shall be identical 
    /// to the number of control points. If the length of the weight vector 
    /// is 0, the default weight 1.0 is assumed for each control point, thus
    /// defining a non-Rational curve. If the number of weight values is less
    /// than the number of control points, all weight values shall be ignored 
    /// and a value of 1.0 shall be used.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// 
    /// \dotfile NurbsCurve_weight.dot
    auto_ptr< MFDouble >  weight;

    /// Specifies the knots of the curve
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// 
    /// \dotfile NurbsCurve_knot.dot
    auto_ptr< MFDouble >  knot;

    /// Defines the order of curve. From a mathematical point of view, the 
    /// curve is defined by a polynomial of the degree order=1. The value
    /// of order shall be greater than or equal to 2. An implementation may
    /// limit order to a certain number. If it does so, then a warning shall
    /// be generated and the surface not displayed. An implementation shall at
    /// least support orders 2,3 and 4. The number of control points shall be 
    /// at least equal to the order of the curve. The order defines the number
    /// of adjacent control points that influence a given control point.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 3 \n
    /// 
    /// \dotfile NurbsCurve_order.dot
    auto_ptr< SFInt32 >  order;

    /// closed define whether or not to evaluate the curve as closed.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> FALSE \n
    /// 
    /// \dotfile NurbsCurve_closed.dot
    auto_ptr< SFBool >  closed;

    /// The controlPoint field 
    ///
    /// <b>Access type:</b> inputOutput 
    ///
    /// \dotfile NurbsCurve_closed.dot 
    auto_ptr< SFCoordinateNode  >  controlPoint;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;

  protected:
    GLUnurbsObj *theNurb;
    auto_ptr< Field > printWarning;
  };
}

#endif
