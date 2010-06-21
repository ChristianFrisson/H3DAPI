varying vec3 normal, eyeVec;
varying vec3 lightDir[2];
uniform sampler2D texture;
uniform sampler2D spec_map;
uniform bool spotOn;
const float cos_outer_cone_angle = 0.97; //cos(0.25)

void main() {
  //vec4 final_color = 
  //  (texture2D( texture, gl_TexCoord[0] ) * gl_FrontMaterial.ambient) + 
  //  (gl_LightSource[0].ambient * gl_FrontMaterial.ambient) +
  //  (gl_LightSource[1].ambient * gl_FrontMaterial.ambient);
  vec4 texture_color = texture2D( texture, gl_TexCoord[0].st );
  float spec_factor = texture2D( spec_map, gl_TexCoord[0].st );
  vec4 final_color = vec4( 0, 0, 0, 1 );

  vec3 N = normalize(normal);
  
  vec3 L[2];
  L[0] = normalize(lightDir[0]);
  float lambert_term[2];
  lambert_term[0] = dot(N,L[0]);

  if( lambert_term[0] > 0.0 ) {
    final_color += gl_LightSource[0].diffuse * 
                   texture_color * 
                   lambert_term[0];
    vec3 R = reflect( -L[0], N );
    vec3 E = normalize(eyeVec);
    float specular = pow( max(dot(R, E), 0.0), 
                     gl_FrontMaterial.shininess ) * spec_factor;
    final_color += gl_LightSource[0].specular * 
                   gl_FrontMaterial.specular * 
                   specular;
  }


  if( spotOn ) {
    L[1] = normalize(lightDir[1]);
    lambert_term[1] = dot(N,L[1]);
    vec3 D = normalize( gl_LightSource[1].spotDirection );

    float cos_cur_angle = dot(-L[1], D);

    float cos_inner_cone_angle = gl_LightSource[1].spotCosCutoff;

    float cos_inner_minus_outer_angle = 
          cos_inner_cone_angle - cos_outer_cone_angle;

    // Precompute falloff, called spot
    float spot = 0.0;
    spot = clamp((cos_cur_angle - cos_outer_cone_angle) / 
           cos_inner_minus_outer_angle, 0.0, 1.0);

    if( lambert_term[1] > 0.0 ) {
      float intensity = 0.5;
      final_color += gl_LightSource[1].diffuse * 
                     gl_FrontMaterial.diffuse * 
                     lambert_term[1] * intensity * spot;
      vec3 R = reflect( -L[1], N );
      vec3 E = normalize(eyeVec);
      float specular = pow( max(dot(R, E), 0.0), 
                       gl_FrontMaterial.shininess );
      final_color += gl_LightSource[1].specular * 
                     gl_FrontMaterial.specular * 
                     specular;


    }
  }

  gl_FragColor = final_color;
//    gl_FragColor =  texture2D( texture, gl_TexCoord[0] );
}
