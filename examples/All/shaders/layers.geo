#version 430 compatibility

layout(triangles) in;
layout(triangle_strip, max_vertices = 6) out;


void main() {

  // Layer 0
  gl_Position= gl_in[0].gl_Position;
  gl_Layer= 0;
  EmitVertex();

  gl_Position= gl_in[1].gl_Position;
  gl_Layer= 0;
  EmitVertex();

  gl_Position= gl_in[2].gl_Position;
  gl_Layer= 0;
  EmitVertex();

  // Layer 1
  gl_Position= gl_in[0].gl_Position;
  gl_Layer= 1;
  EmitVertex();

  gl_Position= gl_in[1].gl_Position;
  gl_Layer= 1;
  EmitVertex();

  gl_Position= gl_in[2].gl_Position;
  gl_Layer= 1;
  EmitVertex();
}
