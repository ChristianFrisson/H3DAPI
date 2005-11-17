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

uniform vec3 LightPosition;

void main(void)
{
    vec3 ecPosition = vec3 (gl_ModelViewMatrix * gl_Vertex);
    vec3 light_pos = vec3( vec4( LightPosition - vec3( 0, 0, 0.6 ), 1) );
    vec3 tnorm      = normalize( gl_NormalMatrix * gl_Normal);
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