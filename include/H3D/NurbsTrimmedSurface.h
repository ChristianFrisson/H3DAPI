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
/// \file NurbsTrimmedSurface.h
/// \brief Header file for NurbsTrimmedSurface, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __NURBSTRIMMEDSURFACE_H__
#define __NURBSTRIMMEDSURFACE_H__

#include <H3D/X3DNurbsSurfaceGeometryNode.h>
#include <H3D/Contour2D.h>

namespace H3D {

  /// \ingroup X3DNodes
  /// \class NurbsTrimmedSurface
  /// \brief The NurbsTrimmedSurface node defines a NURBS surface 
  /// that is trimmed by a set of trimming
  /// loops. The outermost trimming loop shall be defined in a 
  /// counterclockwise direction.
  ///
  /// The trimmingContour field, if specified, shall contain a set of Contour2D
  /// nodes. Trimming loops shall be processed as described for the Contour2D 
  /// node. If no trimming contours are defined, The NurbsTrimmedSurface node 
  /// shall have the same semantics as the NurbsPatchSurface node.
  ///
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/NurbsTrimmedSurface.x3d">NurbsTrimmedSurface.x3d</a>
  ///     ( <a href="examples/NurbsTrimmedSurface.x3d.html">Source</a> )
  ///
  /// \par Internal routes:
  /// \dotfile NurbsTrimmedSurface.dot
  class NurbsTrimmedSurface : 
    public X3DNurbsSurfaceGeometryNode {
  public:

    /// This field class checks whether the given node
    /// is of the right type (Contour2D). 
    /// If this is the case the node is added to the trimmingContour field.
    class H3DAPI_API AddContour: public MFNode {
    public:

      virtual void onAdd( Node *n ) {
        MFNode::onAdd( n );
        Contour2D *contour_2D = 
          dynamic_cast< Contour2D * >( n );

        if( !contour_2D ) {
          stringstream s;
          s << "Expecting Contour2D";
          throw InvalidNodeType( n->getTypeName(),
            s.str(),
            H3D_FULL_LOCATION );
        }
        else {
          NurbsTrimmedSurface * nurbs_trimmed_surface = 
          static_cast< NurbsTrimmedSurface * >( getOwner() );
            nurbs_trimmed_surface->trimmingContour->push_back( n );
        }
      }
    };
#ifdef __BORLANDC__
    friend class AddContour;
#endif

    /// This field class checks whether the given node
    /// is of the right type (Contour2D). 
    /// If this is the case the node is removed from the trimmingContour field.
    class H3DAPI_API RemoveContour: public MFNode {
    public:

      
      virtual void onRemove( Node *n ) {
        Contour2D *contour_2D = 
          dynamic_cast< Contour2D * >( n );

        if( !contour_2D ) {
          stringstream s;
          s << "Expecting Contour2D";
          throw InvalidNodeType( n->getTypeName(),
            s.str(),
            H3D_FULL_LOCATION );
        }
        else {
          NurbsTrimmedSurface * nurbs_trimmed_surface = 
          static_cast< NurbsTrimmedSurface * >( getOwner() );
          nurbs_trimmed_surface->trimmingContour->erase( n );
        }

        MFNode::onRemove( n );
      }
    };
#ifdef __BORLANDC__
    friend class RemoveContour;
#endif

    /// This field class checks whether the given node
    /// is of the right type (Contour2D) for adding or removing. 
    class H3DAPI_API CheckContour: public MFNode {
    public:

      virtual void onAdd( Node *n ) {
        MFNode::onAdd( n );
        Contour2D *contour_2D = 
          dynamic_cast< Contour2D * >( n );

        if( !contour_2D ) {
          stringstream s;
          s << "Expecting Contour2D";
          throw InvalidNodeType( n->getTypeName(),
            s.str(),
            H3D_FULL_LOCATION );
        }
      }

      virtual void onRemove( Node *n ) {
        Contour2D *contour_2D = 
          dynamic_cast< Contour2D * >( n );

        if( !contour_2D ) {
          stringstream s;
          s << "Expecting Contour2D";
          throw InvalidNodeType( n->getTypeName(),
            s.str(),
            H3D_FULL_LOCATION );
        }

        MFNode::onRemove( n );
      }
    };
#ifdef __BORLANDC__
    friend class CheckContour;
#endif
    /// Constructor.
    NurbsTrimmedSurface( Inst< SFNode      >  _metadata = 0,
                         Inst< SFBound     > _bound = 0,
                         Inst< DisplayList > _displayList = 0,
                         Inst< MFBool      > _isTouched = 0,
                         Inst< MFVec3f     > _force = 0,
                         Inst< MFVec3f     > _contactPoint = 0,
                         Inst< MFVec3f     > _contactNormal = 0,
                         Inst< AddContour   > _addTrimmingContour = 0,
                         Inst< RemoveContour > _removeTrimmingContour = 0,
                         Inst< SFCoordinateNode > _controlPoint = 0,
                         Inst< TexCoordOrNurbsTex >  _texCoord = 0,
                         Inst< CheckContour > _trimmingContour = 0,
                         Inst< SFInt32     > _uTessellation = 0,
                         Inst< SFInt32     > _vTessellation = 0,
                         Inst< MFDouble    > _weight = 0,
                         Inst< SFBool      > _solid = 0,
                         Inst< SFBool      > _uClosed = 0,
                         Inst< SFBool      > _vClosed = 0,
                         Inst< SFInt32     > _uDimension = 0,
                         Inst< SFInt32     > _vDimension = 0,
                         Inst< MFDouble    > _uKnot = 0,
                         Inst< MFDouble    > _vKnot = 0,
                         Inst< SFInt32     > _uOrder = 0,
                         Inst< SFInt32     > _vOrder = 0 );

    /// Function called by render to render the small part that differs
    /// between NurbsPatchSurface and NurbsTrimmedSurface. The arguments
    /// are things that are needed to render that difference.
    virtual void renderBetweenBeginEnd(
                                GLUnurbsObj *nurbs_object,
                                X3DTextureCoordinateNode *tex_coord_node,
                                TextureCoordinateGenerator *tex_coord_gen,
                                NurbsTextureCoordinate *nurbs_tex_coord, 
                                GLfloat *texKnotU,
                                GLfloat *texKnotV,
                                GLfloat *texCoordinates, 
                                GLfloat *u_knots, 
                                GLfloat *v_knots, 
                                H3DInt32 &sizeOfVertex,
                                GLfloat *withWeights, 
                                H3DInt32 &uSizeToUse, 
                                H3DInt32 &vSizeToUse, 
                                H3DInt32 &map2Vertex3Or4 );

    /// This field adds specified Contour2D nodes to the trimmingContour field.
    ///
    /// <b>Access type:</b> inputOnly \n
    /// 
    /// \dotfile NurbsTrimmedSurface_addTrimmingContour.dot 
    auto_ptr< AddContour >  addTrimmingContour;

    /// This field removes Contour2D nodes from the trimmingContour field.
    ///
    /// <b>Access type:</b> inputOnly \n
    /// 
    /// \dotfile NurbsTrimmedSurface_removeTrimmingContour.dot 
    auto_ptr< RemoveContour >  removeTrimmingContour;

    /// The trimmingContour field, if specified, shall contain a set of 
    /// Contour2D nodes.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// 
    /// \dotfile NurbsTrimmedSurface_trimmingContour.dot 
    auto_ptr< CheckContour >  trimmingContour;
    
    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  };
}

#endif
