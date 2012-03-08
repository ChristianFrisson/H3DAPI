struct inputs {
  float4 Position : POSITION;
  float4 Normal : NORMAL;
};
struct outputs {
  float4 hPosition : POSITION;
  float4 fresnelTerm : COLOR0;
  float4 refractVec : TEXCOORD0;
  float4 reflectVec : TEXCOORD1;
};

// fresnel approximation
fixed fast_fresnel(float3 I, float3 N,
                   float3 fresnelValues) {
  fixed power = fresnelValues.x;
  fixed scale = fresnelValues.y;
  fixed bias = fresnelValues.z;
  return bias + pow(1.0 - dot(I, N), power) * scale;
}
outputs main(inputs IN,
  uniform float4x4 ModelViewProj,
  uniform float4x4 ModelView,
  uniform float4x4 ModelViewIT,
  uniform float theta) {

  ModelViewProj = glstate.matrix.mvp;
  ModelView = glstate.matrix.modelview[0];
  ModelViewIT = transpose( glstate.matrix.inverse.modelview[0] );

  theta = 0;

  outputs OUT;
  OUT.hPosition = mul(ModelViewProj, IN.Position);
  // convert the position and normal into
  // appropriate spaces
  float3 eyeToVert = mul(ModelView, IN.Position).xyz;
  eyeToVert = normalize(eyeToVert);
  float3 normal = mul(ModelViewIT, IN.Normal).xyz;
  normal = normalize(normal);
  OUT.refractVec.xyz = refract(eyeToVert, normal, 0.95);
  OUT.refractVec.w = 1;
  OUT.reflectVec.xyz = reflect(eyeToVert, normal);
  OUT.reflectVec.w = 1;
  // calculate the fresnel reflection
  OUT.fresnelTerm = fast_fresnel(-eyeToVert, normal,
                                 float3(5.0, 1.0, 0.0));
  return OUT;
}

