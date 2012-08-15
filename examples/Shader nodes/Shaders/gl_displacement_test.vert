attribute vec2 CD_MTFACE_att0;
varying vec2 var0;
attribute vec4 CD_TANGENT_att1;
varying vec4 var1;


varying vec3 varposition;
varying vec3 varnormal;

layout(location = 0)   out vec3 oNormal;
layout(location = 1)   out vec2 oTexCoord;
layout(location = 2)   out vec4 oTangent;

void main()
{
	// vec4 co = gl_ModelViewMatrix * gl_Vertex;

	// varposition = co.xyz;
	// varnormal = normalize(gl_NormalMatrix * gl_Normal);
	// gl_Position = gl_ProjectionMatrix * co;

	// var0 = CD_MTFACE_att0;
	// var1.xyz = normalize((gl_ModelViewMatrix * vec4(CD_TANGENT_att1.xyz, 0)).xyz);
	var1.w = CD_TANGENT_att1.w;
	
  gl_Position.xyz = gl_Vertex.xyz;
  oNormal = gl_Normal;
  //oTexCoord = gl_MultiTexCoord0.xy;
  oTexCoord = CD_MTFACE_att0;
  oTangent = CD_TANGENT_att1;
}

