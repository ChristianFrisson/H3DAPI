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
/// \file HAnimSegment.cpp
/// \brief CPP file for HAnimSegment, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/HAnimSegment.h>

#ifdef MACOSX
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#ifdef FREEGLUT
#include <GL/freeglut.h>
#endif

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase HAnimSegment::database( 
                                    "HAnimSegment", 
                                    &(newInstance<HAnimSegment>), 
                                    typeid( HAnimSegment ),
                                    &X3DGroupingNode::database );

namespace HAnimSegmentInternals {
  FIELDDB_ELEMENT( HAnimSegment, centerOfMass, INPUT_OUTPUT );
  FIELDDB_ELEMENT( HAnimSegment, coord, INPUT_OUTPUT );
  FIELDDB_ELEMENT( HAnimSegment, displacers, INPUT_OUTPUT );
  FIELDDB_ELEMENT( HAnimSegment, mass, INPUT_OUTPUT );
  FIELDDB_ELEMENT( HAnimSegment, momentsOfInertia, INPUT_OUTPUT );
  FIELDDB_ELEMENT( HAnimSegment, name, INPUT_OUTPUT );
}


HAnimSegment::HAnimSegment( 
                     Inst< AddChildren > _addChildren,
                     Inst< RemoveChildren > _removeChildren,
                     Inst< MFChild    > _children,
                     Inst< SFNode     > _metadata,
                     Inst< SFBound    > _bound,
                     Inst< SFVec3f    > _bboxCenter,
                     Inst< SFVec3f    > _bboxSize,
		     Inst< SFVec3f    > _centerOfMass,
		     Inst< SFCoordinateNode > _coord,
		     Inst< MFDisplacer > _displacers,
		     Inst< SFFloat     > _mass,
		     Inst< MFFloat     > _momentsOfInertia,
		     Inst< SFString    > _name ):
  X3DGroupingNode( _addChildren, _removeChildren,
                   _children, _metadata, 
                   _bound, _bboxCenter, _bboxSize ),
  centerOfMass( _centerOfMass ),
  coord( _coord ),
  displacers( _displacers ),
  mass( _mass ),
  momentsOfInertia( _momentsOfInertia ),
  name             ( _name      ) {

  type_name = "HAnimSegment";
  database.initFields( this );

  centerOfMass->setValue( Vec3f( 0, 0, 0 ) );
  mass->setValue( 0 );
  momentsOfInertia->resize( 9, 0 );
}

//void HAnimSegment::render() {
//}

//void HAnimSegment::traverseSG( TraverseInfo &ti ) {
//}

