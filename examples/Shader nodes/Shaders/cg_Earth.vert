


void main( in float4 pos: POSITION,
             in float3 normal: NORMAL,
             in float3 tex_coord: TEXCOORD0,
             out float4 pos_out: POSITION,
             out float3 color: COLOR,
             out float3 tex_coord_out: TEXCOORD0,
             uniform float3 lightPosition,
             uniform float3 viewpointPosition,
             uniform float4x4 viewpointOrn,
             uniform float4x4 viewpointOrnInv  )
{
    float3 ecPosition = float3 ( mul( mul( viewpointOrn, glstate.matrix.modelview[0] ), pos
             ) );
    float3 light_pos = lightPosition - viewpointPosition;
    float4x4 normal_matrix = transpose( mul( viewpointOrnInv, glstate.matrix.inverse.modelview[0]) );
    float3 tnorm = float3( normalize( mul( normal_matrix, float4(
             normal, 0 ) ) ) );
    float3 lightVec   = normalize(light_pos - ecPosition);
    float3 reflectVec = reflect(-light_pos, tnorm);
    float3 viewVec    = normalize(-ecPosition);

    float spec      = clamp(dot(reflectVec, viewVec), 0.0, 1.0);
    spec            = pow(spec, 8.0);
    color.x        = float3 (spec) * float3 (1.0, 0.941, 0.898) * 0.3;

    color.y        = max(dot(lightVec, tnorm), 0.0);

    tex_coord_out   = tex_coord;
    pos_out =  mul(glstate.matrix.mvp, pos );
}