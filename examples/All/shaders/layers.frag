#version 430 compatibility

void main(void ) {
  if(gl_Layer==1){
    gl_FragColor = vec4(0,1,0,1);
  }else{
    gl_FragColor = vec4(1,0,0,1);
  }

}