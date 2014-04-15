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
/// \file LocalFog.cpp
/// \brief CPP file for LocalFog, X3D scene-graph node
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/LocalFog.h>
#include <GL/glew.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase LocalFog::database( "LocalFog", 
                                    &(newInstance<LocalFog>), 
                                    typeid( LocalFog ),
                                    &X3DChildNode::database  );

namespace LocalFogInternals {
  FIELDDB_ELEMENT( LocalFog, color, INPUT_OUTPUT );
  FIELDDB_ELEMENT( LocalFog, fogType, INPUT_OUTPUT );
  FIELDDB_ELEMENT( LocalFog, visibilityRange, INPUT_OUTPUT );
  FIELDDB_ELEMENT( LocalFog, enabled, INPUT_OUTPUT );
}

LocalFog::LocalFog( Inst< SFNode    > _metadata,
                    Inst< SFColor   > _color,
                    Inst< SFString  > _fogType,
                    Inst< SFFloat   > _visibilityRange,
                    Inst< SFBool   > _enabled ) :
  X3DChildNode( _metadata ),
  X3DFogObject( _color, _fogType, _visibilityRange ),
  H3DRenderStateObject(),
  enabled( _enabled ) {
  
  type_name = "LocalFog";
  database.initFields( this );
  enabled->setValue( true );
}

void LocalFog::enableGraphicsState() {
  glPushAttrib( GL_FOG_BIT );
  if( !enabled->getValue() )
    return;
  H3DFloat visibility_range = visibilityRange->getValue();
  if( visibility_range != 0.0f ) {
    const RGB &localFog_color = color->getValue(); 
    GLfloat gl_localFog_color[] = { localFog_color.r, 
                                    localFog_color.g, 
                                    localFog_color.b, 
                                    0.0f };
    const string &type = fogType->getValue();
    if( type == "LINEAR" ) {
      glFogi( GL_FOG_MODE, GL_LINEAR );
    } else if( type == "EXPONENTIAL" ) {
      // Don't think is is possible to set the density to work like x3d spec.
      // Setting it so that the exponent is the same at z = visibility_range / 2 => (exponent=1)
      glFogf( GL_FOG_DENSITY, 2 / visibilityRange->getValue() );
      glFogi( GL_FOG_MODE, GL_EXP );
    } else {
      Console(3) << "Invalid fogType \"" << type << "\". Must be one of \"LINEAR\""
                 << " or \"EXPONENTIAL\" (in \"" << getName() << "\" node )" << endl;
    }
    
    glFogfv( GL_FOG_COLOR, gl_localFog_color );
    glFogf( GL_FOG_START, 0.0f );
    glFogf( GL_FOG_END, visibilityRange->getValue() );
    glEnable( GL_FOG );
  }
}

void LocalFog::disableGraphicsState() {
  glPopAttrib();
}
