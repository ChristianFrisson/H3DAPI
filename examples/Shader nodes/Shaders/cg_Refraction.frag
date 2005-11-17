float4 main(in float3 refractVec : TEXCOORD0,
in float3 reflectVec : TEXCOORD1,
in float3 fresnelTerm : COLOR0,
uniform float Transparency,
uniform samplerCUBE EnvMap,
uniform float enableRefraction,
uniform float enableFresnel) : COLOR
{
float3 refractColor = texCUBE(EnvMap,
refractVec).rgb;
float3 reflectColor = texCUBE(EnvMap,
reflectVec).rgb;
float3 reflectRefract = lerp(refractColor, reflectColor,
fresnelTerm);
float3 finalColor = enableRefraction ?
(enableFresnel ? reflectRefract : refractColor) :
(enableFresnel ? reflectColor : fresnelTerm);
return float4(finalColor, 1.0 - Transparency);
}
