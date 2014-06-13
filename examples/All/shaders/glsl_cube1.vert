#version 120
attribute mat3 attrib;
varying vec3 color;

float calib( float x0, float x2, float x3 ) {
  return (x0 + x2 + x3)/3.0;
}

void main( void ) {
  mat3 a = mat3(attrib);
  float r = calib( a[0][0], a[0][1], a[0][2] );
  float g = calib( a[1][0], a[1][1], a[1][2] );
  float b = calib( a[2][0], a[2][1], a[2][2] );
  color = vec3(r, g, b);
  
  gl_Position = ftransform();
}
