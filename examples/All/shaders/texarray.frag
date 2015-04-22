#version 120
#extension GL_EXT_texture_array : enable

uniform int arrayIndex;
uniform sampler2DArray texArray;

void main(void ) {
  gl_FragColor = texture2DArray( texArray, vec3(gl_TexCoord[0].st, arrayIndex ) );
}
