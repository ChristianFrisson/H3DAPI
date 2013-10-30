#version 120
#extension GL_ARB_texture_rectangle : enable

uniform sampler2DRect texRect[1];

void main(void ) {
  gl_FragColor = texture2DRect( texRect[0], gl_TexCoord[0].st );
}
