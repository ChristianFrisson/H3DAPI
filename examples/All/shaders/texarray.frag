#version 120
#extension GL_EXT_texture_array : enable

uniform arrayIndex;
uniform sampler2DArray texArray[1];

void main(void ) {
  gl_FragColor = texture2DArray( texArray[1], vec3(gl_TexCoord[0].st, arrayIndex ) );
}
