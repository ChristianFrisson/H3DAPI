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
/// \file MultiTextureTransform.cpp
/// \brief CPP file for MultiTextureTransform, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/MultiTextureTransform.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase MultiTextureTransform::database( 
                                       "MultiTextureTransform", 
                                       &(newInstance<MultiTextureTransform>), 
                                       typeid( MultiTextureTransform ),
                                       &X3DTextureTransformNode::database );

namespace MultiTextureTransformInternals {
  FIELDDB_ELEMENT( MultiTextureTransform, textureTransform, INPUT_OUTPUT );
}

MultiTextureTransform::MultiTextureTransform( 
                 Inst< DisplayList            > _displayList,
                 Inst< SFNode                 > _metadata,
                 Inst< MFTextureTransformNode > _textureTransform ) :
  X3DTextureTransformNode( _displayList, _metadata ),
  textureTransform( _textureTransform ) {
  
  type_name = "MultiTextureTransform";
  database.initFields( this );
  textureTransform->route( displayList, id );
}


void MultiTextureTransform::renderForTextureUnit( unsigned int texture_unit ) {
  if( textureTransform->size() == 0 ) return;
  X3DTextureTransformNode *tt = 
    textureTransform->getValueByIndex( texture_unit );
  if( tt ) tt->renderForTextureUnit( texture_unit );

}
void MultiTextureTransform::renderForTextureUnits( unsigned int start_unit,
                                                   unsigned int end_unit ) {
  if( textureTransform->size() == 0 ) return;
  
  // the maximum index that can be used in textureTransform for rendering
  unsigned int max_index = textureTransform->size() - 1;

  if( end_unit > max_index ) {
    unsigned int start_index =  start_unit + end_unit - max_index;
    X3DTextureTransformNode *tt = 
      static_cast< X3DTextureTransformNode * >( textureTransform->back() );
    if( tt ) tt->renderForTextureUnits( start_index,
                                        end_unit );
    end_unit = start_index - 1; 
  }
  
  for( unsigned int i = start_unit; i <= end_unit; ++i ) {
    X3DTextureTransformNode *tt = textureTransform->getValueByIndex( i );
    if( tt ) tt->renderForTextureUnit( i );
  }
}

void MultiTextureTransform::render() {
  stringstream s;
  s << "Cannot use render() function in MultiTextureTransform. Use "
    << "renderForTextureUnit() instead to specify which texture "
    << "to render for. ";
  throw Exception::H3DAPIException( s.str(), H3D_FULL_LOCATION );
}

