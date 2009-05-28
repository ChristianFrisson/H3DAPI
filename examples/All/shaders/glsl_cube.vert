attribute float temperature;
attribute mat4 transformation;
varying vec3 color;

void main( void ) {
  if ( temperature > 15.0 ) {
    color = vec3( 1.0, 0.0, 0.0 ); 
  } else {
    color = vec3( 1.0, 1.0, 0.0 );
  }
  gl_Position = gl_ModelViewProjectionMatrix * transformation * gl_Vertex;
}