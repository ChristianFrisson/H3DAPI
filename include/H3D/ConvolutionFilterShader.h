//////////////////////////////////////////////////////////////////////////////
//    Copyright 2004-2014, SenseGraphics AB
//
//    Any use, or distribution, of this file without permission from the
//    copyright holders is strictly prohibited. Please contact SenseGraphics,
//    www.sensegraphics.com, for more information.
//
//
/// \file ConvolutionFilterShader.h
/// \brief Header file for ConvolutionFilterShader.
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __CONVOLUTIONFILTERSHADER_H__
#define __CONVOLUTIONFILTERSHADER_H__

#include <H3D/MFFloat.h>
#include <H3D/H3DGeneratedFragmentShaderNode.h>
#include <H3D/FieldTemplates.h>

namespace H3D {

  /// \ingroup H3DNodes
  /// \class ConvolutionFilterShader
  /// \brief The ConvolutionFilterShader node generates a shader that applies 
  /// a convolution filter kernel to the input texture. 
  /// 
  /// This can be used for many image processing effects.
  ///
  /// The texture field contains the texture on which to apply the 
  /// filter kernel.
  ///
  /// Some kernels(e.g. Gaussian) are separable meaning that they can 
  /// be performed as separate vertical and horizontal pass. This would mean an
  /// extra write operation for each pixel (from the  first pass), but the number
  /// of texture reads is reduced from kernelSize * kernelSize to
  /// kernelSize + kernelSize which can be a significant speed improvement.
  /// 
  /// The type field determines if the shader should act as part of a separate pass
  /// or do all in one pass. The valid values are:
  /// 
  /// - "HORIZONTAL"  - horizontal pass. 
  /// - "VERTICAL" - vertical pass.
  /// - "FULL" - both horizontal and vertical in the same pass
  ///
  /// The weight field specifies the values in the convolution kernel.
  /// The number of values must equal kernelSize if type is "HORIZONTAL"
  /// or "VERTICAL" and kernelSize*kernelSize if type is "FULL".
  /// 
  /// The kernelSize field specifies the size convolution kernel.
  /// This referes to how many pixels should be sampled in each direction.
  /// 
  /// E.g. a kernel size of 3 and weights 
  ///
  /// [ 0 0 0
  ///   0 1 0
  ///   0 0 0 ]
  /// 
  /// will just return the original image.
  /// 
  /// A simple blur filter could have a weight of
  ///
  /// [0.0 0.2 0.0
  ///  0.2 0.2 0.2,
  ///  0.0 0.2 0.0 ]
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/ConvolutionFilterShader.x3d">ConvolutionFilterShader.x3d</a>
  ///     ( <a href="examples/ConvolutionFilterShader.x3d.html">Source</a> )
  ///   - <a href="../../../H3DAPI/examples/All/ConvolutionFilterShader_postprocess.x3d">ConvolutionFilterShader_postprocess.x3d</a>
  ///     ( <a href="examples/ConvolutionFilterShader_postprocess.x3d.html">Source</a> )
  ///
  /// \par Internal routes:
  /// \dotfile ConvolutionFilterShader.dot
  class H3DAPI_API ConvolutionFilterShader : public H3DGeneratedFragmentShaderNode {
  public:
  
    /// The SFTexture2DNode field is dependent on the displayList field
    /// of the containing X3DTexture2DNode node.
    typedef DependentSFNode< H3DSingleTextureNode, 
                             FieldRef< H3DDisplayListObject,
                                       H3DDisplayListObject::DisplayList,
                                       &H3DDisplayListObject::displayList >, 
                             true >
    SFTexture2DNode;

    class TextureMonitor: public OnValueChangeSField<SFInt32>
    {
    public:
      virtual void onValueChange(  const int &new_value ){
        ConvolutionFilterShader* owner = dynamic_cast<ConvolutionFilterShader*> (getOwner());
        owner->buildShader();
      }
    };

    /// Constructor.
    ConvolutionFilterShader( Inst< DisplayList  > _displayList = 0,
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
           Inst< MFFloat      > _weights     = 0,
           Inst< SFInt32      > _kernelSize  = 0,
           Inst< SFFloat      > _pixelStepOffset = 0);
    
    /// The texture field contains the texture on which to apply the 
    /// filter kernel.
    ///
    /// <b>Access type:</b> inputOutput
    /// \dotfile ConvolutionFilterShader_texture.dot
    auto_ptr< SFTexture2DNode > texture;

    /// Some kernels(e.g. Gaussian) are separable meaning that they can 
    /// be performed as separate vertical and horizontal pass. This would mean an
    /// extra write operation for each pixel (from the  first pass), but the number
    /// of texture reads is reduced from kernelSize * kernelSize to
    /// kernelSize + kernelSize which can be a significant speed improvement.
    /// 
    /// The type field determines if the shader should act as part of a separate pass
    /// or do all in one pass. The valid values are:
    /// 
    /// - "HORIZONTAL"  - horizontal pass. 
    /// - "VERTICAL" - vertical pass.
    /// - "FULL" - both horizontal and vertical in the same pass
    ///
    /// <b>Access type:</b> inputOutput
    /// <b>Default value:</b> "FULL"
    /// <b>Valid values: "HORIZONTAL", "VERTICAL", "FULL" </b> 
    /// \dotfile ConvolutionFilterShader_type.dot
    auto_ptr< SFString > type;

    /// The weight field specifies the values in the convolution kernel.
    /// The number of values must equal kernelSize if type is "HORIZONTAL"
    /// or "VERTICAL" and kernelSize*kernelSize if type is "FULL".
    /// 
    /// <b>Access type:</b> inputOutput
    /// <b>Default value:</b> [1.0]
    /// \dotfile ConvolutionFilterShader_weights.dot
    auto_ptr< MFFloat  > weights;

    /// The kernelSize field specifies the size convolution kernel.
    ///
    /// This refers to how many pixels should be sampled in each direction.
    /// 
    /// <b>Access type:</b> inputOutput
    /// <b>Default value:</b> 1
    /// \dotfile ConvolutionFilterShader_kernelSize.dot
    auto_ptr< SFInt32  > kernelSize;

    /// The pixelStepOffset is used to add customized offset to the pixel step
    /// using in convolving, the metric of this value is pixel, so please keep 
    /// this value relatively small as too many pixel offset will make result strange.
    /// By using this value, you can enhance or weaken the convolving effect with
    /// slight artifact while no extra texture read
    /// <b>Access type:</b> inputOutput
    /// <b>Default value:</b> 0.0
    /// \dotfile ConvolutionFilterShader_pixelStepOffset.dot
    auto_ptr< SFFloat > pixelStepOffset;

    /// Traverse the scene graph.
    virtual void traverseSG( TraverseInfo &ti );

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  protected:

    /// Once texture width changes, rebuild the shader
    auto_ptr<TextureMonitor> textureWidth;

    /// Once texture height changes, rebuild the shader
    auto_ptr<TextureMonitor> textureHeight;


    /// Returns true if all conditions to be able to use the values to build a
    /// proper shader are fulfilled.
    virtual bool canBuildShader();

    /// Overriding buildShader to only build the shader if canBuildShader is true.
    virtual void buildShader();

    /// Adds uniform fields to the shader. A ComposedShader uses its
    /// dynamic fields to define uniform variables. Hence we need to
    /// add dynamic fields for each field that we want to be accessible
    /// in the shader.
    string addUniformFields( ComposedShader *shader );

    /// Returns the shader code for the fragment shader.
    virtual string getFragmentShaderString();

  };
}

#endif
