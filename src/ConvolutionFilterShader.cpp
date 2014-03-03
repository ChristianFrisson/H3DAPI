//////////////////////////////////////////////////////////////////////////////
//    Copyright 2004-2013, SenseGraphics AB
//
//    Any use, or distribution, of this file without permission from the
//    copyright holders is strictly prohibited. Please contact SenseGraphics,
//    www.sensegraphics.com, for more information.
//
//
/// \file ConvolutionFilterShader.cpp
/// \brief CPP file for ConvolutionFilterShader.
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/ConvolutionFilterShader.h>
#include <H3D/GeneratedTexture.h>
#include <H3D/RenderTargetTexture.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase ConvolutionFilterShader::database( 
                                   "ConvolutionFilterShader", 
                                   &newInstance< ConvolutionFilterShader >,
                                   typeid( ConvolutionFilterShader ),
                                   &H3DGeneratedFragmentShaderNode::database );

namespace ConvolutionFilterShaderInternals {
  FIELDDB_ELEMENT( ConvolutionFilterShader, texture, INPUT_OUTPUT );
  FIELDDB_ELEMENT( ConvolutionFilterShader, type, INPUT_OUTPUT );
  FIELDDB_ELEMENT( ConvolutionFilterShader, weights, INPUT_OUTPUT );
  FIELDDB_ELEMENT( ConvolutionFilterShader, kernelSize, INPUT_OUTPUT );
}

ConvolutionFilterShader::ConvolutionFilterShader( Inst< DisplayList  > _displayList,
                            Inst< SFNode          > _metadata,
                            Inst< SFBool          > _isSelected ,
                            Inst< SFBool          > _isValid,
                            Inst< SFBool          > _activate,
                            Inst< SFString        > _language,
                            Inst< MFShaderPart    > _parts,
                            Inst< SFBool          > _suppressUniformWarnings,
                            Inst< MFString        > _fragmentShaderString,
                            Inst< MFString        > _vertexShaderString,
                            Inst< SFTexture2DNode > _texture,
                            Inst< SFString        > _type,
                            Inst< MFFloat         > _weights,
                            Inst< SFInt32         > _kernelSize ) :
  H3DGeneratedFragmentShaderNode( _displayList, _metadata, _isSelected, 
                                  _isValid, _activate, _language, _parts, 
                                  _suppressUniformWarnings, _fragmentShaderString, 
                                  _vertexShaderString ),
  texture( _texture ),
  type( _type ),
  weights( _weights ),
  kernelSize( _kernelSize ) {
  
  type_name = "ConvolutionFilterShader";
  database.initFields( this );


  type->addValidValue( "HORIZONTAL" );
  type->addValidValue( "VERTICAL" );
  type->addValidValue( "FULL" );
  type->setValue( "FULL" );

  kernelSize->setValue( 1 );
  weights->push_back( 1 );

  kernelSize->route( rebuildShader );
  type->route( rebuildShader );
  weights->route( rebuildShader );
}

void ConvolutionFilterShader::traverseSG( TraverseInfo &ti ) {
  H3DGeneratedFragmentShaderNode::traverseSG( ti );
  // use textureOffset do not need texture width and height info anymore
}

bool ConvolutionFilterShader::canBuildShader() {
  H3DInt32 kernel_size = kernelSize->getValue();
  // have texture and kernel size is an odd number larger than 0.
  if( type->getValue()=="FULL" ) {
    if( int( std::sqrt(double(weights->size())) )!= kernel_size ) {
      Console(4)<<"Warning(CovolutionFilterShader)["<<getName()
        <<"]: weights size does not match the sqrt of kernel size"<<endl;
    }
  }else{
    if( weights->size()!= kernel_size ) {
      Console(4)<<"Warning(CovolutionFilterShader)["<<getName()
        <<"]: weights size does not match the kernel size"<<endl;
    }
  }
  return kernel_size > 0 && kernel_size % 2 == 1;
}

void ConvolutionFilterShader::buildShader(){
  if( canBuildShader() ) {
    H3DGeneratedShaderNode::buildShader();
  }
}

string ConvolutionFilterShader::addUniformFields( ComposedShader *shader ) {
  // add dynamic fields for uniform variables 
  stringstream s;
  
    s << addUniformToFragmentShader( shader,
                                     uniqueShaderName( "texture" ), 
                                     "sampler2D",
                                     H3D::Field::INPUT_OUTPUT,
                                     copyAndRouteField( texture ) );
    

    /*s << addUniformToFragmentShader( shader,
                                     uniqueShaderName( "weights" ), 
                                     "float",
                                     H3D::Field::INPUT_OUTPUT,
                                     copyAndRouteField( weights ),
				     nr_weights );*/

  return s.str();
}

void ConvolutionFilterShader::getVaryingVariables( vector< VaryingVariable > &varyings ){
  varyings.push_back(VaryingVariable(
    uniqueShaderName("texCoord"),
    "vec2",
    uniqueShaderName( "texCoord" ) + " = gl_TexCoord[0].st;") );
}

string ConvolutionFilterShader::getFragmentShaderString() {
  if( canBuildShader() ) {
    stringstream s;
    s << "  const int KERNEL_SIZE = " << kernelSize->getValue() << "; " << endl;
    //s << "  const int texture_width =  " << textureWidth->getValueAsString() <<"; "<<endl;
    //s << "  const int texture_height = " << textureHeight->getValueAsString() << "; "<<endl;
    //s << "  // the step in texture coordinates between each pixel " << endl;
    //s << "  vec2 pixel_step = vec2( 1.0 / float( texture_width ), 1.0 / float( texture_height ) ); " << endl;
    string weightsInString = "";
    for( int i = 0; i< weights->size(); ++i ) {
      string currentValue = std::to_string((long double) weights->getValueByIndex(i) );
      if( currentValue.find(".")==std::string::npos ) {
        currentValue+= ".0";
      }
      weightsInString += currentValue+=",";
    }
    weightsInString.erase( weightsInString.size()-1);
    //weightsInString.pop_back();
    s << "  float weights["<< weights->size()<<"] = float["<<weights->size()<<"]("<<weightsInString<<");"  <<endl;
    const string &t = type->getValue(); 

    if( t == "VERTICAL" ) {
      s << "  const int min_index_h = 0; " << endl;
      s << "  const int max_index_h = 0; " << endl;
      s << "  const int min_index_v = -(KERNEL_SIZE - 1)/2; " << endl;
      s << "  const int max_index_v = -min_index_v; " << endl;
    } else if( t == "HORIZONTAL" ) {
      s << "  const int min_index_h = -(KERNEL_SIZE - 1)/2; " << endl;
      s << "  const int max_index_h = -min_index_h; " << endl;
      s << "  const int min_index_v = 0; " << endl;
      s << "  const int max_index_v = 0; " << endl;
    } else {
      if( t != "FULL" ) {
	// print error message
      }
      s << "  const int min_index_h = -(KERNEL_SIZE - 1)/2; " << endl;
      s << "  const int max_index_h = -min_index_h; " << endl;
      s << "  const int min_index_v = min_index_h; " << endl;
      s << "  const int max_index_v = max_index_h; " << endl;
    } 

    s << "  int index = 0; " << endl;
    s << "  vec4 color = vec4( 0.0, 0.0, 0.0, 0.0 ); " << endl;
    s << "  for( int v = min_index_v; v <= max_index_v; v++ ) { " << endl;
    s << "    for( int h = min_index_h; h <= max_index_h; h++ ) { " << endl;
    s << "       ivec2 offset = ivec2( h,v ); " << endl;
    //s << "                           v ); " << endl; 
    //s << "       vec2 tc = gl_TexCoord[0].st; " << endl;
	//s << "       tc.x = clamp(tc.x,0.0,1.0); " << endl;
	//s << "       tc.y = clamp(tc.y,0.0,1.0); " << endl;
    s << "       color = color + textureOffset( " << uniqueShaderName("texture") 
      << ", "<<uniqueShaderName("texCoord")<<", offset  ) * " <<"weights[index]; " << endl;
    s << "       index++; " << endl;
    s << "    } " << endl;
    s << "  } " << endl;
    s << "  generated_color = color; " << endl;//texture2D( " << uniqueShaderName("texture") << ", tc ); " << endl;
    return s.str();
  } else {
    return "";
  }
}


