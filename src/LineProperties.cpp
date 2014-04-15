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
/// \file LineProperties.cpp
/// \brief CPP file for LineProperties, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/LineProperties.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase LineProperties::database( 
                                         "LineProperties", 
                                         &(newInstance<LineProperties>), 
                                         typeid( LineProperties ),
                                         &X3DAppearanceChildNode::database );

namespace LinePropertiesInternals {
  FIELDDB_ELEMENT( LineProperties, applied, INPUT_OUTPUT );
  FIELDDB_ELEMENT( LineProperties, linetype, INPUT_OUTPUT );
  FIELDDB_ELEMENT( LineProperties, linewidthScaleFactor, INPUT_OUTPUT );
}


LineProperties::LineProperties( Inst< DisplayList > _displayList,
                                Inst< SFNode  > _metadata,
                                Inst< SFBool  > _applied,
                                Inst< SFInt32 > _linetype,
                                Inst< SFFloat > _linewidthScaleFactor ) :
  X3DAppearanceChildNode( _displayList, _metadata ),
  applied               ( _applied                ),
  linetype              ( _linetype               ),
  linewidthScaleFactor  ( _linewidthScaleFactor   ) {

  type_name = "LineProperties";
  database.initFields( this );

  applied->setValue( true );
  linetype->setValue( 1 );
  linewidthScaleFactor->setValue( 0 );

  applied->route( displayList );
  linetype->route( displayList );
  linewidthScaleFactor->route( displayList );
}


void LineProperties::render() {
  if( applied->getValue() ) {
    H3DFloat width = linewidthScaleFactor->getValue();
    
    int line_type = linetype->getValue();
    
    if( width <= 0 )
      width = 1;
    glLineWidth( width );
   
    if( line_type != 1 ) { 
      // not solid
      glEnable( GL_LINE_STIPPLE );
      if( line_type == 2 )
        // dashed
        glLineStipple( (GLint)width, 0x7777 );
      else if( line_type == 3 )
        // dotted
        glLineStipple( (GLint)width, 0x1111 );
      else if( line_type == 4 )
        // dashed-dotted
        glLineStipple( (GLint)width, 0x087f ); 
      else if( line_type == 5 )
        //dash-dot-dot
        glLineStipple( (GLint)width, 0x24ff );
      else {
        Console(3) << "Warning: linetype " << line_type << " not supported. Using "
                   << "linetype 1 (solid) instead. Supported line types are "
                   << "1, 2, 3, 4 and 5 (in "  << getName() << "). "<< endl;
      }
    }
  }
}
