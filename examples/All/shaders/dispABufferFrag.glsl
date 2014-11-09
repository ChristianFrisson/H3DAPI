/**
* Fast Single-pass A-Buffer using OpenGL 4.0
* Copyright Cyril Crassin, June 2010
**/

#version 400
#extension GL_NV_gpu_shader5 : enable
#extension GL_EXT_shader_image_load_store : enable
#extension GL_NV_shader_buffer_load : enable
#extension GL_NV_shader_buffer_store : enable
#extension GL_EXT_bindable_uniform : enable

//Macros changed from the C++ side
uniform int SCREEN_WIDTH;
uniform int SCREEN_HEIGHT;
#define BACKGROUND_COLOR_R 1.0f
#define BACKGROUND_COLOR_G 1.0f
#define BACKGROUND_COLOR_B 1.0f
#define ABUFFER_SIZE 16
#define ABUFFER_USE_TEXTURES 1

#define ABUFFER_RESOLVE_USE_SORTING	1

#define ABUFFER_RESOLVE_ALPHA_CORRECTION 0
#define ABUFFER_RESOLVE_GELLY 0

const vec4 backgroundColor=vec4(BACKGROUND_COLOR_R, BACKGROUND_COLOR_G, BACKGROUND_COLOR_B, 0.0f);
const float fragmentAlpha=0.5f;


//Whole number pixel offsets (not necessary just to test the layout keyword !)
//layout(pixel_center_integer) in vec4 gl_FragCoord;

//Input interpolated fragment position
smooth in vec4 fragPos;
//Output fragment color
out vec4 outFragColor;

#if ABUFFER_USE_TEXTURES
//A-Buffer fragments storage
coherent uniform layout(rgba16f) image2DArray abufferImg;
//A-Buffer fragment counter
coherent uniform layout(size1x32) uimage2D abufferCounterImg;
#else
uniform vec4 *d_abuffer;
uniform uint *d_abufferIdx;
#endif


//Local memory array (probably in L1)
vec4 fragmentList[ABUFFER_SIZE];


//Keeps only closest fragment
vec4 resolveClosest(ivec2 coords, int abNumFrag);
//Blend fragments front-to-back
vec4 resolveAlphaBlend(ivec2 coords, int abNumFrag);
//Compute gelly shader
vec4 resolveGelly(ivec2 coords, int abNumFrag);

// void main(void) {
// outFragColor = vec4(1,0,0,1);
// }
//Resolve A-Buffer and blend sorted fragments
void main(void) {
  outFragColor=vec4(0,1,0,1);
  ivec2 coords=ivec2(gl_FragCoord.xy);

  if(coords.x>=0 && coords.y>=0 && coords.x<SCREEN_WIDTH && coords.y<SCREEN_HEIGHT ){

    //Load the number of fragments in the current pixel.
#if ABUFFER_USE_TEXTURES
    int abNumFrag=(int)imageLoad(abufferCounterImg, coords).r;
#else
    int abNumFrag=(int)d_abufferIdx[coords.x+coords.y*SCREEN_WIDTH];

    //Crash without this (WTF ??)
    if(abNumFrag<0 )
      abNumFrag=0;
    if(abNumFrag>ABUFFER_SIZE ){
      abNumFrag=ABUFFER_SIZE;
    }
#endif

    if(abNumFrag > 0){

      //Copute ans output final color for the frame buffer
#if ABUFFER_RESOLVE_USE_SORTING==0	
      //If we only want the closest fragment
      outFragColor=resolveClosest(coords, abNumFrag);
      outFragColor=vec4(0,1,0,1);
      //asdfs
#elif ABUFFER_RESOLVE_GELLY
      //We want to sort and apply gelly shader
      outFragColor=resolveGelly(coords, abNumFrag);
#else
      // this is the place where actually executed
      //We want to sort and blend fragments
      outFragColor=resolveAlphaBlend(coords, abNumFrag);
#endif
    }else{
      //outFragColor = vec4(0.5,0.5,0.5,1.0);
      //If no fragment, write nothing
      discard;
    }
  }
}


vec4 resolveClosest(ivec2 coords, int abNumFrag){

  //Search smallest z
  vec4 minFrag=vec4(0.0f, 0.0f, 0.0f, 1000000.0f);
  for(int i=0; i<abNumFrag; i++){
#	if ABUFFER_USE_TEXTURES
    vec4 val=imageLoad(abufferImg, ivec3(coords, i));
#	else
    vec4 val=d_abuffer[coords.x+coords.y*SCREEN_WIDTH + (i*SCREEN_WIDTH*SCREEN_HEIGHT)];
#	endif
    if(val.w<minFrag.w){
      minFrag=val;
    }
  }
  //Output final color for the frame buffer
  return minFrag;
}


void fillFragmentArray(ivec2 coords, int abNumFrag){
  //Load fragments into a local memory array for sorting
  for(int i=0; i<abNumFrag; i++){
#	if ABUFFER_USE_TEXTURES
    fragmentList[i]=imageLoad(abufferImg, ivec3(coords, i));
#	else
    fragmentList[i]=d_abuffer[coords.x+coords.y*SCREEN_WIDTH + (i*SCREEN_WIDTH*SCREEN_HEIGHT)];
#	endif
  }
}

//Bubble sort used to sort fragments
void bubbleSort(int array_size);

//Blend fragments front-to-back
vec4 resolveAlphaBlend(ivec2 coords, int abNumFrag){

  //Copy fragments in local array
  fillFragmentArray(coords, abNumFrag);

  //Sort fragments in local memory array
  bubbleSort(abNumFrag);

  vec4 finalColor=vec4(0.0f);


  const float sigma = 30.0f;
  float thickness=fragmentList[0].w/2.0f;

  finalColor=vec4(0,0,0,0.5);
  for(int i=0; i<abNumFrag; i++){
    vec4 frag=fragmentList[i];

    vec4 col;
    col.rgb=frag.rgb;
    col.w=fragmentAlpha;	//uses constant alpha

#if ABUFFER_RESOLVE_ALPHA_CORRECTION
    if(i%2==abNumFrag%2)
      thickness=(fragmentList[i+1].w-frag.w)*0.5f;
    col.w=1.0f-pow(1.0f-col.w, thickness* sigma );
#endif
    finalColor = mix(col, finalColor, finalColor.a);
  }


  return finalColor;

}

//Blend fragments front-to-back
vec4 resolveGelly(ivec2 coords, int abNumFrag){

  //Copy fragments in local array
  fillFragmentArray(coords, abNumFrag);

  //Sort fragments in local memory array
  bubbleSort(abNumFrag);


  float thickness=0.0f;
  vec4 accumColor=vec4(0.0f);

  vec4 prevFrag;
  for(int i=0; i<abNumFrag; i++){
    vec4 frag=fragmentList[i];

    if(i%2==1){
      thickness+=frag.w-prevFrag.w;
    }

    vec4 col;
    col.rgb=frag.rgb;
    col.w=0.5f;	//uses constant alpha

    col.rgb=col.rgb*col.w;
    accumColor=accumColor+col*(1.0f-accumColor.a);

    prevFrag=frag;
  }
  accumColor=accumColor+backgroundColor*(1.0f-accumColor.a);


  //float thickness=fragmentList[abNumFrag-1].w-fragmentList[0].w;
  float sigma = 20.0f;
  float Ia = exp(-sigma*thickness);
  float ka = 0.8;

  vec4 finalColor=vec4(0.0f);
  finalColor = ka * Ia + (1.0-ka) * fragmentList[0]; //Same as Bavoil 2005
  //finalColor = ka * Ia + (1.0-ka) * accumColor;   //Uses accumulated Color

  const vec4 jade = vec4(.4, .14, .11, 1.0)* 8.0f;
  const vec4 green = vec4(0.3f, 0.7f, 0.1f, 1.0f)* 4.0f;
  finalColor *= jade ;

  return finalColor;
}


//Bubble sort used to sort fragments
void bubbleSort(int array_size) {
  for (int i = (array_size - 2); i >= 0; --i) {
    for (int j = 0; j <= i; ++j) {
      if (fragmentList[j].w > fragmentList[j+1].w) {
        vec4 temp = fragmentList[j+1];
        fragmentList[j+1] = fragmentList[j];
        fragmentList[j] = temp;
      }
    }
  }
}

//Swap function
void swapFragArray(int n0, int n1){
  vec4 temp = fragmentList[n1];
  fragmentList[n1] = fragmentList[n0];
  fragmentList[n0] = temp;
}
