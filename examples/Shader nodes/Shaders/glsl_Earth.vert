//
// Vertex shader for drawing the earth with multiple textures
//
// Author: Randi Rost
//
// Copyright (c) 2002-2005 3Dlabs Inc. Ltd.
//
// See 3Dlabs-License.txt for license information
//

varying float Diffuse;
varying vec3  Specular;
varying vec2  TexCoord;

uniform vec3 lightPosition;
uniform vec3 viewpointPosition;
uniform mat4 viewpointOrn;

void main(void)
{
    vec3 ecPosition = vec3 ((viewpointOrn * gl_ModelViewMatrix) * gl_Vertex);
    vec3 light_pos = vec3( vec4( lightPosition - viewpointPosition, 1) );
    mat3 viewpointOrn3x3;
    viewpointOrn3x3[0][0] = viewpointOrn[0][0];
    viewpointOrn3x3[0][1] = viewpointOrn[0][1];
    viewpointOrn3x3[0][2] = viewpointOrn[0][2];
    viewpointOrn3x3[1][0] = viewpointOrn[1][0];
    viewpointOrn3x3[1][1] = viewpointOrn[1][1];
    viewpointOrn3x3[1][2] = viewpointOrn[1][2];
    viewpointOrn3x3[2][0] = viewpointOrn[2][0];
    viewpointOrn3x3[2][1] = viewpointOrn[2][1];
    viewpointOrn3x3[2][2] = viewpointOrn[2][2];
    vec3 tnorm      = normalize( (viewpointOrn3x3 * gl_NormalMatrix ) * gl_Normal);
    vec3 lightVec   = normalize(light_pos - ecPosition);
    vec3 reflectVec = reflect(-light_pos, tnorm);
    vec3 viewVec    = normalize(-ecPosition);

    float spec      = clamp(dot(reflectVec, viewVec), 0.0, 1.0);
    spec            = pow(spec, 8.0);
    Specular        = vec3 (spec) * vec3 (1.0, 0.941, 0.898) * 0.3;

    Diffuse         = max(dot(lightVec, tnorm), 0.0);

    TexCoord        = gl_MultiTexCoord0.st;
    gl_Position     = ftransform();
}