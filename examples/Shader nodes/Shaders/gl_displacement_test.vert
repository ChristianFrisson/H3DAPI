#version 150 compatibility
#extension GL_ARB_separate_shader_objects : enable

in vec2 CD_MTFACE_att0;
in vec4 CD_TANGENT_att1;

layout(location = 0)   out vec3 oNormal;
layout(location = 1)   out vec2 oTexCoord;
layout(location = 2)   out vec4 oTangent;

void main()
{
  gl_Position.xyz = gl_Vertex.xyz;
  oNormal = gl_Normal;
  oTexCoord = CD_MTFACE_att0;
  oTangent = CD_TANGENT_att1;
}

