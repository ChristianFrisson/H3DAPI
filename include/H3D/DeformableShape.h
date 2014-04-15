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
/// \file DeformableShape.h
/// \brief Header file for DeformableShape
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __DEFORMABLESHAPE_H__
#define __DEFORMABLESHAPE_H__

#include <H3D/X3DShapeNode.h>
#include <H3D/Coordinate.h>
#include <H3D/X3DComposedGeometryNode.h>
#include <H3D/H3DCoordinateDeformerNode.h>

namespace H3D {

  /// \ingroup H3DNodes
  /// \brief The DeformableShape is an X3DShapeNode which makes it possible
  /// to deform the geometry of the shape when touching it with a haptics
  /// device.
  ///
  /// The deformer field specifies an H3DCoordinateDeformerNode that
  /// determines how the coordinates should be deformed on contact.
  /// If NULL, no deformation will be done.
  ///
  /// The geometry field must contain an X3DComposedGeometryNode, and the
  /// coordinates in its coord field are the ones that will be deformed.
  /// 
  /// The origCoord field contains the coordinates that the 
  /// X3DComposedGeometryNode had when it was added to the 
  /// X3DComposedGeometryNode. These are the coordinates that was used
  /// before any deformation started.
  ///
  /// The deformedCoord field contains the coordinates after deformation of
  /// current contacts of haptics devices. These are the coordinates that
  /// will be used for graphics rendering.
  ///
  /// The restingCoord fields contain the coordinates that the geometry will
  /// go back to when there are no contacts to the geometry any longer. If
  /// the deformation is non-plastic, this will be the same as origCoord.
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/DeformableShape.x3d">DeformableShape.x3d</a>
  ///     ( <a href="examples/DeformableShape.x3d.html">Source</a> )
  class H3DAPI_API DeformableShape : public X3DShapeNode {
  public:
    typedef TypedSFNode< Coordinate > SFCoordinateNode;
    typedef TypedSFNode< H3DCoordinateDeformerNode > SFCoordinateDeformer;
    
    /// The SFGeometryNode is extended to only accept X3DComposedGeometryNode
    /// nodes.
    class SFGeometryNode: public X3DShapeNode::SFGeometryNode {
      public:
      /// Destructor. Sets the value to NULL in order to get the correct
      /// onAdd and onRemove functions to be called and initialize the
      /// different coord fields in the DeformableShape node when changing
      /// the geometry.
      ~SFGeometryNode() {
        value = NULL;
      }

      protected:
      virtual void onAdd( Node *n ) {
        X3DComposedGeometryNode *cg = 
          dynamic_cast< X3DComposedGeometryNode * >( n );
        if( !cg ) {
          stringstream s;
          s << "Expecting X3DComposedGeometryNode";
          throw InvalidNodeType( n->getTypeName(),
                                 s.str(),
                                 H3D_FULL_LOCATION );
        }
        
        X3DShapeNode::SFGeometryNode::onAdd( n );
        DeformableShape *dg = 
          static_cast< DeformableShape * >( getOwner() );
        if( n  ) {
          Coordinate *cg_coord = 
            dynamic_cast< Coordinate * >( cg->coord->getValue() ); 
          Coordinate *dg_orig_coord = dg->origCoord->getValue(); 
          Coordinate *dg_deformed_coord = dg->deformedCoord->getValue(); 
          Coordinate *dg_resting_coord = dg->restingCoord->getValue(); 
          if( cg_coord ) {
            if( dg_orig_coord ) {
              dg_orig_coord->point->setValue( cg_coord->point->getValue() ); 
            }
            if( dg_deformed_coord ) {
              dg_deformed_coord->point->setValue( cg_coord->point->getValue() ); 
            }
            if( dg_resting_coord ) {
              dg_resting_coord->point->setValue( cg_coord->point->getValue() ); 
            }
          }
        }
      }
    };

    /// Constructor.       
    DeformableShape( Inst< SFAppearanceNode > _appearance     = 0,
                     Inst< SFGeometryNode   > _geometry       = 0,
                     Inst< SFHapticGeometry > _hapticGeometry = 0,
                     Inst< SFNode           > _metadata       = 0,
                     Inst< SFBound          > _bound          = 0,
                     Inst< SFVec3f          > _bboxCenter     = 0,
                     Inst< SFVec3f          > _bboxSize       = 0,
                     Inst< SFCoordinateNode > _origCoord      = 0,
                     Inst< SFCoordinateNode > _restingCoord   = 0,
                     Inst< SFCoordinateNode > _deformedCoor   = 0,
                     Inst< SFCoordinateDeformer > _deformer   = 0 );

    /// Traverse the scenegraph. The deformation parameters are updated
    /// in the CoordinateDeformer in coordinateDeformer field.
    /// \param ti The TraverseInfo object containing information about the
    /// traversal.
    virtual void traverseSG( TraverseInfo &ti );

    /// The origCoord field contains the coordinates that the 
    /// X3DComposedGeometryNode had when it was added to the 
    /// X3DComposedGeometryNode. These are the coordinates that was used
    /// before any deformation started.
    ///
    /// <b> Access type: </b> outputOnly
    auto_ptr< SFCoordinateNode > origCoord;

    /// The restingCoord fields contain the coordinates that the geometry will
    /// go back to when there are no contacts to the geometry any longer. If
    /// the deformation is non-plastic, this will be the same as origCoord.
    ///
    /// <b> Access type: </b> outputOnly
    auto_ptr< SFCoordinateNode > restingCoord;

    /// The deformedCoord field contains the coordinates after deformation of
    /// current contacts of haptics devices. These are the coordinates that
    /// will be used for graphics rendering.
    ///
    /// <b> Access type: </b> outputOnly
    auto_ptr< SFCoordinateNode > deformedCoord;   

    /// The deformer field specifies an H3DCoordinateDeformerNode that
    /// determines how the coordinates should be deformed on contact
    /// If NULL, no deformation will be done.
    ///
    /// <b>Access type:</b> inputOutput \n
    auto_ptr< SFCoordinateDeformer > deformer;   

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;

  protected:
    TraverseInfo *last_ti;
    
  };
}

#endif
