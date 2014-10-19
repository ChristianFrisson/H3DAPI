#version 430 compatibility
//begin_preprocessordefines
//end_preprocessordefines

//begin_declarations
layout(location=0) in vec3 fPosition;
layout(location=1) in vec3 fNormal;
//end_declarations

void main()
{
  gl_FragColor = vec4(1,0,0,1);
}