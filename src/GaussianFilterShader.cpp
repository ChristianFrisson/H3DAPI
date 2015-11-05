//////////////////////////////////////////////////////////////////////////////
//    Copyright 2004-2013, SenseGraphics AB
//
//    Any use, or distribution, of this file without permission from the
//    copyright holders is strictly prohibited. Please contact SenseGraphics,
//    www.sensegraphics.com, for more information.
//
//
/// \file GaussianFilterShader.cpp
/// \brief CPP file for GaussianFilterShader.
///
//
//
//////////////////////////////////////////////////////////////////////////////

#include <H3D/GaussianFilterShader.h>
#include <H3D/GeneratedTexture.h>

using namespace H3D;

// Add this node to the H3DNodeDatabase system.
H3DNodeDatabase GaussianFilterShader::database( 
                                               "GaussianFilterShader", 
                                               &newInstance< GaussianFilterShader >,
                                               typeid( GaussianFilterShader ),
                                               &H3DGeneratedFragmentShaderNode::database );

namespace GaussianFilterShaderInternals {
  FIELDDB_ELEMENT( GaussianFilterShader, texture, INPUT_OUTPUT );
  FIELDDB_ELEMENT( GaussianFilterShader, type, INPUT_OUTPUT );
  FIELDDB_ELEMENT( GaussianFilterShader, weights, OUTPUT_ONLY );
  FIELDDB_ELEMENT( GaussianFilterShader, kernelSize, INPUT_OUTPUT );
  FIELDDB_ELEMENT( GaussianFilterShader, sigma, INPUT_OUTPUT);
  FIELDDB_ELEMENT( GaussianFilterShader, pixelStepOffset, INPUT_OUTPUT );
  FIELDDB_ELEMENT( GaussianFilterShader, width, INPUT_OUTPUT );
  FIELDDB_ELEMENT( GaussianFilterShader, height, INPUT_OUTPUT );
  FIELDDB_ELEMENT( GaussianFilterShader, widthInUse, OUTPUT_ONLY);
  FIELDDB_ELEMENT( GaussianFilterShader, heightInUse, OUTPUT_ONLY );
}

GaussianFilterShader::GaussianFilterShader( Inst< DisplayList  > _displayList,
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
                                            Inst< MFWeights       > _weights,
                                            Inst< SFInt32         > _kernelSize,
                                            Inst< SFFloat         > _sigma,
                                            Inst< SFFloat         > _pixelStepOffset,
                                            Inst< SFInt32         > _width,
                                            Inst< SFInt32         > _height,
                                            Inst< TextureMonitor  > _widthInUse,
                                            Inst< TextureMonitor  > _heightInUse) :
ConvolutionFilterShader( _displayList, _metadata, _isSelected, 
                         _isValid, _activate, _language, _parts, 
                         _suppressUniformWarnings, _fragmentShaderString, 
                         _vertexShaderString, _texture, _type, _weights,_kernelSize, 
                         _pixelStepOffset, _width, _height, _widthInUse, _heightInUse ),
  sigma(_sigma) {
  
  type_name = "GaussianFilterShader";
  database.initFields( this );
  sigma->setValue(1.0);
  sigma->route( weights, id );
  kernelSize->route( weights, id );
  type->route( weights, id );
}

std::vector<float> GaussianFilterShader::gauss2D(int _width, int _height, float _sigma) {
  float sigmaSqr = _sigma*_sigma;
  int nr_weights = _width*_height;
  int midX = int(_width /2);
  int midY = int(_height /2);
  float sum = 0;
  std::vector<float> result(nr_weights,0.0);
  for(int  x=0; x<_width;x++) {
    for(int y=0;y<_height;y++) {
      // make use of the symmetry to simplify the calclation
      // result[x*sizeX+y] is the xth row, yth column
      float g = 0;
      if(x>midX&&y>midY)
        g = result[(_width -x-1)*_width +(_height -y-1)];
      else if(x>midX&&y<midY)
        g = result[(_width -x-1)*_width +y];
      else if(x<midX&&y>midY)
        g = result[x*_width +(_height -y-1)];
      else {
        H3DFloat i_x = x - midX;
        H3DFloat i_y = y - midY;
        g = (float) ( 1/(2.0*H3DUtil::Constants::pi*_sigma*_sigma)*H3DUtil::H3DExp(-(i_x*i_x+i_y*i_y)/(2* _sigma*_sigma)) );

      }
     result[x*_width+y] = g;
      sum += g;
    }
  }

  for(int i = 0; i<nr_weights;i++) {
    result[i] = result[i]/sum;
  }
  return result;
}

std::vector<float> GaussianFilterShader::gauss1D(int _width, float _sigma) {
  float sum = 0;
  int mid= (_width -1)/2;
  std::vector<float> result(_width,0.0);
  for(int i=0;i<_width;i++ ) {
    int current_index = i-mid;
    float g = 0;
    g= 1/(H3DUtil::H3DSqrt(2*H3DUtil::Constants::pi)*_sigma) * H3DUtil::H3DExp(-current_index*current_index/(2.0f*_sigma*_sigma));
    result.at(i) = g;
    sum+=g;
  }
  for(int i = 0; i<_width;i++) {
    result[i] = result[i]/sum;
  }

  return result;
}

bool GaussianFilterShader::canBuildShader(){
  // if kernel size is even or less than zero or sigma is less than zero, 
  // no need to build shader.
  return sigma->getValue()>0&&ConvolutionFilterShader::canBuildShader();
}

void GaussianFilterShader::MFWeights::update() { 
  GaussianFilterShader *gfs =
    static_cast< GaussianFilterShader * >( getOwner());
 
  int demand_kernel_size = gfs->kernelSize->getValue();
  float demand_sigma = gfs->sigma->getValue();
  if( demand_sigma<0 || demand_kernel_size<0 || demand_kernel_size%2 == 0 ) {
    // no need to update kernel
    Console(LogLevel::Warning)<<"Invalid sigma or kernel size assigned, will not update kernel."<<endl;
    return;
  }

  const string &_type = gfs->type->getValue();

  if ( _type == "FULL") {
    value = gfs->gauss2D(demand_kernel_size ,demand_kernel_size ,demand_sigma);
  } else if( _type == "VERTICAL"|| _type == "HORIZONTAL") {
    value = gfs->gauss1D(demand_kernel_size,demand_sigma);
  }
}
