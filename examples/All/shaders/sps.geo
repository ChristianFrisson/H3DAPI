#version 430 compatibility

//begin_preprocessordefines
#define SPS_SUPPORTED
#define SPS_ENABLED 1
layout(triangles) in;
#if SPS_ENABLED == 1
layout(triangle_strip, max_vertices = 6) out;
#else
layout(triangle_strip, max_vertices = 3) out;
#endif
//end_preprocessordefines
//begin_declarations
layout(location = 0) in vec4 gPosition[];
layout(location = 1) in vec3 gNormal[];

layout(location = 0) out vec3 fPosition;
layout(location = 1) out vec3 fNormal;
//end_declarations

void main() {
  vec4 p0= gl_in[0].gl_Position;
  vec4 p1= gl_in[1].gl_Position;
  vec4 p2= gl_in[2].gl_Position;

  vec3 n0= gNormal[0];
  vec3 n1= gNormal[1];
  vec3 n2= gNormal[2];

//begin_primitive
#if SPS_ENABLED == 1
const float matrixProjShift = 0.1207;
const float matrixViewShift = 0.03f;

float stereo_dx_mv = matrixViewShift;
float stereo_dx_mvp = gl_ProjectionMatrix[0][0] * stereo_dx_mv;
vec4 mvz = vec4(gl_ModelViewMatrix[0].z, gl_ModelViewMatrix[1].z, gl_ModelViewMatrix[2].z, gl_ModelViewMatrix[3].z);

vec4 mvp_p0 = gl_ModelViewProjectionMatrix*p0;
float stereo_dx0 = stereo_dx_mvp + dot(mvz,p0) * matrixProjShift;
vec4 mvp_p1 = gl_ModelViewProjectionMatrix*p1;
float stereo_dx1 = stereo_dx_mvp + dot(mvz,p1) * matrixProjShift;
vec4 mvp_p2 = gl_ModelViewProjectionMatrix*p2;
float stereo_dx2 = stereo_dx_mvp + dot(mvz,p2) * matrixProjShift;
vec3 mvp_n0= normalize(gl_NormalMatrix*n0);
vec3 mvp_n1= normalize(gl_NormalMatrix*n1);
vec3 mvp_n2= normalize(gl_NormalMatrix*n2);

int eye = 1;
for (int i = 0; i<2; ++i){
#endif
  fPosition = gPosition[0].xyz;
#if SPS_ENABLED == 1
  fPosition.x += stereo_dx_mv;
#endif
#if SPS_ENABLED == 1
  fNormal = mvp_n0;
#else
  fNormal = normalize(gl_NormalMatrix*n0);
#endif
#if SPS_ENABLED == 1
  gl_Position = mvp_p0;
  gl_Position.x += stereo_dx0;
  gl_ViewportIndex = eye;
#else
  gl_Position = gl_ModelViewProjectionMatrix*p0;
#endif
  EmitVertex();

  fPosition = gPosition[1].xyz;
#if SPS_ENABLED == 1
  fPosition.x += stereo_dx_mv;
#endif
#if SPS_ENABLED == 1
  fNormal = mvp_n1;
#else
  fNormal = normalize(gl_NormalMatrix*n1);
#endif
#if SPS_ENABLED == 1
  gl_Position = mvp_p1;
  gl_Position.x += stereo_dx1;
  gl_ViewportIndex = eye;
#else
  gl_Position = gl_ModelViewProjectionMatrix*p1;
#endif
  EmitVertex();

  fPosition = gPosition[2].xyz;
#if SPS_ENABLED == 1
  fPosition.x += stereo_dx_mv;
#endif
#if SPS_ENABLED == 1
  fNormal = mvp_n2;
#else
  fNormal = normalize(gl_NormalMatrix*n2);
#endif
#if SPS_ENABLED == 1
  gl_Position = mvp_p2;
  gl_Position.x += stereo_dx2;
  gl_ViewportIndex = eye;
#else
  gl_Position = gl_ModelViewProjectionMatrix*p2;
#endif
  EmitVertex();

  EndPrimitive();
#if SPS_ENABLED == 1
// swap shifts
    stereo_dx_mv   = -stereo_dx_mv;
    stereo_dx0     = -stereo_dx0;
    stereo_dx1     = -stereo_dx1;
    stereo_dx2     = -stereo_dx2;
    ++eye;
  }
#endif
//end_primitive
}
