#version 430 compatibility

//begin_preprocessordefines
//end_preprocessordefines

//begin_declarations
layout(location = 0) out vec4 position_mv;
layout(location = 1) out vec3 oNormal;
//end_declarations

void main()
{
//begin_body
  gl_Position.xyz = gl_Vertex.xyz;
  position_mv = (gl_ModelViewMatrix * vec4(gl_Vertex.xyz,1.0));
  oNormal = gl_Normal;
//end_body
}