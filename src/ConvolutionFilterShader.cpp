//////////////////////////////////////////////////////////////////////////////
//    Copyright 2004-2014, SenseGraphics AB
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
  FIELDDB_ELEMENT( ConvolutionFilterShader, pixelStepOffset, INPUT_OUTPUT );
  FIELDDB_ELEMENT( ConvolutionFilterShader, width, INPUT_OUTPUT );
  FIELDDB_ELEMENT( ConvolutionFilterShader, height, INPUT_OUTPUT );
  FIELDDB_ELEMENT( ConvolutionFilterShader, widthInUse, OUTPUT_ONLY );
  FIELDDB_ELEMENT( ConvolutionFilterShader, heightInUse, OUTPUT_ONLY );
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
                            Inst< SFInt32         > _kernelSize,
                            Inst< SFFloat         > _pixelStepOffset,
                            Inst< SFInt32  > _width,
                            Inst< SFInt32  > _height,
                            Inst< TextureMonitor         > _widthInUse,
                            Inst< TextureMonitor         > _heightInUse):
  H3DGeneratedFragmentShaderNode( _displayList, _metadata, _isSelected, 
                                  _isValid, _activate, _language, _parts, 
                                  _suppressUniformWarnings, _fragmentShaderString, 
                                  _vertexShaderString ),
  texture( _texture ),
  type( _type ),
  weights( _weights ),
  kernelSize( _kernelSize ),
  pixelStepOffset( _pixelStepOffset ),
  height(_height),
  width(_width),
  heightInUse(_heightInUse),
  widthInUse(_widthInUse){
  
  type_name = "ConvolutionFilterShader";
  database.initFields( this );

  type->addValidValue( "HORIZONTAL" );
  type->addValidValue( "VERTICAL" );
  type->addValidValue( "FULL" );
  type->setValue( "FULL" );

  kernelSize->setValue( 1 );
  weights->push_back( 1 );
  pixelStepOffset->setValue( 0.0 );

  width->setValue(-1);
  height->setValue(-1);
  widthInUse->setValue(0,id);
  heightInUse->setValue(0,id);

  pixelStepOffset->route( rebuildShader );
  kernelSize->route( rebuildShader );
  weights->route( rebuildShader );
  type->route( rebuildShader );
}

void ConvolutionFilterShader::traverseSG( TraverseInfo &ti ) {
  H3DGeneratedFragmentShaderNode::traverseSG( ti );
  //TODO: better to have a general way to handle the texture width and height
  X3DTexture2DNode *t = dynamic_cast<X3DTexture2DNode*> (texture->getValue());
  RenderTargetTexture *rtt = dynamic_cast< RenderTargetTexture* >( texture->getValue() );
  Image *image = NULL;
  if( t ){
  image = t->image->getValue();

  }
  if( image ) {
    if( width->getValue()==-1 ) {
      // use image width
      widthInUse->setValue(image->width(),id);
    }else{
      // use specified width
      widthInUse->setValue(width->getValue(),id);
    }
    if( height->getValue()==-1 ) {
      // use image height
      heightInUse->setValue(image->height(),id);
    }else{
      heightInUse->setValue(height->getValue(),id);
    }
  } else {
    GeneratedTexture *gen_tex = dynamic_cast< GeneratedTexture * >( t );
    if( gen_tex &&
        gen_tex->textureIdIsInitialized()) {
      GLuint tex_id = gen_tex->getTextureId();
      glPushAttrib( GL_TEXTURE_BIT );
      glBindTexture( gen_tex->getTextureTarget(), tex_id );
      GLint h,w;
      glGetTexLevelParameteriv(gen_tex->getTextureTarget(), 0, GL_TEXTURE_WIDTH, &w);
      glGetTexLevelParameteriv(gen_tex->getTextureTarget(), 0, GL_TEXTURE_HEIGHT, &h);
      glPopAttrib();
      if( width->getValue()==-1 ) {
        widthInUse->setValue(w,id);
      }else{
        widthInUse->setValue(width->getValue(),id);
      }
      if( height->getValue()==-1 ) {
        heightInUse->setValue(h,id);
      }else{
        heightInUse->setValue(height->getValue(),id);
      }
    } else {
      widthInUse->setValue(0,id);
      heightInUse->setValue(0,id);
    }
  }
  if( rtt ) {// if texture is a renderTargetTexture
    GLuint tex_id = rtt->getTextureId();
    glPushAttrib( GL_TEXTURE_BIT );
    glBindTexture( rtt->getTextureTarget(), tex_id );
    GLint h,w;
    glGetTexLevelParameteriv(rtt->getTextureTarget(), 0, GL_TEXTURE_WIDTH, &w);
    glGetTexLevelParameteriv(rtt->getTextureTarget(), 0, GL_TEXTURE_HEIGHT, &h);
    glPopAttrib();
    if( width->getValue()==-1 ) {
      widthInUse->setValue(w,id);
    }else{
      widthInUse->setValue(widthInUse->getValue(),id);
    }
    if( heightInUse->getValue()==-1 ) {
      heightInUse->setValue(h,id);
    }else{
      heightInUse->setValue(heightInUse->getValue(),id);
    }
  }
}

bool ConvolutionFilterShader::canBuildShader() {
  H3DInt32 kernel_size = kernelSize->getValue();
  // have texture and kernel size is an odd number larger than 0.
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

  return s.str();
}

string ConvolutionFilterShader::getFragmentShaderString() {
  if( canBuildShader() ) {
    stringstream s;
    int kernel_size = kernelSize->getValue();
    s << "  const int KERNEL_SIZE = " << kernel_size << "; " << endl;
    s << "  const int texture_width =  " << widthInUse->getValueAsString() <<"; "<<endl;
    s << "  const int texture_height = " << heightInUse->getValueAsString() << "; "<<endl;
    s << "  // the step in texture coordinates between each pixel " << endl;
    string pixel_step_offset = pixelStepOffset->getValueAsString();
    if( pixel_step_offset.find(".")==std::string::npos ) {
      pixel_step_offset+= ".0";
    }
    s << "  const vec2 pixel_step = vec2( (1.0+" << pixel_step_offset 
      << ") / float( texture_width ), (1.0+" << pixel_step_offset << ") / float( texture_height ) ); " << endl;
    string weightsInString = "";
    for( unsigned int i = 0; i< weights->size(); ++i ) {
      stringstream ss;
      ss << weights->getValueByIndex(i);
      string currentValue( ss.str() );
      if( currentValue.find(".")==std::string::npos ) {
        currentValue+= ".0";
      }
      weightsInString += currentValue+=",";
    }
    weightsInString.erase( weightsInString.size()-1);
    //weightsInString.pop_back();
    s << "  const float weights["<< weights->size()<<"] = float["<<weights->size()<<"]("<<weightsInString<<");"  <<endl;
    const string &t = type->getValue(); 

    if( t == "VERTICAL" ) {
      s << "  const int min_index_h = 0; " << endl;
      s << "  const int max_index_h = 0; " << endl;
      s << "  const int min_index_v = " <<-(kernel_size - 1)/2 <<";" << endl;
      s << "  const int max_index_v = " << (kernel_size - 1)/2 <<";" << endl;
    } else if( t == "HORIZONTAL" ) {
      s << "  const int min_index_h = " <<-(kernel_size - 1)/2<< ";" << endl;
      s << "  const int max_index_h = " << (kernel_size - 1)/2<< ";" << endl;
      s << "  const int min_index_v = 0; " << endl;
      s << "  const int max_index_v = 0; " << endl;
    } else {
      if( t != "FULL" ) {
      // print error message
      }
      s << "  const int min_index_h = " << -(kernel_size - 1)/2<< ";" << endl;
      s << "  const int max_index_h = " <<  (kernel_size - 1)/2<< ";" << endl;
      s << "  const int min_index_v = " << -(kernel_size - 1)/2<< ";" << endl;
      s << "  const int max_index_v = " <<  (kernel_size - 1)/2<< ";" << endl;
    } 

    s << "  uint index = uint(0); " << endl;
    s << "  vec4 color = vec4( 0.0, 0.0, 0.0, 0.0 ); " << endl;
    s << "  for( int v = min_index_v; v <= max_index_v; v++ ) { " << endl;
    s << "    for( int h = min_index_h; h <= max_index_h; h++ ) { " << endl;
    s << "       vec2 tc = vec2( h,v )*pixel_step + gl_TexCoord[0].st; " << endl;
    s << "       tc = clamp(tc,vec2(0),vec2(1));" << endl;
    s << "       color = texture2D( " << uniqueShaderName("texture") 
      << ", tc  ) * " <<"weights[index] + color; " << endl;
    s << "       index++; " << endl;
    s << "    } " << endl;
    s << "  } " << endl;
    s << "  generated_color = color; " << endl;//texture2D( " << uniqueShaderName("texture") << ", tc ); " << endl;
    return s.str();
  } else {
    return "";
  }
}


