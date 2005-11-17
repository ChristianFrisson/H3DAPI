void main ( in float3 tex_coord: TEXCOORD0,
            in float3 c: COLOR,
            out float3 color: COLOR0,    
            uniform sampler2D EarthDay,
            uniform sampler2D EarthNight,
            uniform sampler2D EarthCloudGloss  )
{
   float Specular = c.x;
   float Diffuse = c.y;

   float2 clouds = tex2D( EarthCloudGloss, tex_coord ).rg;
   float3 daytime = ( tex2D( EarthDay, tex_coord ).rgb * Diffuse +
            Specular * clouds.g ) * ( 1.0 - clouds.r ) + clouds.r * Diffuse;
   float3 nighttime = tex2D(EarthNight, tex_coord).rgb * 
                         (1.0 - clouds.r) * 2.0;

   color = daytime;

   if (Diffuse <= 0.1) {
     float w =  (Diffuse + 0.1) * 5.0; 
     color = nighttime * (1-w) + daytime * (w);
   }    

}




