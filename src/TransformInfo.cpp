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
/// \file TransformInfo.cpp
/// \brief CPP file for TransformInfo, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <TransformInfo.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase TransformInfo::database( 
                        "TransformInfo", 
                        &(newInstance<TransformInfo>), 
                        typeid( TransformInfo ),
                        &X3DChildNode::database );

namespace TransformInfoInternals {
  FIELDDB_ELEMENT( TransformInfo, accForwardMatrix, OUTPUT_ONLY );
  FIELDDB_ELEMENT( TransformInfo, accInverseMatrix, OUTPUT_ONLY );
}

TransformInfo::TransformInfo( Inst< SFNode     > _metadata,
                              Inst< SFMatrix4f > _accForwardMatrix,
                              Inst< SFMatrix4f > _accInverseMatrix ) :
  X3DChildNode( _metadata ),
  accForwardMatrix( _accForwardMatrix ),
  accInverseMatrix( _accInverseMatrix ) {

  type_name = "TransformInfo";
  database.initFields( this );
}

void TransformInfo::traverseSG( TraverseInfo &ti ) {
  X3DChildNode::traverseSG( ti );

  const Matrix4f &acc_frw = ti.getAccForwardMatrix();
  const Matrix4f &acc_inv = ti.getAccInverseMatrix();

  if( acc_frw != accForwardMatrix->getValue() )
    accForwardMatrix->setValue( acc_frw, id );

  if( acc_inv != accInverseMatrix->getValue() )
    accInverseMatrix->setValue( acc_inv, id );
}
