varying vec3 normal, eyeVec;
varying vec3 lightDir[2];
uniform bool spotOn;
  
void main() {
  normal = gl_NormalMatrix * gl_Normal;
  vec4 clip_vertex = gl_ModelViewMatrix * gl_Vertex;
  eyeVec = -vec3(clip_vertex.xyz);
  lightDir[0] = vec3(gl_LightSource[0].position.xyz + eyeVec.xyz);
  if( spotOn ) {
    lightDir[1] = vec3(gl_LightSource[1].position.xyz + eyeVec.xyz);
  }

  gl_Position = ftransform();
  
  // this must be uncommented on ATI graphics cards until they fixed their drivers
  // also does not work on my MacBook pro since apparantly I can only set two attributes
  // and glTexCoord and gl_Position is already used. Commenting out gl_TexCoord and using
  // gl_ClipVertex instead will make clipping work, but texturing to fail
  gl_ClipVertex = clip_vertex;
  gl_TexCoord[0] = gl_MultiTexCoord0;

} 

