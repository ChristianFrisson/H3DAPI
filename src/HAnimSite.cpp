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
/// \file HAnimSite.cpp
/// \brief CPP file for HAnimSite, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/HAnimSite.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase HAnimSite::database( 
                                    "HAnimSite", 
                                    &(newInstance<HAnimSite>), 
                                    typeid( HAnimSite ),
                                    &Transform::database );

namespace HAnimSiteInternals {
  FIELDDB_ELEMENT( HAnimSite, name, INPUT_OUTPUT );
}


HAnimSite::HAnimSite( 
                     Inst< AddChildren > _addChildren,
                     Inst< RemoveChildren > _removeChildren,
                     Inst< SFVec3f    > _center,
                     Inst< MFChild    > _children,
                     Inst< SFNode     > _metadata,
                     Inst< SFRotation > _rotation,
                     Inst< SFVec3f    > _scale,
                     Inst< SFRotation > _scaleOrientation,
                     Inst< SFVec3f    > _translation,
                     Inst< SFBound    > _bound,
                     Inst< SFVec3f    > _bboxCenter,
                     Inst< SFVec3f    > _bboxSize,
                     Inst< SFTransformedBound > _transformedBound,
                     Inst< Matrix > _matrix,
                     Inst< SFMatrix4f > _accumulatedForward,
                     Inst< SFMatrix4f > _accumulatedInverse,
                     Inst< SFString    > _name ):
  Transform( _addChildren, _removeChildren, 
             _center, _children, _metadata, 
             _rotation, _scale, _scaleOrientation, _translation,
             _bound, _bboxCenter, _bboxSize, 
             _transformedBound, _matrix,
             _accumulatedForward, _accumulatedInverse ),
  name             ( _name      ) {

  type_name = "HAnimSite";
  database.initFields( this );

}

//void HAnimSite::render() {
//}

//void HAnimSite::traverseSG( TraverseInfo &ti ) {
//}



