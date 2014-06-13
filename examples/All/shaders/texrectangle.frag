#version 150 compatibility
#extension GL_ARB_texture_rectangle : enable

uniform sampler2DRect texRect;

void main(void ) {
  ivec2 size = textureSize( texRect, 0);
  gl_FragColor = texture2DRect( texRect, vec2( size.x * gl_TexCoord[0].s, size.y * gl_TexCoord[0].t ) );
}
