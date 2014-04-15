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
/// \file SuperShape.h
/// \brief Header file for SuperShape, X3D scene-graph node. Not in X3D-spec.
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __SUPERSHAPE_H__
#define __SUPERSHAPE_H__

#include <H3D/X3DGeometryNode.h>
#include <H3D/SFFloat.h>
#include <H3D/SFInt32.h>
#include <H3D/DependentNodeFields.h>
#include <H3D/X3DCoordinateNode.h>
#include <H3D/CoordBoundField.h>
#include <H3D/X3DNormalNode.h>

namespace H3D {

  /// \ingroup H3DNodes
  /// \class SuperShape
  /// \brief The SuperShape node implements a SuperShape!
  ///
  /// A SuperShape is an implementation of the SuperFormula. The SuperFormula
  /// is a formula that can be used to describe geometric shapes found in
  /// nature. For information about the parameters defining the SuperShape
  /// see 2d-equation on
  /// http://local.wasp.uwa.edu.au/~pbourke/surfaces_curves/supershape3d/
  /// This geometryNode implements a 3D-version of the SuperFormula by using
  /// two functions. The parameter names for one function is ss1_x and for the
  /// other it is ss2. Experiment with values to see what they do.
  ///
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/SuperShape.x3d">SuperShape.x3d</a>
  ///     ( <a href="examples/SuperShape.x3d.html">Source</a> )
  ///
  /// \par Internal routes:
  /// \dotfile SuperShape.dot
  class H3DAPI_API SuperShape : public X3DGeometryNode {
  public:
    
    /// The SFNormalNode is dependent on the propertyChanged field of the 
    /// contained X3DNormalNode.
    typedef DependentSFNode< X3DNormalNode,
                             FieldRef< X3DGeometricPropertyNode,
                                       Field,
                                       &X3DNormalNode::propertyChanged > > 
    SFNormalNode;

    /// The SFCoordinateNode is a class that contain a X3DCoordinateNode.
    /// It updates itself from the fields routed to it. The fields routed to
    /// it are all the fields defining the SuperShape shape.
    ///
    /// routes_in[0] is the ss1_m field of the SuperShape node.
    /// routes_in[1] is the ss1_a field of the SuperShape node.
    /// routes_in[2] is the ss1_b field of the SuperShape node.
    /// routes_in[3] is the ss1_n1 field of the SuperShape node.
    /// routes_in[4] is the ss1_n2 field of the SuperShape node.
    /// routes_in[5] is the ss1_n3 field of the SuperShape node.
    /// routes_in[6] is the ss2_m field of the SuperShape node.
    /// routes_in[7] is the ss2_a field of the SuperShape node.
    /// routes_in[8] is the ss2_b field of the SuperShape node.
    /// routes_in[9] is the ss2_n1 field of the SuperShape node.
    /// routes_in[10] is the ss2_n2 field of the SuperShape node.
    /// routes_in[11] is the ss2_n3 field of the SuperShape node.
    /// routes_in[12] is the resolution field of the SuperShape node.
    ///
    class SFCoordinateNode: public TypedField< DependentSFNode< X3DCoordinateNode,
                             FieldRef< X3DGeometricPropertyNode,
                                       Field,
                                       &X3DCoordinateNode::propertyChanged > >, Types< SFFloat, SFFloat,
                                                              SFFloat, SFFloat,
                                                              SFFloat, SFFloat,
                                                              SFFloat, SFFloat,
                                                              SFFloat, SFFloat,
                                                              SFFloat, SFFloat,
                                                              SFInt32 > > {
      /// Update the bound from the fields routed to it
      virtual void update();

      // Function used internally to define supershape.
      H3DFloat R( H3DFloat phi, H3DFloat m, H3DFloat a, H3DFloat b,
                  H3DFloat n1, H3DFloat n2, H3DFloat n3 ) {
        H3DFloat t2 = m*phi/4.0f;
        H3DFloat t3 = cosf(t2);
        H3DFloat t6 = H3DAbs(t3/a);
        H3DFloat t7 = powf(t6,n2);
        H3DFloat t8 = sinf(t2);
        H3DFloat t11 = H3DAbs(t8/b);
        H3DFloat t12 = powf(t11,n3);
        H3DFloat t14 = powf(t7+t12,-1/n1);
        return t14;
        //return powf(powf(fabsf(cosf((alpha*m)/4.)/a), n2) 
        //            + powf(fabsf(sinf((alpha*m)/4.)/b),n3), -1/n1);
      }

      // Function used internally to define supershape.
      Vec3f SS( H3DFloat theta, H3DFloat phi, H3DFloat sm, H3DFloat sa,
              H3DFloat sb, H3DFloat sn1, H3DFloat sn2, H3DFloat sn3,
              H3DFloat tm, H3DFloat ta, H3DFloat tb, H3DFloat tn1,
              H3DFloat tn2, H3DFloat tn3 ) { 
        H3DFloat r1theta = R( theta, sm, sa, sb, sn1, sn2, sn3 );
        H3DFloat r2phi = R( phi, tm, ta, tb, tn1, tn2, tn3 );

        H3DFloat x = r1theta * cosf(theta) * r2phi * cosf(phi);
        H3DFloat y = r1theta * sin(theta) * r2phi * cosf(phi);
        H3DFloat z = r2phi * sin(phi);

        return Vec3f( x, y, z);
      }
    };

    /// The bound field for SuperShape is a CoordBoundField.
    typedef CoordBoundField SFBound;

    SuperShape( Inst< SFNode   >  _metadata = 0,
                Inst< SFBound     > _bound = 0,
                Inst< SFFloat  > _ss1_m  = 0,
                Inst< SFFloat  > _ss1_a  = 0,
                Inst< SFFloat  > _ss1_b  = 0,
                Inst< SFFloat  > _ss1_n1 = 0,
                Inst< SFFloat  > _ss1_n2 = 0,
                Inst< SFFloat  > _ss1_n3 = 0,
                Inst< SFFloat  > _ss2_m  = 0,
                Inst< SFFloat  > _ss2_a  = 0,
                Inst< SFFloat  > _ss2_b  = 0,
                Inst< SFFloat  > _ss2_n1 = 0,
                Inst< SFFloat  > _ss2_n2 = 0,
                Inst< SFFloat  > _ss2_n3 = 0,
                Inst< SFInt32  > _resolution = 0 );

    virtual void render();

    /// The number of triangles renderered in this geometry.
    virtual int nrTriangles() {
      H3DInt32 res = resolution->getValue();
      if( res < 0 ) return 0;
      else return res * res * 2;
    }

    // Traverse the scenegraph. See X3DGeometryNode::traverseSG
    // for more info.
    virtual void traverseSG( TraverseInfo &ti );
    
    // Fields
    /// Parameter to define supershape.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 4 \n
    /// 
    /// \dotfile SuperShape_ss1_m.dot
    auto_ptr< SFFloat  > ss1_m;

    /// Parameter to define supershape.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 1 \n
    /// 
    /// \dotfile SuperShape_ss1_a.dot
    auto_ptr< SFFloat  > ss1_a;

    /// Parameter to define supershape.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 1 \n
    /// 
    /// \dotfile SuperShape_ss1_b.dot
    auto_ptr< SFFloat  > ss1_b;

    /// Parameter to define supershape.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 100 \n
    /// 
    /// \dotfile SuperShape_ss1_n1.dot
    auto_ptr< SFFloat  > ss1_n1;

    /// Parameter to define supershape.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 100 \n
    /// 
    /// \dotfile SuperShape_ss1_n2.dot
    auto_ptr< SFFloat  > ss1_n2;

    /// Parameter to define supershape.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 100 \n
    /// 
    /// \dotfile SuperShape_ss1_n3.dot
    auto_ptr< SFFloat  > ss1_n3;

    /// Parameter to define supershape.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 4 \n
    /// 
    /// \dotfile SuperShape_ss2_m.dot
    auto_ptr< SFFloat  > ss2_m;

    /// Parameter to define supershape.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 1 \n
    /// 
    /// \dotfile SuperShape_ss2_a.dot
    auto_ptr< SFFloat  > ss2_a;

    /// Parameter to define supershape.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 1 \n
    /// 
    /// \dotfile SuperShape_ss2_b.dot
    auto_ptr< SFFloat  > ss2_b;

    /// Parameter to define supershape.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 100 \n
    /// 
    /// \dotfile SuperShape_ss2_n1.dot
    auto_ptr< SFFloat  > ss2_n1;

    /// Parameter to define supershape.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 100 \n
    /// 
    /// \dotfile SuperShape_ss2_n2.dot
    auto_ptr< SFFloat  > ss2_n2;

    /// Parameter to define supershape.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 100 \n
    /// 
    /// \dotfile SuperShape_ss2_n3.dot
    auto_ptr< SFFloat  > ss2_n3;

    /// Parameter to define the resolution of the supershape.
    /// A higher value generally give a shape that more closely follows the
    /// values generate by the implicit SuperFormula than a lower value.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 128 \n
    /// 
    /// \dotfile SuperShape_resolution.dot
    auto_ptr< SFInt32  > resolution;

    static H3DNodeDatabase database;
  protected:

    /// Start texture coordinate generation. Texture coordinates will be
    /// generated for all texture units used by the currently active texture.
    /// Texture coordinates will be generated 
    /// from the bounding box of the geometry
    void startTexGen();

    /// Stop texture coordinate generation.
    void stopTexGen();

    auto_ptr< SFCoordinateNode > coord;
    auto_ptr< SFNormalNode > normal;

  };
}

#endif
