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
/// \file ShadowTransform.cpp
/// \brief CPP file for ShadowTransform
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/ShadowTransform.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase ShadowTransform::database( "ShadowTransform", 
             &(newInstance<ShadowTransform>), 
             typeid( ShadowTransform ),
             &H3DShadowObjectNode::database );

namespace ShadowTransformInternals {
  FIELDDB_ELEMENT( ShadowTransform, shadowVolume, INPUT_OUTPUT );
  FIELDDB_ELEMENT( ShadowTransform, transform, INPUT_OUTPUT );
}

ShadowTransform::ShadowTransform( Inst< SFNode>  _metadata,
          Inst< SFTransformNode > _transform,
          Inst< MFShadowObjectNode > _shadowVolume ) :
  H3DShadowObjectNode( _metadata, _transform ),
  shadowVolume( _shadowVolume ) {

  type_name = "ShadowTransform";
  database.initFields( this );

}

void ShadowTransform::renderShadow( X3DLightNode *light, 
           bool draw_caps,
           const Matrix4f &local_to_global ) {
  
  MatrixTransform *t = transform->getValue();
  for( MFShadowObjectNode::const_iterator i = shadowVolume->begin();
       i != shadowVolume->end(); i++ ) {
    H3DShadowObjectNode *shadow_volume = static_cast< H3DShadowObjectNode * >(*i);
  
    if( shadow_volume ) {
      Matrix4f m = t ? local_to_global * t->matrix->getValue() : local_to_global;
      shadow_volume->renderShadow( light, draw_caps, m );
    } 
  }
}
