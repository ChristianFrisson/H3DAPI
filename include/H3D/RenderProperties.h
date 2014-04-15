//////////////////////////////////////////////////////////////////////////////
//    Copyright 2004-2014, SenseGraphics AB
//
//    This file is part of H3D API.
//
//    H3D API is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    H3D API is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with H3D API; if not, write to the Free Software
//    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//    A commercial license is also available. Please contact us at 
//    www.sensegraphics.com for more information.
//
//
/// \file RenderProperties.h
/// \brief Header file for RenderProperties, X3D scene-graph node
///
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __RENDERPROPERTIES_H__
#define __RENDERPROPERTIES_H__

#include <H3D/X3DAppearanceChildNode.h>
#include <H3D/SFColor.h>
#include <H3D/SFInt32.h>
#include <H3D/SFColorRGBA.h>
#include <H3D/SFString.h>
#include <H3D/SFFloat.h>

namespace H3D {

  /// \ingroup H3DNodes
  /// \class RenderProperties
  /// \brief The RenderProperties node specifies additional rendering options
  /// that cannot be specified with the X3D Appearance node. 
  ///
  /// The depthTestEnabled field specifies if depth test should be enabled
  /// or not. If depth test is disabled geometries will be drawn over the 
  /// current values in the frame buffer regardless of if it is in front of 
  /// or behind the previous objects drawn.
  ///
  /// The smoothShading field specifies if smooth shading should be used.
  /// If false, flat shading is used.
  ///
  /// The depthBufferWriteEnabled field specifies if depth values should be written
  /// to the depth buffer or not during rendering.
  /// 
  /// The colorBufferRedWriteEnabled field specifies if red color values should be written
  /// to the color buffer or not during rendering.
  ///
  /// The colorBufferGreenWriteEnabled field specifies if green color values should be written
  /// to the color buffer or not during rendering.
  ///
  /// The colorBufferBlueWriteEnabled field specifies if blue color values should be written
  /// to the color buffer or not during rendering.
  /// 
  /// The colorBufferAlphaWriteEnabled field specifies if alpha values should be written
  /// to the color buffer or not during rendering.
  ///
  /// <b>Examples:</b>
  ///   - <a href="../../../H3DAPI/examples/All/RenderProperties.x3d">RenderProperties.x3d</a>
  ///     ( <a href="examples/RenderProperties.x3d.html">Source</a> )
  ///   - <a href="../../../H3DAPI/examples/All/RenderProperties1.x3d">RenderProperties1.x3d</a>
  ///     ( <a href="examples/RenderProperties1.x3d.html">Source</a> )
  ///   - <a href="../../../H3DAPI/examples/All/RenderProperties2.x3d">RenderProperties2.x3d</a>
  ///     ( <a href="examples/RenderProperties2.x3d.html">Source</a> )
  /// 
  /// \par Internal routes:
  /// \dotfile RenderProperties.dot
  class H3DAPI_API RenderProperties : public X3DAppearanceChildNode {
  public:
    
    /// Constructor.
    RenderProperties( Inst< SFNode      >  _metadata   = 0 ,
                      Inst< DisplayList > _displayList      = 0,
                      Inst< SFBool      > _depthTestEnabled = 0,
                      Inst< SFString    > _depthFunc = 0,
                      Inst< SFBool      > _smoothShading    = 0,
                      Inst< SFBool      > _multiPassTransparency = 0,
                      Inst< SFBool      > _depthBufferWriteEnabled = 0,
                      Inst< SFBool      > _colorBufferRedWriteEnabled = 0,
                      Inst< SFBool      > _colorBufferGreenWriteEnabled = 0,
                      Inst< SFBool      > _colorBufferBlueWriteEnabled = 0,
                      Inst< SFBool      > _colorBufferAlphaWriteEnabled = 0,
                      Inst< SFString    > _alphaFunc = 0,
                      Inst< SFFloat     > _alphaFuncValue = 0,
                      Inst< SFString    > _blendFuncSrcFactorRGB = 0,
                      Inst< SFString    > _blendFuncSrcFactorAlpha = 0,
                      Inst< SFString    > _blendFuncDstFactorRGB = 0,
                      Inst< SFString    > _blendFuncDstFactorAlpha = 0,
                      Inst< SFString    > _blendEquationRGB = 0,
                      Inst< SFString    > _blendEquationAlpha = 0,
                      Inst< SFColorRGBA > _blendColor = 0,
                      Inst< SFBool      > _blendEnabled = 0
                      );


    /// Returns a bitmask of the OpenGL attrib bits that will be affected
    /// by this node. The value can be used in glPushAttrib in order
    /// to save the current state.
    virtual GLbitfield getAffectedGLAttribs() {
      return( GL_LIGHTING_BIT | GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    }

    /// Set up the line properties with OpenGL.
    virtual void render();

    /// Returns the default xml containerField attribute value.
    /// For this node it is "fillProperties".
    ///
    virtual string defaultXMLContainerField() {
      return "renderProperties";
    }

    /// The depthTestEnabled field specifies if depth test should be enabled
    /// or not. If depth test is disabled geometries will be drawn over the 
    /// current values in the frame buffer regardless of if they are in front of 
    /// or behind the previous objects drawn.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> true \n
    /// 
    /// \dotfile RenderProperties_depthTestEnabled.dot 
    auto_ptr< SFBool >  depthTestEnabled;

    /// The depthFunc field specifies if a alpha testing should be used and in
    /// that case how the function should look. See
    /// <a href="http://www.opengl.org/sdk/docs/man/xhtml/glDepthFunc.xml">opengl glDepthFunc reference</a>
    /// for more information about each value.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> LESS \n
    /// <b>Valid values:</b> NEVER, LESS, EQUAL, LEQUAL, GREATER, NOTEQUAL, \n
    /// GEQUAL, ALWAYS \n
    /// 
    /// \dotfile RenderProperties_depthFunc.dot 
    auto_ptr< SFString > depthFunc;

    /// The smoothShading field specifies if smooth shading of colors
    /// should be used. If true then
    /// colors are interpolated between vertices of a face. If false then
    /// each face has a uniform color chosen from one of its vertices.
    /// For more information see OpenGL reference for the
    /// command glShadeModel.
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> true \n
    /// 
    /// \dotfile RenderProperties_smoothShading.dot 
    auto_ptr< SFBool > smoothShading;

    /// If the multiPassTransparency field is true, and we have a transparent
    /// shape, the entire scene will be rendered three times, once with all
    /// non-transparant objects drawn, once with all transparant objects drawn
    /// with front face culled and once with all transparent objects with back
    /// face culled. This will avoid many artifact of transparency caused
    /// by rendering objects in the wrong order, however it requires three
    /// traversals of the scenegraph. If set to false, transparent objects
    /// will just be drawn in the order they appear in the scene graph.
    /// 
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> true \n
    /// 
    /// \dotfile RenderProperties_multiPassTransparency.dot 
    auto_ptr< SFBool > multiPassTransparency;

    /// The depthBufferWriteEnabled field specifies if depth values should be written
    /// to the depth buffer of not during rendering.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> true \n
    /// 
    /// \dotfile RenderProperties_depthBufferWriteEnabled.dot 
    auto_ptr< SFBool >  depthBufferWriteEnabled;

    /// The colorBufferRedWriteEnabled field specifies if red color values should be written
    /// to the color buffer of not during rendering.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> true \n
    /// 
    /// \dotfile RenderProperties_colorBufferRedWriteEnabled.dot 
    auto_ptr< SFBool >  colorBufferRedWriteEnabled;

    /// The colorBufferGreenWriteEnabled field specifies if green color values should be written
    /// to the color buffer of not during rendering.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> true \n
    /// 
    /// \dotfile RenderProperties_colorBufferGreenWriteEnabled.dot 
    auto_ptr< SFBool >  colorBufferGreenWriteEnabled;

    /// The colorBufferBlueWriteEnabled field specifies if blue color values should be written
    /// to the color buffer of not during rendering.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> true \n
    /// 
    /// \dotfile RenderProperties_colorBufferBlueWriteEnabled.dot 
    auto_ptr< SFBool >  colorBufferBlueWriteEnabled;

    /// The colorBufferAlphaWriteEnabled field specifies if alpha values should be written
    /// to the color buffer of not during rendering.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> true \n
    /// 
    /// \dotfile RenderProperties_colorBufferAlphaWriteEnabled.dot 
    auto_ptr< SFBool >  colorBufferAlphaWriteEnabled;

    /// The alphaFunc field specifies if a alpha testing should be used and in
    /// that case how the function should look. See
    /// <a href="http://www.opengl.org/sdk/docs/man2/xhtml/glAlphaFunc.xml">opengl glAlphaFunc reference</a>
    /// for more information about each value.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> ALWAYS \n
    /// <b>Valid values:</b> NEVER, LESS, EQUAL, LEQUAL, GREATER, NOTEQUAL, \n
    /// GEQUAL, ALWAYS \n
    /// 
    /// \dotfile RenderProperties_alphaFunc.dot 
    auto_ptr< SFString > alphaFunc;

    /// The alphaFuncValue field specifies the reference value when alpha
    /// testing is enabled. Only used if alphaFunc != ALWAYS.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 0 \n
    /// <b>Valid values:</b> [0,1] \n
    /// 
    /// \dotfile RenderProperties_alphaFuncValue.dot 
    auto_ptr< SFFloat  > alphaFuncValue;

    /// The blendFuncSrcFactorRGB field specifies how the red, blue and green
    /// channels are computed for source when using a blend function. See
    /// <a href="http://www.opengl.org/sdk/docs/man/xhtml/glBlendFuncSeparate.xml">opengl glBlendFuncSeparate reference</a>
    /// for more information about each value.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> SRC_ALPHA \n
    /// <b>Valid values:</b> ZERO, ONE, SRC_COLOR, ONE_MINUS_SRC_COLOR, \n
    /// DST_COLOR, ONE_MINUS_DST_COLOR, SRC_ALPHA, ONE_MINUS_SRC_ALPHA, \n
    /// DST_ALPHA, ONE_MINUS_DST_ALPHA, CONSTANT_COLOR, \n
    /// ONE_MINUS_CONSTANT_COLOR, CONSTANT_ALPHA, ONE_MINUS_CONSTANT_ALPHA, \n
    /// SRC_ALPHA_SATURATE \n
    /// 
    /// \dotfile RenderProperties_blendFuncSrcFactorRGB.dot 
    auto_ptr< SFString > blendFuncSrcFactorRGB;

    /// The blendFuncDstFactorRGB field specifies how the red, blue and green
    /// channels are computed for destination when using a blend function. See
    /// <a href="http://www.opengl.org/sdk/docs/man/xhtml/glBlendFuncSeparate.xml">opengl glBlendFuncSeparate reference</a>
    /// for more information about each value.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> ONE_MINUS_SRC_ALPHA \n
    /// <b>Valid values:</b> ZERO, ONE, SRC_COLOR, ONE_MINUS_SRC_COLOR, \n
    /// DST_COLOR, ONE_MINUS_DST_COLOR, SRC_ALPHA, ONE_MINUS_SRC_ALPHA, \n
    /// DST_ALPHA, ONE_MINUS_DST_ALPHA, CONSTANT_COLOR, \n
    /// ONE_MINUS_CONSTANT_COLOR, CONSTANT_ALPHA, ONE_MINUS_CONSTANT_ALPHA \n
    /// 
    /// \dotfile RenderProperties_blendFuncDstFactorRGB.dot 
    auto_ptr< SFString > blendFuncDstFactorRGB;

    /// The blendFuncSrcFactorAlpha field specifies how the alpha
    /// channel is computed for source when using a blend function. See
    /// <a href="http://www.opengl.org/sdk/docs/man/xhtml/glBlendFuncSeparate.xml">opengl glBlendFuncSeparate reference</a>
    /// for more information about each value.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> SRC_ALPHA \n
    /// <b>Valid values:</b> ZERO, ONE, SRC_COLOR, ONE_MINUS_SRC_COLOR, \n
    /// DST_COLOR, ONE_MINUS_DST_COLOR, SRC_ALPHA, ONE_MINUS_SRC_ALPHA, \n
    /// DST_ALPHA, ONE_MINUS_DST_ALPHA, CONSTANT_COLOR, \n
    /// ONE_MINUS_CONSTANT_COLOR, CONSTANT_ALPHA, ONE_MINUS_CONSTANT_ALPHA, \n
    /// SRC_ALPHA_SATURATE \n
    /// 
    /// \dotfile RenderProperties_blendFuncSrcFactorAlpha.dot 
    auto_ptr< SFString > blendFuncSrcFactorAlpha;

    /// The blendFuncDstFactorAlpha field specifies how the alpha
    /// channel is computed for destination when using a blend function. See
    /// <a href="http://www.opengl.org/sdk/docs/man/xhtml/glBlendFuncSeparate.xml">opengl glBlendFuncSeparate reference</a>
    /// for more information about each value.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> ONE_MINUS_SRC_ALPHA \n
    /// <b>Valid values:</b> ZERO, ONE, SRC_COLOR, ONE_MINUS_SRC_COLOR, \n
    /// DST_COLOR, ONE_MINUS_DST_COLOR, SRC_ALPHA, ONE_MINUS_SRC_ALPHA, \n
    /// DST_ALPHA, ONE_MINUS_DST_ALPHA, CONSTANT_COLOR, \n
    /// ONE_MINUS_CONSTANT_COLOR, CONSTANT_ALPHA, ONE_MINUS_CONSTANT_ALPHA \n
    /// 
    /// \dotfile RenderProperties_blendFuncDstFactorAlpha.dot 
    auto_ptr< SFString > blendFuncDstFactorAlpha;

    /// The blendEquationRGB field specifies the blend equation used to
    /// combine the red, green and blue channel. See
    /// <a href="http://www.opengl.org/sdk/docs/man/xhtml/glBlendEquationSeparate.xml">opengl glBlendEquationSeparate reference</a>
    /// for more information about each value.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> ADD \n
    /// <b>Valid values:</b> ADD, SUBTRACT, REVERSE_SUBTRACT, MIN, MAX \n
    /// 
    /// \dotfile RenderProperties_blendEquationRGB.dot 
    auto_ptr< SFString > blendEquationRGB;

    /// The blendEquationAlpha field specifies the blend equation used to
    /// combine the alpha channel. See
    /// <a href="http://www.opengl.org/sdk/docs/man/xhtml/glBlendEquationSeparate.xml">opengl glBlendEquationSeparate reference</a>
    /// for more information about each value.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> ADD \n
    /// <b>Valid values:</b> ADD, SUBTRACT, REVERSE_SUBTRACT, MIN, MAX \n
    /// 
    /// \dotfile RenderProperties_blendEquationAlpha.dot 
    auto_ptr< SFString > blendEquationAlpha;

    /// The blendColor field specifies the blend color. See
    /// <a href="http://www.opengl.org/sdk/docs/man/xhtml/glBlendColor.xml">opengl glBlendColor reference</a>
    /// for more information.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> 0, 0, 0, 1 \n
    /// 
    /// \dotfile RenderProperties_blendColor.dot 
    auto_ptr< SFColorRGBA > blendColor;
    
    /// The blendEnabled field specifies if blending should be enabled or not.
    ///
    /// <b>Access type:</b> inputOutput \n
    /// <b>Default value:</b> TRUE \n
    /// 
    /// \dotfile RenderProperties_blendEnabled.dot 
    auto_ptr< SFBool > blendEnabled;

    /// The H3DNodeDatabase for this node.
    static H3DNodeDatabase database;
  protected:
    int getSFactor( const string &sfactor_string, GLenum &sfactor );
    int getDFactor( const string &dfactor_string, GLenum &dfactor );
    int getBlendEquation( const string &equation_string, GLenum &equation );
    int getAlphaFunc( const string &alpha_func_string, GLenum &alpha_func );
    int getDepthFunc( const string &depth_func_string, GLenum &depth_func );
  };
}

#endif
