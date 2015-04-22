#version 430 compatibility

uniform int arrayIndex;
uniform int nr_samples;
uniform sampler2DMSArray texArray;

void main(void ) {
  vec2 texel_position = (textureSize(texArray).xy*gl_TexCoord[0].st);
  vec4 color = vec4(0);
  for(int i = 0; i < nr_samples; ++i){
    color += texelFetch(texArray, ivec3(texel_position, arrayIndex), i);
  }
  
  gl_FragColor = color / nr_samples;
}
