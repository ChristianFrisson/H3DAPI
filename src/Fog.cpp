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
/// \file Fog.cpp
/// \brief CPP file for Fog, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/Fog.h>
#include <GL/glew.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase Fog::database( "Fog", 
                               &(newInstance<Fog>), 
                               typeid( Fog ),
                               &X3DBindableNode::database  );

namespace FogInternals {
  FIELDDB_ELEMENT( Fog, color, INPUT_OUTPUT );
  FIELDDB_ELEMENT( Fog, fogType, INPUT_OUTPUT );
  FIELDDB_ELEMENT( Fog, visibilityRange, INPUT_OUTPUT );
}


Fog::Fog( Inst< SFSetBind > _set_bind,
          Inst< SFNode    > _metadata,
          Inst< SFTime    > _bindTime,
          Inst< SFBool    > _isBound,
          Inst< SFColor   > _color,
          Inst< SFString  > _fogType,
          Inst< SFFloat   > _visibilityRange ) :
  X3DBindableNode( "Fog", _set_bind, _metadata, _bindTime, _isBound ),
  X3DFogObject( _color, _fogType, _visibilityRange ) {
  
  type_name = "Fog";
  database.initFields( this );
}

void Fog::renderFog() {
  const RGB &fog_color = color->getValue(); 
  H3DFloat visibility_range = visibilityRange->getValue();
  if( visibility_range != 0.0f ) {
    GLfloat gl_fog_color[] = { fog_color.r, fog_color.g, fog_color.b, 0.0f };
    const string &type = fogType->getValue();
    if( type == "LINEAR" ) {
      glFogi( GL_FOG_MODE, GL_LINEAR );
    } else if( type == "EXPONENTIAL" ) {
      // Don't think is is possible to set the density to work like x3d spec.
      // Setting it so that the exponent is the same at z = visibility_range / 2 => (exponent=1)
      glFogf( GL_FOG_DENSITY, 2 / (scale_local_to_global * visibility_range) );
      glFogi( GL_FOG_MODE, GL_EXP );
    } else {
      Console(3) << "Invalid fogType \"" << type << "\". Must be one of \"LINEAR\""
                 << " or \"EXPONENTIAL\" (in \"" << getName() << "\" node )" << endl;
    }
    
    glFogfv( GL_FOG_COLOR, gl_fog_color );
    glFogf( GL_FOG_START, 0.0f );
    glFogf( GL_FOG_END, scale_local_to_global * visibility_range );
    glEnable( GL_FOG );
  }
}
