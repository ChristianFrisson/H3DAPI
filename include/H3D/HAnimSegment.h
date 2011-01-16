//////////////////////////////////////////////////////////////////////////////
//    Copyright 2004-2010, SenseGraphics AB
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
  /// \par Internal routes:
  /// \dotfile HAnimSegment.dot
  class H3DAPI_API HAnimSegment : public X3DGroupingNode {
  public:
    
    // TODO
    typedef MFNode MFDisplacer;
    typedef SFNode SFCoordinateNode;

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
    //virtual void traverseSG( TraverseInfo &ti );

    auto_ptr< SFVec3f    > centerOfMass;
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

    auto_ptr< SFFloat > mass;
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
  };
}

#endif
