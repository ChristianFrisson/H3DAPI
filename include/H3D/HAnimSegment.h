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
/// \file HAnimSegment.h
/// \brief Header file for HAnimSegment, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __HANIMSEGMENT_H__
#define __HANIMSEGMENT_H__

#include <H3D/X3DGroupingNode.h>
#include <H3D/X3DCoordinateNode.h>
#include <H3D/HAnimDisplacer.h>
#include <H3D/MFFloat.h>
#include <H3D/MFInt32.h>

namespace H3D {

  /// \ingroup X3DNodes
  /// \class HAnimSegment
  /// \brief The HAnimSegment object is a container for information
  /// about a segment of the body.
  ///
  /// The name field contains a name  that is used for 
  /// identifying the object.  
  /// 
  /// The displacers field contains a list of HAnimDisplacer objects that are
  /// used to morph the deformable mesh using the morph targets defined
  /// in the Displacer objects.
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/BoxMan.x3d">BoxMan.x3d</a>
  ///     ( <a href="examples/BoxMan.x3d.html">Source</a> )
  ///
  /// \par Internal routes:
  /// \dotfile HAnimSegment.dot
  class H3DAPI_API HAnimSegment : public X3DGroupingNode {
  public:
    
    typedef TypedMFNode< HAnimDisplacer > MFDisplacer;
    typedef TypedSFNode<X3DCoordinateNode > SFCoordinateNode;

    /// Constructor.
    HAnimSegment( Inst< AddChildren >        _addChildren        = 0,
                  Inst< RemoveChildren >     _removeChildren     = 0,
                  Inst< MFChild    >         _children           = 0,
                  Inst< SFNode     >         _metadata           = 0,
                  Inst< SFBound    >         _bound              = 0,
                  Inst< SFVec3f    >         _bboxCenter         = 0,
                  Inst< SFVec3f    >         _bboxSize           = 0,
                  Inst< SFVec3f    >         _centerOfMass       = 0,
                  Inst< SFCoordinateNode >   _coord              = 0,
                  Inst< MFDisplacer >        _displacers         = 0,
                  Inst< SFFloat     >        _mass               = 0,
                  Inst< MFFloat     >        _momentsOfInertia   = 0,
                  Inst< SFString    >        _name               = 0
                  );

    //virtual void render();
    virtual void traverseSG( TraverseInfo &ti );

    /// The X3D specification does not describe this field at all.
    /// The H3D team have no idea what it is supposed to do and as such it
    /// is ignored.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 0 0 0 \n
    /// \dotfile HAnimSegment_centerOfMass.dot
    auto_ptr< SFVec3f    > centerOfMass;

    /// Contains the coordinate node for the segment. The HAnimDisplacer
    /// nodes in the displacer field will morph the coordinates contained
    /// in the point field of the coordinate node.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 0 0 0 \n
    /// \dotfile HAnimSegment_coord.dot
    auto_ptr< SFCoordinateNode > coord;

    /// The displacers field contains a list of HAnimDisplacer objects that are
    /// used to morph the deformable mesh using the morph targets defined
    /// in the Displacer objects.
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> [] \n
    /// 
    /// \dotfile HAnimSegment_displacers.dot  
    auto_ptr< MFDisplacer >  displacers;

    /// The X3D specification does not describe this field at all.
    /// The H3D team have no idea what it is supposed to do and as such it
    /// is ignored.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 0 \n
    /// \dotfile HAnimSegment_mass.dot
    auto_ptr< SFFloat > mass;

    /// The X3D specification does not describe this field at all.
    /// The H3D team have no idea what it is supposed to do and as such it
    /// is ignored.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 0 0 0 0 0 0 0 0 0 \n
    /// \dotfile HAnimSegment_momentsOfInertia.dot
    auto_ptr< MFFloat > momentsOfInertia;

    /// The name field contains a name  that is used for 
    /// identifying the object. 
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> "" \n
    /// 
    /// \dotfile HAnimSegment_name.dot 
    auto_ptr< SFString    >  name;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  protected:
    /// vector for storing the original points before joint and displacer
    /// modification if the original coord data is double precision.
    vector< Vec3d > points_double;
    
    /// vector for storing the original points before joint and displacer
    /// modification if the original coord data is single precision.
    vector< Vec3f > points_single;
    
    /// The coordinate node that was used as base coordinate in last
    /// traverseSG
    AutoRef< X3DCoordinateNode > current_coordinate;

    /// Template function to apply the displacements 
    /// to segment coordinates. Since the points can
    /// be both floats or doubles a template is used.
    /// \param points The original points before any deformation. The 
    /// displacemet will be added.
    template< class VectorType >
    inline void updateCoordinates( VectorType &points );
  };
}

#endif
