//////////////////////////////////////////////////////////////////////////////
//    Copyright 2004, SenseGraphics AB
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
/// \file ShaderFunctions.h
/// \brief Header file for help functions used by shader nodes.
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __SHADERFUNCTIONS_H__
#define __SHADERFUNCTIONS_H__

#include "H3DDynamicFieldsObject.h"
#include <GL/glew.h>

#ifdef HAVE_CG
#include <Cg/cg.h>
#include <Cg/cgGL.h>
#endif

namespace H3D {
  namespace Shaders {
#ifdef HAVE_CG
    CGprofile cgProfileFromString( const string &profile, 
                                   const string &type );

    /// Set the value of a uniform variable in the given CG shader.
    /// The name of the uniform variable is the same as the name of the field. 
    bool setCGUniformVariableValue( CGprogram program_handle,
                                    Field *field );
#endif
    /// Set the value of a uniform variable in the given GLSL shader.
    /// The name of the uniform variable is the same as the name of the field. 
    bool setGLSLUniformVariableValue( GLhandleARB program_handle,
                                      Field *field );

    void renderTextures( H3DDynamicFieldsObject * );
    void postRenderTextures( H3DDynamicFieldsObject * );
    void preRenderTextures( H3DDynamicFieldsObject * );

  }
}

#endif
