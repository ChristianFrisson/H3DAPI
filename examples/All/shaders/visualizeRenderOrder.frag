

#version 420 compatibility
layout(binding = 0, offset = 0) uniform atomic_uint ac;
uniform int width = 518;
uniform int height = 784;
void main(void)
{
  uint counter = atomicCounterIncrement(ac);
  float r = (counter/float(width)) / float(height);
  gl_FragColor = vec4(r, 0, 0, 1);
}