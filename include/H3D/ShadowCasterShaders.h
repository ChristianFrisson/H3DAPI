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
/// \file ShadowCasterShader.h
/// \brief Header file for ShadowCasterShader.
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __SHADOWCASTERSHADER_H__
#define __SHADOWCASTERSHADER_H__

#include <H3D/ComposedShader.h>



namespace H3D {

  /// Functions for use by the ShadowCaster node to generate a shader to
  /// use for shader volume rendering depening on different options.

  namespace ShadowCasterShaders {
    /// Initialize and enable a shader for the given properties for 
    /// use in a ShadowCaster node.
    void shaderInit( bool cpu_shadows,     
                     bool draw_caps, 
                     bool is_dir_light, 
                     bool single_pass_stereo,
                     float matrixViewShift,
                     float matrixProjShift);

    /// Enable/disable the shader currently in use.
    void shaderToggle( bool on );

    /// Disable the shader and clean up any other settings made by it. 
    void shaderClean();   

    /// Set the point light position in the shader (only valid for geometry shader
    /// version of shader). Returns true on success. 
    bool setPointLightPosition( const Vec3f &pos );

    /// Set the directional light direction in the shader (only valid for geometry shader
    /// version of shader). Returns true on success. 
    bool setDirectionalLightDir( const Vec3f &dir );

    /// Set the transform matrix for the shadow relative to the graphics object
    /// in the shader. Returns true on success.
    bool setTransformMatrix( const Matrix4f & m);

  }
}


#endif
