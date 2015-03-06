//////////////////////////////////////////////////////////////////////////////
//    Copyright 2004-2013, SenseGraphics AB
//
//    Any use, or distribution, of this file without permission from the
//    copyright holders is strictly prohibited. Please contact SenseGraphics,
//    www.sensegraphics.com, for more information.
//
//
/// \file GaussianFilterShader.h
/// \brief Header file for GaussianFilterShader.
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __GAUSSIANFILTERSHADER_H__
#define __GAUSSIANFILTERSHADER_H__

#include <H3D/MFFloat.h>
#include <H3D/ConvolutionFilterShader.h>

namespace H3D {
  /// \ingroup H3DNodes
  /// \class GaussianFilterShader
  /// \brief The GaussianFilterShader is a ConvolutionFilterShader node where 
  /// the weights are automatically calculated from a gaussian function.
  ///
  /// The function used is:
  /// 
  /// If type field is "HORIZONTAL" or "VERTICAL"
  /// f(x) = (1/(2.0*pi*sigma*sigma)*exp(-(x*x)/(2*sigma*sigma)) )
  ///
  /// If type field is "FULL"
  /// f(x,y) = (1/(2.0*pi*sigma*sigma)*exp(-(x*x+y*y)/(2*sigma*sigma)) )
  /// 
  /// The sigma field is the sigma value in the functions above and decides how
  /// much blur the result will be. A higher value of sigma will give a higher 
  /// blur.
  ///
  /// The kernelSize field specifies the size convolution kernel.
  /// This referes to how many pixels should be sampled in each direction.
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/GaussionFilterShader.x3d">GaussionFilterShader.x3d</a>
  ///     ( <a href="examples/GaussionFilterShader.x3d.html">Source</a> )
  ///   - <a href="../../../H3DAPI/examples/All/GaussionFilterShader_postprocess.x3d">GaussionFilterShader_postprocess.x3d</a>
  ///     ( <a href="examples/GaussionFilterShader_postprocess.x3d.html">Source</a> )
  ///
  /// \par Internal routes:
  /// \dotfile GaussianFilterShader.dot
  class H3DAPI_API GaussianFilterShader : 
    public ConvolutionFilterShader {
      /// Specialized field that reCalculate the kernel when it updates
  public:
      
    struct MFWeights : 
        public TypedField< MFFloat, void,  AnyNumber< Field > > {
          virtual void update();
        };

    /// Constructor.
    GaussianFilterShader( Inst< DisplayList  > _displayList = 0,
                          Inst< SFNode       > _metadata    = 0,
                          Inst< SFBool       > _isSelected  = 0,
                          Inst< SFBool       > _isValid     = 0,
                          Inst< SFBool       > _activate    = 0,
                          Inst< SFString     > _language    = 0,
                          Inst< MFShaderPart > _parts       = 0,
                          Inst< SFBool       > _suppressUniformWarnings = 0,
                          Inst< MFString     > _fragmentShaderString    = 0,
                          Inst< MFString     > _vertexShaderString      = 0,
                          Inst< SFTexture2DNode > _texture = 0,
                          Inst< SFString     > _type        = 0,
                          Inst< MFWeights      > _weights     = 0,
                          Inst< SFInt32      > _kernelSize  = 0,
                          Inst< SFFloat      > _sigma  = 0,
                          Inst< SFFloat      > _pixelStepOffset = 0,
                          Inst< SFInt32> _width = 0,
                          Inst< SFInt32> _windowHeight = 0,
                          Inst< TextureMonitor> _widthInUse = 0,
                          Inst< TextureMonitor> _heightInUse = 0);

    /// The sigma field is the sigma value in the gaussian function and decides how
    /// much blur the result will be. A higher value of sigma will give a higher 
    /// blur.
    ///
    /// <b>Access type:</b> inputOutput
    /// <b>Default value:</b> 1
    /// \dotfile GaussianFilterShader_sigma.dot
    auto_ptr< SFFloat > sigma;


    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  protected:

    virtual bool canBuildShader();

    /// regenerate the kernel. 
    //virtual void generateKernel();

  private:
    std::vector<float> gauss2D(int x,int y, float _sigma);
    std::vector<float> gauss1D(int x,float _sigma);


  };
}

#endif
