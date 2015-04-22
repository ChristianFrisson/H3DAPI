#version 430 compatibility 
uniform sampler2DMS texture;
uniform int nr_samples;
void main() {
  vec4 color = vec4(0) ;
  vec2 texel_position = (textureSize(texture)*gl_TexCoord[0].st);
  for(int i = 0; i < nr_samples; ++i){
    color += texelFetch(texture, ivec2(texel_position), i);
  }
  gl_FragColor = color/nr_samples;
}